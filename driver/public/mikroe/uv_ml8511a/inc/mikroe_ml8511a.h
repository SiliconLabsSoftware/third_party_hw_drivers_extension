/***************************************************************************//**
 * @file mikroe_ml8511a.h
 * @brief SCL ML8511A Prototypes
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
#ifndef MIKROE_ML8511A_H_
#define MIKROE_ML8511A_H_

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include "uv.h"
#include "sl_status.h"
#include "drv_analog_in.h"
#include "drv_spi_master.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
//                       Public Functions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_init(mikroe_spi_handle_t spi_instance,
                                mikroe_adc_handle_t adc_instance);

/***************************************************************************//**
 * @brief
 *    Callback Handler Setting function.
 *
 * @param[in] handler
 *    Callback handler function.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_set_callback_handler(uv_callback_t handler);

/***************************************************************************//**
 * @brief
 *    ADC Reading function.
 *
 * @param[out] data_out
 *    AD conversion result [12-bit].
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_read_adc_value(uint16_t *data_out);

/***************************************************************************//**
 * @brief
 *    ADC Voltage Reading function.
 *
 * @param[out] data_out
 *    AD voltage value [V].
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_read_adc_voltage(float *data_out);

/***************************************************************************//**
 * @brief
 *    AN Pin Reading function.
 *
 * @param[out] data_out
 *    AD conversion result [10-bit].
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_read_an_pin_value(uint16_t *data_out);

/***************************************************************************//**
 * @brief
 *    AN Pin Voltage Reading function.
 *
 * @param[out] data_out
 *    AN pin voltage value [V].
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_read_an_pin_voltage(float *data_out);

/***************************************************************************//**
 * @brief
 *    UV Index Calculation function.
 *
 * @param[in] data_in
 *    ADC voltage value for conversion from ADC voltage to UV
 *    index level.
 * @param[out] data_out
 *    UV index level.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_calc_index(float data_in, uint8_t *data_out);

/***************************************************************************//**
 * @brief
 *    Device Power ON function.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_device_enable(void);

/***************************************************************************//**
 * @brief
 *    Device Power OFF function.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_device_disable(void);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_E_PAPER_H_ */
