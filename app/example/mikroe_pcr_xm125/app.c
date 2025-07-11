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

#include "sl_sleeptimer.h"
#include "app_assert.h"
#include "mikroe_xm125.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED            SL_I2C2

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

#define READING_INTERVAL_MSEC        1000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool timer_trigger_process = false;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t ret_code;
  app_printf("Silicon Labs - XM125 - PCR Click Driver Example.\r\n");

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  ret_code = mikroe_xm125_init(app_i2c_instance);
  app_assert_status(ret_code);
  app_printf("mikroe_xm125_init done\r\n");

  ret_code = mikroe_xm125_default_cfg();
  app_assert_status(ret_code);
  app_printf("mikroe_xm125_default_cfg done\r\n");

  ret_code = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                                   READING_INTERVAL_MSEC,
                                                   app_timer_callback,
                                                   NULL,
                                                   0,
                                                   0);
  app_printf("start timer periodic = 0x%lx\r\n", ret_code);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (timer_trigger_process == true) {
    uint32_t distance_data = 0;
    sl_status_t stt = mikroe_xm125_get_distance(&distance_data);

    if (SL_STATUS_OK == stt) {
      app_printf("Distance: %lu mm\r\n", distance_data);
    } else {
      app_printf("Error 0x%lx", stt);
    }
    timer_trigger_process = false;
  }
}

/***************************************************************************//**
 * App timer callback function.
 ******************************************************************************/
void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  timer_trigger_process = true;
}
