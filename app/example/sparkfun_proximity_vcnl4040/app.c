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
#include "sparkfun_vcnl4040.h"
#include "app_assert.h"

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

static mikroe_i2c_handle_t app_i2c_instance = NULL;
static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;
  uint16_t id;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  app_printf("\r======== VCNL4040 Proximity Sensor Driver ========\n");
  sc = sparkfun_vcnl4040_init(app_i2c_instance);
  app_assert(sc == SL_STATUS_OK, "\rVCNL4040 Init fail...\n");
  app_printf("\rVCNL4040 Init done...\n");

  sparkfun_vcnl4040_get_id(&id);
  app_printf("\rDevice ID: 0x%X\n", id);
  app_printf("\r\t==================================\n");

  sc = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                             READING_INTERVAL_MSEC,
                                             app_timer_cb,
                                             (void *) NULL,
                                             0,
                                             0);

  if (sc != SL_STATUS_OK) {
    app_printf("\r > Start Periodic Measurement Fail\n");
  } else {
    app_printf("\r > Start Periodic Measurement\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t sc;
  uint16_t prox;
  uint16_t ambient;
  uint16_t white;

  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;

  sc = sparkfun_vcnl4040_get_proximity(&prox);
  sc |= sparkfun_vcnl4040_get_ambient(&ambient);
  sc |= sparkfun_vcnl4040_get_white(&white);

  if (sc != SL_STATUS_OK) {
    app_printf("\r > Reading data failed\n");
  } else {
    app_printf(
      "\r > Proximity value: % 4d\tAmbient light value: % 4d\tWhite light value: % 4d\n",
      prox,
      ambient,
      white);
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
