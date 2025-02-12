/***************************************************************************//**
 * @file
 * @brief VL53L1X Test application.
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "sl_sleeptimer.h"
#include "sparkfun_vl53l1x.h"
#include "sparkfun_vl53l1x_config.h"

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

// This define enables the periodic distance printing via UART
#define VL53_TEST_APP_PERIODIC_DISTANCE
#define READING_INTERVAL_MSEC        1000

static volatile bool trigger_process = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void vl53_read_distance(void);
static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                               void *data);

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
void app_init(void)
{
  uint8_t sensor_state = 0;
  sl_status_t vl53_status = SL_STATUS_OK;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  app_printf("==== VL53L1X Test application ====\n");
  app_printf("app_init function called...\n");

  // Initialize the sensor with the default settings
  vl53_status = vl53l1x_init(VL53L1X_ADDR, app_i2c_instance);

  // Waiting for device to boot up...
  while (0 == sensor_state) {
    // Read sensor's state (0 = boot state, 1 = device is booted )
    vl53_status = vl53l1x_get_boot_state(VL53L1X_ADDR, &sensor_state);

    if (SL_STATUS_OK != vl53_status) {
      break;
    }

    // Wait for 2 ms
    sl_sleeptimer_delay_millisecond(2);
  }

  if (SL_STATUS_OK == vl53_status) {
    app_printf("Platform I2C communication is OK.\n");
  } else {
    app_printf("Platform I2C communication test has been failed.\n"
               "Please check the I2C bus connection and "
               "the I2C (I2CSPM) configuration.\n");
    return;
  }

  app_printf("VL53L1X booted\n");

  // Optional sensor configuration example function calls, see API documentation
  //   for options
  {
    // Select distance mode
    vl53_status = vl53l1x_set_distance_mode(VL53L1X_ADDR,
                                            VL53L1X_DISTANCE_MODE_LONG);
    // in ms possible values [20,50,100,200,500]
    vl53_status = vl53l1x_set_timing_budget_in_ms(VL53L1X_ADDR, 100);

    // in ms, IM must be >= TB
    vl53_status = vl53l1x_set_inter_measurement_in_ms(VL53L1X_ADDR, 200);

    vl53_status = vl53l1x_set_roi_xy(VL53L1X_ADDR, 16, 16); // min. ROI is 4,4
  }

  // Check return codes of the optional configuration function calls
  if (SL_STATUS_OK == vl53_status) {
    app_printf("Sensor initialization and configuration are done.\n");
  } else {
    app_printf("Sensor initialization and configuration has been failed.\n");
    return;
  }

  // Start ranging
  vl53_status = vl53l1x_start_ranging(VL53L1X_ADDR);

  // Check ranging status
  if (SL_STATUS_OK == vl53_status) {
    app_printf("VL53L1X ranging has been started ...\n");

    // Get measurement result
    vl53_read_distance();
  } else {
    app_printf("Start ranging has been failed.\n");
    return;
  }

#if defined(VL53_TEST_APP_PERIODIC_DISTANCE)
  // Setup a periodic sleep timer with 1000 ms time period
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_callback,
                                        NULL,
                                        0,
                                        0);
  app_printf("Periodic timer is configured to send periodic distance data.\n");
#endif

  app_printf("==================================\n");

#if defined(VL53_TEST_APP_PERIODIC_DISTANCE)
  app_printf("> Periodic measurement:\n");
#endif
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
void app_process_action(void)
{
  if (trigger_process) {
    trigger_process = false;
    vl53_read_distance();
  }
}

static void vl53_read_distance(void)
{
  sl_status_t vl53_status = SL_STATUS_OK;
  uint8_t is_data_ready = 0;
  vl53l1x_result_t result;

  // Check measurement data status
  vl53_status = vl53l1x_check_for_data_ready(VL53L1X_ADDR, &is_data_ready);

  if (0 != is_data_ready) {
    // Measurement data is ready to read from the sensor
    vl53_status = vl53l1x_get_result(VL53L1X_ADDR, &result);

    // Print result
    app_printf(" > Distance: %4u mm, Range status: %1u, "
               "Signal rate: %5u, Ambient rate: %5u, "
               "Number of SPADS: %4u\n", result.distance, result.status,
               result.signal_per_spad, result.ambient, result.signal_per_spad);

    // Clear sensor's interrupt status
    vl53_status = vl53l1x_clear_interrupt(VL53L1X_ADDR);
  }

  if (SL_STATUS_OK != vl53_status) {
    app_printf("VL53L1X sensor operation has been failed "
               "during the periodic distancing.\n");
  }
}

static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
