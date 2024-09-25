/*******************************************************************************
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

#include "sl_sleeptimer.h"
#include "sparkfun_sgp40.h"

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
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

#define READING_INTERVAL_MSEC        1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);
static sl_status_t app_sgp40_init(void);

/// ***************************************************************************//**
// * Initialize application.
// ******************************************************************************/
void app_init(void)
{
  // Click initialization.
  app_printf("    Application Task   \r\n");
  app_printf("-----------------------\r\n");
  app_printf("  Air Quality Sensor  \r\n");
  app_printf("-----------------------\r\n");

  if (app_sgp40_init() != SL_STATUS_OK) {
    app_printf("  SPG40 inits failed  \r\n");
    app_printf("-----------------------\r\n");
  } else {
    app_printf("  SPG40 inits successfully \r\n");
    app_printf("-----------------------\r\n");
    app_printf("  Start measure  \r\n");
    app_printf("-----------------------\r\n");
  }
}

/// ***************************************************************************//**
// * App ticking function.
// ******************************************************************************/
void app_process_action(void)
{
  uint16_t air_quality;
  int32_t voc_index;

  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;

  sparkfun_sgp40_measure_raw(&air_quality, 50, 25);
  app_printf(" RAW data   : %d  \r\n", air_quality);
  app_printf("-----------------------\r\n");

  sparkfun_sgp40_get_voc_index(&voc_index, 50, 25);
  app_printf(" VOC Index   : %ld  \r\n", voc_index);
  app_printf("-----------------------\r\n");
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}

static sl_status_t app_sgp40_init(void)
{
  sl_status_t ret;
  uint16_t results;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  ret = sparkfun_sgp40_init(app_i2c_instance);
  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  sparkfun_sgp40_measure_test(&results);
  if (results != SPARKFUN_SGP40_TEST_PASSED) {
    return SL_STATUS_NOT_READY;
  }
  ret = sparkfun_sgp40_heater_off();
  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }

  sparkfun_sgp40_voc_algorithm_init();

  // Start a periodic timer 200 ms to read data from the sensor
  ret = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                              READING_INTERVAL_MSEC,
                                              app_timer_cb,
                                              (void *) NULL,
                                              0,
                                              0);

  return ret;
}
