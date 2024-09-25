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
#include "mikroe_ml8511a.h"
#include "mikroe_ml8511a_config.h"

#if (defined(SLI_SI917))
#include "sl_adc_instances.h"
#include "sl_si91x_gspi.h"
#include "rsi_debug.h"
#else
#include "em_iadc.h"
#include "sl_spidrv_instances.h"
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#define READING_INTERVAL_MSEC       1000

#if (defined(SLI_SI917))
static uint8_t channel = SL_ADC_CHANNEL_1;
static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#endif

static mikroe_adc_handle_t app_adc_instance = NULL;
static mikroe_spi_handle_t app_spi_instance = NULL;
static volatile bool uv_timer_expire = false;
static sl_sleeptimer_timer_handle_t uv_timer;

static void uv_timer_cb(sl_sleeptimer_timer_handle_t *timer, void *data);
static void application_callback(char *message);

void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
  app_adc_instance = &channel;
  app_spi_instance = &gspi_instance;
#else
  app_adc_instance = IADC0;
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

  app_printf("---- Application Init ----\r\n");

  sc = mikroe_ml8511a_init(app_spi_instance, app_adc_instance);

#if (defined(SLI_SI917))

  /**
   * Due to calling trim_efuse API on ADC init in driver
   * it will change the clock frequency,
   * if we are not initialize the debug again
   * it will print the garbage data in console output.
   */
  DEBUGINIT();
#endif

  if (sc != SL_STATUS_OK) {
    app_printf("---- Application Init Error ----");
    app_printf("---- Please, run program again ----");
    for ( ; ; ) {}
  }
  mikroe_ml8511a_set_callback_handler(application_callback);
  mikroe_ml8511a_device_enable();
  sl_sleeptimer_delay_millisecond(1000);
  app_printf("---- Application Init Done ----\r\n");
  sl_sleeptimer_start_periodic_timer_ms(&uv_timer,
                                        READING_INTERVAL_MSEC,
                                        uv_timer_cb,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static float uv_voltage = 0;
  static uint8_t uv_index = 0;

  if (uv_timer_expire == false) {
    return;
  }
  uv_timer_expire = false;

#if MIKROE_ML8511A_OPERATION_MODE == ADC_MODE
  if (mikroe_ml8511a_read_adc_voltage(&uv_voltage) == SL_STATUS_OK) {
    mikroe_ml8511a_calc_index(uv_voltage, &uv_index);
  }
#elif MIKROE_ML8511A_OPERATION_MODE == AN_MODE
  mikroe_ml8511a_read_an_pin_voltage(&uv_voltage);
  mikroe_ml8511a_calc_index(uv_voltage, &uv_index);
#endif

  app_printf(" UV Index [0-15] : %u\r\n", (uint16_t)uv_index);
  app_printf(" UV ADC Voltage [V] : %.2f\r\n", uv_voltage);
  app_printf("------------------------------\r\n");
}

static void application_callback(char *message)
{
  app_printf("*** %s ***\r\n", message);
}

static void uv_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  uv_timer_expire = true;
}
