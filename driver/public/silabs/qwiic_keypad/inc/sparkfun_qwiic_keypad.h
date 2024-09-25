/***************************************************************************//**
 * @file sparkfun_qwiic_keypad.h
 * @brief Sparkfun Keypad header file.
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

#ifndef SPARKFUN_QWIIC_KEYPAD_H_
#define SPARKFUN_QWIIC_KEYPAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "drv_i2c_master.h"
#include "drv_digital_in.h"
#include "sl_sleeptimer.h"

typedef void (*buttonEvent_callback)(const uint8_t);

enum sparkfun_keypad_registers {
  SPARKFUN_KEYPAD_ID = 0x00,       // Reg: 0x00 - Default I2C Address
  SPARKFUN_KEYPAD_VERSION1,        // Reg: 0x01 - Firmware Number
  SPARKFUN_KEYPAD_VERSION2,        // Reg: 0x02 - Firmware Number
  SPARKFUN_KEYPAD_LAST_BUTTON,     // Reg: 0x03 - oldest button (aka the "first"
                                   //   button pressed)
  SPARKFUN_KEYPAD_TIME_MSB,        // Reg: 0x04 - time in milliseconds since the
                                   //   buttonEvent occured (MSB)
  SPARKFUN_KEYPAD_TIME_LSB,        // Reg: 0x05 - time in milliseconds since the
                                   //   buttonEvent occured (LSB)
  SPARKFUN_KEYPAD_UPDATE_FIFO,     // Reg: 0x06 - "command" from master, set
                                   //   bit0 to command fifo increment
  SPARKFUN_KEYPAD_CHANGE_ADDRESS,  // Reg: 0x07 - Set I2C New Address
                                   //   (re-writable).
};

#define SPARKFUN_KEYPAD_DEFAULT_ADDR 0x4B

// Struct of Firmware version.
typedef struct frw_rev {
  uint8_t major;
  uint8_t minor;
} frw_rev_t;

/***************************************************************************//**
 * @brief
 *  Initialize the keypad.
 *
 * @param[in] i2c_handle
 *  The I2C peripheral to use.
 *
 * @param[in] address
 *  The I2C address to probe.
 *
 * @param[in] address
 *  The I2C address to probe.
 *
 * @return
 *  @retval SL_STATUS_OK An keypad device is present on the I2C bus
 *  @retval SL_STATUS_INITIALIZATION No keypad device present
 ******************************************************************************/
sl_status_t sparkfun_keypad_init(mikroe_i2c_handle_t i2c_handle,
                                 uint8_t address,
                                 buttonEvent_callback user_callback);

/***************************************************************************//**
 * @brief
 *  Sets new I2C address for keypad.
 *
 * @param[in] address
 *  The I2C address to use.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_keypad_set_address(uint8_t address);

/***************************************************************************//**
 * @brief
 *  Gets current I2C address used of keypad.
 *
 * @retval Current I2C address used.
 ******************************************************************************/
uint8_t sparkfun_keypad_get_address(void);

/***************************************************************************//**
 * @brief
 *  Scans I2C address of all keypad that connected on the I2C bus.
 *
 * @param[out] address
 *  The pointer points to the memory area that holds the found addresses.
 * @param[out] num_dev
 *  The number of found device.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_keypad_scan_address(uint8_t *address, uint8_t *num_dev);

/***************************************************************************//**
 * @brief
 *  Check whether a special keypad is present on the I2C bus or not.
 *
 * @param[in] device_id
 *  Address of keypad to check.
 *
 * @return
 *  @retval true A keypad device is present on the I2C bus
 *  @retval false No keypad device present
 ******************************************************************************/
bool sparkfun_keypad_present(uint8_t device_id);

/***************************************************************************//**
 * @brief
 *  Selects device on the I2C bus.
 *
 * @param[in] address
 *  The I2C address of the device that wants to select.
 *
 * @details
 *  This function selects device on the I2C bus based on its I2C address.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_keypad_select_device(uint8_t address);

/***************************************************************************//**
 * @brief
 *  Read Firmware Version from the keypad.
 *
 * @param[out] fwRev
 *  The internal firmware Version.
 *
 * @return
 *  @retval SL_STATUS_OK Success
 *  @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_keypad_get_firmware_version(frw_rev_t *fwRev);

/***************************************************************************//**
 * @brief
 *  Reads the last pressed button from fifo.
 *
 * @param[out] data
 *  The data read from the keypad.
 *  Return 0: fifo is empty
 *
 * @return
 *  @retval SL_STATUS_OK Success
 *  @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_keypad_read_last_button(uint8_t *data);

/***************************************************************************//**
 * @brief
 *  Updates the fifo on the keypad.
 *
 * @detail
 *  Indicator for if button was pressed since last
 *  read of button state (Reg 0x07). Clears after read.
 *
 * @return
 *  @retval SL_STATUS_OK Success
 *  @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_keypad_update_fifo();

/***************************************************************************//**
 * @brief
 *    Reads the time elapsed since the last button event.
 *
 * @param[out] time
 *    The time elapsed in ms
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_TRANSMIT I2C transmit failure
 ******************************************************************************/
sl_status_t sparkfun_keypad_time_since_last_button_event(uint16_t *time);

/***************************************************************************//**
 * @brief
 *    Reads a byte of data to the keypad.
 *
 * @param[in] reg_addr
 *    The register to read
 *
 * @param[out] data
 *    The data to read
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_TRANSMIT I2C transmit failure
 ******************************************************************************/
sl_status_t sparkfun_keypad_read_data(uint8_t reg_addr, uint8_t *data);

/***************************************************************************//**
 * @brief
 *    Writes a byte of data to the keypad.
 *
 * @param[in] reg_addr
 *    The register to write
 *
 * @param[in] data
 *    The data to write
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_TRANSMIT I2C transmit failure
 ******************************************************************************/
sl_status_t sparkfun_keypad_write_register(uint8_t reg_addr, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif // SPARKFUN_QWIIC_KEYPAD_H_
