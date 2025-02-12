/***************************************************************************//**
 * @file mikroe_drv8245p.c
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

#include "mikroe_drv8245p.h"
#include "mikroe_dcmotor29_config.h"

static dcmotor29_t ctx;
static dcmotor29_cfg_t ctx_cfg;

sl_status_t mikroe_drv8245p_init(mikroe_spi_handle_t spi_instance,
                                 mikroe_i2c_handle_t i2c_instance)
{
  if ((NULL == spi_instance) || (NULL == i2c_instance)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  dcmotor29_cfg_setup(&ctx_cfg);

  ctx.i2c.handle = i2c_instance;
  ctx.spi.handle = spi_instance;
  ctx.slave_address = MIKROE_DRV8245P_DEVICE_ADDRESS_A1A0_00;

#if defined(DCMOTOR29_IP_PORT) && defined(DCMOTOR29_IP_PIN)
  ctx_cfg.ip = hal_gpio_pin_name(DCMOTOR29_IP_PORT,
                                 DCMOTOR29_IP_PIN);
#endif

#if defined(DCMOTOR29_RST_PORT) && defined(DCMOTOR29_RST_PIN)
  ctx_cfg.rst = hal_gpio_pin_name(DCMOTOR29_RST_PORT,
                                  DCMOTOR29_RST_PIN);
#endif

#if defined(DCMOTOR29_IN1_PORT) && defined(DCMOTOR29_IN1_PIN)
  ctx_cfg.in1 = hal_gpio_pin_name(DCMOTOR29_IN1_PORT,
                                  DCMOTOR29_IN1_PIN);
#endif

#if defined(DCMOTOR29_FLTIN_PORT) && defined(DCMOTOR29_FLTIN_PIN)
  ctx_cfg.int_pin = hal_gpio_pin_name(DCMOTOR29_FLTIN_PORT,
                                      DCMOTOR29_FLTIN_PIN);
#endif

#if (MIKROE_DCMOTOR29_SPI_UC == 1)
  ctx_cfg.spi_speed = MIKROE_DCMOTOR29_SPI_BITRATE;
#endif

#if defined(DCMOTOR29_CS_PORT) && defined(DCMOTOR29_CS_PIN)
  ctx_cfg.cs = hal_gpio_pin_name(DCMOTOR29_CS_PORT,
                                 DCMOTOR29_CS_PIN);
  digital_out_t struct_cs;
  digital_out_init(&struct_cs, ctx_cfg.cs);
#endif

#if (MIKROE_DCMOTOR29_I2C_UC == 1)
  ctx_cfg.i2c_speed = MIKROE_DCMOTOR29_I2C_SPEED_MODE;
#endif

  if (dcmotor29_init(&ctx, &ctx_cfg) != DCMOTOR29_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_drv8245p_default_cfg(void)
{
  return (dcmotor29_default_cfg(&ctx) == DCMOTOR29_OK)
         ?SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_drv8245p_generic_write(uint8_t reg,
                                          uint8_t *data_in,
                                          uint8_t len)
{
  return (dcmotor29_generic_write(&ctx, reg, data_in, len) == DCMOTOR29_OK)
         ?SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_drv8245p_generic_read(uint8_t reg,
                                         uint8_t *data_out,
                                         uint8_t len)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor29_generic_read(&ctx, reg, data_out, len) == DCMOTOR29_OK)
         ?SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_drv8245p_register_write(uint8_t reg, uint8_t data_in)
{
  return (dcmotor29_register_write(&ctx, reg, data_in) == DCMOTOR29_OK)
         ?SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_drv8245p_register_read(uint8_t reg, uint8_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor29_register_read(&ctx, reg, data_out) == DCMOTOR29_OK)
         ?SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_drv8245p_port_expander_write(uint8_t reg, uint8_t data_in)
{
  return (dcmotor29_port_expander_write(&ctx, reg, data_in) == DCMOTOR29_OK)
         ?SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_drv8245p_port_expander_read(uint8_t reg, uint8_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (dcmotor29_port_expander_read(&ctx, reg, data_out) == DCMOTOR29_OK)
         ?SL_STATUS_OK : SL_STATUS_FAIL;
}

void mikroe_drv8245p_set_in1_pin(uint8_t pin_state)
{
  dcmotor29_set_in1_pin(&ctx, pin_state);
}

void mikroe_drv8245p_set_rst_pin(uint8_t pin_state)
{
  dcmotor29_set_rst_pin(&ctx, pin_state);
}

uint8_t mikroe_drv8245p_get_ip_pin(void)
{
  return dcmotor29_get_ip_pin(&ctx);
}

uint8_t mikroe_drv8245p_get_int_pin(void)
{
  return dcmotor29_get_int_pin(&ctx);
}

sl_status_t mikroe_drv8245p_set_pins(uint8_t set_mask, uint8_t clr_mask)
{
  return (dcmotor29_set_pins(&ctx, set_mask, clr_mask) == DCMOTOR29_OK)
         ?SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_drv8245p_drvoff_state(uint8_t drvoff_state)
{
  return (dcmotor29_drvoff_state(&ctx, drvoff_state) == DCMOTOR29_OK)
         ?SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_drv8245p_drive_motor(uint8_t state)
{
  return (dcmotor29_drive_motor(&ctx, state) == DCMOTOR29_OK)
         ?SL_STATUS_OK : SL_STATUS_FAIL;
}
