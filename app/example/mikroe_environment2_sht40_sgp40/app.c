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
#include "mikroe_sht40_sgp40.h"

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

static bool timer_is_expire = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);
static void app_task(void);

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
void app_init(void)
{
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  if (SL_STATUS_OK != mikroe_environment2_init(app_i2c_instance)) {
    app_printf("Sensor initialized fail!.");
  } else {
    if (mikroe_environment2_sgp40_measure_test() == SL_STATUS_OK) {
      app_printf("    All tests passed\r\n");
      app_printf("      Successfully\r\n");
    } else {
      app_printf("    One or more tests have\r\n");
      app_printf("     Failed\r\n");
    }
    app_printf("-----------------------\r\n");

    mikroe_environment2_sgp40_heater_off();

    mikroe_environment2_config_sensors();

    sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                          READING_INTERVAL_MSEC,
                                          app_timer_cb,
                                          (void *) NULL,
                                          0,
                                          0);
  }
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
void app_process_action(void)
{
  if (timer_is_expire == false) {
    return;
  }
  timer_is_expire = false;
  app_task();
}

static void app_task(void)
{
  uint16_t air_quality;
  float humidity;
  float temperature;
  int32_t voc_index;

  mikroe_environment2_get_temp_hum(&humidity, &temperature);
  app_printf(" Humidity    : %.2f %% \r\n", humidity);
  app_printf(" Temperature : %.2f C \r\n", temperature);

  mikroe_environment2_get_air_quality(&air_quality);
  app_printf(" Air Quality : %d \r\n", air_quality);
  app_printf("- - - - - - - - - -  - \r\n");

  mikroe_environment2_get_voc_index(&voc_index);
  app_printf(" VOC Index   : %d  \r\n", ( uint16_t ) voc_index);
  app_printf("-----------------------\r\n");
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  timer_is_expire = true;
}
