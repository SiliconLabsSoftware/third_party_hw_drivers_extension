/***************************************************************************//**
 * @file mikroe_shtc3.c
 * @brief Mikroe SHTC3 Source File
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

#include "mikroe_shtc3_config.h"
#include "mikroe_shtc3.h"
#include "temphum9.h"

static temphum9_t temphum9;
static temphum9_cfg_t temphum9_cfg;

sl_status_t mikroe_shtc3_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  temphum9.i2c.handle = i2cspm_instance;

  // Call basic setup functions
  temphum9_cfg_setup(&temphum9_cfg);

#if (MIKROE_SHTC3_I2C_UC == 1)
  temphum9_cfg.i2c_speed = MIKROE_SHTC3_I2C_SPEED_MODE;
#endif

  if (temphum9_init(&temphum9, &temphum9_cfg) != TEMPHUM9_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_shtc3_set_i2csmp_instance(
  mikroe_i2c_handle_t i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  temphum9.i2c.handle = i2cspm_instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_shtc3_read_register(uint16_t register_address,
                                       mikroe_shtc3_register_access_mode_t n_data,
                                       uint16_t *register_buffer)
{
  if (((SHTC3_REGISTER_ACCESS_SINGLE_DATA != n_data)
       && (SHTC3_REGISTER_ACCESS_DUAL_DATA != n_data))
      || (NULL == register_buffer)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  temphum9_read_register(&temphum9,
                         register_address,
                         SHTC3_REGISTER_ACCESS_SINGLE_DATA == n_data
                         ?TEMPHUM9_SINGLE_DATA
                         :TEMPHUM9_DUAL_DATA,
                         register_buffer);

  return SL_STATUS_OK;
}

void mikroe_shtc3_send_command(uint16_t command)
{
  temphum9_send_command(&temphum9, command);
}

sl_status_t mikroe_shtc3_get_temperature(mikroe_shtc3_data_mode_t data_mode,
                                         float *temperature)
{
  if (((SHTC3_DATA_MODE_LOW_POWER != data_mode)
       && (SHTC3_DATA_MODE_NORMAL != data_mode)) || (NULL == temperature)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *temperature = temphum9_get_temperature(&temphum9,
                                          SHTC3_DATA_MODE_NORMAL == data_mode
                                          ?TEMPHUM9_NORMAL_MODE
                                          :TEMPHUM9_LOW_POWER_MODE);

  return SL_STATUS_OK;
}

sl_status_t mikroe_shtc3_get_humidity(mikroe_shtc3_data_mode_t data_mode,
                                      float *humidity)
{
  if (((SHTC3_DATA_MODE_LOW_POWER != data_mode)
       && (SHTC3_DATA_MODE_NORMAL != data_mode)) || (NULL == humidity)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *humidity = temphum9_get_relative_humidity(&temphum9,
                                             SHTC3_DATA_MODE_NORMAL == data_mode
                                             ?TEMPHUM9_NORMAL_MODE
                                             :TEMPHUM9_LOW_POWER_MODE);

  return SL_STATUS_OK;
}

sl_status_t mikroe_shtc3_get_temperature_and_humidity(
  mikroe_shtc3_data_mode_t data_mode,
  mikroe_shtc3_measurement_data_t *measurement_data)
{
  float _measurement_data[2];

  if (((SHTC3_DATA_MODE_LOW_POWER != data_mode)
       && (SHTC3_DATA_MODE_NORMAL != data_mode))
      || (NULL == measurement_data)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  temhum9_get_temperature_and_humidity(&temphum9,
                                       SHTC3_DATA_MODE_NORMAL == data_mode
                                       ?TEMPHUM9_NORMAL_MODE
                                       :TEMPHUM9_LOW_POWER_MODE,
                                       _measurement_data);
  measurement_data->temperature = _measurement_data[0];
  measurement_data->humidity = _measurement_data[1];

  return SL_STATUS_OK;
}
