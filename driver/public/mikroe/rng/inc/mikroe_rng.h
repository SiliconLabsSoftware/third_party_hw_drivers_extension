/***************************************************************************//**
 * @file mikroe_rng.h
 * @brief Mikroe RNG Prototypes
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
#ifndef MIKROE_RNG_H_
#define MIKROE_RNG_H_

#include "sl_status.h"
#include "rng.h"
#include "drv_i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIKROE_RNG_RETVAL                  RNG_RETVAL

#define MIKROE_RNG_OK                      RNG_OK
#define MIKROE_RNG_INIT_ERROR              RNG_INIT_ERROR

#define MIKROE_RNG_SLAVE_ADDRESS_GND       RNG_DEVICE_SLAVE_ADDRESS_GND
#define MIKROE_RNG_SLAVE_ADDRESS_VDD       RNG_DEVICE_SLAVE_ADDRESS_VDD
#define MIKROE_RNG_SLAVE_ADDRESS_SDA       RNG_DEVICE_SLAVE_ADDRESS_SDA
#define MIKROE_RNG_SLAVE_ADDRESS_SCL       RNG_DEVICE_SLAVE_ADDRESS_SCL

#define MIKROE_RNG_RESET_DEVICE            RNG_RESET_DEVICE

#define MIKROE_RNG_CONVERSION_REG          RNG_CONVERSION_REG
#define MIKROE_RNG_CONFIG_REG              RNG_CONFIG_REG
#define MIKROE_RNG_LO_THRESH_REG           RNG_LO_THRESH_REG
#define MIKROE_RNG_HI_THRESH_REG           RNG_HI_THRESH_REG

#define MIKROE_RNG_REG_COMP_QUEUE_DISABLE  RNG_CONFIG_REG_COMP_QUEUE_DISABLE
#define MIKROE_RNG_REG_COMP_QUEUE_4_CONV   RNG_CONFIG_REG_COMP_QUEUE_4_CONV
#define MIKROE_RNG_REG_COMP_QUEUE_2_CONV   RNG_CONFIG_REG_COMP_QUEUE_2_CONV
#define MIKROE_RNG_REG_COMP_QUEUE_1_CONV   RNG_CONFIG_REG_COMP_QUEUE_1_CONV
#define MIKROE_RNG_REG_COMP_LAT_NON        RNG_CONFIG_REG_COMP_LAT_NON
#define MIKROE_RNG_REG_COMP_LAT            RNG_CONFIG_REG_COMP_LAT
#define MIKROE_RNG_REG_COMP_POL_LOW        RNG_CONFIG_REG_COMP_POL_LOW
#define MIKROE_RNG_REG_COMP_POL_HIGH       RNG_CONFIG_REG_COMP_POL_HIGH
#define MIKROE_RNG_REG_COMP_MODE_TRAD      RNG_CONFIG_REG_COMP_MODE_TRAD
#define MIKROE_RNG_REG_COMP_MODE_WINDOW    RNG_CONFIG_REG_COMP_MODE_WINDOW
#define MIKROE_RNG_REG_DATA_RATE_8_SPS     RNG_CONFIG_REG_DATA_RATE_8_SPS
#define MIKROE_RNG_REG_DATA_RATE_16_SPS    RNG_CONFIG_REG_DATA_RATE_16_SPS
#define MIKROE_RNG_REG_DATA_RATE_32_SPS    RNG_CONFIG_REG_DATA_RATE_32_SPS
#define MIKROE_RNG_REG_DATA_RATE_64_SPS    RNG_CONFIG_REG_DATA_RATE_64_SPS
#define MIKROE_RNG_REG_DATA_RATE_128_SPS   RNG_CONFIG_REG_DATA_RATE_128_SPS
#define MIKROE_RNG_REG_DATA_RATE_250_SPS   RNG_CONFIG_REG_DATA_RATE_250_SPS
#define MIKROE_RNG_REG_DATA_RATE_475_SPS   RNG_CONFIG_REG_DATA_RATE_475_SPS
#define MIKROE_RNG_REG_DATA_RATE_860_SPS   RNG_CONFIG_REG_DATA_RATE_860_SPS
#define MIKROE_RNG_REG_DEV_MODE_SINGLE     RNG_CONFIG_REG_DEV_MODE_SINGLE
#define MIKROE_RNG_REG_DEV_MODE_CONTIN     RNG_CONFIG_REG_DEV_MODE_CONTIN
#define MIKROE_RNG_REG_PGA_6144            RNG_CONFIG_REG_PGA_6144
#define MIKROE_RNG_REG_PGA_4096            RNG_CONFIG_REG_PGA_4096
#define MIKROE_RNG_REG_PGA_2048            RNG_CONFIG_REG_PGA_2048
#define MIKROE_RNG_REG_PGA_1024            RNG_CONFIG_REG_PGA_1024
#define MIKROE_RNG_REG_PGA_512             RNG_CONFIG_REG_PGA_512
#define MIKROE_RNG_REG_PGA_256_1           RNG_CONFIG_REG_PGA_256_1
#define MIKROE_RNG_REG_PGA_256_2           RNG_CONFIG_REG_PGA_256_2
#define MIKROE_RNG_REG_PGA_256_3           RNG_CONFIG_REG_PGA_256_3
#define MIKROE_RNG_REG_MUX_AINP_AINN_0_1   RNG_CONFIG_REG_MUX_AINP_AINN_0_1
#define MIKROE_RNG_REG_MUX_AINP_AINN_0_3   RNG_CONFIG_REG_MUX_AINP_AINN_0_3
#define MIKROE_RNG_REG_MUX_AINP_AINN_1_3   RNG_CONFIG_REG_MUX_AINP_AINN_1_3
#define MIKROE_RNG_REG_MUX_AINP_AINN_2_3   RNG_CONFIG_REG_MUX_AINP_AINN_2_3
#define MIKROE_RNG_REG_MUX_AINP_AINN_0_GND RNG_CONFIG_REG_MUX_AINP_AINN_0_GND
#define MIKROE_RNG_REG_MUX_AINP_AINN_1_GND RNG_CONFIG_REG_MUX_AINP_AINN_1_GND
#define MIKROE_RNG_REG_MUX_AINP_AINN_2_GND RNG_CONFIG_REG_MUX_AINP_AINN_2_GND
#define MIKROE_RNG_REG_MUX_AINP_AINN_3_GND RNG_CONFIG_REG_MUX_AINP_AINN_3_GND
#define MIKROE_RNG_REG_OS_SINGLE_CONV      RNG_CONFIG_REG_OS_SINGLE_CONV
#define MIKROE_RNG_REG_OS_NO_EFFECT        RNG_CONFIG_REG_OS_NO_EFFECT

#define MIKROE_RNG_REG_LOWTHRES_HOLD       RNG_REG_LOWTHRES_HOLD
#define MIKROE_RNG_REG_HIGH_THRESH_HOLD    RNG_REG_HIGH_THRESH_HOLD

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_rng_init(mikroe_i2c_handle_t i2cspm_instance);

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
sl_status_t mikroe_rng_set_i2csmp_instance(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function executes default configuration for RNG click.
 *
 * @return void
 *
 ******************************************************************************/
void mikroe_rng_default_cfg (void);

/***************************************************************************//**
 * @brief
 *    This function writes data to the desired register.
 *
 * @param[in] reg : Register address.
 * @param[in] reg_data : Data to be written.
 *
 * @return void
 *
 ******************************************************************************/
void mikroe_rng_generic_write(uint8_t reg, uint16_t reg_data);

/***************************************************************************//**
 * @brief
 *    This function reads data from the desired register.
 *
 * @param[in] reg : Register address.
 * @param[out] rx_buf : Output read data
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if rx_buf is null
 *    SL_STATUS_OK - Success
 ******************************************************************************/
sl_status_t mikroe_rng_generic_read (uint8_t reg, uint16_t *rx_buf);

/***************************************************************************//**
 * @brief
 *    This function resets device.
 *
 * @return void
 *
 ******************************************************************************/
void mikroe_rng_reset_device (void);

/***************************************************************************//**
 * @brief
 *    This function sets configuration.
 * @param[in] conf_data : The configuration data
 *
 * @return void
 *
 ******************************************************************************/
void mikroe_rng_set_config (uint16_t conf_data);

/***************************************************************************//**
 * @brief
 *    This function sets desired vref.
 * @param[in] vref_mv : Sets reference voltage in millivolts.
 *
 * @return void
 *
 ******************************************************************************/
void mikroe_rng_set_vref(uint16_t vref_mv);

/***************************************************************************//**
 * @brief
 *    This function gets voltage in millivolts..
 * @param[out] voltage : Variable to store the voltage in mV
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if voltage is null
 *    SL_STATUS_OK - Success
 ******************************************************************************/
sl_status_t mikroe_rng_get_voltage  (float *voltage);

/***************************************************************************//**
 * @brief
 *    Function used for set threshold values.
 * @param[in] thresh_reg  : Register adress.
 * @param[in] thresh_data : Data to be write.
 *
 * @return void
 *
 ******************************************************************************/
void mikroe_rng_set_threshold (uint8_t thresh_reg,
                               uint16_t thresh_data);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_RNG_H_
