/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
#include "mikroe_accurrent.h"
#include "sl_sleeptimer.h"

#if (defined(SLI_SI917))
#include "sl_si91x_gspi.h"
#include "rsi_debug.h"

#define app_printf(...)          DEBUGOUT(__VA_ARGS__)

static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#else
#include "sl_spidrv_instances.h"
#include "app_log.h"

#define app_printf(...)          app_log(__VA_ARGS__)
#endif

#define READING_INTERVAL_MSEC    200

static mikroe_spi_handle_t app_spi_instance = NULL;
static volatile uint8_t timer_flag;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                               void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

  sc = mikroe_accurrent_init(app_spi_instance);
  if (sc != SL_STATUS_OK) {
    app_printf("Initialization error. Please check again ...\r\n");
    return;
  }

  app_printf("Initialization successfully.\r\n");
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_callback,
                                        NULL, 0, 0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (timer_flag) {
    timer_flag = 0;
    app_printf("Current value: %.3f mA\r\n", mikroe_accurrent_get_ma());
  }
}

/***************************************************************************//**
 * Static function implementation.
 ******************************************************************************/
static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                               void *data)
{
  (void) handle;
  (void) data;

  timer_flag = 1;
}
