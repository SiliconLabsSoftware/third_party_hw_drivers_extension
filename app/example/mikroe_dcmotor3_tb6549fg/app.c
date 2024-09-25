/***************************************************************************//**
 * @file
 * @brief Top level application functions
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

#include "mikroe_tb6549fg.h"
#include "sl_pwm_instances.h"
#include "sl_sleeptimer.h"

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

mikroe_pwm_handle_t app_pwm_instance = NULL;
static uint8_t dcmotor3_direction = 1;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#if (defined(SLI_SI917))
  app_pwm_instance = &sl_pwm_channel_0_config;
#else
  app_pwm_instance = &sl_pwm_mikroe;
#endif

  if (mikroe_tb6549fg_init(app_pwm_instance) == SL_STATUS_OK) {
    app_printf("DC Motor 3 Click initializes successfully\n");
  }
  mikroe_tb6549fg_set_duty_cycle(0.0);
  mikroe_tb6549fg_pwm_start();
  sl_sleeptimer_delay_millisecond(1000);
  mikroe_tb6549fg_enable();
  sl_sleeptimer_delay_millisecond(1000);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static int8_t duty_cnt = 1;
  static int8_t duty_inc = 1;
  float duty = duty_cnt / 10.0;

  if (dcmotor3_direction == 1) {
    mikroe_tb6549fg_clockwise();
    app_printf(">>>> CLOCKWISE \n");
    mikroe_tb6549fg_enable();
  } else {
    mikroe_tb6549fg_counter_clockwise();
    app_printf("<<<< COUNTER CLOCKWISE \n");
    mikroe_tb6549fg_enable();
  }
  mikroe_tb6549fg_set_duty_cycle(duty);
  app_printf("Duty: %d%%\r\n", ( uint16_t )(duty_cnt * 10));
  sl_sleeptimer_delay_millisecond(500);

  if (10 == duty_cnt) {
    duty_inc = -1;
  } else if (0 == duty_cnt) {
    duty_inc = 1;

    if (dcmotor3_direction == 1) {
      dcmotor3_direction = 0;
    } else if (dcmotor3_direction == 0) {
      dcmotor3_direction = 1;
    }
  }
  duty_cnt += duty_inc;
}
