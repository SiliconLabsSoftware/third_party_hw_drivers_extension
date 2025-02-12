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
#include "rsi_debug.h"
#include "sl_driver_gpio.h"

#define PIN_INTR_NO               PIN_INTR_0
#define AVL_INTR_NO               0 // available interrupt number
#define app_printf(...)           DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED         SL_I2C2

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "sl_gpio.h"
#include "app_log.h"

#define app_printf(...)           app_log(__VA_ARGS__)
#endif

#define MIKROE_HEARTRATE4_MODE_INTERRUPT
// #define MIKROE_HEARTRATE4_MODE_POLLING
#define READING_INTERVAL_MSEC     500

mikroe_i2c_handle_t app_i2c_instance = NULL;

static volatile bool data_ready = false;

#ifdef MIKROE_HEARTRATE4_MODE_INTERRUPT
#if (defined(SLI_SI917))
static void heartrate4_int_callback(uint32_t int_no)
{
#else // SLI_SI917
static void heartrate4_int_callback(uint8_t int_no, void *ctx)
{
  (void)ctx;
#endif // SLI_SI917
  (void)int_no;

  data_ready = true;
}

#endif // MIKROE_HEARTRATE4_MODE_INTERRUPT

#ifdef MIKROE_HEARTRATE4_MODE_POLLING
static sl_sleeptimer_timer_handle_t app_timer_handle;
static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  data_ready = true;
}

#endif // MIKROE_HEARTRATE4_MODE_POLLING

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#ifdef MIKROE_HEARTRATE4_MODE_INTERRUPT
  int32_t int_no;
  sl_gpio_t gpio_port_pin;
#if (defined(SLI_SI917))
  gpio_port_pin.port = (MAX30101_INT_PORT > 0)
                       ? MAX30101_INT_PORT
                       : (MAX30101_INT_PIN / 16);
  gpio_port_pin.pin = MAX30101_INT_PIN % 16;
  int_no = PIN_INTR_NO;
  sl_gpio_driver_configure_interrupt(&gpio_port_pin,
                                     int_no,
                                     SL_GPIO_INTERRUPT_FALLING_EDGE,
                                     heartrate4_int_callback,
                                     AVL_INTR_NO);
#else // SLI_SI917
  gpio_port_pin.port = MAX30101_INT_PORT;
  gpio_port_pin.pin = MAX30101_INT_PIN;
  int_no = MAX30101_INT_PIN;
  sl_gpio_configure_external_interrupt(&gpio_port_pin,
                                       &int_no,
                                       SL_GPIO_INTERRUPT_FALLING_EDGE,
                                       heartrate4_int_callback,
                                       NULL);
#endif // SLI_SI917
#endif // MIKROE_HEARTRATE4_MODE_INTERRUPT

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else // SLI_SI917
  app_i2c_instance = sl_i2cspm_mikroe;
#endif // SLI_SI917

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
#endif // MIKROE_HEARTRATE4_MODE_INTERRUPT

#ifdef MIKROE_HEARTRATE4_MODE_POLLING
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_cb,
                                        (void *) NULL,
                                        0,
                                        0);
#endif // MIKROE_HEARTRATE4_MODE_POLLING
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint32_t red_samp;
  static uint8_t counter = 200;

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
    counter++;
    // If sample pulse amplitude is not under threshold value 0x8000
    if (red_samp > 0x8000) {
      app_printf("%lu\r\n", red_samp);
      counter = 200;
    } else if (counter > 200) {
      app_printf("Place Finger On Sensor\r\n");
      counter = 0;
    }
  }
}
