/***************************************************************************//**
 * @file app.c
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "sparkfun_qwiic_joystick.h"
#include "sl_sleeptimer.h"

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

#define READING_INTERVAL_MSEC        200

static volatile bool read_position = false;
// Periodic timer handle.
static sl_sleeptimer_timer_handle_t app_periodic_timer;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;
  uint8_t address[255];
  uint8_t num_dev = 0;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  // Initialize joystick.
  sc = sparkfun_joystick_init(app_i2c_instance, SPARKFUN_JOYSTICK_DEFAULT_ADDR);

  if (sc == SL_STATUS_NOT_AVAILABLE) {
    app_printf("Joystick not found on the specified address.\r\r\n");
    app_printf("\r\nScanning address of all Joysticks...\n");
    sparkfun_joystick_scan_address(address, &num_dev);

    if (num_dev == 0) {
      app_printf("No device is found on I2C bus.\r\r\n");
      return;
    }

    for (int i = 0; i < num_dev; i++) {
      app_printf("\rDevice %d: address: 0x%x\n", i + 1, address[i]);
    }

    app_printf("Select the desired device, build and rerun the example.\r\n");
    return;
  } else if (sc != SL_STATUS_OK) {
    app_printf("Failed to initialize Joystick!\r\n");
    return;
  }

  app_printf("Joystick initialized successfully!\r\n");

  // Start timer used for periodic indications.
  sl_sleeptimer_start_periodic_timer_ms(&app_periodic_timer,
                                        READING_INTERVAL_MSEC,
                                        app_periodic_timer_cb,
                                        NULL,
                                        0,
                                        1);
  app_printf("\r\nStart reading Position\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t sc;
  uint16_t data_pos;
  uint8_t bt_pos;

  if (read_position == false) {
    return;
  }

  read_position = false;

  // Reading current horizontal position
  sc = sparkfun_joystick_read_horizontal_position(&data_pos);
  if (sc != SL_STATUS_OK) {
    app_printf("Warning! Invalid Joystick reading\r\n");
  } else {
    app_printf("X = %d, ", data_pos);
  }

  // Reading current vertical position
  sc = sparkfun_joystick_read_vertical_position(&data_pos);
  if (sc != SL_STATUS_OK) {
    app_printf("Warning! Invalid Joystick reading\r\n");
  } else {
    app_printf("Y = %d, ", data_pos);
  }

  // Read button postion
  sc = sparkfun_joystick_read_button_position(&bt_pos);
  if (sc != SL_STATUS_OK) {
    app_printf("Warning! Invalid Joystick reading\r\n");
  } else {
    app_printf("Button = %d\r\n", bt_pos);
  }
}

/**************************************************************************//**
 * Timer callback
 * Called periodically to reading data from joystick.
 *****************************************************************************/
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data)
{
  (void)data;
  (void)timer;

  read_position = true;
}
