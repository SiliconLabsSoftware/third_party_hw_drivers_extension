/***************************************************************************//**
 * @file app.c
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
 * # Experimental Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include "sl_component_catalog.h"
#include "sl_sleeptimer.h"
#include "bme68x.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_SPI_PRESENT
#include "mikroe_bme688_spi.h"
#include "mikroe_bme688_spi_config.h"
#include "sl_si91x_gspi.h"
static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#endif

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_I2C_PRESENT
#include "mikroe_bme688_i2c.h"
#include "mikroe_bme688_i2c_config.h"
#include "sl_i2c_instances.h"
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else /* None Si91x device */
#include "app_log.h"

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_SPI_PRESENT
#include "sl_spidrv_instances.h"
#include "mikroe_bme688_spi.h"
#include "mikroe_bme688_spi_config.h"
#endif

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_I2C_PRESENT
#include "sl_i2cspm_instances.h"
#include "mikroe_bme688_i2c.h"
#include "mikroe_bme688_i2c_config.h"
#endif

#define app_printf(...) app_log(__VA_ARGS__)
#endif

#define READING_INTERVAL_MSEC    2000
#define HEATER_TEMPERATURE       300
#define HEATING_DURATION         100

static bme68x_dev_t bme688;

static volatile bool enable_reading_data = false;
static sl_sleeptimer_timer_handle_t app_periodic_timer;

static sl_status_t app_bme688_init(void);
static sl_status_t app_bme688_get_data(void);
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data);

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_SPI_PRESENT
mikroe_spi_handle_t app_spi_instance = NULL;
#endif
#ifdef SL_CATALOG_ENVIRONMENT3_BME688_I2C_PRESENT
static mikroe_i2c_handle_t app_i2c_instance = NULL;
#endif

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  if (app_bme688_init() != SL_STATUS_OK) {
    app_printf("Initialization error. Please check again ...\r\n");
  } else {
#ifdef SL_CATALOG_ENVIRONMENT3_BME688_SPI_PRESENT
    app_printf("Environment 3 BME688 - SPI Initialization Successful\r\n");
#endif
#ifdef SL_CATALOG_ENVIRONMENT3_BME688_I2C_PRESENT
    app_printf("Environment 3 BME688 - I2C Initialization Successful\r\n");
#endif
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (enable_reading_data) {
    enable_reading_data = false;

    if (app_bme688_get_data() != SL_STATUS_OK) {
      app_printf("Reading error. Please check again ...\r\n");
    }
  }
}

static sl_status_t app_bme688_init(void)
{
  int8_t rslt;
  bme68x_conf_t conf;
  bme68x_heatr_conf_t heatr_conf;

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_I2C_PRESENT
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif
  rslt = mikroe_bme688_i2c_init(&bme688, app_i2c_instance);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }
#endif

#ifdef SL_CATALOG_ENVIRONMENT3_BME688_SPI_PRESENT
#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif
  rslt = mikroe_bme688_spi_init(&bme688, app_spi_instance);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }
#endif

  rslt = bme68x_init(&bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }

  conf.filter = BME68X_FILTER_OFF;
  conf.odr = BME68X_ODR_NONE;
  conf.os_hum = BME68X_OS_16X;
  conf.os_pres = BME68X_OS_1X;
  conf.os_temp = BME68X_OS_2X;
  rslt = bme68x_set_conf(&conf, &bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }

  heatr_conf.enable = BME68X_ENABLE;
  heatr_conf.heatr_temp = HEATER_TEMPERATURE;
  heatr_conf.heatr_dur = HEATING_DURATION;
  rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }

  // Start periodic timer used for read data from sensor.
  return sl_sleeptimer_start_periodic_timer_ms(&app_periodic_timer,
                                               READING_INTERVAL_MSEC,
                                               app_periodic_timer_cb,
                                               (void *) NULL,
                                               0,
                                               0);
}

static sl_status_t app_bme688_get_data(void)
{
  bme68x_data_t data;
  uint8_t n_fields;
  int8_t rslt;
  uint32_t delay_period;
  bme68x_conf_t conf;

  // Trigger a measurement
  rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }
  // Calculate delay period in microseconds
  delay_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme688)
                 + HEATING_DURATION * 1000;
  // Wait for the measurement to complete
  bme688.delay_us(delay_period, bme688.intf_ptr);
  rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme688);
  if (rslt != BME68X_OK) {
    return SL_STATUS_FAIL;
  }

  if (n_fields) {
    app_printf("\r\nTemperature : %.2f" "\xB0" "C\r\n", data.temperature);
    app_printf("Humidity : %.2f %%\r\n", data.humidity);
    app_printf("Pressure : %.2f Pa\r\n", data.pressure);
    app_printf("Gas resistance : %.2f Ohm\r\n", data.gas_resistance);
  }

  return SL_STATUS_OK;
}

static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data)
{
  (void)timer;
  (void)data;

  enable_reading_data = true;
}
