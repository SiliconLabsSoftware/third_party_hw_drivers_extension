/***************************************************************************//**
 * @file sparkfun_qwiic_joystick.h
 * @brief Joystick header file
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
#ifndef SPARKFUN_QWIIC_JOYSTICK_H__
#define SPARKFUN_QWIIC_JOYSTICK_H__

#include "sl_status.h"
#include "stdbool.h"

#include "drv_i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup sparkfun_joystick Joystick - SparkFun Qwiic Joystick
 * @brief
 *  The integration of the SparkFun Qwiic Joystick driver.
 *
 *    @n @section joystick_example joystick example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "sl_i2cspm_instances.h"
 * #include "sparkfun_joystick_attiny85.h"
 *    int main(void)
 *    {
 *      ...
 *      uint16_t data_pos;
 *
 *      // Initialize joystick
 *      sparkfun_joystick_init(sl_i2cspm_qwiic, SPARKFUN_JOYSTICK_I2C_ADDRESS);
 *
 *      while (true) {
 *
 *
 *        // Reading current horizontal position
 *        sparkfun_joystick_read_horizontal_position(&data_pos);
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup  joystick_commands Commands
 * @brief Joystick Commands
 * @{
 ******************************************************************************/
#define SPARKFUN_JOYSTICK_DEFAULT_ADDR             0x20

#define SPARKFUN_JOYSTICK_ID                       0x00
// Firmware Number
#define SPARKFUN_JOYSTICK_FWREV_1                  0x01
#define SPARKFUN_JOYSTICK_FWREV_2                  0x02
// Current Joystick Horizontal Position (MSB)
#define SPARKFUN_JOYSTICK_HORIZONTAL_POSITION_MSB  0x03
// Current Joystick Horizontal Position (LSB)
#define SPARKFUN_JOYSTICK_HORIZONTAL_POSITION_LSB  0x04
// Current Joystick Vertical Position (MSB)
#define SPARKFUN_JOYSTICK_VERTICAL_POSITION_MSB    0x05
// Current Joystick Vertical Position (LSB)
#define SPARKFUN_JOYSTICK_VERTICAL_POSITION_LSB    0x06
// Current Button State (clears Reg 0x08)
#define SPARKFUN_JOYSTICK_BUTTON_POSITION          0x07
// Indicator for if button was pressed since last
// read of button state (Reg 0x07). Clears after read.
#define SPARKFUN_JOYSTICK_BUTTON_STATUS            0x08
// Must be changed to 0x13 before I2C address can be changed
#define SPARKFUN_JOYSTICK_LOCK_REGISTER            0x09
// Set I2C New Address (re-writable).
#define SPARKFUN_JOYSTICK_CHANGE_ADDRESS           0x0A

/** @} (end addtogroup joystick_commands) */

/***************************************************************************//**
* @addtogroup joystick_typedefs Types
* @brief Joystick Types
* @{
*******************************************************************************/

// Struct of Firmware version.
typedef struct frw_rev {
  uint8_t major;
  uint8_t minor;
} frw_rev_t;

/** @} (end addtogroup joystick_typedefs) */

/***************************************************************************//**
 * @brief
 *  Initialize the Joystick.
 *
 * @param[in] i2c_handle
 *  The I2C peripheral to use.
 *
 * @param[in] address
 *  The I2C address to probe.
 *
 * @return
 *  @retval SL_STATUS_OK An joystick device is present on the I2C bus
 *  @retval SL_STATUS_INITIALIZATION No Joystick device present
 ******************************************************************************/
sl_status_t sparkfun_joystick_init(mikroe_i2c_handle_t i2c_handle,
                                   uint8_t address);

/***************************************************************************//**
 * @brief
 *  Sets new I2C address for Joystick.
 *
 * @param[in] address
 *  The I2C address to use.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_joystick_set_address(uint8_t address);

/***************************************************************************//**
 * @brief
 *  Gets current I2C address used of Joystick.
 *
 * @retval Current I2C address used.
 ******************************************************************************/
uint8_t sparkfun_joystick_get_address(void);

/***************************************************************************//**
 * @brief
 *  Scans I2C address of all Joystick that connected on the I2C bus.
 *
 * @param[out] address
 *  The pointer points to the memory area that holds the found addresses.
 * @param[out] num_dev
 *  The number of found device.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_joystick_scan_address(uint8_t *address, uint8_t *num_dev);

/***************************************************************************//**
 * @brief
 *  Check whether a special Joystick is present on the I2C bus or not.
 *
 * @param[in] device_id
 *  Address of Joystick to check.
 *
 * @return
 *  @retval true A joystick device is present on the I2C bus
 *  @retval false No joystick device present
 ******************************************************************************/
bool sparkfun_joystick_present(uint8_t device_id);

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
sl_status_t sparkfun_joystick_select_device(uint8_t address);

/***************************************************************************//**
 * @brief
 *  Read Firmware Version from the Joystick.
 *
 * @param[out] fwRev
 *  The internal firmware Version.
 *
 * @return
 *  @retval SL_STATUS_OK Success
 *  @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_joystick_get_firmware_version(frw_rev_t *fwRev);

/***************************************************************************//**
 * @brief
 *  Reads Current Horizontal Position from Joystick.
 *
 * @param[out] data
 *  The data read from the Joystick.
 *
 * @return
 *  @retval SL_STATUS_OK Success
 *  @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_joystick_read_horizontal_position(uint16_t *data);

/***************************************************************************//**
 * @brief
 *  Reads Current Vertical Position from Joystick.
 *
 * @param[out] data
 *  The data read from the Joystick.
 *
 * @return
 *  @retval SL_STATUS_OK Success
 *  @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_joystick_read_vertical_position(uint16_t *data);

/***************************************************************************//**
 * @brief
 *  Reads Current Button Position from Joystick.
 *
 * @param[out] data
 *  The data read from the Joystick.
 *  Return 0: button is currently being pressed
 *
 * @return
 *  @retval SL_STATUS_OK Success
 *  @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_joystick_read_button_position(uint8_t *data);

/***************************************************************************//**
 * @brief
 *  Reads Status of Button.
 *
 * @param[out] data
 *  The data read from the Joystick.
 *  Returns 1 if button was pressed between reads of button.
 *
 * @detail
 *  Indicator for if button was pressed since last
 *  read of button state (Reg 0x07). Clears after read.
 *
 * @return
 *  @retval SL_STATUS_OK Success
 *  @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_joystick_check_button(uint8_t *data);

/***************************************************************************//**
 * @brief
 *    Reads a byte of data to the Joystick.
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
sl_status_t sparkfun_joystick_read_data(uint8_t reg_addr, uint8_t *data);

/***************************************************************************//**
 * @brief
 *    Writes a byte of data to the Joystick.
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
sl_status_t sparkfun_joystick_write_register(uint8_t reg_addr, uint8_t data);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup sparkfun_joystick) */

#endif // SPARKFUN_QWIIC_JOYSTICK_H__
