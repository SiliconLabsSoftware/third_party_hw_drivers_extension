/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
#include "sl_component_catalog.h"
#include "sl_sleeptimer.h"
#include "bma400.h"

#define MIKROE_BMA400_READ_MODE_INTERRUPT
// #define MIKROE_BMA400_READ_MODE_POLLING

#if (defined(SLI_SI917))
#include "rsi_debug.h"

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_SPI_PRESENT
#include "mikroe_bma400_spi.h"
#include "mikroe_bma400_spi_config.h"
#include "sl_si91x_gspi.h"

static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#endif // SL_CATALOG_MIKROE_ACCEL5_BMA400_SPI_PRESENT

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_I2C_PRESENT
#include "mikroe_bma400_i2c.h"
#include "mikroe_bma400_i2c_config.h"
#include "sl_i2c_instances.h"

#define I2C_INSTANCE_USED     SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif // SL_CATALOG_MIKROE_ACCEL5_BMA400_I2C_PRESENT

#define app_printf(...)       DEBUGOUT(__VA_ARGS__)

#ifdef MIKROE_BMA400_READ_MODE_INTERRUPT
#include "sl_driver_gpio.h"

#define PIN_INTR_NO           PIN_INTR_0
#define AVL_INTR_NO           0 // available interrupt number
#endif // MIKROE_BMA400_READ_MODE_INTERRUPT

#else // SLI_SI917
#include "app_log.h"

#ifdef MIKROE_BMA400_READ_MODE_INTERRUPT
#include "sl_gpio.h"
#endif // MIKROE_BMA400_READ_MODE_INTERRUPT

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_SPI_PRESENT
#include "sl_spidrv_instances.h"
#include "mikroe_bma400_spi.h"
#include "mikroe_bma400_spi_config.h"
#endif // SL_CATALOG_MIKROE_ACCEL5_BMA400_SPI_PRESENT

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_I2C_PRESENT
#include "sl_i2cspm_instances.h"
#include "mikroe_bma400_i2c.h"
#include "mikroe_bma400_i2c_config.h"
#endif // SL_CATALOG_MIKROE_ACCEL5_BMA400_I2C_PRESENT

#define app_printf(...)       app_log(__VA_ARGS__)
#endif // SLI_SI917

#define READING_INTERVAL_MSEC 200
// Earth's gravity in m/s^2
#define GRAVITY_EARTH         (9.80665f)
// 39.0625us per tick
#define SENSOR_TICK_TO_S      (0.0000390625f)

static struct bma400_dev bma;
static volatile bool enable_reading_data = false;

static sl_status_t app_bma400_init(void);
static sl_status_t app_bma400_read_data(void);
static float lsb_to_ms2(int16_t accel_data, uint8_t g_range, uint8_t bit_width);

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_SPI_PRESENT
mikroe_spi_handle_t app_spi_instance = NULL;
#endif // SL_CATALOG_MIKROE_ACCEL5_BMA400_SPI_PRESENT
#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_I2C_PRESENT
static mikroe_i2c_handle_t app_i2c_instance = NULL;
#endif // SL_CATALOG_MIKROE_ACCEL5_BMA400_I2C_PRESENT

#ifdef MIKROE_BMA400_READ_MODE_INTERRUPT
#if (defined(SLI_SI917))
static void app_gpio_int_cb(uint32_t int_no)
{
#else // SLI_SI917
static void app_gpio_int_cb(uint8_t int_no, void *ctx)
{
  (void)ctx;
#endif // SLI_SI917
  (void)int_no;

  enable_reading_data = true;
}

#endif // MIKROE_BMA400_READ_MODE_INTERRUPT

#ifdef MIKROE_BMA400_READ_MODE_POLLING
static sl_sleeptimer_timer_handle_t app_periodic_timer;
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data);

#endif // MIKROE_BMA400_READ_MODE_POLLING

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  if (app_bma400_init() != SL_STATUS_OK) {
    app_printf("Initialization error. Please check again ...\r\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (enable_reading_data) {
    enable_reading_data = false;
    if (app_bma400_read_data() != SL_STATUS_OK) {
      app_printf("Reading error. Please check again ...\r\n");
    }
  }
}

#ifdef MIKROE_BMA400_READ_MODE_POLLING

/**************************************************************************//**
 * Timer callback
 * Called periodically to reading data from BMA400.
 *****************************************************************************/
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data)
{
  (void)data;
  (void)timer;

  enable_reading_data = true;
}

#endif // MIKROE_BMA400_READ_MODE_POLLING

static sl_status_t app_bma400_init(void)
{
  int8_t rslt;
  struct bma400_sensor_conf conf;
  struct bma400_int_enable int_en;

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_SPI_PRESENT
#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
#else // SLI_SI917
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif // SLI_SI917

  // Initialize an SPI interface for BMA400
  rslt = bma400_spi_init(app_spi_instance, &bma);
  if (rslt != BMA400_OK) {
    return SL_STATUS_FAIL;
  }
#endif // SL_CATALOG_MIKROE_ACCEL5_BMA400_SPI_PRESENT

#ifdef SL_CATALOG_MIKROE_ACCEL5_BMA400_I2C_PRESENT
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else // SLI_SI917
  app_i2c_instance = sl_i2cspm_mikroe;
#endif // SLI_SI917

  // Initialize an I2C interface for BMA400
  rslt = bma400_i2c_init(app_i2c_instance, MIKROE_BMA400_ADDR, &bma);
  if (rslt != BMA400_OK) {
    return SL_STATUS_FAIL;
  }
#endif // SL_CATALOG_MIKROE_ACCEL5_BMA400_I2C_PRESENT

  rslt = bma400_soft_reset(&bma);
  if (rslt != BMA400_OK) {
    return SL_STATUS_FAIL;
  }
  rslt = bma400_init(&bma);
  if (rslt != BMA400_OK) {
    return SL_STATUS_FAIL;
  }
  // Select the type of configuration to be modified
  conf.type = BMA400_ACCEL;

  // Get the accelerometer configurations which are set in the sensor
  rslt = bma400_get_sensor_conf(&conf, 1, &bma);
  if (rslt != BMA400_OK) {
    return SL_STATUS_FAIL;
  }

  // Modify the desired configurations as per macros
  // available in bma400_defs.h file
#ifdef MIKROE_BMA400_READ_MODE_INTERRUPT
  conf.param.accel.int_chan = BMA400_INT_CHANNEL_1;
#endif // MIKROE_BMA400_READ_MODE_INTERRUPT
  conf.param.accel.odr = BMA400_ODR_100HZ;
  conf.param.accel.range = BMA400_RANGE_2G;
  conf.param.accel.data_src = BMA400_DATA_SRC_ACCEL_FILT_1;
  // Set the desired configurations to the sensor
  rslt = bma400_set_sensor_conf(&conf, 1, &bma);
  if (rslt != BMA400_OK) {
    return SL_STATUS_FAIL;
  }

  rslt = bma400_set_power_mode(BMA400_MODE_NORMAL, &bma);
  if (rslt != BMA400_OK) {
    return SL_STATUS_FAIL;
  }

  int_en.type = BMA400_DRDY_INT_EN;
  int_en.conf = BMA400_ENABLE;
  rslt = bma400_enable_interrupt(&int_en, 1, &bma);
  if (rslt != BMA400_OK) {
    return SL_STATUS_FAIL;
  }

  app_printf("Accel Gravity data in m/s^2\r\n");

#ifdef MIKROE_BMA400_READ_MODE_POLLING
  // Start timer used for periodic indications.
  sl_sleeptimer_start_periodic_timer_ms(&app_periodic_timer,
                                        READING_INTERVAL_MSEC,
                                        app_periodic_timer_cb,
                                        (void *) NULL,
                                        0,
                                        0);
#endif // MIKROE_BMA400_READ_MODE_POLLING

#ifdef MIKROE_BMA400_READ_MODE_INTERRUPT
  int32_t int_no;
  sl_gpio_t gpio_port_pin;
#if (defined(SLI_SI917))
  gpio_port_pin.port = (MIKROE_BMA400_INT1_PORT > 0)
                       ? MIKROE_BMA400_INT1_PORT
                       : (MIKROE_BMA400_INT1_PIN / 16);
  gpio_port_pin.pin = MIKROE_BMA400_INT1_PIN % 16;
  int_no = PIN_INTR_NO;
  sl_gpio_driver_configure_interrupt(&gpio_port_pin,
                                     int_no,
                                     SL_GPIO_INTERRUPT_RISING_EDGE,
                                     app_gpio_int_cb,
                                     AVL_INTR_NO);
#else // SLI_SI917
  gpio_port_pin.port = MIKROE_BMA400_INT1_PORT;
  gpio_port_pin.pin = MIKROE_BMA400_INT1_PIN;
  int_no = MIKROE_BMA400_INT1_PIN;
  sl_gpio_configure_external_interrupt(&gpio_port_pin,
                                       &int_no,
                                       SL_GPIO_INTERRUPT_RISING_EDGE,
                                       app_gpio_int_cb,
                                       NULL);
#endif // SLI_SI917
#endif // MIKROE_BMA400_READ_MODE_INTERRUPT

  return SL_STATUS_OK;
}

static sl_status_t app_bma400_read_data(void)
{
  int8_t rslt;
  float t, x, y, z;
  struct bma400_sensor_data accel;
  uint16_t int_status = 0;

  rslt = bma400_get_interrupt_status(&int_status, &bma);
  if (rslt != BMA400_OK) {
    return SL_STATUS_FAIL;
  }

  if (int_status & BMA400_ASSERTED_DRDY_INT) {
    rslt = bma400_get_accel_data(BMA400_DATA_SENSOR_TIME, &accel, &bma);
    if (rslt != BMA400_OK) {
      return SL_STATUS_FAIL;
    }

    /* 12-bit accelerometer at range 2G */
    x = lsb_to_ms2(accel.x, 2, 12);
    y = lsb_to_ms2(accel.y, 2, 12);
    z = lsb_to_ms2(accel.z, 2, 12);
    t = (float)accel.sensortime * SENSOR_TICK_TO_S;
    app_printf(
      "Gravity-x : %.2f, Gravity-y : %.2f, Gravity-z : %.2f, t(s) : %.4f\r\n",
      x,
      y,
      z,
      t);
  }

  return SL_STATUS_OK;
}

static float lsb_to_ms2(int16_t accel_data, uint8_t g_range, uint8_t bit_width)
{
  float accel_ms2;
  int16_t half_scale;

  half_scale = 1 << (bit_width - 1);
  accel_ms2 = (GRAVITY_EARTH * accel_data * g_range) / half_scale;

  return accel_ms2;
}
