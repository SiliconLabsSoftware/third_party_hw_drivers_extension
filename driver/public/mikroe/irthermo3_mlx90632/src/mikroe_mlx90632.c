/***************************************************************************//**
 * @file mikroe_mlx90632.c
 * @brief SCL MLX90632 Source File
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

#include "mikroe_mlx90632.h"
#include "mikroe_mlx90632_config.h"

static irthermo3_t irthermo3;
static irthermo3_cfg_t irthermo3_cfg;
static uint8_t adr_rst[4] = { 0x30, 0x05, 0x00, 0x06 };

sl_status_t mikroe_mlx90632_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (i2cspm_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  irthermo3.i2c.handle = i2cspm_instance;

  // Call basic setup functions
  irthermo3_cfg_setup(&irthermo3_cfg);

  irthermo3_cfg.i2c_address = MLX90632_I2C_ADDR;

#if (MIKROE_I2C_MLX90632_UC == 1)
  irthermo3_cfg.i2c_speed = MIKROE_I2C_MLX90632_SPEED_MODE;
#endif

  if (irthermo3_init(&irthermo3, &irthermo3_cfg) != IRTHERMO3_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  if (mikroe_mlx90632_present() != SL_STATUS_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_mlx90632_present(void)
{
  if (mikroe_mlx90632_read_i16(MIKROE_IRTHERMO3_REG_I2C_ADDRESS)
      != (irthermo3_cfg.i2c_address >> 1)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

void mikroe_mlx90632_default_config(void)
{
  mikroe_mlx90632_global_write(adr_rst, 4);
  mikroe_mlx90632_cal();
}

sl_status_t mikroe_mlx90632_set_i2csmp_instance(
  mikroe_i2c_handle_t i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  irthermo3.i2c.handle = i2cspm_instance;

  return SL_STATUS_OK;
}

void mikroe_mlx90632_generic_write(uint16_t reg,
                                   uint8_t *data_buf, uint8_t len)
{
  irthermo3_generic_write(&irthermo3, reg, data_buf, len);
}

void mikroe_mlx90632_generic_read(uint16_t reg,
                                  uint8_t *data_buf, uint8_t len)
{
  irthermo3_generic_read(&irthermo3, reg, data_buf, len);
}

void mikroe_mlx90632_write_u16(uint16_t reg,
                               uint16_t wr_data)
{
  irthermo3_write_u16(&irthermo3, reg, wr_data);
}

int16_t mikroe_mlx90632_read_i16(uint16_t reg)
{
  return irthermo3_read_i16(&irthermo3, reg);
}

int32_t mikroe_mlx90632_read_i32(uint16_t reg)
{
  return irthermo3_read_i32(&irthermo3, reg);
}

void mikroe_mlx90632_global_write(uint8_t *data_buf, uint8_t len)
{
  irthermo3_global_write(&irthermo3, data_buf, len);
}

void mikroe_mlx90632_set_mode(uint8_t mode)
{
  irthermo3_set_mode(&irthermo3, mode);
}

void mikroe_mlx90632_cal(void)
{
  irthermo3_cal(&irthermo3);
}

float mikroe_mlx90632_get_ambient_temperature(void)
{
  return irthermo3_get_ambient_temperature(&irthermo3);
}

float mikroe_mlx90632_get_object_temperature(void)
{
  return irthermo3_get_object_temperature(&irthermo3);
}
