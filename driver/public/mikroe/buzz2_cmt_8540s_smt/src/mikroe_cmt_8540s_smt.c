/***************************************************************************//**
 * @file mikroe_cmt_8540s_smt.c
 * @brief SCL CMT_8540S_SMT Source File
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
#include "mikroe_cmt_8540s_smt.h"

static buzz2_t buzz2;
static buzz2_cfg_t buzz2_cfg;

sl_status_t mikroe_cmt_8540s_smt_init(mikroe_pwm_handle_t pwm_instance)
{
  if (NULL == pwm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  buzz2.pwm.handle = pwm_instance;

  // Call basic setup functions
  buzz2_cfg_setup(&buzz2_cfg);

  if (buzz2_init(&buzz2, &buzz2_cfg) != PWM_SUCCESS) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_cmt_8540s_smt_set_pwm_instance(
  mikroe_pwm_handle_t pwm_instance)
{
  if (NULL == pwm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  buzz2.pwm.handle = pwm_instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_cmt_8540s_smt_set_duty_cycle(float duty_cycle)
{
  if (buzz2_set_duty_cycle(&buzz2, duty_cycle) != PWM_SUCCESS) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_cmt_8540s_smt_pwm_stop(void)
{
  if (buzz2_pwm_stop(&buzz2) != PWM_SUCCESS) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_cmt_8540s_smt_pwm_start(void)
{
  if (buzz2_pwm_start(&buzz2) != PWM_SUCCESS) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

void mikroe_cmt_8540s_smt_play_sound(uint16_t freq,
                                     uint16_t level,
                                     uint16_t duration)
{
  buzz2_play_sound(&buzz2, freq, level, duration);
}
