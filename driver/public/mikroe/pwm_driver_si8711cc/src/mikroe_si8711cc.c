/***************************************************************************//**
 * @file mikroe_si8711cc.c
 * @brief mikroe_si8711cc.c
 * @version 1.0.0
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
#include <stddef.h>
#include "stdbool.h"
#include "mikroe_si8711cc.h"
#include "pwmdriver.h"

static pwmdriver_t pwm_driver;
static pwmdriver_cfg_t pwm_driver_cfg;

sl_status_t mikroe_si8711cc_init(mikroe_pwm_handle_t pwm_instance)
{
  if (NULL == pwm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  pwm_driver.pwm.handle = pwm_instance;

  // Call basic setup functions
  pwmdriver_cfg_setup(&pwm_driver_cfg);

  if (pwmdriver_init(&pwm_driver, &pwm_driver_cfg) != PWMDRIVER_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_si8711cc_set_pwm_instance(
  mikroe_pwm_handle_t pwm_instance)
{
  if (NULL == pwm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pwm_driver.pwm.handle = pwm_instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_si8711cc_set_duty_cycle(float duty_cycle)
{
  pwmdriver_set_duty_cycle(&pwm_driver, duty_cycle);

  return SL_STATUS_OK;
}

sl_status_t mikroe_si8711cc_set_frequency(uint16_t frequency)
{
  return pwm_set_freq(&pwm_driver.pwm, frequency);
}

sl_status_t mikroe_si8711cc_pwm_stop(void)
{
  pwmdriver_pwm_stop(&pwm_driver);

  return SL_STATUS_OK;
}

sl_status_t mikroe_si8711cc_pwm_start(void)
{
  pwmdriver_pwm_start(&pwm_driver);

  return SL_STATUS_OK;
}
