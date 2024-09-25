/***************************************************************************//**
 * @file mikroe_xm125.c
 * @brief mikroe_xm125.c Source File
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

#include "pcr.h"
#include "mikroe_xm125.h"
#include "mikroe_xm125_config.h"

static pcr_t pcr;
static pcr_cfg_t pcr_cfg;

sl_status_t mikroe_xm125_init(mikroe_i2c_handle_t instance)
{
  if (NULL == instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pcr.i2c.handle = instance;

  pcr_cfg_setup(&pcr_cfg);
  pcr_cfg.rst = hal_gpio_pin_name(CONFIG_XM125_RST_PORT,
                                  CONFIG_XM125_RST_PIN);
  pcr_cfg.wup = hal_gpio_pin_name(CONFIG_XM125_WUP_PORT,
                                  CONFIG_XM125_WUP_PIN);
  pcr_cfg.int_pin = hal_gpio_pin_name(CONFIG_XM125_INT_PORT,
                                      CONFIG_XM125_INT_PIN);

#if (MIKROE_XM125_I2C_UC == 1)
  pcr_cfg.i2c_speed = MIKROE_XM125_I2C_SPEED_MODE;
#endif

  if (PCR_OK != pcr_init(&pcr, &pcr_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_xm125_set_i2csmp_instance(mikroe_i2c_handle_t instance)
{
  if (NULL == instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  pcr.i2c.handle = instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_xm125_default_cfg(void)
{
  return (pcr_default_cfg(&pcr) == PCR_OK) ?  SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_xm125_generic_write(uint8_t reg, uint8_t *data_in,
                                       uint8_t len)
{
  if (NULL == data_in) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (PCR_OK != pcr_generic_write(&pcr, reg, data_in, len)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_xm125_generic_read(uint8_t reg, uint8_t *data_out,
                                      uint8_t len)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (PCR_OK != pcr_generic_read(&pcr, reg, data_out, len)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

void mikroe_xm125_set_wup_pin(bool wup_pin_state)
{
  pcr_set_wup_pin(&pcr, (uint8_t)wup_pin_state);
}

void mikroe_xm125_set_rst_pin(bool rst_pin_state)
{
  pcr_set_rst_pin(&pcr, (uint8_t)rst_pin_state);
}

uint8_t mikroe_xm125_get_int_pin(void)
{
  return pcr_get_int_pin(&pcr);
}

void mikroe_xm125_hw_reset(void)
{
  pcr_hw_reset(&pcr);
}

sl_status_t mikroe_xm125_write_reg(uint16_t reg, uint32_t data_in)
{
  return (pcr_write_reg(&pcr, reg,
                        data_in) == PCR_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_xm125_read_reg(uint16_t reg, uint32_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (PCR_OK != pcr_read_reg(&pcr, reg, data_out)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_xm125_get_status(uint32_t *status)
{
  if (NULL == status) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (PCR_OK != pcr_get_status(&pcr, status)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_xm125_check_device_busy(void)
{
  return (pcr_check_if_busy(&pcr) == PCR_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_xm125_check_status_ok(void)
{
  return (pcr_check_if_ok(&pcr) == PCR_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_xm125_get_distance(uint32_t *distance_data)
{
  if (NULL == distance_data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (PCR_OK != pcr_get_distance(&pcr, distance_data)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}
