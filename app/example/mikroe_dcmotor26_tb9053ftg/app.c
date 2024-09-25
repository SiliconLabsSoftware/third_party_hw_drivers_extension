/***************************************************************************//**
 * @file app.c
 * @brief Example application
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

#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "mikroe_tb9053ftg.h"
#include "app_assert.h"

#if (defined(SLI_SI917))
#include "sl_si91x_ssi.h"
#include "sl_i2c_instances.h"
#include "sl_adc_instances.h"
#include "rsi_debug.h"
#else
#include "sl_spidrv_instances.h"
#include "sl_i2cspm_instances.h"
#include "em_iadc.h"
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
#define app_printf(...)              DEBUGOUT(__VA_ARGS__)

static sl_ssi_instance_t ssi_instance = SL_SSI_MASTER_ACTIVE;
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
static uint8_t adc_channel = SL_ADC_CHANNEL_1;
#else
#define app_printf(...)              app_log(__VA_ARGS__)
#endif

#define APP_SET_MODE_TIMER_TIMEOUT   3000
#define APP_SET_DUTY_TIMER_TIMEOUT   500

mikroe_spi_handle_t app_spi_instance = NULL;
mikroe_i2c_handle_t app_i2c_instance = NULL;
mikroe_adc_handle_t app_adc_handle = NULL;

static volatile bool duty_changed = false;
static volatile uint8_t mode;

static sl_sleeptimer_timer_handle_t set_mode_timer;
static sl_sleeptimer_timer_handle_t set_duty_timer;

static void application_task(void);
static void app_set_mode_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data);
static void app_set_duty_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
  app_spi_instance = &ssi_instance;
  app_i2c_instance = &i2c_instance;
  app_adc_handle = &adc_channel;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
  app_i2c_instance = sl_i2cspm_mikroe;
  app_adc_handle = IADC0;
#endif

  app_printf("Mikroe DC Motor 26 Click Driver - Example\n");
  app_printf("======= Application initialization =======\n");

  sc = mikroe_tb9053ftg_init(app_spi_instance,
                             app_i2c_instance,
                             app_adc_handle);
  if (SL_STATUS_OK != sc) {
    app_printf("DC Motor 26 Click initialized failed!\n");
    return;
  } else {
    app_printf("DC Motor 26 Click initialized successfully!\n");
  }

  mikroe_tb9053ftg_default_cfg();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  application_task();
}

/***************************************************************************//**
 * Application task function.
 ******************************************************************************/
static void application_task(void)
{
  sl_status_t sc;
  bool is_running, mode_changed;
  uint16_t duty;
  float current;

  mode = MIKROE_TB9053FTG_MODE_OUTPUT_OFF;
  sc = mikroe_tb9053ftg_set_ch1_operation_mode(mode);
  app_assert_status(sc);
  app_printf("\nCH1 MODE: OFF\n");
  app_printf("Change to FORWARD MODE after 3 seconds ...\n");
  sc = sl_sleeptimer_start_timer_ms(&set_mode_timer,
                                    APP_SET_MODE_TIMER_TIMEOUT,
                                    app_set_mode_timer_callback,
                                    NULL,
                                    0,
                                    0);
  app_assert_status(sc);
  sc = sl_sleeptimer_is_timer_running(&set_duty_timer, &is_running);
  app_assert_status(sc);
  if (!is_running) {
    sc = sl_sleeptimer_start_periodic_timer_ms(&set_duty_timer,
                                               APP_SET_DUTY_TIMER_TIMEOUT,
                                               app_set_duty_timer_callback,
                                               NULL,
                                               0,
                                               0);
    app_assert_status(sc);
  }

  mode_changed = true;
  while (mode_changed) {
    if (MIKROE_TB9053FTG_MODE_FORWARD == mode) {
      sc = mikroe_tb9053ftg_set_ch1_operation_mode(mode);
      app_assert_status(sc);
      app_printf("\nCH1 MODE: FORWARD\n");
      sc = mikroe_tb9053ftg_set_cm_sel_pin(MIKROE_TB9053FTG_PIN_LOW_LEVEL);
      app_assert_status(sc);
      while (duty < MIKROE_TB9053FTG_CONFIG56_DUTY_PERIOD_MAX) {
        if (duty_changed) {
          sc = mikroe_tb9053ftg_set_ch1_duty_period(duty);
          app_assert_status(sc);
          app_printf("Duty: %d\n", duty);
          sc = mikroe_tb9053ftg_get_motor_current(&current);
          app_assert_status(sc);
          app_printf("Current: %1.2f mA\n", current * 1000);
          duty += 100;
          duty_changed = false;
        }
      }
      duty = 0;
      sl_sleeptimer_restart_timer_ms(&set_mode_timer,
                                     3000,
                                     app_set_mode_timer_callback,
                                     NULL,
                                     0,
                                     0);
      mode_changed = false;
      app_printf("Change to BRAKE MODE after 3 seconds ...\n");
    }
  }

  mode_changed = true;
  while (mode_changed) {
    if (MIKROE_TB9053FTG_MODE_BRAKE == mode) {
      sc = mikroe_tb9053ftg_set_ch1_operation_mode(mode);
      app_assert_status(sc);
      app_printf("\nCH1 MODE: BRAKE\n");
      sl_sleeptimer_restart_timer_ms(&set_mode_timer, 3000,
                                     app_set_mode_timer_callback,
                                     NULL,
                                     0,
                                     0);
      app_printf("Change to REVERSE MODE after 3 seconds ...\n");
      mode_changed = false;
    }
  }

  mode_changed = true;
  while (mode_changed) {
    if (MIKROE_TB9053FTG_MODE_REVERSE == mode) {
      sc = mikroe_tb9053ftg_set_ch1_operation_mode(mode);
      app_assert_status(sc);
      sc = mikroe_tb9053ftg_set_cm_sel_pin(MIKROE_TB9053FTG_PIN_HIGH_LEVEL);
      app_assert_status(sc);
      app_printf("\nCH1 MODE: REVERSE\n");
      while (duty < MIKROE_TB9053FTG_CONFIG56_DUTY_PERIOD_MAX) {
        if (duty_changed) {
          mikroe_tb9053ftg_set_ch1_duty_period(duty);
          app_printf("Duty: %d\n", duty);
          mikroe_tb9053ftg_get_motor_current(&current);
          app_printf("Current: %1.2f mA\n", current * 1000);
          duty += 100;
          duty_changed = false;
        }
        sl_sleeptimer_stop_timer(&set_mode_timer);
        mode_changed = false;
      }
    }
  }
}

/***************************************************************************//**
 * App set mode timer callback.
 ******************************************************************************/
static void app_set_mode_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data)
{
  (void) timer;
  (void) data;

  switch (mode) {
    case MIKROE_TB9053FTG_MODE_OUTPUT_OFF:
      mode = MIKROE_TB9053FTG_MODE_FORWARD;
      break;
    case MIKROE_TB9053FTG_MODE_FORWARD:
      mode = MIKROE_TB9053FTG_MODE_BRAKE;
      break;
    case MIKROE_TB9053FTG_MODE_BRAKE:
      mode = MIKROE_TB9053FTG_MODE_REVERSE;
      break;
    default:
      break;
  }
}

/***************************************************************************//**
 * App set duty timer callback.
 ******************************************************************************/
static void app_set_duty_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data)
{
  (void) timer;
  (void) data;

  duty_changed = true;
}
