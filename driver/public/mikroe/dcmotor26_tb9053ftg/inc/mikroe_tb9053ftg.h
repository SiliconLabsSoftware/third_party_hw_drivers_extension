/***************************************************************************//**
 * @file mikroe_tb9053ftg.h
 * @brief SCL TB9053FTG Prototypes
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef _MIKROE_TB9053FTG_H_
#define _MIKROE_TB9053FTG_H_

#include "sl_status.h"
#include "drv_analog_in.h"
#include "drv_i2c_master.h"
#include "drv_spi_master.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIKROE_TB9053FTG_DUMMY                         0x00
#define MIKROE_TB9053FTG_SPI_READ_WRITE_BIT            0x80
#define MIKROE_TB9053FTG_ERROR_FLAG_BIT                0x80

/**
 * @addtogroup dcmotor26_reg
 * @{
 */

/**
 * @brief DC Motor 26 registers list.
 * @details Specified registers list of DC Motor 26 Click driver.
 */
#define MIKROE_TB9053FTG_REG_STATUS1                   0x01
#define MIKROE_TB9053FTG_REG_STATUS2                   0x02
#define MIKROE_TB9053FTG_REG_STATUS3                   0x03
#define MIKROE_TB9053FTG_REG_CONFIG1                   0x04
#define MIKROE_TB9053FTG_REG_CONFIG2                   0x05
#define MIKROE_TB9053FTG_REG_CONFIG3                   0x06
#define MIKROE_TB9053FTG_REG_CONFIG4                   0x07
#define MIKROE_TB9053FTG_REG_CONFIG5                   0x08
#define MIKROE_TB9053FTG_REG_CONFIG6                   0x09
#define MIKROE_TB9053FTG_REG_CONFIG7                   0x0A

/**
 * @brief DC Motor 26 PCA9538A registers.
 * @details Specified PCA9538A registers of DC Motor 26 Click driver.
 */
#define MIKROE_TB9053FTG_REG_INPUT                     0x00
#define MIKROE_TB9053FTG_REG_OUTPUT                    0x01
#define MIKROE_TB9053FTG_REG_POLARITY                  0x02
#define MIKROE_TB9053FTG_REG_CONFIG                    0x03

/*! @} */ // dcmotor26_reg

/**
 * @defgroup dcmotor26_set DC Motor 26 Registers Settings
 * @brief Settings for registers of DC Motor 26 Click driver.
 */

/**
 * @addtogroup dcmotor26_set
 * @{
 */

/**
 * @brief DC Motor 26 CONFIG1/2 register settings.
 * @details Specified CONFIG1/2 register settings for ADC of DC Motor 26 Click
 *   driver.
 */
#define MIKROE_TB9053FTG_CONFIG12_OPEN_LOAD_NOP_100US  0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_OPEN_LOAD_NOP_50US   0x002000ul
#define MIKROE_TB9053FTG_CONFIG12_OPEN_LOAD_NOP_500US  0x004000ul
#define MIKROE_TB9053FTG_CONFIG12_OPEN_LOAD_NOP_MASK   0x006000ul
#define MIKROE_TB9053FTG_CONFIG12_DUTY_OFF_LOW_SIDE    0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_DUTY_OFF_HIGH_SIDE   0x001000ul
#define MIKROE_TB9053FTG_CONFIG12_DUTY_OFF_MASK        0x001000ul
#define MIKROE_TB9053FTG_CONFIG12_VBAT_UVC_0US         0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_VBAT_UVC_100US       0x000800ul
#define MIKROE_TB9053FTG_CONFIG12_VBAT_UVC_MASK        0x000800ul
#define MIKROE_TB9053FTG_CONFIG12_CLIMIT_UP_6p5A       0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_CLIMIT_UP_4p6A       0x000400ul
#define MIKROE_TB9053FTG_CONFIG12_CLIMIT_UP_MASK       0x000400ul
#define MIKROE_TB9053FTG_CONFIG12_CLIMIT_LOW_0p25A     0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_CLIMIT_LOW_0p5A      0x000200ul
#define MIKROE_TB9053FTG_CONFIG12_CLIMIT_LOW_MASK      0x000200ul
#define MIKROE_TB9053FTG_CONFIG12_CLIMIT_LOW_0p25A     0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_CLIMIT_LOW_0p5A      0x000200ul
#define MIKROE_TB9053FTG_CONFIG12_OPEN_LOAD_OP_200mA   0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_OPEN_LOAD_OP_100mA   0x000080ul
#define MIKROE_TB9053FTG_CONFIG12_OPEN_LOAD_OP_300mA   0x000100ul
#define MIKROE_TB9053FTG_CONFIG12_OPEN_LOAD_OP_MASK    0x000180ul
#define MIKROE_TB9053FTG_CONFIG12_SR_MODE_NORMAL       0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_SR_MODE_SLOW1        0x000010ul
#define MIKROE_TB9053FTG_CONFIG12_SR_MODE_SLOW2        0x000020ul
#define MIKROE_TB9053FTG_CONFIG12_SR_MODE_SLOW3        0x000030ul
#define MIKROE_TB9053FTG_CONFIG12_SR_MODE_SLOW4        0x000040ul
#define MIKROE_TB9053FTG_CONFIG12_SR_MODE_FAST2        0x000050ul
#define MIKROE_TB9053FTG_CONFIG12_SR_MODE_FAST1        0x000060ul
#define MIKROE_TB9053FTG_CONFIG12_SR_MODE_MASK         0x000070ul
#define MIKROE_TB9053FTG_CONFIG12_OC_LATCH             0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_OC_AUTO              0x000008ul
#define MIKROE_TB9053FTG_CONFIG12_OC_MASK              0x000008ul
#define MIKROE_TB9053FTG_CONFIG12_OC_LATCH             0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_OC_AUTO              0x000008ul
#define MIKROE_TB9053FTG_CONFIG12_SPI_DISR_10mS        0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_SPI_DISR_100mS       0x000002ul
#define MIKROE_TB9053FTG_CONFIG12_SPI_DISR_1S          0x000004ul
#define MIKROE_TB9053FTG_CONFIG12_SPI_DISR_DISABLE     0x000006ul
#define MIKROE_TB9053FTG_CONFIG12_SPI_DISR_MASK        0x000006ul
#define MIKROE_TB9053FTG_CONFIG12_OPMODE_DISR_OFF      0x000000ul
#define MIKROE_TB9053FTG_CONFIG12_OPMODE_DISR_CONTINUE 0x000001ul
#define MIKROE_TB9053FTG_CONFIG12_OPMODE_DISR_MASK     0x000001ul

/**
 * @brief DC Motor 26 CONFIG5/6 register settings.
 * @details Specified CONFIG5/6 register settings for ADC of DC Motor 26 Click
 *   driver.
 */
#define MIKROE_TB9053FTG_CONFIG56_MOTOR_OPMODE_MAX     3u
#define MIKROE_TB9053FTG_CONFIG56_MOTOR_OPMODE_SHIFT   22ul
#define MIKROE_TB9053FTG_CONFIG56_MOTOR_OPMODE_MASK    0xC00000ul
#define MIKROE_TB9053FTG_CONFIG56_DRV_FREQ_MAX         2047u
#define MIKROE_TB9053FTG_CONFIG56_DRV_FREQ_SHIFT       11ul
#define MIKROE_TB9053FTG_CONFIG56_DRV_FREQ_MASK        0x3FF800ul
#define MIKROE_TB9053FTG_CONFIG56_DUTY_PERIOD_MAX      2047u
#define MIKROE_TB9053FTG_CONFIG56_DUTY_PERIOD_SHIFT    0ul
#define MIKROE_TB9053FTG_CONFIG56_DUTY_PERIOD_MASK     0x0007FFul

/**
 * @brief DC Motor 26 PCA9538A pin mask.
 * @details Specified PCA9538A pin mask of DC Motor 26 Click driver.
 */
#define MIKROE_TB9053FTG_DIAG1_PIN                     0x01
#define MIKROE_TB9053FTG_DIAG2_PIN                     0x02
#define MIKROE_TB9053FTG_EN1_PIN                       0x04
#define MIKROE_TB9053FTG_EN2_PIN                       0x08
#define MIKROE_TB9053FTG_CM_SEL_PIN                    0x10
#define MIKROE_TB9053FTG_SLP_PIN                       0x20

/**
 * @brief DC Motor 26 PCA9538A pin direction config.
 * @details Specified PCA9538A pin direction config of DC Motor 26 Click driver.
 */
#define MIKROE_TB9053FTG_DEFAULT_CONFIG                0xE3

/**
 * @brief DC Motor 26 channel operation mode settings.
 * @details Specified channel operation mode settings for ADC of DC Motor 26
 *   Click driver.
 */
#define MIKROE_TB9053FTG_MODE_OUTPUT_OFF               0x00
#define MIKROE_TB9053FTG_MODE_FORWARD                  0x01
#define MIKROE_TB9053FTG_MODE_REVERSE                  0x02
#define MIKROE_TB9053FTG_MODE_BRAKE                    0x03

/**
 * @brief DC Motor 26 pin logic level setting.
 * @details Specified setting for pin logic level of DC Motor 26 Click driver.
 */
#define MIKROE_TB9053FTG_PIN_LOW_LEVEL                 0
#define MIKROE_TB9053FTG_PIN_HIGH_LEVEL                1

/**
 * @brief DC Motor 26 ADC setting.
 * @details Specified settings for ADC of DC Motor 26 Click driver.
 */
#define MIKROE_TB9053FTG_VREF_3V3                      3.3
#define MIKROE_TB9053FTG_VREF_5V                       5.0

/**
 * @brief DC Motor 26 current calculation values.
 * @details Specified current calculation values for ADC of DC Motor 26 Click
 *   driver.
 */
#define MIKROE_TB9053FTG_NUM_CONVERSIONS               5000
#define MIKROE_TB9053FTG_RSENSE                        220.0f
#define MIKROE_TB9053FTG_VOLTAGE_TO_CURRENT            0.00238167f

/**
 * @brief Data sample selection.
 * @details This macro sets data samples for SPI modules.
 */
#define MIKROE_TB9053FTG_SET_DATA_SAMPLE_EDGE          SET_SPI_DATA_SAMPLE_EDGE
#define MIKROE_TB9053FTG_SET_DATA_SAMPLE_MIDDLE \
  SET_SPI_DATA_SAMPLE_MIDDLE

/*! @} */ // dcmotor26_set

/**
 * @defgroup dcmotor26_map DC Motor 26 MikroBUS Map
 * @brief MikroBUS pin mapping of DC Motor 26 Click driver.
 */

/**
 * @addtogroup dcmotor26_map
 * @{
 */

/**
 * @brief DC Motor 26 initialization function.
 * @details This function initializes all necessary pins and peripherals used
 * for this click board.
 * @param[in] spi_instance : SPI instance handle.
 * @param[in] i2c_instance : I2C instance handle.
 * @param[in] adc : ADC instance handle.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_init(mikroe_spi_handle_t spi_instance,
                                  mikroe_i2c_handle_t i2c_instance,
                                  mikroe_adc_handle_t adc);

/**
 * @brief DC Motor 26 default configuration function.
 * @details This function executes a default configuration of DC Motor 26
 * click board.
 * @return SL_STATUS_OK - success, SL_STATUS_FAIL - error.
 * @note This function can consist any necessary configuration or setting to put
 * device into operating mode.
 */
sl_status_t mikroe_tb9053ftg_default_cfg(void);

/**
 * @brief DC Motor 26 write register function.
 * @details This function writes a 24-bit data to the selected register by using
 *   SPI serial interface
 * and checks if it is written correctly.
 * @param[in] reg : Register address.
 * @param[in] data_in : 24-bit data to be written.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_write_register(uint8_t reg, uint32_t data_in);

/**
 * @brief DC Motor 26 read register function.
 * @details This function reads a 24-bit data from the selected register by
 *   using SPI serial interface.
 * @param[in] reg : Register address.
 * @param[out] data_out : Output read data.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_read_register(uint8_t reg, uint8_t *data_out);

/**
 * @brief DC Motor 26 pca9538a write register function.
 * @details This function writes a desired data byte to the selected PCA9538A
 *   register
 * by using I2C serial interface.
 * @param[in] reg : Register address.
 * @param[in] data_in : Data to be written.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_pca9538a_write_register(uint8_t reg, uint8_t data_in);

/**
 * @brief DC Motor 26 pca9538a read register function.
 * @details This function reads data from the selected PCA9538A register by
 *   using I2C serial interface.
 * @param[in] reg : Register address.
 * @param[out] data_out : Output read data.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_pca9538a_read_register(uint8_t reg, uint8_t *data_out);

/**
 * @brief DC Motor 26 enter sleep mode function.
 * @details This function sets the device to sleep mode by setting the SLP pin
 *   to low logic state.
 * @return None.
 * @note None.
 */
void mikroe_tb9053ftg_enter_sleep_mode(void);

/**
 * @brief DC Motor 26 wake up device function.
 * @details This function wakes up the device from sleep by setting the SLP pin
 *   to high logic state.
 * @return None.
 * @note None.
 */
void mikroe_tb9053ftg_wake_up_device(void);

/**
 * @brief DC Motor 26 get int pin function.
 * @details This function returns the PCA9538A INT pin logic state.
 * @return Pin logic state.
 * @note None.
 */
uint8_t mikroe_tb9053ftg_get_int_pin(void);

/**
 * @brief DC Motor 26 set clk pin function.
 * @details This function sets the CLK pin logic state.
 * @param[in] state : Pin logic state.
 * @return None.
 * @note None.
 */
void mikroe_tb9053ftg_set_clk_pin(uint8_t state);

/**
 * @brief DC Motor 26 set en1 pin function.
 * @details This function sets the en1 pin logic state.
 * @param[in] state : @li @c 0 - Low logic state,
 *                    @li @c 1 - High logic state.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_set_en1_pin(uint8_t state);

/**
 * @brief DC Motor 26 get en1 pin function.
 * @details This function reads the en1 pin logic state.
 * @param[out] state : @li @c 0 - Low logic state,
 *                     @li @c 1 - High logic state.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_get_en1_pin(uint8_t *state);

/**
 * @brief DC Motor 26 set en2 pin function.
 * @details This function sets the en2 pin logic state.
 * @param[in] state : @li @c 0 - Low logic state,
 *                    @li @c 1 - High logic state.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_set_en2_pin(uint8_t state);

/**
 * @brief DC Motor 26 get en2 pin function.
 * @details This function reads the en2 pin logic state.
 * @param[out] state : @li @c 0 - Low logic state,
 *                     @li @c 1 - High logic state.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_get_en2_pin(uint8_t *state);

/**
 * @brief DC Motor 26 set cm sel pin function.
 * @details This function sets the cm sel pin logic state.
 * @param[in] state : @li @c 0 - Low logic state,
 *                    @li @c 1 - High logic state.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_set_cm_sel_pin(uint8_t state);

/**
 * @brief DC Motor 26 get cm sel pin function.
 * @details This function reads the cm sel pin logic state.
 * @param[out] state : @li @c 0 - Low logic state,
 *                     @li @c 1 - High logic state.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_get_cm_sel_pin(uint8_t *state);

/**
 * @brief DC Motor 26 get diag1 pin function.
 * @details This function reads the diag1 pin logic state.
 * @param[out] state : @li @c 0 - Low logic state,
 *                     @li @c 1 - High logic state.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_get_diag1_pin(uint8_t *state);

/**
 * @brief DC Motor 26 get diag2 pin function.
 * @details This function reads the diag2 pin logic state.
 * @param[out] state : @li @c 0 - Low logic state,
 *                     @li @c 1 - High logic state.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_get_diag2_pin(uint8_t *state);

/**
 * @brief DC Motor 26 get sleep pin function.
 * @details This function reads the sleep pin logic state.
 * @param[out] state : @li @c 0 - Low logic state,
 *                     @li @c 1 - High logic state.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_get_sleep_pin(uint8_t *state);

/**
 * @brief DC Motor 26 read AN pin value function.
 * @details This function reads results of AD conversion of the AN pin.
 * @param[out] data_out : Output ADC result.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_read_an_pin_value(uint16_t *data_out);

/**
 * @brief DC Motor 26 read AN pin voltage level function.
 * @details This function reads results of AD conversion of the AN pin and
 * converts them to proportional voltage level.
 * @param[out] data_out : Output voltage level of the analog pin [V].
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note The conversion to voltage depends on the entered configuration of the
 * ADC (resolution, reference voltage).
 */
sl_status_t mikroe_tb9053ftg_read_an_pin_voltage(float *data_out);

/**
 * @brief DC Motor 26 get motor current function.
 * @details This function reads the motor current consumption.
 * @param[out] current : Current value [A].
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note Make sure the CM sel pin state is set properly.
 */
sl_status_t mikroe_tb9053ftg_get_motor_current(float *current);

/**
 * @brief DC Motor 26 set ch1 operation mode function.
 * @details This function sets the operation mode of the channel 1.
 * @param[in] mode : @li @c 0 - OFF,
 *                   @li @c 1 - FORWARD,
 *                   @li @c 2 - REVERSE,
 *                   @li @c 3 - BRAKE.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_set_ch1_operation_mode(uint8_t mode);

/**
 * @brief DC Motor 26 set ch2 operation mode function.
 * @details This function sets the operation mode of the channel 2.
 * @param[in] mode : @li @c 0 - OFF,
 *                   @li @c 1 - FORWARD,
 *                   @li @c 2 - REVERSE,
 *                   @li @c 3 - BRAKE.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note Ineffective for Combined-Channel Mode (LARGE Mode)
 *       (OSEL1 = L and OSEL2 = L).
 */
sl_status_t mikroe_tb9053ftg_set_ch2_operation_mode(uint8_t mode);

/**
 * @brief DC Motor 26 set ch1 drive frequency function.
 * @details This function sets the raw drive frequency of the channel 1.
 * @param[in] drv_freq : Raw drive frequency (0-2047).
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_set_ch1_drive_frequency(uint16_t drv_freq);

/**
 * @brief DC Motor 26 set ch2 drive frequency function.
 * @details This function sets the raw drive frequency of the channel 2.
 * @param[in] drv_freq : Raw drive frequency (0-2047).
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note Ineffective for Combined-Channel Mode (LARGE Mode)
 *       (OSEL1 = L and OSEL2 = L).
 */
sl_status_t mikroe_tb9053ftg_set_ch2_drive_frequency(uint16_t drv_freq);

/**
 * @brief DC Motor 26 set ch1 duty period function.
 * @details This function sets the duty period of the channel 1.
 * @param[in] duty_period : Duty period (0-2047).
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_tb9053ftg_set_ch1_duty_period(uint16_t duty_period);

/**
 * @brief DC Motor 26 set ch1 duty period function.
 * @details This function sets the duty period of the channel 1.
 * @param[in] duty_period : Duty period (0-2047).
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note Ineffective for Combined-Channel Mode (LARGE Mode)
 *       (OSEL1 = L and OSEL2 = L).
 */
sl_status_t mikroe_tb9053ftg_set_ch2_duty_period(uint16_t duty_period);

#ifdef __cplusplus
}
#endif

#endif // _MIKROE_TB9053FTG_H_
