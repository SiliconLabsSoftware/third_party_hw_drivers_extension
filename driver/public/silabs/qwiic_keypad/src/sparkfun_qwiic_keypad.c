/***************************************************************************//**
 * @file sparkfun_qwiic_keypad.c
 * @brief Sparkfun Keypad source file.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "sparkfun_qwiic_keypad.h"
#include "sparkfun_keypad_config.h"

#if (defined(SLI_SI917))
#include "sl_driver_gpio.h"
#define GPIO_M4_INTR              7 // M4 Pin interrupt number
#define AVL_INTR_NO               0 // available interrupt number
#else
#include "gpiointerrupt.h"
#endif

typedef struct
{
  i2c_master_t i2c;
  buttonEvent_callback user_callback;
  digital_in_t interrupt_pin;
}qwiic_keypad_t;

static qwiic_keypad_t qwiic_keypad;

/***************************************************************************//**
 *  Initialize the keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_init(mikroe_i2c_handle_t i2c_handle,
                                 uint8_t address,
                                 buttonEvent_callback user_callback)
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

  qwiic_keypad.i2c.handle = i2c_handle;

#if (SPARKFUN_KEYPAD_I2C_UC == 1)
  i2c_cfg.speed = SPARKFUN_KEYPAD_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&qwiic_keypad.i2c, &i2c_cfg) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  i2c_master_set_speed(&qwiic_keypad.i2c, i2c_cfg.speed);
  i2c_master_set_timeout(&qwiic_keypad.i2c, 0);

  if (NULL != user_callback) {
    qwiic_keypad.user_callback = user_callback;

#ifdef SPARKFUN_KEYPAD_GPIO_INT_PIN
    pin_name_t int_pin_1 = hal_gpio_pin_name(SPARKFUN_KEYPAD_GPIO_INT_PORT,
                                             SPARKFUN_KEYPAD_GPIO_INT_PIN);
    digital_in_init(&qwiic_keypad.interrupt_pin, int_pin_1);

#if (defined(SLI_SI917))
    sl_gpio_t gpio_port_pin = { SPARKFUN_KEYPAD_GPIO_INT_PIN / 16,
                                SPARKFUN_KEYPAD_GPIO_INT_PIN % 16 };
    sl_gpio_driver_configure_interrupt(&gpio_port_pin,
                                       GPIO_M4_INTR,
                                       SL_GPIO_INTERRUPT_FALLING_EDGE,
                                       (void *)qwiic_keypad.user_callback,
                                       AVL_INTR_NO);
#else // None Si91x device
    GPIOINT_CallbackRegister(SPARKFUN_KEYPAD_GPIO_INT_PIN,
                             qwiic_keypad.user_callback);
    GPIO_ExtIntConfig(SPARKFUN_KEYPAD_GPIO_INT_PORT,
                      SPARKFUN_KEYPAD_GPIO_INT_PIN,
                      SPARKFUN_KEYPAD_GPIO_INT_PIN,
                      false,
                      true,
                      true);
#endif
#endif
  }

  if (!sparkfun_keypad_present(address)) {
    // Wait for keypad to become ready
    sl_sleeptimer_delay_millisecond(80);

    if (!sparkfun_keypad_present(address)) {
      return SL_STATUS_NOT_AVAILABLE;
    }
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Check whether a special keypad is present on the I2C bus or not.
 ******************************************************************************/
bool sparkfun_keypad_present(uint8_t device_id)
{
  sl_status_t sc;
  uint8_t backup_addr;

  // Back up the current i2c addr
  backup_addr = qwiic_keypad.i2c.config.addr;

  // Use special addr to check
  i2c_master_set_slave_address(&qwiic_keypad.i2c, device_id);
  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_ID, &device_id);

  // Restore to the backed up i2c addr
  i2c_master_set_slave_address(&qwiic_keypad.i2c, backup_addr);
  if (sc != SL_STATUS_OK) {
    return false;
  }

  return true;
}

/***************************************************************************//**
 *  Sets new I2C address for Sparkfun keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_set_address(uint8_t address)
{
  sl_status_t sc;

  if ((address < 0x08) || (address > 0x77)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sc = sparkfun_keypad_write_register(SPARKFUN_KEYPAD_CHANGE_ADDRESS,
                                      address);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  i2c_master_set_slave_address(&qwiic_keypad.i2c, address);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Gets current I2C address used of keypad.
 ******************************************************************************/
uint8_t sparkfun_keypad_get_address(void)
{
  return qwiic_keypad.i2c.config.addr;
}

/***************************************************************************//**
 *  Scans I2C address of Sparkfun keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_scan_address(uint8_t *address, uint8_t *num_dev)
{
  *num_dev = 0;

  if ((address == NULL) || (num_dev == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  for (uint8_t addr = 0x08; addr < 0x78; addr++) {
    if (sparkfun_keypad_present(addr)) {
      *(address + *num_dev) = addr;
      (*num_dev)++;
    }
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Selects device on the I2C bus.
 ******************************************************************************/
sl_status_t sparkfun_keypad_select_device(uint8_t address)
{
  if ((address < 0x08) || (address > 0x77)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  i2c_master_set_slave_address(&qwiic_keypad.i2c, address);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads Firmware Version from the keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_get_firmware_version(frw_rev_t *fwRev)
{
  sl_status_t sc;
  uint8_t major;
  uint8_t minor;

  if ((fwRev == NULL) || (NULL == qwiic_keypad.i2c.handle)) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_VERSION1, &major);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_VERSION2, &minor);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  fwRev->major = major;
  fwRev->minor = minor;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads the last button pressed from keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_read_last_button(uint8_t *data)
{
  sl_status_t sc;

  if ((data == NULL) || (NULL == qwiic_keypad.i2c.handle)) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_LAST_BUTTON,
                                 data);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Reads the time elapsed since the last button event.
 ******************************************************************************/
sl_status_t sparkfun_keypad_time_since_last_button_event(uint16_t *time)
{
  sl_status_t sc;

  uint8_t msb;
  uint8_t lsb;

  if (time == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_TIME_MSB,
                                 &msb);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  sc = sparkfun_keypad_read_data(SPARKFUN_KEYPAD_TIME_LSB,
                                 &lsb);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  *time = ((uint16_t)msb << 8) | lsb;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  "commands" keypad to plug in the next button into the registerMap
 ******************************************************************************/
sl_status_t sparkfun_keypad_update_fifo()
{
  sl_status_t sc;

  sc = sparkfun_keypad_write_register(SPARKFUN_KEYPAD_UPDATE_FIFO, 0x01);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Reads a byte of data to the keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_read_data(uint8_t reg_addr, uint8_t *data)
{
  if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(&qwiic_keypad.i2c,
                                                       &reg_addr,
                                                       1,
                                                       data,
                                                       1)) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Writes a byte of data to the keypad.
 ******************************************************************************/
sl_status_t sparkfun_keypad_write_register(uint8_t reg_addr, uint8_t data)
{
  uint8_t i2c_write_data[2] = { reg_addr, data };

  if (I2C_MASTER_SUCCESS != i2c_master_write(&qwiic_keypad.i2c,
                                             i2c_write_data,
                                             2)) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}
