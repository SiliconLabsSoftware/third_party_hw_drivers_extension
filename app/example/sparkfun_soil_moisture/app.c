/***************************************************************************//**
 * @file app.c
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
#include "sl_sleeptimer.h"
#include "sparkfun_soil_moisture.h"

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

#define MOISTURE_THRESHOLD           70
#define READING_INTERVAL_MSEC        1000

static volatile bool app_timer_expire = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t sc;
  uint16_t address[255];
  uint8_t num_dev = 0;

  app_printf("\r\t\tSEN-17731 Soil moisture sensor test program\n");

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  sc = sparkfun_soil_moisture_init(app_i2c_instance,
                                   SPARKFUN_SOIL_MOISTURE_DEFAULT_ADDR);
  if (sc == SL_STATUS_NOT_AVAILABLE) {
    app_printf("Soil moisture not found on the specified address.\r\r\n");
    app_printf("\r\nScanning address of all soil moisture sensors...\n");
    sparkfun_soil_moisture_scan_address(address, &num_dev);

    if (num_dev == 0) {
      app_printf("No device is found on I2C bus.\r\r\n");
      return;
    }

    for (int i = 0; i < num_dev; i++) {
      app_printf("\rDevice %d: address: 0x%x\n", i + 1, *(address + i));
    }

    app_printf("Select the desired device, build and rerun the example.\r\n");
    return;
  } else if (sc != SL_STATUS_OK) {
    app_printf("Warning! Failed to initialize soil moisture sensors\r\n");
    return;
  }

  app_printf("Soil moisture sensors initialized successfully!\r\n");

  app_printf("\r\nStart calibrating the sensor...\n");
  sparkfun_soil_moisture_set_dry_value(90);
  sparkfun_soil_moisture_set_wet_value(1023);
  app_printf("\rCalibrating done...\n");

  sc = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                             READING_INTERVAL_MSEC,
                                             app_timer_cb,
                                             NULL,
                                             0,
                                             1);
  if (sc != SL_STATUS_OK) {
    app_printf("\r\n > Start periodic measuring soil moisture Fail\n");
  } else {
    app_printf("\r\n > Start periodic measuring soil moisture...\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint8_t moisture;
  sl_status_t sc;

  if (app_timer_expire == false) {
    return;
  }

  app_timer_expire = false;
  sc = sparkfun_soil_moisture_get_moisture(&moisture);

  if (sc != SL_STATUS_OK) {
    app_printf("\r > Reading data failed\n");
  } else {
    app_printf("\r > Soil moisture: %d%%\n", moisture);
    if (moisture > MOISTURE_THRESHOLD) {
      sparkfun_soil_moisture_led_on();
    } else {
      sparkfun_soil_moisture_led_off();
    }
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
