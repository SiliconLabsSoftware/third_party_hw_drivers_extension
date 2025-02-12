/***************************************************************************//**
 * @file mikroe_bma400_i2c.c
 * @brief I2C abstraction used by BMA400
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
#include "mikroe_bma400_i2c.h"

// Read write length varies based on user requirement
#define READ_WRITE_LENGTH  UINT8_C(46)

// Variable to store the i2cspm instance
typedef struct
{
  i2c_master_t i2c;
  digital_in_t interrupt_pin_1;
  digital_in_t interrupt_pin_2;
  uint8_t   intf_ref;
} bma400_handle_t;

static bma400_handle_t bma400_handle;

// Local prototypes
static int8_t bma400_i2c_read(uint8_t reg_addr,
                              uint8_t *reg_data,
                              uint32_t len,
                              void *intf_ptr);
static int8_t bma400_i2c_write(uint8_t reg_addr,
                               const uint8_t *reg_data,
                               uint32_t len,
                               void *intf_ptr);
static void bma400_delay_us(uint32_t period, void *intf_ptr);

/***************************************************************************//**
 * @brief
 *  Initialize I2C Interface for the BMA400.
 *
 * @param[in] i2cspm
 *  The I2CSPM instance to use.
 * @param[in] bma400_i2c_addr
 *  The I2C address of the BMA400.
 * @param[out] bma400
 *  The BMA400 device structure.
 *
 * @return
 *  @ref BMA400_OK on success.
 *  @ref On failure, BMA400_E_NULL_PTR is returned.
 ******************************************************************************/
int8_t bma400_i2c_init(mikroe_i2c_handle_t i2cspm,
                       uint8_t bma400_i2c_addr,
                       struct bma400_dev *bma400)
{
  if ((bma400 == NULL) || (NULL == i2cspm)) {
    return BMA400_E_NULL_PTR;
  }

  // The device needs startup time
  sl_sleeptimer_delay_millisecond(10);

  i2c_master_config_t i2c_cfg;
  i2c_master_configure_default(&i2c_cfg);

  i2c_cfg.addr = bma400_i2c_addr;
  bma400_handle.i2c.handle = i2cspm;

#if (MIKROE_BMA400_I2C_UC == 1)
  i2c_cfg.speed = MIKROE_BMA400_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&bma400_handle.i2c, &i2c_cfg) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  i2c_master_set_speed(&bma400_handle.i2c, i2c_cfg.speed);
  i2c_master_set_timeout(&bma400_handle.i2c, 0);

  bma400->read = bma400_i2c_read;
  bma400->write = bma400_i2c_write;
  bma400->intf = BMA400_I2C_INTF;

  bma400->intf_ptr = &bma400_handle.intf_ref;
  bma400->delay_us = bma400_delay_us;
  bma400->read_write_len = READ_WRITE_LENGTH;

#ifdef  MIKROE_BMA400_INT1_PORT
  pin_name_t int_pin_1 = hal_gpio_pin_name(MIKROE_BMA400_INT1_PORT,
                                           MIKROE_BMA400_INT1_PIN);
  digital_in_init(&bma400_handle.interrupt_pin_1, int_pin_1);
#endif

#ifdef  MIKROE_BMA400_INT2_PORT
  pin_name_t int_pin_2 = hal_gpio_pin_name(MIKROE_BMA400_INT2_PORT,
                                           MIKROE_BMA400_INT2_PIN);
  digital_in_init(&bma400_handle.interrupt_pin_2, int_pin_2);
#endif

  return BMA400_OK;
}

/***************************************************************************//**
 * @brief I2C read function map to platform
 ******************************************************************************/
static int8_t bma400_i2c_read(uint8_t reg_addr,
                              uint8_t *reg_data,
                              uint32_t len,
                              void *intf_ptr)
{
  (void) intf_ptr;

  if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(&bma400_handle.i2c,
                                                       &reg_addr,
                                                       1,
                                                       reg_data,
                                                       len)) {
    return BMA400_E_COM_FAIL;
  }

  return BMA400_OK;
}

/***************************************************************************//**
 * @brief I2C write function map to platform
 ******************************************************************************/
static int8_t bma400_i2c_write(uint8_t reg_addr,
                               const uint8_t *reg_data,
                               uint32_t len,
                               void *intf_ptr)
{
  (void) intf_ptr;

  uint8_t i2c_write_data[len + 1];

  // Select register and data to write
  i2c_write_data[0] = reg_addr;
  for (uint16_t i = 0; i < (uint16_t)len; i++) {
    i2c_write_data[i + 1] = reg_data[i];
  }

  if (I2C_MASTER_SUCCESS != i2c_master_write(&bma400_handle.i2c,
                                             i2c_write_data,
                                             len + 1)) {
    return BMA400_E_COM_FAIL;
  }

  return BMA400_OK;
}

/***************************************************************************//**
 * @brief delay function map to platform
 ******************************************************************************/
static void bma400_delay_us(uint32_t period, void *intf_ptr)
{
  (void) intf_ptr;
  uint32_t delay_ms = 1;

  if (period > 1000) {
    delay_ms = (period / 1000) + 1;
  }

  sl_sleeptimer_delay_millisecond(delay_ms);
}
