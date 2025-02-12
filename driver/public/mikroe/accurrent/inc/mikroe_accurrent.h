/***************************************************************************//**
 * @file mikroe_accurrent.h
 * @brief SCL ACCURRENT Prototypes
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

#ifndef MIKROE_ACCURRENT_H_
#define MIKROE_ACCURRENT_H_

#include "sl_status.h"
#include "accurrent.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Config Macros
 */

#define MIKROE_ACCURRENT_ADC_ITERATIONS            10
#define MIKROE_ACCURRENT_ADC_RESOLUTION            0x0FFF
#define MIKROE_ACCURRENT_AC_TO_DC_V_SCALE          1.8f
#define MIKROE_ACCURRENT_SENSOR_RESOLUTION         30.0f
#define MIKROE_ACCURRENT_A_TO_MA_SCALE             1000.0f

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] spi_instance
 *    SPI instance handle
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_accurrent_init(mikroe_spi_handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the SPI instance used by platform functions.
 *
 * @param[in] spi_instance
 *    SPI instance handle
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if spi_instance is null.
 ******************************************************************************/
sl_status_t mikroe_accurrent_set_spi_instance(mikroe_spi_handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    This function use the SPI to perform generic transfer.
 *
 * @param[in] wr_buf    Write data buffer
 * @param[in] wr_len    Number of byte in write data buffer
 * @param[in] rd_buf    Read data buffer
 * @param[in] rd_len    Number of byte in read data buffer
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_accurrent_generic_transfer(uint8_t *wr_buf, uint16_t wr_len,
                                       uint8_t *rd_buf, uint16_t rd_len);

/***************************************************************************//**
 * @brief
 *    Gets current value of AC Current in A.
 *
 * @return
 *    Returns float value of current in A.
 ******************************************************************************/
float mikroe_accurrent_get_a(void);

/***************************************************************************//**
 * @brief
 *    Gets current value of AC Current in mA.
 *
 * @return
 *    Returns float value of current in mA.
 ******************************************************************************/
float mikroe_accurrent_get_ma(void);

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* MIKROE_MAX30101_H_ */
