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

#include "mikroe_rng.h"
#include "sl_sleeptimer.h"

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

#define READING_INTERVAL_MSEC        500

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool trigger_process = false;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t stt;

  app_printf("Hello World - Mikroe RNG Click example\r\n");

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  stt = mikroe_rng_init(app_i2c_instance);
  app_printf("mikroe_rng_init = 0x%lx\r\n", stt);
  mikroe_rng_default_cfg();

  stt = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                              READING_INTERVAL_MSEC,
                                              app_timer_callback,
                                              NULL,
                                              0,
                                              0);

  app_printf("sl_sleeptimer_start_periodic = 0x%lx\r\n", stt);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (trigger_process) {
    trigger_process = false;

    float voltage;
    sl_status_t stt = mikroe_rng_get_voltage(&voltage);

    app_printf("mikroe_rng_get_voltage = 0x%lx, voltage = %.3f(mV)\r\n",
               stt, voltage);
  }
}

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
