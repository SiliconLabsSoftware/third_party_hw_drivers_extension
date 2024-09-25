/***************************************************************************//**
 * @file sparkfun_ccs811.h
 * @brief header file for CCS811 sensor driver
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SPARKFUN_CCS811_H
#define SPARKFUN_CCS811_H

#include <stdbool.h>
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "drv_i2c_master.h"
#include "sparkfun_ccs811_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
* @addtogroup ccs811 CCS811 - Gas Sensor
* @brief  Driver for the Cambridge CMOS Sensors CCS811 gas and indoor air
* quality sensor.
*
*     @n @section ccs811_example CCS811 usage example code
*
*     Basic example for initializing measuring equivalent Co2 and TVOC level.
*     @verbatim
*
#include "sl_i2cspm_instances.h"
#include "sl_ccs811.h"
*
*     int main( void )
*     {
*
*     ...
*
*     // Initialize sensor and set measure mode
*     sl_ccs811_init(sl_i2cspm_sensor_gas);
*     sl_ccs811_set_measure_mode(sl_i2cspm_sensor_gas,
*   CCS811_MEASURE_MODE_DRIVE_MODE_1SEC);
*
*     uint16_t eco2;
*     uint16_t tvoc;
*
*     while (true) {
*
*      if ( sl_ccs811_is_data_available(sl_i2cspm_sensor_gas) ) {
*        sl_ccs811_get_measurement(sl_i2cspm_sensor_gas, &eco2, &tvoc);
*      }
*
*      ...
*
*     }
*
*     } @endverbatim
* @{
******************************************************************************/
/**************************************************************************//**
* @addtogroup ccs811_details CCS881 - Gas Sensor Details
* @brief CCS881 register interface
* @{
******************************************************************************/
/**************************************************************************//**
* @addtogroup ccs811_registers Register Addresses
* @brief Register definitions
* @{
******************************************************************************/

/* Status register */
#define SPARKFUN_CCS811_ADDR_STATUS                   0x00

/* Measurement mode and conditions register */
#define SPARKFUN_CCS811_ADDR_MEASURE_MODE             0x01

/* Algorithm result */
#define SPARKFUN_CCS811_ADDR_ALG_RESULT_DATA          0x02

/* Raw ADC data values for resistance and current source used */
#define SPARKFUN_CCS811_ADDR_RAW_DATA                 0x03

/* Temperature and Humidity data can be written to enable compensation */
#define SPARKFUN_CCS811_ADDR_ENV_DATA                 0x05

/* Provides the voltage across the reference
 *    resistor and the voltage across the NTC resistor */
#define SPARKFUN_CCS811_ADDR_NTC                      0x06

/* Thresholds for operation when interrupts
 *    are only generated when eCO2 ppm crosses a threshold */
#define SPARKFUN_CCS811_ADDR_THRESHOLDS               0x10

/* Hardware ID */
#define SPARKFUN_CCS811_ADDR_HW_ID                    0x20

/* Hardware Version */
#define SPARKFUN_CCS811_ADDR_HW_VERSION               0x21

/* Firmware Boot Version */
#define SPARKFUN_CCS811_ADDR_FW_BOOT_VERSION          0x23

/* Firmware Application Version */
#define SPARKFUN_CCS811_ADDR_FW_APP_VERSION           0x24

/* Error ID */
#define SPARKFUN_CCS811_ADDR_ERR_ID                   0xE0

/* Firmware erase */
#define SPARKFUN_CCS811_ADDR_FW_ERASE                 0xF1

/* Firmware programming */
#define SPARKFUN_CCS811_ADDR_FW_PROGRAM               0xF2

/* Firmware verification */
#define SPARKFUN_CCS811_ADDR_FW_VERIFY                0xF3

/* Application start */
#define SPARKFUN_CCS811_ADDR_APP_START                0xF4

/* Software reset */
#define SPARKFUN_CCS811_ADDR_SW_RESET                 0xFF

/** @} (end addtogroup ccs811_registers) */

/**************************************************************************//**
* @addtogroup ccs811_measuremode Measure mode value definitions
* @brief Measure mode value definitions
* @{
******************************************************************************/

/* DRIVE_MODE field bit shift value */
#define SPARKFUN_CCS811_MEASURE_MODE_DRIVE_MODE_SHIFT 4

/* Idle mode, measurements are disabled */
#define SPARKFUN_CCS811_MEASURE_MODE_DRIVE_MODE_IDLE  0x00

/* IAQ Mode 1, a measurement is performed every second */
#define SPARKFUN_CCS811_MEASURE_MODE_DRIVE_MODE_1SEC  0x10

/* IAQ Mode 2, a measurement is performed every 10 seconds */
#define SPARKFUN_CCS811_MEASURE_MODE_DRIVE_MODE_10SEC 0x20

/* IAQ Mode 3, a measurement is performed every 60 seconds */
#define SPARKFUN_CCS811_MEASURE_MODE_DRIVE_MODE_60SEC 0x30

/* IAQ Mode 4, Raw Data Mode, a measurement
 *    is performed every 250ms for external algorithms */
#define SPARKFUN_CCS811_MEASURE_MODE_DRIVE_MODE_RAW   0x40

/* Interrupt generation enable */
#define SPARKFUN_CCS811_MEASURE_MODE_INTERRUPT        0x08

/* Enable interrupt when eCO2 level exceeds threshold */
#define SPARKFUN_CCS811_MEASURE_MODE_THRESH           0x04

/** @} (end addtogroup ccs811_measuremode) */
/** @} (end addtogroup ccs811_details) */

/***************************************************************************//**
 * @brief
 *    Initialize the chip.
 *
 * @param[in] i2cspm
 *   The I2CSPM instance to use.
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_INITIALIZATION Initialization was unsuccessful
 ******************************************************************************/
sl_status_t sparkfun_ccs811_init(mikroe_i2c_handle_t i2cspm);

/***************************************************************************//**
 * @brief
 *    Reads Hardware ID from the CCS811 sensor
 *
 * @param[out] hwID
 *    The Hardware ID of the chip (should be 0x81)
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_ccs811_get_hardware_id(uint8_t *hwID);

/***************************************************************************//**
 * @brief
 *    Set the measurement mode of the CCS811 sensor.
 *
 * @details
 *    This function must be called before reading measurements from the sensor.
 *
 * @param[in] measMode
 *    The desired measurement mode
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_ccs811_set_measure_mode(uint8_t measMode);

/***************************************************************************//**
 * @brief
 *    Perform a firmware update of the CCS811 sensor.
 *
 * @details
 *    This function overwrites the existing firmware, irrespective of the
 *    version number.
 *
 * @param[in] firmware
 *    A buffer containing the contents of the firmware update
 *
 * @param[in] length
 *    The length of the firmware update array
 *
 * @return
 *    @retval SL_STATUS_OK Firmware upgrade successful
 *    @retval SL_STATUS_FAIL Firmware upgrade failed
 ******************************************************************************/
sl_status_t sparkfun_ccs811_update_firmware(const uint8_t *firmware,
                                            uint32_t length);

/***************************************************************************//**
 * @brief
 *    Read out current firmware of the CCS811 sensor.
 *
 * @param[out] fw_version
 *    The current application firmware. The top 4 bits contain the major
 *   firmware
 *    version, the next 4 bits contain the minor and the last 8 bits contain the
 *   patch version.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_ccs811_read_firmware_version(uint16_t *fw_version);

/**************************************************************************//**
* @brief
*    Read the status of the CCS811 sensor.
*
* @param[out] status
*    The content of the CCS811 Status Register
*
* @return
*    @retval SL_STATUS_OK Success
*    @retval SL_STATUS_TRANSMIT I2C transmission error
******************************************************************************/
sl_status_t sparkfun_ccs811_get_status(uint8_t *status);

/***************************************************************************//**
 * @brief
 *    Read data from a specific Mailbox address.
 *
 * @param[in] id
 *    The address of the Mailbox register
 *
 * @param[in] length
 *    The number of bytes to read
 *
 * @param[out] data
 *    The data read from the sensor
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_ccs811_read_mailbox(uint8_t id,
                                         uint8_t length,
                                         uint8_t *data);

/***************************************************************************//**
 * @brief
 *    Switch the CCS811 chip from boot to application mode.
 *
 * @return
 *    @retval SL_STATUS_OK  Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission failure
 *    @retval SL_STATUS_NOT_AVAILABLE Application firmware is not present in the
 *   CCS811 device
 *    @retval SL_STATUS_INVALID_STATE Chip firmware did not switch to
 *   application mode
 ******************************************************************************/
sl_status_t sparkfun_ccs811_start_application(void);

/***************************************************************************//**
 * @brief
 *    Perform software reset on the CCS811.
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_ccs811_software_reset(void);

/***************************************************************************//**
 * @brief
 *    Check whether new measurement data is available.
 *
 * @return
 *    True if new data available, otherwise false
 ******************************************************************************/
bool sparkfun_ccs811_is_data_available(void);

/***************************************************************************//**
 * @brief
 *    Read measurement data (eCO2 and TVOC) from the CCS811 sensor.
 *
 * @param[out] eco2
 *    The equivalent CO2 level (in ppm) read from the sensor
 *
 * @param[out] tvoc
 *    The TVOC level (in ppb) read from the sensor
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_ccs811_get_measurement(uint16_t *eco2,
                                            uint16_t *tvoc);

/***************************************************************************//**
 * @brief
 *    Get the latest readings from the sense resistor of the CCS811 sensor.
 *
 * @param[out] current
 *    The value of current through the sensor
 *
 * @param[out] rawData
 *    The raw ADC reading of the voltage across the sensor with the selected
 *    current
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 *****************************************************************************/
sl_status_t sparkfun_ccs811_get_raw_data(uint16_t *current,
                                         uint16_t *rawData);

/***************************************************************************//**
 * @brief
 *    Write temperature and humidity values to the environmental data regs.
 *
 * @param[in] tempData
 *    The environmental temperature in milliCelsius
 *
 * @param[in] rhData
 *    The relative humidity in millipercent
 *
 * @return
 *    @retval SL_STATUS_OK Success
 *    @retval SL_STATUS_TRANSMIT I2C transmission error
 ******************************************************************************/
sl_status_t sparkfun_ccs811_set_env_data(int32_t tempData,
                                         uint32_t rhData);

/** @} (end addtogroup ccs811) */

#ifdef __cplusplus
}
#endif
#endif // SPARKFUN_CCS811_H
