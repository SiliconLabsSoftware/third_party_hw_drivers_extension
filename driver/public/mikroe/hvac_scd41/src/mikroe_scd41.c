/***************************************************************************//**
 * @file mikroe_scd41.c
 * @brief Mikroe SCD41 Source File
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
#include "mikroe_scd41.h"
#include "mikroe_scd41_config.h"
#include "hvac.h"

static hvac_t scd41;
static hvac_cfg_t scd41_cfg;

sl_status_t mikroe_scd41_init(mikroe_i2c_handle_t i2c_instance)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != i2c_instance) {
    scd41.i2c.handle = i2c_instance;
    hvac_cfg_setup(&scd41_cfg);

#if (MIKROE_SCD41_I2C_UC == 1)
    scd41_cfg.i2c_speed = MIKROE_SCD41_I2C_SPEED_MODE;
#endif

    if (I2C_MASTER_SUCCESS == hvac_init(&scd41, &scd41_cfg)) {
      stt = SL_STATUS_OK;
    } else {
      stt = SL_STATUS_FAIL;
    }
  }
  return stt;
}

sl_status_t mikroe_scd41_set_i2csmp_instance(mikroe_i2c_handle_t i2c_instance)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != i2c_instance) {
    scd41.i2c.handle = i2c_instance;
    stt = SL_STATUS_OK;
  }
  return stt;
}

sl_status_t mikroe_scd41_generic_write(uint8_t reg, uint8_t *tx_buf,
                                       uint8_t tx_len)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != tx_buf)) {
    err_t retval = hvac_generic_write(&scd41, reg, tx_buf, tx_len);
    stt = (retval == 0) ? SL_STATUS_OK : SL_STATUS_IO;
  }
  return stt;
}

sl_status_t mikroe_scd41_generic_read(uint8_t reg, uint8_t *rx_buf,
                                      uint8_t rx_len)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != rx_buf)) {
    err_t retval = hvac_generic_read(&scd41, reg, rx_buf, rx_len);
    stt = (retval == 0) ? SL_STATUS_OK : SL_STATUS_IO;
  }
  return stt;
}

sl_status_t mikroe_scd41_write_data(uint16_t reg, uint16_t tx_data)
{
  err_t retval = hvac_scd40_write_data(&scd41, reg, tx_data);
  sl_status_t stt = (retval == 0) ? SL_STATUS_OK : SL_STATUS_IO;
  return stt;
}

sl_status_t mikroe_scd41_read_data(uint16_t reg, uint16_t *rx_data)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != rx_data)) {
    err_t retval = hvac_scd40_read_data(&scd41, reg, rx_data);
    stt = (retval == 0) ? SL_STATUS_OK : SL_STATUS_IO;
  }
  return stt;
}

void mikroe_scd41_send_cmd(uint16_t cmd)
{
  hvac_scd40_send_cmd(&scd41, cmd);
}

sl_status_t mikroe_scd41_read_measurement(
  mikroe_scd41_measuremen_data_t *m_data)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;
  if (NULL != m_data) {
    hvac_scd40_read_measurement(&scd41, m_data);
    stt = SL_STATUS_OK;
  }

  return stt;
}

sl_status_t mikroe_scd41_get_serial_number(uint16_t *serial_number)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;
  if (NULL != serial_number) {
    hvac_scd40_get_serial_number(&scd41, serial_number);
    stt = SL_STATUS_OK;
  }
  return stt;
}

sl_status_t mikroe_scd41_get_feature_set_version(
  mikroe_scd41_feature_data_t *f_data)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;
  if (NULL != f_data) {
    hvac_scd40_get_feature_set_version(&scd41, f_data);
    stt = SL_STATUS_OK;
  }
  return stt;
}

void mikroe_scd41_set_temperature_offset(float temp_offset)
{
  hvac_scd40_set_temperature_offset(&scd41, temp_offset);
}

float mikroe_scd41_get_temperature_offset(void)
{
  return hvac_scd40_get_temperature_offset(&scd41);
}
