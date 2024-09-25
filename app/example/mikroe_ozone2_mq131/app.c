/***************************************************************************//**
 * @file app.c
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
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
 * # Experimental Quality
 * This code has not been formally tested and is provided as-is. It is not
 * suitable for production environments. In addition, this code will not be
 * maintained and there may be no bug maintenance planned for these resources.
 * Silicon Labs may update projects from time to time.
 ******************************************************************************/

#include "sl_sleeptimer.h"
#include "mikroe_mq131.h"

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

#define READING_INTERVAL_MSEC 1000

#if (defined(SLI_SI917))
#if MIKROE_OZONE2_ADC_SEL == 0
static uint8_t channel = SL_ADC_CHANNEL_1;
#else
static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#endif
#endif

static mikroe_adc_handle_t app_adc_instance = NULL;
static mikroe_spi_handle_t app_spi_instance = NULL;
static volatile bool app_timer_expire = false;
static sl_sleeptimer_timer_handle_t app_timer;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *timer, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
#if MIKROE_OZONE2_ADC_SEL == 0
  app_adc_instance = &channel;
#else
  app_spi_instance = &gspi_instance;
#endif

#else
#if MIKROE_OZONE2_ADC_SEL == 0
  app_adc_instance = IADC0;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

#endif

  app_printf("---- Application Init ----\r\n");
  sc = mikroe_ozone2_init(app_spi_instance, app_adc_instance);

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
  sl_sleeptimer_start_periodic_timer_ms(&app_timer,
                                        READING_INTERVAL_MSEC,
                                        app_timer_cb,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint16_t o3_ppm;

  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;

  if (mikroe_ozone2_read_measurement(&o3_ppm) == SL_STATUS_OK) {
    app_printf("  O3 [ppm] : %u\r\n", o3_ppm);
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
