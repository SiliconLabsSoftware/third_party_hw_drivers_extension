/***************************************************************************//**
 * @file app.c
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
#include "sl_sleeptimer.h"
#include "app_assert.h"
#include "mikroe_em3080w.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_si91x_usart.h"

#define app_printf(...)            DEBUGOUT(__VA_ARGS__)
#define USART_INSTANCE_USED        UART_1

static usart_peripheral_t uart_instance = USART_INSTANCE_USED;
#else /* None Si91x device */
#include "app_log.h"
#include "sl_iostream_init_usart_instances.h"
#include "sl_iostream_init_eusart_instances.h"

#define app_printf(...)            app_log(__VA_ARGS__)
#endif

#define PROCESS_COUNTER            500
#define PROCESS_RX_BUFFER_SIZE     600
#define PROCESS_PARSER_BUFFER_SIZE 600

typedef enum {
  SCAN_ENABLE,
  SCAN_DISABLE,
  SCAN_WAIT
}SCAN_MODE;

mikroe_barcode2_t barcode2;
static uint32_t last_tick = 0;
static SCAN_MODE scan_mode = SCAN_ENABLE;
static char current_parser_buf[PROCESS_PARSER_BUFFER_SIZE];
static mikroe_uart_handle_t app_uart_instance = NULL;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#if (defined(SLI_SI917))
  app_uart_instance = &uart_instance;
#else
  app_uart_instance = sl_iostream_uart_mikroe_handle;
  sl_iostream_set_default(sl_iostream_vcom_handle);
  app_log_iostream_set(sl_iostream_vcom_handle);
#endif

  app_printf("Hello world - Mikroe Barcode2 Example...\r\n");
  sl_status_t sc = mikroe_barcode2_init(&barcode2, app_uart_instance, 0);
  app_assert_status(sc);

  sc = mikroe_barcode2_enable_reset(&barcode2, MIKROE_BARCODE2_ENABLE);
  app_assert_status(sc);

  sl_sleeptimer_delay_millisecond(100);
  sc = mikroe_barcode2_enable_reset(&barcode2, MIKROE_BARCODE2_DISABLE);
  app_assert_status(sc);
  app_printf("mikroe barcode2 init successful\r\n");
}

static void barcode2_process(void)
{
  int32_t rsp_size;
  uint16_t rsp_cnt = 0;

  char uart_rx_buffer[PROCESS_RX_BUFFER_SIZE] = { 0 };
  uint16_t check_buf_cnt;
  uint16_t process_cnt = PROCESS_COUNTER;

  // Clear parser buffer
  memset(current_parser_buf, 0, PROCESS_PARSER_BUFFER_SIZE);

  while (process_cnt != 0) {
    rsp_size = mikroe_barcode2_generic_read(&barcode2,
                                            uart_rx_buffer,
                                            PROCESS_RX_BUFFER_SIZE);

    if (rsp_size > 0) {
      // Validation of the received data
      for ( check_buf_cnt = 0; check_buf_cnt < rsp_size; check_buf_cnt++ ) {
        if (uart_rx_buffer[check_buf_cnt] == 0) {
          uart_rx_buffer[check_buf_cnt] = 13;
        }
      }

      // Storages data in parser buffer
      rsp_cnt += rsp_size;
      if (rsp_cnt < PROCESS_PARSER_BUFFER_SIZE) {
        strncat(current_parser_buf, uart_rx_buffer, rsp_size);
      }

      process_cnt = 3;

      // Clear RX buffer
      memset(uart_rx_buffer, 0, PROCESS_RX_BUFFER_SIZE);
    } else {
      process_cnt--;

      // Process delay
      sl_sleeptimer_delay_millisecond(100);
    }
  }
  if (rsp_cnt > 0) {
    if (rsp_cnt > 80) {
      app_printf("QR Code:\r\n%s\r\n", current_parser_buf);
    } else {
      app_printf("Barcode: \r\n%s\r\n", current_parser_buf);
    }

    app_printf("------------------------\r\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  switch (scan_mode) {
    case SCAN_ENABLE:
      app_printf("Scanning enabled\r\n");
      app_printf("------------------------\r\n");
      mikroe_barcode2_enable_scaning(&barcode2, MIKROE_BARCODE2_ENABLE);
      barcode2_process();
      scan_mode = SCAN_DISABLE;
      break;

    case SCAN_DISABLE:
      mikroe_barcode2_enable_scaning(&barcode2, MIKROE_BARCODE2_DISABLE);
      app_printf("Scanning disabled\r\n");
      app_printf("------------------------\r\n");
      scan_mode = SCAN_WAIT;
      last_tick = sl_sleeptimer_get_tick_count();
      break;

    default:
    case SCAN_WAIT:
      if (sl_sleeptimer_tick_to_ms(sl_sleeptimer_get_tick_count() - last_tick)
          > 1000) {
        scan_mode = SCAN_ENABLE;
      }
      break;
  }
}
