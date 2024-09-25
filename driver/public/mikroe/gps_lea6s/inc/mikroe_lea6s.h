/***************************************************************************//**
 * @file mikroe_lea6s.h
 * @brief mikroe_lea6s.h
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
#ifndef MIKROE_LEA6S_H_
#define MIKROE_LEA6S_H_

#include "sl_status.h"
#include "drv_uart.h"
#include "gps.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup mikroe_lea6s - GPS CLICK
 * @brief
 *  The integration of the Mikroe SDK Click extension mikroe_lea6s driver.
 *  (GPS CLICK board.)
 *
 *    @n @section mikroe_lea6s example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 *      #include "app_log.h"
 *      #include "sl_sleeptimer.h"
 *      #include "mikroe_lea6s.h"
 *      #include "sl_iostream_init_usart_instances.h"
 *      #include <string.h>
 *      #include <stdint.h>
 *      #include "app_queue.h"
 *
 *      #define PROCESS_RX_BUFFER_SIZE     500
 *      #define PROCESS_PARSER_BUFFER_SIZE PROCESS_RX_BUFFER_SIZE
 *      #define PROCESS_QUEUE_SIZE         (PROCESS_RX_BUFFER_SIZE * 2)
 *
 *      static void gps_process (void);
 *      static void parser_application (void);
 *      void timer_callback_fcn(sl_sleeptimer_timer_handle_t *handle,
 *                              void *data);
 *
 *      static uint8_t uart_rx_buffer[PROCESS_RX_BUFFER_SIZE];
 *      static uint8_t parser_buffer[PROCESS_PARSER_BUFFER_SIZE];
 *
 *      APP_QUEUE(uart_rx_queue, uint8_t, PROCESS_QUEUE_SIZE);
 *      static sl_sleeptimer_timer_handle_t timer_handle;
 *      static bool trigger_gps_process = false;
 *
 *      void app_init(void)
 *      {
 *        app_log("Hello World GPS Click !!!\r\n");
 *        app_log("mikroe_lea6s_init = 0x%lx\r\n",
 *                mikroe_lea6s_init(sl_iostream_uart_mikroe_handle));
 *        app_log("app_queue_init = 0x%lx\r\n",
 *                APP_QUEUE_INIT(&uart_rx_queue, uint8_t, PROCESS_QUEUE_SIZE));
 *        mikroe_lea6s_wakeup();
 *        sl_sleeptimer_delay_millisecond(5000);
 *        app_log("mikroe_lea6s_wakeup done\r\n");
 *
 *        app_log("sl_sleeptimer_start_periodic = 0x%lx\r\n",
 *                sl_sleeptimer_start_periodic_timer_ms(&timer_handle,
 *                                                      1000,
 *                                                      timer_callback_fcn,
 *                                                      NULL,
 *                                                      0,
 *                                                      0));
 *      }
 *
 *      ***********************************************************************
 *      * App ticking function.
 *      ***********************************************************************
 *      void app_process_action(void)
 *      {
 *        if (trigger_gps_process) {
 *          trigger_gps_process = false;
 *          gps_process();
 *          parser_application();
 *        }
 *      }
 *
 *      void timer_callback_fcn(sl_sleeptimer_timer_handle_t *handle,
 *                              void *data)
 *      {
 *        (void) data;
 *
 *        if (handle == &timer_handle) {
 *          trigger_gps_process = true;
 *        }
 *      }
 *
 *      static void gps_process(void)
 *      {
 *        uint16_t actual_size;
 *        uint16_t index;
 *        sl_status_t stt = SL_STATUS_OK;
 *
 *        stt = mikroe_lea6s_generic_read((uint8_t *) &uart_rx_buffer,
 *                                        PROCESS_RX_BUFFER_SIZE, &actual_size);
 *
 *        if ((actual_size > 0) && (stt == SL_STATUS_OK)) {
 *          // Validation of the received data and push data to the queue
 *          for ( index = 0; index < actual_size; index++ )
 *          {
 *            if (uart_rx_buffer[index] == 0) {
 *              uart_rx_buffer[index] = 13;
 *            }
 *
 *            if (false == app_queue_is_full(&uart_rx_queue)) {
 *              app_queue_add(&uart_rx_queue, (uart_rx_buffer + index));
 *            }
 *          }
 *
 *          // Clear RX buffer
 *          memset(uart_rx_buffer, 0, PROCESS_RX_BUFFER_SIZE);
 *        }
 *      }
 *
 *      static void parser_application(void)
 *      {
 *        uint8_t element_buf[200] = { 0 };
 *        static uint16_t index = 0;
 *        mikroe_leas6_parser_result_t stt;
 *        uint8_t latitude_int[10];
 *        uint8_t latitude_decimal[10];
 *        uint8_t longitude_int[10];
 *        uint8_t longitude_decimal[10];
 *
 *        while (false == app_queue_is_empty(&uart_rx_queue)) {
 *          app_queue_remove(&uart_rx_queue, parser_buffer + index);
 *
 *          if (*(parser_buffer + index) == '\n') {
 *            stt = mikroe_lea6s_generic_parser(parser_buffer,
 *                                              gps_command_nema_gpgga_e,
 *                                              gpgga_element_latitude_e,
 *                                              element_buf);
 *
 *            if ((strlen((const char *) element_buf) > 0)
 *                && (gps_parser_no_error_e == stt)) {
 *
 *               * Latitude ranges between -90 and 90 degrees.
 *               * Longitude ranges between -180 and 180 degrees.
 *               *
 *               * Convert to degree calculation example:
 *               * Latitude: [N] 02451.71 is 24 degrees north latitude and 51.71
 *               *   minutes.
 *               * Convert the points to degrees 51.71 / 60 = 0.86183.
 *               * The actual Google Map corresponds to 24 + 0.86183 =
 *   +24.86183.
 *               * Longitude: [E] 12100.99 is 121 degrees east long and 0.99
 *   points.
 *               * Convert the points to degrees 0.99 / 60 = 0.0165.
 *               * The actual Google Map is 121 + 0.0165 = +121.0165.
 *               * Combine the converted latitude and longitude data into
 *               * + 24.86183, + 121.0165 and enter the field of Google Map to
 *   find the
 *               * actual corresponding location.
 *
 *
 *
 *            app_log("**************************************************\r\n");
 *
 *              memcpy((void *)latitude_int, (const void *)element_buf, 2);
 *              memcpy((void *)latitude_decimal,
 *                     (const void *)(element_buf + 2),
 *                     strlen((const char *)element_buf) - 2);
 *              double latitude = (atof((const char *)latitude_int)
 *                                 + (atof((const char *)(latitude_decimal)) /
 *   60.0));
 *              app_log("Latitude:  %.6f\r\n", latitude);
 *              memset(element_buf, 0, sizeof(element_buf));
 *
 *              mikroe_lea6s_generic_parser(parser_buffer,
 *                                          gps_command_nema_gpgga_e,
 *                                          gpgga_element_longitude_e,
 *                                          element_buf);
 *
 *              memcpy((void *)longitude_int, (const void *)element_buf, 3);
 *              memcpy((void *)longitude_decimal,
 *                     (const void *) (element_buf + 3),
 *                     strlen((const char *)element_buf) - 3);
 *              double longtitude = (atof((const char *)longitude_int)
 *                                   + (atof((const char *)(longitude_decimal))
 *                                      / 60.0));
 *              app_log("Longitude:  %.6f\r\n", longtitude);
 *              memset(element_buf, 0, sizeof(element_buf));
 *
 *              mikroe_lea6s_generic_parser(parser_buffer,
 *                                          gps_command_nema_gpgga_e,
 *                                          gpgga_element_altitude_e,
 *                                          element_buf);
 *              app_log("Altitude: %s \r\n", element_buf);
 *            }
 *
 *            index = 0;
 *            memset(parser_buffer, 0, PROCESS_PARSER_BUFFER_SIZE);
 *          } else {
 *            if (index < (PROCESS_PARSER_BUFFER_SIZE - 1)) {
 *              index++;
 *            }
 *          }
 *        }
 *      }
 *
 *
 *     @endcode
 *
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *    Typedef for specifying the supported elements of gpgga command.
 ******************************************************************************/
typedef enum {
  gpgga_element_time_e = GPS_GPGGA_TIME,
  gpgga_element_latitude_e = GPS_GPGGA_LATITUDE,
  gpgga_element_latitude_side_e = GPS_GPGGA_LATITUDE_SIDE,
  gpgga_element_longitude_e = GPS_GPGGA_LONGITUDE,
  gpgga_element_longitude_side_e = GPS_GPGGA_LONGITUDE_SIDE,
  gpgga_element_quality_indicator_e = GPS_GPGGA_QUALITY_INDICATOR,
  gpgga_element_number_of_satellites_e = GPS_GPGGA_NUMBER_OF_SATELLITES,
  gpgga_element_h_dilution_of_pos_e = GPS_GPGGA_H_DILUTION_OF_POS,
  gpgga_element_altitude_e = GPS_GPGGA_ALTITUDE,
  gpgga_element_altitude_unit_e =GPS_GPGGA_ALTITUDE_UNIT,
  gpgga_element_geoidal_separation_e = GPS_GPGGA_GEOIDAL_SEPARATION,
  gpgga_element_geoidal_separation_unit_e = GPS_GPGGA_GEOIDAL_SEPARATION_UNIT,
  gpgga_element_time_since_last_dgps_e = GPS_GPGGA_TIME_SINCE_LAST_DGPS,
  gpgga_element_dgps_reference_station_id_e =
    GPS_GPGGA_DGPS_REFERENCE_STATION_ID,
} mikroe_leas6_gpgga_command_elements_t;

/***************************************************************************//**
 * @brief
 *    Typedef for specifying the supported gps response command.
 ******************************************************************************/
typedef enum {
  gps_command_nema_gpgga_e = GPS_NEMA_GPGGA,
  gps_command_nema_gpgll_e = GPS_NEMA_GPGLL,
} mikroe_leas6_gps_command_t;

/***************************************************************************//**
 * @brief
 *    Typedef for specifying the gps response parser result
 ******************************************************************************/
typedef enum {
  gps_parser_no_error_e = GPS_NO_ERROR,
  gps_parser_error_command_or_element_e =  GPS_ERROR_COMMAND_OR_ELEMENT,
  gps_parser_error_start_or_end_command_e = GPS_ERROR_START_OR_END_COMMAND,
  gps_parser_error_next_element_e = GPS_ERROR_NEXT_ELEMENT,
  gps_parser_error_current_element_e = GPS_ERROR_CURRENT_ELEMENT,
  gps_parser_invalid_input_parameter_e = 0x01,
} mikroe_leas6_parser_result_t;

/***************************************************************************//**
 * @brief
 *    Mikroe LEA 6S (GPS CLICK) initialization function.
 *
 * @param[in] uart_handle
 *    IO Stream UART Instance
 *
 * @param[in] blocking
 *    Uart functioning mode
 *
 * @return
 *    SL_STATUS_OK: Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER: if (uart_handle == NULL)
 ******************************************************************************/
sl_status_t mikroe_lea6s_init(mikroe_uart_handle_t uart_handle, bool blocking);

/***************************************************************************//**
 * @brief
 *    Mikroe LEA 6S (GPS CLICK) wakeup function. This function triggers the
 *    reset pin to wakeup GPS module.
 *
 * @param[in]
 *
 * @return
 ******************************************************************************/
void mikroe_lea6s_wakeup(void);

/***************************************************************************//**
 * @brief
 *    Mikroe LEA 6S (GPS CLICK) control reset pin. This function is used to
 *    turn on/off the GPS module.
 *
 * @param[in] state
 *    State of the reset pin: 1 or 0
 *
 * @return
 ******************************************************************************/
void mikroe_lea6s_set_rst_pin(uint8_t state);

/***************************************************************************//**
 * @brief
 *    Mikroe LEA 6S (GPS CLICK) get timepulse pin. This function is used to
 *    get the value of the timepulse pin.
 *
 * @param[in]
 * @return
 *    timepulse value: 1 or 0
 ******************************************************************************/
uint8_t mikroe_lea6s_get_tmpls_pin(void);

/***************************************************************************//**
 * @brief
 *    Mikroe LEA 6S (GPS CLICK) generic write. This function is used to write
 *    a buffer data to the GPS module.
 *
 * @param[in] data_buf
 *    The buffer store data to write.
 * @param[in] len
 *    The buffer data length.
 *
 * @return
 *    SL_STATUS_OK: Successful write data to the GPS module .
 *    SL_STATUS_INVALID_PARAMETER: If (data_buf == NULL)
 ******************************************************************************/
sl_status_t mikroe_lea6s_generic_write(uint8_t *data_buf, uint16_t len);

/***************************************************************************//**
 * @brief
 *    Mikroe LEA 6S (GPS CLICK) generic read. This function is used to read
 *    data, which is received from GPS module.
 *
 * @param[in] data_buf
 *    The buffer store read back data.
 * @param[in] max_len
 *    The maximum length of data to store in the buffer.
 * @param[in] actual_len
 *    The actual length of data read back from GPS module.
 * @return
 *    SL_STATUS_INVALID_PARAMETER: If (data_buf == NULL) or (actual_len == NULL)
 *    SL_STATUS_OK: Successful read data from the GPS module .
 *    SL_STATUS_FAIL: Read data from the GPS module  failed.
 ******************************************************************************/
sl_status_t mikroe_lea6s_generic_read(uint8_t *data_buf,
                                      uint16_t max_len,
                                      uint16_t *actual_len);

/***************************************************************************//**
 * @brief
 *    Mikroe LEA 6S (GPS CLICK) generic parser. This function is used to parser
 *    GPS response data.
 *
 * @param[in] raw_data_buffer
 *    The buffer store read back data from GPS module.
 * @param[in] command
 *    The command to parser. This function supports gps_command_nema_gpgga_e and
 *    gps_command_nema_gpgll_e command.
 * @param[in] element
 *    The element of command to parser.
 * @param[in] element_parser_buffer
 *    The buffer to store element value.
 * @return
 *    gps_parser_no_error_e: If successful parser data.
 *    gps_parser_error_command_or_element_e: If command or element not support.
 *    gps_parser_error_start_or_end_command_e: If error occur at start or end
 *    of command.
 *    gps_parser_error_next_element_e: If error occur at next element.
 *    gps_parser_error_current_element_e:  If error occur at current element.
 *    gps_parser_invalid_input_parameter_e: If (raw_data_buffer == NULL) or
 *    (element_parser_buffer == NULL)
 ******************************************************************************/
mikroe_leas6_parser_result_t
mikroe_lea6s_generic_parser(uint8_t *raw_data_buffer,
                            mikroe_leas6_gps_command_t command,
                            mikroe_leas6_gpgga_command_elements_t element,
                            uint8_t *element_parser_buffer);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_LEA6S_H_
