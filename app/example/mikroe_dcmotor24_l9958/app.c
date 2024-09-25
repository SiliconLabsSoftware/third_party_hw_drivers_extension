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

#include "mikroe_l9958.h"
#include "sl_pwm_instances.h"
#include "sl_sleeptimer.h"
#include "app_assert.h"

#if (defined(SLI_SI917))
#include "sl_si91x_gspi.h"
#else
#include "sl_spidrv_instances.h"
#endif

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#else
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#define APP_TIMER_TIMEOUT 500

#if (defined(SLI_SI917))
static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#endif

mikroe_spi_handle_t app_spi_instance = NULL;
mikroe_pwm_handle_t app_pwm_instance = NULL;

static volatile bool app_timer_expire = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static void app_timer_handle_callback(sl_sleeptimer_timer_handle_t *handle,
                                      void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

#if (defined(SLI_SI917))
  app_pwm_instance = &sl_pwm_channel_0_config;
#else
  app_pwm_instance = &sl_pwm_mikroe;
#endif

  if (mikroe_l9958_init(app_spi_instance,
                        app_pwm_instance) == SL_STATUS_OK) {
    app_printf("DC Motor 24 Click initializes successfully\n");
  }
  sc = mikroe_l9958_default_cfg();
  app_assert_status(sc);
  sl_sleeptimer_delay_millisecond(1000);

  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        APP_TIMER_TIMEOUT,
                                        app_timer_handle_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static int8_t duty_pct = 10;
  static int8_t duty_step = 10;

  if (app_timer_expire) {
    if (SL_STATUS_OK == mikroe_l9958_set_duty_cycle((float)duty_pct / 100)) {
      app_printf("Duty cycle: %d\n", duty_pct);
    }
    if ((100 == duty_pct) || (0 == duty_pct)) {
      duty_step = -duty_step;
      if (0 == duty_pct) {
        app_printf("Switch direction\n");
        mikroe_l9958_switch_direction();
      }
    }
    duty_pct += duty_step;
    app_timer_expire = false;
  }
}

static void app_timer_handle_callback(sl_sleeptimer_timer_handle_t *handle,
                                      void *data)
{
  (void) handle;
  (void) data;
  app_timer_expire = true;
}
