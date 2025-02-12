/***************************************************************************//**
 * @file mikroe_cmt_8540s_smt.h
 * @brief SCL CMT_8540S_SMT Prototypes
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

#ifndef MIKROE_CMT_8540S_SMT_H_
#define MIKROE_CMT_8540S_SMT_H_

#include "sl_status.h"
#include "buzz2.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @addtogroup cmt_8540s_smt CMT-8540S-SMT - magnetic buzzer transducer
 * @brief  Driver for the the CMT-8540S-SMT magnetic buzzer transducer.
 *    @n @section cmt_8540s_smt_example CMT-8540S-SMT example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "sl_pwm_instances.h"
 * #include "mikroe_cmt_8540s_smt.h"
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize buzzer and set mode
 *      mikroe_cmt_8540s_smt_init(&sl_pwm_mikroe);
 *      mikroe_cmt_8540s_smt_set_duty_cycle(0.0);
 *      mikroe_cmt_8540s_smt_pwm_start();
 *
 *      mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
 *
 *      while (true) {
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/**************************************************************************//**
 * @addtogroup mikroe_buzz2_freq Mikroe Buzz2 Freq
 * @brief Freq definitions
 * @{
 ******************************************************************************/
#define MIKROE_BUZZ2_NOTE_C2                65
#define MIKROE_BUZZ2_NOTE_Db2               69
#define MIKROE_BUZZ2_NOTE_D2                73
#define MIKROE_BUZZ2_NOTE_Eb2               78
#define MIKROE_BUZZ2_NOTE_E2                82
#define MIKROE_BUZZ2_NOTE_F2                87
#define MIKROE_BUZZ2_NOTE_Gb2               93
#define MIKROE_BUZZ2_NOTE_G2                98
#define MIKROE_BUZZ2_NOTE_Ab2               104
#define MIKROE_BUZZ2_NOTE_A2                110
#define MIKROE_BUZZ2_NOTE_Bb2               117
#define MIKROE_BUZZ2_NOTE_B2                123
#define MIKROE_BUZZ2_NOTE_C3                131
#define MIKROE_BUZZ2_NOTE_Db3               139
#define MIKROE_BUZZ2_NOTE_D3                147
#define MIKROE_BUZZ2_NOTE_Eb3               156
#define MIKROE_BUZZ2_NOTE_E3                165
#define MIKROE_BUZZ2_NOTE_F3                175
#define MIKROE_BUZZ2_NOTE_Gb3               185
#define MIKROE_BUZZ2_NOTE_G3                196
#define MIKROE_BUZZ2_NOTE_Ab3               208
#define MIKROE_BUZZ2_NOTE_A3                220
#define MIKROE_BUZZ2_NOTE_AS3               233
#define MIKROE_BUZZ2_NOTE_B3                247
#define MIKROE_BUZZ2_NOTE_C4                262
#define MIKROE_BUZZ2_NOTE_Db4               277
#define MIKROE_BUZZ2_NOTE_D4                294
#define MIKROE_BUZZ2_NOTE_Eb4               311
#define MIKROE_BUZZ2_NOTE_E4                330
#define MIKROE_BUZZ2_NOTE_F4                349
#define MIKROE_BUZZ2_NOTE_Gb4               370
#define MIKROE_BUZZ2_NOTE_G4                392
#define MIKROE_BUZZ2_NOTE_Ab4               415
#define MIKROE_BUZZ2_NOTE_A4                440
#define MIKROE_BUZZ2_NOTE_Bb4               466
#define MIKROE_BUZZ2_NOTE_B4                494
#define MIKROE_BUZZ2_NOTE_C5                523
#define MIKROE_BUZZ2_NOTE_Db5               554
#define MIKROE_BUZZ2_NOTE_D5                587
#define MIKROE_BUZZ2_NOTE_Eb5               622
#define MIKROE_BUZZ2_NOTE_E5                659
#define MIKROE_BUZZ2_NOTE_F5                698
#define MIKROE_BUZZ2_NOTE_Gb5               740
#define MIKROE_BUZZ2_NOTE_G5                784
#define MIKROE_BUZZ2_NOTE_Ab5               831
#define MIKROE_BUZZ2_NOTE_A5                880
#define MIKROE_BUZZ2_NOTE_Bb5               932
#define MIKROE_BUZZ2_NOTE_B5                988
#define MIKROE_BUZZ2_NOTE_C6                1047
#define MIKROE_BUZZ2_NOTE_Db6               1109
#define MIKROE_BUZZ2_NOTE_D6                1175
#define MIKROE_BUZZ2_NOTE_Eb6               1245
#define MIKROE_BUZZ2_NOTE_E6                1319
#define MIKROE_BUZZ2_NOTE_F6                1397
#define MIKROE_BUZZ2_NOTE_Gb6               1480
#define MIKROE_BUZZ2_NOTE_G6                1568
#define MIKROE_BUZZ2_NOTE_Ab6               1661
#define MIKROE_BUZZ2_NOTE_A6                1760
#define MIKROE_BUZZ2_NOTE_Bb6               1865
#define MIKROE_BUZZ2_NOTE_B6                1976
#define MIKROE_BUZZ2_NOTE_C7                2093
#define MIKROE_BUZZ2_NOTE_Db7               2217
#define MIKROE_BUZZ2_NOTE_D7                2349
#define MIKROE_BUZZ2_NOTE_Eb7               2489
#define MIKROE_BUZZ2_NOTE_E7                2637
#define MIKROE_BUZZ2_NOTE_F7                2794
#define MIKROE_BUZZ2_NOTE_Gb7               2960
#define MIKROE_BUZZ2_NOTE_G7                3136
#define MIKROE_BUZZ2_NOTE_Ab7               3322
#define MIKROE_BUZZ2_NOTE_A7                3520
#define MIKROE_BUZZ2_NOTE_Bb7               3729
#define MIKROE_BUZZ2_NOTE_B7                3951
#define MIKROE_BUZZ2_NOTE_C8                4186
#define MIKROE_BUZZ2_NOTE_Db8               4435
#define MIKROE_BUZZ2_NOTE_D8                4699
#define MIKROE_BUZZ2_NOTE_Eb8               4978

/** @} (end addtogroup mikroe_buzz2_freq) */

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
sl_status_t mikroe_cmt_8540s_smt_init(mikroe_pwm_handle_t pwm_instance);

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
sl_status_t mikroe_cmt_8540s_smt_set_pwm_instance(
  mikroe_pwm_handle_t pwm_instance);

/***************************************************************************//**
 * @brief
 *    This function sets duty cycle for Buzz2 click
 *
 * @param[in] duty_cycle
 *    duty cycle value
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL
 ******************************************************************************/
sl_status_t mikroe_cmt_8540s_smt_set_duty_cycle (float duty_cycle);

/***************************************************************************//**
 * @brief
 *    This function is used to stop pwm for Buzz2 click
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL
 ******************************************************************************/
sl_status_t mikroe_cmt_8540s_smt_pwm_stop (void);

/***************************************************************************//**
 * @brief
 *    This function is used to start pwm for Buzz2 click
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL
 ******************************************************************************/
sl_status_t mikroe_cmt_8540s_smt_pwm_start (void);

/***************************************************************************//**
 * @brief
 *    This function is used to play sound
 *
 * @param[in] freq
 *    Frequency of sound
 *
 * @param[in] level
 *    Level of sound (up to 1000)
 *
 * @param[in] duration
 *    Duration of sound (up to 1000)
 *
 * @return
 *    none
 ******************************************************************************/
void mikroe_cmt_8540s_smt_play_sound (uint16_t freq,
                                      uint16_t level,
                                      uint16_t duration);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup cmt_8540s_smt) */
#endif // MIKROE_CMT_8540S_SMT_H_
