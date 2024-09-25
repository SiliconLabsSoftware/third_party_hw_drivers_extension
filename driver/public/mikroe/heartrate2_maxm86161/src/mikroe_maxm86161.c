/***************************************************************************//**
 * @file mikroe_maxm86161.c
 * @brief SCL MAXM86161 Source File
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

#include "heartrate2.h"
#include "mikroe_maxm86161.h"
#include "mikroe_maxm86161_config.h"

static heartrate2_t heartrate2;
static heartrate2_cfg_t heartrate2_cfg;

sl_status_t mikroe_maxm86161_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (i2cspm_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  heartrate2.i2c.handle = i2cspm_instance;

  // Call basic setup functions
  heartrate2_cfg_setup(&heartrate2_cfg);

#if defined(MAXM86161_EN_PORT) && defined(MAXM86161_EN_PIN)
  heartrate2_cfg.cs = hal_gpio_pin_name(MAXM86161_EN_PORT, MAXM86161_EN_PIN);
#endif

#if defined(MAXM86161_GP_PORT) && defined(MAXM86161_GP_PIN)
  heartrate2_cfg.an = hal_gpio_pin_name(MAXM86161_GP_PORT, MAXM86161_GP_PIN);
#endif

#if defined(MAXM86161_INT_PORT) && defined(MAXM86161_INT_PIN)
  heartrate2_cfg.int_pin = hal_gpio_pin_name(MAXM86161_INT_PORT,
                                             MAXM86161_INT_PIN);
#endif

  if (heartrate2_init(&heartrate2, &heartrate2_cfg) != HEARTRATE2_OK_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_maxm86161_set_i2csmp_instance(
  mikroe_i2c_handle_t i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  heartrate2.i2c.handle = i2cspm_instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_maxm86161_default_cfg(mikroe_maxm86161_config_led_t cfg_led)
{
  if (((MAXM86161_CONFIG_GREEN != cfg_led)
       && (MAXM86161_CONFIG_IR != cfg_led)
       && (MAXM86161_CONFIG_RED != cfg_led))) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  switch (cfg_led)
  {
    case MAXM86161_CONFIG_GREEN:
    {
      heartrate2_default_cfg(&heartrate2, HEARTRATE2_CONFIG_GREEN);
      break;
    }

    case MAXM86161_CONFIG_IR:
    {
      heartrate2_default_cfg(&heartrate2, HEARTRATE2_CONFIG_IR);
      break;
    }

    default:
    {
      heartrate2_default_cfg(&heartrate2, HEARTRATE2_CONFIG_RED);
    }
  }
  return SL_STATUS_OK;
}

void mikroe_maxm86161_generic_write(uint8_t register_address,
                                    uint8_t write_data)
{
  heartrate2_generic_write(&heartrate2, register_address, write_data);
}

uint8_t mikroe_maxm86161_generic_read(uint8_t register_address)
{
  return heartrate2_generic_read(&heartrate2, register_address);
}

sl_status_t mikroe_maxm86161_read_buf(uint8_t reg,
                                      uint8_t *data_buf,
                                      uint8_t len)
{
  if (NULL == data_buf) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  heartrate2_read_buf(&heartrate2, reg, data_buf, len);

  return SL_STATUS_OK;
}

sl_status_t mikroe_maxm86161_read_fifo(
  mikroe_maxm86161_fifo_data_t *mikroe_fifo)
{
  if (NULL == mikroe_fifo) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  heartrate2_read_fifo(&heartrate2, (heartrate2_fifo_data_t *)mikroe_fifo);
  return SL_STATUS_OK;
}

float mikroe_maxm86161_read_temp(void)
{
  return heartrate2_read_temp(&heartrate2);
}

void mikroe_maxm86161_soft_reset(void)
{
  heartrate2_soft_reset(&heartrate2);
}

sl_status_t mikroe_maxm86161_ppg_config(
  mikroe_maxm86161_ppg_cfg_t *mikroe_ppg_cfg)
{
  if (NULL == mikroe_ppg_cfg) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (HEARTRATE2_ERROR
      == heartrate2_ppg_config(&heartrate2,
                               (heartrate2_ppg_cfg_t *)mikroe_ppg_cfg)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_maxm86161_ledpa_config(
  mikroe_maxm86161_ledpa_t *mikroe_ledpa)
{
  if (NULL == mikroe_ledpa) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  heartrate2_ledpa_config(&heartrate2, (heartrate2_ledpa_t *)mikroe_ledpa);
  return SL_STATUS_OK;
}

sl_status_t mikroe_maxm86161_led_range_curr_config(
  mikroe_maxm86161_led_range_curr_t *mikroe_led_range)
{
  if (NULL == mikroe_led_range) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (HEARTRATE2_ERROR == heartrate2_led_range_curr_config(
        &heartrate2,
        (heartrate2_led_range_curr_t *)mikroe_led_range)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_maxm86161_led_sequence_config(
  mikroe_maxm86161_ledsq_cfg_t *mikroe_ledsq)
{
  if (NULL == mikroe_ledsq) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (HEARTRATE2_ERROR == heartrate2_led_sequence_config(&heartrate2,
                                                         (heartrate2_ledsq_cfg_t
                                                          *)mikroe_ledsq)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_maxm86161_int_control(
  mikroe_maxm86161_int_t *mikroe_int_ctrl)
{
  if (NULL == mikroe_int_ctrl) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (HEARTRATE2_ERROR == heartrate2_int_control(&heartrate2,
                                                 (heartrate2_int_t *)
                                                 mikroe_int_ctrl)) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

sl_status_t mikroe_maxm86161_int_status(
  mikroe_maxm86161_int_t *mikroe_int_status)
{
  if (NULL == mikroe_int_status) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  heartrate2_int_status(&heartrate2, (heartrate2_int_t *)mikroe_int_status);
  return SL_STATUS_OK;
}

uint8_t mikroe_maxm86161_get_int(void)
{
  return heartrate2_get_int(&heartrate2);
}

uint8_t mikroe_maxm86161_get_gp(void)
{
  return heartrate2_get_gp(&heartrate2);
}

sl_status_t mikroe_maxm86161_set_en(mikroe_state_pin_t state)
{
  if (((MAXM86161_PIN_HIGH != state) && (MAXM86161_PIN_LOW != state))) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  heartrate2_set_en(&heartrate2, MAXM86161_PIN_HIGH == state
                    ?HEARTRATE2_PIN_HIGH : HEARTRATE2_PIN_LOW);
  return SL_STATUS_OK;
}
