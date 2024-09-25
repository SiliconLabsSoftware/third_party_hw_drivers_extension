/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "mikroe_accurrent.h"
#include "sl_sleeptimer.h"

#if (defined(SLI_SI917))
#include "sl_si91x_gspi.h"
#else
#include "sl_spidrv_instances.h"
#endif

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#else
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#if (defined(SLI_SI917))
static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#endif

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
                                        1000,
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
