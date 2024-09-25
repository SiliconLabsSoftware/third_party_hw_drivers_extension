/***************************************************************************//**
 * @file sparkfun_vcnl4040_platform.c
 * @brief platform for SPARKFUN VCNL4040 Proximity sensor
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

#include <stdio.h>
#include "sl_status.h"
#include "sparkfun_vcnl4040_platform.h"
#include "drv_i2c_master.h"

// -----------------------------------------------------------------------------
//                          Public functions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 *  Read two consecutive bytes from a given 'command code' location
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_i2c_read_command(i2c_master_t *i2cspm,
                                               uint8_t command,
                                               uint16_t *data)
{
  uint8_t pdata[2];

  if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(i2cspm, &command, 1,
                                                       pdata, 2)) {
    return SL_STATUS_TRANSMIT;
  }
  *data = (uint16_t)(pdata[1] << 8) + pdata[0];

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Write two consecutive bytes to a given 'command code' location
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_i2c_write_command(i2c_master_t *i2cspm,
                                                uint8_t command,
                                                uint16_t data)
{
  uint8_t send_data[3];

  send_data[0] = command;
  send_data[1] = (uint8_t)(data & 0xFF);
  send_data[2] = (uint8_t)(data >> 8);

  if (I2C_MASTER_SUCCESS != i2c_master_write(i2cspm, send_data, 3)) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Read lower byte from a given 'command code' location
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_i2c_read_command_lower(i2c_master_t *i2cspm,
                                                     uint8_t command,
                                                     uint8_t *data)
{
  sl_status_t ret;
  uint16_t pdata = 0;

  ret = sparkfun_vcnl4040_i2c_read_command(i2cspm, command, &pdata);
  *data = (uint8_t)(pdata & 0xFF);

  return ret;
}

/***************************************************************************//**
 *  Read upper byte from a given 'command code' location
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_i2c_read_command_upper(i2c_master_t *i2cspm,
                                                     uint8_t command,
                                                     uint8_t *data)
{
  uint16_t pdata = 0;
  sl_status_t ret;

  ret = sparkfun_vcnl4040_i2c_read_command(i2cspm, command, &pdata);
  *data = (uint8_t)(pdata >> 8);

  return ret;
}

/***************************************************************************//**
 *  Write lower byte to a given 'command code' location
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_i2c_write_command_lower(i2c_master_t *i2cspm,
                                                      uint8_t command,
                                                      uint8_t data)
{
  uint16_t command_value;
  sl_status_t ret;

  ret = sparkfun_vcnl4040_i2c_read_command(i2cspm, command, &command_value);

  if (ret != SL_STATUS_OK) {
    return ret;
  }
  command_value &= 0xFF00;
  command_value |= (uint16_t)(data);

  return sparkfun_vcnl4040_i2c_write_command(i2cspm,
                                             command,
                                             command_value);
}

/***************************************************************************//**
 *  Write upper byte to a given 'command code' location
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_i2c_write_command_upper(i2c_master_t *i2cspm,
                                                      uint8_t command,
                                                      uint8_t data)
{
  uint16_t command_value;
  sl_status_t ret;

  if (i2cspm == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  ret = sparkfun_vcnl4040_i2c_read_command(i2cspm, command, &command_value);

  if (ret != SL_STATUS_OK) {
    return ret;
  }
  command_value &= 0x00FF;
  command_value |= (uint16_t)data << 8;

  return sparkfun_vcnl4040_i2c_write_command(i2cspm, command, command_value);
}

/***************************************************************************//**
 *  Masked write of upper/lower byte at a given 'command code' location
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_i2c_masked_write_command(i2c_master_t *i2cspm,
                                                       uint8_t command,
                                                       bool command_height,
                                                       uint8_t mask,
                                                       uint8_t data)
{
  uint8_t reg_value;
  sl_status_t ret;

  if (i2cspm == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  if (command_height == LOWER) {
    if (sparkfun_vcnl4040_i2c_read_command_lower(i2cspm, command,
                                                 &reg_value) != SL_STATUS_OK) {
      return SL_STATUS_FAIL;
    }
    reg_value &= mask;
    reg_value |= data;
    ret = sparkfun_vcnl4040_i2c_write_command_lower(i2cspm, command, data);

    return ret;
  } else {
    if (sparkfun_vcnl4040_i2c_read_command_upper(i2cspm, command,
                                                 &reg_value) != SL_STATUS_OK) {
      return SL_STATUS_FAIL;
    }
    reg_value &= mask;
    reg_value |= data;
    ret = sparkfun_vcnl4040_i2c_write_command_upper(i2cspm, command, data);

    return ret;
  }
}
