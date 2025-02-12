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

#include "sl_sleeptimer.h"
#include "mikroe_pca9685.h"

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

#define INTERVAL_MSEC                10

static mikroe_i2c_handle_t app_i2c_instance = NULL;
static mikroe_servo_pos_and_res_t pos_res_setting =
{
  .min_position = 0,
  .max_position = 180,
  .low_resolution = 0,
  .high_resolution = 330,
};
static sl_sleeptimer_timer_handle_t servo_position_handle;
static int16_t count;
static bool counter_clockwise = true;
static volatile bool servo_position_expire = false;
static volatile bool adjust_position = false;

static void servo_position_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif
  app_printf("Application Initialization.\n");
  sc = mikroe_pca9685_init(app_i2c_instance);

  if (sc == SL_STATUS_OK) {
    app_printf("Mikroe Servo Click is initialized.\n");
  } else {
    app_printf("Mikroe Servo Click initialization failed.\n");
  }

  mikroe_pca9685_default_cfg();
  mikroe_pca9685_setting(&pos_res_setting);

  sl_sleeptimer_start_periodic_timer_ms(&servo_position_handle,
                                        INTERVAL_MSEC,
                                        servo_position_cb,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (servo_position_expire == false) {
    return;
  }
  servo_position_expire = false;
  if (counter_clockwise) {
    app_printf("----------------------------\n");
    app_printf("------Counter clockwise-----\n");

    count = pos_res_setting.min_position;
    while (count <= pos_res_setting.max_position) {
      if (adjust_position) {
        mikroe_pca9685_set_position(MIKROE_SERVO_MOTOR_1, count);
        app_printf("Position: %u\n", (uint16_t)count);
        count++;
        adjust_position = false;
      }
    }
  } else {
    app_printf("---------------------------\n");
    app_printf("---------Clockwise---------\n");

    count = pos_res_setting.max_position;
    while (count >= pos_res_setting.min_position) {
      if (adjust_position) {
        mikroe_pca9685_set_position(MIKROE_SERVO_MOTOR_1, count);
        app_printf("Position: %u\n", (uint16_t)count);
        count--;
        adjust_position = false;
      }
    }
  }
  counter_clockwise = !counter_clockwise;
}

/***************************************************************************//**
 * Sleep timer callback function.
 ******************************************************************************/
static void servo_position_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;

  servo_position_expire = true;
  adjust_position = true;
}
