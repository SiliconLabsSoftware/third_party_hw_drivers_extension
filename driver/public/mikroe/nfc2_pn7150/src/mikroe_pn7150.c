/***************************************************************************//**
 * @file mikroe_pn7150.c
 * @brief Mikroe PN7150 Source File
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

#include "mikroe_pn7150_config.h"
#include "mikroe_pn7150.h"

static nfc2_t nfc2;
static nfc2_cfg_t nfc2_cfg;

/***************************************************************************//**
 *    Initialization function.
 ******************************************************************************/
sl_status_t mikroe_pn7150_init(mikroe_i2c_handle_t i2c_instance)
{
  if (NULL == i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  nfc2.i2c.handle = i2c_instance;

  // Configure default I2C address
  nfc2.slave_address = MIKROE_PN7150_ADDR;

  // Call basic setup functions
  nfc2_cfg_setup(&nfc2_cfg);

#if (MIKROE_PN7150_I2C_UC == 1)
  nfc2_cfg.i2c_speed = MIKROE_PN7150_I2C_SPEED_MODE;
#endif

#if defined(MIKROE_PN7150_INT_PORT) && defined(MIKROE_PN7150_INT_PIN)
  nfc2_cfg.int_pin = hal_gpio_pin_name(MIKROE_PN7150_INT_PORT,
                                       MIKROE_PN7150_INT_PIN);
#endif

#if defined(MIKROE_PN7150_RESET_PORT) && defined(MIKROE_PN7150_RESET_PIN)
  nfc2_cfg.rst = hal_gpio_pin_name(MIKROE_PN7150_RESET_PORT,
                                   MIKROE_PN7150_RESET_PIN);
#endif

  if (nfc2_init(&nfc2, &nfc2_cfg) != I2C_MASTER_SUCCESS) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    This function sets the IC2SPM instance used by platform functions.
 ******************************************************************************/
sl_status_t mikroe_pn7150_set_i2csmp_instance(mikroe_i2c_handle_t i2c_instance)
{
  if (NULL == i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  nfc2.i2c.handle = i2c_instance;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * NFC 2 default configuration function.
 ******************************************************************************/
void mikroe_pn7150_default_cfg(mikroe_pn7150_control_packet_t *ctrl_packet)
{
  nfc2_default_cfg(&nfc2, (control_packet_t *)ctrl_packet);
}

/***************************************************************************//**
 * NFC 2 I2C writing function.
 ******************************************************************************/
sl_status_t mikroe_pn7150_generic_write(uint8_t *p_tx_data, uint8_t n_bytes)
{
  err_t err = NFC2_OK;

  if (NULL == p_tx_data) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  err = nfc2_generic_write(&nfc2, p_tx_data, n_bytes);

  return (err == NFC2_OK) ? SL_STATUS_OK : SL_STATUS_TRANSMIT;
}

/***************************************************************************//**
 * NFC 2 I2C reading function.
 ******************************************************************************/
sl_status_t mikroe_pn7150_generic_read(uint8_t *p_rx_h_data,
                                       uint8_t *p_rx_p_data)
{
  err_t err = NFC2_OK;

  if ((NULL == p_rx_h_data) || (NULL == p_rx_p_data)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  err = nfc2_generic_read(&nfc2, p_rx_h_data, p_rx_p_data);

  return (err == NFC2_OK) ? SL_STATUS_OK : SL_STATUS_TRANSMIT;
}

/***************************************************************************//**
 * HW reset function.
 ******************************************************************************/
void mikroe_pn7150_hw_reset(void)
{
  nfc2_hw_reset(&nfc2);
}

/***************************************************************************//**
 * Get data function.
 ******************************************************************************/
sl_status_t mikroe_pn7150_get_data(uint8_t *len, uint8_t *p_rx_data)
{
  if ((NULL == len) || (NULL == p_rx_data)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  *len = nfc2_get_data(&nfc2, p_rx_data);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Read control packet data function.
 ******************************************************************************/
sl_status_t mikroe_pn7150_read_ctrl_packet_data(
  mikroe_pn7150_control_packet_t *ctrl_packet)
{
  if (NULL == ctrl_packet) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  nfc2_read_ctrl_packet_data(&nfc2, (control_packet_t *)ctrl_packet);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Write control packet data function.
 ******************************************************************************/
void mikroe_pn7150_write_ctrl_packet_data(
  mikroe_pn7150_control_packet_t ctrl_packet)
{
  control_packet_t *ctrl_pck = (control_packet_t *)&ctrl_packet;
  nfc2_write_ctrl_packet_data(&nfc2, *ctrl_pck);
}

/***************************************************************************//**
 * Check IRQ ststus function.
 ******************************************************************************/
bool mikroe_pn7150_check_irq(void)
{
  return nfc2_check_irq(&nfc2);
}

/***************************************************************************//**
 * Core reset command function.
 ******************************************************************************/
void mikroe_pn7150_cmd_core_reset(void)
{
  nfc2_cmd_core_reset(&nfc2);
}

/***************************************************************************//**
 * Core init command function.
 ******************************************************************************/
void mikroe_pn7150_cmd_core_init(void)
{
  nfc2_cmd_core_init(&nfc2);
}

/***************************************************************************//**
 * Disable standby mode command function.
 ******************************************************************************/
void mikroe_pn7150_cmd_disable_standby_mode(void)
{
  nfc2_cmd_disable_standby_mode(&nfc2);
}

/***************************************************************************//**
 * Antenna test function.
 ******************************************************************************/
void mikroe_pn7150_cmd_antenna_test(uint8_t sel_ant)
{
  nfc2_cmd_antenna_test(&nfc2, sel_ant);
}

/***************************************************************************//**
 * NFC test procedure command function.
 ******************************************************************************/
void mikroe_pn7150_cmd_test_procedure(void)
{
  nfc2_cmd_test_procedure(&nfc2);
}

/***************************************************************************//**
 * Core reset command function.
 ******************************************************************************/
void mikroe_pn7150_core_reset(void)
{
  nfc2_core_reset(&nfc2);
}

/***************************************************************************//**
 * Core initialization command function.
 ******************************************************************************/
void mikroe_pn7150_init_core(void)
{
  nfc2_init_core(&nfc2);
}

/***************************************************************************//**
 * Set default configuration function.
 ******************************************************************************/
void mikroe_pn7150_core_set_default_config(void)
{
  nfc2_core_set_default_config(&nfc2);
}

/***************************************************************************//**
 * Start discovery command function.
 ******************************************************************************/
void mikroe_pn7150_core_set_protocol_config(void)
{
  nfc2_core_set_protocol_config(&nfc2);
}

/***************************************************************************//**
 * Start discovery command function.
 ******************************************************************************/
void mikroe_pn7150_cmd_start_discovery(void)
{
  nfc2_cmd_start_discovery(&nfc2);
}

/***************************************************************************//**
 * Activate remote mifare card command function.
 ******************************************************************************/
void mikroe_pn7150_activate_rmt_mifare_card(void)
{
  nfc2_activate_rmt_mifare_card(&nfc2);
}

/***************************************************************************//**
 * Authenticate sector command function.
 ******************************************************************************/
void mikroe_pn7150_cmd_authenticate_sector(uint8_t sel_sect_addr)
{
  nfc2_cmd_authenticate_sector(&nfc2, sel_sect_addr);
}

/***************************************************************************//**
 * Card disconnected command function.
 ******************************************************************************/
void mikroe_pn7150_cmd_card_disconnected(void)
{
  nfc2_cmd_card_disconnected(&nfc2);
}
