/***************************************************************************//**
 * @file mikroe_mq131.h
 * @brief Mikroe Ozone 2 Click Driver API
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
#ifndef MIKROE_MQ131_H
#define MIKROE_MQ131_H

#include "sl_status.h"
#include "ozone2.h"
#include "mikroe_mq131_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief Initialize Sensor.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t mikroe_ozone2_init(mikroe_spi_handle_t spi,
                               mikroe_adc_handle_t adc);

/***************************************************************************//**
 * @brief Read ADC voltage.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t mikroe_ozone2_read_signal_voltage (float *data_out);

/***************************************************************************//**
 * @brief Read ADC measurement data.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t mikroe_ozone2_read_measurement (uint16_t *data_out);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_MQ131_H
