/***************************************************************************//**
 * @file app.c
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
#include "mikroe_ad8318.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_si91x_gspi.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)

static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#else /* None Si91x device */
#include "app_log.h"
#include "sl_spidrv_instances.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

#define READING_INTERVAL_MSEC        1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;
static mikroe_spi_handle_t app_spi_instance = NULL;

static void application_task(void);
static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

  app_printf("Application initialization.\r\n");

#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

  sc = mikroe_ad8318_init(app_spi_instance);
  if (sc == SL_STATUS_OK) {
    app_printf("RF Meter Click is initialized.\r\n");
  } else {
    app_printf("RF Meter Click initialization is failed!\r\n");
  }

  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_cb,
                                        (void *) NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (app_timer_expire == false) {
    return;
  }

  app_timer_expire = false;
  application_task();
}

/***************************************************************************//**
 * Application task.
 ******************************************************************************/
static void application_task(void)
{
  sl_status_t sc;
  float signal;

  sc = mikroe_ad8318_get_signal_strength(MIKROE_AD8318_DEF_SLOPE,
                                         MIKROE_AD8318_DEF_INTERCEPT,
                                         &signal);
  if (sc == SL_STATUS_OK) {
    app_printf("Signal strength: %.2f dBm\r\n", signal);
  } else {
    app_printf("Reading signal strength is failed!\r\n");
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
