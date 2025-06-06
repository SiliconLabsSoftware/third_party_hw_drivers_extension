/***************************************************************************//**
 * @file app.c
 * @brief Example application
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "mikroe_veml6035.h"
#include "sl_sleeptimer.h"

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

/***************************************************************************//**
 *                           Definition
 ******************************************************************************/
#define CONTROL_INTERVAL_MSEC        1000

/***************************************************************************//**
 *                           Static Functions
 ******************************************************************************/
static mikroe_i2c_handle_t app_i2c_instance = NULL;
static volatile bool trigger_process = false;

/***************************************************************************//**
 *                           Global Variables
 ******************************************************************************/
static sl_sleeptimer_timer_handle_t app_timer_handle;
float lx_val;
float resolution = 0.0128; // Typical resolution (lx/cnt)

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t stt;

  app_printf("Hello World - Mikroe Ambient 11 Click example\r\n");

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif
  // Initalizes I2C driver, applies low sensitiviti settings ( GAIN = 0, DG = 0,
  //  SENS = 0 and IT = 100ms )
  stt = mikroe_ambient11_init(app_i2c_instance);
  sl_sleeptimer_delay_millisecond(100);

  app_printf("mikroe_ambient11_init = 0x%x\r\n", (uint16_t)stt);
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        CONTROL_INTERVAL_MSEC,
                                        app_timer_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************/ /**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (trigger_process) {
    trigger_process = false;
    lx_val = mikroe_ambient11_calc_illumination(resolution);
    app_printf("Illumination : %.2f lx \r\n", lx_val);
    app_printf("-------------------------\r\n");
  }
}

/***************************************************************************/ /**
 * Callback for app timer.
 ******************************************************************************/
void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
