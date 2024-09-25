/***************************************************************************//**
 * @file mirkroe_emg.h
 * @brief SCL EMG Prototypes
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

#ifndef MIKROE_EMG_H_
#define MIKROE_EMG_H_

#include "sl_status.h"
#include "drv_analog_in.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
* @brief EMG initialization function.
* @details This function initializes all necessary pins and peripherals used
* for this click board.
*
* @param[in] adc
*   ADC or iADC instance
*
* @return SL_STATUS_OK on Success, otherwise on failure.
*
******************************************************************************/
sl_status_t mikroe_emg_init(mikroe_adc_handle_t handle);

/**************************************************************************//**
* @brief EMG read AN pin value function.
* @details This function reads results of AD conversion of the AN pin.
*
* @param[out] data_out
*    Output ADC result.
*
* @return SL_STATUS_OK on Success, otherwise on failure.
*
******************************************************************************/
sl_status_t mikroe_emg_read_an_pin_value(uint16_t *data_out);

/**************************************************************************//**
* @brief EMG read AN pin voltage level function.
* @details This function reads results of AD conversion of the AN pin and
* converts them to proportional voltage level
*
* @param[in] data_out
*    Output voltage level of the analog pin [V].
*
* @return SL_STATUS_OK on Success, otherwise on failure.
*
******************************************************************************/
sl_status_t mikroe_emg_read_an_pin_voltage(float *data_out);

/** @} (end addtogroup EMG) */

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_EMG_H_ */
