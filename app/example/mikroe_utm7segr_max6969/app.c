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
#include "mikroe_max6969.h"
#include "sl_sleeptimer.h"
#include "sl_pwm_instances.h"
#include "app_assert.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_si91x_gspi.h"

#define app_printf(...) DEBUGOUT(__VA_ARGS__)

static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#else /* None Si91x device */
#include "app_log.h"
#include "sl_spidrv_instances.h"

#define app_printf(...) app_log(__VA_ARGS__)
#endif

#define TIMEOUT_MS      1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;
mikroe_pwm_handle_t app_pwm_instance = NULL;
static mikroe_spi_handle_t app_spi_instance = NULL;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_printf("UT-M 7-SEG R Click Example\r\n");
#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
  app_pwm_instance = &sl_pwm_channel_0_config;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
  app_pwm_instance = &sl_pwm_mikroe;
#endif
  sl_status_t stt = mikroe_max6969_init(app_spi_instance, app_pwm_instance);
  app_assert_status(stt);

  mikroe_max6969_display_number(0x00, MIKROE_UTM7SEGR_NO_DOT);
  app_printf("UT-M 7-SEG R Test contrast:\r\n");
  app_printf("Value(%%):");

  for (uint8_t i = 0; i < 10; i++)
  {
    uint8_t contrast_value = (i + 1) * 10;
    app_printf(" %02d", contrast_value);
    mikroe_max6969_set_contrast(contrast_value);

    sl_sleeptimer_delay_millisecond(1000);
  }

  app_printf("\r\nUT-M 7-SEG R set contrast default: 50%%\r\n");
  mikroe_max6969_set_contrast(50);

  app_printf("Start periodic timer %d(ms)\r\n", TIMEOUT_MS);
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        TIMEOUT_MS,
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
  static uint8_t cnt = 0;

  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;
  app_printf("mikroe_max6969_display_number: %d\r\n", cnt);
  mikroe_max6969_display_number(cnt, MIKROE_UTM7SEGR_NO_DOT);
  cnt++;
  if (cnt >= 100) {
    cnt = 0;
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
