/***************************************************************************//**
 * @file mikroe_shtc3.h
 * @brief SCL SHTC3 Prototypes
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

#ifndef MIKROE_SHTC3_H_
#define MIKROE_SHTC3_H_

#include "sl_status.h"
#include "temphum9.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup mikroe_shtc3 SHTC3 - TEMP & HUM 9 Click
 * @brief
 *  The integration of the Mikroe SDK Click extension SHTC3 driver.
 *  (TEMP & HUM 9 Click board.)
 *
 *    @n @section shtc3_example shtc3 example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "sl_i2cspm_instances.h"
 * #include "mikroe_shtc3.h"
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize sensor and set mode
 *      mikroe_shtc3_init(sl_i2cspm_mikroe);
 *      mikroe_shtc3_send_command(MIKROE_SHTC3_CMD_SLEEP);
 *      mikroe_shtc3_send_command(MIKROE_SHTC3_CMD_WAKEUP);
 *
 *      mikroe_shtc3_measurement_data_t measurement_data;
 *      while (true) {
 *
 *        mikroe_shtc3_get_temperature_and_humidity(SHTC3_DATA_MODE_NORMAL,
 *                                                  &measurement_data);
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
 * @addtogroup  shtc3_commands Commands
 * @brief SHTC3 Commands
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *    Go to sleep mode
 ******************************************************************************/
#define  MIKROE_SHTC3_CMD_SLEEP                0xB098

/***************************************************************************//**
 * @brief
 *    Wake up device
 ******************************************************************************/
#define  MIKROE_SHTC3_CMD_WAKEUP               0x3517

/***************************************************************************//**
 * @brief
 *    Perform soft reset
 ******************************************************************************/
#define  MIKROE_SHTC3_CMD_SOFT_RESET           0x805D

/***************************************************************************//**
 * @brief
 *    Perform reset
 ******************************************************************************/
#define  MIKROE_SHTC3_CMD_GENERAL_CALL_RESET   0x0006

/***************************************************************************//**
 * @brief
 *    Read device ID.
 ******************************************************************************/
#define  MIKROE_SHTC3_CMD_READ_ID              0xEFC8

/** @} (end addtogroup shtc3_commands) */

/***************************************************************************//**
 * @addtogroup shtc3_typedefs Types
 * @brief SHTC3 Types
 * @{
 *******************************************************************************/

/***************************************************************************//**
 * @brief
 *    Typedef for specifying the supported data measurement modes.
 ******************************************************************************/
typedef enum {
  SHTC3_DATA_MODE_NORMAL, /*!< Normal mode */
  SHTC3_DATA_MODE_LOW_POWER /*!< Low power mode */
} mikroe_shtc3_data_mode_t;

/***************************************************************************//**
 * @brief
 *    Typedef for specifying the supported register access modes.
 ******************************************************************************/
typedef enum {
  SHTC3_REGISTER_ACCESS_SINGLE_DATA, /*!< Single data access mode */
  SHTC3_REGISTER_ACCESS_DUAL_DATA /*!< Dual data access mode */
} mikroe_shtc3_register_access_mode_t;

/***************************************************************************//**
 * @brief
 *    Typedef for specifying the packed measurement data.
 ******************************************************************************/
typedef struct {
  float temperature; /*!< Temperature */
  float humidity; /*!< Relative humidity */
} mikroe_shtc3_measurement_data_t;

/** @} (end addtogroup shtc3_typedefs) */

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_shtc3_init(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the IC2SPM instance used by platform functions.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_shtc3_set_i2csmp_instance(
  mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    Read register content.
 *
 * @param[in] register_address
 *    Register address.
 *
 * @param[in] n_data
 *    Register access mode. (Single data or dual data.)
 *
 * @param[out] register_buffer
 *    Array to store read data.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if register_buffer is null
 *    or n_data is invalid.
 ******************************************************************************/
sl_status_t mikroe_shtc3_read_register(uint16_t register_address,
                                       mikroe_shtc3_register_access_mode_t n_data,
                                       uint16_t *register_buffer);

/***************************************************************************//**
 * @brief
 *    Send a command to the device.
 *
 * @param[in] command
 *    Command code, see command defines for details.
 *
 ******************************************************************************/
void mikroe_shtc3_send_command(uint16_t command);

/***************************************************************************//**
 * @brief
 *    Get temperature.
 *
 * @param[in] data_mode
 *    Mode to be used to measure the temperature. (Normal or low power mode.)
 *
 * @param[out] temperature
 *    Variable to store the temperature value.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if temperature is null
 *    or data_mode is invalid.
 ******************************************************************************/
sl_status_t mikroe_shtc3_get_temperature(mikroe_shtc3_data_mode_t data_mode,
                                         float *temperature);

/***************************************************************************//**
 * @brief
 *    Get relative humidity.
 *
 * @param[in] data_mode
 *    Mode to be used to measure the temperature. (Normal or low power mode.)
 *
 * @param[out] humidity
 *    Variable to store the relative humidity value.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if temperature is null
 *    or data_mode is invalid.
 ******************************************************************************/
sl_status_t mikroe_shtc3_get_humidity(mikroe_shtc3_data_mode_t data_mode,
                                      float *humidity);

/***************************************************************************//**
 * @brief
 *    Get temperature and relative humidity.
 *
 * @param[in] data_mode
 *    Mode to be used to measure the temperature. (Normal or low power mode.)
 *
 * @param[out] measurement_data
 *    Variable to store the temperature and relative humidity values.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if measurement_data is null
 *    or data_mode is invalid.
 ******************************************************************************/
sl_status_t mikroe_shtc3_get_temperature_and_humidity(
  mikroe_shtc3_data_mode_t data_mode,
  mikroe_shtc3_measurement_data_t *measurement_data);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup mikroe_shtc3) */

#endif // MIKROE_SHTC3_H_
