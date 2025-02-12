/***************************************************************************//**
 * @file mikroe_bme688_i2c.c
 * @brief I2C abstraction used by BME688
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#include "sl_sleeptimer.h"
#include "mikroe_bme688_i2c.h"
#include "mikroe_bme688_i2c_config.h"

// Local prototypes
static int8_t mikroe_bme688_i2c_read(uint8_t reg_addr,
                                     uint8_t *reg_data,
                                     uint32_t len,
                                     void *intf_ptr);
static int8_t mikroe_bme688_i2c_write(uint8_t reg_addr,
                                      const uint8_t *reg_data,
                                      uint32_t len,
                                      void *intf_ptr);
static void mikroe_bme688_i2c_delay_us(uint32_t period,
                                       void *intf_ptr);

static i2c_master_t bme68x;

/***************************************************************************//**
 *  Initializes an I2C interface for BME688.
 *  As entry point, call this API before using other APIs.
 ******************************************************************************/
int8_t mikroe_bme688_i2c_init(bme68x_dev_t *bme688,
                              mikroe_i2c_handle_t i2c_instance)
{
  if ((bme688 == NULL) || (NULL == i2c_instance)) {
    return BME68X_E_NULL_PTR;
  }
  i2c_master_config_t i2c_cfg;
  i2c_master_configure_default(&i2c_cfg);

  bme68x.handle = i2c_instance;
  i2c_cfg.addr = BME68X_I2C_ADDR_LOW;
  i2c_cfg.timeout_pass_count = 0;

#if (MIKROE_BME688_I2C_UC == 1)
  i2c_cfg.speed = MIKROE_BME688_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&bme68x, &i2c_cfg) == I2C_MASTER_ERROR) {
    return BME68X_E_COM_FAIL;
  }

  bme688->read = mikroe_bme688_i2c_read;
  bme688->write = mikroe_bme688_i2c_write;
  bme688->intf = BME68X_I2C_INTF;

  bme688->delay_us = mikroe_bme688_i2c_delay_us;

  /* The ambient temperature in deg C is used for
   * defining the heater temperature
   */
  bme688->amb_temp = 25;

  return BME68X_OK;
}

/***************************************************************************//**
 * @brief
 *   I2C read function map to platform.
 *
 * @param[in] reg_addr
 *  The start address of the register to send to device.
 * @param[out] reg_data
 *  The data read from the sensor.
 * @param[in] len
 *  Number of registers to read.
 * @param[in] intf_ptr
 *  Interface pointer
 *
 * @return
 *  @ref BME68x_OK on success.
 *  @ref On failure, BME68X_E_COM_FAIL is returned.
 ******************************************************************************/
static int8_t mikroe_bme688_i2c_read(uint8_t reg_addr,
                                     uint8_t *reg_data,
                                     uint32_t len,
                                     void *intf_ptr)
{
  (void)intf_ptr;

  if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(&bme68x,
                                                       &reg_addr,
                                                       1,
                                                       reg_data,
                                                       len)) {
    return BME68X_E_COM_FAIL;
  }

  return BME68X_OK;
}

/***************************************************************************//**
 * @brief
 *   I2C write function map to platform.
 *
 * @param[in] reg_addr
 *  The start address of the register to send to device.
 * @param[out] reg_data
 *  The data write to the sensor.
 * @param[in] len
 *  Number of registers to write.
 * @param[in] intf_ptr
 *  Interface pointer
 *
 * @return
 *  @ref BME68x_OK on success.
 *  @ref On failure, BME68X_E_COM_FAIL is returned.
 ******************************************************************************/
static int8_t mikroe_bme688_i2c_write(uint8_t reg_addr,
                                      const uint8_t *reg_data,
                                      uint32_t len,
                                      void *intf_ptr)
{
  (void)intf_ptr;
  uint8_t write_buff[len + 1];

  write_buff[0] = reg_addr;
  for (uint32_t i = 0; i < len; i++)
  {
    *(write_buff + i + 1) = *(reg_data + i);
  }

  if (I2C_MASTER_SUCCESS != i2c_master_write(&bme68x,
                                             write_buff,
                                             1 + len)) {
    return BME68X_E_COM_FAIL;
  }

  return BME68X_OK;
}

/***************************************************************************//**
 * @brief
 *  Delay function map to platform
 *
 * @param[in] period
 *  Delay in ms
 * @param[in] intf_ptr
 *  Interface pointer
 *
 ******************************************************************************/
static void mikroe_bme688_i2c_delay_us(uint32_t period, void *intf_ptr)
{
  (void) intf_ptr;
  uint32_t delay_ms = 1;

  if (period > 1000) {
    delay_ms = (period / 1000) + 1;
  }

  sl_sleeptimer_delay_millisecond(delay_ms);
}
