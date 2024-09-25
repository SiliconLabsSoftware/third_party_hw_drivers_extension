/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#include <string.h>
#include <stdlib.h>
#include "app_queue.h"
#include "sl_sleeptimer.h"
#include "mikroe_lea6s.h"

#if (defined(SLI_SI917))
#include "sl_si91x_usart.h"
#include "rsi_debug.h"
#else
#include "sl_iostream_init_usart_instances.h"
#include "sl_iostream_init_eusart_instances.h"
#include "app_log.h"
#endif

#define PROCESS_RX_BUFFER_SIZE         500
#define PROCESS_PARSER_BUFFER_SIZE     PROCESS_RX_BUFFER_SIZE
#define PROCESS_QUEUE_SIZE             (PROCESS_RX_BUFFER_SIZE * 2)

#if (defined(SLI_SI917))
#define app_printf(...)                DEBUGOUT(__VA_ARGS__)
#define USART_INSTANCE_USED            UART_1
static usart_peripheral_t uart_instance = USART_INSTANCE_USED;
#else
#define app_printf(...)                app_log(__VA_ARGS__)
#endif

static void gps_process (void);
static void parser_application (void);
void timer_callback_fcn(sl_sleeptimer_timer_handle_t *handle, void *data);

static uint8_t uart_rx_buffer[PROCESS_RX_BUFFER_SIZE];
static uint8_t parser_buffer[PROCESS_PARSER_BUFFER_SIZE];

APP_QUEUE(uart_rx_queue, uint8_t, PROCESS_QUEUE_SIZE);
static sl_sleeptimer_timer_handle_t timer_handle;
static bool trigger_gps_process = false;
mikroe_uart_handle_t app_uart_instance = NULL;

void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
  app_uart_instance = &uart_instance;
#else
  app_uart_instance = sl_iostream_uart_mikroe_handle;
  app_log_iostream_set(sl_iostream_vcom_handle);
#endif

  app_printf("Hello World GPS Click !!!\r\n");
  sc = mikroe_lea6s_init(app_uart_instance, true);
  app_printf("mikroe_lea6s_init = 0x%lx\r\n", sc);
  sc = APP_QUEUE_INIT(&uart_rx_queue, uint8_t, PROCESS_QUEUE_SIZE);
  app_printf("app_queue_init = 0x%lx\r\n", sc);
  mikroe_lea6s_wakeup();
  sl_sleeptimer_delay_millisecond(5000);
  app_printf("mikroe_lea6s_wakeup done\r\n");
  sc = sl_sleeptimer_start_periodic_timer_ms(&timer_handle,
                                             800,
                                             timer_callback_fcn,
                                             NULL,
                                             0,
                                             0);
  app_printf("sl_sleeptimer_start_periodic = 0x%lx\r\n", sc);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (trigger_gps_process) {
    trigger_gps_process = false;
    gps_process();
    parser_application();
  }
}

void timer_callback_fcn(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;

  if (handle == &timer_handle) {
    trigger_gps_process = true;
  }
}

static void gps_process(void)
{
  uint16_t actual_size;
  uint16_t index;
  sl_status_t stt = SL_STATUS_OK;

  stt = mikroe_lea6s_generic_read((uint8_t *) &uart_rx_buffer,
                                  PROCESS_RX_BUFFER_SIZE, &actual_size);

  if ((actual_size > 0) && (stt == SL_STATUS_OK)) {
    // Validation of the received data and push data to the queue
    for ( index = 0; index < actual_size; index++ )
    {
      if (uart_rx_buffer[index] == 0) {
        uart_rx_buffer[index] = 13;
      }

      if (false == app_queue_is_full(&uart_rx_queue)) {
        app_queue_add(&uart_rx_queue, (uart_rx_buffer + index));
      }
    }

    // Clear RX buffer
    memset(uart_rx_buffer, 0, PROCESS_RX_BUFFER_SIZE);
  }
}

static void parser_application(void)
{
  uint8_t element_buf[200] = { 0 };
  static uint16_t index = 0;
  mikroe_leas6_parser_result_t stt;
  uint8_t latitude_int[10];
  uint8_t latitude_decimal[10];
  uint8_t longitude_int[10];
  uint8_t longitude_decimal[10];

  while (false == app_queue_is_empty(&uart_rx_queue)) {
    app_queue_remove(&uart_rx_queue, parser_buffer + index);

    if (*(parser_buffer + index) == '\n') {
      stt = mikroe_lea6s_generic_parser(parser_buffer,
                                        gps_command_nema_gpgga_e,
                                        gpgga_element_latitude_e,
                                        element_buf);

      if ((strlen((const char *) element_buf) > 0)
          && (gps_parser_no_error_e == stt)) {
        /*
         * Latitude ranges between -90 and 90 degrees.
         * Longitude ranges between -180 and 180 degrees.
         *
         * Convert to degree calculation example:
         * Latitude: [N] 02451.71 is 24 degrees north latitude and 51.71
         *   minutes.
         * Convert the points to degrees 51.71 / 60 = 0.86183.
         * The actual Google Map corresponds to 24 + 0.86183 = +24.86183.
         * Longitude: [E] 12100.99 is 121 degrees east long and 0.99 points.
         * Convert the points to degrees 0.99 / 60 = 0.0165.
         * The actual Google Map is 121 + 0.0165 = +121.0165.
         * Combine the converted latitude and longitude data into
         * + 24.86183, + 121.0165 and enter the field of Google Map to find the
         * actual corresponding location.
         */
        app_printf("**************************************************\r\n");

        memcpy((void *)latitude_int, (const void *)element_buf, 2);
        memcpy((void *)latitude_decimal,
               (const void *)(element_buf + 2),
               strlen((const char *)element_buf) - 2);
        double latitude = (atof((const char *)latitude_int)
                           + (atof((const char *)(latitude_decimal)) / 60.0));
        app_printf("Latitude:  %.6f\r\n", latitude);
        memset(element_buf, 0, sizeof(element_buf));

        mikroe_lea6s_generic_parser(parser_buffer,
                                    gps_command_nema_gpgga_e,
                                    gpgga_element_longitude_e,
                                    element_buf);

        memcpy((void *)longitude_int, (const void *)element_buf, 3);
        memcpy((void *)longitude_decimal,
               (const void *) (element_buf + 3),
               strlen((const char *)element_buf) - 3);
        double longtitude = (atof((const char *)longitude_int)
                             + (atof((const char *)(longitude_decimal))
                                / 60.0));
        app_printf("Longitude:  %.6f\r\n", longtitude);
        memset(element_buf, 0, sizeof(element_buf));

        mikroe_lea6s_generic_parser(parser_buffer,
                                    gps_command_nema_gpgga_e,
                                    gpgga_element_altitude_e,
                                    element_buf);
        app_printf("Altitude: %s \r\n", element_buf);
      }

      index = 0;
      memset(parser_buffer, 0, PROCESS_PARSER_BUFFER_SIZE);
    } else {
      if (index < (PROCESS_PARSER_BUFFER_SIZE - 1)) {
        index++;
      }
    }
  }
}
