/***************************************************************************//**
 * @file sparkfun_qwiic_joystick.c
 * @brief Sparkfun Joystick source file.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include "sl_sleeptimer.h"
#include "sparkfun_qwiic_joystick.h"
#include "sparkfun_qwiic_joystick_config.h"

typedef struct
{
  i2c_master_t i2c;
} joystick_handle_t;

static joystick_handle_t joystick_handle;

/***************************************************************************//**
 *  Initialize the Joystick.
 ******************************************************************************/
sl_status_t sparkfun_joystick_init(mikroe_i2c_handle_t i2c_handle,
                                   uint8_t address)
{
  if (i2c_handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  if ((address < 0x08) || (address > 0x77)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  i2c_master_config_t i2c_cfg;
  i2c_master_configure_default(&i2c_cfg);

  i2c_cfg.addr = address;

  joystick_handle.i2c.handle = i2c_handle;

#if (SPARKFUN_QWIIC_JOYSTICK_I2C_UC == 1)
  i2c_cfg.speed = SPARKFUN_QWIIC_JOYSTICK_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&joystick_handle.i2c, &i2c_cfg) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  i2c_master_set_speed(&joystick_handle.i2c, i2c_cfg.speed);
  i2c_master_set_timeout(&joystick_handle.i2c, 0);

  if (!sparkfun_joystick_present(address)) {
    // Wait for joystick to become ready
    sl_sleeptimer_delay_millisecond(80);

    if (!sparkfun_joystick_present(address)) {
      return SL_STATUS_NOT_AVAILABLE;
    }
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Check whether a special Joystick is present on the I2C bus or not.
 ******************************************************************************/
bool sparkfun_joystick_present(uint8_t device_id)
{
  sl_status_t sc;
  uint8_t backup_addr;

  // Back up the current i2c addr
  backup_addr = joystick_handle.i2c.config.addr;
  // Use special addr to check
  i2c_master_set_slave_address(&joystick_handle.i2c, device_id);
  sc = sparkfun_joystick_read_data(SPARKFUN_JOYSTICK_ID, &device_id);
  // Restore to the backed up i2c addr
  i2c_master_set_slave_address(&joystick_handle.i2c, backup_addr);
  if (sc != SL_STATUS_OK) {
    return false;
  }

  return true;
}

/***************************************************************************//**
 *  Sets new I2C address for Sparkfun Joystick.
 ******************************************************************************/
sl_status_t sparkfun_joystick_set_address(uint8_t address)
{
  sl_status_t sc;

  if ((address < 0x08) || (address > 0x77)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  /* SPARKFUN_JOYSTICK_LOCK_REGISTER must be changed to 0x13
   * before I2C address can be changed
   */
  sc = sparkfun_joystick_write_register(SPARKFUN_JOYSTICK_LOCK_REGISTER, 0x13);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  sc = sparkfun_joystick_write_register(SPARKFUN_JOYSTICK_CHANGE_ADDRESS,
                                        address);
  if (sc != SL_STATUS_OK) {
    return sc;
  }
  i2c_master_set_slave_address(&joystick_handle.i2c, address);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Gets current I2C address used of Joystick.
 ******************************************************************************/
uint8_t sparkfun_joystick_get_address(void)
{
  return joystick_handle.i2c.config.addr;
}

/***************************************************************************//**
 *  Scans I2C address of Sparkfun Joystick.
 ******************************************************************************/
sl_status_t sparkfun_joystick_scan_address(uint8_t *address, uint8_t *num_dev)
{
  *num_dev = 0;

  if ((address == NULL) || (num_dev == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  for (uint8_t addr = 0x08; addr < 0x78; addr++) {
    if (sparkfun_joystick_present(addr)) {
      *(address + *num_dev) = addr;
      (*num_dev)++;
    }
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Selects device on the I2C bus.
 ******************************************************************************/
sl_status_t sparkfun_joystick_select_device(uint8_t address)
{
  if ((address < 0x08) || (address > 0x77)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  i2c_master_set_slave_address(&joystick_handle.i2c, address);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads Firmware Version from the Joystick.
 ******************************************************************************/
sl_status_t sparkfun_joystick_get_firmware_version(frw_rev_t *fwRev)
{
  sl_status_t sc;
  uint8_t major;
  uint8_t minor;

  if (fwRev == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_joystick_read_data(SPARKFUN_JOYSTICK_FWREV_1, &major);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  sc = sparkfun_joystick_read_data(SPARKFUN_JOYSTICK_FWREV_2, &minor);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  fwRev->major = major;
  fwRev->minor = minor;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads Current Horizontal Position from Joystick.
 ******************************************************************************/
sl_status_t sparkfun_joystick_read_horizontal_position(uint16_t *data)
{
  sl_status_t sc;
  uint8_t msb;
  uint8_t lsb;

  if (data == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_joystick_read_data(SPARKFUN_JOYSTICK_HORIZONTAL_POSITION_MSB,
                                   &msb);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  sc = sparkfun_joystick_read_data(SPARKFUN_JOYSTICK_HORIZONTAL_POSITION_LSB,
                                   &lsb);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  *data = (((uint16_t) msb << 8) | lsb) >> 6;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads Current Vertical Position from Joystick.
 ******************************************************************************/
sl_status_t sparkfun_joystick_read_vertical_position(uint16_t *data)
{
  sl_status_t sc;
  uint8_t msb;
  uint8_t lsb;

  if (data == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_joystick_read_data(SPARKFUN_JOYSTICK_VERTICAL_POSITION_MSB,
                                   &msb);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  sc = sparkfun_joystick_read_data(SPARKFUN_JOYSTICK_VERTICAL_POSITION_LSB,
                                   &lsb);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  *data = (((uint16_t) msb << 8) | lsb) >> 6;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads Current Button Position from Joystick.
 ******************************************************************************/
sl_status_t sparkfun_joystick_read_button_position(uint8_t *data)
{
  sl_status_t sc;
  uint8_t pos;

  if (data == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_joystick_read_data(SPARKFUN_JOYSTICK_BUTTON_POSITION, &pos);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  *data = pos;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads Status of Button.
 ******************************************************************************/
sl_status_t sparkfun_joystick_check_button(uint8_t *data)
{
  sl_status_t sc;
  uint8_t staus;

  if (data == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_joystick_read_data(SPARKFUN_JOYSTICK_BUTTON_STATUS, &staus);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  // We've read this status bit, now clear it
  sc = sparkfun_joystick_write_register(SPARKFUN_JOYSTICK_BUTTON_STATUS, 0x00);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  *data = staus;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Reads a byte of data to the Joystick.
 ******************************************************************************/
sl_status_t sparkfun_joystick_read_data(uint8_t reg_addr, uint8_t *data)
{
  if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(&joystick_handle.i2c,
                                                       &reg_addr,
                                                       1,
                                                       data,
                                                       1)) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Writes a byte of data to the Joystick.
 ******************************************************************************/
sl_status_t sparkfun_joystick_write_register(uint8_t reg_addr, uint8_t data)
{
  uint8_t i2c_write_data[2] = { reg_addr, data };

  if (I2C_MASTER_SUCCESS != i2c_master_write(&joystick_handle.i2c,
                                             i2c_write_data,
                                             2)) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}
