/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include <string.h>

#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "app_assert.h"
#include "mikroe_a172mrq.h"

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

#define PROCESS_COUTER                 100
#define READING_INTERVAL_MSEC          5000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;
static uint8_t flag;
mikroe_uart_handle_t app_uart_instance = NULL;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);
static void app_a172mrq_process(void);
static void app_a172mrq_reg_one(uint8_t fngr_number);
static void app_a172mrq_compare(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t ret_code;

#if (defined(SLI_SI917))
  app_uart_instance = &uart_instance;
#else
  app_uart_instance = sl_iostream_uart_mikroe_handle;

  sl_iostream_set_default(sl_iostream_vcom_handle);
  app_log_iostream_set(sl_iostream_vcom_handle);
#endif

  // Initialize Fingerprint 2 Click
  mikroe_a172mrq_cfg_setup();
  ret_code = mikroe_a172mrq_init(app_uart_instance);
  app_assert_status(ret_code);

  // Reset Fingerprint 2 Click
  mikroe_a172mrq_reset();
  sl_sleeptimer_delay_millisecond(1000);
  app_printf("Fingerprint 2 Click is initialized.\r\n");

  // Write fingerprint data at position 0
  app_a172mrq_reg_one(0);
  sl_sleeptimer_delay_millisecond(1000);

  ret_code = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                                   READING_INTERVAL_MSEC,
                                                   app_timer_cb,
                                                   (void *) NULL,
                                                   0,
                                                   0);
  app_assert_status(ret_code);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;

  app_a172mrq_compare();
}

static void app_a172mrq_process(void)
{
  int32_t rsp_size;
  sl_status_t ret_code;
  int32_t check_buf_cnt;
  uint8_t process_cnt = PROCESS_COUTER;
  char uart_rx_buffer[MIKROE_FP2_RX_BUFFER_SIZE] = { 0 };
  flag = 0;

  while (process_cnt != 0)
  {
    ret_code = mikroe_a172mrq_generic_read(uart_rx_buffer,
                                           MIKROE_FP2_RX_BUFFER_SIZE,
                                           &rsp_size);
    if (SL_STATUS_OK == ret_code) {
      if (rsp_size > 0) {
        // Validation of received data
        for (check_buf_cnt = 0; check_buf_cnt < rsp_size; check_buf_cnt++)
        {
          if (uart_rx_buffer[check_buf_cnt] == 0) {
            uart_rx_buffer[check_buf_cnt] = 13;
          }
        }
        app_printf("%s", uart_rx_buffer);
        if (strstr(uart_rx_buffer, "</R>")) {
          flag = 1;
          process_cnt = 5;
        }
        // Clear RX buffer
        memset(uart_rx_buffer, 0, MIKROE_FP2_RX_BUFFER_SIZE);
      } else {
        process_cnt--;
        // Process delay
        sl_sleeptimer_delay_millisecond(100);
      }
    } else {
      app_printf("Fingerprint processing failed!\r\n");
    }
  }
}

// Register a fingerprint on index
static void app_a172mrq_reg_one(uint8_t fngr_number)
{
  app_printf("Registration is being processed.\r\n");
  sl_sleeptimer_delay_millisecond(500);

  mikroe_a172mrq_reg_one_fp(fngr_number);
  do{
    app_a172mrq_process();
  }
  while (flag == 0);
}

// Compare fingerprint on input with all other fingerprints that are memorized.
static void app_a172mrq_compare(void)
{
  sl_status_t ret_code;

  ret_code = mikroe_a172mrq_generic_write(
    MIKROE_FINGERPRINT2_CMD_FP_CMP, strlen(MIKROE_FINGERPRINT2_CMD_FP_CMP));
  if (SL_STATUS_OK == ret_code) {
    app_printf("Comparison is being processed.\r\n");
    app_a172mrq_process();

    do
    {
      app_a172mrq_process();
    }
    while (flag == 0);
  } else {
    app_printf("Comparison processing failed!\r\n");
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
