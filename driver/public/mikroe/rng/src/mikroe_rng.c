/***************************************************************************//**
 * @file mikroe_rng.c
 * @brief Mikroe RNG Source File
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
#include "mikroe_rng.h"
#include "mikroe_rng_config.h"
#include "rng.h"

static rng_t rng;
static rng_cfg_t rng_cfg;

sl_status_t mikroe_rng_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  rng.i2c.handle = i2cspm_instance;
  rng_cfg_setup(&rng_cfg);
  rng_cfg.i2c_address = RNG_DEVICE_SLAVE_ADDRESS;

#if (MIKROE_I2C_RNG_UC == 1)
  rng_cfg.i2c_speed = MIKROE_I2C_RNG_SPEED_MODE;
#endif

  if (RNG_OK != rng_init(&rng, &rng_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }
  return SL_STATUS_OK;
}

sl_status_t mikroe_rng_set_i2csmp_instance(mikroe_i2c_handle_t i2cspm_instance)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != i2cspm_instance) {
    rng.i2c.handle = i2cspm_instance;
    stt = SL_STATUS_OK;
  }
  return stt;
}

void mikroe_rng_default_cfg(void)
{
  rng_default_cfg(&rng);
}

void mikroe_rng_generic_write(uint8_t reg, uint16_t reg_data)
{
  rng_write_function(&rng, reg, reg_data);
}

sl_status_t mikroe_rng_generic_read(uint8_t reg, uint16_t *rx_buf)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != rx_buf)) {
    *rx_buf = rng_read_function(&rng, reg);
    stt = SL_STATUS_OK;
  }

  return stt;
}

void mikroe_rng_reset_device(void)
{
  rng_reset_device(&rng);
}

void mikroe_rng_set_config(uint16_t conf_data)
{
  rng_set_config(&rng, conf_data);
}

void mikroe_rng_set_vref(uint16_t vref_mv)
{
  rng_set_vref(&rng, vref_mv);
}

sl_status_t mikroe_rng_get_voltage(float *voltage)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != voltage)) {
    *voltage = rng_get_voltage(&rng);
    stt = SL_STATUS_OK;
  }

  return stt;
}

void mikroe_rng_set_threshold(uint8_t thresh_reg,
                              uint16_t thresh_data)
{
  rng_set_threshold(&rng, thresh_reg, thresh_data);
}
