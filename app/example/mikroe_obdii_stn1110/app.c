/***************************************************************************//**
 * @file
 * @brief Top level application functions
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

#include <string.h>
#include <stdlib.h>
#include "sl_status.h"
#include "app_assert.h"
#include "sl_sleeptimer.h"
#include "mikroe_stn1110.h"

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

#define PROCESS_BUFFER_SIZE            200
#define READING_INTERVAL_MSEC          1000

static int32_t app_buf_len = 0;
static uint8_t app_buf[PROCESS_BUFFER_SIZE] = { 0 };

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;
mikroe_uart_handle_t app_uart_instance = NULL;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * @brief
 *   This function clears memory of application buffer and reset its length.
 ******************************************************************************/
static void app_obdii_clear_buf(void);

/***************************************************************************//**
 * @brief
 *   This function logs data from application buffer.
 ******************************************************************************/
static void app_obdii_log_buf(void);

/***************************************************************************//**
 * @brief
 *   This function reads data from device and concatenates data to
 *   application buffer.
 * @return @retval SL_STATUS_OK Success
 *         @retval SL_STATUS_FAIL  Reading error
 ******************************************************************************/
static sl_status_t app_obdii_process(void);

/***************************************************************************//**
 * @brief
 *   This function checks for response and returns the status of response.
 * @param[in] rsp
 *   Expected response.
 * @return @retval SL_STATUS_OK Success
 *         @retval SL_STATUS_TIMEOUT Time-out
 *         @retval SL_STATUS_FAIL  Checking error
 ******************************************************************************/
static sl_status_t app_obdii_rsp_check(uint8_t *rsp);

static void application_task(void);

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

  ret_code = mikroe_stn1110_init(app_uart_instance);
  app_assert_status(ret_code);
  app_printf("Mikroe OBDii Click is initialized.\r\n");

  mikroe_stn1110_reset_device();
  sl_sleeptimer_delay_millisecond(1000);

  app_obdii_process();
  app_obdii_clear_buf();

  app_printf("> Reset device\r\n");
  mikroe_stn1110_send_command((uint8_t *)MIKROE_OBDII_CMD_RESET_DEVICE);
  app_obdii_rsp_check((uint8_t *)MIKROE_OBDII_RSP_PROMPT);
  app_obdii_log_buf();
  sl_sleeptimer_delay_millisecond(1000);

  app_printf(" Disable echo\r\n");
  mikroe_stn1110_send_command((uint8_t *)MIKROE_OBDII_CMD_DISABLE_ECHO);
  app_obdii_rsp_check((uint8_t *)MIKROE_OBDII_RSP_PROMPT);
  app_obdii_log_buf();

  app_printf(" Remove spaces\r\n");
  mikroe_stn1110_send_command((uint8_t *)MIKROE_OBDII_CMD_SPACES_OFF);
  app_obdii_rsp_check((uint8_t *)MIKROE_OBDII_RSP_PROMPT);
  app_obdii_log_buf();

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

  application_task();
}

static void app_obdii_clear_buf(void)
{
  memset(app_buf, 0, app_buf_len);
  app_buf_len = 0;
}

static void application_task(void)
{
  uint8_t *start_ptr = NULL;
  char *ptr = NULL;
  uint8_t data_buf[5] = { 0 };
  uint16_t rpm = 0;
  uint8_t speed = 0;

  app_printf(" Get current RPM\r\n");
  mikroe_stn1110_send_command((uint8_t *)MIKROE_OBDII_CMD_GET_CURRENT_RPM);
  app_obdii_rsp_check((uint8_t *)MIKROE_OBDII_RSP_PROMPT);
  start_ptr = (uint8_t *)strstr((const char *)app_buf,
                                MIKROE_OBDII_RSP_CURRENT_RPM);
  if (start_ptr) {
    memcpy(data_buf, (start_ptr + 4), 4);
    data_buf[4] = 0;
    rpm = (uint16_t)strtol((const char *)data_buf, &ptr, 16) / 4;
    app_printf("RPM: %u\r\n\n>", rpm);
  } else {
    app_obdii_log_buf();
  }

  app_printf(" Get current speed\r\n");
  mikroe_stn1110_send_command((uint8_t *)MIKROE_OBDII_CMD_GET_CURRENT_SPEED);
  app_obdii_rsp_check((uint8_t *)MIKROE_OBDII_RSP_PROMPT);
  start_ptr = (uint8_t *)strstr((const char *)app_buf,
                                MIKROE_OBDII_RSP_CURRENT_SPEED);
  if (start_ptr) {
    memcpy(data_buf, (start_ptr + 4), 2);
    data_buf[2] = 0;
    speed = (uint8_t)strtol((const char *)data_buf, &ptr, 16);
    app_printf("Speed: %u km/h\r\n\n>", ( uint16_t ) speed);
  } else {
    app_obdii_log_buf();
  }
  sl_sleeptimer_delay_millisecond(1000);
}

static sl_status_t app_obdii_process(void)
{
  uint8_t rx_buf[PROCESS_BUFFER_SIZE] = { 0 };
  int32_t rx_size = 0;
  rx_size = mikroe_stn1110_generic_read(rx_buf, PROCESS_BUFFER_SIZE);
  if (rx_size > 0) {
    int32_t buf_cnt = app_buf_len;
    if (((app_buf_len + rx_size) > PROCESS_BUFFER_SIZE) && (app_buf_len > 0)) {
      buf_cnt = PROCESS_BUFFER_SIZE
                - ((app_buf_len + rx_size) - PROCESS_BUFFER_SIZE);
      memmove(app_buf, &app_buf[PROCESS_BUFFER_SIZE - buf_cnt], buf_cnt);
    }
    for ( int32_t rx_cnt = 0; rx_cnt < rx_size; rx_cnt++ )
    {
      if (rx_buf[rx_cnt]) {
        app_buf[buf_cnt++] = rx_buf[rx_cnt];
        if (app_buf_len < PROCESS_BUFFER_SIZE) {
          app_buf_len++;
        }
      }
    }
    return SL_STATUS_OK;
  }
  return SL_STATUS_FAIL;
}

static void app_obdii_log_buf(void)
{
  for ( int32_t buf_cnt = 0; buf_cnt < app_buf_len; buf_cnt++ )
  {
    app_printf("%c", app_buf[buf_cnt]);
  }
}

static sl_status_t app_obdii_rsp_check(uint8_t *rsp)
{
  uint32_t timeout_cnt = 0;
  uint32_t timeout = 60000;
  app_obdii_clear_buf();
  app_obdii_process();
  while (0 == strstr((const char *)app_buf, (const char *)rsp))
  {
    app_obdii_process();
    if (timeout_cnt++ > timeout) {
      app_obdii_clear_buf();
      return SL_STATUS_TIMEOUT;
    }
    sl_sleeptimer_delay_millisecond(1);
  }
  sl_sleeptimer_delay_millisecond(100);
  app_obdii_process();
  if (strstr((const char *)app_buf, (const char *)rsp)) {
    return SL_STATUS_OK;
  }
  return SL_STATUS_FAIL;
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
