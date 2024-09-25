/***************************************************************************//**
 * @file mikroe_si8711cc.h
 * @brief mikroe_si8711cc.h
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
#ifndef MIKROE_SI8711CC_H_
#define MIKROE_SI8711CC_H_
#include "sl_status.h"
#include "drv_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] pwm_instance
 *    PWM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_si8711cc_init(mikroe_pwm_handle_t pwm_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the PWM instance used by platform functions.
 *
 * @param[in] pwm_instance
 *    PWM instance, default: sl_pwm_mikroe
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if pwm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_si8711cc_set_pwm_instance(
  mikroe_pwm_handle_t pwm_instance);

/***************************************************************************//**
 * @brief
 *    This function sets duty cycle for PWM Driver
 *
 * @param[in] duty_cycle
 *    duty cycle value
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL
 ******************************************************************************/
sl_status_t mikroe_si8711cc_set_duty_cycle(float duty_cycle);

/***************************************************************************//**
 * @brief
 *    This function sets frequency for PWM Driver
 *
 * @param[in] frequency
 *    frequency value
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL
 ******************************************************************************/
sl_status_t mikroe_si8711cc_set_frequency(uint16_t frequency);

/***************************************************************************//**
 * @brief
 *    This function is used to stop pwm for PWM Driver
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL
 ******************************************************************************/
sl_status_t mikroe_si8711cc_pwm_stop(void);

/***************************************************************************//**
 * @brief
 *    This function is used to start pwm for PWM Driver
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL
 ******************************************************************************/
sl_status_t mikroe_si8711cc_pwm_start(void);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_SI8711CC_H_ */
