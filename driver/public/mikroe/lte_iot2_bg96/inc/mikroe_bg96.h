/***************************************************************************//**
 * @file mikroe_bg96.h
 * @brief mikroe_bg96.h
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
#ifndef MIKROE_BG96_H_
#define MIKROE_BG96_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "sl_status.h"
#include "drv_digital_in.h"
#include "drv_digital_out.h"
#include "at_parser_core.h"
#include "drv_uart.h"
#include "mikroe_bg96_config.h"

/*******************************************************************************
 ********************************   MACROS   ***********************************
 ******************************************************************************/
#define BG96_GPIO_H_TIME 1000
#define BG96_TIMEOUT_MS  15000
#define DATA_MAX_LENGTH  80u

typedef enum {
  set_sms_mode_pdu = 0,
  set_sms_mode_text,
}set_sms_mode_t;

typedef enum {
  service_domain_type_PSOnly_e = 0,
  service_domain_type_CS_and_PS_e,
}config_service_domain_type_t;

/*******************************************************************************
 *****************************   STRUCTURES   **********************************
 ******************************************************************************/
typedef struct {
  uint8_t socket;
  uint16_t port;
  uint8_t port_type[15];
  uint8_t *address;
} bg96_nb_connection_t;

typedef struct {
  uint8_t phone_number[32];
  uint8_t sms_text_content[256];
}bg96_sms_text_t;

typedef struct {
  uint8_t service_center_number[32];
  uint8_t phone_number[32];
  uint8_t sms_text_content[256];
}bg96_sms_pdu_t;

/**************************************************************************//**
 * @brief
 *   BG96 module initialize function.
 *
 *****************************************************************************/
void bg96_init(mikroe_uart_handle_t handle);

/**************************************************************************//**
 * @brief
 *   BG96 is alive status getter function.
 *
 *****************************************************************************/
bool bg96_is_alive(void);

/**************************************************************************//**
 * @brief
 *   BG96 module sleep function.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if wake-up initialization was successful.
 *    SL_STATUS_BUSY if sleep/wake up sequence has been already started
 *    SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG timer error.
 *    SL_STATUS_INVALID_PARAMETER timer related error.
 *****************************************************************************/
sl_status_t bg96_sleep(at_scheduler_status_t *output_object);

/**************************************************************************//**
 * @brief
 *   BG96 module wake up function.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if wake-up initialization was successful.
 *    SL_STATUS_BUSY if sleep/wake up sequence has been already started.
 *    SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG timer error.
 *    SL_STATUS_INVALID_PARAMETER timer related error.
 *****************************************************************************/
sl_status_t bg96_wake_up(at_scheduler_status_t *output_object);

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
 *****************************************************************************/
sl_status_t bg96_gnss_start(at_scheduler_status_t *output_object);

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
 *****************************************************************************/
sl_status_t bg96_gnss_get_position(at_scheduler_status_t *output_object);

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
 *****************************************************************************/
sl_status_t bg96_gnss_stop(at_scheduler_status_t *output_object);

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT initialization.
 *
 *****************************************************************************/
void bg96_nb_init(mikroe_uart_handle_t handle);

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
 *****************************************************************************/
sl_status_t bg96_nb_open_connection(bg96_nb_connection_t *connection,
                                    at_scheduler_status_t *output_object);

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
 *****************************************************************************/
sl_status_t bg96_network_registration(at_scheduler_status_t *output_object);

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
 *****************************************************************************/
sl_status_t bg96_nb_close_connection(bg96_nb_connection_t *connection,
                                     at_scheduler_status_t *output_object);

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
 *****************************************************************************/
sl_status_t bg96_nb_send_data(bg96_nb_connection_t *connection,
                              uint8_t *data,
                              at_scheduler_status_t *output_object);

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT receive data function.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *****************************************************************************/
sl_status_t bg96_nb_receive_data(at_scheduler_status_t *output_object);

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
 *****************************************************************************/
sl_status_t bg96_read_ip(at_scheduler_status_t *output_object);

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT get actual operator function.
 *
 * @param[in] connection
 *    Pointer to the connection descriptor structure.
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *****************************************************************************/
sl_status_t bg96_get_operator(at_scheduler_status_t *output_object);

/**************************************************************************//**
 * @brief
 *    BG96 NB IoT read HW IMEI number.
 *
 * @param[out] output_object
 *    Pointer to the output object which contains the command status and
 *    output data..
 *
 * @return
 *    SL_STATUS_OK if command successfully added to the command queue.
 *    SL_STATUS_FAIL if scheduler is busy or command queue is full.
 *****************************************************************************/
sl_status_t bg96_read_imei(at_scheduler_status_t *output_object);

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
 *****************************************************************************/
sl_status_t bg96_read_infor(at_scheduler_status_t *output_object);

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
 *****************************************************************************/
sl_status_t bg96_set_te_gsm(at_scheduler_status_t *output_object);

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
 *****************************************************************************/
sl_status_t bg96_config_service_domain(at_scheduler_status_t *output_object,
                                       config_service_domain_type_t type);

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
 *****************************************************************************/
sl_status_t bg96_set_sms_mode(at_scheduler_status_t *output_object,
                              set_sms_mode_t mode);

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
 *****************************************************************************/
sl_status_t bg96_send_sms_text(at_scheduler_status_t *output_object,
                               bg96_sms_text_t *sms_text_object);

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
 *****************************************************************************/
sl_status_t bg96_send_sms_pdu(at_scheduler_status_t *output_object,
                              bg96_sms_pdu_t *sms_pdu_object);

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
 *****************************************************************************/
sl_status_t bg96_set_sim_apn(at_scheduler_status_t *output_object,
                             uint8_t *sim_apn);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_BG96_H_ */
