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

#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "mikroe_stretch.h"

#if (defined(SLI_SI917))
#include "sl_adc_instances.h"
#include "rsi_debug.h"
#else
#include "em_iadc.h"
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
static uint8_t channel = SL_ADC_CHANNEL_1;
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

static volatile bool stretch_timer_expire = false;
static sl_sleeptimer_timer_handle_t stretch_timer;
static mikroe_adc_handle_t app_adc_handle = NULL;

static void stretch_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t status;
  app_printf("Application Init\n");

#if (defined(SLI_SI917))
  app_adc_handle = &channel;
#else
  app_adc_handle = IADC0;
#endif
  status = mikroe_stretch_init(app_adc_handle);
#if (defined(SLI_SI917))

  /**
   * Due to calling trim_efuse API on ADC init in driver
   * it will change the clock frequency,
   * if we are not initialize the debug again
   * it will print the garbage data in console output.
   */
  DEBUGINIT();
#endif
  if (status != SL_STATUS_OK) {
    app_printf("Application Init Error!\n");
    app_printf("Please, run program again ...\n");

    for (;;) {}
  }
  app_printf("Stretch Click is initialized.\n");
  sl_sleeptimer_start_periodic_timer_ms(&stretch_timer,
                                        500,
                                        stretch_sleeptimer_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint16_t stretch_an_value = 0;

  if (stretch_timer_expire == true) {
    stretch_timer_expire = false;
    if (mikroe_stretch_generic_read(&stretch_an_value) == SL_STATUS_OK) {
      app_printf("ADC Value: %u\r\n", stretch_an_value);
      if (stretch_an_value < 500) {
        mikroe_stretch_turn_on_led();
      } else {
        mikroe_stretch_turn_off_led();
      }
      app_printf("Resistance Value: %u\r\n", stretch_an_value);
    }
  }
}

/***************************************************************************//**
 * Sleeptimer callback function.
 ******************************************************************************/
static void stretch_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                        void *data)
{
  (void)timer;
  (void)data;

  stretch_timer_expire = true;
}
