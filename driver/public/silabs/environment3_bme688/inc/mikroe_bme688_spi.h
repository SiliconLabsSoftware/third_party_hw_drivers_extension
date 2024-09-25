/***************************************************************************//**
 * @file mikroe_bme688_spi.h
 * @brief SPI abstraction used by BME688
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
#ifndef MIKROE_BME688_SPI_H
#define MIKROE_BME688_SPI_H
#include "bme68x_defs.h"
#include "bme68x.h"
#include "drv_spi_master.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bme68x_conf bme68x_conf_t;
typedef struct bme68x_data bme68x_data_t;
typedef struct bme68x_heatr_conf bme68x_heatr_conf_t;
typedef struct bme68x_dev bme68x_dev_t;

/***************************************************************************//**
 * @addtogroup BME688 - GAS Sensor
 * @brief Driver for the Bosch Sensortec BME688
 *
 *
 *   @n @section bme688_example BME688 example
 *
 *     Basic example for performing gas measurement: @n @n
 *      @code{.c}
 *
 *   #include "sl_i2cspm_instances.h"
 *   #include "mikroe_bme688_spi.h"
 *   #include "bme68x.h"
 *
 *   int main( void )
 *   {
 *
 *     ...
 *
 *     bme68x_conf_t conf;
 *     bme68x_heatr_conf_t heatr_conf;
 *     uint32_t del_period;
 *     static bme68x_dev_t bme688;
 *     static bme68x_i2c_t bme688_spi;
 *     bme68x_data_t data;
 *
 *     // Initialize an SPI interface for BME688
 *     bme688_spi.handle = sl_spi_mikroe_handle;
 *     bme688.intf_ptr = &bme688_spi;
 *     bme68x_spi_init(&bme688);
 *
 *     bme68x_init(&bme688);
 *
 *     conf.filter = BME68X_FILTER_OFF;
 *     conf.odr = BME68X_ODR_NONE;
 *     conf.os_hum = BME68X_OS_16X;
 *     conf.os_pres = BME68X_OS_1X;
 *     conf.os_temp = BME68X_OS_2X;
 *     bme68x_set_conf(&conf, &bme688);
 *
 *     heatr_conf.enable = BME68X_ENABLE;
 *     heatr_conf.heatr_temp = 300;
 *     heatr_conf.heatr_dur = 100;
 *     bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme688);
 *
 *     // Trigger a measurement
 *     bme68x_set_op_mode(BME68X_FORCED_MODE, &bme688);
 *     del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme688)
 *                  + (heatr_conf.heatr_dur * 1000);
 *     // Wait for the measurement to complete
 *     bme688.delay_us(del_period, bme688.intf_ptr);
 *     bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme688);
 *
 *     ...
 *
 *   } @endverbatim
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *  Initialize an SPI Interface for the BME688.
 *  As entry point, call this API before using other APIs.
 *
 * @param[in, out] bme688
 *  Structure instance of bme688_dev_t
 *
 * @return
 *  @ref BME68X_OK on success.
 *  @ref On failure, BME68X_E_NULL_PTR is returned.
 ******************************************************************************/
int8_t mikroe_bme688_spi_init(bme68x_dev_t *bme688,
                              mikroe_spi_handle_t spi_handle);

/** @} (end addtogroup BME688) */

#ifdef __cplusplus
}
#endif

#endif // End of MIKROE_BME688_SPI_H
