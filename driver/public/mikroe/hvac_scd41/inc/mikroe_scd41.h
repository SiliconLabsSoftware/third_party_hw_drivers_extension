/***************************************************************************//**
 * @file mikroe_scd41.h
 * @brief Mikroe SCD41 Prototypes
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
#ifndef MIKROE_HVAC_SCD41_INC_H_
#define MIKROE_HVAC_SCD41_INC_H_

#include "sl_status.h"
#include "hvac.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief HVAC List of SCD41 sensor commands.
 * @details Specified list of SCD41 sensor commands of HVAC Click driver.
 */
#define MIKROE_SCD41_START_PERIODIC_MEASUREMENT                          0x21B1
#define MIKROE_SCD41_START_LOW_POWER_PERIODIC_MEASUREMENT                0x21AC
#define MIKROE_SCD41_START_ULTRA_LOW_POWER_PERIODIC_MEASUREMENT          0x21A7
#define MIKROE_SCD41_MEASURE_SINGLE_SHOT                                 0x219D
#define MIKROE_SCD41_MEASURE_SINGLE_SHOT_RHT_ONLY                        0x2196
#define MIKROE_SCD41_READ_MEASUREMENT                                    0xEC05
#define MIKROE_SCD41_STOP_PERIODIC_MEASUREMENT                           0x3F86
#define MIKROE_SCD41_GET_SERIAL_NUMBER                                   0x3682
#define MIKROE_SCD41_GET_FEATURE_SET_VERSION                             0x202F
#define MIKROE_SCD41_GET_TEMPERATURE_OFFSET                              0x2318
#define MIKROE_SCD41_SET_TEMPERATURE_OFFSET                              0x241D
#define MIKROE_SCD41_PERFORM_FORCED_RECALIBRATION                        0x362F
#define MIKROE_SCD41_GET_AUTOMATIC_SELF_CALIBRATION_ENABLED              0x2313
#define MIKROE_SCD41_SET_AUTOMATIC_SELF_CALIBRATION_ENABLED              0x2416
#define MIKROE_SCD41_GET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD       0x2340
#define MIKROE_SCD41_SET_AUTOMATIC_SELF_CALIBRATION_INITIAL_PERIOD       0x2445
#define MIKROE_SCD41_GET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD      0x234B
#define MIKROE_SCD41_SET_AUTOMATIC_SELF_CALIBRATION_STANDARD_PERIOD      0x244E
#define MIKROE_SCD41_PERSIST_SETTINGS                                    0x3615
#define MIKROE_SCD41_PERFORM_FACTORY_RESET                               0x3632
#define MIKROE_SCD41_REINIT                                              0x3646

/**
 * @brief HVAC device address setting.
 * @details Specified setting for device slave address selection of
 * HVAC SCD41 Click driver.
 */
#define MIKROE_SCD41_SCD41_SLAVE_ADDR                                    0x62

typedef measuremen_data_t mikroe_scd41_measuremen_data_t;
typedef feature_data_t mikroe_scd41_feature_data_t;
typedef mass_and_num_cnt_data_t mikroe_scd41_mass_and_num_cnt_data_t;

/***************************************************************************//**
 * @brief
 * Initialization function.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_scd41_init(mikroe_i2c_handle_t i2c_instance);

/***************************************************************************//**
 * @brief
 * This function sets the IC2SPM instance used by platform functions.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_scd41_set_i2csmp_instance(mikroe_i2c_handle_t i2c_instance);

/***************************************************************************//**
 * @brief
 * This function writes a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 *
 * @param[in] reg : Start register address.
 * @param[in] tx_buf : Data to be written.
 * @param[in] tx_len : Number of bytes to be written.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if tx_buf is null
 *    SL_STATUS_OK - Success
 *    SL_STATUS_IO - Error
 ******************************************************************************/
sl_status_t mikroe_scd41_generic_write(uint8_t reg,
                                       uint8_t *tx_buf,
                                       uint8_t tx_len);

/***************************************************************************//**
 * @brief
 * This function reads a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 *
 * @param[in] reg : Start register address.
 * @param[out] rx_buf : Output read data
 * @param[in] rx_len : Number of bytes to be read.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if rx_buf is null
 *    SL_STATUS_OK - Success
 *    SL_STATUS_IO - Error
 ******************************************************************************/
sl_status_t mikroe_scd41_generic_read (uint8_t reg, uint8_t *rx_buf,
                                       uint8_t rx_len);

/***************************************************************************//**
 * @brief
 * The function writes a 16-bit data to the targeted 16-bit
 * register address of the SCD41 CO2, RH and T Sensor on the HVAC click board.
 * @param[in] reg : 16-bit register address.
 * @param[in] tx_data : 16-bit data to be written.
 *
 * @return
 *    SL_STATUS_OK - Success
 *    SL_STATUS_IO - Error
 ******************************************************************************/
sl_status_t mikroe_scd41_write_data (uint16_t reg, uint16_t tx_data);

/***************************************************************************//**
 * @brief
 * The function read a 16-bit data to the targeted 16-bit
 * register address of the SCD41 CO2, RH and T Sensor on the HVAC click board.
 * @param[in] reg : 16-bit register address.
 * @param[out] rx_data : Pointer to the memory location where 16-bit read
 * data be stored.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if rx_data is null
 *    SL_STATUS_OK - Success
 *    SL_STATUS_IO - Error
 ******************************************************************************/
sl_status_t mikroe_scd41_read_data (uint16_t reg, uint16_t *rx_data);

/***************************************************************************//**
 * @brief
 * The function send command to the SCD41 CO2, RH and T Sensor
 * on the HVAC click board.
 * @param[in] reg : 16-bit command.
 *
 * @return Nothing.
 ******************************************************************************/
void mikroe_scd41_send_cmd (uint16_t cmd);

/***************************************************************************//**
 * @brief
 * The function read sensor measurement data of the SCD41 CO2, RH and T Sensor
 * on the HVAC click board.
 * @param[in] m_data : Pointer to the memory location where structure data be
 * stored.
 *
 * @return
 * SL_STATUS_INVALID_PARAMETER if m_data is null
 * SL_STATUS_OK - Success
 ******************************************************************************/
sl_status_t mikroe_scd41_read_measurement (
  mikroe_scd41_measuremen_data_t *m_data);

/***************************************************************************//**
 * @brief
 * The function get serial number value of SCD41 sensor on the HVAC click board.
 * @param[in] serial_number : Pointer to the memory location where serial number
 *   value
 * be stored.
 *
 * @return
 * SL_STATUS_INVALID_PARAMETER if serial_number is null
 * SL_STATUS_OK - Success
 ******************************************************************************/
sl_status_t mikroe_scd41_get_serial_number (uint16_t *serial_number);

/***************************************************************************//**
 * @brief
 * The function get feature set version value of SCD41 sensor on the HVAC click
 * board.
 * @param[in] f_data : Pointer to the memory location where structure data be
 * stored.
 *
 * @return
 * SL_STATUS_INVALID_PARAMETER if f_data is null
 * SL_STATUS_OK - Success
 ******************************************************************************/
sl_status_t mikroe_scd41_get_feature_set_version(
  mikroe_scd41_feature_data_t *f_data);

/***************************************************************************//**
 * @brief
 * The function set temperature offset value of SCD41 sensor on the HVAC click
 * board.
 * @param[in] temp_offset: Float temperature offset value ( degree Celsius ).
 * stored.
 *
 * @return Nothing.
 ******************************************************************************/
void mikroe_scd41_set_temperature_offset(float temp_offset);

/***************************************************************************//**
 * @brief
 * The function get temperature offset value of SCD41 sensor on the HVAC click
 * board.
 *
 * @return Float temperature offset value ( degree Celsius ).
 ******************************************************************************/
float mikroe_scd41_get_temperature_offset(void);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_HVAC_SCD41_INC_H_ */
