/***************************************************************************//**
 * @file mikroe_stretch.h
 * @brief SCL stretch Prototypes
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef MIKROE_STRETCH_H_
#define MIKROE_STRETCH_H_

#include "sl_status.h"
#include "stretch.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Stretch initialization function.
 * @details This function initializes all necessary pins and peripherals used
 * for this click board.
 * @return @li @c  SL_STATUS_OK - Success,
 *         @li @c  SL_STATUS_FAIL - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_stretch_init(mikroe_adc_handle_t handle);

/**
 * @brief Stretch read AN pin value function.
 * @details This function reads results of AD conversion of the AN pin.
 * @param[out] data_out : Output ADC result.
 * @return @li @c  SL_STATUS_OK - Success,
 *         @li @c  SL_STATUS_INVALID_PARAMETER - Invalid function parameter.
 * @note None.
 */
sl_status_t mikroe_stretch_generic_read(uint16_t *data_out);

/**
 * @brief Stretch turn on the LED function.
 * @details This function turn on the LED by set PWM pin.
 * @return Nothing.
 */
void mikroe_stretch_turn_on_led(void);

/**
 * @brief Stretch turn off the LED function.
 * @details This function turn off the LED by clear PWM pin.
 * @return Nothing.
 */
void mikroe_stretch_turn_off_led(void);

#ifdef __cplusplus
}
#endif
#endif // MIKROE_STRETCH_H_
