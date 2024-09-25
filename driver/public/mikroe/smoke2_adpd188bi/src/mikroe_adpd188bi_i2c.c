/***************************************************************************//**
 * @file mikroe_adpd188bi.c
 * @brief Mikroe ADPD188BI Source File
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
#include "mikroe_adpd188bi_i2c.h"
#include "mikroe_smoke2_adpd188bi_i2c_config.h"

static smoke2_t smoke2;
static smoke2_cfg_t smoke2_cfg;

sl_status_t mikroe_adpd188bi_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  smoke2.i2c.handle = i2cspm_instance;
  smoke2_cfg_setup(&smoke2_cfg);

#if defined(MIKROE_SMOKE2_INT_PORT) && defined(MIKROE_SMOKE2_INT_PIN)
  smoke2_cfg.int_pin = hal_gpio_pin_name(MIKROE_SMOKE2_INT_PORT,
                                         MIKROE_SMOKE2_INT_PIN);
#endif

#if (MIKROE_SMOKE2_I2C_UC == 1)
  smoke2_cfg.i2c_speed = MIKROE_SMOKE2_I2C_SPEED_MODE;
#endif

  smoke2_drv_interface_selection(&smoke2_cfg, SMOKE2_DRV_SEL_I2C);

  if (SMOKE2_OK != smoke2_init(&smoke2, &smoke2_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }
  return SL_STATUS_OK;
}

sl_status_t mikroe_adpd188bi_set_i2csmp_instance(
  mikroe_i2c_handle_t i2cspm_instance)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != i2cspm_instance) {
    smoke2.i2c.handle = i2cspm_instance;
    stt = SL_STATUS_OK;
  }
  return stt;
}

void mikroe_adpd188bi_default_cfg(void)
{
  smoke2_default_cfg(&smoke2);
}

sl_status_t mikroe_adpd188bi_generic_write(uint8_t reg,
                                           uint8_t *data_in,
                                           uint8_t len)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;
  if (NULL != data_in) {
    err_t st = smoke2_generic_write(&smoke2, reg, data_in, len);

    if (st == 0) {
      stt = SL_STATUS_OK;
    } else {
      stt = SL_STATUS_FAIL;
    }
  }
  return stt;
}

sl_status_t mikroe_adpd188bi_generic_read(uint8_t reg,
                                          uint8_t *data_out,
                                          uint8_t len)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;
  if (NULL != data_out) {
    err_t st = smoke2_generic_read(&smoke2, reg, data_out, len);

    if (st == 0) {
      stt = SL_STATUS_OK;
    } else {
      stt = SL_STATUS_FAIL;
    }
  }
  return stt;
}

uint8_t mikroe_adpd188bi_get_int_pin(void)
{
  return smoke2_get_int_pin(&smoke2);
}

void mikroe_adpd188bi_write_data(uint8_t reg, uint16_t tx_data)
{
  smoke2_write_data(&smoke2, reg, tx_data);
}

uint16_t mikroe_adpd188bi_read_data(uint8_t reg)
{
  return smoke2_read_data(&smoke2, reg);
}

void mikroe_adpd188bi_set_bit(uint8_t reg, uint8_t bit_num, uint8_t val)
{
  smoke2_set_bit(&smoke2, reg, bit_num, val);
}

uint8_t mikroe_adpd188bi_get_bit(uint8_t reg, uint8_t bit_num)
{
  return smoke2_get_bit(&smoke2, reg, bit_num);
}

void mikroe_adpd188bi_set_mode(uint8_t mode)
{
  smoke2_set_mode(&smoke2, mode);
}

sl_status_t mikroe_adpd188bi_get_int(uint8_t *fifo,
                                     uint8_t *slot_a,
                                     uint8_t *slot_b)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != fifo) && (NULL != slot_a) && (NULL != slot_b)) {
    smoke2_get_int(&smoke2, fifo, slot_a, slot_b);
    stt = SL_STATUS_OK;
  }

  return stt;
}

uint16_t mikroe_adpd188bi_read_sens_data(uint8_t slot, uint8_t chn)
{
  return smoke2_read_sens_data(&smoke2, slot, chn);
}

void mikroe_adpd188bi_soft_reset(void)
{
  smoke2_soft_reset(&smoke2);
}

uint16_t mikroe_adpd188bi_smoke_calibration(uint16_t threshold)
{
  return smoke2_smoke_calibration(&smoke2, threshold);
}

uint8_t mikroe_adpd188bi_check_smoke(void)
{
  return smoke2_check_smoke(&smoke2);
}
