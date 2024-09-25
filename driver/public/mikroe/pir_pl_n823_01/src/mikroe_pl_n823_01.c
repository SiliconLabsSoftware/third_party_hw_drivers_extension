/***************************************************************************//**
 * @file mikroe_pl_n823_01.c
 * @brief SCL Mikroe PIR pl_n823_01 Source file.
 * @version 1.2.0
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

#include "mikroe_pl_n823_01.h"
#include "mikroe_pir_pl_n823_01_config.h"
#include "pir.h"

static pir_t pir_ctx;
static pir_cfg_t pir_cfg;

sl_status_t mikroe_pl_n823_01_init(mikroe_i2c_handle_t instance)
{
  if (instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pir_cfg_setup(&pir_cfg);
  pir_ctx.i2c.handle = instance;

#if defined(PL_N823_01_ANALOG_OUTPUT_PORT) \
  && defined(PL_N823_01_ANALOG_OUTPUT_PIN)
  pir_cfg.an = hal_gpio_pin_name(PL_N823_01_ANALOG_OUTPUT_PORT,
                                 PL_N823_01_ANALOG_OUTPUT_PIN);
#endif

#if (MIKROE_PL_N823_01_I2C_UC == 1)
  pir_cfg.i2c_speed = MIKROE_PL_N823_01_I2C_SPEED_MODE;
#endif

  if (PIR_OK != pir_init(&pir_ctx, &pir_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_pl_n823_01_set_i2c_instance(mikroe_i2c_handle_t instance)
{
  if (NULL == instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pir_ctx.i2c.handle = instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_pl_n823_01_generic_write(uint8_t reg, uint8_t *data_buf,
                                            uint8_t len)
{
  if (data_buf == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pir_generic_write(&pir_ctx, reg, data_buf, len);

  return SL_STATUS_OK;
}

sl_status_t mikroe_pl_n823_01_generic_read(uint8_t reg, uint8_t *data_buf,
                                           uint8_t len)
{
  if (data_buf == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pir_generic_read(&pir_ctx, reg, data_buf, len);

  return SL_STATUS_OK;
}

uint16_t mikroe_pl_n823_01_get_adc(void)
{
  return pir_get_adc(&pir_ctx);
}

uint16_t mikroe_pl_n823_01_reg_read(void)
{
  return pir_reg_read(&pir_ctx);
}

float mikroe_pl_n823_01_get_mili_volt(uint16_t ref_voltage)
{
  if (ref_voltage <= 0) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return pir_get_mili_volt(&pir_ctx, ref_voltage);
}

float mikroe_pl_n823_01_scale_results(uint16_t in_val, uint16_t out_min,
                                      uint16_t out_max)
{
  return pir_scale_results(&pir_ctx, in_val, out_min, out_max);
}
