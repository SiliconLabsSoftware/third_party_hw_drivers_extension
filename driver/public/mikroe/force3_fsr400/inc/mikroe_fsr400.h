/***************************************************************************//**
 * @file mikroe_fsr400.h
 * @brief Mikroe FSR400 Prototypes
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
#ifndef MIKROE_FORCE3_FSR400_H_
#define MIKROE_FORCE3_FSR400_H_

#include "sl_status.h"
#include "force3.h"
#include "drv_i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FORCE3_FSR400_DEVICE_SLAVE_ADDR   FORCE3_DEVICE_SLAVE_ADDR

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_fsr400_init(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the IC2SPM instance used by platform functions.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_fsr400_set_i2csmp_instance(
  mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function writes a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 *
 * @param[in] reg : Start register address.
 * @param[in] tx_buf : Data to be written.
 * @param[in] tx_len : Number of bytes to be written.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if tx_buf is null
 *    SL_STATUS_OK - Success
 ******************************************************************************/
sl_status_t mikroe_fsr400_generic_write(uint8_t reg,
                                        uint8_t *tx_buf,
                                        uint8_t tx_len);

/***************************************************************************//**
 * @brief
 *    This function reads a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 *
 * @param[in] reg : Start register address.
 * @param[out] rx_buf : Output read data
 * @param[in] rx_len : Number of bytes to be read.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if rx_buf is null
 *    SL_STATUS_OK - Success
 ******************************************************************************/
sl_status_t mikroe_fsr400_generic_read (uint8_t reg, uint8_t *rx_buf,
                                        uint8_t rx_len);

/***************************************************************************//**
 * @brief
 *    This function reads raw data.
 * @param[in]
 * @param[out] data : Variable to store the data read from sensor
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if data is null
 *    SL_STATUS_OK - Success
 ******************************************************************************/
sl_status_t mikroe_fsr400_get_raw_data (uint16_t *data);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_FORCE3_FSR400_H_
