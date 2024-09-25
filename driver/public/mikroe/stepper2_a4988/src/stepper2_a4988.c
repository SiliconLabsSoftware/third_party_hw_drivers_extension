/***************************************************************************//**
 * @file stepper_a4988.c
 * @brief Stepper motor driver.
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
 * This code has been minimally tested to ensure that it builds with
 * the specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include "stepper2_a4988.h"
#include "em_cmu.h"
#include "em_timer.h"
#include "em_gpio.h"
#include "sl_status.h"
// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------
static void on_sleeptimer_callback(sl_sleeptimer_timer_handle_t *handle,
                                   void *data);
static sl_status_t verify_pwm_frequency(a4988_stepper_inst_t *inst);
static CMU_Clock_TypeDef get_timer_clock(TIMER_TypeDef *timer);

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * Initializes GPIOs needed to interface with the driver IC.
 ******************************************************************************/
sl_status_t a4988_stepper_init(a4988_stepper_inst_t *stepper_inst,
                               sl_pwm_instance_t *pwm_inst)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != stepper_inst) && (NULL != pwm_inst)) {
    CMU_ClockEnable(cmuClock_GPIO, true);

    GPIO_PinModeSet(stepper_inst->gpio_config.step_port,
                    stepper_inst->gpio_config.step_pin,
                    gpioModePushPull, 0);
    GPIO_PinModeSet(stepper_inst->gpio_config.rst_port,
                    stepper_inst->gpio_config.rst_pin,
                    gpioModePushPull, 1);
    GPIO_PinModeSet(stepper_inst->gpio_config.dir_port,
                    stepper_inst->gpio_config.dir_pin,
                    gpioModePushPull, 1);
    GPIO_PinModeSet(stepper_inst->gpio_config.enable_port,
                    stepper_inst->gpio_config.enable_pin,
                    gpioModePushPull, 0);
    GPIO_PinModeSet(stepper_inst->gpio_config.sleep_port,
                    stepper_inst->gpio_config.sleep_pin,
                    gpioModePushPull, 1);

    stepper_inst->pwm = pwm_inst;
    sl_pwm_set_duty_cycle(stepper_inst->pwm, 50);
    stepper_inst->state = IDLE;

    stt = verify_pwm_frequency(stepper_inst);
  }
  return stt;
}

/***************************************************************************//**
 * Activates the RESET input on the driver IC which sets the translator
 * to a predefined Home state and turns off all of the FET outputs.
 * After the reset cycle, the driver is enabled.
 ******************************************************************************/
sl_status_t a4988_stepper_reset(a4988_stepper_inst_t *inst)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != inst) {
    a4988_stepper_enable(inst, false);
    GPIO_PinOutClear(inst->gpio_config.rst_port, inst->gpio_config.rst_pin);
    GPIO_PinOutSet(inst->gpio_config.rst_port, inst->gpio_config.rst_pin);
    a4988_stepper_enable(inst, true);

    stt = SL_STATUS_OK;
  }

  return stt;
}

/***************************************************************************//**
 * Enables or disables the motor driver IC. (turns on or off all of the
 * FET outputs of the driver).
 ******************************************************************************/
sl_status_t a4988_stepper_enable(a4988_stepper_inst_t *inst, bool en)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != inst) {
    if (en) {
      GPIO_PinOutClear(inst->gpio_config.enable_port,
                       inst->gpio_config.enable_pin);
      inst->state = IDLE;
    } else {
      if (inst->state == RUNNING) {
        sl_pwm_stop(inst->pwm);
      }

      GPIO_PinOutSet(inst->gpio_config.enable_port,
                     inst->gpio_config.enable_pin);

      inst->state = DISABLED;
    }
    stt = SL_STATUS_OK;
  }

  return stt;
}

/***************************************************************************//**
 * Returns with the inner state of the driver.
 ******************************************************************************/
a4988_stepper_state_t a4988_stepper_get_state(a4988_stepper_inst_t *inst)
{
  if (NULL != inst) {
    return inst->state;
  } else {
    return ERROR;
  }
}

/***************************************************************************//**
 * Sets the speed of the motor in RPM.
 ******************************************************************************/
sl_status_t a4988_stepper_set_speed(a4988_stepper_inst_t *inst, uint32_t rpm)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != inst) {
    a4988_stepper_state_t curr_state = inst->state;

    if (rpm > A4988_STEPPER_MAX_RPM) {
      return SL_STATUS_INVALID_PARAMETER;
    }

    a4988_stepper_enable(inst, false);

    sl_pwm_deinit(inst->pwm);

    sl_pwm_config_t pwm_config = {
      .frequency = (rpm * A4988_STEPPER_MOTOR_STEPS_PER_REV) / 60,
      .polarity = PWM_ACTIVE_HIGH,
    };

    sl_pwm_init(inst->pwm, &pwm_config);
    sl_pwm_set_duty_cycle(inst->pwm, 50);

    a4988_stepper_enable(inst, true);
    inst->rpm = rpm;

    if (curr_state == RUNNING) {
      a4988_stepper_start(inst);
    }

    stt = SL_STATUS_OK;
  }

  return stt;
}

/***************************************************************************//**
 * Sets the direction of rotation.
 ******************************************************************************/
sl_status_t a4988_stepper_set_dir(a4988_stepper_inst_t *inst,
                                  a4988_stepper_dir_t dir)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != inst) && ((dir == CW) || (dir == CCW))) {
    if (inst->dir == dir) {
      return SL_STATUS_OK;
    }

    // Stop stepper if it is running
    if (inst->state == RUNNING) {
      a4988_stepper_stop(inst);
    }

    if (dir == CW) {
      GPIO_PinOutSet(inst->gpio_config.dir_port, inst->gpio_config.dir_pin);
    } else if (dir == CCW) {
      GPIO_PinOutClear(inst->gpio_config.dir_port, inst->gpio_config.dir_pin);
    }

    inst->dir = dir;
    stt = SL_STATUS_OK;
  }

  return stt;
}

/***************************************************************************//**
 * Starts the motor with the configured speed and direction.
 ******************************************************************************/
sl_status_t a4988_stepper_start(a4988_stepper_inst_t *inst)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != inst) {
    if (inst->state != IDLE) {
      return SL_STATUS_INVALID_STATE;
    }

    if (inst->rpm > A4988_STEPPER_MAX_RPM) {
      return SL_STATUS_INVALID_CONFIGURATION;
    }

    sl_pwm_start(inst->pwm);
    inst->state = RUNNING;

    stt = SL_STATUS_OK;
  }

  return stt;
}

/***************************************************************************//**
 * Stops the motor.
 ******************************************************************************/
sl_status_t a4988_stepper_stop(a4988_stepper_inst_t *inst)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != inst) {
    if (inst->state != RUNNING) {
      return SL_STATUS_INVALID_STATE;
    }

    sl_pwm_stop(inst->pwm);
    GPIO_PinOutClear(inst->gpio_config.step_port, inst->gpio_config.step_pin);
    inst->state = IDLE;

    stt = SL_STATUS_OK;
  }
  return stt;
}

/***************************************************************************//**
 * Sleep the IC A4988.
 ******************************************************************************/
sl_status_t a4988_stepper_sleep(a4988_stepper_inst_t *inst)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != inst) {
    a4988_stepper_enable(inst, false);
    GPIO_PinOutClear(inst->gpio_config.sleep_port, inst->gpio_config.sleep_pin);
    inst->state = SLEEP;

    stt = SL_STATUS_OK;
  }
  return stt;
}

/***************************************************************************//**
 * Wake up the IC A4988.
 ******************************************************************************/
sl_status_t a4988_stepper_wakeup(a4988_stepper_inst_t *inst)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != inst) {
    GPIO_PinOutSet(inst->gpio_config.sleep_port, inst->gpio_config.sleep_pin);
    sl_sleeptimer_delay_millisecond(1);
    a4988_stepper_enable(inst, true);

    stt = SL_STATUS_OK;
  }
  return stt;
}

/***************************************************************************//**
 * Moves the motor by a given number of steps. This is a non-blocking function.
 * The speed of the movement can be set with the a4988_stepper_set_speed()
 * function.
 ******************************************************************************/
sl_status_t a4988_stepper_step(a4988_stepper_inst_t *inst,
                               sl_sleeptimer_timer_handle_t *sleeptimer_handle,
                               uint32_t step_count)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != inst) && (NULL != sleeptimer_handle)) {
    uint32_t tick_count;
    uint32_t sleeptimer_frequency;

    if (inst->state != IDLE) {
      return SL_STATUS_INVALID_STATE;
    }

    if (inst->rpm > A4988_STEPPER_MAX_RPM) {
      return SL_STATUS_INVALID_CONFIGURATION;
    }

    inst->curr_step = 0;
    inst->num_step = step_count * 2;
    inst->state = RUNNING;

    GPIO_PinOutClear(inst->gpio_config.step_port, inst->gpio_config.step_pin);

    sleeptimer_frequency = sl_sleeptimer_get_timer_frequency();
    // Calculate the sleeptimer tick number according to the motor speed
    tick_count = (sleeptimer_frequency * 30)
                 / (inst->rpm * A4988_STEPPER_MOTOR_STEPS_PER_REV);

    stt = sl_sleeptimer_start_periodic_timer(sleeptimer_handle,
                                             tick_count,
                                             on_sleeptimer_callback,
                                             (void *)inst,
                                             0,
                                             0);
  }

  return stt;
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * Timer callback for the non-blocking stepper_step function.
 ******************************************************************************/
static void on_sleeptimer_callback(sl_sleeptimer_timer_handle_t *handle,
                                   void *data)
{
  a4988_stepper_inst_t *inst = (a4988_stepper_inst_t *)data;

  GPIO_PinOutToggle(inst->gpio_config.step_port, inst->gpio_config.step_pin);
  inst->curr_step++;

  if (inst->curr_step == inst->num_step) {
    inst->curr_step = 0;
    inst->num_step = 0;
    inst->state = IDLE;
    sl_sleeptimer_stop_timer(handle);
    GPIO_PinOutClear(inst->gpio_config.step_port, inst->gpio_config.step_pin);
  }
}

/***************************************************************************//**
 * Utility function used to verify the frequency of the PWM instance assigned
 * to the driver.
 ******************************************************************************/
static sl_status_t verify_pwm_frequency(a4988_stepper_inst_t *inst)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != inst) {
    uint32_t frequency = 0;

    // Verify actual configured frequency based on register values.
    frequency = (CMU_ClockFreqGet(get_timer_clock(inst->pwm->timer))
                 / (TIMER_TopGet(inst->pwm->timer) + 1));

    // Store RPM value in the instance
    inst->rpm = (frequency * 60) / A4988_STEPPER_MOTOR_STEPS_PER_REV;

    stt = (inst->rpm > A4988_STEPPER_MAX_RPM) ? SL_STATUS_INVALID_CONFIGURATION
          : SL_STATUS_OK;
  }
  return stt;
}

/***************************************************************************//**
 * Utility function used to get the clock of a timer.
 ******************************************************************************/
static CMU_Clock_TypeDef get_timer_clock(TIMER_TypeDef *timer)
{
#if defined(_CMU_HFCLKSEL_MASK) || defined(_CMU_CMD_HFCLKSEL_MASK)
  CMU_Clock_TypeDef timer_clock = cmuClock_HF;
#elif defined(_CMU_SYSCLKCTRL_MASK)
  CMU_Clock_TypeDef timer_clock = cmuClock_SYSCLK;
#else
#error "Unknown root of clock tree"
#endif

  switch ((uint32_t)timer) {
#if defined(TIMER0_BASE)
    case TIMER0_BASE:
      timer_clock = cmuClock_TIMER0;
      break;
#endif
#if defined(TIMER1_BASE)
    case TIMER1_BASE:
      timer_clock = cmuClock_TIMER1;
      break;
#endif
#if defined(TIMER2_BASE)
    case TIMER2_BASE:
      timer_clock = cmuClock_TIMER2;
      break;
#endif
#if defined(TIMER3_BASE)
    case TIMER3_BASE:
      timer_clock = cmuClock_TIMER3;
      break;
#endif
#if defined(TIMER4_BASE)
    case TIMER4_BASE:
      timer_clock = cmuClock_TIMER4;
      break;
#endif
#if defined(TIMER5_BASE)
    case TIMER5_BASE:
      timer_clock = cmuClock_TIMER5;
      break;
#endif
#if defined(TIMER6_BASE)
    case TIMER6_BASE:
      timer_clock = cmuClock_TIMER6;
      break;
#endif
#if defined(WTIMER0_BASE)
    case WTIMER0_BASE:
      timer_clock = cmuClock_WTIMER0;
      break;
#endif
#if defined(WTIMER1_BASE)
    case WTIMER1_BASE:
      timer_clock = cmuClock_WTIMER1;
      break;
#endif
#if defined(WTIMER2_BASE)
    case WTIMER2_BASE:
      timer_clock = cmuClock_WTIMER2;
      break;
#endif
#if defined(WTIMER3_BASE)
    case WTIMER3_BASE:
      timer_clock = cmuClock_WTIMER3;
      break;
#endif
    default:
      EFM_ASSERT(0);
      break;
  }
  return timer_clock;
}
