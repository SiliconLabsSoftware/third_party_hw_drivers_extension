/***************************************************************************//**
 * @file app.c
 * @brief Example application
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
#include "app.h"

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

static volatile bool new_keypad_event = false;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

#ifdef SAPRKFUN_KEYPAD_INT_PIN_EN

/***************************************************************************//**
 * Callback for the interrupt.
 ******************************************************************************/
void app_sparkfun_buttonEvent_callback(const uint8_t pin)
{
  (void)pin;
  new_keypad_event = true;
}

#else //SAPRKFUN_KEYPAD_INT_PIN_EN

static sl_sleeptimer_timer_handle_t button_read_timer;

/***************************************************************************//**
 * Callback for the timer.
 ******************************************************************************/
static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;
  new_keypad_event = true;
}

/***************************************************************************//**
 * Initialize the sleep timer.
 ******************************************************************************/
void app_init_sleeptimer(void)
{
  sl_status_t error_code;

  error_code = sl_sleeptimer_start_periodic_timer_ms(&button_read_timer,
                                                     BUTTON_EVENT_CHECK_FREQUENCY_MS,
                                                     app_timer_callback,
                                                     (void *)NULL,
                                                     0,
                                                     0);
  if (error_code == SL_STATUS_OK) {
    app_printf("Sleep timer initialized successfully.\r\n");
  } else if (error_code == SL_STATUS_NULL_POINTER) {
    app_printf("Error, invslid sleeptimer handle.\r\n");
  } else if (error_code == SL_STATUS_INVALID_STATE) {
    app_printf("Error, sleeptimer already running.\r\n");
  } else if (error_code == SL_STATUS_INVALID_PARAMETER) {
    app_printf("Error, invalid sleeptimer time_out parameter.\r\n");
  }
}

#endif //SAPRKFUN_KEYPAD_INT_PIN_EN

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_init_keypad();

#ifndef SAPRKFUN_KEYPAD_INT_PIN_EN
  app_init_sleeptimer();
#endif //SAPRKFUN_KEYPAD_INT_PIN_EN
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (new_keypad_event == true) {
    new_keypad_event = false;
    app_handle_new_button();
  }
}

/***************************************************************************//**
 * Initialize the keypad.
 ******************************************************************************/
void app_init_keypad(void)
{
  sl_status_t status;
  uint8_t address[255];
  uint8_t num_dev = 0;
  frw_rev_t fw;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

#ifdef SAPRKFUN_KEYPAD_INT_PIN_EN
  status = sparkfun_keypad_init(app_i2c_instance, SPARKFUN_KEYPAD_DEFAULT_ADDR,
                                &app_sparkfun_buttonEvent_callback);
#else
  status = sparkfun_keypad_init(app_i2c_instance,
                                SPARKFUN_KEYPAD_DEFAULT_ADDR,
                                NULL);
#endif

  if (status == SL_STATUS_NOT_AVAILABLE) {
    app_printf("Keypad not found on the specified address.\r\r\n");
    app_printf("\r\nScanning address of all keypads...\n");
    status = sparkfun_keypad_scan_address(address, &num_dev);

    if (num_dev == 0) {
      app_printf("No device is found on I2C bus.\r\r\n");
      return;
    }

    for (int i = 0; i < num_dev; i++) {
      app_printf("\rDevice %d: address: 0x%x\n", i + 1, address[i]);
    }

    app_printf("Select the desired device, build and rerun the example.\r\n");
    return;
  } else if (status != SL_STATUS_OK) {
    app_printf("Failed to initialize Keypad!\r\n");
    return;
  }

  app_printf("Keypad initialized successfully!\r\n");

  if (SL_STATUS_OK == sparkfun_keypad_get_firmware_version(&fw)) {
    app_printf("Keypad FW: %02d.%02d\r\n", fw.major, fw.minor);
  }
}

/***************************************************************************//**
 * This function handles the interfacing with the keypad.
 ******************************************************************************/
void app_handle_new_button(void)
{
  uint8_t new_btn = 0;
  sparkfun_keypad_update_fifo();
  sparkfun_keypad_read_last_button(&new_btn);
  if (new_btn > 0) {
    app_printf("The last pressed button: %c\r\n", new_btn);
  }
}
