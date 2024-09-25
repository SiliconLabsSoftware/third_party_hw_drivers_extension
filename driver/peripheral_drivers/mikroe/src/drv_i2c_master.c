/***************************************************************************//**
 * @file drv_i2c_master.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - I2C Master
 * @version 1.0.0
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

#include "sl_i2cspm.h"
#include "drv_i2c_master.h"
#include "sl_status.h"

static i2c_master_t *_owner = NULL;
static uint32_t last_i2c_speed_used;

static void i2c_master_config_speed(i2c_master_t *obj);
static err_t _acquire(i2c_master_t *obj, bool obj_open_state);

void i2c_master_configure_default(i2c_master_config_t *config)
{
  config->addr = 0;

  config->sda = 0xFFFFFFFF;
  config->scl = 0xFFFFFFFF;

  config->speed = I2C_MASTER_SPEED_STANDARD;
  config->timeout_pass_count = 10000;
}

err_t i2c_master_open(i2c_master_t *obj, i2c_master_config_t *config)
{
  i2c_master_config_t *p_config = &obj->config;
  memcpy(p_config, config, sizeof(i2c_master_config_t));

  return _acquire(obj, true);
}

err_t i2c_master_set_speed(i2c_master_t *obj, uint32_t speed)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  obj->config.speed = speed;
  // Configure i2c speed
  i2c_master_config_speed(obj);

  return I2C_MASTER_SUCCESS;
}

err_t i2c_master_set_timeout(i2c_master_t *obj, uint16_t timeout_pass_count)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  obj->config.timeout_pass_count = timeout_pass_count;
  // TODO Set timeout
  return I2C_MASTER_SUCCESS;
}

err_t i2c_master_set_slave_address(i2c_master_t *obj, uint8_t address)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  obj->config.addr = address;
  return I2C_MASTER_SUCCESS;
}

err_t i2c_master_write(i2c_master_t *obj,
                       uint8_t *write_data_buf,
                       size_t len_write_data)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  I2C_TransferSeq_TypeDef seq;

  if (last_i2c_speed_used != obj->config.speed) {
    i2c_master_config_speed(obj);
  }

  seq.addr = obj->config.addr << 1;
  seq.flags = I2C_FLAG_WRITE;

  // Write buffer
  seq.buf[0].data = write_data_buf;
  seq.buf[0].len = len_write_data;

  if (I2CSPM_Transfer((sl_i2cspm_t *)obj->handle, &seq) != i2cTransferDone) {
    return I2C_MASTER_ERROR;
  }
  return I2C_MASTER_SUCCESS;
}

err_t i2c_master_read(i2c_master_t *obj,
                      uint8_t *read_data_buf,
                      size_t len_read_data)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  I2C_TransferSeq_TypeDef seq;

  if (last_i2c_speed_used != obj->config.speed) {
    i2c_master_config_speed(obj);
  }

  seq.addr = obj->config.addr << 1;
  seq.flags = I2C_FLAG_READ;

  // Read buffer
  seq.buf[0].data = read_data_buf;
  seq.buf[0].len = len_read_data;

  if (I2CSPM_Transfer((sl_i2cspm_t *)obj->handle, &seq) != i2cTransferDone) {
    return I2C_MASTER_ERROR;
  }
  return I2C_MASTER_SUCCESS;
}

err_t i2c_master_write_then_read(i2c_master_t *obj,
                                 uint8_t *write_data_buf,
                                 size_t len_write_data,
                                 uint8_t *read_data_buf,
                                 size_t len_read_data)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  I2C_TransferSeq_TypeDef seq;

  if (last_i2c_speed_used != obj->config.speed) {
    i2c_master_config_speed(obj);
  }

  seq.addr = obj->config.addr << 1;
  seq.flags = I2C_FLAG_WRITE_READ;

  // Write buffer
  seq.buf[0].data = write_data_buf;
  seq.buf[0].len = len_write_data;

  // Read buffer
  seq.buf[1].data = read_data_buf;
  seq.buf[1].len = len_read_data;

  if (I2CSPM_Transfer((sl_i2cspm_t *)obj->handle, &seq) != i2cTransferDone) {
    return I2C_MASTER_ERROR;
  }
  return I2C_MASTER_SUCCESS;
}

static err_t _acquire(i2c_master_t *obj, bool obj_open_state)
{
  err_t status = ACQUIRE_SUCCESS;

  if ((obj_open_state == true) && (_owner == obj)) {
    return ACQUIRE_FAIL;
  }

  if (_owner != obj) {
    _owner = obj;
  }

  return status;
}

void i2c_master_close(i2c_master_t *obj)
{
  obj->handle = NULL;
  _owner = NULL;
}

static void i2c_master_config_speed(i2c_master_t *obj)
{
  uint32_t speed;
  I2C_ClockHLR_TypeDef i2c_mode;

  last_i2c_speed_used = obj->config.speed;

  switch (obj->config.speed) {
    default:
    case I2C_MASTER_SPEED_STANDARD:
      speed = I2C_FREQ_STANDARD_MAX;
      i2c_mode = i2cClockHLRStandard;
      break;
    case I2C_MASTER_SPEED_FULL:
      speed = I2C_FREQ_FAST_MAX;
      i2c_mode = i2cClockHLRAsymetric;
      break;
    case I2C_MASTER_SPEED_FAST:
      speed = I2C_FREQ_FASTPLUS_MAX;
      i2c_mode = i2cClockHLRFast;
      break;
  }
  // Set reference clock to zero to get the default reference clock
  I2C_BusFreqSet((sl_i2cspm_t *)obj->handle, 0, speed, i2c_mode);
}

// ------------------------------------------------------------------------- END
