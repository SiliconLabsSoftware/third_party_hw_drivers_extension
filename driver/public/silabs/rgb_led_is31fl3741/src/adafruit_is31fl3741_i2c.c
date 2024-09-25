/***************************************************************************//**
 * @file adafruit_is31fl3741_i2c.c
 * @brief Adafruit RGB LED platform source file.
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

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include "adafruit_is31fl3741_i2c.h"

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------
static i2c_master_t *is31fl3741_i2c = NULL;

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 *  Assign the I2C instance for adafruit RGB LED.
 *****************************************************************************/
void adafruit_is31fl3741_i2c_set_instance(i2c_master_t *i2cspm)
{
  is31fl3741_i2c = i2cspm;
}

/**************************************************************************//**
 *  Write data to register of RGB LED IS31FL3741.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_i2c_write_reg(uint8_t reg_addr,
                                              uint8_t *data,
                                              uint8_t len)

{
  uint8_t i2c_write_data[len + 1];

  i2c_write_data[0] = reg_addr;
  memcpy(i2c_write_data + 1, data, len);

  if (I2C_MASTER_SUCCESS != i2c_master_write(is31fl3741_i2c,
                                             i2c_write_data,
                                             len + 1)) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Read data from register of RGB LED IS31FL3741.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_i2c_read_reg(uint8_t reg_addr,
                                             uint8_t *data,
                                             uint8_t len)

{
  if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(is31fl3741_i2c,
                                                       &reg_addr,
                                                       1,
                                                       data,
                                                       len)) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Write data to RGB LED IS31FL3741.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_i2c_write(uint8_t *data, uint8_t len)
{
  uint8_t i2c_write_data[len];

  memcpy(i2c_write_data, data, len);

  if (I2C_MASTER_SUCCESS != i2c_master_write(is31fl3741_i2c,
                                             i2c_write_data,
                                             len)) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Select device via I2C address.
 *****************************************************************************/
void adafruit_is31fl3741_i2c_select_device(uint8_t addr)
{
  i2c_master_set_slave_address(is31fl3741_i2c, addr);
}
