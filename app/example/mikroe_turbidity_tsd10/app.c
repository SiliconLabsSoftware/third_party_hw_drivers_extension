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

#include "mikroe_tsd10.h"
#include "sl_sleeptimer.h"
#include "sl_status.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED             SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

static float sensor_ntu_value;
static volatile bool turbidity_timer_expire = false;
static sl_sleeptimer_timer_handle_t turbidity_timer;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void turbidity_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                          void *data);

void app_init(void)
{
  app_printf("Mikroe Turbidity - TSD10 sensor example\r\n");

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  if (SL_STATUS_OK == mikroe_turbidity_init(app_i2c_instance)) {
    app_printf("Sensor initialization done.\n");
  } else {
    app_printf("Sensor initialization failed.\n");
  }

  sl_sleeptimer_start_periodic_timer_ms(&turbidity_timer,
                                        500,
                                        turbidity_sleeptimer_callback,
                                        (void *) NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static sl_status_t ret;
  if (turbidity_timer_expire) {
    turbidity_timer_expire = false;

    ret = mikroe_turbidity_get_ntu(&sensor_ntu_value);
    if (SL_STATUS_OK == ret) {
      app_printf("NTU value: %.2f\n", sensor_ntu_value);
    } else {
      app_printf("Sensor read failed\n");
    }
  }
}

void turbidity_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                   void *data)
{
  (void) data;
  (void) timer;

  turbidity_timer_expire = true;
}
