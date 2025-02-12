/***************************************************************************//**
 * @file drv_uart.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - UART
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

#include <string.h>
#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif
#include "sl_atomic.h"

#include "drv_uart.h"
#include "sl_status.h"
#include "sl_iostream.h"
#include "sl_iostream_uart.h"
#include "sl_iostream_usart.h"

// Compatibility layer for peripheral and em lib
#if defined(LDMA0)
#define LDMA_PERIPH   LDMA0
#else
#define LDMA_PERIPH   LDMA
#endif

static inline size_t uart_get_data_available(
  sl_iostream_uart_context_t *uart_context);
static sl_status_t uart_clear_rx_buffer(sl_iostream_t *stream);
static void uart_config_baudrate(uart_t *obj);
static void uart_config_frame(uart_t *obj);

static uart_t *_owner = NULL;

static err_t _acquire(uart_t *obj, bool obj_open_state)
{
  err_t status = ACQUIRE_SUCCESS;

  if ((obj_open_state == true) && (_owner == obj)) {
    return ACQUIRE_FAIL;
  }

  if (_owner != obj) {
    _owner = obj;
  }

  return status;
}

void uart_configure_default(uart_config_t *config)
{
  config->tx_pin = 0xFFFFFFFF;
  config->rx_pin = 0xFFFFFFFF;

  config->baud = 115200;
  config->data_bits = UART_DATA_BITS_DEFAULT;
  config->parity = UART_PARITY_DEFAULT;
  config->stop_bits = UART_STOP_BITS_DEFAULT;
  config->tx_ring_size = 0;
  config->rx_ring_size = 0;
}

err_t uart_open(uart_t *obj, uart_config_t *config)
{
  uart_config_t *p_config = &obj->config;
  memcpy(p_config, config, sizeof(uart_config_t));
  sl_iostream_uart_t *ptr = (sl_iostream_uart_t *)obj->handle;

  // Configure baudrate.
  uart_config_baudrate(obj);

  // Configure frame.
  uart_config_frame(obj);

  // Don't use auto cr lf
  sl_iostream_uart_set_auto_cr_lf(ptr, false);

  // Default to blocking mode
  uart_set_blocking(obj, true);

  return _acquire(obj, true);
}

err_t uart_set_baud(uart_t *obj, uint32_t baud)
{
  if (baud > 0) {
    if (_acquire(obj, false) != ACQUIRE_FAIL) {
      obj->config.baud = baud;
      // Configure baudrate.
      uart_config_baudrate(obj);
      return UART_SUCCESS;
    }
  }
  return UART_ERROR;
}

err_t uart_set_parity(uart_t *obj, uart_parity_t parity)
{
  if (parity <= UART_PARITY_ODD) {
    if (_acquire(obj, false) != ACQUIRE_FAIL) {
      obj->config.parity = parity;

      // Configure frame.
      uart_config_frame(obj);
      return UART_SUCCESS;
    }
  }
  return UART_ERROR;
}

err_t uart_set_stop_bits(uart_t *obj, uart_stop_bits_t stop)
{
  if (stop <= UART_STOP_BITS_TWO) {
    if (_acquire(obj, false) != ACQUIRE_FAIL) {
      obj->config.stop_bits = stop;

      // Configure frame.
      uart_config_frame(obj);
      return UART_SUCCESS;
    }
  }
  return UART_ERROR;
}

err_t uart_set_data_bits(uart_t *obj, uart_data_bits_t bits)
{
  if (bits <= UART_DATA_BITS_9) {
    if (_acquire(obj, false) != ACQUIRE_FAIL) {
      obj->config.data_bits = bits;

      // Configure frame.
      uart_config_frame(obj);
      return UART_SUCCESS;
    }
  }
  return UART_ERROR;
}

void uart_set_blocking(uart_t *obj, bool blocking)
{
#if (defined(SL_CATALOG_KERNEL_PRESENT))
  sl_iostream_uart_t *ptr = (sl_iostream_uart_t *)obj->handle;
  sl_iostream_uart_set_read_block(ptr, blocking);
#endif
  obj->is_blocking = blocking;
}

err_t uart_write(uart_t *obj, uint8_t *buffer, size_t size)
{
  sl_status_t status;

  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    sl_iostream_uart_t *ptr = (sl_iostream_uart_t *)obj->handle;
    status = sl_iostream_write(&(ptr->stream),
                               buffer,
                               size);
    if (SL_STATUS_OK == status) {
      return size;
    }
  }
  return UART_ERROR;
}

err_t uart_read(uart_t *obj, uint8_t *buffer, size_t size)
{
  sl_status_t status;
  size_t read_size;
  int32_t total_size = 0;

  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    sl_iostream_uart_t *ptr = (sl_iostream_uart_t *)obj->handle;
    if (obj->is_blocking) {
      while (size) {
        status = sl_iostream_read(&(ptr->stream),
                                  buffer,
                                  size,
                                  &read_size);
        if (SL_STATUS_OK == status) {
          if (read_size > 0) {
            if (read_size <= size) {
              size -= read_size;
              buffer += read_size;
              total_size += read_size;
            } else {
              size = 0;
            }
          }
        }
      }
    } else {   // Non-blocking read
      status = sl_iostream_read(&(ptr->stream),
                                buffer,
                                size,
                                &read_size);
      if (SL_STATUS_OK == status) {
        total_size = read_size;
      }
    }
    return total_size;
  }
  return UART_ERROR;
}

err_t uart_print(uart_t *obj, char *text)
{
  size_t data_written = 0;

  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    sl_iostream_uart_t *ptr = (sl_iostream_uart_t *)obj->handle;
    while (*text) {
      if (SL_STATUS_OK != sl_iostream_write(&(ptr->stream), text++, 1)) {
        return data_written;
      }
      data_written++;
    }
    return data_written;
  } else {
    return UART_ERROR;
  }
}

err_t uart_println(uart_t *obj, char *text)
{
  size_t data_written = 0;

  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    sl_iostream_uart_t *ptr = (sl_iostream_uart_t *)obj->handle;
    while (*text) {
      if (SL_STATUS_OK != sl_iostream_write(&(ptr->stream), text++, 1)) {
        return data_written;
      }
      data_written++;
    }
    if (SL_STATUS_OK != sl_iostream_write(&(ptr->stream), (uint8_t *)"\r\n",
                                          2)) {
      return data_written;
    }
    data_written += 2;
    return data_written;
  } else {
    return UART_ERROR;
  }
}

size_t uart_bytes_available(uart_t *obj)
{
  sl_iostream_uart_t *ptr = (sl_iostream_uart_t *)obj->handle;
  sl_iostream_usart_context_t *ctx =
    (sl_iostream_usart_context_t *)ptr->stream.context;

  return uart_get_data_available(&(ctx->context));
}

void uart_clear(uart_t *obj)
{
  sl_iostream_uart_t *ptr = (sl_iostream_uart_t *)obj->handle;
  uart_clear_rx_buffer(&(ptr->stream));
}

void uart_close(uart_t *obj)
{
  obj->handle = NULL;
  _owner = NULL;
}

/***************************************************************************//**
 * Get the next byte to be written to by the (L)DMA.
 *
 * @note Function should only be called if the LDMA is NOT in the new data detect
 * mode.
 ******************************************************************************/
static inline uint8_t * __get_write_ptr(
  const sl_iostream_uart_context_t *uart_context)
{
  uint8_t *write_ptr = NULL;
  Ecode_t ecode;
  bool dma_done;

  ecode = DMADRV_TransferDone(uart_context->dma.channel, &dma_done);
  EFM_ASSERT(ecode == ECODE_OK);

  if (dma_done) {
    // When the DMA is completely done, it has wrapped over the circular buffer
    // and filled it up completely.
    write_ptr = uart_context->rx_read_ptr;
  } else {
    write_ptr = (uint8_t *)LDMA_PERIPH->CH[uart_context->dma.channel].DST;
  }

  // Sanity check for buffer over/underflow
  EFM_ASSERT(write_ptr <= (uart_context->rx_buffer + uart_context->rx_buffer_len)
             && write_ptr >= uart_context->rx_buffer);

  // Wrap dst around
  if (write_ptr == (uart_context->rx_buffer + uart_context->rx_buffer_len)) {
    write_ptr = uart_context->rx_buffer;
  }

  return write_ptr;
}

/***************************************************************************//**
 * Compute how many bytes available to read in UART ring buffer.
 *
 * @note Caller must ensure that rx buffer was not empty prior to calling,
 * or will hit an assert.
 ******************************************************************************/
static inline size_t uart_get_data_available(
  sl_iostream_uart_context_t *uart_context)
{
  const uint8_t *write_ptr = __get_write_ptr(uart_context);

  if (write_ptr > uart_context->rx_read_ptr) {
    // Read data between read_ptr and write_ptr
    return write_ptr - uart_context->rx_read_ptr;
  } else {
    // write_ptr wrapped around. Read data from read_ptr to end of buffer.
    // Sanity check that the read pointer didn't overflow.
    EFM_ASSERT(uart_context->rx_read_ptr
               < (uart_context->rx_buffer + uart_context->rx_buffer_len));

    return (uart_context->rx_buffer + uart_context->rx_buffer_len)
           - uart_context->rx_read_ptr;
  }
}

static sl_status_t uart_clear_rx_buffer(sl_iostream_t *stream)
{
  sl_iostream_usart_context_t *context =
    (sl_iostream_usart_context_t *)stream->context;
  size_t data_size;

  // Empty read buffer
  data_size = uart_get_data_available(&(context->context));
  while (data_size > 0) {
    uint8_t tmp;
    size_t read_size = 0;

    if (SL_STATUS_OK != sl_iostream_read(stream,
                                         &tmp,
                                         1,
                                         &read_size)) {
      break;
    }
    if (read_size == 0) {
      break;
    }
    data_size--;
  }

  return SL_STATUS_OK;
}

static void uart_config_baudrate(uart_t *obj)
{
  sl_iostream_uart_t *ptr = (sl_iostream_uart_t *)obj->handle;
  sl_iostream_usart_context_t *ctx =
    (sl_iostream_usart_context_t *)ptr->stream.context;

  // Configure baudrate.
  USART_BaudrateAsyncSet(ctx->usart,
                         0,
                         obj->config.baud,
                         usartOVS4);
}

static void uart_config_frame(uart_t *obj)
{
  sl_iostream_uart_t *ptr = (sl_iostream_uart_t *)obj->handle;
  sl_iostream_usart_context_t *ctx =
    (sl_iostream_usart_context_t *)ptr->stream.context;
  uint32_t parity, stopbits, databits;

  switch (obj->config.data_bits) {
    case UART_DATA_BITS_7:
      databits = USART_FRAME_DATABITS_SEVEN;
      break;
    case UART_DATA_BITS_8:
      databits = USART_FRAME_DATABITS_EIGHT;
      break;
    case UART_DATA_BITS_9:
      databits = USART_FRAME_DATABITS_NINE;
      break;
    default:
      databits = ctx->usart->FRAME & _USART_FRAME_MASK;
      break;
  }
  switch (obj->config.parity) {
    case UART_PARITY_NONE:
      parity = USART_FRAME_PARITY_NONE;
      break;
    case UART_PARITY_EVEN:
      parity = USART_FRAME_PARITY_EVEN;
      break;
    case UART_PARITY_ODD:
      parity = USART_FRAME_PARITY_ODD;
      break;
    default:
      parity = ctx->usart->FRAME & _USART_FRAME_PARITY_MASK;
      break;
  }
  switch (obj->config.stop_bits) {
    case UART_STOP_BITS_HALF:
      stopbits = USART_FRAME_STOPBITS_HALF;
      break;
    case UART_STOP_BITS_ONE:
      stopbits = USART_FRAME_STOPBITS_ONE;
      break;
    case UART_STOP_BITS_ONE_AND_A_HALF:
      stopbits = USART_FRAME_STOPBITS_ONEANDAHALF;
      break;
    case UART_STOP_BITS_TWO:
      stopbits = USART_FRAME_STOPBITS_TWO;
      break;
    default:
      stopbits = ctx->usart->FRAME & _USART_FRAME_STOPBITS_MASK;
      break;
  }

  /* Configure databits, leave stopbits and parity. */
  ctx->usart->FRAME = databits | stopbits | parity;
}

// ------------------------------------------------------------------------- END
