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
#include "sl_sleeptimer.h"
#include "mikroe_lea6s.h"

#if (defined(SLI_SI917))
#include "sl_si91x_usart.h"
#include "rsi_debug.h"

#define app_printf(...)                DEBUGOUT(__VA_ARGS__)
#define USART_INSTANCE_USED            UART_1

static usart_peripheral_t uart_instance = USART_INSTANCE_USED;
#else
#include "sl_iostream_init_usart_instances.h"
#include "sl_iostream_init_eusart_instances.h"
#include "app_log.h"

#define app_printf(...)                app_log(__VA_ARGS__)
#endif

#define PROCESS_RX_BUFFER_SIZE         500

static uint8_t uart_rx_buffer[PROCESS_RX_BUFFER_SIZE];
static mikroe_uart_handle_t app_uart_instance = NULL;

static void parser_application (uint8_t *ptr);

void app_init(void)
{
#if (defined(SLI_SI917))
  app_uart_instance = &uart_instance;
#else
  app_uart_instance = sl_iostream_uart_mikroe_handle;
  app_log_iostream_set(sl_iostream_vcom_handle);
#endif

  app_printf("LEA-6S - GPS Click (Mikroe) Example\r\n");
  mikroe_lea6s_init(app_uart_instance, false);
  mikroe_lea6s_wakeup();
  sl_sleeptimer_delay_millisecond(5000);
  app_printf("Start get the GPS data\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint16_t len;
  static uint16_t index = 0;
  char *ptr = NULL;
  char *ptr2 = NULL;

  if (index == sizeof(uart_rx_buffer) - 1) {
    index = 0;
  }
  sl_status_t stt = mikroe_lea6s_generic_read(&uart_rx_buffer[index], 1, &len);
  if ((len > 0) && (stt == SL_STATUS_OK)) {
    index += len;
    uart_rx_buffer[index] = '\0';
    ptr = strchr((char *)uart_rx_buffer, '$');
    if (ptr) {
      ptr2 = strchr((char *)ptr, '\n');
    }
    if (ptr && ptr2) {
      index = 0;
      parser_application((uint8_t *)ptr);
    }
  }
}

static void parser_application(uint8_t *ptr)
{
  uint8_t element_buf[200] = { 0 };
  mikroe_leas6_parser_result_t stt;
  uint8_t latitude_int[10];
  uint8_t latitude_decimal[10];
  uint8_t longitude_int[10];
  uint8_t longitude_decimal[10];

  stt = mikroe_lea6s_generic_parser(ptr,
                                    gps_command_nema_gpgga_e,
                                    gpgga_element_latitude_e,
                                    element_buf);

  if ((strlen((const char *) element_buf) > 0)
      && (gps_parser_no_error_e == stt)) {
    app_printf("**************************************************\r\n");

    memcpy((void *)latitude_int, (const void *)element_buf, 2);
    memcpy((void *)latitude_decimal,
           (const void *)(element_buf + 2),
           strlen((const char *)element_buf) - 2);
    double latitude = (atof((const char *)latitude_int)
                       + (atof((const char *)(latitude_decimal)) / 60.0));
    app_printf("Latitude:  %.6f\r\n", latitude);
    memset(element_buf, 0, sizeof(element_buf));

    mikroe_lea6s_generic_parser(ptr,
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

    mikroe_lea6s_generic_parser(ptr,
                                gps_command_nema_gpgga_e,
                                gpgga_element_altitude_e,
                                element_buf);
    app_printf("Altitude: %s \r\n", element_buf);
  }
}
