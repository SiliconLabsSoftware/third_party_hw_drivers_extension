/***************************************************************************//**
 * @file mikroe_pca9685.h
 * @brief SCL Mikroe Servo Click Header File
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

#ifndef MIKROE_PCA9685_H_
#define MIKROE_PCA9685_H_

#include "sl_status.h"
#include "servo.h"
#include "mikroe_servo_config.h"

typedef struct {
  uint8_t min_position;
  uint8_t max_position;
  uint16_t low_resolution;
  uint16_t high_resolution;
} mikroe_servo_pos_and_res_t;

/**
 * \defgroup servo_registers Servo Registers
 * \{
 */
#define MIKROE_SERVO_REG_MODE_1                      0x00
#define MIKROE_SERVO_REG_MODE_2                      0x01
#define MIKROE_SERVO_REG_SUB_ADDRESS_1               0x02
#define MIKROE_SERVO_REG_SUB_ADDRESS_2               0x03
#define MIKROE_SERVO_REG_SUB_ADDRESS_3               0x04
#define MIKROE_SERVO_REG_ALL_CALL_ADR                0x05

#define MIKROE_SERVO_REG_MOTOR_1_ON_L                0x06
#define MIKROE_SERVO_REG_MOTOR_1_ON_H                0x07
#define MIKROE_SERVO_REG_MOTOR_1_OFF_L               0x08
#define MIKROE_SERVO_REG_MOTOR_1_OFF_H               0x09
#define MIKROE_SERVO_REG_MOTOR_2_ON_L                0x0A
#define MIKROE_SERVO_REG_MOTOR_2_ON_H                0x0B
#define MIKROE_SERVO_REG_MOTOR_2_OFF_L               0x0C
#define MIKROE_SERVO_REG_MOTOR_2_OFF_H               0x0D
#define MIKROE_SERVO_REG_MOTOR_3_ON_L                0x0E
#define MIKROE_SERVO_REG_MOTOR_3_ON_H                0x0F
#define MIKROE_SERVO_REG_MOTOR_3_OFF_L               0x10
#define MIKROE_SERVO_REG_MOTOR_3_OFF_H               0x11
#define MIKROE_SERVO_REG_MOTOR_4_ON_L                0x12
#define MIKROE_SERVO_REG_MOTOR_4_ON_H                0x13
#define MIKROE_SERVO_REG_MOTOR_4_OFF_L               0x14
#define MIKROE_SERVO_REG_MOTOR_4_OFF_H               0x15
#define MIKROE_SERVO_REG_MOTOR_5_ON_L                0x16
#define MIKROE_SERVO_REG_MOTOR_5_ON_H                0x17
#define MIKROE_SERVO_REG_MOTOR_5_OFF_L               0x18
#define MIKROE_SERVO_REG_MOTOR_5_OFF_H               0x19
#define MIKROE_SERVO_REG_MOTOR_6_ON_L                0x1A
#define MIKROE_SERVO_REG_MOTOR_6_ON_H                0x1B
#define MIKROE_SERVO_REG_MOTOR_6_OFF_L               0x1C
#define MIKROE_SERVO_REG_MOTOR_6_OFF_H               0x1D
#define MIKROE_SERVO_REG_MOTOR_7_ON_L                0x1E
#define MIKROE_SERVO_REG_MOTOR_7_ON_H                0x1F
#define MIKROE_SERVO_REG_MOTOR_7_OFF_L               0x20
#define MIKROE_SERVO_REG_MOTOR_7_OFF_H               0x21
#define MIKROE_SERVO_REG_MOTOR_8_ON_L                0x22
#define MIKROE_SERVO_REG_MOTOR_8_ON_H                0x23
#define MIKROE_SERVO_REG_MOTOR_8_OFF_L               0x24
#define MIKROE_SERVO_REG_MOTOR_8_OFF_H               0x25
#define MIKROE_SERVO_REG_MOTOR_9_ON_L                0x26
#define MIKROE_SERVO_REG_MOTOR_9_ON_H                0x27
#define MIKROE_SERVO_REG_MOTOR_9_OFF_L               0x28
#define MIKROE_SERVO_REG_MOTOR_9_OFF_H               0x29
#define MIKROE_SERVO_REG_MOTOR_10_ON_L               0x2A
#define MIKROE_SERVO_REG_MOTOR_10_ON_H               0x2B
#define MIKROE_SERVO_REG_MOTOR_10_OFF_L              0x2C
#define MIKROE_SERVO_REG_MOTOR_10_OFF_H              0x2D
#define MIKROE_SERVO_REG_MOTOR_11_ON_L               0x2E
#define MIKROE_SERVO_REG_MOTOR_11_ON_H               0x2F
#define MIKROE_SERVO_REG_MOTOR_11_OFF_L              0x30
#define MIKROE_SERVO_REG_MOTOR_11_OFF_H              0x31
#define MIKROE_SERVO_REG_MOTOR_12_ON_L               0x32
#define MIKROE_SERVO_REG_MOTOR_12_ON_H               0x33
#define MIKROE_SERVO_REG_MOTOR_12_OFF_L              0x34
#define MIKROE_SERVO_REG_MOTOR_12_OFF_H              0x35
#define MIKROE_SERVO_REG_MOTOR_13_ON_L               0x36
#define MIKROE_SERVO_REG_MOTOR_13_ON_H               0x37
#define MIKROE_SERVO_REG_MOTOR_13_OFF_L              0x38
#define MIKROE_SERVO_REG_MOTOR_13_OFF_H              0x39
#define MIKROE_SERVO_REG_MOTOR_14_ON_L               0x3A
#define MIKROE_SERVO_REG_MOTOR_14_ON_H               0x3B
#define MIKROE_SERVO_REG_MOTOR_14_OFF_L              0x3C
#define MIKROE_SERVO_REG_MOTOR_14_OFF_H              0x3D
#define MIKROE_SERVO_REG_MOTOR_15_ON_L               0x3E
#define MIKROE_SERVO_REG_MOTOR_15_ON_H               0x3F
#define MIKROE_SERVO_REG_MOTOR_15_OFF_L              0x40
#define MIKROE_SERVO_REG_MOTOR_15_OFF_H              0x41
#define MIKROE_SERVO_REG_MOTOR_16_ON_L               0x42
#define MIKROE_SERVO_REG_MOTOR_16_ON_H               0x43
#define MIKROE_SERVO_REG_MOTOR_16_OFF_L              0x44
#define MIKROE_SERVO_REG_MOTOR_16_OFF_H              0x45

#define MIKROE_SERVO_REG_ALL_MOTOR_ON_L              0xFA
#define MIKROE_SERVO_REG_ALL_MOTOR_ON_H              0xFB
#define MIKROE_SERVO_REG_ALL_MOTOR_OFF_L             0xFC
#define MIKROE_SERVO_REG_ALL_MOTOR_OFF_H             0xFD
#define MIKROE_SERVO_REG_TEST_MODE                   0xFF

/** \} */

/**
 * \defgroup mode_1_register Mode 1 Register
 * \{
 */
#define MIKROE_SERVO_MODE1_RESTART_ENABLE            0x01 << 7
#define MIKROE_SERVO_MODE1_RESTART_DISABLE           0x00 << 7
#define MIKROE_SERVO_MODE1_INTERNAL_CLOCK            0x00 << 6
#define MIKROE_SERVO_MODE1_EXTCLK_PIN_CLOCK          0x01 << 6
#define MIKROE_SERVO_MODE1_AUTO_INCREMENT_ENABLE     0x01 << 5
#define MIKROE_SERVO_MODE1_AUTO_INCREMENT_DISABLE    0x00 << 5
#define MIKROE_SERVO_MODE1_NORMAL_MODE               0x00 << 4
#define MIKROE_SERVO_MODE1_LOW_POWER_MODE            0x01 << 4
#define MIKROE_SERVO_MODE1_USE_SUBADR_1              0x01 << 3
#define MIKROE_SERVO_MODE1_NO_USE_SUBADR_1           0x00 << 3
#define MIKROE_SERVO_MODE1_USE_SUBADR_2              0x01 << 2
#define MIKROE_SERVO_MODE1_NO_USE_SUBADR_2           0x00 << 2
#define MIKROE_SERVO_MODE1_USE_SUBADR_3              0x01 << 1
#define MIKROE_SERVO_MODE1_NO_USE_SUBADR_3           0x00 << 1
#define MIKROE_SERVO_MODE1_USE_ALL_CALL_ADR          0x01
#define MIKROE_SERVO_MODE1_NO_USE_ALL_CALL_ADR       0x00

/** \} */

/**
 * \defgroup mode_2_register Mode 2 Register
 * \{
 */
#define MIKROE_SERVO_MODE2_OUT_LOGIC_NOT_INVERTED    0x00 << 4
#define MIKROE_SERVO_MODE2_OUT_LOGIC_INVERTED        0x01 << 4
#define MIKROE_SERVO_MODE2_OUT_CHANGE_ON_STOP_CMD    0x00 << 3
#define MIKROE_SERVO_MODE2_OUT_CHANGE_ON_ACK_CMD     0x01 << 3
#define MIKROE_SERVO_MODE2_OPEN_DRAIN_STRUCTURE      0x00 << 2
#define MIKROE_SERVO_MODE2_TOTEM_POLE_STRUCTURE      0x01 << 2

/** \} */

/**
 * \defgroup servo_min_max Servo Min Max
 * \{
 */
#define MIKROE_SERVO_DEFAULT_LOW_RESOLUTION          0
#define MIKROE_SERVO_DEFAULT_HIGH_RESOLUTION         330

#define MIKROE_SERVO_GENERAL_CALL_ADR                0x00

#define MIKROE_SERVO_VREF_3300                       3300
#define MIKROE_SERVO_VREF_5000                       5000

/** \} */

/**
 * \defgroup servo_motor Servo Motor
 * \{
 */
#define MIKROE_SERVO_MOTOR_1                         0x06
#define MIKROE_SERVO_MOTOR_2                         0x0A
#define MIKROE_SERVO_MOTOR_3                         0x0E
#define MIKROE_SERVO_MOTOR_4                         0x12
#define MIKROE_SERVO_MOTOR_5                         0x16
#define MIKROE_SERVO_MOTOR_6                         0x1A
#define MIKROE_SERVO_MOTOR_7                         0x1E
#define MIKROE_SERVO_MOTOR_8                         0x22
#define MIKROE_SERVO_MOTOR_9                         0x26
#define MIKROE_SERVO_MOTOR_10                        0x2A
#define MIKROE_SERVO_MOTOR_11                        0x2E
#define MIKROE_SERVO_MOTOR_12                        0x32
#define MIKROE_SERVO_MOTOR_13                        0x36
#define MIKROE_SERVO_MOTOR_14                        0x3A
#define MIKROE_SERVO_MOTOR_15                        0x3E
#define MIKROE_SERVO_MOTOR_16                        0x42

#define MIKROE_SERVO_POSITIVE_CH0_NEGATIVE_CH1       0xA0
#define MIKROE_SERVO_POSITIVE_CH2_NEGATIVE_CH3       0xA1
#define MIKROE_SERVO_POSITIVE_CH4_NEGATIVE_CH5       0xA2
#define MIKROE_SERVO_POSITIVE_CH6_NEGATIVE_CH7       0xA3
#define MIKROE_SERVO_POSITIVE_CH8_NEGATIVE_CH9       0xA4
#define MIKROE_SERVO_POSITIVE_CH10_NEGATIVE_CH11     0xA5
#define MIKROE_SERVO_POSITIVE_CH12_NEGATIVE_CH13     0xA6
#define MIKROE_SERVO_POSITIVE_CH14_NEGATIVE_CH15     0xA7
#define MIKROE_SERVO_POSITIVE_CH1_NEGATIVE_CH0       0xA8
#define MIKROE_SERVO_POSITIVE_CH3_NEGATIVE_CH2       0xA9
#define MIKROE_SERVO_POSITIVE_CH5_NEGATIVE_CH4       0xAA
#define MIKROE_SERVO_POSITIVE_CH7_NEGATIVE_CH6       0xAB
#define MIKROE_SERVO_POSITIVE_CH9_NEGATIVE_CH8       0xAC
#define MIKROE_SERVO_POSITIVE_CH11_NEGATIVE_CH10     0xAD
#define MIKROE_SERVO_POSITIVE_CH13_NEGATIVE_CH12     0xAE
#define MIKROE_SERVO_POSITIVE_CH15_NEGATIVE_CH14     0xAF

#define MIKROE_SERVO_POSITIVE_CH0                    0xB0
#define MIKROE_SERVO_POSITIVE_CH2                    0xB1
#define MIKROE_SERVO_POSITIVE_CH4                    0xB2
#define MIKROE_SERVO_POSITIVE_CH6                    0xB3
#define MIKROE_SERVO_POSITIVE_CH8                    0xB4
#define MIKROE_SERVO_POSITIVE_CH10                   0xB5
#define MIKROE_SERVO_POSITIVE_CH12                   0xB6
#define MIKROE_SERVO_POSITIVE_CH14                   0xB7
#define MIKROE_SERVO_POSITIVE_CH1                    0xB8
#define MIKROE_SERVO_POSITIVE_CH3                    0xB9
#define MIKROE_SERVO_POSITIVE_CH5                    0xBA
#define MIKROE_SERVO_POSITIVE_CH7                    0xBB
#define MIKROE_SERVO_POSITIVE_CH9                    0xBC
#define MIKROE_SERVO_POSITIVE_CH11                   0xBD
#define MIKROE_SERVO_POSITIVE_CH13                   0xBE
#define MIKROE_SERVO_POSITIVE_CH15                   0xBF

/** \} */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Click Default Configuration function.
 *
 *
 * @description This function executes default configuration for Servo click.
 */
void mikroe_pca9685_default_cfg(void);

/**
 * @brief Set I2C instance, address for PCA9685 and LTC2497.
 *
 * @param i2cspm_instance I2C handle instance.
 *
 * @return status of function.
 *
 * @description This function set the I2C instance and the address for PCA9685
 *   and LTC2497.
 */
sl_status_t mikroe_pca9685_set_i2c_instance(mikroe_i2c_handle_t i2c_instance);

/**
 * @brief Initialization function.
 *
 * @param i2cspm_instance I2C handle instance.
 *
 * @return status of function.
 *
 * @description This function initializes all necessary pins and peripherals
 *   used for this click.
 */
sl_status_t mikroe_pca9685_init(mikroe_i2c_handle_t i2c_instance);

/**
 * @brief Generic write function of pca9685.
 *
 * @param reg          Register address.
 * @param data_buf     Data buf to be written.
 * @param len          Number of the bytes in data buf.
 *
 * @return status of function.
 *
 * @description This function writes data to the desired register using
 *   slave_address of pca9685.
 */
sl_status_t mikroe_pca9685_generic_write_of_pca9685(uint8_t reg,
                                                    uint8_t *data_buf,
                                                    uint8_t len);

/**
 * @brief Generic write function of ltc2497.
 *
 * @param reg          Register address.
 * @param data_buf     Data buf to be written.
 * @param len          Number of the bytes in data buf.
 *
 * @return status of function.
 *
 * @description This function writes data to the desired register using
 *   slave_address of ltc2497.
 */
sl_status_t mikroe_pca9685_generic_write_of_ltc2497(uint8_t reg,
                                                    uint8_t *data_buf,
                                                    uint8_t len);

/**
 * @brief Generic read function of pca9685.
 *
 * @param reg          Register address.
 * @param data_buf     Output data buf.
 * @param len          Number of the bytes to be read.
 *
 * @return status of function.
 *
 * @description This function reads data from the desired register using
 *   slave_address of pca9685.
 */
sl_status_t mikroe_pca9685_generic_read_of_pca9685(uint8_t reg,
                                                   uint8_t *data_buf,
                                                   uint8_t len);

/**
 * @brief Generic read function of ltc2497.
 *
 * @param reg          Register address.
 * @param data_buf     Output data buf.
 * @param len          Number of the bytes to be read.
 *
 * @return status of function.
 *
 * @description This function reads data from the desired register using
 *   slave_address of ltc2497.
 */
sl_status_t mikroe_pca9685_generic_read_of_ltc2497(uint8_t reg,
                                                   uint8_t *data_buf,
                                                   uint8_t len);

/**
 * @brief Setting function.
 *
 * @param mikroe_pos_and_res  Structure with position and resolution.
 *
 * @description This function setting position and resolution.
 */
void mikroe_pca9685_setting(mikroe_servo_pos_and_res_t *pos_and_res);

/**
 * @brief Set Vref function.
 *
 * @param v_ref  V ref.
 *
 * @return status of function.
 *
 * @description This function settings Vref of Servo Clicks.
 */
sl_status_t mikroe_pca9685_set_vref(uint16_t v_ref);

/**
 * @brief Stop function.
 *
 * @description When the servo_stop() function is started, all controls and
 *   communication with the servo motor are blocked.
 * To restore communication, you need to call the servo_start() function.
 * Use this function when you need to shut down the all motors quickly.
 */
void mikroe_pca9685_stop(void);

/**
 * @brief Start function.
 *
 * @description Function for start work servo motor.
 */
void mikroe_pca9685_start(void);

/**
 * @brief Soft reset function.
 *
 * @description Functions for soft reset chip.
 */
void mikroe_pca9685_soft_reset(void);

/**
 * @brief Sleep function.
 *
 * @description Functions for sleep mode chip.
 */
void mikroe_pca9685_sleep(void);

/**
 * @brief Set mode function.
 *
 * @param mode      One of the two modes to be set.
 * @param w_data    Data that will be written in the register.
 *
 * @return status of function.
 *
 * @description This function sets mod.
 *
 * @note
 * Options for mode 1 that can be set:
 *      Restart ( enable or disable )
 *      Clock ( internal clock or EXTCLK pin clock )
 *      Auto-Increment ( enable or disable )
 *      Mode ( normal mode or low power mod )
 *      SubAddress 1 ( uses or does not use )
 *      SubAddress 2 ( uses or does not use )
 *      SubAddress 3 ( uses or does not use )
 *      Motor all call address ( uses or does not use )
 *
 * Options for mode 2 that can be set:
 *      Output logic state ( not inverted or inverted )
 *      Outputs change ( Outputs change on STOP or ACK command )
 *      Outputs configured ( open-drain structure or totem pole structure)
 */
sl_status_t mikroe_pca9685_set_mode(uint8_t mode, uint8_t w_data);

/**
 * @brief Set position function.
 *
 * @param motor     Motor to be set.
 * @param position  Position on which the motor will be set.
 *
 * @return status of function.
 *
 * @description This function sets position.
 */
sl_status_t mikroe_pca9685_set_position(uint8_t motor, uint8_t position);

/**
 * @brief Set frequency function.
 *
 * @param freq     Motor to be set.
 *
 * @description This function sets frequency.
 */
void mikroe_pca9685_set_freq(uint16_t freq);

/**
 * @brief Functions for reading adc value of current.
 *
 * @param adc_value Store ADC value.
 * @param channel  The channel from which it is read.
 *
 * @return status of function.
 *
 * @description This function reads adc value of current.
 */
sl_status_t mikroe_pca9685_get_channel(uint8_t channel, uint32_t *adc_value);

/**
 * @brief Functions for reading current in mA.
 *
 * @param channel  The channel from which it is read.
 * @param current_ma Store the current consumption in miliAmpe.
 *
 * @description This function reads the current value of Servo Click witch motor
 *   spends.
 */
sl_status_t mikroe_pca9685_get_current(uint8_t channel, uint16_t *current_ma);

#ifdef __cplusplus
}
#endif
#endif /* MIKROE_PCA9685_H_ */
