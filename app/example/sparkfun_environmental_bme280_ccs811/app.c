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

#include "sparkfun_bme280.h"
#include "sparkfun_ccs811.h"

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

#define READING_INTERVAL_MSEC        1000

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

static mikroe_i2c_handle_t app_i2c_instance = NULL;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_printf("Application Initialization.\n");

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  sparkfun_bme280_i2c(app_i2c_instance, SPARKFUN_BME280_ADDR);
  if (sparkfun_bme280_init() == SL_STATUS_OK) {
    app_printf("BME280 on 0x%02X I2C address found and initialized.\n",
               SPARKFUN_BME280_ADDR);
  } else {
    app_printf("BME280 on 0x%02X I2C address not found. Check cables. "
               "\n\r Try also alternative address:", SPARKFUN_BME280_ADDR);
    if (SPARKFUN_BME280_ADDR == 0x77) {
      app_printf(" 0x76\n");
    } else {
      app_printf(" 0x77\n");
    }
  }

  if (!sparkfun_ccs811_init(app_i2c_instance)) {
    app_printf("CCS811 on 0x%02X I2C address found and initialized.\n",
               SPARKFUN_CCS811_ADDR);
  } else {
    app_printf("CCS811 on 0x%02X I2C address not found. Check cables. "
               "\n\r Try also alternative address\n",
               SPARKFUN_CCS811_ADDR);
    if (SPARKFUN_CCS811_ADDR == 0x5B) {
      app_printf(" 0x5A\n");
    } else {
      app_printf(" 0x5B\n");
    }
  }

  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_cb,
                                        (void *) NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint16_t eco2, tvoc;
  int32_t temp = 0;
  uint32_t hum = 0;
  uint32_t press = 0;

  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;

  sparkfun_bme280_ctrl_measure_set_to_work();

  if (SL_STATUS_OK == sparkfun_bme280_read_temperature(&temp)) {
    app_printf("Temperature: %ld %cC\n", (temp / 100), 0XF8);
  }

  if (SL_STATUS_OK == sparkfun_bme280_read_humidity(&hum)) {
    app_printf("Humidity: %ld%%\n", hum / 1000);
  }

  if (SL_STATUS_OK == sparkfun_bme280_read_pressure(&press)) {
    app_printf("Presure: %ld mBar\n", press);
  }

  if (!sparkfun_ccs811_get_measurement(&eco2, &tvoc)) {
    app_printf("CO2: %d ppm\n", eco2);
    app_printf("TVOC: %d ppb\n", tvoc);
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
