/***************************************************************************//**
 * @file mikroe_a3967.h
 * @brief Mikroe Stepper driver.
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

#ifndef MIKROE_A3967_H_
#define MIKROE_A3967_H_

#include <stdbool.h>
#include "sl_status.h"

#include "drv_digital_out.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIKROE_A3967_STEP_MIN_FREQUENCY   1
#define MIKROE_A3967_STEP_MAX_FREQUENCY   500000

/***************************************************************************//**
 * @brief
 *    Stepper direction of rotation. Values :
 *  - <b>MIKROE_A3967_CLOCKWISE (0x0):</b> Stepper rotate clockwise
 *  - <b>MIKROE_A3967_COUNTERCLOCKWISE (0x1):</b> Stepper
 *    rotate counterclockwise
 ******************************************************************************/
typedef enum mikroe_a3967_direction {
  MIKROE_A3967_CLOCKWISE = 0,
  MIKROE_A3967_COUNTERCLOCKWISE = 1,
} mikroe_a3967_direction_t;

/***************************************************************************//**
 * @brief
 *    Stepper modes. Values :
 *     - <b>MIKROE_A3967_FULL_STEP (0x0):</b> full step mode
 *     - <b>MIKROE_A3967_HALF_STEP (0x1):</b> half step mode
 *     - <b>MIKROE_A3967_QUARTER_STEP (0x2):</b> quarter step mode
 *     - <b>MIKROE_A3967_EIGHTH_STEP (0x3):</b> eighth step mode
 ******************************************************************************/
typedef enum mikroe_a3967_mode {
  MIKROE_A3967_FULL_STEP = 0,
  MIKROE_A3967_HALF_STEP = 1,
  MIKROE_A3967_QUARTER_STEP = 2,
  MIKROE_A3967_EIGHTH_STEP = 3
} mikroe_a3967_mode_t;
// -----------------------------------------------------------------------------
//                          Public Function Declarations
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *    Stepper initialization function. This function must call first before
 *    using other driver APIs.
 *    If not have any config, Stepper will operation in default configuraton:
 *    - Default mode: MIKROE_A3967_FULL_STEP
 *    - Default direction: MIKROE_A3967_CLOCKWISE
 *    - Default frequency: MIKROE_A3967_STEP_FREQUENCY
 * @return
 *    SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t mikroe_a3967_init(void);

/***************************************************************************//**
 * @brief
 *    Configure stepper motor mode to run in full/half and micro-steps
 * @param[in] mode Enum @ref a3967_step_mode_t. Values :
 *     - <b>MIKROE_A3967_FULL_STEP (0x0):</b> full step mode
 *     - <b>MIKROE_A3967_HALF_STEP (0x1):</b> half step mode
 *     - <b>MIKROE_A3967_QUARTER_STEP (0x2):</b> quarter step mode
 *     - <b>MIKROE_A3967_EIGHTH_STEP (0x3):</b> eighth step mode
 *     - Default: MIKROE_A3967_FULL_STEP
 * @return
 *    SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t mikroe_a3967_config_mode(mikroe_a3967_mode_t mode);

/***************************************************************************//**
 * @brief
 *    Sets the direction of rotation.
 *
 * @param[in] dir Direction of rotation. Values:
 *     - <b>MIKROE_A3967_CLOCKWISE (0x0):</b> rotate clockwise
 *     - <b>MIKROE_A3967_COUNTERCLOCKWISE (0x1):</b> rotate counterclockwise
 *     - Default: MIKROE_A3967_CLOCKWISE
 * @return
 *    SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t mikroe_a3967_set_direction(mikroe_a3967_direction_t dir);

/***************************************************************************//**
 * @brief
 *    Sets the speed of step motor.
 *
 * @param[in] freq PWM Frequency in STEP pin
 *            Value depend on motor used.
 *            Max frequency a3967 support is 500 kHz.
 *
 * @return
 *    SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t mikroe_a3967_set_step_frequency(uint32_t freq);

/***************************************************************************//**
 * @brief
 *    Starts the motor with the configured direction.
 *    This function will make the motor run until call @ref mikroe_a3967_stop
 *    Or use @ref mikroe_a3967_step to run predefined step.
 ******************************************************************************/
sl_status_t mikroe_a3967_start(void);

/***************************************************************************//**
 * @brief
 *    Stops the motor.
 ******************************************************************************/
void mikroe_a3967_stop(void);

/***************************************************************************//**
 * @brief
 *    Get the current state of motor.
 * @return
 *    - <b>false (0x0):</b> motor is not running
 *    - <b>true (0x1):</b> motor is running
 ******************************************************************************/
bool mikroe_a3967_get_state(void);

/***************************************************************************//**
 * @brief
 *    Move the motor by a given number of steps. This is a
 *    non-blocking function. Device must be wait until the
 *    motor finish rotation.
 *
 * @param[in] step
 *    Number of steps to move.
 *
 * @return
 *    SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t mikroe_a3967_step(uint16_t step);

#ifdef __cplusplus
}
#endif
#endif // MIKROE_A3967_H_
