/***************************************************************************//**
 * @file mikroe_mlx90672.h
 * @brief SCL MLX90672 Prototypes
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

#ifndef MIKROE_MLX90632_H_
#define MIKROE_MLX90632_H_

#include "sl_status.h"
#include "drv_i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup mikroe_MLX90632 - IrThermo 3 Click
 * @brief  The integration of the Mikroe SDK Click extension MLX90632 driver.
 *         (IrThermo 3 Click board.)
 *
 *    @n @section mlx90632_example MLX90632 example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "mikroe_mlx90632.h"
 * #include "sl_i2cspm_instances.h"
 * #include "app_log.h"
 * #include "sl_sleeptimer.h"
 *    int main(void)
 *    {
 *      ...
 *
 *    if (mikroe_mlx90632_init(sl_i2cspm_mikroe) == SL_STATUS_OK) {
 *        app_log("IrThermo 3 Click initializes successfully\n");
 *    }
 *
 *  ` mikroe_mlx90632_default_config();
 *
 *    while (true) {
 *
 *      app_log("ambient temp value: %.2f\n",
 *   mikroe_mlx90632_get_ambient_temperature());
 *      app_log("object temp value: %.2f\n",
 *   mikroe_mlx90632_get_object_temperature());
 *      sl_sleeptimer_delay_millisecond(2000);
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

// -------------------------------------------------------------- PUBLIC MACROS

/**
 * \defgroup macros Macros
 * \{
 */

/**
 * \defgroup device_slave Device slave address
 * \{
 */
#define MIKROE_IRTHERMO3_DEV_ADR_GND                   0x3A
#define MIKROE_IRTHERMO3_DEV_ADR_VCC                   0x3B

/** \} */

/**
 * \defgroup eeprom_ver EEPROM version
 * \{
 */
#define MIKROE_IRTHERMO3_EEPROM_VERSION                0x240B

/** \} */

/**
 * \defgroup eeprom_constants EEPROM constants
 * \{
 */
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_P_R_LSW      0x240C
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_P_R_MSW      0x240D
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_P_G_LSW      0x240E
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_P_G_MSW      0x240F
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_P_T_LSW      0x2410
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_P_T_MSW      0x2411
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_P_O_LSW      0x2412
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_P_O_MSW      0x2413
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_AA_LSW       0x2414
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_AA_MSW       0x2415
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_AB_LSW       0x2416
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_AB_MSW       0x2417
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_BA_LSW       0x2418
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_BA_MSW       0x2419
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_BB_LSW       0x241A
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_BB_MSW       0x241B
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_CA_LSW       0x241C
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_CA_MSW       0x241D
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_CB_LSW       0x241E
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_CB_MSW       0x241F
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_DA_LSW       0x2420
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_DA_MSW       0x2421
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_DB_LSW       0x2422
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_DB_MSW       0x2423
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_EA_LSW       0x2424
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_EA_MSW       0x2425
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_EB_LSW       0x2426
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_EB_MSW       0x2427
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_FA_LSW       0x2428
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_FA_MSW       0x2429
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_FB_LSW       0x242A
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_FB_MSW       0x242B
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_GA_LSW       0x242C
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_GA_MSW       0x242D
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_GB           0x242E
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_KA           0x242E
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_KB           0x242E
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_HA_COSTUMER  0x2481
#define MIKROE_IRTHERMO3_EEPROM_CAL_CONST_HB_COSTUMER  0x2482

/** \} */

/**
 * \defgroup ctrl_status Control and status registers
 * \{
 */
#define MIKROE_IRTHERMO3_EEPROM_CTRL_REG               0x24D4
#define MIKROE_IRTHERMO3_EEPROM_I2C_ADDRESS            0x24D5
#define MIKROE_IRTHERMO3_REG_I2C_ADDRESS               0x3000
#define MIKROE_IRTHERMO3_REG_CONTROL                   0x3001
#define MIKROE_IRTHERMO3_REG_STATUS                    0x3FFF

/** \} */

/**
 * \defgroup ram_registers RAM Registers
 * \{
 */
#define MIKROE_IRTHERMO3_RAW_DATA_1                    0x4000
#define MIKROE_IRTHERMO3_RAW_DATA_2                    0x4001
#define MIKROE_IRTHERMO3_RAW_DATA_3                    0x4002
#define MIKROE_IRTHERMO3_RAW_DATA_4                    0x4003
#define MIKROE_IRTHERMO3_RAW_DATA_5                    0x4004
#define MIKROE_IRTHERMO3_RAW_DATA_6                    0x4005
#define MIKROE_IRTHERMO3_RAW_DATA_7                    0x4006
#define MIKROE_IRTHERMO3_RAW_DATA_8                    0x4007
#define MIKROE_IRTHERMO3_RAW_DATA_9                    0x4008

/** \} */

/**
 * \defgroup modes Device mode
 * \{
 */
#define MIKROE_IRTHERMO3_MODE_SLEEP                    0x02
#define MIKROE_IRTHERMO3_MODE_STEPIN                   0x04
#define MIKROE_IRTHERMO3_MODE_CONTINOUS                0x06

/** \} */

/** \} */ // End group macro

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
sl_status_t mikroe_mlx90632_init(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *  Check whether the board is present on the bus or not.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_mlx90632_present(void);

/***************************************************************************//**
 * @brief
 *    Default configuration for IrThermo 3 Click
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_mlx90632_default_config(void);

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
sl_status_t mikroe_mlx90632_set_i2csmp_instance(
  mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    Generic write function.
 *
 * @param[in] reg
 *    Register address
 *
 * @param[in] data_buf
 *    Data buffer to be written.
 *
 * @param[in] len
 *    Number of the bytes in data buffer.
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_mlx90632_generic_write(uint16_t reg,
                                   uint8_t *data_buf, uint8_t len);

/***************************************************************************//**
 * @brief
 *    Generic read function.
 *
 * @param[in] reg
 *    Register address
 *
 * @param[in] data_buf
 *    Output data buffer
 *
 * @param[in] len
 *    Number of the bytes to be read
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_mlx90632_generic_read(uint16_t reg,
                                  uint8_t *data_buf, uint8_t len);

/***************************************************************************//**
 * @brief
 *     Write u16 data function.
 *
 * @param[in] reg
 *    Register address
 *
 * @param[in] wr_data
 *    Data to be written.
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_mlx90632_write_u16(uint16_t reg,
                               uint16_t wr_data);

/***************************************************************************//**
 * @brief
 *    Read i16 data function.
 *
 * @param[in] reg
 *    Register address.
 *
 * @return
 *    Two bytes signed data which is read from the address.
 ******************************************************************************/
int16_t mikroe_mlx90632_read_i16(uint16_t reg);

/***************************************************************************//**
 * @brief
 *    Read i32 dat function.
 *
 * @param[in] reg
 *    Register address.
 *
 * @return
 *    Four bytes signed data which is read from the address.
 ******************************************************************************/
int32_t mikroe_mlx90632_read_i32(uint16_t reg);

/***************************************************************************//**
 * @brief
 *    Global write function.
 *
 * @param[in] reg
 *    Register address.
 *
 * @return
 *    Four bytes signed data which is read from the address.
 *
 * @note This function is used exclusively to perform Addressed reset
 *       ( which resets the device ) or EEPROM unlock (for customer access)
 *        on slave address defined device
 ******************************************************************************/
void mikroe_mlx90632_global_write(uint8_t *data_buf, uint8_t len);

/***************************************************************************//**
 * @brief
 *    Set device mode function
 *
 * @param[in] mode
 *    Mode to be set.
 *
 * @return
 *    Four bytes signed data which is read from the address.
 *
 * @description
 *    This function is used to set the devices mode to sleep, step mode or
 *    continuous mode.
 ******************************************************************************/
void mikroe_mlx90632_set_mode(uint8_t mode);

/***************************************************************************//**
 * @brief
 *    Calibration function
 *
 * @return
 *    None
 *
 * @description
 *    Function is used to calibrate the device.
 *
 * @note
 *    This function must be used at least once in order to get correct readings!
 ******************************************************************************/
void mikroe_mlx90632_cal(void);

/***************************************************************************//**
 * @brief
 *    Read Ambient Temperature function
 *
 * @return
 *    Ambient temperature.
 ******************************************************************************/
float mikroe_mlx90632_get_ambient_temperature(void);

/***************************************************************************//**
 * @brief
 *    Read Object Temperature function
 *
 * @return
 *    Object temperature.
 ******************************************************************************/
float mikroe_mlx90632_get_object_temperature(void);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_MLX90632_H_
