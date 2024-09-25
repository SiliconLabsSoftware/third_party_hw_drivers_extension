/***************************************************************************//**
 * @file adafruit_is31fl3741_i2c.h
 * @brief Adafruit RGB LED platform header file.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#ifndef ADAFRUIT_IS31FL3741_I2C_H_
#define ADAFRUIT_IS31FL3741_I2C_H_

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include <stdio.h>
#include "sl_status.h"
#include "drv_i2c_master.h"
#include "adafruit_is31fl3741_params.h"

// -----------------------------------------------------------------------------
//                               Macros
// -----------------------------------------------------------------------------

#define ADAFRUIT_IS31FL3741_COMMAND_REGISTER              0xFD
#define ADAFRUIT_IS31FL3741_COMMAND_REGISTER_LOCK         0xFE
#define ADAFRUIT_IS31FL3741_INT_MASK_REGISTER             0xF0
#define ADAFRUIT_IS31FL3741_INT_STATUS_REGISTER           0xF1
#define ADAFRUIT_IS31FL3741_ID_REGISTER                   0xFC

#define ADAFRUIT_IS31FL3741_FUNCREG_CONFIG                0x00
#define ADAFRUIT_IS31FL3741_FUNCREG_GCURRENT              0x01
#define ADAFRUIT_IS31FL3741_FUNCREG_RESET                 0x3F

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *  This function set the I2C instance for communication.
 *
 * @param[in] i2cspm
 *  I2CSPM Instance for communication.
 *
 ******************************************************************************/
void adafruit_is31fl3741_i2c_set_instance(i2c_master_t *i2cspm);

/***************************************************************************//**
 * @brief
 *  This function writes data to specific address of the RGB LED IS31FL3741.
 *
 * @param[in] reg_addr
 *  The address of the register.
 * @param[in] data
 *  Sent data.
 * @param[in] len
 *  Length of sent data.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_TRANSMIT if I2C transmit failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_i2c_write_reg(uint8_t reg_addr,
                                              uint8_t *data,
                                              uint8_t len);

/***************************************************************************//**
 * @brief
 *  This function reads data from specific address of the RGB LED IS31FL3741.
 *
 * @param[in] reg_addr
 *  The address of the register.
 * @param[out] data
 *  Sent data.
 * @param[in] len
 *  Length of sent data.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_TRANSMIT if I2C transmit failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_i2c_read_reg(uint8_t reg_addr,
                                             uint8_t *data,
                                             uint8_t len);

/***************************************************************************//**
 * @brief
 *  This function sends data to the RGB LED IS31FL3741.
 *
 * @param[in] data
 *  Sent data.
 * @param[in] len
 *  Length of the sent data.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_TRANSMIT if I2C transmit failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_i2c_write(uint8_t *data, uint8_t len);

/***************************************************************************//**
 * @brief
 *  Select device via I2C address.
 *
 * @param[in] addr
 *  I2C address.
 ******************************************************************************/

void adafruit_is31fl3741_i2c_select_device(uint8_t addr);

#endif /* ADAFRUIT_IS31FL3741_I2C_H_ */
