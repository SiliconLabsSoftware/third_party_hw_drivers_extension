/***************************************************************************//**
 * @file mikroe_max30101.c
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

#include "heartrate4.h"
#include "mikroe_max30101.h"
#include "mikroe_max30101_config.h"

static heartrate4_t heartrate4;
static heartrate4_cfg_t heartrate4_cfg;

sl_status_t mikroe_max30101_init(mikroe_i2c_handle_t i2c_instance)
{
  if (i2c_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  heartrate4.i2c.handle = i2c_instance;

  // Call basic setup functions
  heartrate4_cfg_setup(&heartrate4_cfg);

#if defined(MAX30101_INT_PORT) && defined(MAX30101_INT_PIN)
  heartrate4_cfg.int1 = hal_gpio_pin_name(MAX30101_INT_PORT,
                                          MAX30101_INT_PIN);
#endif

  if (heartrate4_init(&heartrate4, &heartrate4_cfg) != HEARTRATE4_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  mikroe_max30101_default_cfg();

  if (mikroe_max30101_present() != HEARTRATE4_OK) {
    return SL_STATUS_NOT_AVAILABLE;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Check whether a special BMA400 is present on the bus or not.
 ******************************************************************************/
sl_status_t mikroe_max30101_present(void)
{
  uint8_t chip_id;

  chip_id = mikroe_max30101_read_data(MIKROE_MAX30101_REG_CHIP_ID);
  if (chip_id != MIKROE_MAX30101_CHIP_ID) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_max30101_set_i2c_instance(mikroe_i2c_handle_t i2c_instance)
{
  if (NULL == i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  heartrate4.i2c.handle = i2c_instance;

  return SL_STATUS_OK;
}

void mikroe_max30101_default_cfg(void)
{
  heartrate4_default_cfg(&heartrate4);
}

void mikroe_max30101_write_data(uint8_t wr_addr,
                                uint8_t wr_data)
{
  heartrate4_write_data(&heartrate4, wr_addr, wr_data);
}

uint8_t mikroe_max30101_read_data(uint8_t rd_addr)
{
  return heartrate4_read_data(&heartrate4, rd_addr);
}

sl_status_t mikroe_max30101_multi_read(uint8_t rd_addr,
                                       uint8_t *buffer,
                                       uint8_t cnt)
{
  if (NULL == buffer) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  heartrate4_multi_read(&heartrate4, rd_addr, buffer, cnt);
  return SL_STATUS_OK;
}

void mikroe_max30101_set_new_value(uint8_t reg, uint8_t mask, uint8_t value)
{
  heartrate4_set_new_value(&heartrate4, reg, mask, value);
}

uint8_t mikroe_max30101_get_intrrupt(uint8_t flag)
{
  return heartrate4_get_intrrupt(&heartrate4, flag);
}

void mikroe_max30101_enable_disable_interrupts(uint8_t interrupt_flag,
                                               uint8_t enable_flag)
{
  heartrate4_enable_disable_interrupts(&heartrate4, interrupt_flag,
                                       enable_flag);
}

void mikroe_max30101_setting_function(uint8_t flag, uint8_t mode)
{
  heartrate4_setting_function(&heartrate4, flag, mode);
}

void mikroe_max30101_setting_prox_and_amp(uint8_t flag, uint8_t write_data)
{
  heartrate4_setting_prox_and_amp(&heartrate4, flag, write_data);
}

void mikroe_max30101_enable_slot(uint8_t slot_num, uint8_t dev)
{
  heartrate4_enable_slot(&heartrate4, slot_num, dev);
}

void mikroe_max30101_disable_slots(void)
{
  heartrate4_disable_slots(&heartrate4);
}

void mikroe_max30101_clear_fifo(void)
{
  heartrate4_clear_fifo(&heartrate4);
}

void mikroe_max30101_fifo_setting(uint8_t flag, uint8_t samp_num)
{
  heartrate4_fifo_setting(&heartrate4, flag, samp_num);
}

uint8_t mikroe_max30101_get_write_ptr(void)
{
  return heartrate4_get_write_ptr(&heartrate4);
}

uint8_t mikroe_max30101_get_read_ptr(void)
{
  return heartrate4_get_read_ptr(&heartrate4);
}

float mikroe_max30101_read_temp_c(void)
{
  return heartrate4_read_temp_c(&heartrate4);
}

float mikroe_max30101_read_temp_f(void)
{
  return heartrate4_read_temp_f(&heartrate4);
}

void mikroe_max30101_reset(void)
{
  heartrate4_reset(&heartrate4);
}

uint8_t mikroe_max30101_check_int(void)
{
  return heartrate4_check_int(&heartrate4);
}

uint32_t mikroe_max30101_read_fifo_oldest_sample()
{
  return heartrate4_read_fifo_oldest_sample(&heartrate4);
}

uint32_t mikroe_max30101_check_int_pow_and_temp(uint8_t flag)
{
  return heartrate4_check_int_pow_and_temp(&heartrate4, flag);
}

uint16_t mikroe_max30101_read_fifo_all_samples(uint8_t *buff)
{
  return heartrate4_read_fifo_all_samples(&heartrate4, buff);
}

uint32_t mikroe_max30101_get_red_val(void)
{
  return heartrate4_get_red_val(&heartrate4);
}

void mikroe_max30101_set_registers(mikroe_max30101_set_registers_t *registers)
{
  return heartrate4_set_registers(&heartrate4,
                                  (heartrate4_set_registers_t *)registers);
}
