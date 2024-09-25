/***************************************************************************//**
 * @file mikroe_bg96.c
 * @brief mikroe_bg96.c
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
#include <sl_string.h>
#include "sl_sleeptimer.h"
#include "at_parser_core.h"
#include "at_parser_utility.h"
#include "mikroe_bg96.h"

#define BITMASK_7BITS                       0x7F
#define BITMASK_8BITS                       0xFF
#define BITMASK_HIGH_4BITS                  0xF0
#define BITMASK_LOW_4BITS                   0x0F
#define TYPE_OF_ADDRESS_INTERNATIONAL_PHONE 0x91
#define SMS_MAX_7BIT_TEXT_LENGTH            160
#define SMS_MAX_PDU_LENGTH                  256
#define SMS_DELIVER_ONE_MESSAGE             0x04
#define SMS_SUBMIT                          0x11

static const char digits[] =
{
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static enum {
  bg96_ready = 0, bg96_wait_for_gpio, bg96_wait_for_device,
} bg96_state;

static bool required_state = false;
static at_scheduler_status_t *global_output;

static digital_out_t pwk_pin;
static digital_in_t sta_pin;

static sl_sleeptimer_timer_handle_t bg96_timer_gpio_handle;
static sl_sleeptimer_timer_handle_t bg96_timer_process_handler;
static void bg96_timer_gpio_callback(sl_sleeptimer_timer_handle_t *handle,
                                     void *data);
static void bg96_timer_process_callback(sl_sleeptimer_timer_handle_t *handle,
                                        void *data);

static uint32_t pdu_encode(uint8_t *service_center_number,
                           uint8_t *phone_number,
                           uint8_t *sms_text,
                           uint8_t *output_buffer,
                           uint16_t buffer_size);

static uint32_t encode_pdu_message(uint8_t *sms_text,
                                   int16_t sms_text_length,
                                   uint8_t *output_buffer,
                                   uint16_t buffer_size);
static uint32_t encode_phone_number (uint8_t *phone_number,
                                     uint8_t *output_buffer,
                                     uint16_t buffer_size);

static void str_cut_chr (uint8_t *str, uint8_t chr);
static void uint8_to_hex(uint8_t input, uint8_t *output);
static void uint8_to_str(uint8_t input, uint8_t *output);

/**************************************************************************//**
 * @brief
 *   BG96 module initialize function.
 *
 *****************************************************************************/
void bg96_init(mikroe_uart_handle_t handle)
{
  at_parser_init(handle);
  digital_out_init(&pwk_pin,
                   hal_gpio_pin_name(BG96_PWK_PORT, BG96_PWK_PIN));
  digital_in_pulldown_init(&sta_pin,
                           hal_gpio_pin_name(BG96_STA_PORT, BG96_STA_PIN));

  sl_sleeptimer_start_periodic_timer_ms(&bg96_timer_process_handler,
                                        200,
                                        bg96_timer_process_callback,
                                        NULL,
                                        0,
                                        0);
}

/**************************************************************************//**
 * @brief
 *   BG96 is alive status getter function.
 *
 *****************************************************************************/
bool bg96_is_alive(void)
{
  return (bool) digital_in_read(&sta_pin);
}

/**************************************************************************//**
 * @brief
 *   BG96 module sleep function.
 *
 * @return
 *    SL_STATUS_OK if sleep initialization was successful.
 *    SL_STATUS_BUSY if sleep/wake up sequence has been already started.
 *    SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG timer error.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_sleep(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t timer_status;

    if (bg96_ready == bg96_state) {
      if (bg96_is_alive()) {
        if (NULL == output_object) {
          return SL_STATUS_ALLOCATION_FAILED;
        }

        global_output = output_object;
        digital_out_high(&pwk_pin);
        timer_status = sl_sleeptimer_restart_timer_ms(&bg96_timer_gpio_handle,
                                                      BG96_GPIO_H_TIME,
                                                      bg96_timer_gpio_callback,
                                                      (void *) NULL,
                                                      0,
                                                      0);

        if (SL_STATUS_OK == timer_status) {
          required_state = false;
          bg96_state = bg96_wait_for_gpio;
          output_object->status = SL_STATUS_BUSY;
          output_object->error_code = 0;
          return SL_STATUS_OK;
        } else {
          return timer_status;
        }
      } else {
        // device is already sleeping
        output_object->status = SL_STATUS_OK;
        output_object->error_code = 0;
      }
      return SL_STATUS_OK;
    } else {
      return SL_STATUS_BUSY;
    }
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *   BG96 module wake up function.
 *
 * @return
 *    SL_STATUS_OK if wake-up initialization was successful.
 *    SL_STATUS_BUSY if sleep/wake up sequence has been already started.
 *    SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG timer error.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_wake_up(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    if (bg96_ready == bg96_state) {
      if (!bg96_is_alive()) {
        global_output = output_object;
        digital_out_high(&pwk_pin);
        sl_status_t timer_status = sl_sleeptimer_restart_timer_ms(
          &bg96_timer_gpio_handle,
          BG96_GPIO_H_TIME,
          bg96_timer_gpio_callback,
          (void *) NULL,
          0,
          0);

        if (SL_STATUS_OK == timer_status) {
          required_state = true;
          bg96_state = bg96_wait_for_gpio;
          output_object->status = SL_STATUS_BUSY;
          output_object->error_code = 0;
          return SL_STATUS_OK;
        } else {
          return timer_status;
        }
      } else {
        // device is already wakeup
        output_object->status = SL_STATUS_OK;
        output_object->error_code = 0;
      }

      return SL_STATUS_OK;
    } else {
      return SL_STATUS_BUSY;
    }
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    Initialization of GNSS function.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_gnss_start(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static const at_cmd_desc_t at_qgps = { "AT+QGPS=1", at_gps_start_stop_cb,
                                           AT_DEFAULT_TIMEOUT };

    validate(cmd_status, at_parser_add_cmd_to_q(&at_qgps));
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    Get the GNSS location string.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_gnss_get_position(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static const at_cmd_desc_t at_gpsloc = { "AT+QGPSLOC?", at_gpsloc_cb,
                                             AT_DEFAULT_TIMEOUT };

    validate(cmd_status, at_parser_add_cmd_to_q(&at_gpsloc));
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return SL_STATUS_OK;
  }

  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    Stop GNSS function.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_gnss_stop(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static const at_cmd_desc_t at_imei = { "AT+QGPSEND", at_gps_start_stop_cb,
                                           AT_DEFAULT_TIMEOUT };

    validate(cmd_status, at_parser_add_cmd_to_q(&at_imei));
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return SL_STATUS_OK;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT initialization.
 *
 *****************************************************************************/
void bg96_nb_init(mikroe_uart_handle_t handle)
{
  bg96_init(handle);
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT open a TCP or UDP connection.
 *
 * @param[in] connection
 *    Pointer to the connection descriptor structure.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL or connection ==
 *   NULL.
 *****************************************************************************/
sl_status_t bg96_nb_open_connection(bg96_nb_connection_t *connection,
                                    at_scheduler_status_t *output_object)
{
  if ((NULL != output_object) && (NULL != connection)) {
    sl_status_t cmd_status = SL_STATUS_OK;
    // format example:
    //   "AT+QIOPEN=1,0,\"TCP\",\"cloudsocket.hologram.io\",9999,0,1"
    uint8_t conn_string[50];
    uint8_t base_cmd[] = "AT+QIOPEN=1,";
    static at_cmd_desc_t at_open = { "", at_open_cb, AT_OPEN_TIMEOUT };
    static const at_cmd_desc_t at_qstate = { "AT+QISTATE=0,1", at_qistate_cb,
                                             AT_DEFAULT_TIMEOUT };

    at_parser_clear_cmd(&at_open);
    validate(cmd_status,
             at_parser_extend_cmd(&at_open, base_cmd));
    if (!strncmp((const char *) connection->port_type, "TCP", 15)
        || !strncmp((const char *) connection->port_type, "UDP", 15)) {
      snprintf((char *) conn_string, 50, "%d,\"%s\",\"%s\",%d,0,1",
               (int) connection->socket, (const char *) connection->port_type,
               (const char *) connection->address, (int) connection->port);
      validate(cmd_status, at_parser_extend_cmd(&at_open, conn_string));
      validate(cmd_status, at_parser_add_cmd_to_q(&at_open));
    } else if (!strncmp((const char *) connection->port_type, "TCP LISTENER",
                        15)
               || !strncmp((const char *) connection->port_type,
                           "UDP LISTENER", 15)) {
      snprintf((char *) conn_string, 50, "%d,\"%s\",\"%s\",0,%d,0",
               (int) connection->socket, (const char *) connection->port_type,
               (const char *) connection->address, (int) connection->port);
      validate(cmd_status, at_parser_extend_cmd(&at_open, conn_string));
      validate(cmd_status, at_parser_add_cmd_to_q(&at_open));
      validate(cmd_status, at_parser_add_cmd_to_q(&at_qstate));
    } else {
      return SL_STATUS_FAIL;
    }
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return cmd_status;
  }

  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT network registration.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_network_registration(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static const at_cmd_desc_t at_cmds[] =
    {
      { "AT+CFUN=0",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT },
      {
        "AT+QCFG=\"nbsibscramble\",0",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
      {
        "AT+QCFG=\"nwscanmode\",0,1",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
      {
        "AT+QCFG=\"roamservice\",2,1",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
      {
        "AT+QCFG=\"nwscanseq\",020103,1",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
      {
        "AT+QCFG=\"band\",0,0,80,1",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
      {
        "AT+QCFG=\"iotopmode\",1,1",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
      {
        "AT+QCFG=\"servicedomain\",1,1",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
      {
        "AT+CGDCONT=1,\"IP\",\"hologram\"",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
      { "AT+CFUN=1", at_ok_error_cb,
        AT_DEFAULT_TIMEOUT },
      {
        "AT+COPS?",
        at_cops_cb,
        AT_DEFAULT_TIMEOUT
      },
      {
        "AT+CREG=1;+CGREG=1;+CEREG=1",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
      {
        "AT+COPS=0",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
      {
        "AT+QICSGP=1,1,\"hologram\",\"\",\"\",1",
        at_ok_error_cb,
        AT_DEFAULT_TIMEOUT
      },
    };

    uint8_t at_cmd_size = sizeof(at_cmds) / sizeof(at_cmd_desc_t);
    uint8_t i;

    for (i = 0; i < at_cmd_size; i++) {
      validate(cmd_status, at_parser_add_cmd_to_q(&at_cmds[i]));
    }
    validate(cmd_status, at_parser_start_scheduler(output_object));

    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT close client connection.
 *
 * @param[in] connection
 *    Pointer to the connection descriptor structure.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL or connection ==
 *   NULL.
 *****************************************************************************/
sl_status_t bg96_nb_close_connection(bg96_nb_connection_t *connection,
                                     at_scheduler_status_t *output_object)
{
  if ((NULL != output_object) && (NULL != connection)) {
    sl_status_t cmd_status = SL_STATUS_OK;
    uint8_t conn_string[5];
    uint8_t base_cmd[] = "AT+QICLOSE=";
    static at_cmd_desc_t at_close = { "", at_ok_error_cb, AT_OPEN_TIMEOUT };

    snprintf((char *) conn_string, 5, "%d", (int) connection->socket);

    at_parser_clear_cmd(&at_close);
    validate(cmd_status, at_parser_extend_cmd(&at_close, base_cmd));
    validate(cmd_status, at_parser_extend_cmd(&at_close, conn_string));
    validate(cmd_status, at_parser_add_cmd_to_q(&at_close));
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB send data function.
 *
 * @param[in] connection
 *    Pointer to the connection descriptor structure.
 *
 * @param[in] data
 *    Pointer to the data to send.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL or data == NULL or
 *    connection == NULL.
 *****************************************************************************/
sl_status_t bg96_nb_send_data(bg96_nb_connection_t *connection,
                              uint8_t *data,
                              at_scheduler_status_t *output_object)
{
  if ((NULL != output_object) && (NULL != data) && (NULL != connection)) {
    sl_status_t cmd_status = SL_STATUS_OK;
    uint8_t data_l_string[10];
    uint8_t base_cmd[] = "AT+QISEND=";
    static at_cmd_desc_t at_qisend = { "", at_send_cb, AT_DEFAULT_TIMEOUT };
    static at_cmd_desc_t at_data = { "", at_data_cb, AT_SEND_TIMEOUT };

    at_parser_clear_cmd(&at_qisend);
    at_parser_clear_cmd(&at_data);
    validate(cmd_status, at_parser_extend_cmd(&at_qisend, base_cmd));
    validate(cmd_status, at_parser_extend_cmd(&at_data, data));

    size_t data_length = sl_strlen((char *) at_data.cms_string);
    if (data_length < DATA_MAX_LENGTH) {
      snprintf((char *) data_l_string, 10, "%d,%d", (int) connection->socket,
               (int) data_length);
      validate(cmd_status, at_parser_extend_cmd(&at_qisend, data_l_string));
      validate(cmd_status, at_parser_add_cmd_to_q(&at_qisend));
      validate(cmd_status, at_parser_add_cmd_to_q(&at_data));
      validate(cmd_status, at_parser_start_scheduler(output_object));
      return cmd_status;
    }
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT receive data function.
 *
 * @param[in] connection
 *    Pointer to the connection descriptor structure.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_nb_receive_data(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static const at_cmd_desc_t at_qird = { "AT+QIRD=11,100", at_recv_cb,
                                           AT_DEFAULT_TIMEOUT };

    validate(cmd_status, at_parser_add_cmd_to_q(&at_qird));
    validate(cmd_status, at_parser_start_scheduler(output_object));

    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT read actual IP address function.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_read_ip(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static const at_cmd_desc_t at_ip = { "AT+QIACT?", at_ip_cb,
                                         AT_DEFAULT_TIMEOUT };

    validate(cmd_status, at_parser_add_cmd_to_q(&at_ip));
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT get actual operator function.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_get_operator(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static const at_cmd_desc_t at_cops =
    { "AT+COPS?", at_cops_cb, AT_DEFAULT_TIMEOUT };

    validate(cmd_status, at_parser_add_cmd_to_q(&at_cops));
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT read HW IMEI number.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_read_imei(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static const at_cmd_desc_t at_imei = { "AT+GSN", at_imei_cb,
                                           AT_DEFAULT_TIMEOUT };

    validate(cmd_status, at_parser_add_cmd_to_q(&at_imei));
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *   BG96 module get information function.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_read_infor(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static const at_cmd_desc_t at_infor = { "ATI", at_infor_cb,
                                            AT_DEFAULT_TIMEOUT };

    validate(cmd_status, at_parser_add_cmd_to_q(&at_infor));
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT set TE character set as GSM.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data..
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_set_te_gsm(at_scheduler_status_t *output_object)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static const at_cmd_desc_t at_te_gsm = { "AT+CSCS=\"GSM\"", at_te_gsm_cb,
                                             AT_DEFAULT_TIMEOUT };

    validate(cmd_status, at_parser_add_cmd_to_q(&at_te_gsm));
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT Configure the service domain of UE.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @param[in] type
 *    Variable to set type of the service domain
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_config_service_domain(at_scheduler_status_t *output_object,
                                       config_service_domain_type_t type)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static at_cmd_desc_t at_config_service_domain = {
      "", at_service_domain_cb, AT_DEFAULT_TIMEOUT
    };

    at_parser_clear_cmd(&at_config_service_domain);

    if (type == service_domain_type_PSOnly_e) {
      sl_strcpy_s((char *)at_config_service_domain.cms_string,
                  CMD_MAX_SIZE,
                  (const char * )"AT+QCFG=\"servicedomain\",1,1");
    } else if (type == service_domain_type_CS_and_PS_e) {
      sl_strcpy_s((char *)at_config_service_domain.cms_string,
                  CMD_MAX_SIZE,
                  (const char * )"AT+QCFG=\"servicedomain\",2,1");
    }

    validate(cmd_status, at_parser_add_cmd_to_q(&at_config_service_domain));
    validate(cmd_status, at_parser_start_scheduler(output_object));

    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT set SMS mode
 *
 * @param[in] mode
 *    Variable to set sms mode
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL.
 *****************************************************************************/
sl_status_t bg96_set_sms_mode(at_scheduler_status_t *output_object,
                              set_sms_mode_t mode)
{
  if (NULL != output_object) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static at_cmd_desc_t at_set_sms_mode = {
      "", set_sms_mode_cb, AT_DEFAULT_TIMEOUT
    };

    at_parser_clear_cmd(&at_set_sms_mode);

    if (mode == set_sms_mode_pdu) {
      sl_strcpy_s((char *)at_set_sms_mode.cms_string, CMD_MAX_SIZE,
                  (const char * )"AT+CMGF=0");
    } else if (mode == set_sms_mode_text) {
      sl_strcpy_s((char *)at_set_sms_mode.cms_string, CMD_MAX_SIZE,
                  (const char * )"AT+CMGF=1");
    }

    validate(cmd_status, at_parser_add_cmd_to_q(&at_set_sms_mode));
    validate(cmd_status, at_parser_start_scheduler(output_object));
    return cmd_status;
  }

  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT send SMS Text
 *
 * @param[in] sms_text_object
 *    Pointer to the sms text descriptor structure.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL or sms_text_object
 *    == NULL.
 *****************************************************************************/
sl_status_t bg96_send_sms_text(at_scheduler_status_t *output_object,
                               bg96_sms_text_t *sms_text_object)
{
  if ((NULL != output_object) && (NULL != sms_text_object)) {
    sl_status_t cmd_status = SL_STATUS_OK;
    uint8_t base_cmd[] = "AT+CMGS=";
    static at_cmd_desc_t at_sms_cmd = {
      "", at_sms_send_command_cb, AT_DEFAULT_TIMEOUT
    };
    static at_cmd_desc_t at_sms_data = {
      "", at_sms_send_data_cb, AT_SMS_TEXT_TIMEOUT
    };

    at_parser_clear_cmd(&at_sms_cmd);
    at_parser_clear_cmd(&at_sms_data);

    validate(cmd_status,
             at_parser_extend_cmd(&at_sms_cmd, (uint8_t *)base_cmd));
    validate(cmd_status, at_parser_extend_cmd(&at_sms_cmd, (uint8_t *)"\""));
    validate(cmd_status, at_parser_extend_cmd(&at_sms_cmd,
                                              sms_text_object->phone_number));
    validate(cmd_status, at_parser_extend_cmd(&at_sms_cmd, (uint8_t *)"\""));

    validate(cmd_status, at_parser_extend_cmd(&at_sms_data,
                                              sms_text_object->sms_text_content));
    validate(cmd_status, at_parser_extend_cmd(&at_sms_data, (uint8_t *)"\032"));

    validate(cmd_status, at_parser_add_cmd_to_q(&at_sms_cmd));
    validate(cmd_status, at_parser_add_cmd_to_q(&at_sms_data));

    validate(cmd_status, at_parser_start_scheduler(output_object));

    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT send SMS PDU
 *
 * @param[in] sms_pdu_object
 *    Pointer to the sms pdu descriptor structure.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL or sms_pdu_object
 *    == NULL.
 *****************************************************************************/
sl_status_t bg96_send_sms_pdu(at_scheduler_status_t *output_object,
                              bg96_sms_pdu_t *sms_pdu_object)
{
  if ((NULL != output_object) && (NULL != sms_pdu_object)) {
    sl_status_t cmd_status = SL_STATUS_OK;
    uint8_t smsc[32] = { 0 };
    uint8_t phone_num[32] = { 0 };
    uint8_t byte_buf[4] = { 0 };
    uint8_t pdu_buf[SMS_MAX_PDU_LENGTH] = { 0 };

    uint8_t base_cmd[] = "AT+CMGS=";
    static at_cmd_desc_t at_sms_cmd = { "",
                                        at_sms_send_command_cb,
                                        AT_DEFAULT_TIMEOUT };
    static at_cmd_desc_t at_sms_data = {
      "", at_sms_send_data_cb, AT_SMS_TEXT_TIMEOUT
    };
    at_parser_clear_cmd(&at_sms_cmd);
    at_parser_clear_cmd(&at_sms_data);

    sl_strcpy_s((char *) smsc,
                32,
                (const char *) sms_pdu_object->service_center_number);
    sl_strcpy_s((char *) phone_num,
                32,
                (const char *) sms_pdu_object->phone_number);
    str_cut_chr(smsc, '+');
    str_cut_chr(phone_num, '+');

    int16_t pdu_buf_len = pdu_encode(smsc,
                                     phone_num,
                                     sms_pdu_object->sms_text_content,
                                     pdu_buf,
                                     SMS_MAX_PDU_LENGTH);

    if (pdu_buf_len < 0) {
      return SL_STATUS_FAIL;
    }

    uint8_t length = pdu_buf_len - ((sl_strlen((char *) smsc) - 1) / 2 + 3);
    uint8_to_str(length, byte_buf);
    str_cut_chr(byte_buf, ' ');
    validate(cmd_status,
             at_parser_extend_cmd(&at_sms_cmd, (uint8_t *)base_cmd));
    validate(cmd_status,
             at_parser_extend_cmd(&at_sms_cmd, (uint8_t *)byte_buf));

    for ( int16_t cnt = 0; cnt < pdu_buf_len; cnt++ )
    {
      uint8_to_hex(pdu_buf[cnt], byte_buf);
      validate(cmd_status, at_parser_extend_cmd(&at_sms_data, byte_buf));
    }

    validate(cmd_status, at_parser_extend_cmd(&at_sms_data, (uint8_t *)"\032"));

    validate(cmd_status, at_parser_add_cmd_to_q(&at_sms_cmd));
    validate(cmd_status, at_parser_add_cmd_to_q(&at_sms_data));

    validate(cmd_status, at_parser_start_scheduler(output_object));

    return cmd_status;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT set SIM APN
 *
 * @param[in] sim_apn
 *    Pointer to the SIM APN descriptor .
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *    SL_STATUS_INVALID_PARAMETER if output_object == NULL or sim_apn == NULL
 *****************************************************************************/
sl_status_t bg96_set_sim_apn(at_scheduler_status_t *output_object,
                             uint8_t *sim_apn)
{
  if ((NULL != output_object) && (NULL != sim_apn)) {
    sl_status_t cmd_status = SL_STATUS_OK;
    static at_cmd_desc_t at_set_apn = {
      "AT+CGDCONT=1,\"IP\",\"", at_set_sim_apn_cb, AT_DEFAULT_TIMEOUT
    };

    validate(cmd_status, at_parser_extend_cmd(&at_set_apn, (uint8_t *)sim_apn));
    validate(cmd_status, at_parser_extend_cmd(&at_set_apn, (uint8_t *)"\""));

    validate(cmd_status, at_parser_add_cmd_to_q(&at_set_apn));
    validate(cmd_status, at_parser_start_scheduler(output_object));

    return cmd_status;
  }

  return SL_STATUS_INVALID_PARAMETER;
}

static uint32_t pdu_encode(uint8_t *service_center_number,
                           uint8_t *phone_number,
                           uint8_t *sms_text,
                           uint8_t *output_buffer,
                           uint16_t buffer_size)
{
  int16_t output_buffer_length = 0;

  if ((NULL != service_center_number) && (NULL != phone_number)
      && (NULL != sms_text) && (NULL != output_buffer)) {
    if (buffer_size < 2) {
      return SL_STATUS_FAIL;
    }

    // 1. Set SMS center number.
    int16_t length = 0;
    if ((service_center_number != 0)
        && (sl_strlen((char *) service_center_number) > 0)) {
      // Add type of address.
      output_buffer[1] = TYPE_OF_ADDRESS_INTERNATIONAL_PHONE;
      length = encode_phone_number(service_center_number,
                                   output_buffer + 2,
                                   buffer_size - 2);
      if ((length < 0) && (length >= 254)) {
        return SL_STATUS_FAIL;
      }
      length++;
    }
    output_buffer[0] = length;
    output_buffer_length = length + 1;
    if (output_buffer_length + 4 > buffer_size) {
      return SL_STATUS_FAIL;      // Check if it has space for four more bytes.
    }

    // 2. Set type of message.
    output_buffer[output_buffer_length++] = SMS_SUBMIT;
    output_buffer[output_buffer_length++] = 0x00;      // Message reference.

    // 3. Set phone number.
    output_buffer[output_buffer_length] = sl_strlen((char *) phone_number);
    output_buffer[output_buffer_length
                  + 1] = TYPE_OF_ADDRESS_INTERNATIONAL_PHONE;
    length = encode_phone_number(phone_number,
                                 output_buffer + output_buffer_length + 2,
                                 buffer_size - output_buffer_length - 2);
    output_buffer_length += length + 2;
    if (output_buffer_length + 4 > buffer_size) {
      return SL_STATUS_FAIL;      // Check if it has space for four more bytes.
    }

    // 4. Protocol identifiers.
    output_buffer[output_buffer_length++] = 0x00;      // TP-PID: Protocol
                                                       //   identifier.
    output_buffer[output_buffer_length++] = 0x00;      // TP-DCS: Data coding
                                                       //   scheme.
    output_buffer[output_buffer_length++] = 0xB0;      // TP-VP: Validity: 10
                                                       //   days

    // 5. SMS message.
    int16_t sms_text_length = sl_strlen((char *) sms_text);
    if (sms_text_length > SMS_MAX_7BIT_TEXT_LENGTH) {
      return SL_STATUS_FAIL;
    }
    output_buffer[output_buffer_length++] = sms_text_length;

    length = encode_pdu_message(sms_text,
                                sms_text_length,
                                output_buffer + output_buffer_length,
                                buffer_size - output_buffer_length);
    if (length < 0) {
      return SL_STATUS_FAIL;
    }
    output_buffer_length += length;
  }

  return output_buffer_length;
}

static uint32_t encode_pdu_message(uint8_t *sms_text, int16_t sms_text_length,
                                   uint8_t *output_buffer, uint16_t buffer_size)
{
  int16_t output_buffer_length = 0;

  if ((NULL != sms_text) && (NULL != output_buffer)) {
    // Check if output buffer is big enough.
    if ((sms_text_length * 7 + 7) / 8 > buffer_size) {
      return SL_STATUS_FAIL;
    }

    int16_t carry_on_bits = 1;
    int16_t i = 0;

    for ( ; i < sms_text_length - 1; ++i )
    {
      output_buffer[output_buffer_length++] =
        ((sms_text[i] & BITMASK_7BITS) >> (carry_on_bits - 1))
        | ((sms_text[i + 1] & BITMASK_7BITS) << (8 - carry_on_bits));
      carry_on_bits++;
      if (carry_on_bits == 8) {
        carry_on_bits = 1;
        ++i;
      }
    }

    if (i < sms_text_length) {
      output_buffer[output_buffer_length++] = (sms_text[i] & BITMASK_7BITS) >>
                                              (carry_on_bits - 1);
    }
  }

  return output_buffer_length;
}

static uint32_t encode_phone_number(uint8_t *phone_number,
                                    uint8_t *output_buffer,
                                    uint16_t buffer_size)
{
  int16_t output_buffer_length = 0;

  if ((NULL != phone_number) && (NULL != output_buffer)) {
    int16_t phone_number_length = sl_strlen((char *) phone_number);

    // Check if the output buffer is big enough.
    if ((phone_number_length + 1) / 2 > buffer_size) {
      return SL_STATUS_FAIL;
    }

    int16_t i = 0;
    for ( ; i < phone_number_length; ++i )
    {
      if ((phone_number[i] < '0') && (phone_number[i] > '9')) {
        return SL_STATUS_FAIL;
      }

      if (i % 2 == 0) {
        output_buffer[output_buffer_length++] = BITMASK_HIGH_4BITS
                                                | (phone_number[i] - '0');
      } else {
        output_buffer[output_buffer_length - 1] =
          (output_buffer[output_buffer_length - 1] & BITMASK_LOW_4BITS)
          | ((phone_number[i] - '0') << 4);
      }
    }
  }
  return output_buffer_length;
}

static void str_cut_chr(uint8_t *str, uint8_t chr)
{
  uint16_t cnt_0, cnt_1;
  for ( cnt_0 = 0; cnt_0 < sl_strlen((char *) str); cnt_0++ )
  {
    if (str[cnt_0] == chr) {
      for ( cnt_1 = cnt_0; cnt_1 < sl_strlen((char *) str); cnt_1++ )
      {
        str[cnt_1] = str[cnt_1 + 1];
      }
    }
  }
}

static void uint8_to_hex(uint8_t input, uint8_t *output)
{
  if (NULL != output) {
    output[0] = digits[input >> 4];
    output[1] = digits[input & 0xF];
    output[2] = 0;
  }
}

static void uint8_to_str(uint8_t input, uint8_t *output)
{
  if (NULL != output) {
    uint8_t digit_pos;

    for ( digit_pos = 0; digit_pos < 3; digit_pos++ )
    {
      output[digit_pos] = ' ';
    }

    output[digit_pos--] = 0;

    while (1)
    {
      output[digit_pos] = (input % 10u) + 48;
      input = input / 10u;
      if (digit_pos == 0) {
        break;
      }

      digit_pos--;
    }
  }
}

/**************************************************************************//**
 * @brief
 *   Local callback function for bg96 timer.
 *
 *****************************************************************************/
static void bg96_timer_gpio_callback(sl_sleeptimer_timer_handle_t *handle,
                                     void *data)
{
  (void) data;

  if (handle == &bg96_timer_gpio_handle) {
    switch (bg96_state) {
      case bg96_wait_for_gpio:
        digital_out_low(&pwk_pin);
        bg96_state = bg96_wait_for_device;

        sl_sleeptimer_restart_timer_ms(&bg96_timer_gpio_handle,
                                       BG96_TIMEOUT_MS,
                                       bg96_timer_gpio_callback,
                                       (void *) NULL,
                                       0,
                                       0);
        break;
      case bg96_wait_for_device:
        if (global_output != NULL) {
          global_output->error_code = SL_STATUS_TIMEOUT;
          global_output->status = SL_STATUS_OK;
        }
        bg96_state = bg96_ready;
        break;
      default:
        bg96_state = bg96_ready;
    }
  }
}

static void bg96_timer_process_callback(sl_sleeptimer_timer_handle_t *handle,
                                        void *data)
{
  (void)data;

  if (handle == &bg96_timer_process_handler) {
    switch (bg96_state) {
      case bg96_ready:
        break;
      case bg96_wait_for_gpio:
        break;
      case bg96_wait_for_device:
        if (bg96_is_alive() == required_state) {
          sl_sleeptimer_stop_timer(&bg96_timer_gpio_handle);

          if (global_output != NULL) {
            global_output->error_code = 0;
            global_output->status = SL_STATUS_OK;
          }
          bg96_state = bg96_ready;
        }
        break;
    }
  }
}
