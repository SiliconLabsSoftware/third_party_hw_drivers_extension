/***************************************************************************//**
 * @file mikroe_pl_n823_01.h
 * @brief SCL Mikroe PIR PL_N823_01 header file
 * @version 1.2.0
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

#ifndef MIKROE_PL_N823_01_H_
#define MIKROE_PL_N823_01_H_

#include "sl_status.h"
#include "drv_i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief Initialization function.
 *
 * @description This function initializes all necessary pins and peripherals
 *   used for this click.
 ******************************************************************************/
sl_status_t mikroe_pl_n823_01_init(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the IC2 instance used by platform functions.
 *
 * @param[in] i2c_instance
 *    I2C instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_pl_n823_01_set_i2c_instance(mikroe_i2c_handle_t instance);

/***************************************************************************//**
 * @brief Generic write function.
 *
 * @param reg          Register address.
 * @param data_buf     Data buf to be written.
 * @param len          Number of the bytes in data buf.
 *
 * @description This function writes data  to the desired register.
 ******************************************************************************/
sl_status_t mikroe_pl_n823_01_generic_write(uint8_t reg, uint8_t *data_buf,
                                            uint8_t len);

/***************************************************************************//**
 * @brief Generic read function.
 *
 * @param reg          Register address.
 * @param data_buf     Output data buf
 * @param len          Number of the bytes to be read
 *
 * @description This function reads data from the desired register.
 ******************************************************************************/
sl_status_t mikroe_pl_n823_01_generic_read (uint8_t reg, uint8_t *data_buf,
                                            uint8_t len);

/***************************************************************************//**
 * @brief Reading 12bit ADC value.
 *
 * This function reads single ADC conversion data
 ******************************************************************************/
uint16_t mikroe_pl_n823_01_get_adc(void);

/***************************************************************************//**
 * @brief Reading register.
 *
 * This function reads register
 ******************************************************************************/
uint16_t mikroe_pl_n823_01_reg_read(void);

/***************************************************************************//**
 * @brief Reading ADC data in mili Volts
 *
 * @param ref_voltage reference voltage in miliVolts (3300 or 5000 miliVolts).
 *
 * This function reads 12bit ADC data and convets it to miliVolts
 ******************************************************************************/
float mikroe_pl_n823_01_get_mili_volt(uint16_t ref_voltage);

/***************************************************************************//**
 * @brief Scaling ADC results
 *
 * @param in_val    ADC conversion raw data
 * @param out_min   lower limit of output data
 * @param out_max   upper limit of output data
 *
 * This function scales ADC conversion data (inVal) to desired range (from
 *   outMin to outMax) and returns scaled data (outVal)
 ******************************************************************************/
float mikroe_pl_n823_01_scale_results (uint16_t in_val, uint16_t out_min,
                                       uint16_t out_max);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_PL_N823_01_H_
