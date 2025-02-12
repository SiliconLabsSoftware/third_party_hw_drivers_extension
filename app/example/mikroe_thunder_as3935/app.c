/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "mikroe_thunder_as3935.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_si91x_ssi.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)

static sl_ssi_instance_t ssi_instance = SL_SSI_PRIMARY_ACTIVE;
#else /* None Si91x device */
#include "app_log.h"
#include "sl_spidrv_instances.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

#define READING_INTERVAL_MSEC        1000

static volatile bool flag = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static mikroe_spi_handle_t app_spi_instance = NULL;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle,
                         void *data);

void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
  app_spi_instance = &ssi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

  app_printf("---- Application Init ----\r\n");

  sc = mikroe_thunder_as3935_init(app_spi_instance);

  if (sc != SL_STATUS_OK) {
    app_printf("---- Application Init Error ----");
    app_printf("---- Please, run program again ----");
    for ( ; ; ) {}
  }
  mikroe_thunder_as3935_default_cfg();
  app_printf("---- Application Init Done ----\r\n");
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
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
  uint8_t storm_mode;
  uint32_t storm_energy;
  uint8_t storm_distance;

  if (flag) {
    storm_mode = mikroe_thunder_as3935_check_int();

    if (THUNDER_NOISE_LEVEL_INTERR == storm_mode) {
      app_printf("Noise level too high\r\n\n");
    } else if (THUNDER_DISTURBER_INTERR == storm_mode) {
      app_printf("Disturber detected\r\n\n");
    } else if (THUNDER_LIGHTNING_INTERR == storm_mode) {
      mikroe_thunder_as3935_get_storm_info(&storm_energy, &storm_distance);
      app_printf("Energy of the single lightning : %lu\r\n", storm_energy);
      app_printf("Distance estimation : %u km\r\n\n",
                 ( uint16_t ) storm_distance);
      // Reset configuration to prepare for the next measurement
      mikroe_thunder_as3935_default_cfg();
    }
    flag = false;
  }
}

void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  flag = true;
}
