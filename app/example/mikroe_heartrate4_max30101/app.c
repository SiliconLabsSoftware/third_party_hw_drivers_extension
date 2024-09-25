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

#include "sl_sleeptimer.h"
#include "mikroe_max30101_config.h"
#include "mikroe_max30101.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#else
#include "sl_i2cspm_instances.h"
#endif

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#else
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#include "sl_driver_gpio.h"
#else
#include "gpiointerrupt.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

// #define MIKROE_HEARTRATE4_MODE_INTERRUPT
#define MIKROE_HEARTRATE4_MODE_POLLING

#define READING_INTERVAL_MSEC        3000

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

mikroe_i2c_handle_t app_i2c_instance = NULL;

static volatile bool data_ready = false;

#ifdef MIKROE_HEARTRATE4_MODE_INTERRUPT
static void heartrate4_int_callback(uint8_t intNo)
{
  (void) intNo;

  data_ready = true;
}

#endif

#ifdef MIKROE_HEARTRATE4_MODE_POLLING
static sl_sleeptimer_timer_handle_t app_timer_handle;
static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  data_ready = true;
}

#endif

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#ifdef MIKROE_HEARTRATE4_MODE_INTERRUPT
#if (defined(SLI_SI917))
  sl_gpio_t gpio_port_pin = {
    MAX30101_INT_PIN / 16,
    MAX30101_INT_PIN % 16
  };
  sl_gpio_driver_configure_interrupt(&gpio_port_pin,
                                     GPIO_M4_INTR,
                                     SL_GPIO_INTERRUPT_RISING_EDGE | SL_GPIO_INTERRUPT_FALLING_EDGE,
                                     (void *)&heartrate4_int_callback,
                                     AVL_INTR_NO);
#else
  GPIO_PinModeSet(MAX30101_INT_PORT, MAX30101_INT_PIN, gpioModeInputPull, 1);
  GPIO_ExtIntConfig(MAX30101_INT_PORT,
                    MAX30101_INT_PIN,
                    MAX30101_INT_PIN,
                    false,
                    true,
                    true);
  GPIOINT_CallbackRegister(MAX30101_INT_PIN, heartrate4_int_callback);
#endif
#endif

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  if (mikroe_max30101_init(app_i2c_instance) == SL_STATUS_OK) {
    app_printf("MAX30101 init successfully\n");
  }
  sl_sleeptimer_delay_millisecond(2000);

#ifdef MIKROE_HEARTRATE4_MODE_INTERRUPT

  /*
   * In SpO2 and HR modes, this interrupt triggers when there is a new sample
   * in the data FIFO. The interrupt is cleared by reading the Interrupt Status
   * 1 register (0x00), or by reading the FIFO_DATA register.
   */
  mikroe_max30101_get_intrrupt(1);

  // Clear interrupt flag of FIFO almost full flag
  mikroe_max30101_get_red_val();
#endif

#ifdef MIKROE_HEARTRATE4_MODE_POLLING
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_cb,
                                        (void *) NULL,
                                        0,
                                        0);
#endif
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static uint32_t red_samp;

  if (data_ready == false) {
    return;
  }

  data_ready = false;
  // Only get data if MAX30101 is available on the bus
  if (SL_STATUS_OK != mikroe_max30101_present()) {
    app_printf("Error: Unable to check MAX30101 is present on the bus.\r\n");
    return;
  }

  if (mikroe_max30101_get_intrrupt(1) & 0x40) {
    red_samp = mikroe_max30101_get_red_val();
    // If sample pulse amplitude is not under threshold value 0x8000
    if (red_samp > 0x8000) {
      app_printf("%lu\r\n", red_samp);
    } else {
      app_printf("Place Finger On Sensor\r\n");
    }
  }
}
