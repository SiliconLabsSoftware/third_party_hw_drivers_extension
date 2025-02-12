/***************************************************************************//**
 * @file mikroe_drv8245p.h
 * @brief SCL DRV8245P Prototypes
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

#ifndef _MIKROE_DRV8245P_H_
#define _MIKROE_DRV8245P_H_

#include "sl_status.h"
#include "dcmotor29.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup dcmotor29_reg
 * @{
 */

/**
 * @brief DC Motor 29 description register.
 * @details Specified register for description of DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_REG_DEVICE_ID                   0x00
#define MIKROE_DRV8245P_REG_FAULT_SUMMARY               0x01
#define MIKROE_DRV8245P_REG_STATUS1                     0x02
#define MIKROE_DRV8245P_REG_STATUS2                     0x03
#define MIKROE_DRV8245P_REG_COMMAND                     0x08
#define MIKROE_DRV8245P_REG_SPI_IN                      0x09
#define MIKROE_DRV8245P_REG_CONFIG1                     0x0A
#define MIKROE_DRV8245P_REG_CONFIG2                     0x0B
#define MIKROE_DRV8245P_REG_CONFIG3                     0x0C
#define MIKROE_DRV8245P_REG_CONFIG4                     0x0D
#define MIKROE_DRV8245P_REG_INPUT_PORT                  0x00
#define MIKROE_DRV8245P_REG_OUTPUT_PORT                 0x01
#define MIKROE_DRV8245P_REG_POLARITY_INV                0x02
#define MIKROE_DRV8245P_REG_CONFIG                      0x03

/*! @} */ // dcmotor29_reg

/**
 * @defgroup dcmotor29_set DC Motor 29 Registers Settings
 * @brief Settings for registers of DC Motor 29 Click driver.
 */

/**
 * @addtogroup dcmotor29_set
 * @{
 */

/**
 * @brief DC Motor 29 description setting.
 * @details Specified setting for description of DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_CLR_FLT_CMD                     0x80
#define MIKROE_DRV8245P_SPI_IN_UNLOCK_CMD               0x10
#define MIKROE_DRV8245P_SPI_IN_LOCK_CMD                 0x08
#define MIKROE_DRV8245P_REG_UNLOCK_CMD                  0x01
#define MIKROE_DRV8245P_REG_LOCK_CMD                    0x02

/**
 * @brief DC Motor 29 pin setting.
 * @details Specified setting for pin selection of
 * DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_NONE_PIN                        0x00
#define MIKROE_DRV8245P_IN2_PIN                         0x01
#define MIKROE_DRV8245P_DOF_PIN                         0x02
#define MIKROE_DRV8245P_FLT_PIN                         0x04
#define MIKROE_DRV8245P_ALL_PIN                         0x07

/**
 * @brief DC Motor 29 read register.
 * @details Specified setting for reading registers of
 * DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_READ_REG_MASK                   0x40

/**
 * @brief DC Motor 29 DRVOFF pin setting.
 * @details Specified setting for DRVOFF pin selection of
 * DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_DRVOFF_ON                       0x01
#define MIKROE_DRV8245P_DRVOFF_OFF                      0x00

/**
 * @brief DC Motor 29 pin setting.
 * @details Specified setting for pin selection of
 * DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_PIN_STATE_HIGH                  0x01
#define MIKROE_DRV8245P_PIN_STATE_LOW                   0x00

/**
 * @brief DC Motor 29 SPI_IN Register setting.
 * @details Specified setting for SPI_IN Register selection of
 * DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_S_EN_DEVOFF_MASK                0x08
#define MIKROE_DRV8245P_S_EN_DEVOFF2_MASK               0x04
#define MIKROE_DRV8245P_S_EN_IN1_MASK                   0x02
#define MIKROE_DRV8245P_S_EN_IN2_MASK                   0x01
#define MIKROE_DRV8245P_S_EN_NONE                       0x00

/**
 * @brief DC Motor 29 CONFIG1 Register setting.
 * @details Specified setting for CONFIG1 Register selection of
 * DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_EN_OLA                          0x80
#define MIKROE_DRV8245P_VMOV_SEL_DISABLED               0x60
#define MIKROE_DRV8245P_VMOV_SEL_18V                    0x40
#define MIKROE_DRV8245P_VMOV_SEL_28V                    0x20
#define MIKROE_DRV8245P_VMOV_SEL_35V                    0x00
#define MIKROE_DRV8245P_SSC_DISABLE                     0x10
#define MIKROE_DRV8245P_OCP_RETRY                       0x08
#define MIKROE_DRV8245P_TSD_RETRY                       0x04
#define MIKROE_DRV8245P_VMOV_RETRY                      0x02
#define MIKROE_DRV8245P_OLA_RETRY                       0x01

/**
 * @brief DC Motor 29 CONFIG3 Register setting.
 * @details Specified setting for CONFIG3 Register selection of
 * DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_TOF_50_US                       0xC0
#define MIKROE_DRV8245P_TOF_40_US                       0x80
#define MIKROE_DRV8245P_TOF_30_US                       0x40
#define MIKROE_DRV8245P_TOF_20_US                       0x00
#define MIKROE_DRV8245P_S_SR_50_V_US                    0x1C
#define MIKROE_DRV8245P_S_SR_38_V_US                    0x18
#define MIKROE_DRV8245P_S_SR_26_V_US                    0x14
#define MIKROE_DRV8245P_S_SR_20_V_US                    0x10
#define MIKROE_DRV8245P_S_SR_14_V_US                    0x0C
#define MIKROE_DRV8245P_S_SR_9_8V_US                    0x08
#define MIKROE_DRV8245P_S_SR_5_V_US                     0x04
#define MIKROE_DRV8245P_S_SR_1_5V_US                    0x00
#define MIKROE_DRV8245P_S_MODE_PWM                      0x03
#define MIKROE_DRV8245P_S_MODE_INDEPENDENT              0x01
#define MIKROE_DRV8245P_S_MODE_PH_EN                    0x00

/**
 * @brief DC Motor 29 motor states setting.
 * @details Specified setting for motor state selection of
 * DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_DRIVE_MOTOR_CW                  0x00
#define MIKROE_DRV8245P_DRIVE_MOTOR_CCW                 0x01
#define MIKROE_DRV8245P_DRIVE_MOTOR_BRAKE               0x02
#define MIKROE_DRV8245P_DRIVE_MOTOR_COASTING            0x03

/**
 * @brief DC Motor 29 device address setting.
 * @details Specified setting for device slave address selection of
 * DC Motor 29 Click driver.
 */
#define MIKROE_DRV8245P_DEVICE_ADDRESS_A1A0_00          0x70
#define MIKROE_DRV8245P_DEVICE_ADDRESS_A1A0_01          0x71
#define MIKROE_DRV8245P_DEVICE_ADDRESS_A1A0_10          0x72
#define MIKROE_DRV8245P_DEVICE_ADDRESS_A1A0_11          0x73

/**
 * @brief Data sample selection.
 * @details This macro sets data samples for SPI modules.
 * @note Available only on Microchip PIC family devices.
 * This macro will set data sampling for all SPI modules on MCU.
 * Can be overwritten with @b dcmotor29_init which will set
 * @b SET_SPI_DATA_SAMPLE_MIDDLE by default on the mapped mikrobus.
 */
#define MIKROE_DRV8245P_SET_DATA_SAMPLE_EDGE            SET_SPI_DATA_SAMPLE_EDGE
#define MIKROE_DRV8245P_SET_DATA_SAMPLE_MIDDLE \
  SET_SPI_DATA_SAMPLE_MIDDLE

/*! @} */ // dcmotor29_set

/**
 * @brief DC Motor 29 initialization function.
 * @details This function initializes all necessary pins and peripherals used
 * for this click board.
 * @param[in] spi_instance : SPI instance handle.
 * @param[in] i2c_instance : I2C instance handle.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_drv8245p_init(mikroe_spi_handle_t spi_instance,
                                 mikroe_i2c_handle_t i2c_instance);

/**
 * @brief DC Motor 29 default configuration function.
 * @details This function executes a default configuration of DC Motor 29
 * click board.
 * @return SL_STATUS_OK - success, SL_STATUS_FAIL - error.
 * @note This function can consist any necessary configuration or setting to put
 * device into operating mode.
 */
sl_status_t mikroe_drv8245p_default_cfg(void);

/**
 * @brief DC Motor 29 write data function.
 * @details This function writes a desired number of data bytes starting from
 * the selected register by using SPI serial interface.
 * @param[in] reg : Start register address.
 * @param[in] data_in : Data to be written.
 * @param[in] len : Number of bytes to be written.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_drv8245p_generic_write(uint8_t reg,
                                          uint8_t *data_in,
                                          uint8_t len);

/**
 * @brief DC Motor 29 read data function.
 * @details This function reads a desired number of data bytes starting from
 * the selected register by using SPI serial interface.
 * @param[in] reg : Start register address.
 * @param[out] data_out : Output read data.
 * @param[in] len : Number of bytes to be read.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_drv8245p_generic_read(uint8_t reg,
                                         uint8_t *data_out,
                                         uint8_t len);

/**
 * @brief DC Motor 29 write register function.
 * @details This function writes a data byte into
 * the selected register by using SPI serial interface.
 * @param[in] reg : Register address.
 * @param[in] data_in : Data to be written.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_drv8245p_register_write(uint8_t reg, uint8_t data_in);

/**
 * @brief DC Motor 29 read register function.
 * @details This function reads a data byte from
 * the selected register by using SPI serial interface.
 * @param[in] reg : Register address.
 * @param[out] data_out : Output read data.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_drv8245p_register_read(uint8_t reg, uint8_t *data_out);

/**
 * @brief DC Motor 29 port expander write register function.
 * @details This function writes a data byte into
 * the selected register of the port expander.
 * @param[in] reg : Start register address.
 * @param[in] data_in : Data to be written.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_drv8245p_port_expander_write(uint8_t reg, uint8_t data_in);

/**
 * @brief DC Motor 29 port expander read register function.
 * @details This function reads a data byte from
 * the selected register of the port expander.
 * @param[in] reg : Start register address.
 * @param[out] data_out : Output read data.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_drv8245p_port_expander_read(uint8_t reg, uint8_t *data_out);

/**
 * @brief DC Motor 29 set IN1 pin function.
 * @details This function is used to set state of the IN1 pin.
 * @param[in] pin_state : Pin state.
 * @return Nothing.
 * @note None.
 */
void mikroe_drv8245p_set_in1_pin(uint8_t pin_state);

/**
 * @brief DC Motor 29 set RST pin function.
 * @details This function is used to set state of the RST pin.
 * @param[in] pin_state : Pin state.
 * @return Nothing.
 * @note None.
 */
void mikroe_drv8245p_set_rst_pin(uint8_t pin_state);

/**
 * @brief DC Motor 29 get IP pin function.
 * @details This function is used to get state of the IP pin.
 * @return State of the IP pin.
 * @note None.
 */
uint8_t mikroe_drv8245p_get_ip_pin(void);

/**
 * @brief DC Motor 29 get INT pin function.
 * @details This function is used to get state of the INT pin.
 * @return State of the INT pin.
 * @note None.
 */
uint8_t mikroe_drv8245p_get_int_pin(void);

/**
 * @brief DC Motor 29 set pins function.
 * @details This function sets and clears the value of the selected pins of port expander.
 * @param[in] clr_mask : Pin clear mask.
 * @param[in] set_mask : Pin set mask.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_drv8245p_set_pins(uint8_t set_mask, uint8_t clr_mask);

/**
 * @brief DC Motor 29 control drvoff function.
 * @details This function sets the state of the DRVOFF pin, enabling or disabling outputs.
 * @param[in] drvoff_state : State of the DRVOFF pin.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_drv8245p_drvoff_state(uint8_t drvoff_state);

/**
 * @brief DC Motor 29 drive motor function.
 * @details This function sets motor driver into the selected state.
 * @param[in] state : State of the driver.
 * @return SL_STATUS_OK - Success,
 *         SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_drv8245p_drive_motor(uint8_t state);

#ifdef __cplusplus
}
#endif

#endif // _MIKROE_DRV8245P_H_
