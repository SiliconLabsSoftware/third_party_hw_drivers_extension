/***************************************************************************//**
 * @file drv_i2c_master_si91x.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - I2C Master for Si91x
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

#include "drv_i2c_master.h"
#include "sl_si91x_i2c.h"
#include "sl_status.h"

#define TX_THRESHOLD       0       // tx threshold value
#define RX_THRESHOLD       0       // rx threshold value

static i2c_master_t *_owner = NULL;
static uint32_t last_i2c_speed_used;

static err_t _acquire(i2c_master_t *obj, bool obj_open_state);
static err_t i2c_master_set_configuration(i2c_master_t *obj);

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
  sl_i2c_status_t i2c_status;
  sl_i2c_instance_t i2c_handle = *(sl_i2c_instance_t *)obj->handle;

  i2c_master_config_t *p_config = &obj->config;
  memcpy(p_config, config, sizeof(i2c_master_config_t));

  if (_acquire(obj, true) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  if (i2c_master_set_configuration(obj) != I2C_MASTER_SUCCESS) {
    return I2C_MASTER_ERROR;
  }
  // Configuring RX and TX FIFO thresholds
  i2c_status
    = sl_i2c_driver_configure_fifo_threshold(i2c_handle,
                                             TX_THRESHOLD,
                                             RX_THRESHOLD);
  if (i2c_status != SL_I2C_SUCCESS) {
    return I2C_MASTER_ERROR;
  }

  return I2C_MASTER_SUCCESS;
}

err_t i2c_master_set_speed(i2c_master_t *obj, uint32_t speed)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  obj->config.speed = speed;
  // Configure i2c speed
  return i2c_master_set_configuration(obj);
}

err_t i2c_master_set_timeout(i2c_master_t *obj, uint16_t timeout_pass_count)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    obj->config.timeout_pass_count = timeout_pass_count;
    // TODO Set timeout
    return I2C_MASTER_SUCCESS;
  }

  return I2C_MASTER_ERROR;
}

err_t i2c_master_set_slave_address(i2c_master_t *obj, uint8_t address)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    obj->config.addr = address;
    return I2C_MASTER_SUCCESS;
  }

  return I2C_MASTER_ERROR;
}

err_t i2c_master_write(i2c_master_t *obj,
                       uint8_t *write_data_buf,
                       size_t len_write_data)
{
  sl_i2c_status_t i2c_status;
  sl_i2c_instance_t i2c_handle = *(sl_i2c_instance_t *)obj->handle;

  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  if (last_i2c_speed_used != obj->config.speed) {
    i2c_master_set_configuration(obj);
  }

  i2c_status = sl_i2c_driver_send_data_blocking(i2c_handle,
                                                obj->config.addr,
                                                write_data_buf,
                                                len_write_data);
  if (i2c_status != SL_I2C_SUCCESS) {
    return I2C_MASTER_ERROR;
  }

  sl_si91x_i2c_wait_till_i2c_is_idle(i2c_handle);
  return I2C_MASTER_SUCCESS;
}

err_t i2c_master_read(i2c_master_t *obj,
                      uint8_t *read_data_buf,
                      size_t len_read_data)
{
  sl_i2c_status_t i2c_status;
  sl_i2c_instance_t i2c_handle = *(sl_i2c_instance_t *)obj->handle;

  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  if (last_i2c_speed_used != obj->config.speed) {
    i2c_master_set_configuration(obj);
  }

  i2c_status
    = sl_i2c_driver_receive_data_blocking(i2c_handle,
                                          obj->config.addr,
                                          read_data_buf,
                                          len_read_data);
  if (i2c_status != SL_I2C_SUCCESS) {
    return I2C_MASTER_ERROR;
  }
  sl_si91x_i2c_wait_till_i2c_is_idle(i2c_handle);
  return I2C_MASTER_SUCCESS;
}

err_t i2c_master_write_then_read(i2c_master_t *obj,
                                 uint8_t *write_data_buf,
                                 size_t len_write_data,
                                 uint8_t *read_data_buf,
                                 size_t len_read_data)
{
  sl_i2c_status_t i2c_status;
  sl_i2c_instance_t i2c_handle = *(sl_i2c_instance_t *)obj->handle;

  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return I2C_MASTER_ERROR;
  }

  if (last_i2c_speed_used != obj->config.speed) {
    i2c_master_set_configuration(obj);
  }

  // Enabling combined format transfer, by enabling repeated start
  sl_i2c_driver_enable_repeated_start(i2c_handle, TRUE);
  i2c_status
    = sl_i2c_driver_send_data_blocking(i2c_handle,
                                       obj->config.addr,
                                       write_data_buf,
                                       len_write_data);
  if (i2c_status != SL_I2C_SUCCESS) {
    return I2C_MASTER_ERROR;
  }

  // Adding delay for synchronization before leader sends read request
  for (uint32_t x = 0; x < 2500; x++) {
    __NOP();
  }
  // Disabling repeated start before last cycle of transfer
  sl_i2c_driver_enable_repeated_start(i2c_handle, FALSE);
  i2c_status
    = sl_i2c_driver_receive_data_blocking(i2c_handle,
                                          obj->config.addr,
                                          read_data_buf,
                                          len_read_data);
  if (i2c_status != SL_I2C_SUCCESS) {
    return I2C_MASTER_ERROR;
  }
  sl_si91x_i2c_wait_till_i2c_is_idle(i2c_handle);
  return I2C_MASTER_SUCCESS;
}

void i2c_master_close(i2c_master_t *obj)
{
  obj->handle = NULL;
  _owner = NULL;
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

static err_t i2c_master_set_configuration(i2c_master_t *obj)
{
  sl_i2c_status_t i2c_status;
  sl_i2c_instance_t i2c_handle = *(sl_i2c_instance_t *)obj->handle;
  sl_i2c_config_t i2c_config = {
    .mode = SL_I2C_LEADER_MODE,
    .operating_mode = SL_I2C_STANDARD_MODE,
    .transfer_type = SL_I2C_USING_INTERRUPT,
    .i2c_callback = NULL,
  };

  last_i2c_speed_used = obj->config.speed;

  if (obj->config.speed >= I2C_MASTER_SPEED_FAST) {
    i2c_config.operating_mode = SL_I2C_FAST_PLUS_MODE;
  } else {
    i2c_config.operating_mode = obj->config.speed + 1;
  }

  /**
   * Re-Initializing I2C instance
   * (update i2c config-strucure name as per instance used)
   */
  i2c_status = sl_i2c_driver_init(i2c_handle,
                                  &i2c_config);
  if (i2c_status != SL_I2C_SUCCESS) {
    return I2C_MASTER_ERROR;
  }

  return I2C_MASTER_SUCCESS;
}

// ------------------------------------------------------------------------- END
