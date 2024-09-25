/***************************************************************************//**
 * @file mikroe_maxm86161.h
 * @brief SCL MAXM86161 Prototypes
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

#ifndef MIKROE_MAXM86161_H_
#define MIKROE_MAXM86161_H_

#include "sl_status.h"
#include "drv_i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup mikroe_maxm86161 MAXM86161 - HEARTRATE2 Click
 * @brief  The integration of the Mikroe SDK Click extension MAXM86161 driver.
 *         (HEARTRATE 2 Click board.)
 *
 *    @n @section maxm86161_example MAXM86161 example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "sl_i2cspm_instances.h"
 * #include "mikroe_maxm86161.h"
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize sensor and set mode
 *      mikroe_maxm86161_init(sl_i2cspm_mikroe);
 *      mikroe_maxm86161_set_en(MAXM86161_PIN_HIGH);
 *      mikroe_maxm86161_soft_reset();
 *      mikroe_maxm86161_default_cfg(MAXM86161_CONFIG_GREEN)
 *
 *      mikroe_maxm86161_fifo_data_t fifo_object;
 *
 *      while (true) {
 *
 *        mikroe_maxm86161_read_fifo(&fifo_object);
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/**
 * \defgroup maxm86161_macros Macros
 * \{
 * \defgroup device_register_address Device register address
 * \{
 */
#define MIKROE_MAXM86161_REG_INT_STATUS_1                         0x00
#define MIKROE_MAXM86161_REG_INT_STATUS_2                         0x01
#define MIKROE_MAXM86161_REG_INT_ENABLE_1                         0x02
#define MIKROE_MAXM86161_REG_INT_ENABLE_2                         0x03
#define MIKROE_MAXM86161_REG_FIFO_WRITE_PTR                       0x04
#define MIKROE_MAXM86161_REG_FIFO_READ_PTR                        0x05
#define MIKROE_MAXM86161_REG_OVERFLOW_CNT                         0x06
#define MIKROE_MAXM86161_REG_FIFO_DATA_CNT                        0x07
#define MIKROE_MAXM86161_REG_FIFO_DATA_REG                        0x08
#define MIKROE_MAXM86161_REG_FIFO_CFG_1                           0x09
#define MIKROE_MAXM86161_REG_FIFO_CFG_2                           0x0A
#define MIKROE_MAXM86161_REG_SYSTEM_CTRL                          0x0D
#define MIKROE_MAXM86161_REG_PPG_SYNC_CTRL                        0x10
#define MIKROE_MAXM86161_REG_PPG_CFG_1                            0x11
#define MIKROE_MAXM86161_REG_PPG_CFG_2                            0x12
#define MIKROE_MAXM86161_REG_PPG_CFG_3                            0x13
#define MIKROE_MAXM86161_REG_PROX_INT_THRESHOLD                   0x14
#define MIKROE_MAXM86161_REG_PHOTO_DIODE_BIAS                     0x15
#define MIKROE_MAXM86161_REG_PICKET_FENCE                         0x16
#define MIKROE_MAXM86161_REG_LED_SQUENCE_REG_1                    0x20
#define MIKROE_MAXM86161_REG_LED_SQUENCE_REG_2                    0x21
#define MIKROE_MAXM86161_REG_LED_SQUENCE_REG_3                    0x22
#define MIKROE_MAXM86161_REG_LED_1_PA                             0x23
#define MIKROE_MAXM86161_REG_LED_2_PA                             0x24
#define MIKROE_MAXM86161_REG_LED_3_PA                             0x25
#define MIKROE_MAXM86161_REG_LED_PILOT_PA                         0x29
#define MIKROE_MAXM86161_REG_LED_RANGE_1                          0x2A
#define MIKROE_MAXM86161_REG_S1_HI_RES_DAC_1                      0x2C
#define MIKROE_MAXM86161_REG_S2_HI_RES_DAC_1                      0x2D
#define MIKROE_MAXM86161_REG_S3_HI_RES_DAC_1                      0x2E
#define MIKROE_MAXM86161_REG_S4_HI_RES_DAC_1                      0x2F
#define MIKROE_MAXM86161_REG_S5_HI_RES_DAC_1                      0x30
#define MIKROE_MAXM86161_REG_S6_HI_RES_DAC_1                      0x31
#define MIKROE_MAXM86161_REG_DIE_TEMP_CFG                         0x40
#define MIKROE_MAXM86161_REG_DIE_TEMP_INT                         0x41
#define MIKROE_MAXM86161_REG_DIE_TEMP_FRAC                        0x42
#define MIKROE_MAXM86161_REG_DAC_CALIBRATION_EN                   0x50
#define MIKROE_MAXM86161_REG_SHA_CMD                              0xF0
#define MIKROE_MAXM86161_REG_SHA_CFG                              0xF1
#define MIKROE_MAXM86161_REG_MEM_CTRL                             0xF2
#define MIKROE_MAXM86161_REG_MEM_INDEX                            0xF3
#define MIKROE_MAXM86161_REG_MEM_DATA                             0xF4
#define MIKROE_MAXM86161_REG_REV_ID                               0xFE
#define MIKROE_MAXM86161_REG_PART_ID                              0xFF

/** \} */

/**
 * \defgroup system_control_values System control values
 * \{
 */
#define MIKROE_MAXM86161_SYS_CTRL_SW_RESET                        0x01
#define MIKROE_MAXM86161_SYS_CTRL_SHUT_DOWN                       0x02
#define MIKROE_MAXM86161_SYS_CTRL_POWER_ON                        0x00
#define MIKROE_MAXM86161_SYS_CTRL_LOW_PWR_MODE                    0x04
#define MIKROE_MAXM86161_SYS_CTRL_SINGLE_PPG                      0x08
#define MIKROE_MAXM86161_SYS_CTRL_DUAL_PPG                        0x00

/** \} */

/**
 * \defgroup ppg_configuration_1_values PPG configuration 1 values
 * \{
 */
#define MIKROE_MAXM86161_PPG_CFG_1_ALC_ENABLE                     0x00
#define MIKROE_MAXM86161_PPG_CFG_1_ALC_DISABLE                    0x80
#define MIKROE_MAXM86161_PPG_CFG_1_ADD_OFFSET                     0x40
#define MIKROE_MAXM86161_PPG_CFG_1_NO_OFFSET                      0x00
#define MIKROE_MAXM86161_PPG_CFG_1_ADC_RANGE_4096                 0x00
#define MIKROE_MAXM86161_PPG_CFG_1_ADC_RANGE_8192                 0x04
#define MIKROE_MAXM86161_PPG_CFG_1_ADC_RANGE_16384                0x08
#define MIKROE_MAXM86161_PPG_CFG_1_ADC_RANGE_32768                0x0C
#define MIKROE_MAXM86161_PPG_CFG_1_PPG_TINT_21p3                  0x00
#define MIKROE_MAXM86161_PPG_CFG_1_PPG_TINT_35p9                  0x01
#define MIKROE_MAXM86161_PPG_CFG_1_PPG_TINT_65p2                  0x02
#define MIKROE_MAXM86161_PPG_CFG_1_PPG_TINT_123p8                 0x03

/** \} */

/**
 * \defgroup ppg_configuration_2_values PPG configuration 2 values
 * \{
 */
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_24p995_N1             0x00
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_50p027_N1             0x08
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_84p021_N1             0x10
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_99p902_N1             0x18
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_199p805_N1            0x20
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_399p610_N1            0x28
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_24p995_N2             0x30
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_50p027_N2             0x38
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_84p021_N2             0x40
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_99p902_N2             0x48
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_8p000_N1              0x50
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_16p000_N1             0x58
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_32p000_N1             0x60
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_64p000_N1             0x68
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_128p000_N1            0x70
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_256p000_N1            0x78
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_512p000_N1            0x80
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_1024p000_N1           0x88
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_2048p000_N1           0x90
#define MIKROE_MAXM86161_PPG_CFG_2_CLK_FREQ_4096p000_N1           0x98
#define MIKROE_MAXM86161_PPG_CFG_2_SMP_AVG_1                      0x00
#define MIKROE_MAXM86161_PPG_CFG_2_SMP_AVG_2                      0x01
#define MIKROE_MAXM86161_PPG_CFG_2_SMP_AVG_4                      0x02
#define MIKROE_MAXM86161_PPG_CFG_2_SMP_AVG_8                      0x03
#define MIKROE_MAXM86161_PPG_CFG_2_SMP_AVG_16                     0x04
#define MIKROE_MAXM86161_PPG_CFG_2_SMP_AVG_32                     0x05
#define MIKROE_MAXM86161_PPG_CFG_2_SMP_AVG_64                     0x06
#define MIKROE_MAXM86161_PPG_CFG_2_SMP_AVG_128                    0x07

/** \} */

/**
 * \defgroup ppg_configuration_3_values PPG configuration 3 values
 * \{
 */
#define MIKROE_MAXM86161_PPG_CFG_3_LED_SETLNG_4                   0x00
#define MIKROE_MAXM86161_PPG_CFG_3_LED_SETLNG_6                   0x40
#define MIKROE_MAXM86161_PPG_CFG_3_LED_SETLNG_8                   0x80
#define MIKROE_MAXM86161_PPG_CFG_3_LED_SETLNG_12                  0xC0
#define MIKROE_MAXM86161_PPG_CFG_3_FILTER_CDM                     0x00
#define MIKROE_MAXM86161_PPG_CFG_3_FILTER_FDM                     0x20
#define MIKROE_MAXM86161_PPG_CFG_3_BURST_RATE_8HZ                 0x00
#define MIKROE_MAXM86161_PPG_CFG_3_BURST_RATE_32HZ                0x02
#define MIKROE_MAXM86161_PPG_CFG_3_BURST_RATE_84HZ                0x04
#define MIKROE_MAXM86161_PPG_CFG_3_BURST_RATE_256HZ               0x06
#define MIKROE_MAXM86161_PPG_CFG_3_BURST_ENABLE                   0x01
#define MIKROE_MAXM86161_PPG_CFG_3_BURST_DISABLE                  0x00

/** \} */

/**
 * \defgroup photo_diode_values Photo diode values
 * \{
 */
#define MIKROE_MAXM86161_PHOTO_DIODE_BIAS1_0_65_PF                0x01
#define MIKROE_MAXM86161_PHOTO_DIODE_BIAS1_65_130_PF              0x05
#define MIKROE_MAXM86161_PHOTO_DIODE_BIAS1_130_260_PF             0x06
#define MIKROE_MAXM86161_PHOTO_DIODE_BIAS1_260_520_PF             0x07
#define MIKROE_MAXM86161_PHOTO_DIODE_BIAS2_0_65_PF                0x10
#define MIKROE_MAXM86161_PHOTO_DIODE_BIAS2_65_130_PF              0x50
#define MIKROE_MAXM86161_PHOTO_DIODE_BIAS2_130_260_PF             0x60
#define MIKROE_MAXM86161_PHOTO_DIODE_BIAS2_260_520_PF             0x70

/** \} */

/**
 * \defgroup led_range_values Led range values
 * \{
 */
#define MIKROE_MAXM86161_LED_RANGE1_LED1_CURRENT_31_MA            0x00
#define MIKROE_MAXM86161_LED_RANGE1_LED1_CURRENT_62_MA            0x01
#define MIKROE_MAXM86161_LED_RANGE1_LED1_CURRENT_93_MA            0x02
#define MIKROE_MAXM86161_LED_RANGE1_LED1_CURRENT_124_MA           0x03
#define MIKROE_MAXM86161_LED_RANGE1_LED2_CURRENT_31_MA            0x00
#define MIKROE_MAXM86161_LED_RANGE1_LED2_CURRENT_62_MA            0x04
#define MIKROE_MAXM86161_LED_RANGE1_LED2_CURRENT_93_MA            0x08
#define MIKROE_MAXM86161_LED_RANGE1_LED2_CURRENT_124_MA           0x0C
#define MIKROE_MAXM86161_LED_RANGE1_LED3_CURRENT_31_MA            0x00
#define MIKROE_MAXM86161_LED_RANGE1_LED3_CURRENT_62_MA            0x10
#define MIKROE_MAXM86161_LED_RANGE1_LED3_CURRENT_93_MA            0x20
#define MIKROE_MAXM86161_LED_RANGE1_LED3_CURRENT_124_MA           0x30

/** \} */

/**
 * \defgroup fifo_configuration_2_values Fifo configuration 2 values
 * \{
 */
#define MIKROE_MAXM86161_FIFO_CFG_2_FLUSH_FIFO                    0x10
#define MIKROE_MAXM86161_FIFO_CFG_2_FIFO_READ_DATA_CLR            0x08
#define MIKROE_MAXM86161_FIFO_CFG_2_FIFO_STAT_DATA_CLR            0x00
#define MIKROE_MAXM86161_FIFO_CFG_2_FULL_TYPE_RPT                 0x00
#define MIKROE_MAXM86161_FIFO_CFG_2_FULL_TYPE_ONCE                0x04
#define MIKROE_MAXM86161_FIFO_CFG_2_FIFO_ROLL_OVER                0x02
#define MIKROE_MAXM86161_FIFO_CFG_2_FIFO_STOP                     0x00

/** \} */

/**
 * \defgroup interrupt_values Interrupt values
 * \{
 */
#define MIKROE_MAXM86161_INT_1_FULL                               0x80
#define MIKROE_MAXM86161_INT_1_NOT_FULL                           0x00
#define MIKROE_MAXM86161_INT_1_DATA_RDY                           0x40
#define MIKROE_MAXM86161_INT_1_DATA_NOT_RDY                       0x00
#define MIKROE_MAXM86161_INT_1_ALC_OVERFLOW                       0x20
#define MIKROE_MAXM86161_INT_1_ALC_NORMAL                         0x00
#define MIKROE_MAXM86161_INT_1_PROXIMITY_INT                      0x10
#define MIKROE_MAXM86161_INT_1_PROXIMITY_NORMAL                   0x00
#define MIKROE_MAXM86161_INT_1_LED_COMPLIANT                      0x08
#define MIKROE_MAXM86161_INT_1_LED_NOT_COMPLIANT                  0x00
#define MIKROE_MAXM86161_INT_1_DIE_TEMP_RDY                       0x04
#define MIKROE_MAXM86161_INT_1_DIE_TEMP_NOT_RDY                   0x00
#define MIKROE_MAXM86161_INT_1_PWR_RDY                            0x01
#define MIKROE_MAXM86161_INT_1_PWR_NOT_RDY                        0x00
#define MIKROE_MAXM86161_INT_2_SHA_AUTHENTICATION_DONE            0x01
#define MIKROE_MAXM86161_INT_2_SHA_AUTHENTICATION_NOT_DONE        0x00

/** \} */

/**
 * \defgroup fifo_tag_stucture_values Fifo tag stucture values
 * \{
 */
#define MIKROE_MAXM86161_FIFO_TAG_PPG1_LEDC1                      0x01
#define MIKROE_MAXM86161_FIFO_TAG_PPG1_LEDC2                      0x02
#define MIKROE_MAXM86161_FIFO_TAG_PPG1_LEDC3                      0x03
#define MIKROE_MAXM86161_FIFO_TAG_PPG1_LEDC4                      0x04
#define MIKROE_MAXM86161_FIFO_TAG_PPG1_LEDC5                      0x05
#define MIKROE_MAXM86161_FIFO_TAG_PPG1_LEDC6                      0x06
#define MIKROE_MAXM86161_FIFO_TAG_PPF1_LEDC1                      0x0D
#define MIKROE_MAXM86161_FIFO_TAG_PPF1_LEDC2                      0x0E
#define MIKROE_MAXM86161_FIFO_TAG_PPF1_LEDC3                      0x0F
#define MIKROE_MAXM86161_FIFO_TAG_PROX1                           0x19
#define MIKROE_MAXM86161_FIFO_TAG_SUB_DAC_UPDATE                  0x1D
#define MIKROE_MAXM86161_FIFO_TAG_INVALID_DATA                    0x1E
#define MIKROE_MAXM86161_FIFO_TAG_TIME_STAMP                      0x1F

/** \} */

/**
 * \defgroup led_sequence_stucture_values Led sequence structure values
 * \{
 */
#define MIKROE_MAXM86161_LEDSQ_GREEN                              0x1
#define MIKROE_MAXM86161_LEDSQ_IR                                 0x2
#define MIKROE_MAXM86161_LEDSQ_RED                                0x3
#define MIKROE_MAXM86161_LEDSQ_PILOT_LED1                         0x8
#define MIKROE_MAXM86161_LEDSQ_DIRECT_AMBIENT                     0x9
#define MIKROE_MAXM86161_LEDSQ_OFF                                0x0

/** \} */

/**
 * \defgroup device_status_values Device status values
 * \{
 */
#define MIKROE_MAXM86161_DEV_ID                                   0x36

/** \} */

/**
 * \defgroup int_stucture_values Int structure values
 * \{
 */
#define MIKROE_MAXM86161_INT_ENABLE                               0x01
#define MIKROE_MAXM86161_INT_DISABLE                              0x00

/** \} */

/**
 * \defgroup ppg_configuration_alc PPG configuration Alc
 * \{
 */
#define MIKROE_MAXM86161_PPG_CFG_ALC_EN                           0x01
#define MIKROE_MAXM86161_PPG_CFG_ALC_DS                           0x00

/** \} */

/**
 * \defgroup ppg_configuration_offset PPG configuration Offset
 * \{
 */
#define MIKROE_MAXM86161_PPG_CFG_OFFSET_ADD                       0x01
#define MIKROE_MAXM86161_PPG_CFG_OFFSET_NO                        0x00

/** \} */

/**
 * \defgroup ppg_configuration_tint PPG configuration Tint
 * \{
 */
#define MIKROE_MAXM86161_PPG_CFG_TINT_14p8_US                     0x00
#define MIKROE_MAXM86161_PPG_CFG_TINT_29p4_US                     0x01
#define MIKROE_MAXM86161_PPG_CFG_TINT_58p7_US                     0x02
#define MIKROE_MAXM86161_PPG_CFG_TINT_117p3_US                    0x03

/** \} */

/**
 * \defgroup ppg_configuration_led_range PPG configuration Led Range
 * \{
 */
#define MIKROE_MAXM86161_PPG_CFG_LED_RANGE_4k                     0x00
#define MIKROE_MAXM86161_PPG_CFG_LED_RANGE_8k                     0x01
#define MIKROE_MAXM86161_PPG_CFG_LED_RANGE_16k                    0x02
#define MIKROE_MAXM86161_PPG_CFG_LED_RANGE_32k                    0x03

/** \} */

/**
 * \defgroup ppg_configuration_smp_rate_p1 PPG configuration Smp Rate P1
 * \{
 */
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_24sps                0x00
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_50sps                0x01
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_84sps                0x02
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_99sps                0x03
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_199sps               0x04
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_399sps               0x05
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P2_24sps                0x06
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P2_50sps                0x07
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P2_84sps                0x08
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P2_99sps                0x09
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_8sps                 0x0A
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_16sps                0x0B
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_32sps                0x0C
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_64sps                0x0D
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_128sps               0x0E
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_256sps               0x0F
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_512sps               0x10
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_1024sps              0x11
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_2048sps              0x12
#define MIKROE_MAXM86161_PPG_CFG_SMP_RATE_P1_4096sps              0x13

/** \} */

/**
 * \defgroup ppg_configuration_smp_avg PPG configuration Smp Avg
 * \{
 */
#define MIKROE_MAXM86161_PPG_CFG_SMP_AVG_1                        0x00
#define MIKROE_MAXM86161_PPG_CFG_SMP_AVG_2                        0x01
#define MIKROE_MAXM86161_PPG_CFG_SMP_AVG_4                        0x02
#define MIKROE_MAXM86161_PPG_CFG_SMP_AVG_8                        0x03
#define MIKROE_MAXM86161_PPG_CFG_SMP_AVG_16                       0x04
#define MIKROE_MAXM86161_PPG_CFG_SMP_AVG_32                       0x05
#define MIKROE_MAXM86161_PPG_CFG_SMP_AVG_64                       0x06
#define MIKROE_MAXM86161_PPG_CFG_SMP_AVG_128                      0x07

/** \} */

/**
 * \defgroup led_range_current Led Range Current
 * \{
 */
#define MIKROE_MAXM86161_LED_RANGE_CURRENT_31_MA                  0x00
#define MIKROE_MAXM86161_LED_RANGE_CURRENT_62_MA                  0x01
#define MIKROE_MAXM86161_LED_RANGE_CURRENT_93_MA                  0x02
#define MIKROE_MAXM86161_LED_RANGE_CURRENT_124_MA                 0x03

/** \} */

/**
 * \defgroup maxm86161_typedefs Types
 * \{
 */

/***************************************************************************//**
 * @brief
 *    Typedef for config led
 ******************************************************************************/
typedef enum {
  MAXM86161_CONFIG_GREEN,
  MAXM86161_CONFIG_IR,
  MAXM86161_CONFIG_RED
} mikroe_maxm86161_config_led_t;

/***************************************************************************//**
 * @brief
 *    Typedef for specifying state pin
 ******************************************************************************/
typedef enum {
  MAXM86161_PIN_HIGH,
  MAXM86161_PIN_LOW
} mikroe_state_pin_t;

/***************************************************************************//**
 * @brief
 *    Structure for reading fifo data
 ******************************************************************************/
typedef struct
{
  uint32_t data_val;
  uint8_t  tag;
} mikroe_maxm86161_fifo_data_t;

/***************************************************************************//**
 * @brief
 *    Structure configurating ppg
 ******************************************************************************/
typedef struct
{
  uint8_t alc;
  uint8_t offset;
  uint8_t ppg_tint;
  uint8_t led_range;
  uint8_t smp_rate;
  uint8_t smp_freq;
} mikroe_maxm86161_ppg_cfg_t;

/***************************************************************************//**
 * @brief
 *    Structure led brightness
 ******************************************************************************/
typedef struct
{
  uint8_t green;
  uint8_t ir;
  uint8_t red;
} mikroe_maxm86161_ledpa_t;

/***************************************************************************//**
 * @brief
 *    Structure setting led current range
 ******************************************************************************/
typedef struct
{
  uint8_t green;
  uint8_t ir;
  uint8_t red;
} mikroe_maxm86161_led_range_curr_t;

/***************************************************************************//**
 * @brief
 *    Structure configurating led sequence
 ******************************************************************************/
typedef struct
{
  uint8_t ledsq1;
  uint8_t ledsq2;
  uint8_t ledsq3;
  uint8_t ledsq4;
  uint8_t ledsq5;
  uint8_t ledsq6;
} mikroe_maxm86161_ledsq_cfg_t;

/***************************************************************************//**
 * @brief
 *    Structure for interrupt status
 ******************************************************************************/
typedef struct
{
  uint8_t full_fifo;
  uint8_t data_rdy;
  uint8_t alc_ovf;
  uint8_t proxy;
  uint8_t led_compliant;
  uint8_t die_temp;
  uint8_t pwr_rdy;
  uint8_t sha;
} mikroe_maxm86161_int_t;

/** \} */

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
sl_status_t mikroe_maxm86161_init(mikroe_i2c_handle_t i2cspm_instance);

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
sl_status_t mikroe_maxm86161_set_i2csmp_instance(
  mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function executes default configuration for HeartRate2 click
 *
 * @param[in] cfg_led
 *    Select one of led enum to enable and set configuration for selected led
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER
 ******************************************************************************/
sl_status_t mikroe_maxm86161_default_cfg(mikroe_maxm86161_config_led_t cfg_led);

/***************************************************************************//**
 * @brief
 *    This function writes data to the desired register.
 *
 * @param[in] register_address
 *    Register address
 *
 * @param[in] write_data
 *    Data to be written.
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_maxm86161_generic_write(
  uint8_t register_address,
  uint8_t write_data);

/***************************************************************************//**
 * @brief
 *    This function reads data from the desired register.
 *
 * @param[in] register_address
 *    Register address
 *
 * @return
 *    Data is read from register
 ******************************************************************************/
uint8_t mikroe_maxm86161_generic_read(uint8_t register_address);

/***************************************************************************//**
 * @brief
 *    This function reads data buffer from the desired register.
 *
 * @param[in] reg
 *    Register address
 *
 * @param[in] data_buf
 *    Read buffer to store data.
 *
 * @param[in] len
 *    Size of read buffer.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if data_buf is NULL
 ******************************************************************************/
sl_status_t mikroe_maxm86161_read_buf(
  uint8_t reg,
  uint8_t *data_buf,
  uint8_t len);

/***************************************************************************//**
 * @brief
 *    Function for reading data from fifo register.
 *
 * @param[in] mikroe_fifo
 *    Pointer to fifo register.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if mikroe_fifo is NULL
 ******************************************************************************/
sl_status_t mikroe_maxm86161_read_fifo(
  mikroe_maxm86161_fifo_data_t *mikroe_fifo);

/***************************************************************************//**
 * @brief
 *    Function for reading temperature of device.
 *
 * @return
 *    Float temperature of the device
 *
 * @note
 *    You should enable die temperature interrupt status,
 *    and you should turn off leds to get right temperature.
 ******************************************************************************/
float mikroe_maxm86161_read_temp(void);

/***************************************************************************//**
 * @brief
 *    This function restarts device.
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_maxm86161_soft_reset(void);

/***************************************************************************//**
 * @brief
 *    Function for configurating ppg registers.
 *
 * @param[in] mikroe_ppg_cfg
 *    Struct with stored configuration.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if mikroe_ppg_cfg is NULL
 ******************************************************************************/
sl_status_t mikroe_maxm86161_ppg_config(
  mikroe_maxm86161_ppg_cfg_t *mikroe_ppg_cfg);

/***************************************************************************//**
 * @brief
 *    Function for configurating led pa registers.
 *
 * @param[in] mikroe_ledpa
 *    Struct with stored configuration.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if mikroe_ledpa is NULL
 ******************************************************************************/
sl_status_t mikroe_maxm86161_ledpa_config(
  mikroe_maxm86161_ledpa_t *mikroe_ledpa);

/***************************************************************************//**
 * @brief
 *    Function for configurating led current range registers.
 *
 * @param[in] mikroe_led_range
 *    Struct with stored configuration.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if mikroe_led_range is NULL
 ******************************************************************************/
sl_status_t mikroe_maxm86161_led_range_curr_config(
  mikroe_maxm86161_led_range_curr_t *mikroe_led_range);

/***************************************************************************//**
 * @brief
 *    Function for configurating led sequence registers.
 *
 * @param[in] mikroe_ledsq
 *    Struct with stored configuration.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if mikroe_ledsq is NULL
 ******************************************************************************/
sl_status_t mikroe_maxm86161_led_sequence_config(
  mikroe_maxm86161_ledsq_cfg_t *mikroe_ledsq);

/***************************************************************************//**
 * @brief
 *    Function for configurating interrupt registers.
 *
 * @param[in] mikroe_int_ctrl
 *    Struct with stored configuration.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if mikroe_int_ctrl is NULL
 ******************************************************************************/
sl_status_t mikroe_maxm86161_int_control(
  mikroe_maxm86161_int_t *mikroe_int_ctrl);

/***************************************************************************//**
 * @brief
 *    Function for reading interrupt status.
 *
 * @param[in] mikroe_int_status
 *    Struct with stored configuration.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if mikroe_int_status is NULL
 ******************************************************************************/
sl_status_t mikroe_maxm86161_int_status(
  mikroe_maxm86161_int_t *mikroe_int_status);

/***************************************************************************//**
 * @brief
 *    Function for reading int pin status.
 *
 * @return
 *    uint8_t int pin status.
 ******************************************************************************/
uint8_t mikroe_maxm86161_get_int(void);

/***************************************************************************//**
 * @brief
 *    Function for reading gp pin status.
 *
 * @return
 *    gp pin status.
 ******************************************************************************/
uint8_t mikroe_maxm86161_get_gp(void);

/***************************************************************************//**
 * @brief
 *    This function settings en pin status.
 *
 * @param[in] state
 *    State of the pin to set
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER
 ******************************************************************************/
sl_status_t mikroe_maxm86161_set_en(mikroe_state_pin_t state);

#ifdef __cplusplus
}
#endif

/** @} */
#endif // MIKROE_MAXM86161_H_
