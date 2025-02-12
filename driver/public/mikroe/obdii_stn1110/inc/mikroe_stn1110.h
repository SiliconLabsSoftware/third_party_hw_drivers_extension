/***************************************************************************//**
 * @file mikroe_stn1110.h
 * @brief SCL fingerprint2 Prototypes
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

#ifndef MIKROE_OBDII_H_
#define MIKROE_OBDII_H_

#include "sl_status.h"
#include "obdii.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup mikroe_stn1110 STN1110 - OBDii Click
 * @brief
 *  The integration of the Mikroe SDK Click extension STN1110 driver.
 *  (OBDii Click board.)
 *
 *    @n @section stn1110_example stn1110 example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "sl_iostream_init_usart_instances.h"
 * #include "mikroe_stn1110.h"
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize and set mode
 *      mikroe_stn1110_init(sl_iostream_uart_mikroe_handle);
 *      mikroe_stn1110_send_command(OBDII_CMD_RESET_DEVICE);
 *      // Check response here
 *      mikroe_stn1110_send_command(OBDII_CMD_GET_CURRENT_RPM);
 *      // Check response here
 *
 *      while (true) {
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief Mikroe OBDII control commands.
 * @details Specified setting for control commands of OBDII Click driver.
 ******************************************************************************/
#define MIKROE_OBDII_CMD_FIRMWARE_VERSION   OBDII_CMD_FIRMWARE_VERSION
#define MIKROE_OBDII_CMD_DISABLE_ECHO       OBDII_CMD_DISABLE_ECHO
#define MIKROE_OBDII_CMD_ENABLE_ECHO        OBDII_CMD_ENABLE_ECHO
#define MIKROE_OBDII_CMD_AUTO_PROTOCOL      OBDII_CMD_AUTO_PROTOCOL
#define MIKROE_OBDII_CMD_SPACES_OFF         OBDII_CMD_SPACES_OFF
#define MIKROE_OBDII_CMD_RESET_DEFAULT      OBDII_CMD_RESET_DEFAULT
#define MIKROE_OBDII_CMD_RESET_DEVICE       OBDII_CMD_RESET_DEVICE
#define MIKROE_OBDII_CMD_GET_CURRENT_RPM    OBDII_CMD_GET_CURRENT_RPM
#define MIKROE_OBDII_CMD_GET_CURRENT_SPEED  OBDII_CMD_GET_CURRENT_SPEED

/***************************************************************************//**
 * @brief Mikroe OBDII device response for AT commands.
 * @details Device response after commands.
 ******************************************************************************/
#define MIKROE_OBDII_RSP_OK                 OBDII_RSP_OK
#define MIKROE_OBDII_RSP_UNKNOWN_CMD        OBDII_RSP_UNKNOWN_CMD
#define MIKROE_OBDII_RSP_PROMPT             OBDII_RSP_PROMPT
#define MIKROE_OBDII_RSP_CURRENT_RPM        OBDII_RSP_CURRENT_RPM
#define MIKROE_OBDII_RSP_CURRENT_SPEED      OBDII_RSP_CURRENT_SPEED

/***************************************************************************//**
 * @brief
 *   This function initializes all necessary pins and peripherals used
 *   for this click board.
 *
 * @param[in] handle
 *   UART instance.
 *
 * @return
 *   SL_STATUS_OK if successful initialization.
 *   Other status if initialization failed.
 ******************************************************************************/
sl_status_t mikroe_stn1110_init(mikroe_uart_handle_t handle);

/***************************************************************************//**
 * @brief
 *   This function sets the UART instance used by platform functions.
 *
 * @param[in] handle
 *   UART instance.
 *
 * @return
 *   SL_STATUS_OK if there are no errors.
 *   SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_stn1110_set_uart_instance(mikroe_uart_handle_t handle);

/***************************************************************************//**
 * @brief
 *   This function writes a desired number of data bytes by using UART serial
 *   interface.
 *
 * @param[in] data_in
 *   Data buffer for sending.
 *
 * @return
 *   SL_STATUS_OK if there are no errors.
 *   SL_STATUS_INVALID_PARAMETER if data_out is null.
 ******************************************************************************/
sl_status_t mikroe_stn1110_generic_write(uint8_t *data_in, uint16_t len);

/***************************************************************************//**
 * @brief
 *   This function reads a desired number of data bytes by using UART serial
 *   interface.
 *
 * @param[out] data_out
 *   Output read data.
 *
 * @return
 *   SL_STATUS_OK if there are no errors.
 *   SL_STATUS_INVALID_PARAMETER if data_out is null.
 ******************************************************************************/
sl_status_t mikroe_stn1110_generic_read(uint8_t *data_out, uint16_t len);

/***************************************************************************//**
 * @brief
 *   This function sends command string by using UART serial interface.
 *
 * @param[in] cmd
 *   Command string to send.
 *
 * @return
 *   SL_STATUS_OK if there are no errors.
 *   SL_STATUS_INVALID_PARAMETER if data_out is null.
 ******************************************************************************/
sl_status_t mikroe_stn1110_send_command(uint8_t *cmd);

/***************************************************************************//**
 * @brief
 *   This function resets the device by toggling the RST pin.
 *
 * @return None.
 ******************************************************************************/
void mikroe_stn1110_reset_device(void);

/***************************************************************************//**
 * @brief
 *   This function sets the RST pin logic state.
 *
 * @param[in] state
 *   Pin logic state.
 *
 * @return None.
 ******************************************************************************/
void mikroe_stn1110_set_rst_pin(uint8_t state);

/***************************************************************************//**
 * @brief
 *   This function returns the INT pin logic state.
 *
 * @return Pin logic state.
 ******************************************************************************/
uint8_t mikroe_stn1110_get_int_pin(void);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup stn1110) */

#endif /* MIKROE_OBDII_H_ */
