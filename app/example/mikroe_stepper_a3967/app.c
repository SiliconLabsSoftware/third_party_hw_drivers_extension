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

#include "mikroe_a3967.h"
#include "sl_sleeptimer.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#else
#include "app_log.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

// The 28BYJ-48 motor features a 1/64 reduction gear set.
#define MOTOR_GEAR_RATIO             64

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc = SL_STATUS_OK;

  app_printf("Stepper Click demo application!!!\r\n");

  // specifications of the motor
  mikroe_a3967_init();
  mikroe_a3967_set_direction(MIKROE_A3967_CLOCKWISE);
  mikroe_a3967_set_step_frequency(500);
  mikroe_a3967_config_mode(MIKROE_A3967_FULL_STEP);

  app_printf("A3967 Stepper Start\r\n");

  // The 28BYJ-48 motor have 64 steps per revolution.
  // So it need (64 * MOTOR_GEAR_RATIO) step to complete one revolution.
  sc = mikroe_a3967_step(16 * MOTOR_GEAR_RATIO);
  if (sc != SL_STATUS_OK) {
    app_printf("error code = 0x%lx\r\n", sc);
  }

  // Need to wait until the motor stop
  while (mikroe_a3967_get_state()) {}
  mikroe_a3967_set_direction(MIKROE_A3967_COUNTERCLOCKWISE);
  sc = mikroe_a3967_step(32 * MOTOR_GEAR_RATIO);
  if (sc != SL_STATUS_OK) {
    app_printf("error code = 0x%lx\r\n", sc);
  }

  // Need to wait until the motor stop
  while (mikroe_a3967_get_state()) {}
  mikroe_a3967_set_direction(MIKROE_A3967_CLOCKWISE);
  sc = mikroe_a3967_step(64 * MOTOR_GEAR_RATIO);
  if (sc != SL_STATUS_OK) {
    app_printf("error code = 0x%lx\r\n", sc);
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
