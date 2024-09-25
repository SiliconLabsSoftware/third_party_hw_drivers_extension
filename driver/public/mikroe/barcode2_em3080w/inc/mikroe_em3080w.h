/***************************************************************************//**
 * @file mikroe_em3080w.h
 * @brief Barcode Decoder Driver
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#ifndef MIKROE_EM3080W_H_
#define MIKROE_EM3080W_H_

#include "sl_status.h"
#include "drv_uart.h"
#include "barcode2.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup enable_and_disable_state Enable And Disable State
 * \{
 */
#define MIKROE_BARCODE2_DISABLE BARCODE2_DISABLE
#define MIKROE_BARCODE2_ENABLE  BARCODE2_ENABLE

typedef enum {
  BARCODE2_SUCCESS=0,
  BARCODE2_ERROR=-1,
  BARCODE2_INVALID_PARAMETER=-2
}mikroe_barcode2_er_t;

/**
 * @brief Click configuration structure definition.
 */
typedef struct
{
  uint16_t          baud_rate;       // Clock speed.
  bool              uart_blocking;
  uart_data_bits_t  data_bit;        // Data bits.
  uart_parity_t     parity_bit;      // Parity bit.
  uart_stop_bits_t  stop_bit;        // Stop bits.
} mikroe_barcode2_cfg_t;

/**
 * @brief Click ctx object definition.
 */
typedef barcode2_t mikroe_barcode2_t;

/***************************************************************************//**
 * @brief Initialization function.
 *
 * @param ctx Click object.
 * @param cfg Click configuration structure.
 *
 * @description This function initializes all necessary pins and peripherals
 *   used for this click.
 ******************************************************************************/
sl_status_t mikroe_barcode2_init (mikroe_barcode2_t *barcode2,
                                  mikroe_uart_handle_t uart_handle,
                                  uint16_t baud_rate);

/***************************************************************************//**
 * @brief Generic write function.
 *
 * @param ctx       Click object.
 * @param data_buf  Data buffer for sends.
 * @param len       Number of bytes for sends.
 *
 * @description This function write desired number of bytes.
 ******************************************************************************/
sl_status_t mikroe_barcode2_generic_write (mikroe_barcode2_t *barcode2,
                                           char *data_buf,
                                           uint16_t len);

/***************************************************************************//**
 * @brief Generic read function.
 *
 * @param ctx       Click object.
 * @param data_buf  Data buffer for read data.
 * @param max_len   The maximum length of data that can be read.
 * @return Number of reads data.
 *
 * @description This function read maximum length of data.
 ******************************************************************************/
int32_t mikroe_barcode2_generic_read (mikroe_barcode2_t *barcode2,
                                      char *data_buf,
                                      uint16_t max_len);

/***************************************************************************//**
 * @brief Enable scaning function
 *
 * @param ctx       Click object.
 * @param en_state
 * <pre>
 * - 0x00 ( LOW )  : Enable.
 * - 0x01 ( HIGH ) : Disable.
 * </pre>
 *
 * @description The function enables or disables barcode scaning
 *              depending on state parametar value.
 ******************************************************************************/
sl_status_t mikroe_barcode2_enable_scaning (mikroe_barcode2_t *barcode2,
                                            uint8_t en_state);

/***************************************************************************//**
 * @brief Enable reset function
 *
 * @param ctx       Click object.
 * @param en_state
 * <pre>
 * - 0x00 ( LOW )  : Enable.
 * - 0x01 ( HIGH ) : Disable.
 * </pre>
 *
 * @description The function enables or disables barcode scaner reset
 *              depending on state parametar value.
 ******************************************************************************/
sl_status_t mikroe_barcode2_enable_reset (mikroe_barcode2_t *barcode2,
                                          uint8_t en_state);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_EM3080W_H_
