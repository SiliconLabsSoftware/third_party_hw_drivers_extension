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
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "mikroe_pl_n823_01.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED            SL_I2C2

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

#define APP_TIMER_INTERVAL_MS        1000

static sl_sleeptimer_timer_handle_t app_timer;
static volatile bool update = true;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void app_timer_handler(sl_sleeptimer_timer_handle_t *timer, void *data);

/***************************************************************************//**
 * App timer handler function.
 ******************************************************************************/
static void app_timer_handler(sl_sleeptimer_timer_handle_t *timer, void *data)
{
  (void) timer;
  (void) data;

  update = true;
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;
  app_printf("=======================================\n");
  app_printf("Silicon Labs Example - MikroE PIR Click.\n");
  app_printf(" Application initialized successfully.\n");

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  sc = mikroe_pl_n823_01_init(app_i2c_instance);
  if (sc != SL_STATUS_OK) {
    app_printf("MikroE PIR Click initialized failed!\n");
  }
  app_printf("=======================================\n");
  sl_sleeptimer_start_periodic_timer_ms(&app_timer,
                                        APP_TIMER_INTERVAL_MS,
                                        app_timer_handler,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint16_t adc_val;
  float map_out;

  if (update) {
    adc_val = mikroe_pl_n823_01_get_adc();
    map_out = mikroe_pl_n823_01_scale_results(adc_val, 0, 3303);

    app_printf("[PL-N823-01]: Voltage: %.2f mV\n", map_out);
    update = false;
  }
}
