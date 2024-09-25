/***************************************************************************//**
 * @file mikroe_uwb_dwm1000.h
 * @brief mikroe uwb dwm1000 header file
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

#ifndef _MIKROE_MIKROE_DWM1000_DWM1000_H
#define _MIKROE_MIKROE_DWM1000_DWM1000_H

#include "sl_status.h"
#include "stdint.h"
#include "drv_spi_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \} */

/**
 * \defgroup error_code Error Code
 * \{
 */
#define MIKROE_DWM1000_OK                           0
#define MIKROE_DWM1000_ERROR                        -1

/** \} */

/**
 * \defgroup no_sub_address_macro No sub address macro
 * \{
 */
#define MIKROE_DWM1000_SUB_NO                       0x00

/** \} */

/**
 * \defgroup register_address_macros Register address macros
 * \{
 */
#define MIKROE_DWM1000_REG_DEV_ID                   0x00
#define MIKROE_DWM1000_REG_EU_ID                    0x01
#define MIKROE_DWM1000_REG_PAN_ID                   0x03
#define MIKROE_DWM1000_REG_SYS_CFG                  0x04
#define MIKROE_DWM1000_REG_SYS_CNT                  0x06
#define MIKROE_DWM1000_REG_SYS_TX_CTRL              0x08
#define MIKROE_DWM1000_REG_TX_DATA_BUF              0x09
#define MIKROE_DWM1000_REG_DX_TIME                  0x0A
#define MIKROE_DWM1000_REG_RX_TIMEOUT               0x0C
#define MIKROE_DWM1000_REG_SYS_CTRL                 0x0D
#define MIKROE_DWM1000_REG_SYS_EVENT_MASK           0x0E
#define MIKROE_DWM1000_REG_EVENT_STATUS             0x0F
#define MIKROE_DWM1000_REG_RX_INFO                  0x10
#define MIKROE_DWM1000_REG_RX_BUF                   0x11
#define MIKROE_DWM1000_REG_RX_QUALITY_INFO          0x12
#define MIKROE_DWM1000_REG_RX_TIME_INTERVAL         0x13
#define MIKROE_DWM1000_REG_RX_TIME_OFFSET           0x14
#define MIKROE_DWM1000_REG_RX_MESSAGE_TOA           0x15
#define MIKROE_DWM1000_REG_TX_MESSAGE_TOS           0x17
#define MIKROE_DWM1000_REG_TX_ANTD                  0x18
#define MIKROE_DWM1000_REG_SYS_STATE                0x19
#define MIKROE_DWM1000_REG_ACK_RESPONSE_TIME        0x1A
#define MIKROE_DWM1000_REG_RX_SNIFF_CFG             0x1D
#define MIKROE_DWM1000_REG_TX_POWER                 0x1E
#define MIKROE_DWM1000_REG_CHN_CTRL                 0x1F
#define MIKROE_DWM1000_REG_SFD                      0x21
#define MIKROE_DWM1000_REG_AUTO_GAIN_CFG            0x23
#define MIKROE_DWM1000_REG_EXTERNAL_SYNC            0x24
#define MIKROE_DWM1000_REG_READ_AA_DATA             0x25
#define MIKROE_DWM1000_REG_GPIO_CTRL                0x26
#define MIKROE_DWM1000_REG_DRX_CFG                  0x27
#define MIKROE_DWM1000_REG_RF_CFG                   0x28
#define MIKROE_DWM1000_REG_TX_CALIBRATION           0x2A
#define MIKROE_DWM1000_REG_FREQ_SYNTH_CTRL          0x2B
#define MIKROE_DWM1000_REG_ALWAYS_ON_REG            0x2C
#define MIKROE_DWM1000_REG_OTP_INTERFACE            0x2D
#define MIKROE_DWM1000_REG_LEAD_EDGE_DET_CTRL       0x2E
#define MIKROE_DWM1000_REG_DIGITAL_DIAG_IF          0x2F
#define MIKROE_DWM1000_REG_POWER_MANAGE_SYS_CTRL    0x36

/** \} */

/**
 * \defgroup otp_memory OTP memory
 * \{
 */
#define MIKROE_DWM1000_OTP_WRITE                    0x00
#define MIKROE_DWM1000_OTP_ADR                      0x04
#define MIKROE_DWM1000_OTP_CTRL                     0x06
#define MIKROE_DWM1000_OTP_STATUS                   0x08
#define MIKROE_DWM1000_OTP_RDAT                     0x0A
#define MIKROE_DWM1000_OTP_SRDAT                    0x0E
#define MIKROE_DWM1000_OTP_SF                       0x12

/** \} */

/**
 * \defgroup data_rate Data rate
 * \{
 */
#define MIKROE_DWM1000_DATA_RATE_110KBSPS           0x00
#define MIKROE_DWM1000_DATA_RATE_850KBSPS           0x01
#define MIKROE_DWM1000_DATA_RATE_6800KBSPS          0x02

/** \} */

/**
 * \defgroup pulese_frequency Pulese frequency
 * \{
 */
#define MIKROE_DWM1000_PULSTE_FREQ_4MHZ             0x00
#define MIKROE_DWM1000_PULSTE_FREQ_16MHZ            0x01
#define MIKROE_DWM1000_PULSTE_FREQ_64MHZ            0x02

/** \} */

/**
 * \defgroup preamble_length Preamble length
 * \{
 */
#define MIKROE_DWM1000_PREAMBLE_LEN_64              0x01
#define MIKROE_DWM1000_PREAMBLE_LEN_128             0x05
#define MIKROE_DWM1000_PREAMBLE_LEN_256             0x09
#define MIKROE_DWM1000_PREAMBLE_LEN_512             0x0D
#define MIKROE_DWM1000_PREAMBLE_LEN_1024            0x02
#define MIKROE_DWM1000_PREAMBLE_LEN_1536            0x06
#define MIKROE_DWM1000_PREAMBLE_LEN_2048            0x0A
#define MIKROE_DWM1000_PREAMBLE_LEN_4096            0x03

/** \} */

/**
 * \defgroup pac_size PAC size
 * \{
 */
#define MIKROE_DWM1000_PAC_SIZE_8                   8
#define MIKROE_DWM1000_PAC_SIZE_16                  16
#define MIKROE_DWM1000_PAC_SIZE_32                  32
#define MIKROE_DWM1000_PAC_SIZE_64                  64

/** \} */

/**
 * \defgroup channel Channel
 * \{
 */
#define MIKROE_DWM1000_CHANNEL_1                    1
#define MIKROE_DWM1000_CHANNEL_2                    2
#define MIKROE_DWM1000_CHANNEL_3                    3
#define MIKROE_DWM1000_CHANNEL_4                    4
#define MIKROE_DWM1000_CHANNEL_5                    5
#define MIKROE_DWM1000_CHANNEL_7                    7

/** \} */

/**
 * \defgroup preamble_code Preamble code
 * \{
 */
#define MIKROE_DWM1000_PREAMBLE_CODE_1              1
#define MIKROE_DWM1000_PREAMBLE_CODE_2              2
#define MIKROE_DWM1000_PREAMBLE_CODE_3              3
#define MIKROE_DWM1000_PREAMBLE_CODE_4              4
#define MIKROE_DWM1000_PREAMBLE_CODE_5              5
#define MIKROE_DWM1000_PREAMBLE_CODE_6              6
#define MIKROE_DWM1000_PREAMBLE_CODE_7              7
#define MIKROE_DWM1000_PREAMBLE_CODE_8              8
#define MIKROE_DWM1000_PREAMBLE_CODE_9              9
#define MIKROE_DWM1000_PREAMBLE_CODE_10             10
#define MIKROE_DWM1000_PREAMBLE_CODE_11             11
#define MIKROE_DWM1000_PREAMBLE_CODE_12             12
#define MIKROE_DWM1000_PREAMBLE_CODE_13             13
#define MIKROE_DWM1000_PREAMBLE_CODE_14             14
#define MIKROE_DWM1000_PREAMBLE_CODE_15             15
#define MIKROE_DWM1000_PREAMBLE_CODE_16             16
#define MIKROE_DWM1000_PREAMBLE_CODE_17             17
#define MIKROE_DWM1000_PREAMBLE_CODE_18             18
#define MIKROE_DWM1000_PREAMBLE_CODE_19             19
#define MIKROE_DWM1000_PREAMBLE_CODE_20             20
#define MIKROE_DWM1000_PREAMBLE_CODE_21             21
#define MIKROE_DWM1000_PREAMBLE_CODE_22             22
#define MIKROE_DWM1000_PREAMBLE_CODE_23             23
#define MIKROE_DWM1000_PREAMBLE_CODE_24             24

/** \} */

/**
 * \defgroup frame_length Frame length
 * \{
 */
#define MIKROE_DWM1000_FRAME_LEN_NORMAL             0x00
#define MIKROE_DWM1000_FRAME_LEN_EXTENDED           0x03

/** \} */

/**
 * \defgroup configuration_tranmit_type_mode Configuration tranmit type mode
 * \{
 */
#define MIKROE_DWM1000_HIGH                         1
#define MIKROE_DWM1000_LOW                          0

/** \} */

/**
 * \defgroup device_mode Device mode
 * \{
 */
#define MIKROE_DWM1000_DEV_ERROR                    0xFF
#define MIKROE_DWM1000_DEV_OK                       0x00

/** \} */

/**
 * \defgroup pin_bit_state Pin/bit state
 * \{
 */
#define MIKROE_DWM1000_MODE_IDLE                    1
#define MIKROE_DWM1000_MODE_RX                      2
#define MIKROE_DWM1000_MODE_TX                      3

/** \} */

/**
 * \defgroup function_return Function return
 * \{
 */
#define MIKROE_DWM1000_TAG                          0xDECA

/**
 * \defgroup constants Constants
 * \{
 */

extern const uint8_t MIKROE_DWM1000_TMODE_LONGDATA_RANGE_LOWPOWER[3];
extern const uint8_t MIKROE_DWM1000_TMODE_LONGDATA_RANGE_ACCURACY[3];
extern const uint8_t MIKROE_DWM1000_TMODE_LONGDATA_FAST_LOWPOWER[3];
extern const uint8_t MIKROE_DWM1000_TMODE_LONGDATA_FAST_ACCURACY[3];
extern const uint8_t MIKROE_DWM1000_TMODE_SHORTDATA_FAST_LOWPOWER[3];
extern const uint8_t MIKROE_DWM1000_TMODE_SHORTDATA_FAST_ACCURACY[3];

/*!
 * @addtogroup MIKROE DWM1000 CLICK PROTOTYPES
 * @brief API for configuring and manipulating UWB DWM1000 Click driver.
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *    This function initializes all necessary pins and peripherals used
 * for this click board.
 * @param[in]
 *    spi_instance : An SPI driver instance handle.
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL Initialization failed.
 *    SL_STATUS_NULL_POINTER if spi_instance is null.
 ******************************************************************************/
sl_status_t mikroe_dwm1000_init (mikroe_spi_handle_t spi_instance);

/**
 * @brief Function for writing data to register
 *
 * @param reg_adr         Register address.
 * @param tx_buf          Transmit buffer.
 * @param buf_len         Transmit buffer length.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 *    SL_STATUS_ALREADY_INITIALIZED Already initialized.
 *
 * @details This function write data to register.
 */
sl_status_t mikroe_dwm1000_generic_write (
  uint8_t reg_adr,
  uint8_t *tx_buf,
  uint16_t buf_len);

/**
 * @brief Function for reading data from register
 *
 * @param reg_adr         Register address.
 * @param rx_buf          Receive buffer.
 * @param buf_len         Receive buffer length.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 *
 * @details This function read data from register.
 */
sl_status_t mikroe_dwm1000_generic_read (
  uint8_t reg_adr,
  uint8_t *rx_buf,
  uint16_t buf_len);

/**
 * @brief Function for setting rst pin status
 *
 * @param status          Pin status.
 *
 * @details This function sets rst pin status.
 */
void mikroe_dwm1000_set_rst_pin_status (uint8_t status);

/**
 * @brief Function for getting irq pin status
 *
 *
 * @returns QINT pin status
 *
 * @details This function gets irq pin status.
 */
uint8_t mikroe_dwm1000_get_qint_pin_status (void);

/**
 * @brief Function for reading OTM memory
 *
 * @param otp_adr         Otp register address.
 * @param rx_buf          Receive data buffer.
 *
 * @details This function read OTM memory.
 */
void mikroe_dwm1000_read_otp (uint16_t otp_adr, uint8_t *rx_buf);

/**
 * @brief Function for setting single bit state
 *
 * @param reg_adr         Register address of bit.
 * @param bit_num         Bit number.
 * @param bit_state       Bit state.
 *
 * @details This function set single bit state.
 */
void mikroe_dwm1000_set_bit (
  uint8_t reg_adr,
  uint16_t bit_num,
  uint8_t bit_state);

/**
 * @brief Function for getting single bit state
 *
 * @param reg_adr         Register address of bit.
 * @param bit_num         Bit number.
 *
 * @returns Bit state getting single bit state.
 *
 * @details This function
 */
uint8_t mikroe_dwm1000_get_bit (uint8_t reg_adr, uint16_t bit_num);

/**
 * @brief Function for getting length of register data
 *
 * @param reg_adr         Register address.
 *
 * @returns Register length
 *
 * @details This function get length of register data
 */
uint16_t mikroe_dwm1000_reg_data_len (uint8_t reg_adr);

/**
 * @brief Function for setting device address and network ID
 *
 * @param dev_adr         Device address.
 * @param net_id          Network ID.
 *
 * @details This function set device address and network ID
 */
void mikroe_dwm1000_set_dev_adr_n_network_id (uint16_t dev_adr,
                                              uint16_t net_id);

/**
 * @brief Function for setting data rate
 *
 * @param rate            Data rate.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL if there is an error setting data rate.
 *
 * @details This function set data rate.
 */
sl_status_t mikroe_dwm1000_set_data_rate (uint8_t rate);

/**
 * @brief Function for setting pulse frequency
 *
 * @param freq            Device pulse frequency.
 *
 * @details This function set pulse frequency.
 */
void mikroe_dwm1000_set_pulse_freq (uint8_t freq);

/**
 * @brief Function for setting preamble length
 *
 * @param prealen         Device preamble length.
 *
 * @details This function set preamble length.
 */
void mikroe_dwm1000_set_preamble_len (uint8_t prealen);

/**
 * @brief Function for setting preamble code
 *
 * @param preamble        Device preamble code.
 *
 * @details This function set preamble code.
 */
void mikroe_dwm1000_set_preable_code (uint8_t preamble);

/**
 * @brief Function for setting channel
 *
 * @param channel         Device channel.
 *
 * @details This function set channel.
 */
void mikroe_dwm1000_set_channel (uint8_t channel);

/**
 * @brief Function for setting transmit type mode
 *
 * @param t_t             Transmit type mode.
 *
 * @details This function set transmit type mode.
 */
void mikroe_dwm1000_set_transmit_type (const uint8_t *t_t);

/**
 * @brief Function for getting transmit data
 *
 * @param rx_buf          Receive buffer.
 * @param len_buf         Transmit data length.
 *
 * @details This function get transmit data.
 */
void mikroe_dwm1000_get_transmit (uint8_t *rx_buf, uint16_t len_buf);

/**
 * @brief Function for getting transmit length
 *
 * @returns transmit length
 *
 * @details This function get transmit length.
 */
uint8_t mikroe_dwm1000_get_transmit_len ();

/**
 * @brief Function for setting transmit data
 *
 * @param tx_buf          Transmit buffer.
 * @param len_buf         Transmit data length.
 *
 * @details This function set transmit data.
 */
void mikroe_dwm1000_set_transmit (uint8_t *tx_buf, uint16_t len_buf);

/**
 * @brief Function for setting smart power option
 *
 * @param smart_power     Smart power data.
 *
 * @details This function set smart power option.
 */
void mikroe_dwm1000_use_smart_power (uint8_t smart_power);

/**
 * @brief Function for setting frame check option
 *
 * @param fc_val          Frame check data.
 *
 * @details This function set frame check option.
 */
void mikroe_dwm1000_frame_check (uint8_t fc_val);

/**
 * @brief Function for setting frame filter option
 *
 * @param ff_val          Frame filter data.
 *
 * @details This function set frame filter option.
 */
void mikroe_dwm1000_frame_filter (uint8_t ff_val);

/**
 * @brief Function for setting device working mode
 *
 * @param mode            Device working mode.
 *
 * @details This function set device working mode.
 * @note Should be first function called in driver after initialization.
 */
void mikroe_dwm1000_set_mode (uint8_t mode);

/**
 * @brief Function for starting communication of device
 *
 * @details This function start communication of device.
 * @note Function is called to start transmiting or receiving data.
 */
void mikroe_dwm1000_start_transceiver (void);

/**
 * @brief Function for getting transmit status
 *
 * @returns OK or ERROR macro
 *
 * @details This function get transmit status.
 * @return Returns status is relative to previous selected device mode.
 *
 */
uint8_t mikroe_dwm1000_get_transmit_status (void);

/**
 * @brief Function for clearing status
 *
 * @details This function clear status.
 * @note Clearing status relative to previous sleceted device mode.
 */
void mikroe_dwm1000_clear_status (void);

/**
 * @brief Function for setting interrupt mask
 *
 * @details This function set interrupt mask.
 * @note Setting interrupt mask relative to previous set device mode.
 */
void mikroe_dwm1000_int_mask_set (void);

/**
 * @brief Function for tuneing configuration for set values
 *
 * @details This function tune configuration for set values.
 * @note Should be called after other configuration is set. It will do some
 *  additional configuration to device relative to some other parameteres.
 */
void mikroe_dwm1000_tune_config (void);

/**
 * @brief Function for reseting device
 *
 * @details This function reset device.
 */
void mikroe_dwm1000_dev_reset (void);

/**
 * @brief Function for enabling device
 *
 * @details This function enables device.
 * @return
 */
void mikroe_dwm1000_enable (void);

#ifdef __cplusplus
}
#endif
#endif // MIKROE_UWB_DWM1000_H
