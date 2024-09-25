/***************************************************************************//**
 * @file drv_uart.h
 * @brief mikroSDK 2.0 Click Peripheral Drivers
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

#ifndef _DRV_UART_H_
#define _DRV_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "drv_name.h"

typedef const void *mikroe_uart_handle_t; ///< Created UART handle type

typedef enum
{
  UART_SUCCESS = 0,
  UART_ERROR = (-1)
} uart_err_t;

typedef enum
{
  UART_DATA_BITS_7 = 0,
  UART_DATA_BITS_8,
  UART_DATA_BITS_9,

  UART_DATA_BITS_DEFAULT = UART_DATA_BITS_8
} uart_data_bits_t;

typedef enum
{
  UART_PARITY_NONE = 0,
  UART_PARITY_EVEN,
  UART_PARITY_ODD,

  UART_PARITY_DEFAULT = UART_PARITY_NONE
} uart_parity_t;

typedef enum
{
  UART_STOP_BITS_HALF = 0,
  UART_STOP_BITS_ONE,
  UART_STOP_BITS_ONE_AND_A_HALF,
  UART_STOP_BITS_TWO,

  UART_STOP_BITS_DEFAULT = UART_STOP_BITS_ONE
} uart_stop_bits_t;

typedef struct
{
  pin_name_t tx_pin;
  pin_name_t rx_pin;

  uint32_t baud;
  uart_data_bits_t data_bits;
  uart_parity_t parity;
  uart_stop_bits_t stop_bits;
  size_t tx_ring_size;
  size_t rx_ring_size;
} uart_config_t;

typedef struct
{
  mikroe_uart_handle_t handle;
  void *tx_ring_buffer;
  void *rx_ring_buffer;
  uart_config_t config;
  bool is_tx_irq_enabled;
  bool is_rx_irq_enabled;
  bool is_blocking;
} uart_t;

void uart_configure_default(uart_config_t *config);
err_t uart_open(uart_t *obj, uart_config_t *config);
err_t uart_set_baud(uart_t *obj, uint32_t baud);
err_t uart_set_parity(uart_t *obj, uart_parity_t parity);
err_t uart_set_stop_bits(uart_t *obj, uart_stop_bits_t stop);
err_t uart_set_data_bits(uart_t *obj, uart_data_bits_t bits);
void uart_set_blocking(uart_t *obj, bool blocking);
err_t uart_write(uart_t *obj, uint8_t *buffer, size_t size);
err_t uart_print(uart_t *obj, char *text);
err_t uart_println(uart_t *obj, char *text);
err_t uart_read(uart_t *obj, uint8_t *buffer, size_t size);
size_t uart_bytes_available(uart_t *obj);
void uart_clear(uart_t *obj);
void uart_close(uart_t *obj);

#ifdef __cplusplus
}
#endif

#endif // _DRV_UART_H_
// ------------------------------------------------------------------------- END
