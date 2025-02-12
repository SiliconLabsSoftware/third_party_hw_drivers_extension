/*******************************************************************************
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
#include "sparkfun_bmi270.h"
#include "app_assert.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)

#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;

#else /* None Si91x device */
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

typedef struct {
  double accelerometer[3]; ///< accelerometer values: (x, y, z) axis
  double gyroscope[3]; ///< gyroscope values: (x, y, z) axis
  double temperature;
} bmi270_data_t;

#define READING_INTERVAL_MSEC    500

///< To store BMI270 configuration.
static bmi270_cfg_data_t bmi_cfg_data;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);
static sl_status_t app_bmi270_init(void);

// *****************************************************************************
// * Initialize application.
// *****************************************************************************
void app_init(void)
{
  app_printf("-----------------------\r\n");
  app_printf("Sparkfun BMI270 - 6DOF IMU Sensor\r\n");
  app_printf("Application Task\r\n");
  app_printf("-----------------------\r\n");

  if (app_bmi270_init() != SL_STATUS_OK) {
    app_printf("BMI270 inits failed\r\n");
    app_printf("-----------------------\r\n");
  } else {
    app_printf("BMI270 inits successfully\r\n");
    app_printf("-----------------------\r\n");
    app_printf("Start measure\r\n");
    app_printf("-----------------------\r\n");
  }
}

// *****************************************************************************
// * App ticking function.
// *****************************************************************************
void app_process_action(void)
{
  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;

  bmi270_data_t bmi270_data;
  sl_status_t ret;

  ret = sparkfun_bmi270_read_acc_reading(&bmi_cfg_data,
                                         bmi270_data.accelerometer);
  if (SL_STATUS_OK == ret) {
    app_printf("Acc value: x[%0.4f] - y[%0.4f] - z[%0.4f]\r\n",
               bmi270_data.accelerometer[0],
               bmi270_data.accelerometer[1],
               bmi270_data.accelerometer[2]);
  }

  ret = sparkfun_bmi270_read_gyro_reading(&bmi_cfg_data, bmi270_data.gyroscope);
  if (SL_STATUS_OK == ret) {
    app_printf("Gyr value: x[%0.4f] - y[%0.4f] - z[%0.4f]\r\n",
               bmi270_data.gyroscope[0],
               bmi270_data.gyroscope[1],
               bmi270_data.gyroscope[2]);
  }

  ret = sparkfun_bmi270_read_temp_reading(&bmi_cfg_data,
                                          &bmi270_data.temperature);
  if (SL_STATUS_OK == ret) {
    app_printf("Temp value: %0.4f%cC\r\n", bmi270_data.temperature, 176);
  }
  app_printf("-----------------------------------------\r\n");
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}

static sl_status_t app_bmi270_init(void)
{
  sl_status_t ret = SL_STATUS_OK;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  /// Initializing I2C instance and slave address
  bmi_cfg_data.i2c_instance = app_i2c_instance;
  bmi_cfg_data.bmi270_slave_address = BMI270_ADDR;

  /// Initializing IMU sensor configuration

  /// append ORing of all the needed features of bmi270
  bmi_cfg_data.enable_features.features = BMI270_FEATURE_ACC
                                          | BMI270_FEATURE_GYRO
                                          | BMI270_FEATURE_TEMP;

  /// append the various configuration of accelerometer
  bmi_cfg_data.acc_config.bw = BMI270_ACCEL_NORM_AVG4;
  bmi_cfg_data.acc_config.odr = BMI270_ACCEL_ODR_100HZ;
  bmi_cfg_data.acc_config.filter = BMI270_ACCEL_PERFO_OPT;
  bmi_cfg_data.acc_config.range = BMI270_ACCEL_RANGE_2G;

  /// append the various configuration of gyroscope
  bmi_cfg_data.gyro_config.bw = BMI270_GYRO_BWP_NORMAL;
  bmi_cfg_data.gyro_config.odr = BMI270_GYRO_ODR_200HZ;
  bmi_cfg_data.gyro_config.filter = BMI270_GYRO_F_PERFO_OPT;
  bmi_cfg_data.gyro_config.range = BMI270_GYRO_RANGE_2000DPS;
  bmi_cfg_data.gyro_config.noise = BMI270_GYRO_N_POWER_OPT;

  ret = sparkfun_bmi270_init(&bmi_cfg_data);
  app_assert_status(ret);

  ret = sparkfun_bmi270_enable_and_config_features(&bmi_cfg_data);
  app_assert_status(ret);

  // Start a periodic timer READING_INTERVAL_MSEC ms to read data from the
  //   sensor
  ret = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                              READING_INTERVAL_MSEC,
                                              app_timer_cb,
                                              (void *) NULL,
                                              0,
                                              0);
  app_assert_status(ret);
  return ret;
}
