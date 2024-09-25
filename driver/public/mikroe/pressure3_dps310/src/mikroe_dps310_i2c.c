/***************************************************************************//**
 * @file mikroe_dps310_i2c.c
 * @brief SCL DPS310 source file
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "drv_digital_out.h"
#include "drv_digital_in.h"
#include "mikroe_dps310_i2c.h"
#include "mikroe_dps310_i2c_config.h"
#include "pressure3.h"

/********************************************************************************
 * static variables
 *******************************************************************************/

static pressure3_coeff_t coeff_struct;
static pressure3_t ctx;

/********************************************************************************
 * public function
 *******************************************************************************/
sl_status_t mikroe_pressure3_init(mikroe_i2c_handle_t i2c_instance)
{
  pressure3_cfg_t cfg;

  if (i2c_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Call basic setup functions
  pressure3_cfg_setup(&cfg);
  // Configure default i2csmp instance
  ctx.i2c.handle = i2c_instance;

#if (MIKROE_DPS310_I2C_UC == 1)
  cfg.i2c_speed = MIKROE_DPS310_I2C_SPEED_MODE;
#endif

  if (PRESSURE3_OK != pressure3_init(&ctx, &cfg)) {
    return SL_STATUS_INITIALIZATION;
  }

  pressure3_default_cfg(&ctx);
  return SL_STATUS_OK;
}

sl_status_t mikroe_pressure3_generic_write(uint8_t reg,
                                           uint8_t *data_buf,
                                           uint8_t len)
{
  if ((data_buf == NULL) || (len == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pressure3_generic_write(&ctx, reg, data_buf, len);
  return SL_STATUS_OK;
}

sl_status_t mikroe_pressure3_generic_read(uint8_t reg,
                                          uint8_t *data_buf,
                                          uint8_t len)
{
  if ((data_buf == NULL) || (len == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pressure3_generic_read(&ctx, reg, data_buf, len);
  return SL_STATUS_OK;
}

sl_status_t mikroe_pressure3_get_t_p_data(float *temperature,
                                          float *pressure)
{
  if ((temperature == NULL) || (pressure == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pressure3_get_t_p_data(&ctx,
                         temperature,
                         pressure,
                         &coeff_struct);
  return SL_STATUS_OK;
}

sl_status_t mikroe_pressure3_get_coefficients(
  mikroe_pressure3_coeff_t *coeff_struct)
{
  if (coeff_struct == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pressure3_get_coefficients(&ctx, (pressure3_coeff_t *)coeff_struct);
  return SL_STATUS_OK;
}

int32_t mikroe_pressure3_get_measurement_data(uint8_t mbs_reg_addr)
{
  return pressure3_get_measurement_data(&ctx, mbs_reg_addr);
}

sl_status_t mikroe_pressure3_set_pressure_configuration(uint8_t press_config)
{
  pressure3_set_pressure_configuration(&ctx, press_config);
  return SL_STATUS_OK;
}

uint8_t mikroe_pressure3_get_pressure_configuration(void)
{
  return pressure3_get_pressure_configuration(&ctx);
}

sl_status_t mikroe_pressure3_set_temperature_configuration(uint8_t temp_config)
{
  pressure3_set_temperature_configuration(&ctx, temp_config);
  return SL_STATUS_OK;
}

uint8_t mikroe_pressure3_get_temperature_configuration(void)
{
  return pressure3_get_temperature_configuration(&ctx);
}

sl_status_t mikroe_pressure3_set_operating_mode(uint8_t operation_mode)
{
  pressure3_set_operating_mode(&ctx, operation_mode);
  return SL_STATUS_OK;
}

uint8_t mikroe_pressure3_get_operating_mode(void)
{
  return pressure3_get_operating_mode(&ctx);
}

sl_status_t mikroe_pressure3_set_configuration(uint8_t confg_data)
{
  pressure3_set_configuration(&ctx, confg_data);
  return SL_STATUS_OK;
}

uint8_t mikroe_pressure3_get_configuration(void)
{
  return pressure3_get_configuration(&ctx);
}

uint8_t mikroe_pressure3_get_interrupt_status(void)
{
  return pressure3_get_interrupt_status(&ctx);
}

uint8_t mikroe_pressure3_get_fifo_status(void)
{
  return pressure3_get_fifo_status(&ctx);
}

sl_status_t mikroe_pressure3_soft_reset(void)
{
  pressure3_soft_reset(&ctx);
  return SL_STATUS_OK;
}

// ------------------------------------------------------------------------- END
