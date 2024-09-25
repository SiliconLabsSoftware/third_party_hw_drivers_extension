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
 * # Experimental Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include "mikroe_ph2.h"
#include "app_assert.h"
#include "sl_sleeptimer.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

#define READING_INTERVAL_MSEC        1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool trigger_process = false;
static mikroe_i2c_handle_t app_i2c_instance = NULL;
void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t stt;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  app_printf("Hello World - Mikroe PH2 Click example\r\n");
  stt = mikroe_ph2_init(app_i2c_instance);
  app_assert_status(stt);

  app_printf("================================\r\n");
  app_printf("Performing calibration\r\n");
  app_printf("================================\r\n");
  app_printf("Disconect BNC connector, then Short-circuit it\r\n");
  app_printf("Adjust offset potentiometer\r\n");
  app_printf("================================\r\n");
  app_printf("STAT1 Blink --> turn clockwise\r\n");
  app_printf("STAT2 Blink --> turn counter-clockwise\r\n");
  app_printf("================================\r\n");

  stt = mikroe_ph2_calibrate_offset();
  app_assert_status(stt);

  app_printf("Offset calibration completed!\r\n");
  app_printf("================================\r\n");
  app_printf("Connect probe back\r\n");
  app_printf("================================\r\n");
  sl_sleeptimer_delay_millisecond(5000);
  app_printf("Place probe into pH \r\n");
  app_printf("neutral substance for\r\n");
  app_printf("mid point calibration\r\n");
  app_printf("================================\r\n");
  sl_sleeptimer_delay_millisecond(5000);
  app_printf("Starting calibration \r\n");
  app_printf("================================\r\n");

  stt = mikroe_ph2_calibrate(7);
  app_assert_status(stt);

  app_printf("PH Calibration completed! \r\n");
  app_printf("================================\r\n");

  stt = mikroe_ds18b20_init();
  app_assert_status(stt);

  stt = mikroe_ds18b20_check_communication();
  app_assert_status(stt);

  stt = mikroe_ds18b20_default_cfg();
  app_assert_status(stt);

  app_printf("Ds18b20 ininit done! \r\n");

  app_printf("sl_sleeptimer_start_periodic = 0x%lx\r\n",
             sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                                   READING_INTERVAL_MSEC,
                                                   app_timer_callback,
                                                   NULL,
                                                   0,
                                                   0));
  app_printf(" Application Task \r\n");
  app_printf("================================\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (trigger_process) {
    trigger_process = false;

    float ph_value = 0.0;
    sl_status_t stt = mikroe_ph2_calculate_ph(&ph_value);
    if (SL_STATUS_OK == stt) {
      app_printf(" pH value: %.3f \r\n", ph_value);
      app_printf(" ================================ \r\n");
    }

    float temperature = 0.0;
    stt = mikroe_ds18b20_read_temperature(&temperature);
    if (SL_STATUS_OK == stt) {
      app_printf(" Temperature value: %.3f \r\n", temperature);
      app_printf(" ================================ \r\n");
    }
  }
}

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
