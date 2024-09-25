/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensirion_uart_hal.h"
#include "sl_status.h"

static uart_t s_sensirion_uart;
static uint8_t s_sensirion_tx_buffer[256];
static uint8_t s_sensirion_rx_buffer[256];

/*
 * INSTRUCTIONS
 * ============
 *
 * Implement all functions where they are marked with TODO: implement
 * Follow the function specification in the comments.
 */

/**
 * Initialize all hard- and software components that are needed for the UART
 * communication.
 */
sl_status_t sensirion_uart_init(mikroe_uart_handle_t uart_handle)
{
  uint32_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != uart_handle) {
    uart_config_t cfg;

    s_sensirion_uart.handle = uart_handle;
    s_sensirion_uart.tx_ring_buffer = s_sensirion_tx_buffer;
    s_sensirion_uart.rx_ring_buffer = s_sensirion_rx_buffer;
    s_sensirion_uart.is_blocking = false;

    uart_configure_default(&cfg);

    cfg.tx_ring_size = sizeof(s_sensirion_tx_buffer);
    cfg.rx_ring_size = sizeof(s_sensirion_rx_buffer);

    if (uart_open(&s_sensirion_uart, &cfg) != 0) {
      return SL_STATUS_FAIL;
    }

    uart_set_blocking(&s_sensirion_uart, false);

    stt = SL_STATUS_OK;
  }
  return stt;
}

/**
 * sensirion_uart_select_port() - select the UART port index to use
 *                                THE IMPLEMENTATION IS OPTIONAL ON SINGLE-PORT
 *                                SETUPS (only one SPS30)
 *
 * Return:      SL_STATUS_OK on success, an error code otherwise
 */
sl_status_t sensirion_uart_select_port(uint8_t port)
{
  (void)port;
  return SL_STATUS_OK;
}

/**
 * sensirion_uart_open() - initialize UART
 *
 * Return:      SL_STATUS_OK on success, an error code otherwise
 */
sl_status_t sensirion_uart_open()
{
  return SL_STATUS_OK;
}

/**
 * sensirion_uart_close() - release UART resources
 *
 * Return:      SL_STATUS_OK on success, an error code otherwise
 */
sl_status_t sensirion_uart_close()
{
  // TODO: implement
  return SL_STATUS_OK;
}

/**
 * sensirion_uart_tx() - transmit data over UART
 *
 * @data_len:   number of bytes to send
 * @data:       data to send
 * @tx_bytes:   Actual number of bytes sent
 * @Return:     SL_STATUS_OK on success, an error code otherwise
 */
sl_status_t sensirion_uart_tx(uint16_t data_len,
                              const uint8_t *data,
                              uint16_t *tx_bytes)
{
  if ((NULL != data) && (NULL != s_sensirion_uart.handle)) {
    err_t status = uart_write(&s_sensirion_uart, (uint8_t *)data, data_len);

    if (UART_ERROR != status) {
      if (NULL != tx_bytes) {
        *tx_bytes = status;
      }
      return SL_STATUS_OK;
    }
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**
 * sensirion_uart_rx() - receive data over UART
 *
 * @data_len:   max number of bytes to receive
 * @data:       Memory where received data is stored
 * @rx_bytes:   Actual number of bytes received
 * @Return:     SL_STATUS_OK on success, an error code otherwise
 */
sl_status_t sensirion_uart_rx(uint16_t max_data_len,
                              uint8_t *data,
                              uint16_t *rx_bytes)
{
  if ((NULL != data) && (NULL != s_sensirion_uart.handle)) {
    err_t status = uart_read(&s_sensirion_uart, data, max_data_len);

    if (UART_ERROR != status) {
      if (NULL != rx_bytes) {
        *rx_bytes = status;
      }
      return SL_STATUS_OK;
    }
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_INVALID_PARAMETER;
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * Despite the unit, a <10 millisecond precision is sufficient.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(uint32_t useconds)
{
  uint32_t delay_ms = 1;

  if (useconds > 1000) {
    delay_ms = (useconds / 1000) + 1;
  }

  sl_sleeptimer_delay_millisecond(delay_ms);
}
