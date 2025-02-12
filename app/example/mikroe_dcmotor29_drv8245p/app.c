/***************************************************************************//**
 * @file app.c
 * @brief Example application
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

#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "mikroe_drv8245p.h"
#include "app_assert.h"

#if (defined(SLI_SI917))
#include "sl_si91x_ssi.h"
#include "sl_i2c_instances.h"
#include "rsi_debug.h"
#else
#include "sl_spidrv_instances.h"
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
#define app_printf(...)              DEBUGOUT(__VA_ARGS__)

static sl_ssi_instance_t ssi_instance = SL_SSI_MASTER_ACTIVE;
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#define app_printf(...)              app_log(__VA_ARGS__)
#endif

#define APP_SET_STATE_TIMER_TIMEOUT   3000

mikroe_spi_handle_t app_spi_instance = NULL;
mikroe_i2c_handle_t app_i2c_instance = NULL;

static volatile uint8_t state;
static volatile uint8_t state_changed;

static sl_sleeptimer_timer_handle_t set_state_timer;
static void app_set_state_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                         void *data);
static void application_task(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
  app_spi_instance = &ssi_instance;
  app_i2c_instance = &i2c_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  app_printf("Mikroe DC Motor 29 Click Driver - Example\n");
  app_printf("======= Application initialization =======\n");

  sc = mikroe_drv8245p_init(app_spi_instance,
                            app_i2c_instance);
  if (SL_STATUS_OK != sc) {
    app_printf("DC Motor 29 Click initialized failed!\n");
    return;
  } else {
    app_printf("DC Motor 29 Click initialized successfully!\n");
  }

  sc = mikroe_drv8245p_default_cfg();
  app_assert_status(sc);

  state = MIKROE_DRV8245P_DRIVE_MOTOR_CW;
  state_changed = 1;
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  application_task();
}

/***************************************************************************//**
 * Application task function.
 ******************************************************************************/
static void application_task(void)
{
  sl_status_t sc;

  if (state_changed) {
    state_changed = 0;
    mikroe_drv8245p_drive_motor(state);

    switch (state) {
      case MIKROE_DRV8245P_DRIVE_MOTOR_CW:
        app_printf("\n State: Clockwise\n");
        app_printf("Change state after 3 seconds ...\n");
        break;
      case MIKROE_DRV8245P_DRIVE_MOTOR_BRAKE:
        app_printf("\n State: Brake\n");
        app_printf("Change state after 3 seconds ...\n");
        break;
      case MIKROE_DRV8245P_DRIVE_MOTOR_CCW:
        app_printf("\n State: Counter-clockwise\n");
        app_printf("Change state after 3 seconds ...\n");
        break;
      case MIKROE_DRV8245P_DRIVE_MOTOR_COASTING:
        app_printf("\n State: Coasting\n");
        app_printf("Change state after 3 seconds ...\n");
        break;
      default:
        break;
    }
    sc = sl_sleeptimer_start_timer_ms(&set_state_timer,
                                      APP_SET_STATE_TIMER_TIMEOUT,
                                      app_set_state_timer_callback,
                                      NULL, 0, 0);
    app_assert_status(sc);
  }
}

/***************************************************************************//**
 * App set state timer callback.
 ******************************************************************************/
static void app_set_state_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                         void *data)
{
  (void) timer;
  (void) data;

  switch (state) {
    case MIKROE_DRV8245P_DRIVE_MOTOR_CW:
      state = MIKROE_DRV8245P_DRIVE_MOTOR_BRAKE;
      break;
    case MIKROE_DRV8245P_DRIVE_MOTOR_BRAKE:
      state = MIKROE_DRV8245P_DRIVE_MOTOR_CCW;
      break;
    case MIKROE_DRV8245P_DRIVE_MOTOR_CCW:
      state = MIKROE_DRV8245P_DRIVE_MOTOR_COASTING;
      break;
    case MIKROE_DRV8245P_DRIVE_MOTOR_COASTING:
      state = MIKROE_DRV8245P_DRIVE_MOTOR_CW;
      break;
    default:
      break;
  }

  state_changed = 1;
}
