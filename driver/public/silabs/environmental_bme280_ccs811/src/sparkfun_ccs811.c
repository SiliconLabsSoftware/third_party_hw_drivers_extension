/***************************************************************************//**
 * @file
 * @brief Driver for the Cambridge CMOS Sensors CCS811 gas and indoor air
 * quality sensor
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "drv_digital_out.h"
#include "sparkfun_ccs811_config.h"
#include "sparkfun_ccs811.h"

#define SPARKFUN_CCS811_HW_ID           (0x81)

typedef struct {
  i2c_master_t i2c;
  uint8_t slave_address;
  digital_out_t wak_pin;
} air_quality_t;

static air_quality_t air_quality;

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
// Local prototypes
static uint32_t sparkfun_ccs811_erase_application(void);
static uint32_t sparkfun_ccs811_verify_application(bool *appValid);
static uint32_t sparkfun_ccs811_program_firmware(uint8_t buffer[]);
static uint32_t sparkfun_ccs811_set_app_start(void);

/** @endcond */

/**************************************************************************//**
* @defgroup CCS811 CCS811 - Indoor Air Quality Sensor
* @{
* @brief Driver for the Cambridge CMOS Sensors CCS811 gas and indoor air
* quality sensor
******************************************************************************/

/***************************************************************************//**
 * @brief
 *    Wakes up the Air Quality / Gas Sensor
 *
 * @param[in] wake
 *    Set true to wake up, false otherwise
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
static uint32_t sparkfun_ccs811_wake(bool wake)
{
  if (wake) {
    digital_out_low(&air_quality.wak_pin);
    sl_sleeptimer_delay_millisecond(1);
  } else {
    digital_out_high(&air_quality.wak_pin);
    sl_sleeptimer_delay_millisecond(1);
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Initializes the chip
 ******************************************************************************/
sl_status_t sparkfun_ccs811_init(mikroe_i2c_handle_t i2cspm)
{
  uint8_t id;
  sl_status_t status;
  i2c_master_config_t air_quality_config;

  // Configure default i2csmp instance
  air_quality.i2c.handle = i2cspm;

  i2c_master_configure_default(&air_quality_config);

  air_quality_config.addr = SPARKFUN_CCS811_ADDR;
  air_quality.slave_address = air_quality_config.addr;

#if (SPARKFUN_CCS811_I2C_UC == 1)
  air_quality_config.speed = SPARKFUN_CCS811_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&air_quality.i2c,
                      &air_quality_config) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  i2c_master_set_slave_address(&air_quality.i2c, air_quality.slave_address);
  i2c_master_set_speed(&air_quality.i2c, air_quality_config.speed);
  i2c_master_set_timeout(&air_quality.i2c, 0);

  digital_out_init(&air_quality.wak_pin,
                   hal_gpio_pin_name(SPARKFUN_CCS811_WAKE_PORT,
                                     SPARKFUN_CCS811_WAKE_PIN));

  // Set the wake pin low
  sparkfun_ccs811_wake(true);

  // About 80 ms required to reliably start the device, wait a bit more
  sl_sleeptimer_delay_millisecond(100);

  // Check if the chip present and working by reading the hardware ID
  status = sparkfun_ccs811_get_hardware_id(&id);
  if ((status != SL_STATUS_OK) || (id != SPARKFUN_CCS811_HW_ID)) {
    return SL_STATUS_INITIALIZATION;
  }

  // Switch from boot mode to application mode
  status = sparkfun_ccs811_start_application();
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = sparkfun_ccs811_set_measure_mode(
    SPARKFUN_CCS811_MEASURE_MODE_DRIVE_MODE_1SEC);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Go back to sleep
  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    De-initializes the chip
 ******************************************************************************/
sl_status_t sparkfun_ccs811_deinit(i2c_master_t *i2cspm)
{
  // Disable the sensor
  (void)i2cspm;

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Reads Hardware ID from the CCS811 sensor
 ******************************************************************************/
sl_status_t sparkfun_ccs811_get_hardware_id(uint8_t *hardwareID)
{
  sl_status_t result;

  result = sparkfun_ccs811_read_mailbox(SPARKFUN_CCS811_ADDR_HW_ID,
                                        1,
                                        hardwareID);

  return result;
}

/**************************************************************************//**
*    Reads the status of the CCS811 sensor
******************************************************************************/
sl_status_t sparkfun_ccs811_get_status(uint8_t *status)
{
  sl_status_t result;

  result = sparkfun_ccs811_read_mailbox(SPARKFUN_CCS811_ADDR_STATUS,
                                        1,
                                        status);

  return result;
}

/***************************************************************************//**
 *    Checks if new measurement data available
 ******************************************************************************/
bool sparkfun_ccs811_is_data_available(void)
{
  bool state;
  sl_status_t status;
  uint8_t reg;

  state = false;

  // Read the status register
  status = sparkfun_ccs811_get_status(&reg);

  // Check if the DATA_READY bit is set
  if ((status == SL_STATUS_OK) && ((reg & 0x08) == 0x08)) {
    state = true;
  }

  return state;
}

/***************************************************************************//**
 *    Switches the CCS811 chip from boot to application mode
 ******************************************************************************/
sl_status_t sparkfun_ccs811_start_application(void)
{
  sl_status_t result;
  uint8_t status;

  // Read status
  result = sparkfun_ccs811_read_mailbox(SPARKFUN_CCS811_ADDR_STATUS,
                                        1,
                                        &status);

  /* If no application firmware present in the CCS811 then return an error
   * message
   */
  if ((status & 0x10) != 0x10) {
    return SL_STATUS_NOT_AVAILABLE;
  }

  // Issue APP_START
  result = sparkfun_ccs811_set_app_start();
  if (result != SL_STATUS_OK) {
    return result;
  }

  // Wait 1 ms after app start to send new i2c commands
  sl_sleeptimer_delay_millisecond(1);

  // Check status again
  result = sparkfun_ccs811_read_mailbox(SPARKFUN_CCS811_ADDR_STATUS,
                                        1,
                                        &status);

  /* If the chip firmware did not switch to application mode then return with
   * error
   */
  if ((status & 0x90) != 0x90) {
    return SL_STATUS_INVALID_STATE;
  }

  return result;
}

/***************************************************************************//**
 *    Reads data from a specific Mailbox address
 ******************************************************************************/
sl_status_t sparkfun_ccs811_read_mailbox(uint8_t id,
                                         uint8_t length,
                                         uint8_t *data)
{
  sparkfun_ccs811_wake(true);

  if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(&air_quality.i2c,
                                                       &id,
                                                       1,
                                                       data,
                                                       length)) {
    return SL_STATUS_TRANSMIT;
  }

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Reads measurement data (eCO2 and TVOC) from the CCS811 sensor
 ******************************************************************************/
sl_status_t sparkfun_ccs811_get_measurement(uint16_t *eco2,
                                            uint16_t *tvoc)
{
  uint8_t i2c_read_data[4];
  uint8_t i2c_write_data = SPARKFUN_CCS811_ADDR_ALG_RESULT_DATA;

  sparkfun_ccs811_wake(true);

  if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(&air_quality.i2c,
                                                       &i2c_write_data,
                                                       1,
                                                       i2c_read_data,
                                                       4)) {
    return SL_STATUS_TRANSMIT;
  }

  // Convert the read bytes to 16 bit values
  *eco2 = ((uint16_t) i2c_read_data[0] << 8) + (uint16_t) i2c_read_data[1];
  *tvoc = ((uint16_t) i2c_read_data[2] << 8) + (uint16_t) i2c_read_data[3];

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Gets the latest readings from the sense resistor of the CCS811 sensor
 *****************************************************************************/
sl_status_t sparkfun_ccs811_get_raw_data(uint16_t *current,
                                         uint16_t *rawData)
{
  uint8_t i2c_read_data[2];
  uint8_t i2c_write_data = SPARKFUN_CCS811_ADDR_RAW_DATA;

  sparkfun_ccs811_wake(true);

  if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(&air_quality.i2c,
                                                       &i2c_write_data,
                                                       1,
                                                       i2c_read_data,
                                                       2)) {
    return SL_STATUS_TRANSMIT;
  }

  // current: the upper six bits of Byte0
  *current = (uint16_t) ((i2c_read_data[0] >> 2) & 0x3F);

  // raw ADC reading: the lower two bits of Byte0 is the MSB, Byte1 is the LSB
  *rawData =
    (uint16_t) ((i2c_read_data[0] & 0x03) << 8) + (uint16_t) i2c_read_data[1];

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Performs software reset on the CCS811
 ******************************************************************************/
sl_status_t sparkfun_ccs811_software_reset(void)
{
  uint8_t i2c_write_data[5];

  sparkfun_ccs811_wake(true);

  /* Write the 0x11 0xE5 0x72 0x8A key sequence to software reset register
   * The key sequence is used to prevent accidental reset
   */
  i2c_write_data[0] = SPARKFUN_CCS811_ADDR_SW_RESET;
  i2c_write_data[1] = 0x11;
  i2c_write_data[2] = 0xE5;
  i2c_write_data[3] = 0x72;
  i2c_write_data[4] = 0x8A;

  if (I2C_MASTER_SUCCESS != i2c_master_write(&air_quality.i2c,
                                             i2c_write_data,
                                             5)) {
    return SL_STATUS_TRANSMIT;
  }

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Sets the measurement mode of the CCS811 sensor
 ******************************************************************************/
sl_status_t sparkfun_ccs811_set_measure_mode(uint8_t measMode)
{
  uint8_t i2c_write_data[2];

  sparkfun_ccs811_wake(true);

  // Bits 7,6,2,1 and 0 are reserved, clear them
  measMode = (measMode & 0x38);

  // Write to the measurement mode register
  i2c_write_data[0] = SPARKFUN_CCS811_ADDR_MEASURE_MODE;
  i2c_write_data[1] = measMode;

  if (I2C_MASTER_SUCCESS != i2c_master_write(&air_quality.i2c,
                                             i2c_write_data,
                                             2)) {
    return SL_STATUS_TRANSMIT;
  }

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    Changes the mode of the CCS811 from Boot mode to running the application
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
static uint32_t sparkfun_ccs811_set_app_start(void)
{
  uint8_t i2c_write_data;

  sparkfun_ccs811_wake(true);

  /* Perform a write with no data to the APP_START register to change the
   * state from boot mode to application mode
   */
  i2c_write_data = SPARKFUN_CCS811_ADDR_APP_START;

  if (I2C_MASTER_SUCCESS != i2c_master_write(&air_quality.i2c,
                                             &i2c_write_data,
                                             1)) {
    return SL_STATUS_TRANSMIT;
  }

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Writes temperature and humidity values to the environmental data regs
 ******************************************************************************/
sl_status_t sparkfun_ccs811_set_env_data(int32_t tempData,
                                         uint32_t rhData)
{
  uint8_t i2c_write_data[5];
  uint8_t humidityRegValue;
  uint8_t temperatureRegValue;

  sparkfun_ccs811_wake(true);

  /* The CCS811 currently supports only 0.5% resolution
   * If the fraction greater than 0.7 then round up the value
   * Shift to the left by one to meet the required data format
   */
  if (((rhData % 1000) / 100) > 7) {
    humidityRegValue = (rhData / 1000 + 1) << 1;
  } else {
    humidityRegValue = (rhData / 1000) << 1;
  }

  /* If the fraction is greater than 0.2 or less than 0.8 set the
   * LSB bit, which is the most significant bit of the fraction 2^(-1) = 0.5
   */
  if ((((rhData % 1000) / 100) > 2) && (((rhData % 1000) / 100) < 8)) {
    humidityRegValue |= 0x01;
  }

  /* Add +25 C to the temperature value
   * The CCS811 currently supports only 0.5C resolution
   * If the fraction greater than 0.7 then round up the value
   * Shift to the left by one to meet the required data format
   */
  tempData += 25000;
  if (((tempData % 1000) / 100) > 7) {
    temperatureRegValue = (tempData / 1000 + 1) << 1;
  } else {
    temperatureRegValue = (tempData / 1000) << 1;
  }

  /* If the fraction is greater than 0.2 or less than 0.8 set the
   * LSB bit, which is the most significant bit of the fraction 2^(-1) = 0.5
   */
  if ((((tempData % 1000) / 100) > 2) && (((tempData % 1000) / 100) < 8)) {
    temperatureRegValue |= 0x01;
  }

  /* Write the correctly formatted values to the environmental data register
   * The LSB bytes of the humidity and temperature data are 0x00 because
   * the current CCS811 supports only 0.5% and 0.5C resolution
   */
  i2c_write_data[0] = SPARKFUN_CCS811_ADDR_ENV_DATA;
  i2c_write_data[1] = humidityRegValue;
  i2c_write_data[2] = 0x00;
  i2c_write_data[3] = temperatureRegValue;
  i2c_write_data[4] = 0x00;

  if (I2C_MASTER_SUCCESS != i2c_master_write(&air_quality.i2c,
                                             i2c_write_data,
                                             5)) {
    return SL_STATUS_TRANSMIT;
  }

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Read out current firmware of the CCS811 sensor
 ******************************************************************************/
sl_status_t sparkfun_ccs811_read_firmware_version(uint16_t *fw_version)
{
  sl_status_t status;
  uint8_t buffer[2];
  status = sparkfun_ccs811_read_mailbox(SPARKFUN_CCS811_ADDR_FW_APP_VERSION,
                                        2,
                                        buffer);
  if (status != SL_STATUS_OK) {
    return status;
  }

  *fw_version = (buffer[0] << 8) + buffer[1];

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Performs a firmware update of the CCS811 sensor
 ******************************************************************************/
sl_status_t sparkfun_ccs811_update_firmware(const uint8_t *firmware,
                                            uint32_t length)
{
  sl_status_t status;
  uint8_t reg;
  uint8_t buffer[8];
  uint32_t i;
  bool valid;

  /*************************************************************************/
  /** Put CCS811 in BOOT mode                                             **/
  /*************************************************************************/
  status = sparkfun_ccs811_get_status(&reg);
  if (status != SL_STATUS_OK) {
    return status;
  }

  if ((reg & 0x80) == 0x80) {
    // In APP mode - Go to BOOT mode
    status = sparkfun_ccs811_software_reset();
    if ((status & 0x80) != 0x00) {
      return SL_STATUS_FAIL;
    }

    sl_sleeptimer_delay_millisecond(100);

    // Check for BOOT mode
    status = sparkfun_ccs811_get_status(&reg);
    if ((reg & 0x80) != 0x00) {
      return SL_STATUS_FAIL;
    }
  }

  /*************************************************************************/
  /** Erase                                                               **/
  /*************************************************************************/
  status = sparkfun_ccs811_get_status(&reg);
  if ((reg & 0x10) == 0x10) {
    // Valid application - Erase this
    status = sparkfun_ccs811_erase_application();
    sl_sleeptimer_delay_millisecond(500);
    if (status != SL_STATUS_OK) {
      return SL_STATUS_FAIL;
    }

    sl_sleeptimer_delay_millisecond(100);

    // Check APP_VALID flag again
    status = sparkfun_ccs811_get_status(&reg);
    if ((reg & 0x10) != 0x00) {
      return SL_STATUS_FAIL;
    }
  }

  /*************************************************************************/
  /** Program APP                                                         **/
  /*************************************************************************/
  for ( i = 0; i < length; i += 8 ) {
    // Read the next 8 bytes from the firmware file
    memcpy(&buffer, &firmware[i], 8);

    // Write 8 bytes to the device's flash memory
    status = sparkfun_ccs811_program_firmware(buffer);
    if (status != SL_STATUS_OK) {
      return SL_STATUS_FAIL;
    }
  }

  /*************************************************************************/
  /** Verify APP                                                          **/
  /*************************************************************************/
  status = sparkfun_ccs811_verify_application(&valid);
  if ((status != SL_STATUS_OK) || (valid == false)) {
    return false;
  }

  // Wait at least 70ms before issuing i2c transmissions to the CCS811 sensor
  sl_sleeptimer_delay_millisecond(70);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    Writes 8 bytes of data to the firmware memory of the CCS811
 *
 * @param[in] buffer
 *    An 8-byte length buffer containing the data to write
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
static uint32_t sparkfun_ccs811_program_firmware(uint8_t buffer[])
{
  uint8_t i2c_write_data[9];

  sparkfun_ccs811_wake(true);

  // Send the Write Data to FLASH command and 8 bytes of binary program code
  i2c_write_data[0] = SPARKFUN_CCS811_ADDR_FW_PROGRAM;
  i2c_write_data[1] = buffer[0];
  i2c_write_data[2] = buffer[1];
  i2c_write_data[3] = buffer[2];
  i2c_write_data[4] = buffer[3];
  i2c_write_data[5] = buffer[4];
  i2c_write_data[6] = buffer[5];
  i2c_write_data[7] = buffer[6];
  i2c_write_data[8] = buffer[7];

  if (I2C_MASTER_SUCCESS != i2c_master_write(&air_quality.i2c,
                                             i2c_write_data,
                                             9)) {
    return SL_STATUS_TRANSMIT;
  }

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    Verifies if the application firmware downloaded to the CCS811
 *    was received error free
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 *****************************************************************************/
static uint32_t sparkfun_ccs811_verify_application(bool *appValid)
{
  uint8_t reg;
  uint8_t i2c_write_data = SPARKFUN_CCS811_ADDR_FW_VERIFY;

  *appValid = false;

  sparkfun_ccs811_wake(true);

  /* Write (with no data) to the verify register to check if the firmware
   * programmed to the CCS811 was received error free
   */
  if (I2C_MASTER_SUCCESS != i2c_master_write(&air_quality.i2c,
                                             &i2c_write_data,
                                             1)) {
    return SL_STATUS_TRANSMIT;
  }

  // Wait until the Verify command finishes
  sl_sleeptimer_delay_millisecond(100);

  // Check the status register to see if there were no errors
  if ((sparkfun_ccs811_get_status(&reg) == SL_STATUS_OK)
      && ((reg & 0x10) == 0x10)) {
    *appValid = true;
  }

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    Erases the application firmware on the CCS811
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
static uint32_t sparkfun_ccs811_erase_application(void)
{
  uint8_t i2c_write_data[5];

  sparkfun_ccs811_wake(true);

  /* Write the 0xE7 0xA7 0xE6 0x09 key sequence to firmware erase register
   * The key sequence is used to prevent accidental erase
   */
  i2c_write_data[0] = SPARKFUN_CCS811_ADDR_FW_ERASE;
  i2c_write_data[1] = 0xE7;
  i2c_write_data[2] = 0xA7;
  i2c_write_data[3] = 0xE6;
  i2c_write_data[4] = 0x09;

  if (I2C_MASTER_SUCCESS != i2c_master_write(&air_quality.i2c,
                                             i2c_write_data,
                                             5)) {
    return SL_STATUS_TRANSMIT;
  }

  sparkfun_ccs811_wake(false);

  return SL_STATUS_OK;
}

/** @} (end defgroup CCS811) */
