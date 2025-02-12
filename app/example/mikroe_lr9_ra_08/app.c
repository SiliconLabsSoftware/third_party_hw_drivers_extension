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
#include "mikroe_ra_08.h"
#include "sl_sleeptimer.h"

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

// Comment the line below to switch application mode to receiver
#define DEMO_APP_TRANSMITTER

// Default RF configuration
#define LR9_RF_CFG_DEFAULT_TX          "433000000,1,1,1,22,1"
#define LR9_RF_CFG_DEFAULT_RX          "433000000,1,1,1,1"
#define LR9_DEMO_STRING                "Silabs"
// Receiver string sequences
#define PROCESS_START_RECEIVE          "Recv:"
#define PROCESS_ATDTRX                 "AT"
// Response timeout
#define RESPONSE_TIMEOUT               100000
// Application buffer size
#define APP_BUFFER_SIZE                500
#define PROCESS_BUFFER_SIZE            100
#define APP_TIMER_INTERVAL             2000

static sl_sleeptimer_timer_handle_t app_timer;
static volatile bool timer_trigger;
static uint8_t app_buf[APP_BUFFER_SIZE] = { 0 };
static uint32_t app_buf_len = 0;
mikroe_uart_handle_t app_uart_instance = NULL;

static void mikroe_ra_08_clear_app_buffer(void);
static void mikroe_ra_08_log_app_buffer(void);

#ifndef DEMO_APP_TRANSMITTER
static void mikroe_ra_08_log_receiver(void);

#endif
static void mikroe_ra_08_log_response(void);
static sl_status_t mikroe_ra_08_process(void);
static void application_task(void);
static void app_timer_callback(sl_sleeptimer_timer_handle_t *timer, void *data);

/***************************************************************************//**
 * Application clear app buffer function.
 ******************************************************************************/
static void mikroe_ra_08_clear_app_buffer(void)
{
  memset(app_buf, 0, app_buf_len);
  app_buf_len = 0;
}

/***************************************************************************//**
 * Application log buffer function.
 ******************************************************************************/
static void mikroe_ra_08_log_app_buffer(void)
{
  for (uint32_t i = 0; i < app_buf_len; i++) {
    app_printf("%c", app_buf[i]);
  }
  app_printf("\n");
}

/***************************************************************************//**
 * Log receiver function.
 ******************************************************************************/
#ifndef DEMO_APP_TRANSMITTER
static void mikroe_ra_08_log_receiver(void)
{
  uint32_t timeout_cnt = 0;
  mikroe_ra_08_clear_app_buffer( );
  mikroe_ra_08_process();
  while (0 == strstr((char *)app_buf, PROCESS_START_RECEIVE))
  {
    mikroe_ra_08_process();
    if (timeout_cnt++ > RESPONSE_TIMEOUT) {
      mikroe_ra_08_clear_app_buffer();
      app_printf("[ERROR]: Time out!\n");
      break;
    }
    sl_sleeptimer_delay_millisecond(1);
  }
  mikroe_ra_08_process();
  if (strstr((char *)app_buf, PROCESS_ATDTRX)) {
    app_printf(" Receive: ");
    for ( uint32_t buf_cnt = 15; buf_cnt < 15 + strlen(LR9_DEMO_STRING);
          buf_cnt++ )
    {
      app_printf("%c", app_buf[buf_cnt]);
    }
    app_printf("\r\n");
  }
}

#endif

/***************************************************************************//**
 * Process response funtion.
 ******************************************************************************/
static void mikroe_ra_08_log_response(void)
{
  if (SL_STATUS_OK == mikroe_ra_08_process()) {
    mikroe_ra_08_log_app_buffer();
    mikroe_ra_08_clear_app_buffer();
  }
}

static sl_status_t mikroe_ra_08_process(void)
{
  uint8_t rx_buf[PROCESS_BUFFER_SIZE] = { 0 };
  int32_t overflow_bytes = 0;
  int32_t rx_cnt = 0;
  int32_t rx_size;
  sl_status_t sc = mikroe_ra_08_generic_read(rx_buf,
                                             PROCESS_BUFFER_SIZE,
                                             &rx_size);
  if ((sc == SL_STATUS_OK) && (rx_size <= APP_BUFFER_SIZE)) {
    if ((app_buf_len + rx_size) > APP_BUFFER_SIZE) {
      overflow_bytes = (app_buf_len + rx_size) - APP_BUFFER_SIZE;
      app_buf_len = APP_BUFFER_SIZE - rx_size;
      memmove(app_buf, &app_buf[overflow_bytes], app_buf_len);
      memset(&app_buf[app_buf_len], 0, overflow_bytes);
    }
    for ( rx_cnt = 0; rx_cnt < rx_size; rx_cnt++ )
    {
      if (rx_buf[rx_cnt]) {
        app_buf[app_buf_len++] = rx_buf[rx_cnt];
      }
    }
    return SL_STATUS_OK;
  }
  return SL_STATUS_FAIL;
}

/***************************************************************************//**
 * App timer callback function.
 ******************************************************************************/
static void app_timer_callback(sl_sleeptimer_timer_handle_t *timer, void *data)
{
  (void) timer;
  (void) data;

  timer_trigger = true;
}

/***************************************************************************//**
 * Application task function.
 ******************************************************************************/
static void application_task(void)
{
#ifdef DEMO_APP_TRANSMITTER
  mikroe_ra_08_send_data_frame(MIKROE_RA_08_DTRX_CONFIG_DATA,
                               MIKROE_RA_08_NB_TRIALS_2,
                               (uint8_t *)LR9_DEMO_STRING);
  mikroe_ra_08_log_response();
#endif
}

/***************************************************************************//**
 * Application init function.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
  app_uart_instance = &uart_instance;
#else
  app_uart_instance = sl_iostream_uart_mikroe_handle;
  app_log_iostream_set(sl_iostream_vcom_handle);
#endif

  app_printf("Silicon Labs - LoRa 9 Click example.\n");

  sc = mikroe_ra_08_init(app_uart_instance);
  if (SL_STATUS_OK != sc) {
    app_printf("LoRa 9 Click initialized failed!\n");
    return;
  }
  mikroe_ra_08_hw_reset();
  sl_sleeptimer_delay_millisecond(500);

  mikroe_ra_08_inquire_command((uint8_t *)MIKROE_RA_08_CMD_CTXADDRSET,
                               MIKROE_RA_08_EQUAL_ENABLE);
  mikroe_ra_08_log_response();
  sl_sleeptimer_delay_millisecond(500);
  mikroe_ra_08_inquire_command((uint8_t *)MIKROE_RA_08_CMD_CADDRSET,
                               MIKROE_RA_08_EQUAL_ENABLE);
  mikroe_ra_08_log_response();
  sl_sleeptimer_delay_millisecond(500);
#ifdef DEMO_APP_TRANSMITTER
  app_printf("Transmitter Role\n");
  mikroe_ra_08_write_command((uint8_t *)MIKROE_RA_08_CMD_CTX,
                             (uint8_t *)LR9_RF_CFG_DEFAULT_TX);
  mikroe_ra_08_log_response();
  sl_sleeptimer_delay_millisecond(500);
#else
  app_printf("Receiver Role\n");
  mikroe_ra_08_write_command((uint8_t *)MIKROE_RA_08_CMD_CRXS,
                             (uint8_t *)LR9_RF_CFG_DEFAULT_RX);
  mikroe_ra_08_log_response();
  sl_sleeptimer_delay_millisecond(500);
#endif
  mikroe_ra_08_log_response();
  sl_sleeptimer_delay_millisecond(500);

  sl_sleeptimer_start_periodic_timer_ms(&app_timer,
                                        APP_TIMER_INTERVAL,
                                        app_timer_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (timer_trigger) {
    application_task();
    timer_trigger = false;
  }
#ifndef DEMO_APP_TRANSMITTER
  mikroe_ra_08_log_receiver();
#endif
}
