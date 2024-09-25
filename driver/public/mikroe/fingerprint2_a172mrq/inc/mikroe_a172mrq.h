/***************************************************************************//**
 * @file mikroe_fingerprint2.c
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

#ifndef MIKROE_FINGERPRINT2_H_
#define MIKROE_FINGERPRINT2_H_

#include "sl_status.h"
#include "drv_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup cmd Command
 * \{
 */
#define MIKROE_FINGERPRINT2_CMD_GET_FW_VER       "<C>GetFWVer</C>"
#define MIKROE_FINGERPRINT2_CMD_GET_DS           "<C>GetDS</C>"
#define MIKROE_FINGERPRINT2_CMD_FP_CMP           "<C>CompareFingerprint</C>"
#define MIKROE_FINGERPRINT2_CMD_FP_REG           "<C>RegisterFingerprint</C>"
#define MIKROE_FINGERPRINT2_CMD_FP_CLR           "<C>ClearRegisteredFp</C>"
#define MIKROE_FINGERPRINT2_CMD_FP_IMG_INFO      "<C>FpImageInformation</C>"
#define MIKROE_FINGERPRINT2_CMD_FP_IMG_SCAN      "<C>ScanFpImage</C>"
#define MIKROE_FINGERPRINT2_CMD_FP_REG_NO        "<C>CheckRegisteredNo</C>"
#define MIKROE_FINGERPRINT2_CMD_LOCK_DEV         "<C>LockDevice</C>"
#define MIKROE_FINGERPRINT2_CMD_UNLOCK_DEV       "<C>UnlockCompareFp</C>"

/** \} */

/**
 * \defgroup driver Driver define
 * \{
 */
#define MIKROE_FP2_RX_BUFFER_SIZE                800
#define MIKROE_FP2_TX_BUFFER_SIZE                100

/** \} */

/**
 * \defgroup a172mrq Public function
 * \{
 */

/**
 * @brief Config Object Initialization function.
 *
 * @description This function initializes click configuration structure to init
 *   state.
 * @note All used pins will be set to unconnected state.
 */
void mikroe_a172mrq_cfg_setup(void);

/**
 * @brief Initialization function.
 * @details This function initializes all necessary pins and peripherals
 *          used for this click.
 * @param[in] handle : Click context object.
 * @return
 *    SL_STATUS_OK if there are no errors
 *    SL_STATUS_INVALID_PARAMETER if spi_instance is null
 */
sl_status_t mikroe_a172mrq_init(mikroe_uart_handle_t handle);

/**
 * @brief Set uart instance function.
 * @details This function sets the uart used for this click.
 * @param[in] handle : Click context object.
 * @return
 *    SL_STATUS_OK if there are no errors
 *    SL_STATUS_INVALID_PARAMETER if spi_instance is null
 */
sl_status_t mikroe_a172mrq_set_uart_instance(mikroe_uart_handle_t handle);

/**
 * @brief Generic write function.
 * @details This function write specific number of bytes
 * @param data_buf    Data buffer for sends.
 * @param len         Number of bytes for sends.
 */
sl_status_t mikroe_a172mrq_generic_write(char *data_buf, uint16_t len);

/**
 * @brief Generic read function.
 * @details This function read maximum length of data.
 * @param data_buf    Data buffer for read data.
 * @param max_len     The maximum length of data that can be read.
 * @param rsp_size    Size of data that read.
 * @returns Status of function.
 */
sl_status_t mikroe_a172mrq_generic_read(char *data_buf,
                                        uint16_t max_len,
                                        int32_t *rsp_size);

/**
 * @brief Generic function for reading ld1 pin status
 * @details This function read ld1 pin status.
 * @returns Pin status.
 */
uint8_t mikroe_a172mrq_get_ld1_status(void);

/**
 * @brief Generic function for reading ld2 pin status
 * @details This function read ld2 pin status
 * @returns Pin status
 */
uint8_t mikroe_a172mrq_get_ld2_status(void);

/**
 * @brief Generic function for setting rst pin status
 * @details This function sets rst pin status.
 * @param status State of the pin
 */
void mikroe_a172mrq_set_rst_status(uint8_t status);

/**
 * @brief Generic function for setting gp1 pin status
 * @details This function sets gp1 pin status.
 * @param status State of the pin.
 */
void mikroe_a172mrq_set_gp1_status(uint8_t status);

/**
 * @brief Generic function for setting gp2 pin status
 * @description This function sets gp2 pin status.
 * @param status State of the pin
 */
void mikroe_a172mrq_set_gp2_status(uint8_t status);

/**
 * @brief Function for restarting device
 * @description This function restarts device.
 */
void mikroe_a172mrq_reset(void);

/**
 * @brief Function for fingerprint registration on index
 * @description This function registrates fingerprint on index.
 * @param fp_index Fingerprint index
 */
void mikroe_a172mrq_reg_one_fp(uint8_t fp_index);

/**
 * @brief Function for deleting fingerprint on index
 * @details This function deletes fingerprint on index
 * @param fp_index Fingerprint index
 */
void mikroe_a172mrq_delete_one_fp(uint8_t fp_index);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup a172mrq) */

#endif // MIKROE_FINGERPRINT2_H_
