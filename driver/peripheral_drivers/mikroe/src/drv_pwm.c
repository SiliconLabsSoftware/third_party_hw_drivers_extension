/***************************************************************************//**
 * @file drv_pwm.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - PWM
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

#include <string.h>
#include "em_timer.h"
#include "sl_pwm.h"
#include "drv_pwm.h"

static pwm_t *_owner = NULL;

static err_t _acquire(pwm_t *obj, bool obj_open_state);

void pwm_configure_default(pwm_config_t *config)
{
  config->pin = 0xFFFFFFFF;
  config->freq_hz = 0;
}

err_t pwm_open(pwm_t *obj, pwm_config_t *config)
{
  pwm_config_t *p_config = &obj->config;
  memcpy(p_config, config, sizeof(pwm_config_t));

  return _acquire(obj, true);
}

err_t pwm_start(pwm_t *obj)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    sl_pwm_start((sl_pwm_instance_t *)obj->handle);
    return PWM_SUCCESS;
  } else {
    return PWM_ERROR;
  }
}

err_t pwm_stop(pwm_t *obj)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    sl_pwm_stop((sl_pwm_instance_t *)obj->handle);
    return PWM_SUCCESS;
  } else {
    return PWM_ERROR;
  }
}

err_t pwm_set_duty(pwm_t *obj, float duty_ratio)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    sl_pwm_set_duty_cycle((sl_pwm_instance_t *)obj->handle, duty_ratio * 100);
    return PWM_SUCCESS;
  } else {
    return PWM_ERROR;
  }
}

err_t pwm_set_freq(pwm_t *obj, uint32_t freq_hz)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return PWM_ERROR;
  }

  sl_pwm_config_t sl_pwm_config;

  obj->config.freq_hz = freq_hz;

  if (sl_pwm_deinit((sl_pwm_instance_t *)obj->handle) != SL_STATUS_OK) {
    return PWM_ERROR;
  }
  sl_pwm_config.frequency = freq_hz;
  sl_pwm_config.polarity = PWM_ACTIVE_HIGH;
  if (sl_pwm_init((sl_pwm_instance_t *)obj->handle, &sl_pwm_config)
      != SL_STATUS_OK) {
    return PWM_ERROR;
  }
  return PWM_SUCCESS;
}

void pwm_close(pwm_t *obj)
{
  sl_pwm_deinit((sl_pwm_instance_t *)obj->handle);
  obj->handle = NULL;
  _owner = NULL;
}

static err_t _acquire(pwm_t *obj, bool obj_open_state)
{
  err_t status = ACQUIRE_SUCCESS;

  if ((obj_open_state == true) && (_owner == obj)) {
    return ACQUIRE_FAIL;
  }

  if (_owner != obj) {
    _owner = obj;
  }

  return status;
}

// ------------------------------------------------------------------------- END
