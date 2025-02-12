/***************************************************************************//**
 * @file mikroe_sht40.h
 * @brief mikroe_sht40.h Prototypes
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
#ifndef MIKROE_SHT40_H
#define MIKROE_SHT40_H

#include "sl_status.h"
#include "temphum15.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup mikroe_sht40 - TEMP & HUM 15 Click
 * @brief
 *  The integration of the Mikroe SDK Click extension SHT40 driver.
 *  (TEMP & HUM 15 Click board.)
 *
 *    @n @section sht40_example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 *    #include "mikroe_sht40.h"
 *    #include "sl_i2cspm_instances.h"
 *
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize sensor and set mode
 *      mikroe_sht40_init(sl_i2cspm_mikroe);
 *
 *      uint32_t sht40_serial;
 *      mikroe_sht40_read_serial(&sht40_serial);
 *
 *      mikroe_sht40_measurement_data_t sht40Data;
 *      while (true) {
 *
 *        mikroe_sht40_get_temp_and_hum(mikroe_sht40_precision_mode_high,
 *                                         &sht40Data);
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
 * @brief
 *    Specified setting for device precision mode selection of
 * Temp&Hum 15 Click driver.
 ******************************************************************************/
typedef enum {
  mikroe_sht40_precision_mode_high = TEMPHUM15_MODE_HIGH_PRECISION,
  mikroe_sht40_precision_mode_medium = TEMPHUM15_MODE_MEDIUM_PRECISION,
  mikroe_sht40_precision_mode_low = TEMPHUM15_MODE_LOW_PRECISION,
}mikroe_sht40_precision_mode_e;

/***************************************************************************//**
 * @brief
 *    Typedef for specifying the packed measurement data.
 ******************************************************************************/
typedef struct {
  float temperature; /*!< Temperature */
  float humidity; /*!< Relative humidity */
} mikroe_sht40_measurement_data_t;

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_sht40_init(mikroe_i2c_handle_t instance);

/***************************************************************************//**
 * @brief
 *    This function sets the IC2SPM instance used by platform functions.
 *
 * @param[in] instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_sht40_set_i2csmp_instance(mikroe_i2c_handle_t instance);

/***************************************************************************//**
 * @brief
 *    This function writes a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 *
 * @param[in] reg : Start register address.
 * @param[in] tx_buf : Data to be written.
 * @param[in] tx_len : Number of bytes to be written.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if tx_buf is null
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_sht40_generic_write(uint8_t reg, uint8_t *tx_buf,
                                       uint8_t tx_len);

/***************************************************************************//**
 * @brief
 *    This function reads a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 *
 * @param[in] reg : Start register address.
 * @param[out] rx_buf : Output read data
 * @param[in] rx_len : Number of bytes to be read.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if rx_buf is null
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_sht40_generic_read (uint8_t reg, uint8_t *rx_buf,
                                       uint8_t rx_len);

/***************************************************************************//**
 * @brief
 *    This function reads temperature and humidity with the
 * power consumption based on the precision mode selected.
 *
 * @param[in] precision_mode : Precision mode setting.
 * @param[out] measurement_data : Variable to store the temperature and
 *    relative humidity values.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if measurement_data is null or precision_mode
 *    is invalid
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_sht40_get_temp_and_hum (
  mikroe_sht40_precision_mode_e precision_mode,
  mikroe_sht40_measurement_data_t *measurement_data);

/***************************************************************************//**
 * @brief
 *    This function sends soft reset command to the sensor.
 *
 * @return
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_sht40_soft_reset(void);

/***************************************************************************//**
 * @brief
 * This function reads unique serial number of the sensor.
 *
 * @param[out] serial_number : Output serial number.
 * @return
 *    SL_STATUS_INVALID_PARAMETER if serial_number is null
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_sht40_read_serial (uint32_t *serial_number);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_SHT40_H
