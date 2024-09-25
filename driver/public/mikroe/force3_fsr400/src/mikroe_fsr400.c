/***************************************************************************//**
 * @file mikroe_fsr400.c
 * @brief Mikroe FSR400 Source File
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
#include "mikroe_fsr400.h"
#include "mikroe_fsr400_config.h"
#include "force3.h"

static force3_t force3;
static force3_cfg_t force3_cfg;

sl_status_t mikroe_fsr400_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  force3.i2c.handle = i2cspm_instance;
  force3_cfg_setup(&force3_cfg);

#if (MIKROE_I2C_FSR400_UC == 1)
  force3_cfg.i2c_speed = MIKROE_I2C_FSR400_SPEED_MODE;
#endif

  if (FORCE3_OK != force3_init(&force3, &force3_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_fsr400_set_i2csmp_instance(
  mikroe_i2c_handle_t i2cspm_instance)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != i2cspm_instance) {
    force3.i2c.handle = i2cspm_instance;
    stt = SL_STATUS_OK;
  }
  return stt;
}

sl_status_t mikroe_fsr400_generic_write(uint8_t reg, uint8_t *tx_buf,
                                        uint8_t tx_len)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != tx_buf)) {
    force3_generic_write(&force3, reg, tx_buf, tx_len);
    stt = SL_STATUS_OK;
  }
  return stt;
}

sl_status_t mikroe_fsr400_generic_read(uint8_t reg, uint8_t *rx_buf,
                                       uint8_t rx_len)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != rx_buf)) {
    force3_generic_read(&force3, reg, rx_buf, rx_len);
    stt = SL_STATUS_OK;
  }
  return stt;
}

sl_status_t mikroe_fsr400_get_raw_data(uint16_t *data)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != data)) {
    *data = force3_read_raw_data(&force3);
    stt = SL_STATUS_OK;
  }
  return stt;
}
