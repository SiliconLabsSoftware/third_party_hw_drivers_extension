/***************************************************************************//**
 * @file mikroe_pn7150.h
 * @brief SCL PN7150 Prototypes
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

#ifndef _MIKROE_PN7150_H_
#define _MIKROE_PN7150_H_

#include "sl_status.h"
#include "drv_i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup PN7150 Driver
 * @brief PN7150 types and APIs.
 * @{
 ******************************************************************************/

/**
 * @brief NFC 2 Click control packet object.
 * @details Control packet object definition of NFC 2 Click driver.
 */
typedef struct
{
  uint8_t message_type;
  uint8_t pck_bound_flag;
  uint8_t group_ident;
  uint8_t opcode_ident;
  uint8_t payload_length;
  uint8_t payload[256];
} mikroe_pn7150_control_packet_t;

/**
 * @brief NFC 2 Click data packet object.
 * @details Data packet object definition of NFC 2 Click driver.
 */
typedef struct
{
  uint8_t message_type;
  uint8_t pck_bound_flag;
  uint8_t conn_ident;
  uint8_t payload_length;
  uint8_t payload[256];
} mikroe_pn7150_data_packet_t;

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] i2c_instance
 *    I2CSPM instance
 *
 * @return SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_pn7150_init(mikroe_i2c_handle_t i2c_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the IC2SPM instance used by platform functions.
 *
 * @param[in] i2c_instance
 *    I2CSPM instance
 *
 * @return SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_pn7150_set_i2csmp_instance(mikroe_i2c_handle_t i2c_instance);

/***************************************************************************//**
 * @brief NFC 2 default configuration function.
 * @details This function executes a default configuration of NFC 2
 * click board.
 * @param[out] ctrl_pck : Pointer to the memory location of structure.
 * @return Nothing.
 * @note This function can consist any necessary configuration or setting to put
 * device into operating mode.
 ******************************************************************************/
void mikroe_pn7150_default_cfg(mikroe_pn7150_control_packet_t *ctrl_pck);

/***************************************************************************//**
 * @brief NFC 2 I2C writing function.
 * @details This function writes a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 * @param[in] p_tx_data : Data to be written.
 * @param[in] n_bytes : Number of bytes to be written.
 * @return SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_pn7150_generic_write(uint8_t *p_tx_data, uint8_t n_bytes);

/***************************************************************************//**
 * @brief NFC 2 I2C reading function.
 * @details This function reads a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 * @param[out] p_rx_h_data : Pointer to the memory location where header data be
 *   stored.
 * @param[out] p_rx_p_data : Pointer to the memory location where payload data
 *   be stored.
 * @return SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_pn7150_generic_read(uint8_t *p_rx_h_data,
                                       uint8_t *p_rx_p_data);

/***************************************************************************//**
 * @brief HW reset function.
 * @details The function hw reset
 ******************************************************************************/
void mikroe_pn7150_hw_reset(void);

/***************************************************************************//**
 * @brief Get data function.
 * @details The function read a sequential data starting
 * of the PN7150 High performance NFC controller on the NFC 2 click board.
 * @param[out] len : Number od data.
 * @param[out] p_rx_data : Pointer to the memory location where data be stored.
 * @return SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_pn7150_get_data(uint8_t *len, uint8_t *p_rx_data);

/***************************************************************************//**
 * @brief Read control packet data function.
 * @details The function read control packet data
 * of the PN7150 High performance NFC controller on the NFC 2 click board.
 * @param[in] ctrl_pck : Pointer to the memory location of structure.
 * @return SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_pn7150_read_ctrl_packet_data(
  mikroe_pn7150_control_packet_t *ctrl_packet);

/***************************************************************************//**
 * @brief Write control packet data function.
 * @details The function write control packet data
 * to the PN7150 High performance NFC controller on the NFC 2 click board.
 * @param[in] ctrl_pck : Structure where data be stored.
 ******************************************************************************/
void mikroe_pn7150_write_ctrl_packet_data(
  mikroe_pn7150_control_packet_t ctrl_packet);

/***************************************************************************//**
 * @brief Check IRQ ststus function.
 * @details The function check interrupt state and
 * return state of the INT pin
 * @return 0 - No active, 1 - Active.
 ******************************************************************************/
bool mikroe_pn7150_check_irq(void);

/***************************************************************************//**
 * @brief Core reset command function.
 * @details The function set core reset command
 * to the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_cmd_core_reset(void);

/***************************************************************************//**
 * @brief Core init command function.
 * @details The function set core init command
 * to the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_cmd_core_init(void);

/***************************************************************************//**
 * @brief Disable standby mode command function.
 * @details The function set disable standby mode command
 * to the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_cmd_disable_standby_mode(void);

/***************************************************************************//**
 * @brief Antenna test function.
 * @details The function set antenna test
 * to the PN7150 High performance NFC controller on the NFC 2 click board.
 * @param[in] ctx : Click context object.
 ******************************************************************************/
void mikroe_pn7150_cmd_antenna_test(uint8_t sel_ant);

/***************************************************************************//**
 * @brief NFC test procedure command function.
 * @details The function set test procedure command
 * to the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_cmd_test_procedure(void);

/***************************************************************************//**
 * @brief Core reset command function.
 * @details The function set core reset command
 * to the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_core_reset(void);

/***************************************************************************//**
 * @brief Core initialization command function.
 * @details The function set core initialization command
 * to the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_init_core(void);

/***************************************************************************//**
 * @brief Set default configuration function.
 * @details The function set default configuration
 * of the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_core_set_default_config(void);

/***************************************************************************//**
 * @brief Set protocol configuration function.
 * @details The function set protocol configuration
 * of the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_core_set_protocol_config(void);

/***************************************************************************//**
 * @brief Start discovery command function.
 * @details The function set start discovery command
 * of the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_cmd_start_discovery(void);

/***************************************************************************//**
 * @brief Activate remote mifare card command function.
 * @details The function set activate remote mifare card command
 * of the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_activate_rmt_mifare_card(void);

/***************************************************************************//**
 * @brief Authenticate sector command function.
 * @details The function set authenticate sector command
 * of the PN7150 High performance NFC controller on the NFC 2 click board.
 * @param[in] ctx : Click context object.
 ******************************************************************************/
void mikroe_pn7150_cmd_authenticate_sector(uint8_t sel_sect_addr);

/***************************************************************************//**
 * @brief Card disconnected command function.
 * @details The function set card disconnected command
 * of the PN7150 High performance NFC controller on the NFC 2 click board.
 ******************************************************************************/
void mikroe_pn7150_cmd_card_disconnected(void);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup PN7150 Driver) */
#endif /* _MIKROE_PN7150_H_ */
