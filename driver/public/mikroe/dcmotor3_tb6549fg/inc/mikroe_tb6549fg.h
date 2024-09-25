/***************************************************************************//**
 * @file mikroe_tb6549fg.h
 * @brief SCL TB6549FG Prototypes
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

#ifndef MIKROE_TB6549FG_H_
#define MIKROE_TB6549FG_H_

#include "sl_status.h"
#include "drv_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup mikroe_TB6549FG - DC Motor 3 Click
 * @brief  The integration of the Mikroe SDK Click extension TB6549FG driver.
 *         (DC Motor 3 Click board.)
 *
 *    @n @section tb6549fg_example TB6549FG example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "mikroe_tb6549fg.h"
 * #include "sl_pwm_instances.h"
 * #include "app_log.h"
 * #include "sl_sleeptimer.h"
 *    int main(void)
 *    {
 *      ...
 *
 *    if (mikroe_tb6549fg_init(sl_pwm_mikroe) == SL_STATUS_OK) {
 *        app_log("DC Motor 3 Click initializes successfully\n");
 *    }
 *
 *    mikroe_tb6549fg_set_duty_cycle(0.0);
 *    mikroe_tb6549fg_pwm_start();
 *    sl_sleeptimer_delay_millisecond(1000);
 *    mikroe_tb6549fg_enable();
 *    sl_sleeptimer_delay_millisecond(1000);
 *
 *    while (true) {
 *
 *      mikroe_tb6549fg_clockwise();
 *      app_log(">>>> CLOCKWISE \n");
 *      mikroe_tb6549fg_enable();
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

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
sl_status_t mikroe_tb6549fg_init(mikroe_pwm_handle_t pwm_instance);

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
sl_status_t mikroe_tb6549fg_set_pwm_instance(mikroe_pwm_handle_t pwm_instance);

/***************************************************************************//**
 * @brief
 *    This function sets duty cycle for DC Motor 3 click
 *
 * @param[in] duty_cycle
 *    Duty cycle value
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_tb6549fg_set_duty_cycle(float duty_cycle);

/***************************************************************************//**
 * @brief
 *    This function is used to stop pwm for DC Motor 3 click
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_tb6549fg_pwm_stop(void);

/***************************************************************************//**
 * @brief
 *    This function is used to stop pwm for DC Motor 3 click
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_tb6549fg_pwm_start(void);

/***************************************************************************//**
 * @brief
 *    This function is used to enable DC Motor 3 click
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_tb6549fg_enable(void);

/***************************************************************************//**
 * @brief
 *    This function is used to disable the engine function
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_tb6549fg_standby(void);

/***************************************************************************//**
 * @brief
 *    This function is used to set the direction of rotation in the counter
 *    clockwise direction function
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_tb6549fg_counter_clockwise(void);

/***************************************************************************//**
 * @brief
 *    This function is used to set the direction of rotation in the clockwise
 *    direction function
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_tb6549fg_clockwise(void);

/***************************************************************************//**
 * @brief
 *    This function is used to brake the engine function
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_tb6549fg_short_brake(void);

/***************************************************************************//**
 * @brief
 *    This function is used to stop the engine function
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_tb6549fg_stop(void);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_TB6549FG_H_
