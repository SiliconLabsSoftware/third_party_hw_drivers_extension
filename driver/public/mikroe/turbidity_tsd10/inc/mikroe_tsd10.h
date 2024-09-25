/***************************************************************************//**
 * @file mikroe_tsd10.h
 * @brief mikroe_tsd10.h Prototypes
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

#ifndef MIKROE_TURBIDITY_H
#define MIKROE_TURBIDITY_H

#include "sl_status.h"
#include "drv_i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup turbidity Turbidity Click Driver
 * @brief API for configuring and manipulating Turbidity Click driver.
 * @{
 */

/*******************************************************************************
 * @brief
 *    This function initializes all necessary pins and peripherals used
 * for this click board.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_turbidity_init (mikroe_i2c_handle_t i2cspm_instance);

/*******************************************************************************
 * @brief
 *       This function reads a desired number of data bytes starting
 * by using I2C serial interface.
 *
 * @param[out] rx_buf : Output read data.
 * @param[in] rx_len : Number of bytes to be read.
 *
 * @return
 *    SL_STATUS_OK - Success.
 *    SL_STATUS_INVALID_PARAMETER if rx_buf is null or rx_len is zero.
 *    SL_STATUS_FAIL - Failed.
 ******************************************************************************/
sl_status_t mikroe_turbidity_generic_read (uint8_t *rx_buf, uint8_t rx_len);

/*******************************************************************************
 * @brief
 *    This function reads 12-bit ADC data of the
 * TSD-10 Turbidity Sensor on the Turbidity click board™.
 *
 * @param[out] adc_val : Output 12-bit ADC data.
 *
 * @return
 *    SL_STATUS_OK - Success.
 *    SL_STATUS_INVALID_PARAMETER if adc_val is null.
 *    SL_STATUS_FAIL - Failed.
 ******************************************************************************/
sl_status_t mikroe_turbidity_read_adc (uint16_t *adc_val);

/*******************************************************************************
 * @brief
 *    This function reads 12-bit ADC data and calculate the voltage of the
 * TSD-10 Turbidity Sensor on the Turbidity click board™.
 *
 * @param[out] voltage : Voltage ( mV ).
 *
 * @return
 *    SL_STATUS_OK - Success.
 *    SL_STATUS_INVALID_PARAMETER if voltage is null.
 *    SL_STATUS_FAIL - Failed.
 *
 ******************************************************************************/
sl_status_t mikroe_turbidity_get_adc_voltage (float *voltage);

/*******************************************************************************
 * @brief
 *    This function reads 12-bit ADC data and
 * calculate the Nephelometric Turbidity Units ( NTU ) of the
 * TSD-10 Turbidity Sensor on the Turbidity click board™.
 *
 * @param[out] ntu : NTU data ( from 0 to 4000 ).
 *
 * @return
 *    SL_STATUS_OK - Success.
 *    SL_STATUS_INVALID_PARAMETER if ntu is null.
 *    SL_STATUS_FAIL - Failed.
 ******************************************************************************/
sl_status_t mikroe_turbidity_get_ntu (float *ntu);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_TURBIDITY_H
