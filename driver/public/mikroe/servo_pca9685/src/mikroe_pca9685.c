/***************************************************************************//**
 * @file mikroe_pca9685.c
 * @brief SCL Mikroe Servo Click Source File
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

#include <stddef.h>
#include "mikroe_pca9685.h"

static servo_t servo_ctx;
static servo_cfg_t servo_cfg;

sl_status_t mikroe_pca9685_set_i2c_instance(mikroe_i2c_handle_t i2c_instance)
{
  if (NULL == i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  servo_ctx.i2c.handle = i2c_instance;

  return SL_STATUS_OK;
}

void mikroe_pca9685_default_cfg(void)
{
  servo_default_cfg(&servo_ctx);
}

sl_status_t mikroe_pca9685_init(mikroe_i2c_handle_t i2c_instance)
{
  if (NULL == i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  servo_cfg_setup(&servo_cfg);
  servo_cfg.i2c_address_of_ltc2497 = LTC2497_ADDRESS;
  servo_cfg.i2c_address_of_pca9685 = PCA9685_ADDRESS;

  servo_ctx.i2c.handle = i2c_instance;

#if defined(SERVO_OUTPUT_ENABLE_PORT) && defined(SERVO_OUTPUT_ENABLE_PIN)
  servo_cfg.oe = hal_gpio_pin_name(SERVO_OUTPUT_ENABLE_PORT,
                                   SERVO_OUTPUT_ENABLE_PIN);
#endif

#if (MIKROE_SERVO_CLICK_I2C_UC == 1)
  servo_cfg.i2c_speed = MIKROE_SERVO_CLICK_I2C_SPEED_MODE;
#endif

  if (SERVO_OK != servo_init(&servo_ctx, &servo_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }

  servo_default_cfg(&servo_ctx);
  return SL_STATUS_OK;
}

sl_status_t mikroe_pca9685_generic_write_of_pca9685(uint8_t reg,
                                                    uint8_t *data_buf,
                                                    uint8_t len)
{
  if (NULL == data_buf) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  servo_generic_write_of_pca9685(&servo_ctx, reg, data_buf, len);

  return SL_STATUS_OK;
}

sl_status_t mikroe_pca9685_generic_write_of_ltc2497(uint8_t reg,
                                                    uint8_t *data_buf,
                                                    uint8_t len)
{
  if (NULL == data_buf) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  servo_generic_write_of_ltc2497(&servo_ctx, reg, data_buf, len);

  return SL_STATUS_OK;
}

sl_status_t mikroe_pca9685_generic_read_of_pca9685(uint8_t reg,
                                                   uint8_t *data_buf,
                                                   uint8_t len)
{
  if (NULL == data_buf) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  servo_generic_read_of_pca9685(&servo_ctx, reg, data_buf, len);

  return SL_STATUS_OK;
}

sl_status_t mikroe_pca9685_generic_read_of_ltc2497(uint8_t reg,
                                                   uint8_t *data_buf,
                                                   uint8_t len)
{
  if (NULL == data_buf) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  servo_generic_read_of_ltc2497(&servo_ctx, reg, data_buf, len);

  return SL_STATUS_OK;
}

void mikroe_pca9685_setting(mikroe_servo_pos_and_res_t *pos_and_res)
{
  servo_pos_and_res_t servo_pos_and_res;

  servo_pos_and_res.high_resolution = pos_and_res->high_resolution;
  servo_pos_and_res.low_resolution = pos_and_res->low_resolution;
  servo_pos_and_res.max_position = pos_and_res->max_position;
  servo_pos_and_res.min_position = pos_and_res->min_position;
  servo_setting(&servo_ctx, servo_pos_and_res);
}

sl_status_t mikroe_pca9685_set_vref(uint16_t v_ref)
{
  if ((v_ref != MIKROE_SERVO_VREF_3300) || (v_ref != MIKROE_SERVO_VREF_5000)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  servo_set_vref(&servo_ctx, v_ref);

  return SL_STATUS_OK;
}

void mikroe_pca9685_stop(void)
{
  servo_stop(&servo_ctx);
}

void mikroe_pca9685_start(void)
{
  servo_start(&servo_ctx);
}

void mikroe_pca9685_soft_reset(void)
{
  servo_soft_reset(&servo_ctx);
}

void mikroe_pca9685_sleep(void)
{
  servo_sleep(&servo_ctx);
}

sl_status_t mikroe_pca9685_set_mode(uint8_t mode, uint8_t w_data)
{
  if ((mode != MIKROE_SERVO_REG_MODE_1) || (mode != MIKROE_SERVO_REG_MODE_2)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  servo_set_mode(&servo_ctx, mode, w_data);

  return SL_STATUS_OK;
}

sl_status_t mikroe_pca9685_set_position(uint8_t motor, uint8_t position)
{
  if ((motor < MIKROE_SERVO_MOTOR_1) || (motor > MIKROE_SERVO_MOTOR_16)
      || (position < servo_ctx.min_pos) || (position > servo_ctx.max_pos)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  servo_set_position(&servo_ctx, motor, position);

  return SL_STATUS_OK;
}

void mikroe_pca9685_set_freq(uint16_t freq)
{
  servo_set_freq(&servo_ctx, freq);
}

sl_status_t mikroe_pca9685_get_channel(uint8_t channel, uint32_t *adc_value)
{
  if ((NULL == adc_value) || (channel < MIKROE_SERVO_POSITIVE_CH0_NEGATIVE_CH1)
      || (channel > MIKROE_SERVO_POSITIVE_CH15)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *adc_value = servo_get_channel(&servo_ctx, channel);

  return SL_STATUS_OK;
}

sl_status_t mikroe_pca9685_get_current(uint8_t channel, uint16_t *current_ma)
{
  if ((NULL == current_ma) || (channel < MIKROE_SERVO_POSITIVE_CH0_NEGATIVE_CH1)
      || (channel > MIKROE_SERVO_POSITIVE_CH15)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *current_ma = servo_get_current(&servo_ctx, channel);

  return SL_STATUS_OK;
}
