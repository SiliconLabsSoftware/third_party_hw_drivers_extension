/***************************************************************************//**
 * @file mirkroe_ad8318.h
 * @brief SCL AD8318 Prototypes
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

#ifndef MIKROE_AD8318_H_
#define MIKROE_AD8318_H_

#include "sl_status.h"
#include "rfmeter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup calc_const Calculation constants
 * \{
 */
#define MIKROE_AD8318_FILTER_USEFULL_DATA  0x1FFE
#define MIKROE_AD8318_ADC_RESOLUTION       4096
#define MIKROE_AD8318_DATA_SAMPLE_NUM      10
#define MIKROE_AD8318_DEF_VREF             2.5
#define MIKROE_AD8318_DEF_SLOPE            -0.025
#define MIKROE_AD8318_DEF_INTERCEPT        20.0
#define MIKROE_AD8318_DEF_LIMIT_HIGH       2.0
#define MIKROE_AD8318_DEF_LIMIT_LOW        0.5

/** \} */

/**
 * @brief Config Object Initialization function.
 *
 * @description This function initializes click configuration structure to init
 *   state.
 * @note All used pins will be set to unconnected state.
 */
void mikroe_ad8318_cfg_setup(void);

/**
 * @brief Initialization function.
 * @param spi_instance SPI handle instance.
 *
 * @description This function initializes all necessary pins and peripherals
 *   used for this click.
 */
sl_status_t mikroe_ad8318_set_spi_instance(mikroe_spi_handle_t spi_instance);

/**
 * @brief Initialization function.
 * @param spi_instance SPI handle instance.
 *
 * @description This function initializes all necessary pins and peripherals
 *   used for this click.
 */
sl_status_t mikroe_ad8318_init(mikroe_spi_handle_t spi_instance);

/**
 * @brief Data read function
 *
 * @param read_data  Data readed.
 *
 * @returns status of function.
 *
 * Function is used to read data from output register.
 **/
sl_status_t mikroe_ad8318_read_data(uint16_t *read_data);

/**
 * @brief Get raw data function
 *
 * @param raw_data  Raw data.
 *
 * @returns status of function.
 *
 * Function is used to read raw data value from output register.
 **/
sl_status_t mikroe_ad8318_get_raw_data(uint16_t *raw_data);

/**
 * @brief Get voltage function
 *
 * @param voltage  Voltage.
 *
 * @returns status of function.
 *
 * Function is used to read raw voltage value from output register.
 **/
sl_status_t mikroe_ad8318_get_voltage(float *voltage);

/**
 * @brief Get signal strenght function
 *
 * @param slope float value that represents slope
 * @param intercept float value that represents intercept
 * @param signal_strength  Signal strenght.
 * @returns status of function.
 *
 * Function is used to calculate radio frequency signal strenght in a vicinity.
 **/
sl_status_t mikroe_ad8318_get_signal_strength(float slope,
                                              float intercept,
                                              float *signal_strength);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_AD8318_H_ */
