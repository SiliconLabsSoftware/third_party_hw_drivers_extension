/***************************************************************************//**
 * @file mikroe_veml6035.h
 * @brief veml6035 Prototypes
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef MIKROE_AMBIENT11_H_
#define MIKROE_AMBIENT11_H_

#include "sl_status.h"
#include "ambient11.h"

#ifdef __cplusplus
extern "C" {
#endif

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
sl_status_t mikroe_ambient11_init(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function writes data to the desired register.
 *
 * @param[in] wr_cmd : Command to be written.
 * @param[in] wr_data : Data to be written.
 *
 * @return void
 *
 ******************************************************************************/
void mikroe_ambient11_generic_write(uint8_t wr_cmd, uint16_t wr_data);

/***************************************************************************//**
 * @brief
 *    This function reads data from the desired register.
 *
 * @param[in] reg_addr : The desired register.
 *
 * @return Data in 'uint16_t' datatype.
 *
 ******************************************************************************/
uint16_t mikroe_ambient11_generic_read (uint8_t reg_addr);

/***************************************************************************//**
 * @brief
 *    This function is used to calculate ambiental illumination.
 *
 * @note The devices resolution depends on settings applied.
 * User should consult the datasheet and choose resolution value
 * that corresponds to the settings applied.
 *
 * @param[in] typical_res : float value that defines resolution.
 *
 * @return Ambiental illumination in 'float' datatype
 *
 ******************************************************************************/
float mikroe_ambient11_calc_illumination (float typical_res);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_RGB_H_ */
