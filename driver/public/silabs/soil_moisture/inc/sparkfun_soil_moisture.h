/***************************************************************************//**
 * @file sparkfun_soil_moisture.h
 * @brief Sparkfun Soil moisture sensor header file
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
#ifndef SPARKFUN_SOIL_MOISTURE_H_
#define SPARKFUN_SOIL_MOISTURE_H_

#include "sl_status.h"
#include "drv_i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup SEN17731 SEN17731 */

/***************************************************************************//**
 * @addtogroup SEN17731
 * @brief  SEN17731 Soil Moisture Hardware Driver
 * @details
 * @{
 *
 *
 *    @n @section sen17731_example SEN17731 example
 *
 *      Basic example for performing measurement: @n @n
 *      @code{.c}
 *
 * #include "sl_i2cspm_instances.h"
 * #include "sen17731.h"
 *    int main(void)
 *    {
 *      ...
 *      uint8_t moisture;
 *
 *      // Set I2CSPM instance for the sensor
 *      sen17731_set_i2cspm_instance(sl_i2cspm_qwiic);
 *
 *      // Calibrating the sensor
 *      sen17731_set_dry_value(90);
 *      sen17731_set_wet_value(1023);
 *
 *      // Read the soil moisture value
 *      sen17731_get_moisture(&moisture);
 *
 *
 *      ...
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/
// -----------------------------------------------------------------------------
//                               Macros
// -----------------------------------------------------------------------------

// sensor default i2c address
#define SPARKFUN_SOIL_MOISTURE_DEFAULT_ADDR               0x28

// sensor command codes
#define LED_OFF                                           0x00
#define LED_ON                                            0x01
#define CHANGE_ADDRESS                                    0x03
#define GET_VALUE                                         0x05

/***************************************************************************//**
 * @brief
 *  Struct to hold the wet and dry threshhold for calibration.
 ******************************************************************************/
typedef struct {
  uint16_t dry_value;       /*!< value in driest environment */
  uint16_t wet_value;       /*!< value in wetest environment */
} sparkfun_soil_moisture_calibration_t;

// -----------------------------------------------------------------------------
//                       Public Function Definitions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *  Initialize the sparkfun soil moisture sensor.
 *
 * @param[in] i2cspm
 *  The I2C peripheral to use.
 * @param[in] address
 *  The I2C address to use
 *
 * @details
 *  This function initialize the sparkfun soil moisture sensor. It set the
 *  I2C instance and I2C address for the sensor.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_ALREADY_INITIALIZED Initialization is done before.
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_init(mikroe_i2c_handle_t i2cspm,
                                        uint16_t address);

/***************************************************************************//**
 * @brief
 *  Check if the sparkfun soil moisture sensor is on the I2C bus or not.
 *
 * @param[in] address
 *  The I2C address to use to check the presence of the sensor.
 *
 * @retval False false.
 * @retval True  true.
 ******************************************************************************/
bool sparkfun_soil_moisture_is_present(uint16_t address);

/***************************************************************************//**
 * @brief
 *  Sets I2C address for sparkfun soil moisture sensor.
 *
 * @param[in] address
 *  The I2C address to use.
 *
 * @details
 *  This function sets the I2C address for sparkfun soil moisture sensor and
 *  stores the I2C address in static config.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_set_address(uint16_t address);

/***************************************************************************//**
 * @brief
 *  Scans I2C address of all sensors that connected on the I2C bus.
 *
 * @param[out] address
 *  The pointer points to the memory area that holds the found addresses.
 * @param[out] num_dev
 *  The number of found device.
 *
 * @details
 *  This function scans the I2C address for all sensors on the I2C bus and
 *  stores the I2C address in static config.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_scan_address(uint16_t *address,
                                                uint8_t *num_dev);

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
sl_status_t sparkfun_soil_moisture_select_device(uint16_t address);

/**************************************************************************//**
 * @brief
 *  Gets current I2C address of sparkfun soil moisture sensor.
 *
 * @retval The I2C address of sparkfun soil moisture sensor.
 *****************************************************************************/
uint16_t sparkfun_soil_moisture_get_device_address(void);

/***************************************************************************//**
 * @brief
 *  Turns on the LED of sparkfun soil moisture sensor.
 *
 * @details
 *  This function turns on the LED on the sparkfun soil moisture sensor
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_led_on(void);

/***************************************************************************//**
 * @brief
 *  Turns off the LED of sparkfun soil moisture sensor.
 *
 * @details
 *  This function turns off the LED on the sparkfun soil moisture sensor
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_led_off(void);

/***************************************************************************//**
 * @brief
 *  Sets dry value for calibration.
 *
 * @param[in] dry_value
 *  The output of the sensor in dry environment.
 *
 * @details
 *  This function sets the dry value to calibrate for sparkfun soil moisture
 *  sensor.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_set_dry_value(uint16_t dry_value);

/***************************************************************************//**
 * @brief
 *  Sets wet value for calibration.
 *
 * @param[in] wet_value
 *  The output of the sensor in wet environment.
 *
 * @details
 *  This function sets the wet value to calibrate for sparkfun soil moisture
 *  sensor.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_set_wet_value(uint16_t wet_value);

/***************************************************************************//**
 * @brief
 *  Gets the range of moisture value.
 *
 * @param[out] range_values
 *  The range of the moisture value that read out of the sensor.
 *
 * @details
 *  This function gets the range of moisture values that read out from the
 *  sensor.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_get_range_values(
  sparkfun_soil_moisture_calibration_t *range_values);

/***************************************************************************//**
 * @brief
 *  Gets the calibrated moisture value from the sparkfun moisture sensor.
 *
 * @details
 *  This function gets the moisture value which calibrated from the sparkfun
 *  moisture sensor.
 *
 * @param[out] moisture
 *  The soil moisture value percentage which already calibrated.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_get_moisture(uint8_t *moisture);

/***************************************************************************//**
 * @brief
 *  Gets the moisture raw value from the sparkfun moisture sensor.
 *
 * @details
 *  This function gets the raw value from the sparkfun moisture sensor. This
 *  value need to be calibrated to be appropriate with specified soil.
 *
 * @param[out] value
 *  The raw value from the sparkfun sensor.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_FAIL Failure
 ******************************************************************************/
sl_status_t sparkfun_soil_moisture_get_moisture_raw(uint16_t *value);

#ifdef __cplusplus
}
#endif

#endif /* SPARKFUN_SOIL_MOISTURE_H_ */
