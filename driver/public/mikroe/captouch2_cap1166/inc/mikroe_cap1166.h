/***************************************************************************//**
 * @file mikroe_cap1166.h
 * @brief SCL CAP1166 Prototypes
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

#ifndef MIKROE_CAP1166_H_
#define MIKROE_CAP1166_H_

#include "sl_status.h"
#include "captouch2.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup cap1166 CAP1166 - CAP TOUCH 2 CLICK
 * @brief  Driver for the Mikroe CAP TOUCH 2 CLICK
 *
 *    @n @section cap1166_example CAP1166 example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "sl_spidrv_instances.h"
 * #include "mikroe_cap1166.h"
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize sensor and set mode
 *      mikroe_cap1166_init(sl_spidrv_mikroe_handle);
 *      mikroe_cap1166_reset();
 *      mikroe_cap1166_default_cfg();
 *
 *      uint8_t sensor_results[6];
 *
 *      while (true) {
 *
 *          mikroe_cap1166_detect_touch(&sensor_results[0]);
 *
 *          ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup cap1166_registers Register Addresses
 * @brief Register definitions
 * @{
 ******************************************************************************/
#define MIKROE_CAPTOUCH2_MAIN_CONTROL_REG                   0x00
#define MIKROE_CAPTOUCH2_GEN_STATUS_REG                     0x02
#define MIKROE_CAPTOUCH2_SENS_IN_STATUS_REG                 0x03
#define MIKROE_CAPTOUCH2_NOISE_FLAG_REG                     0x0A
#define MIKROE_CAPTOUCH2_SENS_IN1_DELTA_CNT_REG             0x10
#define MIKROE_CAPTOUCH2_SENS_IN2_DELTA_CNT_REG             0x11
#define MIKROE_CAPTOUCH2_SENS_IN3_DELTA_CNT_REG             0x12
#define MIKROE_CAPTOUCH2_SENS_IN4_DELTA_CNT_REG             0x13
#define MIKROE_CAPTOUCH2_SENS_IN5_DELTA_CNT_REG             0x14
#define MIKROE_CAPTOUCH2_SENS_IN6_DELTA_CNT_REG             0x15
#define MIKROE_CAPTOUCH2_SENSITIVITY_CON_REG                0x1F
#define MIKROE_CAPTOUCH2_CONFIG_REG                         0x20
#define MIKROE_CAPTOUCH2_SENS_IN_EN_REG                     0x21
#define MIKROE_CAPTOUCH2_SENS_IN_CONFIG_REG                 0x22
#define MIKROE_CAPTOUCH2_SENS_IN_CONFIG2_REG                0x23
#define MIKROE_CAPTOUCH2_AVRG_AND_SAMPL_CONFIG_REG          0x24
#define MIKROE_CAPTOUCH2_CALIB_ACTIVATE_REG                 0x26
#define MIKROE_CAPTOUCH2_INTERR_EN_REG                      0x27
#define MIKROE_CAPTOUCH2_REPEAT_RATE_EN_REG                 0x28
#define MIKROE_CAPTOUCH2_MULTIPLE_TOUCH_CONFIG_REG          0x2A
#define MIKROE_CAPTOUCH2_MULTIPLE_TOUCH_PATTERN_CONFIG_REG  0x2B
#define MIKROE_CAPTOUCH2_MULTIPLE_TOUCH_PATTERN_REG         0x2D
#define MIKROE_CAPTOUCH2_RECALIB_CONFIG_REG                 0x2F
#define MIKROE_CAPTOUCH2_SENS_IN1_THRESHOLD_REG             0x30
#define MIKROE_CAPTOUCH2_SENS_IN2_THRESHOLD_REG             0x31
#define MIKROE_CAPTOUCH2_SENS_IN3_THRESHOLD_REG             0x32
#define MIKROE_CAPTOUCH2_SENS_IN4_THRESHOLD_REG             0x33
#define MIKROE_CAPTOUCH2_SENS_IN5_THRESHOLD_REG             0x34
#define MIKROE_CAPTOUCH2_SENS_IN6_THRESHOLD_REG             0x35
#define MIKROE_CAPTOUCH2_SENS_IN_NOISE_THRESHOLD_REG        0x38
#define MIKROE_CAPTOUCH2_STANDBY_CHANN_REG                  0x40
#define MIKROE_CAPTOUCH2_STANDBY_CONFIG_REG                 0x41
#define MIKROE_CAPTOUCH2_STANDBY_SENSITIVITY_REG            0x42
#define MIKROE_CAPTOUCH2_STANDBY_THRESHOLD_REG              0x43
#define MIKROE_CAPTOUCH2_CONFIG2_REG                        0x44
#define MIKROE_CAPTOUCH2_SENS_IN1_BASE_CNT_REG              0x50
#define MIKROE_CAPTOUCH2_SENS_IN2_BASE_CNT_REG              0x51
#define MIKROE_CAPTOUCH2_SENS_IN3_BASE_CNT_REG              0x52
#define MIKROE_CAPTOUCH2_SENS_IN4_BASE_CNT_REG              0x53
#define MIKROE_CAPTOUCH2_SENS_IN5_BASE_CNT_REG              0x54
#define MIKROE_CAPTOUCH2_SENS_IN6_BASE_CNT_REG              0x55
#define MIKROE_CAPTOUCH2_LED_OUTPUT_TYPE_REG                0x71
#define MIKROE_CAPTOUCH2_SENS_IN_LED_LINK_REG               0x72
#define MIKROE_CAPTOUCH2_LED_POLARITY_REG                   0x73
#define MIKROE_CAPTOUCH2_LED_OUT_CON_REG                    0x74
#define MIKROE_CAPTOUCH2_LED_LINK_TRANS_CON_REG             0x77
#define MIKROE_CAPTOUCH2_LED_MIRROR_CON_REG                 0x79
#define MIKROE_CAPTOUCH2_LED_BEHAVIOR1_REG                  0x81
#define MIKROE_CAPTOUCH2_LED_BEHAVIOR2_REG                  0x82
#define MIKROE_CAPTOUCH2_LED_PULSE1_PERIOD_REG              0x84
#define MIKROE_CAPTOUCH2_LED_PULSE2_PERIOD_REG              0x85
#define MIKROE_CAPTOUCH2_LED_BREATHE_PERIOD_REG             0x86
#define MIKROE_CAPTOUCH2_LED_CONFIG_REG                     0x88
#define MIKROE_CAPTOUCH2_LED_PULSE1_DUTY_REG                0x90
#define MIKROE_CAPTOUCH2_LED_PULSE2_DUTY_REG                0x91
#define MIKROE_CAPTOUCH2_LED_BREATHE_DUTY_REG               0x92
#define MIKROE_CAPTOUCH2_LED_DIRECT_DUTY_REG                0x93
#define MIKROE_CAPTOUCH2_LED_DIRECT_RAMP_RATES_REG          0x94
#define MIKROE_CAPTOUCH2_LED_OFF_DELAY_REG                  0x95
#define MIKROE_CAPTOUCH2_SENS_IN1_CALIB_REG                 0xB1
#define MIKROE_CAPTOUCH2_SENS_IN2_CALIB_REG                 0xB2
#define MIKROE_CAPTOUCH2_SENS_IN3_CALIB_REG                 0xB3
#define MIKROE_CAPTOUCH2_SENS_IN4_CALIB_REG                 0xB4
#define MIKROE_CAPTOUCH2_SENS_IN5_CALIB_REG                 0xB5
#define MIKROE_CAPTOUCH2_SENS_IN6_CALIB_REG                 0xB6
#define MIKROE_CAPTOUCH2_SENS_IN_CALIB_LSB1_REG             0xB9
#define MIKROE_CAPTOUCH2_SENS_IN_CALIB_LSB2_REG             0xBA
#define MIKROE_CAPTOUCH2_PRODUCT_ID_REG                     0xFD
#define MIKROE_CAPTOUCH2_MANUFACT_ID_REG                    0xFE
#define MIKROE_CAPTOUCH2_REVISION_REG                       0xFF

/** @} (end addtogroup cap1166_registers) */

/***************************************************************************//**
 * @brief
 *    Click Default Configuration function.
 *
 * @note
 * <pre>
 *     Cap Touch 2 is configured to works in Active mode (Normal mode).
 *     All inputs are enabled in this mode.
 *     In this example the interrupt will be generated when touch is detected
 *     and when touch is released. Also inputs 4, 5 and 6 will generate
 *     interrupt as long as the touch is detected (press and hold event), while
 *     inputs 1, 2 and 3 will generate interrupt only once on one touch
 *     detection, after which the touch will be released.
 *
 *     Note: Standby mode should be used when fewer sensor inputs are enabled,
 *     and when they are programmed to have more sensitivity.
 * </pre>
 ******************************************************************************/
void mikroe_cap1166_default_cfg(void);

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL if error spi communication.
 ******************************************************************************/
sl_status_t mikroe_cap1166_init(mikroe_spi_handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the SPI instance used by platform functions
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors
 *    SL_STATUS_INVALID_PARAMETER if spi_instance is null
 ******************************************************************************/
sl_status_t mikroe_cap1166_set_spi_instance(mikroe_spi_handle_t spi_instance);

/**
 * @brief Generic Read function
 *
 * @param ctx            Click object.
 * @param reg_addr         Address which from data be read
 * @param rx_data          Buffer where data be stored
 * @param n_bytes          Number of bytes which be read
 *
 * @description Function reads data from register.
 */

/***************************************************************************//**
 * @brief
 *    Generic Read function
 *
 * @param[in] reg_addr
 *    Address which from data be read
 * @param[in] n_bytes
 *    Number of bytes which be read
 *
 * @param[out] rx_data
 *    Buffer where data be stored
 *
 * @return
 *    SL_STATUS_OK if there are no errors, SL_STATUS_FAIL otherwise
 *
 ******************************************************************************/
sl_status_t mikroe_cap1166_read_reg(uint8_t reg_addr,
                                    uint8_t *rx_data,
                                    uint8_t n_bytes);

/***************************************************************************//**
 * @brief
 *    Generic Write function
 *
 * @param[in] reg_addr
 *    Address where data be written
 * @param[in] tx_data
 *    Data which be written
 *
 ******************************************************************************/
void mikroe_cap1166_write_reg(uint8_t reg_addr, uint8_t tx_data);

/***************************************************************************//**
 * @brief
 *    Generic transfer function
 *
 * @param[in] wr_buf
 *    Write data buffer
 * @param[in] wr_len
 *    Number of byte in write data buffer
 * @param[in] rd_len
 *    Number of byte in read data buffer
 *
 * @param[out] rd_buf
 *    Read data buffer
 *
 * @return
 *    SL_STATUS_OK if there are no errors, SL_STATUS_INVALID_PARAMETER if wr_buf
 *    and rd_buf are NULL pointer
 *
 ******************************************************************************/
sl_status_t mikroe_cap1166_generic_transfer(uint8_t *wr_buf,
                                            uint16_t wr_len,
                                            uint8_t *rd_buf,
                                            uint16_t rd_len);

/***************************************************************************//**
 * @brief
 *    Touch Detect function
 *
 * @param[out] in_sens
 *    Results of detecting touch on sensor inputs
 *
 * @return
 *    SL_STATUS_OK if there are no errors, SL_STATUS_INVALID_PARAMETER if
 *    in_sens is NULL pointer
 *
 * @note
 * <pre>
 *     Function detects touch on sensor inputs and checks is touch detected or
 *     is touch released.
 *     If noise occured on determined input, the touch will not be detected.
 * </pre>
 *
 ******************************************************************************/
sl_status_t mikroe_cap1166_detect_touch(uint8_t *in_sens);

/***************************************************************************//**
 * @brief
 *    Active Mode Set function
 *
 * @param[in] analog_gain
 *    Possible gain value is 1, 2, 4 or 8
 * @param[in] en_input
 *    Enable or disable sensor inputs
 *
 ******************************************************************************/
void mikroe_cap1166_set_active_mode(uint8_t analog_gain, uint8_t en_input);

/***************************************************************************//**
 * @brief
 *    Standby Mode Set function
 *
 * @param[in] analog_gain
 *    Possible gain value is 1, 2, 4 or 8
 * @param[in] en_input
 *    Enable or disable sensor inputs
 *
 ******************************************************************************/
void mikroe_cap1166_set_standby_mode(uint8_t analog_gain, uint8_t en_input);

/***************************************************************************//**
 * @brief
 *    DeepSleep Mode Set function
 *
 ******************************************************************************/
void mikroe_cap1166_set_deep_sleep_mode(void);

/***************************************************************************//**
 * @brief
 *    Interrupt Check function
 *
 * @return State of ALERT pin, 0 or 1
 ******************************************************************************/
uint8_t mikroe_cap1166_check_interrupt(void);

/***************************************************************************//**
 * @brief
 *    Reset function
 *
 ******************************************************************************/
void mikroe_cap1166_reset(void);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup cap1166) */

#endif /* MIKROE_CAP1166_H_ */
