/***************************************************************************//**
 * @file mikroe_a4988.h
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

#ifndef STEPPER2_A4988_H_
#define STEPPER2_A4988_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MIKROE_A4988_STEP_FREQUENCY_MIN   1
#define MIKROE_A4988_STEP_FREQUENCY_MAX   500000

/***************************************************************************//**
 * @addtogroup Stepper motor driver
 * @{
 *
 * @brief
 *  The implementation of a stepper motor driver. It is primary made for the
 *  A4988 stepper motor driver to drive bipolar stepper motors.
 ******************************************************************************/

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include <stdbool.h>
#include "sl_status.h"
#include "drv_digital_out.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *    Typedef for specifying the direction of rotation.
 ******************************************************************************/
typedef enum {
  CLOCKWISE,
  COUNTERCLOCKWISE
} mikroe_a4988_direction_t;

/***************************************************************************//**
 * @brief
 *    Typedef for the state of operation.
 ******************************************************************************/
typedef enum {
  DISABLED,
  IDLE,
  RUNNING,
  SLEEP,
  ERROR,
} mikroe_a4988_state_t;

/***************************************************************************//**
 * @brief
 *    Mikroe A4988 instance
 ******************************************************************************/
typedef struct {
  digital_out_t step;
  digital_out_t dir;
  digital_out_t enable;
  digital_out_t rst;
  digital_out_t sleep;

  uint32_t step_frequency;
  mikroe_a4988_state_t state;
  mikroe_a4988_direction_t direction;
} mikroe_a4988_t;

// -----------------------------------------------------------------------------
//                          Public Function Declarations
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *    Stepper2 initialization function. This function must call first before
 *    using other driver APIs.
 *    If not have any config, Stepper2 will operation in default configuration:
 *    - Default direction: MIKROE_A4988_CLOCKWISE
 *    - Default frequency: MIKROE_A4988_STEP_FREQUENCY
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_CONFIGURATION if step freq higher than the limit
 ******************************************************************************/
sl_status_t mikroe_a4988_init(void);

/***************************************************************************//**
 * @brief
 *    Activates the RESET input on the driver which sets the translator
 *    to a predefined Home state and turns off all of the FET outputs.
 *    After the reset cycle, the driver is enabled.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t mikroe_a4988_reset(void);

/***************************************************************************//**
 * @brief
 *    Enables or disable the motor driver (turns on or off all of the
 *    FET outputs of the driver).
 *
 * @param[in] en
 *    True or false to enable or disable, respectively.
 *
 * @return
 *    Error status
 ******************************************************************************/
sl_status_t mikroe_a4988_enable(bool en);

/***************************************************************************//**
 * @brief
 *    Returns with the inner state of the driver.
 *
 * @return
 *    Inner state of the driver.
 ******************************************************************************/
mikroe_a4988_state_t mikroe_a4988_get_state(void);

/***************************************************************************//**
 * @brief
 *    Sets the step frequency of the motor.
 *
 * @param[in] freq
 *    Desired step frequency in Hz. This parameter has to be less than the
 *    MIKROE_A4988_STEP_FREQUENCY_MAX value.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if speed is out of valid range.
 ******************************************************************************/
sl_status_t mikroe_a4988_set_step_frequency(uint32_t freq);

/***************************************************************************//**
 * @brief
 *    Sets the direction of rotation.
 *
 * @param[in] direction
 *    Direction of rotation.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if input is not mikroe_a4988_direction_t.
 ******************************************************************************/
sl_status_t mikroe_a4988_set_direction(mikroe_a4988_direction_t direction);

/***************************************************************************//**
 * @brief
 *    Starts the motor with the configured speed and direction.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_STATE if motor is not in idle state.
 *    SL_STATUS_INVALID_CONFIGURATION if RPM speed higher than the limit.
 ******************************************************************************/
sl_status_t mikroe_a4988_start(void);

/***************************************************************************//**
 * @brief
 *    Stops the motor.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_STATE if motor is not in running state.
 ******************************************************************************/
sl_status_t mikroe_a4988_stop(void);

/***************************************************************************//**
 * @brief
 *    Sleep the A4988 IC. This device supports the Sleep mode, which is
 *    activated by a LOW logic level on the SLEEP pin. This will power down the
 *    unused sections of the A4988 IC, reducing power consumption to a minimum.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_a4988_sleep(void);

/***************************************************************************//**
 * @brief
 *    Wake up the A4988 IC. After the wake-up event (logic HIGH on the SLEEP
 *    pin), at least 1ms of delay is required until the charge pump capacitors
 *    are recharged, allowing normal operation of the output stage drivers.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_a4988_wakeup(void);

/***************************************************************************//**
 * @brief
 *    Move the motor by a given number of steps. This is a
 *    non-blocking function. The speed of the movement can be set with the
 *    mikroe_a4988_set_step_frequency() function.
 *
 * @param[in] step_count
 *    Number of steps to move.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_STATE if motor is not in idle state.
 *    SL_STATUS_INVALID_CONFIGURATION if step frequency higher than the limit.
 *    SL_STATUS_INVALID_PARAMETER if sleeptimer handle parameter is NULL.
 ******************************************************************************/
sl_status_t mikroe_a4988_step(uint32_t step);

/** @} (end addtogroup Stepper motor driver) */

#ifdef __cplusplus
}
#endif

#endif /* STEPPER2_A4988_H_ */
