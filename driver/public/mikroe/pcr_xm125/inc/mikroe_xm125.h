/***************************************************************************//**
 * @file mikroe_xm125.h
 * @brief mikroe_xm125.h Prototypes
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
#ifndef MIKROE_PCR_XM125_H_
#define MIKROE_PCR_XM125_H_

#include "sl_status.h"
#include "pcr.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup pcr PCR Click Driver
 * @brief API for configuring and manipulating PCR Click driver.
 * @{
 */

/**
 * @defgroup pcr_reg PCR Registers List
 * @brief List of registers of PCR Click driver.
 */

/**
 * @addtogroup pcr_reg
 * @{
 */

/**
 * @brief PCR description register.
 * @details Specified register for description of PCR Click driver.
 */
#define MIKROE_XM125_REG_VERSION                PCR_REG_VERSION
#define MIKROE_XM125_REG_PROTOCOL_STATUS        PCR_REG_PROTOCOL_STATUS
#define MIKROE_XM125_REG_MEASURE_CNT            PCR_REG_MEASURE_CNT
#define MIKROE_XM125_REG_DETECTOR_STATUS        PCR_REG_DETECTOR_STATUS
#define MIKROE_XM125_REG_DISTANCE_RESULT        PCR_REG_DISTANCE_RESULT
#define MIKROE_XM125_REG_PEAK0_DISTANCE         PCR_REG_PEAK0_DISTANCE
#define MIKROE_XM125_REG_PEAK1_DISTANCE         PCR_REG_PEAK1_DISTANCE
#define MIKROE_XM125_REG_PEAK2_DISTANCE         PCR_REG_PEAK2_DISTANCE
#define MIKROE_XM125_REG_PEAK3_DISTANCE         PCR_REG_PEAK3_DISTANCE
#define MIKROE_XM125_REG_PEAK4_DISTANCE         PCR_REG_PEAK4_DISTANCE
#define MIKROE_XM125_REG_PEAK5_DISTANCE         PCR_REG_PEAK5_DISTANCE
#define MIKROE_XM125_REG_PEAK6_DISTANCE         PCR_REG_PEAK6_DISTANCE
#define MIKROE_XM125_REG_PEAK7_DISTANCE         PCR_REG_PEAK7_DISTANCE
#define MIKROE_XM125_REG_PEAK8_DISTANCE         PCR_REG_PEAK8_DISTANCE
#define MIKROE_XM125_REG_PEAK9_DISTANCE         PCR_REG_PEAK9_DISTANCE
#define MIKROE_XM125_REG_PEAK0_STRENGTH         PCR_REG_PEAK0_STRENGTH
#define MIKROE_XM125_REG_PEAK1_STRENGTH         PCR_REG_PEAK1_STRENGTH
#define MIKROE_XM125_REG_PEAK2_STRENGTH         PCR_REG_PEAK2_STRENGTH
#define MIKROE_XM125_REG_PEAK3_STRENGTH         PCR_REG_PEAK3_STRENGTH
#define MIKROE_XM125_REG_PEAK4_STRENGTH         PCR_REG_PEAK4_STRENGTH
#define MIKROE_XM125_REG_PEAK5_STRENGTH         PCR_REG_PEAK5_STRENGTH
#define MIKROE_XM125_REG_PEAK6_STRENGTH         PCR_REG_PEAK6_STRENGTH
#define MIKROE_XM125_REG_PEAK7_STRENGTH         PCR_REG_PEAK7_STRENGTH
#define MIKROE_XM125_REG_PEAK8_STRENGTH         PCR_REG_PEAK8_STRENGTH
#define MIKROE_XM125_REG_PEAK9_STRENGTH         PCR_REG_PEAK9_STRENGTH
#define MIKROE_XM125_REG_START                  PCR_REG_START
#define MIKROE_XM125_REG_END                    PCR_REG_END
#define MIKROE_XM125_REG_MAX_STEP_LEN           PCR_REG_MAX_STEP_LEN
#define MIKROE_XM125_REG_CLOSE_RANGE_LEAK       PCR_REG_CLOSE_RANGE_LEAK
#define MIKROE_XM125_REG_SIGNAL_QUALITY         PCR_REG_SIGNAL_QUALITY
#define MIKROE_XM125_REG_MAX_PROFILE            PCR_REG_MAX_PROFILE
#define MIKROE_XM125_REG_TRESHOLD_SENSITIVITY   PCR_REG_TRESHOLD_SENSITIVITY
#define MIKROE_XM125_REG_PEAK_SORTI             PCR_REG_PEAK_SORTI
#define MIKROE_XM125_REG_MAX_FRAMES_RECORDED    PCR_REG_MAX_FRAMES_RECORDED
#define MIKROE_XM125_REG_AMPLITUDE_TERSHOLD     PCR_REG_AMPLITUDE_TERSHOLD
#define MIKROE_XM125_REG_TERSHOLD_SENSITIVITY   PCR_REG_TERSHOLD_SENSITIVITY
#define MIKROE_XM125_REG_REFLECTOR_SHAPE        PCR_REG_REFLECTOR_SHAPE
#define MIKROE_XM125_REG_STRENGHT_TRESHOLD      PCR_REG_STRENGHT_TRESHOLD
#define MIKROE_XM125_REG_MEASURE_ON_WAKEUP      PCR_REG_MEASURE_ON_WAKEUP
#define MIKROE_XM125_REG_COMMAND                PCR_REG_COMMAND

/*! @} */ // pcr_reg

/**
 * @defgroup pcr_set PCR Registers Settings
 * @brief Settings for registers of PCR Click driver.
 */

/**
 * @addtogroup pcr_set
 * @{
 */

/**
 * @brief PCR description setting.
 * @details Specified setting for description of PCR Click driver.
 */

/**
 * @brief PCR Detector Status register setting.
 * @details Specified setting for Detector Status register of PCR Click driver.
 */
#define MIKROE_XM125_STATUS_RSS_REG_OK            PCR_STATUS_RSS_REG_OK
#define MIKROE_XM125_STATUS_CFG_CREATE_OK         PCR_STATUS_CFG_CREATE_OK
#define MIKROE_XM125_STATUS_SENSOR_CREATE_OK      PCR_STATUS_SENSOR_CREATE_OK
#define MIKROE_XM125_STATUS_DETECTOR_CREATE_OK    PCR_STATUS_DETECTOR_CREATE_OK
#define MIKROE_XM125_STATUS_DETECTOR_BUFFER_OK    PCR_STATUS_DETECTOR_BUFFER_OK
#define MIKROE_XM125_STATUS_SENSOR_BUFFER_OK      PCR_STATUS_SENSOR_BUFFER_OK
#define MIKROE_XM125_STATUS_CALIB_BUFFER_OK       PCR_STATUS_CALIB_BUFFER_OK
#define MIKROE_XM125_STATUS_CFG_APPLY_OK          PCR_STATUS_CFG_APPLY_OK
#define MIKROE_XM125_STATUS_SENSOR_CALIB_OK       PCR_STATUS_SENSOR_CALIB_OK
#define MIKROE_XM125_STATUS_DETECTOR_CALIB_OK     PCR_STATUS_DETECTOR_CALIB_OK
#define MIKROE_XM125_STATUS_RSS_REG_ERR           PCR_STATUS_RSS_REG_ERR
#define MIKROE_XM125_STATUS_CFG_CREATE_ERR        PCR_STATUS_CFG_CREATE_ERR
#define MIKROE_XM125_STATUS_SENSOR_CREATE_ERR     PCR_STATUS_SENSOR_CREATE_ERR
#define MIKROE_XM125_STATUS_DETECTOR_CREATE_ERR   PCR_STATUS_DETECTOR_CREATE_ERR
#define MIKROE_XM125_STATUS_DETECTOR_BUFFER_ERR   PCR_STATUS_DETECTOR_BUFFER_ERR
#define MIKROE_XM125_STATUS_SENSOR_BUFFER_ERR     PCR_STATUS_SENSOR_BUFFER_ERR
#define MIKROE_XM125_STATUS_CALIB_BUFFER_ERR      PCR_STATUS_CALIB_BUFFER_ERR
#define MIKROE_XM125_STATUS_CFG_APPLY_ERR         PCR_STATUS_CFG_APPLY_ERR
#define MIKROE_XM125_STATUS_SENSOR_CALIB_ERR      PCR_STATUS_SENSOR_CALIB_ERR
#define MIKROE_XM125_STATUS_DETECTOR_CALIB_ERR    PCR_STATUS_DETECTOR_CALIB_ERR
#define MIKROE_XM125_STATUS_DETECTOR_ERR          PCR_STATUS_DETECTOR_ERR
#define MIKROE_XM125_STATUS_BUSY                  PCR_STATUS_BUSY

/**
 * @brief PCR commands setting.
 * @details Specified commands of PCR Click driver.
 */
#define MIKROE_XM125_APPLY_CFG_AND_CALIB_CMD      PCR_APPLY_CFG_AND_CALIB_CMD
#define MIKROE_XM125_MEASURE_DISTANCE_CMD         PCR_MEASURE_DISTANCE_CMD
#define MIKROE_XM125_APPLY_CONFIGURATION_CMD      PCR_APPLY_CONFIGURATION_CMD
#define MIKROE_XM125_CALIBRATE_CMD                PCR_CALIBRATE_CMD
#define MIKROE_XM125_RECALIBRATE_CMD              PCR_RECALIBRATE_CMD
#define MIKROE_XM125_ENABLE_UART_LOGS_CMD         PCR_ENABLE_UART_LOGS_CMD
#define MIKROE_XM125_DISABLE_UART_LOGS_CMD        PCR_DISABLE_UART_LOGS_CMD
#define MIKROE_XM125_LOG_CONFIGURATION_CMD        PCR_LOG_CONFIGURATION_CMD
#define MIKROE_XM125_RESET_MODULE_CMD             PCR_RESET_MODULE_CMD

/**
 * @brief PCR distance setting.
 * @details Specified minimum and maximum distance setting of PCR Click driver.
 */
#define MIKROE_XM125_MIN_DISTANCE                 PCR_MIN_DISTANCE
#define MIKROE_XM125_MAX_DISTANCE                 PCR_MAX_DISTANCE

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] instance
 *    I2C instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_xm125_init(mikroe_i2c_handle_t instance);

/***************************************************************************//**
 * @brief
 *    This function sets the IC2SPM instance used by platform functions.
 *
 * @param[in] instance
 *    I2C instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_xm125_set_i2csmp_instance(mikroe_i2c_handle_t instance);

/***************************************************************************//**
 * @brief
 *    This function executes a default configuration of PCR click board.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *
 ******************************************************************************/
sl_status_t mikroe_xm125_default_cfg(void);

/***************************************************************************//**
 * @brief
 *    This function writes a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 *
 * @param[in] reg : Start register address.
 * @param[in] data_in : Data to be written.
 * @param[in] len : Number of bytes to be written.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if data_in is null
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_xm125_generic_write(uint8_t reg, uint8_t *data_in,
                                       uint8_t len);

/***************************************************************************//**
 * @brief
 *    This function reads a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 *
 * @param[in] reg : Start register address.
 * @param[out] data_out : Output read data
 * @param[in] len : Number of bytes to be read.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if data_out is null
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_xm125_generic_read (uint8_t reg, uint8_t *data_out,
                                       uint8_t len);

/***************************************************************************//**
 * @brief
 *    This function is used to set wake up pin state of PCR click board.
 *
 * @param[in] pin_state : State of the wake up pin.
 *
 *
 ******************************************************************************/
void mikroe_xm125_set_wup_pin (bool wup_pin_state);

/***************************************************************************//**
 * @brief
 *    This function is used to set reset pin state of PCR click board.
 *
 * @param[in] pin_state : State of the reset pin.
 *
 *
 ******************************************************************************/
void mikroe_xm125_set_rst_pin(bool rst_pin_state);

/***************************************************************************//**
 * @brief
 * This function is used to read interrupt pin state of PCR click board.
 *
 * @return
 *    State of the interrupt pin.
 *
 ******************************************************************************/
uint8_t mikroe_xm125_get_int_pin (void);

/***************************************************************************//**
 * @brief
 * This function is used to perform hardware reset of PCR click board.
 *
 ******************************************************************************/
void mikroe_xm125_hw_reset (void);

/***************************************************************************//**
 * @brief
 *    This function writes a desired data bytes into
 * the selected register by using I2C serial interface.
 *
 * @param[in] reg : Start register address.
 * @param[in] data_in : Data to be written.
 *
 * @return
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_xm125_write_reg(uint16_t reg, uint32_t data_in);

/***************************************************************************//**
 * @brief
 *    This function reads a desired data bytes from
 * the selected register by using I2C serial interface.
 *
 * @param[in] reg : Start register address.
 * @param[out] data_out : Output read data.
 *
 * @return
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_xm125_read_reg(uint16_t reg, uint32_t *data_out);

/***************************************************************************//**
 * @brief
 *    This function is used to read status register of the PCR click board.
 * @param[out] status : Read status data.
 *
 * @return
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_xm125_get_status(uint32_t *status);

/***************************************************************************//**
 * @brief
 *    This function is used to check if the PCR click board is busy.
 * @return
 *    SL_STATUS_OK - device not busy
 *    SL_STATUS_FAIL - Device busy.
 *
 ******************************************************************************/
sl_status_t mikroe_xm125_check_device_busy(void);

/***************************************************************************//**
 * @brief
 *    This function is used to read status register, if any error flags are
 * up, function will return error.
 * @return
 *    SL_STATUS_OK
 *    SL_STATUS_FAIL
 *
 ******************************************************************************/
sl_status_t mikroe_xm125_check_status_ok(void);

/***************************************************************************//**
 * @brief
 *    This function is used to read distance from the PCR click board.
 *
 * @param[out] distance_data : Read distance data.
 *
 * @return
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_xm125_get_distance(uint32_t *distance_data);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_PCR_XM125_H_
