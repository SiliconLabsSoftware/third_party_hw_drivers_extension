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
#include "mikroe_mic2.h"

#if (defined(SLI_SI917))
#include "sl_adc_instances.h"
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)          DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED        SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
static uint8_t channel = SL_ADC_CHANNEL_1;
#else
#include "em_iadc.h"
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...)          app_log(__VA_ARGS__)
#endif

#define READING_INTERVAL_MSEC    500

static mikroe_mic2_data_t adc_value;
static volatile bool mic2_timer_expire = false;
static sl_sleeptimer_timer_handle_t mic2_timer;
static mikroe_adc_handle_t app_adc_handle = NULL;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void mic2_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
  app_adc_handle = &channel;
  app_i2c_instance = &i2c_instance;
#else
  app_adc_handle = IADC0;
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  app_printf("Application Initialization.\n");
  sc = mikroe_mic2_init(app_i2c_instance, app_adc_handle);
#if (defined(SLI_SI917))

  /**
   * Due to calling trim_efuse API on ADC init in driver
   * it will change the clock frequency,
   * if we are not initialize the debug again
   * it will print the garbage data in console output.
   */
  DEBUGINIT();
#endif
  if (!sc) {
    app_printf("MIC 2 Click is initialized.\n");
  } else {
    app_printf("Status code: %lx\n", sc);
    app_printf("MIC 2 Click initialization is failed!\n");
  }
  mikroe_mic2_set_potentiometer(35);

  sl_sleeptimer_start_periodic_timer_ms(&mic2_timer,
                                        READING_INTERVAL_MSEC,
                                        mic2_sleeptimer_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * Simple timer callback function.
 ******************************************************************************/
static void mic2_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data)
{
  (void)timer;
  (void)data;

  mic2_timer_expire = true;
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t sc;
  float db_value;

  if (mic2_timer_expire) {
    mic2_timer_expire = false;
    sc = mikroe_mic2_generic_read(&adc_value);
    if (SL_STATUS_OK == sc) {
      db_value = (adc_value + 83.2073) / 11.003;
      app_printf("Sound intensity: %.2f dB.\n", db_value);
    } else {
      app_printf("Reading the ADC value is failing.!\n");
    }
  }
}
