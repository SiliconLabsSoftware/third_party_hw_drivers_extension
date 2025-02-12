/***************************************************************************//**
 * @file mikroe_dps310_i2c.h
 * @brief SCL DPS310 Prototypes
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef MIKROE_PRESSURE3_H
#define MIKROE_PRESSURE3_H

#include "sl_status.h"
#include "pressure3.h"

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------- PUBLIC MACROS

/**
 * \defgroup macros Macros
 * \{
 */

/** \} */

/**
 * \defgroup configuration_register Configuration Registers
 * \{
 */
#define MIKROE_PRESSURE3_I2C_ADDRESS                                      0x76
#define MIKROE_PRESSURE3_REG_PSR_B2                                       0x00
#define MIKROE_PRESSURE3_REG_PSR_B1                                       0x01
#define MIKROE_PRESSURE3_REG_PSR_B0                                       0x02
#define MIKROE_PRESSURE3_REG_TMP_B2                                       0x03
#define MIKROE_PRESSURE3_REG_TMP_B1                                       0x04
#define MIKROE_PRESSURE3_REG_TMP_B0                                       0x05
#define MIKROE_PRESSURE3_REG_PRS_CFG                                      0x06
#define MIKROE_PRESSURE3_REG_TMP_CFG                                      0x07
#define MIKROE_PRESSURE3_REG_MEAS_CFG                                     0x08
#define MIKROE_PRESSURE3_REG_CFG_REG                                      0x09
#define MIKROE_PRESSURE3_REG_INT_STS                                      0x0A
#define MIKROE_PRESSURE3_REG_FIFO_STS                                     0x0B
#define MIKROE_PRESSURE3_REG_RESET                                        0x0C
#define MIKROE_PRESSURE3_REG_PRODUCT_ID                                   0x0D
#define MIKROE_PRESSURE3_REG_COEF_C0_MBS                                  0x10
#define MIKROE_PRESSURE3_REG_COEF_C0_LBS_C1_MBS                           0x11
#define MIKROE_PRESSURE3_REG_COEF_C1_LBS                                  0x12
#define MIKROE_PRESSURE3_REG_COEF_C00_MBS                                 0x13
#define MIKROE_PRESSURE3_REG_COEF_C00_LSB                                 0x14
#define MIKROE_PRESSURE3_REG_COEF_C00_XLSB_C10_MSB                        0x15
#define MIKROE_PRESSURE3_REG_COEF_C10_LSB                                 0x16
#define MIKROE_PRESSURE3_REG_COEF_C10_XLSB                                0x17
#define MIKROE_PRESSURE3_REG_COEF_C01_MSB                                 0x18
#define MIKROE_PRESSURE3_REG_COEF_C01_LSB                                 0x19
#define MIKROE_PRESSURE3_REG_COEF_C11_MSB                                 0x1A
#define MIKROE_PRESSURE3_REG_COEF_C11_LSB                                 0x1B
#define MIKROE_PRESSURE3_REG_COEF_C20_MSB                                 0x1C
#define MIKROE_PRESSURE3_REG_COEF_C20_LSB                                 0x1D
#define MIKROE_PRESSURE3_REG_COEF_C21_MSB                                 0x1E
#define MIKROE_PRESSURE3_REG_COEF_C21_LSB                                 0x1F
#define MIKROE_PRESSURE3_REG_COEF_C30_MSB                                 0x20
#define MIKROE_PRESSURE3_REG_COEF_C30_LSB                                 0x21
#define MIKROE_PRESSURE3_REG_COEF_SRCE                                    0x28

/** \} */

/**
 * \defgroup pressure_configuration   Pressure Configuration
 * \{
 */
#define MIKROE_PRESSURE3_BIT_MASK_PM_RATE_1_PER_SEC                       0x00
#define MIKROE_PRESSURE3_BIT_MASK_PM_RATE_2_PER_SEC                       0x10
#define MIKROE_PRESSURE3_BIT_MASK_PM_RATE_4_PER_SEC                       0x20
#define MIKROE_PRESSURE3_BIT_MASK_PM_RATE_8_PER_SEC                       0x30
#define MIKROE_PRESSURE3_BIT_MASK_PM_RATE_16_PER_SEC                      0x40
#define MIKROE_PRESSURE3_BIT_MASK_PM_RATE_32_PER_SEC                      0x50
#define MIKROE_PRESSURE3_BIT_MASK_PM_RATE_64_PER_SEC                      0x60
#define MIKROE_PRESSURE3_BIT_MASK_PM_RATE_128_PER_SEC                     0x70
#define MIKROE_PRESSURE3_BIT_MASK_PM_PRC_1_PER_SEC                        0x00
#define MIKROE_PRESSURE3_BIT_MASK_PM_PRC_2_PER_SEC                        0x01
#define MIKROE_PRESSURE3_BIT_MASK_PM_PRC_4_PER_SEC                        0x02
#define MIKROE_PRESSURE3_BIT_MASK_PM_PRC_8_PER_SEC                        0x03
#define MIKROE_PRESSURE3_BIT_MASK_PM_PRC_16_PER_SEC                       0x04
#define MIKROE_PRESSURE3_BIT_MASK_PM_PRC_32_PER_SEC                       0x05
#define MIKROE_PRESSURE3_BIT_MASK_PM_PRC_64_PER_SEC                       0x06
#define MIKROE_PRESSURE3_BIT_MASK_PM_PRC_128_PER_SEC                      0x07

/** \} */

/**
 * \defgroup Temperature_configuration   Temperature Configuration
 * \{
 */
#define MIKROE_PRESSURE3_BIT_MASK_TMP_EXT_INTERNAL_SENSOR                 0x00
#define MIKROE_PRESSURE3_BIT_MASK_TMP_EXT_EXTERNAL_SENSOR                 0x80
#define MIKROE_PRESSURE3_BIT_MASK_TMP_RATE_1_PER_SEC                      0x00
#define MIKROE_PRESSURE3_BIT_MASK_TMP_RATE_2_PER_SEC                      0x10
#define MIKROE_PRESSURE3_BIT_MASK_TMP_RATE_4_PER_SEC                      0x20
#define MIKROE_PRESSURE3_BIT_MASK_TMP_RATE_8_PER_SEC                      0x30
#define MIKROE_PRESSURE3_BIT_MASK_TMP_RATE_16_PER_SEC                     0x40
#define MIKROE_PRESSURE3_BIT_MASK_TMP_RATE_32_PER_SEC                     0x50
#define MIKROE_PRESSURE3_BIT_MASK_TMP_RATE_64_PER_SEC                     0x60
#define MIKROE_PRESSURE3_BIT_MASK_TMP_RATE_128_PER_SEC                    0x70
#define MIKROE_PRESSURE3_BIT_MASK_TMP_PRC_1_PER_SEC                       0x00
#define MIKROE_PRESSURE3_BIT_MASK_TMP_PRC_2_PER_SEC                       0x01
#define MIKROE_PRESSURE3_BIT_MASK_TMP_PRC_4_PER_SEC                       0x02
#define MIKROE_PRESSURE3_BIT_MASK_TMP_PRC_8_PER_SEC                       0x03
#define MIKROE_PRESSURE3_BIT_MASK_TMP_PRC_16_PER_SEC                      0x04
#define MIKROE_PRESSURE3_BIT_MASK_TMP_PRC_32_PER_SEC                      0x05
#define MIKROE_PRESSURE3_BIT_MASK_TMP_PRC_64_PER_SEC                      0x06
#define MIKROE_PRESSURE3_BIT_MASK_TMP_PRC_128_PER_SEC                     0x07

/** \} */

/**
 * \defgroup senor_operating_mode_and_status    Sensor Operating Mode and Status
 * \{
 */
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_COEF_RDY_AVB                   0x80
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_COEF_RDY_NO_AVB                0x00
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_SENSOR_RDY_CMP                 0x40
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_SENSOR_RDY_NO_CMP              0x00
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_TMP_RDY                        0x20
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_PRS_RDY                        0x10
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_MEAS_CTRL_IDLE                 0x00
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_MEAS_CTRL_PRS                  0x01
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_MEAS_CTRL_TMP                  0x02
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_MEAS_CTRL_CNT_PRS              0x05
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_MEAS_CTRL_CNT_TMP              0x06
#define MIKROE_PRESSURE3_BIT_MASK_MEAS_CFG_MEAS_CTRL_CNT_PRS_TMP          0x07

/** \} */

/**
 * \defgroup interrupt_and_fifo_configuration     Interrupt and FIFO
 *   configuration
 * \{
 */
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_INT_HL_LOW                      0x00
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_INT_HL_HIGH                     0x80
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_INT_FIFO_DISABLE                0x00
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_INT_FIFO_ENABLE                 0x40
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_INT_TMP_DISABLE                 0x00
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_INT_TMP_ENABLE                  0x20
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_INT_PRS_DISABLE                 0x00
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_INT_PRS_ENABLE                  0x10
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_T_SHIFT_NO_SHIFT                0x00
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_T_SHIFT_SHIFT                   0x08
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_P_SHIFT_NO_SHIFT                0x00
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_P_SHIFT_SHIFT                   0x04
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_FIFO_EN_DISABLE                 0x00
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_FIFO_EN_ENABLE                  0x02
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_SPI_MODE_DISABLE                0x00
#define MIKROE_PRESSURE3_BIT_MASK_CFG_REG_SPI_MODE_ENABLE                 0x01

/** \} */

/**
 * \defgroup interrupt_status     Interrupt Status
 * \{
 */
#define MIKROE_PRESSURE3_BIT_MASK_INT_STS_INT_FIFO_FULL_NO_ACTIVE         0x00
#define MIKROE_PRESSURE3_BIT_MASK_INT_STS_INT_FIFO_FULL_ACTIVE            0x04
#define MIKROE_PRESSURE3_BIT_MASK_INT_STS_INT_TMP_NO_ACTIVE               0x00
#define MIKROE_PRESSURE3_BIT_MASK_INT_STS_INT_TMP_ACTIVE                  0x02
#define MIKROE_PRESSURE3_BIT_MASK_INT_STS_INT_PRS_NO_ACTIVE               0x00
#define MIKROE_PRESSURE3_BIT_MASK_INT_STS_INT_PRS_ACTIVE                  0x01

/** \} */

/**
 * \defgroup fifo_status     FIFO Status
 * \{
 */
#define MIKROE_PRESSURE3_BIT_MASK_FIFO_STS_FIFO_FULL_NO                   0x00
#define MIKROE_PRESSURE3_BIT_MASK_FIFO_STS_FIFO_FULL_FULL                 0x02
#define MIKROE_PRESSURE3_BIT_MASK_FIFO_STS_FIFO_EMPTY_NO                  0x00
#define MIKROE_PRESSURE3_BIT_MASK_FIFO_STS_FIFO_EMPTY_EMPTY               0x01

/** \} */

/**
 * \defgroup coefficients_source     Coefficients Source
 * \{
 */
#define MIKROE_PRESSURE3_BIT_MASK_TMP_COEF_SRCE_INTERNAL_SENSOR           0x00
#define MIKROE_PRESSURE3_BIT_MASK_TMP_COEF_SRCE_EXTERNAL_SENSOR           0x80

/** \} */

/** \} */ // End group macro
// --------------------------------------------------------------- PUBLIC TYPES

/**
 * \defgroup type Types
 * \{
 */

/**
 * @brief Communication type.
 */
typedef uint8_t  mikroe_pressure3_select_t;

/**
 * @brief Coefficients structure.
 */
typedef struct
{
  int16_t coeff_c0;
  int16_t coeff_c1;
  int32_t coeff_c00;
  int32_t coeff_c10;
  int16_t coeff_c01;
  int16_t coeff_c11;
  int16_t coeff_c20;
  int16_t coeff_c21;
  int16_t coeff_c30;
} mikroe_pressure3_coeff_t;

/** \} */ // End types group
// ----------------------------------------------- PUBLIC FUNCTION DECLARATIONS

/**
 * \defgroup public_function Public function
 * \{
 */

/**
 * @brief Initialization function.
 *
 * @param i2cspm_instance i2c instance.
 *
 * @description This function initializes all necessary pins and peripherals
 *   used for this click.
 */
sl_status_t mikroe_pressure3_init(mikroe_i2c_handle_t i2c_instance);

/**
 * @brief Generic write function.
 *
 * @param reg        Register address.
 * @param data_buf   Output data buf
 * @param len        Number of the bytes to be read
 *
 * @description This function writes data to the desired register.
 */
sl_status_t mikroe_pressure3_generic_write(uint8_t reg,
                                           uint8_t *data_buf,
                                           uint8_t len);

/**
 * @brief Generic read function.
 *
 * @param reg          Register address.
 * @param data_buf  Data buf to be written.
 * @param len          Number of the bytes in data buf.
 *
 * @description This function reads data from the desired register.
 */
sl_status_t mikroe_pressure3_generic_read(uint8_t reg,
                                          uint8_t *data_buf,
                                          uint8_t len);

/**
 * @brief Get temperature pressure.
 *
 * @param temperature          pointer to float temperature value in degrees
 *   Celsius [ �C ].
 * @param presure              pointer to float presure value [ mbar ].
 *
 * @description This function reads data from the desired register.
 */
sl_status_t mikroe_pressure3_get_t_p_data(float *temperature, float *pressure);

/**
 * @brief Get coefficients.
 *
 * @param coeff_struct         Structure.
 *
 * @description This function read the measurement data by read from the
 * three target register address of DPS310 sensor on Pressure 3 click board.
 */
sl_status_t mikroe_pressure3_get_coefficients(
  mikroe_pressure3_coeff_t *coeff_struct);

/**
 * @brief Read the value from the desired register.
 *
 * @param mbs_reg_addr         register address.
 *
 * @description This function reads data from the desired register.
 */
int32_t mikroe_pressure3_get_measurement_data(uint8_t mbs_reg_addr);

/**
 * @brief Set the pressure configuration function.
 *
 * @param press_config      pressConfig
 * - [ bit7 ]
 *    - Reserved.
 * - [ bit6 : bit4 ] Pressure measurement rate:
 *    - 000 : 1 measurements pr. sec.
 *    - 001 : 2 measurements pr. sec.
 *    - 010 : 4 measurements pr. sec.
 *    - 011 : 8 measurements pr. sec.
 *    - 100 : 16 measurements pr. sec.
 *    - 101 : 32 measurements pr. sec.
 *    - 110 : 64 measurements pr. sec.
 *    - 111 : 128 measurements pr. sec.
 * - [ bit3 : bit0 ] Pressure oversampling rate:
 *    - 0000 : Single. (Low Precision)
 *    - 0001 : 2 times (Low Power).
 *    - 0010 : 4 times.
 *    - 0011 : 8 times.
 *    - 0100 : 16 times (Standard).
 *    - 0101 : 32 times.
 *    - 0110 : 64 times (High Precision).
 *    - 0111 : 128 times.
 *    - 1xxx : Reserved
 *
 * @description This function set the pressure configuration by write
 *   pressConfig data to the
 * target _PRESSURE3_REG_PRS_CFG register of DPS310 sensor on Pressure 3 click
 *   board.
 */
sl_status_t mikroe_pressure3_set_pressure_configuration(uint8_t press_config);

/**
 * @brief Get the pressure configuration function.
 *
 * @description This function get the pressure configuration by read from the
 * target _PRESSURE3_REG_PRS_CFG register of DPS310 sensor on Pressure 3 click
 *   board.
 */
uint8_t mikroe_pressure3_get_pressure_configuration (void);

/**
 * @brief Set the temperature configuration function.
 *
 * @param temp_config      tempConfig
 * - [ bit7 ]
 *    - 0 : Internal sensor (in ASIC)
 *    - 1 : External sensor (in pressure sensor MEMS element)
 * - [ bit6 : bit4 ] Temperature measurement rate:
 *    - 000 : 1 measurements pr. sec.
 *    - 001 : 2 measurements pr. sec.
 *    - 010 : 4 measurements pr. sec.
 *    - 011 : 8 measurements pr. sec.
 *    - 100 : 16 measurements pr. sec.
 *    - 101 : 32 measurements pr. sec.
 *    - 110 : 64 measurements pr. sec.
 *    - 111 : 128 measurements pr. sec.
 * - [ bit3 : bit0 ] Temperature oversampling (precision):
 *    - 0000 :  single. (Default) - Measurement time 3.6 ms.
 *    - 0001 : 2 times (Low Power).
 *    - 0010 : 4 times.
 *    - 0011 : 8 times.
 *    - 0100 : 16 times (Standard).
 *    - 0101 : 32 times.
 *    - 0110 : 64 times (High Precision).
 *    - 0111 : 128 times.
 *    - 1xxx : Reserved
 *
 * @description This function set the temperature configuration by write
 *   tempConfig data to the
 * target _PRESSURE3_REG_TMP_CFG register of DPS310 sensor on Pressure 3 click
 *   board.
 */
sl_status_t mikroe_pressure3_set_temperature_configuration(uint8_t temp_config);

/**
 * @brief Get the temperature configuration function.
 *
 * @description This function get the temperature configuration by write
 *   tempConfig data to the
 * target _PRESSURE3_REG_TMP_CFG register of DPS310 sensor on Pressure 3 click
 *   board.
 */
uint8_t mikroe_pressure3_get_temperature_configuration(void);

/**
 * @brief Get the sensor operating mode function.
 *
 * @param operation_mode      8 bits array according to user configuration.
 *
 * @description This function set the sensor operating mode by write
 *   operationMode data to the
 * target _PRESSURE3_REG_MEAS_CFG register of DPS310 sensor on Pressure 3 click
 *   board.
 */
sl_status_t mikroe_pressure3_set_operating_mode(uint8_t operation_mode);

/**
 * @brief Get the sensor operating mode function.
 *
 * @description This function set the sensor operating mode by write
 *   operationMode data to the
 * target _PRESSURE3_REG_MEAS_CFG register of DPS310 sensor on Pressure 3 click
 *   board.
 */
uint8_t mikroe_pressure3_get_operating_mode(void);

/**
 * @brief Set the configuration function.
 *
 * @param confg_data      8 bits array according to user configuration.
 *
 * @description This function set the configuration by write confgData data to
 *   the
 * target _PRESSURE3_REG_CFG_REG register of DPS310 sensor on Pressure 3 click
 *   board.
 */
sl_status_t mikroe_pressure3_set_configuration(uint8_t confg_data);

/**
 * @brief Get the configuration function.
 *
 * @description This function get the configuration read the byte of data from
 *   the
 * target _PRESSURE3_REG_CFG_REG register of DPS310 sensor on Pressure 3 click
 *   board.
 */
uint8_t mikroe_pressure3_get_configuration(void);

/**
 * @brief Get the interrupt status function.
 *
 * @description This function get the interrupt status by read the byte of data
 *   from the
 * target _PRESSURE3_REG_INT_STS register of DPS310 sensor on Pressure 3 click
 *   board.
 */
uint8_t mikroe_pressure3_get_interrupt_status(void);

/**
 * @brief Get the FIFO Status function.
 *
 * @description This function get the interrupt The FIFO is empty by read the
 *   byte of data from the
 * target _PRESSURE3_REG_FIFO_STS register of DPS310 sensor on Pressure 3 click
 *   board.
 */
uint8_t mikroe_pressure3_get_fifo_status(void);

/**
 * @brief Soft reset function.
 *
 * @description This function set soft reset by write 0x09 to the
 * target _PRESSURE3_REG_RESET register of DPS310 sensor on Pressure 3 click
 *   board..
 */
sl_status_t mikroe_pressure3_soft_reset(void);

#ifdef __cplusplus
}
#endif
#endif // _MIKROE_PRESSURE3_H_

/** \} */ // End public_function group
/// \}    // End click Driver group

/*! @} */
// ------------------------------------------------------------------------- END
