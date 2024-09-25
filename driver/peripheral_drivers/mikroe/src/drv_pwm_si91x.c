/***************************************************************************//**
 * @file drv_pwm_si91x.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - PWM for Si91x
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

#include "drv_pwm.h"
#include "sl_si91x_pwm.h"
#include "sl_status.h"

#define DUTY_CYCLE_UPDATE 0x01  // Enable duty cycle updating bit in register

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
  sl_pwm_config_t *ptr = (sl_pwm_config_t *)obj->handle;

  memcpy(p_config, config, sizeof(pwm_config_t));
  if (_acquire(obj, true) == ACQUIRE_FAIL) {
    return PWM_ERROR;
  }

  ptr->is_polarity_high = SL_POLARITYH_LOW;
  // set the PWM configuration parameters
  if (sl_si91x_pwm_set_configuration(ptr) != SL_STATUS_OK) {
    return PWM_ERROR;
  }

  // set base time period control
  if (sl_si91x_pwm_control_period(SL_TIME_PERIOD_POSTSCALE_1_1,
                                  SL_TIME_PERIOD_PRESCALE_1,
                                  ptr->channel) != SL_STATUS_OK) {
    return PWM_ERROR;
  }

  // set duty cycle control parameters
  if (sl_si91x_pwm_configure_duty_cycle(SL_DUTY_CYCLE_ENABLE,
                                        DUTY_CYCLE_UPDATE,
                                        ptr->channel) != SL_STATUS_OK) {
    return PWM_ERROR;
  }

  return PWM_SUCCESS;
}

err_t pwm_start(pwm_t *obj)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return PWM_ERROR;
  }

  sl_pwm_config_t *ptr = (sl_pwm_config_t *)obj->handle;

  if (sl_si91x_pwm_start(ptr->channel) != SL_STATUS_OK) {
    return PWM_ERROR;
  }
  return PWM_SUCCESS;
}

err_t pwm_stop(pwm_t *obj)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return PWM_ERROR;
  }

  sl_pwm_config_t *ptr = (sl_pwm_config_t *)obj->handle;

  if (sl_si91x_pwm_stop(ptr->channel) != SL_STATUS_OK) {
    return PWM_ERROR;
  }
  return PWM_SUCCESS;
}

err_t pwm_set_duty(pwm_t *obj, float duty_ratio)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return PWM_ERROR;
  }

  sl_pwm_config_t *ptr = (sl_pwm_config_t *)obj->handle;
  uint16_t time_period;
  uint32_t ticks;

  // Get time period
  if (sl_si91x_pwm_get_time_period(ptr->channel,
                                   &time_period) != SL_STATUS_OK) {
    return PWM_ERROR;
  }
  ticks = (uint32_t)(time_period * (1 - duty_ratio));
  // Set Duty cycle value for channel
  if (sl_si91x_pwm_set_duty_cycle(ticks,
                                  ptr->channel) != SL_STATUS_OK) {
    return PWM_ERROR;
  }
  return PWM_SUCCESS;
}

err_t pwm_set_freq(pwm_t *obj, uint32_t freq_hz)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return PWM_ERROR;
  }

  sl_pwm_config_t *ptr = (sl_pwm_config_t *)obj->handle;

  obj->config.freq_hz = freq_hz;

  ptr->is_polarity_high = SL_POLARITYH_LOW;
  // Update PWM frequency into channel config
  ptr->frequency = freq_hz;
  // set the PWM configuration parameters
  if (sl_si91x_pwm_set_configuration(ptr) != SL_STATUS_OK) {
    return PWM_ERROR;
  }
  return PWM_SUCCESS;
}

void pwm_close(pwm_t *obj)
{
  sl_si91x_pwm_deinit();
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
