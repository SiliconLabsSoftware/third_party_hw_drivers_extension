/***************************************************************************//**
 * @file app_iostream_cli.c
 * @brief source of simple CLI
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sl_string.h"

#include "at_parser_core.h"
#include "at_parser_events.h"
#include "mikroe_bg96.h"
#include "bg96_at_commands.h"
#include "app_iostream_cli.h"
#include "rsi_debug.h"

#define app_printf(...)                DEBUGOUT(__VA_ARGS__)

/*******************************************************************************
 *******************************   MACROS   ************************************
 ******************************************************************************/
#ifndef BUFSIZE
#define BUFSIZE    80
#endif

/* AT+QGPSLOC Acquire Positioning Information
 * Response: "+QGPSLOC: <UTC>,<latitude>,<longitude>,<hdop>,<altitude>,<fix>,
 * <cog>,<spkm>,<spkn>,<date>,<nsat>"
 *
 */
typedef enum {
  gpsloc_element_UTC_time_e = 1,
  gpsloc_element_latitude_e,
  gpsloc_element_longitude_e,
  gpsloc_element_hdop_e,
  gpsloc_element_altitude_e,
  gpsloc_element_fix_e,
  gpsloc_element_cog_e,
  gpsloc_element_spkm_e,
  gpsloc_element_spkn_e,
  gpsloc_element_date_e,
  gpsloc_element_nsat_e,
} bg96_gnssloc_response_elements_t;

/*******************************************************************************
 ***************   STATIC FUNCTION DECLARATIONS   ******************************
 ******************************************************************************/
static void app_parser(uint8_t *buf);
static void sim_status(void);
static void sim_status_handler(void *handler_data);
static void wakeup(void);
static void wakeup_handler(void *handler_data);
static void sleep(void);
static void sleep_handler(void *handler_data);
static void infor(void);
static void infor_handler(void *handler_data);
static void imei(void);
static void imei_handler(void *handler_data);
static void text_mode(void);
static void set_text_mode_handler(void *handler_data);
static void pdu_mode(void);
static void set_pdu_mode_handler(void *handler_data);
static void set_gsm(void);
static void set_gsm_handler(void *handler_data);
static void set_service_domain(void);
static void set_service_domain_handler(void *handler_data);
static void sms_text(void);
static void sms_text_handler(void *handler_data);
static void sms_pdu(void);
static void sms_pdu_handler(void *handler_data);
static void sim_apn(void);
static void sim_apn_handler(void *handler_data);
static void ip(void);
static void ip_handler(void *handler_data);
static void cops(void);
static void cops_handler(void *handler_data);
static void net_config(void);
static void net_config_handler(void *handler_data);
static void net_gprs_status(void);
static void net_gprs_status_handler(void *handler_data);
static void net_lte_status(void);
static void net_lte_status_handler(void *handler_data);
static void act(void);
static void act_handler(void *handler_data);
static void deact(void);
static void deact_handler(void *handler_data);
static void tcp_open(void);
static void tcp_open_handler(void *handler_data);
static void tcp_send(void);
static void tcp_send_handler(void *handler_data);
static void tcp_close(void);
static void tcp_close_handler(void *handler_data);
static void gps_start(void);
static void gps_start_handler(void *handler_data);
static void gps_get_location(void);
static void gps_get_location_handler(void *handler_data);
static void gps_stop(void);
static void gps_stop_handler(void *handler_data);
static sl_status_t bg96_gpsloc_generic_parser(uint8_t *input_string,
                                              uint8_t *parser_buf,
                                              bg96_gnssloc_response_elements_t element);

/*******************************************************************************
 **************************  GLOBAL VAAIBLES   *********************************
 ******************************************************************************/
static uint8_t buffer[BUFSIZE];
static bool at_processing = false;

/**************************************************************************//**
 * @brief
 *   Command and handler definitions for the CLI.
 *
 *****************************************************************************/
static cli_cmd_t cli_cmds[] = {
  { "wakeup", wakeup },
  { "sleep", sleep },
  { "imei", imei },
  { "infor", infor },
  { "simstatus", sim_status },
  { "gsm", set_gsm },
  { "service", set_service_domain },
  { "textmode", text_mode },
  { "pdumode", pdu_mode },
  { "smstext", sms_text },
  { "smspdu", sms_pdu },
  { "netconfig", net_config },
  { "cops", cops },
  { "netgprs", net_gprs_status },
  { "netlte", net_lte_status },
  { "simapn", sim_apn },
  { "act", act },
  { "deact", deact },
  { "ip", ip },
  { "tcpopen", tcp_open },
  { "tcpsend", tcp_send },
  { "tcpclose", tcp_close },
  { "gpsstart", gps_start },
  { "location", gps_get_location },
  { "gpsstop", gps_stop },
};
static uint8_t cli_cmd_size = sizeof(cli_cmds) / sizeof(cli_cmds[0]);

at_scheduler_status_t output_object = {
  SL_STATUS_NOT_INITIALIZED,
  0, ""
};

/**************************************************************************//**
 * @brief
 *   App layer process function.
 *   This function SHALL be called periodically in the main loop.
 *
 *****************************************************************************/
void app_iostream_cli_process_action(void)
{
  char c = 0;
  static uint8_t index = 0;
  static bool start_cli = false;

#if (defined(SLI_SI917))
  c = DEBUGIN();
#else

  // Retrieve characters, print local echo and full line back
  sl_iostream_getchar(sl_iostream_vcom_handle, &c);
#endif

  if (c > 0) {
    if ((c == '\r') || (c == '\n')) {
      app_printf("%c", '\r');
      app_printf("%c", '\n');
      buffer[index] = '\0';
      app_parser(buffer);
      index = 0;
      start_cli = false;
    } else {
      if (index < BUFSIZE - 1) {
        buffer[index] = c;
        index++;
      }

      if (start_cli == false) {
        start_cli = true;
        app_printf("[>>>] ");
      }

      // Local echo
      app_printf("%c", c);
    }
  }
}

/**************************************************************************//**
 * @brief
 *   App lacer incoming command parser function.
 *
 * @param[in] buf
 *    Pointer to the buffer which contains the input command.
 *
 *****************************************************************************/
static void app_parser(uint8_t *buf)
{
  uint8_t i;

  for (i = 0; i < cli_cmd_size; i++) {
    if (!strncmp((const char *) cli_cmds[i].cmd, (const char *) buf,
                 CLI_CMD_LENGTH)) {
      cli_cmds[i].handler();
      return;
    }
  }
  app_printf("Can't recognize command!\r\n");
}

/**************************************************************************//**
 *  Command handler functions
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *    Wake up  function.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void wakeup(void)
{
  at_parser_init_output_object(&output_object);
  bg96_wake_up(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  wakeup_handler, (void *) &output_object);
  app_printf("Waking up device!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Wake up handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void wakeup_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;
  if (l_output->error_code) {
    app_printf("Error while waking up: %d\r\n", l_output->error_code);
  } else {
    app_printf("Device is up!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Sleep function.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void sleep(void)
{
  at_parser_init_output_object(&output_object);
  bg96_sleep(&output_object);
  at_listen_event((uint8_t *) &output_object.status,
                  SL_STATUS_OK,
                  sleep_handler,
                  (void *) &output_object);
  app_printf("Put the device to sleep!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Sleep handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void sleep_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;
  if (l_output->error_code) {
    app_printf("Error while putting to sleep: %d\r\n", l_output->error_code);
  } else {
    app_printf("Device went to sleep!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Get IMEI function.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void imei(void)
{
  at_parser_init_output_object(&output_object);
  bg96_read_imei(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK, imei_handler,
                  (void *) &output_object);
  app_printf("Read IMEI command sent!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Get IMEI handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void imei_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while reading IMEI: %d\r\n", l_output->error_code);
  } else {
    app_printf("IMEI: %s\r\n", l_output->response_data);
  }
}

/***************************************************************************//**
 * @brief
 *    Get BG96 Module information function.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void infor(void)
{
  at_parser_init_output_object(&output_object);
  bg96_read_infor(&output_object);
  at_listen_event((uint8_t *) &output_object.status,
                  SL_STATUS_OK,
                  infor_handler,
                  (void *) &output_object);
  app_printf("Get Quectel BG96 module infor command sent!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Get BG96 Module information handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void infor_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while reading BG96 Infor: %d\r\n", l_output->error_code);
  } else {
    app_printf("%s\r\n", l_output->response_data);
  }
}

/***************************************************************************//**
 * @brief
 *    Select TE Character Set to GSM function. Call this function before sending
 *    sms. Result will be available in the global output_object.
 *
 ******************************************************************************/
static void set_gsm(void)
{
  at_parser_init_output_object(&output_object);
  bg96_set_te_gsm(&output_object);
  at_listen_event((uint8_t *) &output_object.status,
                  SL_STATUS_OK,
                  set_gsm_handler,
                  (void *) &output_object);
  app_printf("Select TE Character Set to GSM command sent!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Select TE Character Set to GSM handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void set_gsm_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while set te character set GSM: %d\r\n",
               l_output->error_code);
  } else {
    app_printf("Set GSM OK\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Configure the service domain of UE function
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void set_service_domain(void)
{
  at_parser_init_output_object(&output_object);
  bg96_config_service_domain(&output_object, service_domain_type_CS_and_PS_e);
  at_listen_event((uint8_t *) &output_object.status,
                  SL_STATUS_OK, set_service_domain_handler,
                  (void *) &output_object);
  app_printf("Set the service domain CS_and_PS command sent!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Configure the service domain of UE handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void set_service_domain_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while configure the service domain of UE: %d\r\n",
               l_output->error_code);
  } else {
    app_printf("Configure the service domain OK\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Set BG96 to Text Mode function. Call this function before sending sms
 *   text.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void text_mode(void)
{
  at_parser_init_output_object(&output_object);
  bg96_set_sms_mode(&output_object, set_sms_mode_text);
  at_listen_event((uint8_t *) &output_object.status,
                  SL_STATUS_OK,
                  set_text_mode_handler,
                  (void *) &output_object);
  app_printf("Set BG96 to text mode command sent!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Set BG96 to Text Mode handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void set_text_mode_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while set SMS text mode %d\r\n", l_output->error_code);
  } else {
    app_printf("Set SMS text mode OK\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Set BG96 to PDU Mode function. Call this function before sending sms pdu.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void pdu_mode(void)
{
  at_parser_init_output_object(&output_object);
  bg96_set_sms_mode(&output_object, set_sms_mode_pdu);
  at_listen_event((uint8_t *) &output_object.status,
                  SL_STATUS_OK,
                  set_pdu_mode_handler,
                  (void *) &output_object);
  app_printf("Set BG96 to pdu mode command sent!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Set BG96 to PDU Mode handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void set_pdu_mode_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while set SMS pdu mode %d\r\n", l_output->error_code);
  } else {
    app_printf("Set SMS pdu mode OK\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Send SMS Text function.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void sms_text(void)
{
  /// TODO: Replace *** with the receiver's phone number
  bg96_sms_text_t sms_text = { "***", \
                               "Hello World - LTE IoT 2 Click Board !!! - (sms text mode)" };

  at_parser_init_output_object(&output_object);
  bg96_send_sms_text(&output_object, &sms_text);

  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  sms_text_handler, (void *) &output_object);
  app_printf("SMS text command sent!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Send SMS Text handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void sms_text_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while send sms text: %d\r\n",
               l_output->error_code);
  } else {
    app_printf("Send sms text mode ok, response data: %s\r\n",
               l_output->response_data);
  }
}

/***************************************************************************//**
 * @brief
 *    Send SMS PDU function.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void sms_pdu(void)
{
  bg96_sms_pdu_t sms_pdu = {
    "***", /// TODO: Replace *** with the service center phone number.
    "***", /// TODO: Replace *** with the receiver's phone number
    "Hello World - LTE IoT 2 Click Board !!! - (sms pdu mode)"
  };

  at_parser_init_output_object(&output_object);
  bg96_send_sms_pdu(&output_object, &sms_pdu);

  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  sms_pdu_handler, (void *) &output_object);
  app_printf("SMS pdu command sent!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Send SMS PDU handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void sms_pdu_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while send sms pdu: %d\r\n",
               l_output->error_code);
  } else {
    app_printf("Send sms text mode ok, response data: %s\r\n",
               l_output->response_data);
  }
}

/***************************************************************************//**
 * @brief
 *    Set SIM APN function.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void sim_apn(void)
{
/// TODO: Replace *** with the Access point name.
  uint8_t sim_apn[30] = "***";

  at_parser_init_output_object(&output_object);
  bg96_set_sim_apn(&output_object, (uint8_t *)sim_apn);

  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  sim_apn_handler, (void *) &output_object);
  app_printf("Set SIM APN command sent!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Set SIM APN handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void sim_apn_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while set sim apn: %d\r\n", l_output->error_code);
  } else {
    app_printf("Set SIM APN ok\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Get IO address function.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void ip(void)
{
  at_parser_init_output_object(&output_object);
  bg96_read_ip_addr(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK, ip_handler,
                  (void *) &output_object);
  app_printf("Read IP command sent!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Get IP address handler function.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently  handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void ip_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while reading IP: %d\r\n%s\r\n", l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("IP address: %s\r\n", l_output->response_data);
  }
}

/***************************************************************************//**
 * @brief
 *    Query SIM status.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void sim_status(void)
{
  at_parser_init_output_object(&output_object);
  bg96_sim_status(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  sim_status_handler, (void *) &output_object);
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Query SIM status handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void sim_status_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("[E]: Query SIM status error: %d\r\n",
               l_output->error_code);
  } else {
    app_printf("SIM card ready to use!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Network configuration.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void net_config(void)
{
  at_parser_init_output_object(&output_object);
  bg96_network_registration(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  net_config_handler, (void *) &output_object);
  app_printf("\r\nNetwork configuring...\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Network configuration handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void net_config_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("[E]: Network configuration error: %d\r\n%s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("Network configuration done!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Query the status of GPRS service
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void net_gprs_status(void)
{
  at_parser_init_output_object(&output_object);
  bg96_query_gprs_service(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  net_gprs_status_handler, (void *) &output_object);
  app_printf("GPRS network querying...\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Query the status of GPRS service handler
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void net_gprs_status_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;
  if (l_output->error_code) {
    app_printf("[E]: Registered GPRS network error: %d\r\n%s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("Registered to GPRS network!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Query the status of LTE service
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void net_lte_status(void)
{
  at_parser_init_output_object(&output_object);
  bg96_query_lte_service(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  net_lte_status_handler, (void *) &output_object);
  app_printf("LTE network querying...\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Query the status of LTE service handler
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void net_lte_status_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;
  if (l_output->error_code) {
    app_printf("[E]: Registered LTE network error: %d\r\n%s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("Registered to LTE network!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Activate a PDP Context.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void act(void)
{
  at_parser_init_output_object(&output_object);
  bg96_activate_pdp_context(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  act_handler, (void *) &output_object);
  app_printf("\r\nActivating a PDP Context...\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Activate a PDP context handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void act_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;
  if (l_output->error_code) {
    app_printf("[E]: Activate a PDP Context error: %d\r\n%s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("Activated a PDP Context successfully!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Deactivate a PDP Context.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void deact(void)
{
  at_parser_init_output_object(&output_object);
  bg96_deactivate_pdp_context(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  deact_handler, (void *) &output_object);
  app_printf("\r\nDeactivating a PDP Context...\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Deactivate a PDP context handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void deact_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;
  if (l_output->error_code) {
    app_printf("[E]: Deactivate a PDP Context error: %d\r\n%s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("Deactivated a PDP Context successfully!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    Get actual operator.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void cops(void)
{
  at_parser_init_output_object(&output_object);
  bg96_get_operator(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK, cops_handler,
                  (void *) &output_object);
  app_printf("Getting operator!\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    Get actual operator handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void cops_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("Error while getting operator: %d\r\n %s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("Actual operator: %s\r\n", l_output->response_data);
  }
}

/***************************************************************************//**
 * @brief
 *    TCP: Open connection.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void tcp_open(void)
{
  bg96_tcp_connection_t connection = {
    0,
    9999,
    "TCP",
    (uint8_t *) "cloudsocket.hologram.io"
  };

  at_parser_init_output_object(&output_object);
  bg96_tcp_open_connection(&connection, &output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  tcp_open_handler, (void *) &output_object);
  app_printf("TCP: Connection opening...\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    TCP: Open connection handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void tcp_open_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("[E]: TCP open connection error: %d\r\n%s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("TCP: Connection opened!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    TCP: Send data on an opened channel.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void tcp_send(void)
{
  bg96_tcp_connection_t connection = {
    0,
    9999,
    "TCP",
    (uint8_t *) "cloudsocket.hologram.io"
  };

/*
 * TODO: Replace DEVICE_KEY with the Hologram device key found here:
 * https://support.hologram.io/hc/en-us/articles/360035212714
 * Please note that this is different than the device ID
 * and must be manually generated.
 */
  uint8_t data_to_send[] =
    "{\"k\":\"DEVICE_KEY\",\"d\":\"Hello Silabs!\",\"t\":\"my_topic\"}";

  at_parser_init_output_object(&output_object);
  bg96_tcp_send_data(&connection, data_to_send, &output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  tcp_send_handler,
                  (void *) &output_object);
  app_printf("TCP: Data sending...\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    TCP: Send data handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void tcp_send_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;
  if (l_output->error_code) {
    app_printf("[E]: TCP send data error: %d\r\n%s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("TCP: Data sent!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    TCP: Close connection.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void tcp_close(void)
{
  bg96_tcp_connection_t connection = {
    0,
    9999,
    "TCP",
    (uint8_t *) "cloudsocket.hologram.io"
  };

  at_parser_init_output_object(&output_object);
  bg96_tcp_close_connection(&connection, &output_object);
  at_listen_event((uint8_t *) &output_object.status,
                  SL_STATUS_OK,
                  tcp_close_handler,
                  (void *) &output_object);
  app_printf("\r\nTCP: Connection closing...\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    TCP: Close connection handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void tcp_close_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("[E]: TCP close connection error: %d\r\n%s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("TCP: Connection closed!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    GPS: Start session.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void gps_start(void)
{
  at_parser_init_output_object(&output_object);
  bg96_gnss_start(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  gps_start_handler, (void *) &output_object);
  app_printf("\r\nGPS: Session starting...\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    GPS: Start session handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void gps_start_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("[E]: GPS start session error: %d\r\n%s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("GPS: Session started!\r\n");
  }
}

/***************************************************************************//**
 * @brief
 *    GPS: Get location.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void gps_get_location(void)
{
  at_parser_init_output_object(&output_object);
  bg96_gnss_get_position(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  gps_get_location_handler, (void *) &output_object);
  app_printf("\r\nGPS: Location getting...\r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    GPS: Get location handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *    +QGPSLOC: <UTC>,<latitude>,<longitude>,<hdop>,<altitude>,<fix>,<cog>,
 *    <spkm>,<spkn>,<date>,<nsat>
 ******************************************************************************/
static void gps_get_location_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("[E]: GPS get location error: %d\r\n %s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("GPS: Location raw data: %s\r\n",
               l_output->response_data);

    uint8_t element_buf[200] = { 0 };
    sl_status_t stt = bg96_gpsloc_generic_parser(l_output->response_data,
                                                 element_buf,
                                                 gpsloc_element_latitude_e);

    if ((sl_strlen((char *) element_buf) > 0) && (SL_STATUS_OK == stt)) {
      /*
       * Latitude ranges between -90 and 90 degrees.
       * Longitude ranges between -180 and 180 degrees.
       *
       * Convert to degree calculation example:
       * Latitude: [N] 2451.71 is 24 degrees north latitude and 51.71
       *   minutes.
       * Convert the points to degrees 51.71 / 60 = 0.86183.
       * The actual Google Map corresponds to 24 + 0.86183 = +24.86183.
       * Longitude: [E] 12100.99 is 121 degrees east long and 0.99 points.
       * Convert the points to degrees 0.99 / 60 = 0.0165.
       * The actual Google Map is 121 + 0.0165 = +121.0165.
       * Combine the converted latitude and longitude data into
       * (+24.86183, +121.0165) and enter the field of Google Map to find the
       * actual corresponding location.
       */

      uint8_t latitude_int[10];
      uint8_t latitude_decimal[10];
      uint8_t longitude_int[10];
      uint8_t longitude_decimal[10];

      // latitude_int = 2 character
      memcpy((void *)latitude_int, (const void *)element_buf, 2);
      // Ignore addition one character = 'N' or 'S'
      memcpy((void *)latitude_decimal,
             (const void *)(element_buf + 2),
             strlen((const char *)element_buf) - 3);
      double latitude = (atof((const char *)latitude_int)
                         + (atof((const char *)(latitude_decimal)) / 60.0));
      app_printf("Latitude:  %.6f\r\n", latitude);
      memset(element_buf, 0, sizeof(element_buf));

      bg96_gpsloc_generic_parser(l_output->response_data,
                                 element_buf,
                                 gpsloc_element_longitude_e);
      // longitude_int = 3 character
      memcpy((void *)longitude_int, (const void *)element_buf, 3);
      // Ignore addition one character = 'E' or 'W'
      memcpy((void *)longitude_decimal,
             (const void *) (element_buf + 3),
             strlen((const char *)element_buf) - 4);
      double longtitude = (atof((const char *)longitude_int)
                           + (atof((const char *)(longitude_decimal))
                              / 60.0));
      app_printf("GPS: Longitude data:  %.6f\r\n", longtitude);
      memset(element_buf, 0, sizeof(element_buf));

      bg96_gpsloc_generic_parser(l_output->response_data,
                                 element_buf,
                                 gpsloc_element_altitude_e);
      app_printf("GPS: Latitude data: %sM\r\n", element_buf);

      bg96_gpsloc_generic_parser(l_output->response_data,
                                 element_buf,
                                 gpsloc_element_UTC_time_e);
      uint8_t hour[3] = { 0 };
      uint8_t minuste[3] = { 0 };
      uint8_t seconds[3] = { 0 };
      memcpy((void *)hour, (const void *)element_buf, 2);
      memcpy((void *)minuste, (const void *)element_buf + 2, 2);
      memcpy((void *)seconds, (const void *)element_buf + 4, 2);

      app_printf("GPS: Time UTC: %sh:%sm:%ss\r\n", hour, minuste, seconds);

      bg96_gpsloc_generic_parser(l_output->response_data,
                                 element_buf,
                                 gpsloc_element_date_e);
      uint8_t day[3] = { 0 };
      uint8_t month[3] = { 0 };
      uint8_t year[3] = { 0 };
      memcpy((void *)day, (const void *)element_buf, 2);
      memcpy((void *)month, (const void *)element_buf + 2, 2);
      memcpy((void *)year, (const void *)element_buf + 4, 2);

      app_printf("GPS: Date: %s/%s/20%s\r\n", day, month, year);
    }
  }
}

sl_status_t bg96_gpsloc_generic_parser(uint8_t *input_string,
                                       uint8_t *parser_buf,
                                       bg96_gnssloc_response_elements_t element)
{
  uint8_t rsp_buf[200];
  uint8_t element_cnt = 0;
  uint8_t *start_rsp;
  uint8_t *element_start;
  uint8_t *element_end;

  start_rsp = (uint8_t *)strstr((const char *) input_string,
                                (const char *)"+QGPSLOC: ");

  if ((start_rsp != 0)) {
    memset(rsp_buf, 0, 200);
    sl_strcpy_s((char *)rsp_buf, 200, (const char *) start_rsp);
    element_start = rsp_buf + strlen((const char *)"+QGPSLOC: ");

    do
    {
      if (element_cnt != 0) {
        element_start =
          (uint8_t *)strstr((const char *) element_start, (const char *)",");
        if (element_start != 0) {
          element_start++;
        }
      }
      element_cnt++;
    } while (element_cnt < element);

    element_end = (uint8_t *)strstr((const char *) element_start,
                                    (const char *)",");

    if ((element_start != 0) && (element_end != 0)) {
      strncpy((char *) parser_buf,
              (const char *)element_start,
              element_end - element_start);
    }
  } else {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    GPS: Stop session.
 *    Result will be available in the global output_object.
 *
 ******************************************************************************/
static void gps_stop(void)
{
  at_parser_init_output_object(&output_object);
  bg96_gnss_stop(&output_object);
  at_listen_event((uint8_t *) &output_object.status, SL_STATUS_OK,
                  gps_stop_handler, (void *) &output_object);
  app_printf("\r\nGPS: Session stopping... \r\n");
  at_processing = true;
  while (at_processing) {
    at_parser_process();
    at_platform_process();
    at_event_process();
  }
}

/***************************************************************************//**
 * @brief
 *    GPS: Stop session handler.
 *
 * @param[in] handler_data
 *    Data sent by the event handler.
 *    Currently handler_data is a pointer to an at_scheduler_status_t.
 *
 ******************************************************************************/
static void gps_stop_handler(void *handler_data)
{
  at_processing = false;
  at_scheduler_status_t *l_output = (at_scheduler_status_t *) handler_data;

  if (l_output->error_code) {
    app_printf("[E]: GPS stop session error: %d\r\n%s\r\n",
               l_output->error_code,
               l_output->response_data);
  } else {
    app_printf("GPS: Session closed!\r\n");
  }
}
