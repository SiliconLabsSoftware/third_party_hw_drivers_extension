/***************************************************************************//**
 * @file mikroe_a4988.c
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
#include "sl_sleeptimer.h"
#include "mikroe_a4988.h"
#include "mikroe_a4988_config.h"

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------
static sl_sleeptimer_timer_handle_t mikroe_a4988_timer_handle;
static volatile uint32_t pulses_count;
static mikroe_a4988_t stepper2;

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------
static void run_callback(sl_sleeptimer_timer_handle_t *handle, void *data);
static void step_callback(sl_sleeptimer_timer_handle_t *handle, void *data);
static uint32_t step_frequency_to_tick(void);

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * Initializes GPIOs needed to interface with the driver IC.
 ******************************************************************************/
sl_status_t mikroe_a4988_init(void)
{
  sl_status_t sc = SL_STATUS_OK;

  digital_out_init(&stepper2.step,
                   hal_gpio_pin_name(MIKROE_A4988_STEP_PORT,
                                     MIKROE_A4988_STEP_PIN));
  digital_out_init(&stepper2.dir,
                   hal_gpio_pin_name(MIKROE_A4988_DIR_PORT,
                                     MIKROE_A4988_DIR_PIN));
  digital_out_init(&stepper2.enable,
                   hal_gpio_pin_name(MIKROE_A4988_ENABLE_PORT,
                                     MIKROE_A4988_ENABLE_PIN));
  digital_out_init(&stepper2.rst,
                   hal_gpio_pin_name(MIKROE_A4988_RST_PORT,
                                     MIKROE_A4988_RST_PIN));
  digital_out_init(&stepper2.sleep,
                   hal_gpio_pin_name(MIKROE_A4988_SLEEP_PORT,
                                     MIKROE_A4988_SLEEP_PIN));
  digital_out_low(&stepper2.step);
  digital_out_high(&stepper2.rst);
  digital_out_high(&stepper2.dir);
  digital_out_low(&stepper2.enable);
  digital_out_high(&stepper2.sleep);

  stepper2.step_frequency = MIKROE_A4988_STEP_FREQUENCY;
  stepper2.state = IDLE;
  stepper2.direction = CLOCKWISE;

  return sc;
}

/***************************************************************************//**
 * Activates the RESET input on the driver IC which sets the translator
 * to a predefined Home state and turns off all of the FET outputs.
 * After the reset cycle, the driver is enabled.
 ******************************************************************************/
sl_status_t mikroe_a4988_reset(void)
{
  sl_status_t sc = SL_STATUS_OK;

  sc = mikroe_a4988_enable(false);
  if (sc != SL_STATUS_OK) {
    return SL_STATUS_INVALID_STATE;
  }
  digital_out_low(&stepper2.rst);
  digital_out_high(&stepper2.rst);
  sc = mikroe_a4988_enable(true);

  return sc;
}

/***************************************************************************//**
 * Enables or disables the motor driver IC. (turns on or off all of the
 * FET outputs of the driver).
 ******************************************************************************/
sl_status_t mikroe_a4988_enable(bool en)
{
  sl_status_t sc = SL_STATUS_OK;

  if (en) {
    digital_out_low(&stepper2.enable);
    stepper2.state = IDLE;
  } else {
    if (stepper2.state == RUNNING) {
      sc = SL_STATUS_INVALID_STATE;
    }
    digital_out_high(&stepper2.enable);
    stepper2.state = DISABLED;
  }

  return sc;
}

/***************************************************************************//**
 * Returns with the inner state of the driver.
 ******************************************************************************/
mikroe_a4988_state_t mikroe_a4988_get_state(void)
{
  return stepper2.state;
}

/***************************************************************************//**
 * Sets the speed of the motor in RPM.
 ******************************************************************************/
sl_status_t mikroe_a4988_set_step_frequency(uint32_t freq)
{
  sl_status_t sc = SL_STATUS_OK;

  if (stepper2.state == RUNNING) {
    return SL_STATUS_INVALID_STATE;
  }

  if ((freq > MIKROE_A4988_STEP_FREQUENCY_MIN)
      || (freq < MIKROE_A4988_STEP_FREQUENCY_MAX)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  mikroe_a4988_enable(true);
  stepper2.step_frequency = freq;

  return sc;
}

/***************************************************************************//**
 * Sets the direction of rotation.
 ******************************************************************************/
sl_status_t mikroe_a4988_set_direction(mikroe_a4988_direction_t direction)
{
  sl_status_t sc = SL_STATUS_OK;

  if ((direction == CLOCKWISE) || (direction == COUNTERCLOCKWISE)) {
    if (stepper2.direction == direction) {
      return SL_STATUS_OK;
    }

    // Stop stepper if it is running
    if (stepper2.state == RUNNING) {
      sc = mikroe_a4988_stop();
    }

    if (direction == CLOCKWISE) {
      digital_out_high(&stepper2.dir);
    } else if (direction == COUNTERCLOCKWISE) {
      digital_out_low(&stepper2.dir);
    }

    stepper2.direction = direction;
  }

  return sc;
}

/***************************************************************************//**
 * Starts the motor with the configured speed and direction.
 ******************************************************************************/
sl_status_t mikroe_a4988_start()
{
  sl_status_t sc = SL_STATUS_OK;
  uint32_t timeout;

  if (stepper2.state != IDLE) {
    return SL_STATUS_INVALID_STATE;
  }

  if (stepper2.step_frequency > MIKROE_A4988_STEP_FREQUENCY_MAX) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }

  digital_out_low(&stepper2.step);
  timeout = step_frequency_to_tick();
  sc = sl_sleeptimer_start_periodic_timer(&mikroe_a4988_timer_handle,
                                          timeout,
                                          run_callback,
                                          NULL, 0, 0);
  stepper2.state = RUNNING;

  return sc;
}

/***************************************************************************//**
 * Stops the motor.
 ******************************************************************************/
sl_status_t mikroe_a4988_stop()
{
  sl_status_t sc = SL_STATUS_OK;

  if (stepper2.state != RUNNING) {
    return SL_STATUS_INVALID_STATE;
  }

  sc = sl_sleeptimer_stop_timer(&mikroe_a4988_timer_handle);
  stepper2.state = IDLE;

  return sc;
}

/***************************************************************************//**
 * Sleep the IC A4988.
 ******************************************************************************/
sl_status_t mikroe_a4988_sleep()
{
  sl_status_t sc = SL_STATUS_OK;

  sc = mikroe_a4988_enable(false);
  digital_out_low(&stepper2.sleep);
  stepper2.state = SLEEP;

  return sc;
}

/***************************************************************************//**
 * Wake up the IC A4988.
 ******************************************************************************/
sl_status_t mikroe_a4988_wakeup()
{
  sl_status_t sc = SL_STATUS_OK;

  digital_out_high(&stepper2.sleep);
  sl_sleeptimer_delay_millisecond(1);
  sc = mikroe_a4988_enable(true);

  return sc;
}

/***************************************************************************//**
 * Moves the motor by a given number of steps.
 ******************************************************************************/
sl_status_t mikroe_a4988_step(uint32_t step_count)
{
  sl_status_t sc = SL_STATUS_OK;
  uint32_t timeout;

  if (stepper2.state != IDLE) {
    return SL_STATUS_INVALID_STATE;
  }

  if (stepper2.step_frequency > MIKROE_A4988_STEP_FREQUENCY_MAX) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }

  digital_out_low(&stepper2.step);
  timeout = step_frequency_to_tick();
  pulses_count = step_count;
  sc = sl_sleeptimer_start_periodic_timer(&mikroe_a4988_timer_handle,
                                          timeout,
                                          step_callback,
                                          NULL, 0, 0);
  stepper2.state = RUNNING;

  return sc;
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * Timer callback for the non-blocking stepper_step function.
 ******************************************************************************/
static void step_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;
  pulses_count--;

  digital_out_toggle(&stepper2.step);
  if (pulses_count == 0) {
    sl_sleeptimer_stop_timer(&mikroe_a4988_timer_handle);
  }
}

static void run_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  digital_out_toggle(&stepper2.step);
}

/*******************************************************************************
 * Converts STEP Frequency in ticks.
 ******************************************************************************/
static uint32_t step_frequency_to_tick(void)
{
  uint32_t timer_frequency = sl_sleeptimer_get_timer_frequency();

  return timer_frequency / stepper2.step_frequency;
}
