/***************************************************************************//**
 * @file app.c
 * @brief Example application
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

#include "sl_sleeptimer.h"
#include "mikroe_shtc3.h"

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

#define READING_INTERVAL_MSEC        1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool trigger_process = false;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  app_printf(
    "-- SHTC3 - Temp&Hum 9 Click board driver, example application. --\n");

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  if (SL_STATUS_OK != mikroe_shtc3_init(app_i2c_instance)) {
    app_printf("> SHTC3 - Temp&Hum 9 Click board driver init failed.\n");
    return;
  }

  mikroe_shtc3_send_command(MIKROE_SHTC3_CMD_SLEEP);
  sl_sleeptimer_delay_millisecond(500);
  mikroe_shtc3_send_command(MIKROE_SHTC3_CMD_WAKEUP);
  sl_sleeptimer_delay_millisecond(100);

  app_printf("> App init done.\n");
  app_printf("> Starting periodic measurement.\n");
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
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
  mikroe_shtc3_measurement_data_t measurement_data;

  if (trigger_process) {
    trigger_process = false;
    mikroe_shtc3_get_temperature_and_humidity(SHTC3_DATA_MODE_NORMAL,
                                              &measurement_data);

    app_printf(">> Temp: %.2f °C RH: %.2f %%\n", measurement_data.temperature,
               measurement_data.humidity);
  }
}

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
