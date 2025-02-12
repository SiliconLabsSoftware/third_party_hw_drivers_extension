/***************************************************************************//**
 * @file mikroe_ra_08.h
 * @brief SCL Mikroe LR RA-08 header file
 * @version 1.2.0
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
#ifndef MIKROE_RA_08_H_
#define MIKROE_RA_08_H_

#include "sl_status.h"
#include "lr9.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup lr9 LR 9 Click Driver
 * @brief API for configuring and manipulating LR 9 Click driver.
 * @{
 */

/**
 * @defgroup mikroe_ra_08_cmd LR 9 Device Settings
 * @brief Settings for registers of LR 9 Click driver.
 */

/**
 * @addtogroup mikroe_ra_08_cmd
 * @{
 */

/**
 * @brief LR 9 AT Command.
 * @details Specified setting for AT Command of LR 9 Click driver.
 */
#define MIKROE_RA_08_CMD_AT                    "AT"

/**
 * @brief LR 9 LoRaWAN General Command Sets commands.
 * @details Specified setting for LoRaWAN General Command Sets Commands of LR 9
 *   Click driver.
 */
#define MIKROE_RA_08_CMD_CGMI                  "CGMI"
#define MIKROE_RA_08_CMD_CGMM                  "CGMM"
#define MIKROE_RA_08_CMD_CGMR                  "CGMR"
#define MIKROE_RA_08_CMD_CGSN                  "CGSN"
#define MIKROE_RA_08_CMD_CGBR                  "CGBR"

/**
 * @brief LR 9 LoRaWAN Network Related Parameter Setup Command Sets commands.
 * @details Specified setting for LoRaWAN Network Related Parameter Setup
 *   Command Sets Commands of LR 9 Click driver.
 */
#define MIKROE_RA_08_CMD_CJOINMODE             "CJOINMODE"
#define MIKROE_RA_08_CMD_CDEVEUI               "CDEVEUI"
#define MIKROE_RA_08_CMD_CAPPEUI               "CAPPEUI"
#define MIKROE_RA_08_CMD_CAPPKEY               "CAPPKEY"
#define MIKROE_RA_08_CMD_CDEVADDR              "CDEVADDR"
#define MIKROE_RA_08_CMD_CAPPSKEY              "CAPPSKEY"
#define MIKROE_RA_08_CMD_CNWKSKEY              "CNWKSKEY"
#define MIKROE_RA_08_CMD_CFREQBANDMASK         "CFREQBANDMASK"
#define MIKROE_RA_08_CMD_CULDLMODE             "CULDLMODE"
#define MIKROE_RA_08_CMD_CADDMUTICAST          "CADDMUTICAST"
#define MIKROE_RA_08_CMD_CDELMUTICAST          "CDELMUTICAST"
#define MIKROE_RA_08_CMD_CNUMMUTICAST          "CNUMMUTICAST"
#define MIKROE_RA_08_CMD_CWORKMODE             "CWORKMODE"
#define MIKROE_RA_08_CMD_CCLASS                "CCLASS"
#define MIKROE_RA_08_CMD_CBL                   "CBL"
#define MIKROE_RA_08_CMD_CSTATUS               "CSTATUS"
#define MIKROE_RA_08_CMD_CJOIN                 "CJOIN"
#define MIKROE_RA_08_CMD_CPINGSLOTINFOREQ      "CPINGSLOTINFOREQ"
#define MIKROE_RA_08_CMD_DTRX                  "DTRX"
#define MIKROE_RA_08_CMD_DRX                   "DRX"

/**
 * @brief LR 9 LoRaWAN MAC Setup Command Sets.
 * @details Specified setting for LoRaWAN MAC Setup Command Sets of LR 9 Click
 *   driver.
 */
#define MIKROE_RA_08_CMD_CCONFIRM              "CCONFIRM"
#define MIKROE_RA_08_CMD_CAPPPORT              "CAPPPORT"
#define MIKROE_RA_08_CMD_CDATARATE             "CDATARATE"
#define MIKROE_RA_08_CMD_CRSSI                 "CRSSI"
#define MIKROE_RA_08_CMD_CNBTRIALS             "CNBTRIALS"
#define MIKROE_RA_08_CMD_CRM                   "CRM"
#define MIKROE_RA_08_CMD_CTXP                  "CTXP"
#define MIKROE_RA_08_CMD_CLINKCHECK            "CLINKCHECK"
#define MIKROE_RA_08_CMD_CADR                  "CADR"
#define MIKROE_RA_08_CMD_CRXP                  "CRXP"
#define MIKROE_RA_08_CMD_CRX1DELAY             "CRX1DELAY"
#define MIKROE_RA_08_CMD_CSAVE                 "CSAVE"
#define MIKROE_RA_08_CMD_CRESTORE              "CRESTORE"

/**
 * @brief LR 9 Other Command Sets.
 * @details Specified setting for Other Command Sets of LR 9 Click driver.
 */
#define MIKROE_RA_08_CMD_IREBOOT               "IREBOOT"
#define MIKROE_RA_08_CMD_ILOGLVL               "ILOGLVL"

/**
 * @brief LR 9 Private Command Sets.
 * @details Specified setting for Private Command Sets of LR 9 Click driver.
 */
#define MIKROE_RA_08_CMD_CKEYSPROTECT          "CKEYSPROTECT"

/**
 * @brief LR 9 LoRa Test Commands.
 * @details Specified setting for LoRa Test Commands of LR 9 Click driver.
 */
#define MIKROE_RA_08_CMD_CTXADDRSET            "CTXADDRSET"
#define MIKROE_RA_08_CMD_CADDRSET              "CADDRSET"
#define MIKROE_RA_08_CMD_CTX                   "CTX"
#define MIKROE_RA_08_CMD_CRX                   "CRX"
#define MIKROE_RA_08_CMD_CRXS                  "CRXS"

/**
 * @brief LR 9 Data Receive Configuration Commands.
 * @details Specified setting for Data Receive Configuration Commands of LR 9
 *   Click driver.
 */
#define MIKROE_RA_08_DTRX_CONFIG               "0"
#define MIKROE_RA_08_DTRX_UNCONFIG             "1"

/**
 * @brief LR 9 configuration and limits data values.
 * @details Specified setting for configuration data values of LR 9 Click
 *   driver.
 */
#define MIKROE_RA_08_DTRX_CONFIG_DATA          0
#define MIKROE_RA_08_DTRX_UNCONFIG_DATA        1
#define MIKROE_RA_08_NB_TRIALS_MIN             1
#define MIKROE_RA_08_NB_TRIALS_2               2
#define MIKROE_RA_08_NB_TRIALS_MAX             15
#define MIKROE_RA_08_DATA_FRAME_MAX            10
#define MIKROE_RA_08_MSG_START_POS             16

/**
 * @brief LR 9 control symbols.
 * @details Specified setting for control symbols of LR 9 Click driver.
 */
#define MIKROE_RA_08_SYMBOL_END_CMD            "\r\n"
#define MIKROE_RA_08_SYMBOL_QUERY              "?"
#define MIKROE_RA_08_SYMBOL_SET_VAL_CMD        "="
#define MIKROE_RA_08_SYMBOL_PREFIX_CMD         "+"
#define MIKROE_RA_08_SYMBOL_SEPARATOR          ","
#define MIKROE_RA_08_SYMBOL_SPACE              " "
#define MIKROE_RA_08_SYMBOL_NULL               ""
#define MIKROE_RA_08_SYMBOL_QUOTE              "\""
#define MIKROE_RA_08_ASCII_SPACE               0x20
#define MIKROE_RA_08_ASCII_QUOTE               0x22

/**
 * @brief LR 9 driver buffer size.
 * @details Specified size of driver ring buffer.
 * @note Increase buffer size if needed.
 */
#define LR9_TX_DRV_BUFFER_SIZE                 100
#define LR9_RX_DRV_BUFFER_SIZE                 300

/*! @} */ // mikroe_ra_08_cmd

/**
 * @defgroup mikroe_ra_08_map LR 9 MikroBUS Map
 * @brief MikroBUS pin mapping of LR 9 Click driver.
 */

/**
 * @addtogroup mikroe_ra_08_map
 * @{
 */

/**
 * @brief LR 9 Click enable equal to data symbol.
 * @details Predefined enum values for driver enable equal to data symbol.
 */
typedef enum
{
  MIKROE_RA_08_EQUAL_DISABLE = 0,
  MIKROE_RA_08_EQUAL_ENABLE
} mikroe_ra_08_en_set_t;

/**
 * @brief LR 9 configuration object setup function.
 * @details This function initializes click configuration structure to initial
 * values.
 * @return Nothing.
 * @note The all used pins will be set to unconnected state.
 */
void mikroe_ra_08_cfg_setup(void);

/**
 * @brief LR 9 initialization function.
 * @details This function initializes all necessary pins and peripherals used
 * for this click board.
 * @param[in] instance : UART handler
 * @return @li @c  SL_STATUS_OK - Success,
 *         @li @c  SL_SATATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_ra_08_init(mikroe_uart_handle_t  instance);

/**
 * @brief LR 9 HW reset function.
 * @details This function performs a hardware reset of the device.
 * @return Nothing.
 * @note None.
 */
void mikroe_ra_08_hw_reset(void);

/**
 * @brief LR 9 set BOOT pin state function.
 * @details This function sets a desired logic state of the BT pin.
 * @param[in] state : Pin logic state.
 * @return Nothing.
 * @note None.
 */
void mikroe_ra_08_set_boot_pin(uint8_t state);

/**
 * @brief LR 9 data writing function.
 * @details This function writes a desired number of data bytes by using UART
 *   serial interface.
 * @param[in] data_in : Data buffer for sending.
 * @param[in] len : Number of bytes for sending.
 * @return @li @c  SL_STATUS_OK - Success,
 *         @li @c  SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_ra_08_generic_write(uint8_t *data_in, uint16_t len);

/**
 * @brief LR 9 data reading function.
 * @details This function reads a desired number of data bytes by using UART
 *   serial interface.
 * @param[in] ctx : Click context object.
 * See #lr9_t object definition for detailed explanation.
 * @param[out] data_out : Output read data.
 * @param[out] rx_size : Number of bytes is read.
 * @param[in] len : Number of bytes to be read.
 * @return @li @c  SL_STATUS_OK - Success,
 *         @li @c  SL_STATUS_FAIL - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
sl_status_t mikroe_ra_08_generic_read(uint8_t *data_out,
                                      uint16_t len,
                                      int32_t *rx_size);

/**
 * @brief LR 9 command writing function.
 * @details This function writes a desired command and parameter by using UART
 *   serial interface.
 * @param[in] ctx : Click context object.
 * See #lr9_t object definition for detailed explanation.
 * @param[in] command : Command buffer for sending.
 * @param[in] param : Parameter buffer for sending.
 * @return @li @c  SL_STATUS_OK - Success,
 *         @li @c  SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_ra_08_write_command(uint8_t *command, uint8_t *param);

/**
 * @brief LR 9 inquire writing function.
 * @details Using the UART serial interface, this function writes the desired
 *   query command
 * with or without the included equals symbol.
 * @param[in] command : Command buffer for sending.
 * @param[in] en_set : en_set.
 * @return @li @c  SL_STATUS_OK - Success,
 *         @li @c  SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_ra_08_inquire_command(uint8_t *command,
                                         mikroe_ra_08_en_set_t en_set);

/**
 * @brief LR 9 send data frame function.
 * @details This function sends the desired data frame by using the UART serial
 *   interface.
 * @param[out] data_frame : data_frame.
 * @param[in] confirm : confirm.
 * @param[in] nbtrials : nbtrials.
 * @return @li @c  SL_STATUS_OK - Success,
 *         @li @c  SL_STATUS_FAIL - Error.
 * @note None.
 */
sl_status_t mikroe_ra_08_send_data_frame(uint8_t confirm,
                                         uint8_t nbtrials,
                                         uint8_t *data_frame);

#ifdef __cplusplus
}
#endif
#endif // MIKROE_RA_08_H_
