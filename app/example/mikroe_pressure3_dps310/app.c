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
#include "sl_component_catalog.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#include "app_log.h"
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#ifdef SL_CATALOG_MIKROE_PRESSURE3_SPI_PRESENT
#include "mikroe_dps310_spi.h"

#if (defined(SLI_SI917))
#include "sl_si91x_gspi.h"

static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#else
#include "sl_spidrv_instances.h"
#endif

mikroe_spi_handle_t app_spi_instance = NULL;
#endif

#ifdef SL_CATALOG_MIKROE_PRESSURE3_I2C_PRESENT
#include "mikroe_dps310_i2c.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#else
#include "sl_i2cspm_instances.h"
#endif

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

static mikroe_i2c_handle_t app_i2c_instance = NULL;
#endif

#define READING_INTERVAL_MSEC    1000

static volatile bool enable_reading_data = false;
static sl_sleeptimer_timer_handle_t app_periodic_timer;

static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#ifdef SL_CATALOG_MIKROE_PRESSURE3_SPI_PRESENT
#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

  mikroe_pressure3_init(app_spi_instance);
#endif

#ifdef SL_CATALOG_MIKROE_PRESSURE3_I2C_PRESENT
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif
  mikroe_pressure3_init(app_i2c_instance);
#endif

  app_printf("Sensor initialization done.\n");
  // Start periodic timer used for read data from sensor.
  sl_sleeptimer_start_periodic_timer_ms(&app_periodic_timer,
                                        READING_INTERVAL_MSEC,
                                        app_periodic_timer_cb,
                                        (void *) NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  float pressure;
  float temperature;

  if (enable_reading_data) {
    enable_reading_data = false;

    mikroe_pressure3_get_t_p_data(&temperature, &pressure);
    app_printf(" * Pressure: %.2f mbar * \r\n", pressure);
    app_printf(" * Temperature: %.2f C * \r\n", temperature);
    app_printf(" ----------------------- \r\n");
  }
}

static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data)
{
  (void)timer;
  (void)data;

  enable_reading_data = true;
}
