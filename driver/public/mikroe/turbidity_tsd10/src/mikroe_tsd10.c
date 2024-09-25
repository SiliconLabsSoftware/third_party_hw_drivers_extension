/***************************************************************************//**
 * @file mikroe_tsd10.c
 * @brief TSD10 Source file
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

#include "turbidity.h"
#include "mikroe_tsd10.h"
#include "mikroe_tsd10_config.h"

static turbidity_t turbi;
static turbidity_cfg_t turbi_cfg;

sl_status_t mikroe_turbidity_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  turbi.i2c.handle = i2cspm_instance;
  turbidity_cfg_setup(&turbi_cfg);

#if (MIKROE_I2C_TSD10_UC == 1)
  turbi_cfg.i2c_speed = MIKROE_I2C_TSD10_SPEED_MODE;
#endif

  if (I2C_MASTER_SUCCESS != turbidity_init(&turbi, &turbi_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_turbidity_generic_read(uint8_t *rx_buf, uint8_t rx_len)
{
  if ((rx_buf == NULL) || (rx_len <= 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (turbidity_generic_read(&turbi, rx_buf,
                                 rx_len) == 0) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_turbidity_read_adc(uint16_t *adc_val)
{
  if (adc_val == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (turbidity_read_adc(&turbi,
                             adc_val) == 0) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_turbidity_get_adc_voltage(float *voltage)
{
  if (voltage == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (turbidity_get_adc_voltage(&turbi,
                                    voltage)
          == 0) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_turbidity_get_ntu(float *ntu)
{
  if (ntu == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (turbidity_get_ntu(&turbi, ntu) == 0) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

// ------------------------------------------------------------------------- END
