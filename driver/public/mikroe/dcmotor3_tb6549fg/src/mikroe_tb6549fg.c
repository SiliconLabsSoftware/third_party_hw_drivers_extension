/***************************************************************************//**
 * @file mikroe_tb6549fg.c
 * @brief SCL TB6549FG Source File
 * @version 1.0.0
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

#include <stddef.h>
#include "mikroe_tb6549fg_config.h"
#include "mikroe_tb6549fg.h"

static dcmotor3_t dcmotor3;
static dcmotor3_cfg_t dcmotor3_cfg;

sl_status_t mikroe_tb6549fg_init(mikroe_pwm_handle_t pwm_instance)
{
  if (pwm_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  dcmotor3.pwm.handle = pwm_instance;

  // Call basic setup functions
  dcmotor3_cfg_setup(&dcmotor3_cfg);

#if defined(DCMOTOR3_IN1_PORT) && defined(DCMOTOR3_IN1_PIN)
  dcmotor3_cfg.in1 = hal_gpio_pin_name(DCMOTOR3_IN1_PORT, DCMOTOR3_IN1_PIN);
#endif

#if defined(DCMOTOR3_IN2_PORT) && defined(DCMOTOR3_IN2_PIN)
  dcmotor3_cfg.in2 = hal_gpio_pin_name(DCMOTOR3_IN2_PORT, DCMOTOR3_IN2_PIN);
#endif

#if defined(DCMOTOR3_SLP_PORT) && defined(DCMOTOR3_SLP_PIN)
  dcmotor3_cfg.slp = hal_gpio_pin_name(DCMOTOR3_SLP_PORT, DCMOTOR3_SLP_PIN);
#endif

  if (dcmotor3_init(&dcmotor3, &dcmotor3_cfg) != DCMOTOR3_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_tb6549fg_set_pwm_instance(mikroe_pwm_handle_t pwm_instance)
{
  if (NULL == pwm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  dcmotor3.pwm.handle = pwm_instance;

  return SL_STATUS_OK;
}

void mikroe_tb6549fg_set_duty_cycle(float duty_cycle)
{
  dcmotor3_set_duty_cycle(&dcmotor3, duty_cycle);
}

void mikroe_tb6549fg_pwm_stop(void)
{
  dcmotor3_pwm_stop(&dcmotor3);
}

void mikroe_tb6549fg_pwm_start(void)
{
  dcmotor3_pwm_start(&dcmotor3);
}

void mikroe_tb6549fg_enable(void)
{
  dcmotor3_enable(&dcmotor3);
}

void mikroe_tb6549fg_standby(void)
{
  dcmotor3_standby(&dcmotor3);
}

void mikroe_tb6549fg_counter_clockwise(void)
{
  dcmotor3_counter_clockwise(&dcmotor3);
}

void mikroe_tb6549fg_clockwise(void)
{
  dcmotor3_clockwise(&dcmotor3);
}

void mikroe_tb6549fg_short_brake(void)
{
  dcmotor3_short_brake(&dcmotor3);
}

void mikroe_tb6549fg_stop(void)
{
  dcmotor3_stop(&dcmotor3);
}
