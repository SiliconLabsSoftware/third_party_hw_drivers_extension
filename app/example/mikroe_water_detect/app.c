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
#include "mikroe_water_detect.h"
#include "mikroe_water_detect_config.h"

#if (defined(SLI_SI917))
#include "sl_driver_gpio.h"
#include "rsi_debug.h"

#define PIN_INTR_NO                  PIN_INTR_0
#define AVL_INTR_NO                  0 // available interrupt number
#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#else // SLI_SI917
#include "sl_gpio.h"
#include "app_log.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif // SLI_SI917

#define READING_INTERVAL_MSEC        200

#define MIKROE_WATER_DETECT_MODE_INTERRUPT
// #define MIKROE_WATER_DETECT_MODE_POLLING

static volatile uint8_t wd_state = 0;
static uint8_t wd_state_old = 0;

#ifdef MIKROE_WATER_DETECT_MODE_INTERRUPT
#if (defined(SLI_SI917))
static void wd_int_callback(uint32_t int_no)
{
#else // SLI_SI917
static void wd_int_callback(uint8_t int_no, void *ctx)
{
  (void)ctx;
#endif // SLI_SI917
  (void)int_no;
  wd_state = mikroe_water_detect_get_status();
}

#endif // MIKROE_WATER_DETECT_MODE_INTERRUPT

#ifdef MIKROE_WATER_DETECT_MODE_POLLING
static sl_sleeptimer_timer_handle_t wd_timer_handle;
static void wd_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;
  wd_state = mikroe_water_detect_get_status();
}

#endif // MIKROE_WATER_DETECT_MODE_POLLING

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  mikroe_water_detect_init();

#ifdef MIKROE_WATER_DETECT_MODE_POLLING
  sl_sleeptimer_start_periodic_timer_ms(&wd_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        wd_timer_callback,
                                        (void *) NULL,
                                        0,
                                        0);
#endif // MIKROE_WATER_DETECT_MODE_POLLING

#ifdef MIKROE_WATER_DETECT_MODE_INTERRUPT
  int32_t int_no;
  sl_gpio_t gpio_port_pin;
#if (defined(SLI_SI917))
  gpio_port_pin.port = (WATER_DETECT_INT_PORT > 0)
                       ? WATER_DETECT_INT_PORT : (WATER_DETECT_INT_PIN / 16);
  gpio_port_pin.pin = WATER_DETECT_INT_PIN % 16;
  int_no = PIN_INTR_NO;
  sl_gpio_driver_configure_interrupt(&gpio_port_pin,
                                     int_no,
                                     SL_GPIO_INTERRUPT_RISING_EDGE
                                     | SL_GPIO_INTERRUPT_FALLING_EDGE,
                                     wd_int_callback,
                                     AVL_INTR_NO);
#else // SLI_SI917
  gpio_port_pin.port = WATER_DETECT_INT_PORT;
  gpio_port_pin.pin = WATER_DETECT_INT_PIN;
  int_no = WATER_DETECT_INT_PIN;
  sl_gpio_configure_external_interrupt(&gpio_port_pin,
                                       &int_no,
                                       SL_GPIO_INTERRUPT_RISING_FALLING_EDGE,
                                       wd_int_callback,
                                       NULL);
#endif // SLI_SI917
#endif // MIKROE_WATER_DETECT_MODE_INTERRUPT
  app_printf("  Initialization Driver   \r\n");
  app_printf("------------------------- \r\n");
  app_printf("  Wait to detect water... \r\n");
  app_printf("------------------------- \r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (wd_state > wd_state_old) {
    app_printf(" >  Water is detected  < \r\n");
    app_printf("------------------------- \r\n");
    wd_state_old = 1;
  }

  if (wd_state < wd_state_old) {
    app_printf(" There is no water \r\n");
    app_printf("------------------- \r\n");
    wd_state_old = 0;
  }
}
