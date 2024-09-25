/***************************************************************************//**
 * @file mikroe_ph2.h
 * @brief Mikroe Ph2 Prototypes
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef MIKROE_PH2_INC_H_
#define MIKROE_PH2_INC_H_

#include "sl_status.h"
#include "drv_i2c_master.h"
#include "drv_one_wire.h"
#include "ph2.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup ph2 pH 2 Click Driver
 * @brief API for configuring and manipulating pH 2 Click driver.
 * @{
 */

/**
 * @defgroup ph2_cmd pH 2 Commands List
 * @brief List of commands of pH 2 Click driver.
 */

/**
 * @addtogroup ph2_cmd
 * @{
 */

/**
 * @brief pH 2 DS18B20 commands list.
 * @details Specified commands list for setting DS18B20 of pH 2 Click driver.
 */
#define MIKROE_PH2_CONVERT_TEMPERATURE  PH2_CMD_CONVERT_TEMPERATURE
#define MIKROE_PH2_WRITE_SCRATCHPAD     PH2_CMD_WRITE_SCRATCHPAD
#define MIKROE_PH2_READ_SCRATCHPAD      PH2_CMD_READ_SCRATCHPAD
#define MIKROE_PH2_COPY_SCRATCHPAD      PH2_CMD_COPY_SCRATCHPAD
#define MIKROE_PH2_RECALL_SCRATCHPAD    PH2_CMD_RECALL_SCRATCHPAD
#define MIKROE_PH2_READ_POWER_SUPPLY    PH2_CMD_READ_POWER_SUPPLY

/*! @} */ // ph2_cmd

/**
 * @defgroup ph2_set pH 2 Registers Settings
 * @brief Settings for registers of pH 2 Click driver.
 */

/**
 * @addtogroup ph2_set
 * @{
 */

/**
 * @brief pH 2 description setting.
 * @details Specified setting for description of pH 2 Click driver.
 */
#define MIKROE_PH2_PH_CALCULATION_COEF        PH2_PH_CALCULATION_COEF

#define MIKROE_PH2_CONFIG_ADDRESS             PH2_CONFIG_ADDRESS

/**
 * @brief pH 2 ADC setting.
 * @details Specified settings for ADC of pH 2 Click driver.
 */
#define MIKROE_PH2_ADC_RESOLUTION             PH2_ADC_RESOLUTION
#define MIKROE_PH2_VREF_3V3                   PH2_VREF_3V3
#define MIKROE_PH2_VREF_5V                    PH2_VREF_5V

/**
 * @brief pH 2 device address setting.
 * @details Specified setting for device slave address selection of
 * pH 2 Click driver.
 */
#define MIKROE_PH2_SET_DEV_ADDR               PH2_SET_DEV_ADDR

/**
 * @brief pH 2 DS18B20 family code value.
 * @details Specified family code value of DS18B20 of pH 2 2 Click driver.
 */
#define MIKROE_PH2_FAMILY_CODE                PH2_FAMILY_CODE

/**
 * @brief pH 2 config register settings.
 * @details Specified config register settings of pH 2 Click driver.
 */

#define MIKROE_PH2_CONFIG_RESOLUTION_9BIT     PH2_CONFIG_RESOLUTION_9BIT
#define MIKROE_PH2_CONFIG_RESOLUTION_10BIT    PH2_CONFIG_RESOLUTION_10BIT
#define MIKROE_PH2_CONFIG_RESOLUTION_11BIT    PH2_CONFIG_RESOLUTION_11BIT
#define MIKROE_PH2_CONFIG_RESOLUTION_12BIT    PH2_CONFIG_RESOLUTION_12BIT
#define MIKROE_PH2_CONFIG_RESOLUTION_BIT_MASK PH2_CONFIG_RESOLUTION_BIT_MASK
#define MIKROE_PH2_CONFIG_RESERVED_BITS       PH2_CONFIG_RESERVED_BITS

/**
 * @brief pH 2 temperature calculation values.
 * @details Specified temperature calculation values of pH 2 Click driver.
 */
#define MIKROE_PH2_DATA_RESOLUTION            PH2_DATA_RESOLUTION

/**
 * @brief pH 2 default config values.
 * @details Specified default config values of pH 2 Click driver.
 */
#define MIKROE_PH2_DEFAULT_ADDRESS            PH2_DEFAULT_ADDRESS
#define MIKROE_PH2_DEFAULT_RESOLUTION         PH2_DEFAULT_RESOLUTION
#define MIKROE_PH2_DEFAULT_TEMP_HIGH_ALARM    PH2_DEFAULT_TEMP_HIGH_ALARM
#define MIKROE_PH2_DEFAULT_TEMP_LOW_ALARM     PH2_DEFAULT_TEMP_LOW_ALARM

/***************************************************************************//**
 * @brief
 * Initialization function.
 *
 * @param[in] i2c_instance
 *    mikroe_i2c_handle_t instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 *    SL_STATUS_INITIALIZATION Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ph2_init(mikroe_i2c_handle_t i2c_instance);

/***************************************************************************//**
 * @brief
 * This function sets the mikroe_i2c_handle_t instance used by platform functions.
 *
 * @param[in] i2c_instance
 *    mikroe_i2c_handle_t instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_ph2_set_i2c_instance(mikroe_i2c_handle_t i2c_instance);

/***************************************************************************//**
 * @brief
 * This function read raw ADC value function.
 *
 * @param[in] raw_adc
 *     Output ADC result
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_read_raw_adc(uint16_t *raw_adc);

/***************************************************************************//**
 * @brief
 * This function reads raw ADC value and converts it to proportional voltage
 * level.
 *
 * @param[in] voltage
 *     Output voltage level (volts).
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_read_voltage(float *voltage);

/***************************************************************************//**
 * @brief
 * This function sets the voltage reference for pH 2 click driver.
 *
 * @param[in] vref
 *     Reference voltage (volts).
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_set_vref(float vref);

/***************************************************************************//**
 * @brief
 * This function is performing calibration of Ph 2 click driver
 * with known pH value substance.
 *
 * @param[in] pH_value
 *     pH value of the substance where calibration is performed.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_calibrate(float pH_value);

/***************************************************************************//**
 * @brief
 * This function is reading pH value wof substance where pH probe is placed.
 *
 * @param[in] pH_value
 *     pH value of the substance.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 * @note This function calculates the pH value with the assumption that the
 * temperature of the substance is 25 deg Celsius.
 ******************************************************************************/
sl_status_t mikroe_ph2_calculate_ph(float *pH_value);

/***************************************************************************//**
 * @brief
 * This function is used to calibrate offset of Ph 2 click driver.
 * @param[in]:
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 * @note Disconnect the probe and short-circuit the BNC connector in order to
 * set the reference voltage. Adjust the potentiometer until the LEDs stop
 * blinking or until the voltage of the OUT pin is exactly at half the value of
 * the selected VCC voltage. If you are using LEDs for calibration, turn the
 * potentiometer clockwise when STAT1 LED is blinking and counter-clockwise
 * when STAT2 LED is blinking.
 ******************************************************************************/
sl_status_t mikroe_ph2_calibrate_offset(void);

/***************************************************************************//**
 * @brief
 * This function is used turn on/off STAT1 LED of Ph 2 click driver.
 * @param[in] state
 *     TRUE: turn on
 *     FALSE: turn off
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_enable_st1(bool state);

/***************************************************************************//**
 * @brief
 * This function is used turn on/off STAT2 LED of Ph 2 click driver.
 * @param[in] state
 *     TRUE: turn on
 *     FALSE: turn off
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_enable_st2(bool state);

/***************************************************************************//**
 * @brief
 * This function initializes all necessary pins and peripherals used
 * for DS18B20 sensor.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_INITIALIZATION: Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ds18b20_init(void);

/***************************************************************************//**
 * @brief
 * This function checks the communication by reading the ROM address
 * as well as verifying family code and Maxim CRC 8.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_check_communication(void);

/***************************************************************************//**
 * @brief
 * This function selects the device one OneWirebus
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_select_device(void);

/***************************************************************************//**
 * @brief
 * This function reads a desired number of scratchpad bytes using One Wire
 * communication protocol.
 * @param[out] scratchpad
 *             Output read scratchpad.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_read_scratchpad(uint8_t *scratchpad);

/***************************************************************************//**
 * @brief
 * This function writes the temperature thresholds and configuration byte to
 * the scratchpad.
 * @param[in] temp_high : Temperature high limit.
 * @param[in] temp_low : Temperature low limit.
 * @param[in] config : Configuration byte.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_write_scratchpad(uint8_t temp_high,
                                            uint8_t temp_low,
                                            uint8_t config);

/***************************************************************************//**
 * @brief
 * This function executes a default configuration of pH 2 DS18B20 sensor.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_default_cfg (void);

/***************************************************************************//**
 * @brief
 * This function starts the measurement by sending an appropriate command for
 * that.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_start_measurement(void);

/***************************************************************************//**
 * @brief
 * This function reads the temperature value in Celsius.
 * @param[out] temperature : Temperature in Celsius.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_read_temperature (float *temperature);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_PH2_INC_H_
