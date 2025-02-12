/***************************************************************************//**
 * @file mikroe_tb9053ftg.c
 * @brief SCL TB9053FTG Prototypes
 * @version 1.0.0
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

#include "mikroe_tb9053ftg.h"
#include "mikroe_dcmotor26_config.h"

static dcmotor26_t ctx;
static dcmotor26_cfg_t ctx_cfg;

sl_status_t mikroe_tb9053ftg_init(mikroe_spi_handle_t spi_instance,
                                  mikroe_i2c_handle_t i2c_instance,
                                  mikroe_adc_handle_t adc)
{
  if ((NULL == spi_instance)
      || (NULL == i2c_instance) || (NULL == adc)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  dcmotor26_cfg_setup(&ctx_cfg);

  ctx.i2c.handle = i2c_instance;
  ctx.spi.handle = spi_instance;
  ctx.slave_address = DCMOTOR26_PCA9538A_ADDRESS;
  ctx.adc.handle = adc;

  ctx_cfg.spi_speed = 1000000;

#if defined(DCMOTOR26_CHANNEL_MONITOR_PORT) \
  && defined(DCMOTOR26_CHANNEL_MONITOR_PIN)
  ctx_cfg.cm = hal_gpio_pin_name(DCMOTOR26_CHANNEL_MONITOR_PORT,
                                 DCMOTOR26_CHANNEL_MONITOR_PIN);
#endif

#if defined(DCMOTOR26_SLEEP_PORT) && defined(DCMOTOR26_SLEEP_PIN)
  ctx_cfg.slp = hal_gpio_pin_name(DCMOTOR26_SLEEP_PORT,
                                  DCMOTOR26_SLEEP_PIN);
#endif

#if defined(DCMOTOR26_INT_PORT) && defined(DCMOTOR26_INT_PIN)
  ctx_cfg.int_pin = hal_gpio_pin_name(DCMOTOR26_INT_PORT,
                                      DCMOTOR26_INT_PIN);
#endif

#if defined(DCMOTOR26_PWM_PORT) && defined(DCMOTOR26_PWM_PIN)
  ctx_cfg.clk = hal_gpio_pin_name(DCMOTOR26_PWM_PORT,
                                  DCMOTOR26_PWM_PIN);
#endif

#if (MIKROE_DCMOTOR26_SPI_UC == 1)
  ctx_cfg.spi_speed = MIKROE_DCMOTOR26_SPI_BITRATE;
#endif

#if defined(DCMOTOR26_CS_PORT) && defined(DCMOTOR26_CS_PIN)
  ctx_cfg.cs = hal_gpio_pin_name(DCMOTOR26_CS_PORT,
                                 DCMOTOR26_CS_PIN);
  // CS pin need to init here since the mikroe_sdk_v2 missed this step
  digital_out_t struct_cs;
  digital_out_init(&struct_cs, ctx_cfg.cs);
#endif

#if (MIKROE_DCMOTOR26_I2C_UC == 1)
  ctx_cfg.i2c_speed = MIKROE_DCMOTOR26_I2C_SPEED_MODE;
#endif

  if (dcmotor26_init(&ctx, &ctx_cfg) != DCMOTOR26_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_tb9053ftg_default_cfg(void)
{
  return (dcmotor26_default_cfg(&ctx)
          == DCMOTOR26_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_write_register(uint8_t reg, uint32_t data_in)
{
  return (dcmotor26_write_register(&ctx, reg, data_in) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_read_register(uint8_t reg, uint32_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_read_register(&ctx, reg, data_out) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_pca9538a_write_register(uint8_t reg, uint8_t data_in)
{
  return (dcmotor26_pca9538a_write_register(&ctx, reg, data_in) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_pca9538a_read_register(uint8_t reg, uint8_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_pca9538a_read_register(&ctx, reg, data_out) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

void mikroe_tb9053ftg_enter_sleep_mode(void)
{
  dcmotor26_enter_sleep_mode(&ctx);
}

void mikroe_tb9053ftg_wake_up_device(void)
{
  dcmotor26_wake_up_device(&ctx);
}

uint8_t mikroe_tb9053ftg_get_int_pin(void)
{
  return dcmotor26_get_int_pin(&ctx);
}

void mikroe_tb9053ftg_set_clk_pin(uint8_t state)
{
  dcmotor26_set_clk_pin(&ctx, state);
}

sl_status_t mikroe_tb9053ftg_set_en1_pin(uint8_t state)
{
  return (dcmotor26_set_en1_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_en1_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_en1_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_en2_pin(uint8_t state)
{
  return (dcmotor26_set_en2_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_en2_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_en2_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_cm_sel_pin(uint8_t state)
{
  return (dcmotor26_set_cm_sel_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_cm_sel_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_cm_sel_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_diag1_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_diag1_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_diag2_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_diag2_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_sleep_pin(uint8_t *state)
{
  if (NULL == state) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_sleep_pin(&ctx, state) == DCMOTOR26_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_read_an_pin_value(uint16_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_read_an_pin_value(&ctx, data_out) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_read_an_pin_voltage(float *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_read_an_pin_voltage(&ctx, data_out) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_get_motor_current(float *current)
{
  if (NULL == current) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor26_get_motor_current(&ctx, current) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch1_operation_mode(uint8_t mode)
{
  return (dcmotor26_set_ch1_operation_mode(&ctx, mode) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch2_operation_mode(uint8_t mode)
{
  return (dcmotor26_set_ch2_operation_mode(&ctx, mode) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch1_drive_frequency(uint16_t drv_freq)
{
  return (dcmotor26_set_ch1_drive_frequency(&ctx, drv_freq) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch2_drive_frequency(uint16_t drv_freq)
{
  return (dcmotor26_set_ch2_drive_frequency(&ctx, drv_freq) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch1_duty_period(uint16_t duty_period)
{
  return (dcmotor26_set_ch1_duty_period(&ctx, duty_period) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_tb9053ftg_set_ch2_duty_period(uint16_t duty_period)
{
  return (dcmotor26_set_ch2_duty_period(&ctx, duty_period) == DCMOTOR26_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}
