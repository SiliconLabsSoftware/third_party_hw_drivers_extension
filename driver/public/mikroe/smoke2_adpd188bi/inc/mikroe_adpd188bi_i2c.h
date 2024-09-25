/***************************************************************************//**
 * @file mikroe_adpd188bi.h
 * @brief Mikroe ADPD188BI Prototypes
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
#ifndef MIKROE_ADPD188BI_H_
#define MIKROE_ADPD188BI_H_

#include "sl_status.h"
#include "smoke2.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIKROE_ADPD188BI_REG_STATUS               SMOKE2_REG_STATUS
#define MIKROE_ADPD188BI_REG_INT_MASK             SMOKE2_REG_INT_MASK
#define MIKROE_ADPD188BI_REG_GPIO_DRV             SMOKE2_REG_GPIO_DRV
#define MIKROE_ADPD188BI_REG_BG_STATUS            SMOKE2_REG_BG_STATUS
#define MIKROE_ADPD188BI_REG_FIFO_THR             SMOKE2_REG_FIFO_THR
#define MIKROE_ADPD188BI_REG_DEVID                SMOKE2_REG_DEVID
#define MIKROE_ADPD188BI_REG_I2CS_ID              SMOKE2_REG_I2CS_ID
#define MIKROE_ADPD188BI_REG_CLK_RATIO            SMOKE2_REG_CLK_RATIO
#define MIKROE_ADPD188BI_REG_GPIO_CTRL            SMOKE2_REG_GPIO_CTRL
#define MIKROE_ADPD188BI_REG_SLAVE_ADDR_KEY       SMOKE2_REG_SLAVE_ADDR_KEY
#define MIKROE_ADPD188BI_REG_SW_RESET             SMOKE2_REG_SW_RESET
#define MIKROE_ADPD188BI_REG_MODE                 SMOKE2_REG_MODE
#define MIKROE_ADPD188BI_REG_SLOT_EN              SMOKE2_REG_SLOT_EN
#define MIKROE_ADPD188BI_REG_FSAMPLE              SMOKE2_REG_FSAMPLE
#define MIKROE_ADPD188BI_REG_PD_LED_SELECT        SMOKE2_REG_PD_LED_SELECT
#define MIKROE_ADPD188BI_REG_NUM_AVG              SMOKE2_REG_NUM_AVG
#define MIKROE_ADPD188BI_REG_BIG_MEAS_A           SMOKE2_REG_BIG_MEAS_A
#define MIKROE_ADPD188BI_REG_INT_SEQ_A            SMOKE2_REG_INT_SEQ_A
#define MIKROE_ADPD188BI_REG_SLOTA_CH1_OFFSET     SMOKE2_REG_SLOTA_CH1_OFFSET
#define MIKROE_ADPD188BI_REG_SLOTA_CH2_OFFSET     SMOKE2_REG_SLOTA_CH2_OFFSET
#define MIKROE_ADPD188BI_REG_SLOTA_CH3_OFFSET     SMOKE2_REG_SLOTA_CH3_OFFSET
#define MIKROE_ADPD188BI_REG_SLOTA_CH4_OFFSET     SMOKE2_REG_SLOTA_CH4_OFFSET
#define MIKROE_ADPD188BI_REG_BIG_MEAS_B           SMOKE2_REG_BIG_MEAS_B
#define MIKROE_ADPD188BI_REG_INT_SEQ_B            SMOKE2_REG_INT_SEQ_B
#define MIKROE_ADPD188BI_REG_SLOTB_CH1_OFFSET     SMOKE2_REG_SLOTB_CH1_OFFSET
#define MIKROE_ADPD188BI_REG_SLOTB_CH2_OFFSET     SMOKE2_REG_SLOTB_CH2_OFFSET
#define MIKROE_ADPD188BI_REG_SLOTB_CH3_OFFSET     SMOKE2_REG_SLOTB_CH3_OFFSET
#define MIKROE_ADPD188BI_REG_SLOTB_CH4_OFFSET     SMOKE2_REG_SLOTB_CH4_OFFSET
#define MIKROE_ADPD188BI_REG_ILED3_COARSE         SMOKE2_REG_ILED3_COARSE
#define MIKROE_ADPD188BI_REG_ILED1_COARSE         SMOKE2_REG_ILED1_COARSE
#define MIKROE_ADPD188BI_REG_ILED2_COARSE         SMOKE2_REG_ILED2_COARSE
#define MIKROE_ADPD188BI_REG_ILED_FINE            SMOKE2_REG_ILED_FINE
#define MIKROE_ADPD188BI_REG_SLOTA_LED_PULSE      SMOKE2_REG_SLOTA_LED_PULSE
#define MIKROE_ADPD188BI_REG_SLOTA_NUM_PULSES     SMOKE2_REG_SLOTA_NUM_PULSES
#define MIKROE_ADPD188BI_REG_LED_DISABLE          SMOKE2_REG_LED_DISABLE
#define MIKROE_ADPD188BI_REG_SLOTB_LED_PULSE      SMOKE2_REG_SLOTB_LED_PULSE
#define MIKROE_ADPD188BI_REG_SLOTB_NUM_PULSES     SMOKE2_REG_SLOTB_NUM_PULSES
#define MIKROE_ADPD188BI_REG_ALT_PWR_DN           SMOKE2_REG_ALT_PWR_DN
#define MIKROE_ADPD188BI_REG_EXT_SYNC_STARTUP     SMOKE2_REG_EXT_SYNC_STARTUP
#define MIKROE_ADPD188BI_REG_SLOTA_AFE_WINDOW     SMOKE2_REG_SLOTA_AFE_WINDOW
#define MIKROE_ADPD188BI_REG_SLOTB_AFE_WINDOW     SMOKE2_REG_SLOTB_AFE_WINDOW
#define MIKROE_ADPD188BI_REG_AFE_PWR_CFG1         SMOKE2_REG_AFE_PWR_CFG1
#define MIKROE_ADPD188BI_REG_SLOTA_FLOAT_LED      SMOKE2_REG_SLOTA_FLOAT_LED
#define MIKROE_ADPD188BI_REG_SLOTB_FLOAT_LED      SMOKE2_REG_SLOTB_FLOAT_LED
#define MIKROE_ADPD188BI_REG_SLOTA_TIA_CFG        SMOKE2_REG_SLOTA_TIA_CFG
#define MIKROE_ADPD188BI_REG_SLOTA_AFE_CFG        SMOKE2_REG_SLOTA_AFE_CFG
#define MIKROE_ADPD188BI_REG_SLOTB_TIA_CFG        SMOKE2_REG_SLOTB_TIA_CFG
#define MIKROE_ADPD188BI_REG_SLOTB_AFE_CFG        SMOKE2_REG_SLOTB_AFE_CFG
#define MIKROE_ADPD188BI_REG_SAMPLE_CLK           SMOKE2_REG_SAMPLE_CLK
#define MIKROE_ADPD188BI_REG_CLK32M_ADJUST        SMOKE2_REG_CLK32M_ADJUST
#define MIKROE_ADPD188BI_REG_EXT_SYNC_SEL         SMOKE2_REG_EXT_SYNC_SEL
#define MIKROE_ADPD188BI_REG_CLK32M_CAL_EN        SMOKE2_REG_CLK32M_CAL_EN
#define MIKROE_ADPD188BI_REG_AFE_PWR_CFG2         SMOKE2_REG_AFE_PWR_CFG2
#define MIKROE_ADPD188BI_REG_TIA_INDEP_GAIN       SMOKE2_REG_TIA_INDEP_GAIN
#define MIKROE_ADPD188BI_REG_MATH                 SMOKE2_REG_MATH
#define MIKROE_ADPD188BI_REG_FLT_CONFIG_B         SMOKE2_REG_FLT_CONFIG_B
#define MIKROE_ADPD188BI_REG_FLT_LED_FIRE         SMOKE2_REG_FLT_LED_FIRE
#define MIKROE_ADPD188BI_REG_FLT_CONFIG_A         SMOKE2_REG_FLT_CONFIG_A
#define MIKROE_ADPD188BI_REG_DATA_ACCESS_CTL      SMOKE2_REG_DATA_ACCESS_CTL
#define MIKROE_ADPD188BI_REG_FIFO_ACCESS          SMOKE2_REG_FIFO_ACCESS
#define MIKROE_ADPD188BI_REG_SLOTA_CH1            SMOKE2_REG_SLOTA_CH1
#define MIKROE_ADPD188BI_REG_SLOTA_CH2            SMOKE2_REG_SLOTA_CH2
#define MIKROE_ADPD188BI_REG_SLOTA_CH3            SMOKE2_REG_SLOTA_CH3
#define MIKROE_ADPD188BI_REG_SLOTA_CH4            SMOKE2_REG_SLOTA_CH4
#define MIKROE_ADPD188BI_REG_SLOTB_CH1            SMOKE2_REG_SLOTB_CH1
#define MIKROE_ADPD188BI_REG_SLOTB_CH2            SMOKE2_REG_SLOTB_CH2
#define MIKROE_ADPD188BI_REG_SLOTB_CH3            SMOKE2_REG_SLOTB_CH3
#define MIKROE_ADPD188BI_REG_SLOTB_CH4            SMOKE2_REG_SLOTB_CH4
#define MIKROE_ADPD188BI_REG_A_CH1_LOW            SMOKE2_REG_A_CH1_LOW
#define MIKROE_ADPD188BI_REG_A_CH2_LOW            SMOKE2_REG_A_CH2_LOW
#define MIKROE_ADPD188BI_REG_A_CH3_LOW            SMOKE2_REG_A_CH3_LOW
#define MIKROE_ADPD188BI_REG_A_CH4_LOW            SMOKE2_REG_A_CH4_LOW
#define MIKROE_ADPD188BI_REG_A_CH1_HIGH           SMOKE2_REG_A_CH1_HIGH
#define MIKROE_ADPD188BI_REG_A_CH2_HIGH           SMOKE2_REG_A_CH2_HIGH
#define MIKROE_ADPD188BI_REG_A_CH3_HIGH           SMOKE2_REG_A_CH3_HIGH
#define MIKROE_ADPD188BI_REG_A_CH4_HIGH           SMOKE2_REG_A_CH4_HIGH
#define MIKROE_ADPD188BI_REG_B_CH1_LOW            SMOKE2_REG_B_CH1_LOW
#define MIKROE_ADPD188BI_REG_B_CH2_LOW            SMOKE2_REG_B_CH2_LOW
#define MIKROE_ADPD188BI_REG_B_CH3_LOW            SMOKE2_REG_B_CH3_LOW
#define MIKROE_ADPD188BI_REG_B_CH4_LOW            SMOKE2_REG_B_CH4_LOW
#define MIKROE_ADPD188BI_REG_B_CH1_HIGH           SMOKE2_REG_B_CH1_HIGH
#define MIKROE_ADPD188BI_REG_B_CH2_HIGH           SMOKE2_REG_B_CH2_HIGH
#define MIKROE_ADPD188BI_REG_B_CH3_HIGH           SMOKE2_REG_B_CH3_HIGH
#define MIKROE_ADPD188BI_REG_B_CH4_HIGH           SMOKE2_REG_B_CH4_HIGH

/*! @} */ // mikroe_adpd188bi_reg

/**
 * @defgroup mikroe_adpd188bi_set Smoke 2 Registers Settings
 * @brief Settings for registers of Smoke 2 Click driver.
 */

/**
 * @addtogroup mikroe_adpd188bi_set
 * @{
 */

/**
 * @brief Smoke 2 description setting.
 * @details Specified setting for description of Smoke 2 Click driver.
 */
#define MIKROE_ADPD188BI_SLOT_A                   SMOKE2_SLOT_A
#define MIKROE_ADPD188BI_SLOT_B                   SMOKE2_SLOT_B

/**
 * @brief Smoke 2 description setting to select channel.
 * @details Specified setting for select channel Smoke 2 Click driver.
 */
#define MIKROE_ADPD188BI_CHN_1                    SMOKE2_CHN_1
#define MIKROE_ADPD188BI_CHN_2                    SMOKE2_CHN_2
#define MIKROE_ADPD188BI_CHN_3                    SMOKE2_CHN_3
#define MIKROE_ADPD188BI_CHN_4                    SMOKE2_CHN_4

/**
 * @brief Smoke 2 description setting to select mode.
 * @details Specified setting for select mdoe Smoke 2 Click driver.
 */
#define MIKROE_ADPD188BI_MODE_IDLE                SMOKE2_MODE_IDLE
#define MIKROE_ADPD188BI_MODE_PROGRAM             SMOKE2_MODE_PROGRAM
#define MIKROE_ADPD188BI_MODE_NORMAL              SMOKE2_MODE_NORMAL

/**
 * @brief Smoke 2 description setting for detection smoke.
 * @details Specified setting for detection smoke Smoke 2 Click driver.
 */
#define MIKROE_ADPD188BI_SMOKE_DETECTED           SMOKE2_SMOKE_DETECTED
#define MIKROE_ADPD188BI_SMOKE_NOT_DETECTED       SMOKE2_SMOKE_NOT_DETECTED

/**
 * @brief Smoke 2 description setting for read and write.
 * @details Specified setting for read and write Smoke 2 Click driver.
 */
#define MIKROE_ADPD188BI_READ_CMD                 SMOKE2_READ_CMD
#define MIKROE_ADPD188BI_WRITE_CMD                SMOKE2_WRITE_CMD

/**
 * @brief Smoke 2 device address setting.
 * @details Specified setting for device slave address selection of
 * Smoke 2 Click driver.
 */
#define MIKROE_ADPD188BI_SET_DEV_ADDR             SMOKE2_SET_DEV_ADDR

/**
 * @brief Data sample selection.
 * @details This macro sets data samples for SPI modules.
 * @note Available only on Microchip PIC family devices.
 * This macro will set data sampling for all SPI modules on MCU.
 * Can be overwritten with @b mikroe_adpd188bi_init which will set
 * @b SET_SPI_DATA_SAMPLE_MIDDLE by default on the mapped mikrobus.
 */
#define MIKROE_ADPD188BI_SET_DATA_SAMPLE_EDGE     SMOKE2_SET_DATA_SAMPLE_EDGE
#define MIKROE_ADPD188BI_SET_DATA_SAMPLE_MIDDLE   SMOKE2_SET_DATA_SAMPLE_MIDDLE

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
sl_status_t mikroe_adpd188bi_init(mikroe_i2c_handle_t i2cspm_instance);

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
sl_status_t mikroe_adpd188bi_set_i2csmp_instance(
  mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function executes default configuration for mikroe_adpd188bi.
 *
 * @return void
 *
 ******************************************************************************/
void mikroe_adpd188bi_default_cfg (void);

/***************************************************************************//**
 * @brief
 *    This function writes data to the desired register.
 *
 * @param[in] reg : Register address.
 * @param[in] data_in : Data to be written.
 * @param[in] len : The number of data length in byte
 * @return  SL_STATUS_INVALID_PARAMETER if data_in is null
 *          SL_STATUS_OK - Success
 *
 ******************************************************************************/
sl_status_t mikroe_adpd188bi_generic_write(uint8_t reg,
                                           uint8_t *data_in,
                                           uint8_t len);

/***************************************************************************//**
 * @brief
 *    This function reads data from the desired register.
 *
 * @param[in] reg : Register address.
 * @param[out] data_out : Output read data
 * @param[in] len : The number of data length in byte
 * @return
 *    SL_STATUS_INVALID_PARAMETER if data_out is null
 *    SL_STATUS_OK - Success
 ******************************************************************************/
sl_status_t mikroe_adpd188bi_generic_read (uint8_t reg,
                                           uint8_t *data_out,
                                           uint8_t len);

/***************************************************************************//**
 * @brief
 *    This function read value of the interrupt pin
 * @param[in] :
 * @param[out] :
 * @return The value of interrupt pin.
 *
 ******************************************************************************/
uint8_t mikroe_adpd188bi_get_int_pin (void);

/***************************************************************************//**
 * @brief
 *    This function writes data to the desired register.
 *
 * @param[in] reg : Register address.
 * @param[in] tx_data : Data to be written.
 * @return : None
 *
 ******************************************************************************/
void mikroe_adpd188bi_write_data(uint8_t reg, uint16_t tx_data);

/***************************************************************************//**
 * @brief
 *    This function reads data from the desired register.
 *
 * @param[in] reg : Register address.
 * @return :  Output read data
 *
 ******************************************************************************/
uint16_t mikroe_adpd188bi_read_data(uint8_t reg);

/***************************************************************************//**
 * @brief
 *    This function sets the value according to the specific bit in the desired
 *    register
 *
 * @param[in] reg : Register address.
 * @param[in] bit_num : Specific bit number in the register.
 * @param[in] val : Value to be written.
 * @return :  void
 *
 ******************************************************************************/
void mikroe_adpd188bi_set_bit (uint8_t reg, uint8_t bit_num, uint8_t val);

/***************************************************************************//**
 * @brief
 *    This function gets the value according to the specific bit in the desired
 *    register
 *
 * @param[in] reg : Register address.
 * @param[in] bit_num : Specific bit number in the register.
 * @return :  Output read data
 *
 ******************************************************************************/
uint8_t mikroe_adpd188bi_get_bit (uint8_t reg, uint8_t bit_num);

/***************************************************************************//**
 * @brief
 *    This function set the mode for smoke2_adpd188bi
 *
 * @param[in] mode : Mode for setting.
 * @return :
 *
 ******************************************************************************/
void mikroe_adpd188bi_set_mode (uint8_t mode);

/***************************************************************************//**
 * @brief
 *    This function get the int
 *
 * @param[in] fifo : FiFo pointer
 * @param[in] slot_a : Slot_a pointer
 * @param[in] slot_b : Slot_b pointer
 * @return :  SL_STATUS_OK or SL_STATUS_INVALID_PARAMETER
 *
 ******************************************************************************/
sl_status_t mikroe_adpd188bi_get_int(uint8_t *fifo,
                                     uint8_t *slot_a,
                                     uint8_t *slot_b);

/***************************************************************************//**
 * @brief
 *    This function read the sens data
 *
 * @param[in] slot : slot number
 * @param[in] chn : channel number
 * @return :  The current sens data
 *
 ******************************************************************************/
uint16_t mikroe_adpd188bi_read_sens_data (uint8_t slot, uint8_t chn);

/***************************************************************************//**
 * @brief
 *    This function perform the software reset
 * @return :  None
 *
 ******************************************************************************/
void mikroe_adpd188bi_soft_reset (void);

/***************************************************************************//**
 * @brief
 *    This function perform calibrate for mikroe_adpd188bi
 *
 * @param[in] threshold : The threshold to set
 * @return :  The calibration value
 *
 ******************************************************************************/
uint16_t mikroe_adpd188bi_smoke_calibration (uint16_t threshold);

/***************************************************************************//**
 * @brief
 *    This function checks whether the sensing value is above threshold setting.
 *
 * @return :  SMOKE2_SMOKE_DETECTED or  SMOKE2_SMOKE_NOT_DETECTED or 0 for ERROR
 *
 ******************************************************************************/
uint8_t mikroe_adpd188bi_check_smoke (void);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_ADPD188BI_H_ */
