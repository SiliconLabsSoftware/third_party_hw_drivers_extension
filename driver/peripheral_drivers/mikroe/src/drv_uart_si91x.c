/***************************************************************************//**
 * @file drv_uart_si91x.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - UART for Si91x
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
#include "sl_core.h"
#include "sl_si91x_usart.h"
#include "drv_uart.h"
#include "ring.h"

static uart_t *_owner = NULL;
static sl_usart_handle_t drv_usart_handle;
static sl_si91x_usart_control_config_t drv_usart_config;

static volatile boolean_t usart_send_complete = false;
static volatile boolean_t is_rx_enable = false;
static ring_buf8_t ring_rx_handle;
static uint8_t rx_buffer;

static usart_parity_typedef_t uart_parity_mapping(uart_parity_t parity);
static usart_stopbit_typedef_t uart_stopbit_mapping(uart_stop_bits_t stop_bits);
static usart_databits_typedef_t uart_databits_mapping(
  uart_data_bits_t data_bits);
void usart_callback_event(uint32_t event);

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
  if (_acquire(obj, true) == ACQUIRE_FAIL) {
    return UART_ERROR;
  }

  sl_status_t status;

  drv_usart_config.baudrate = obj->config.baud;
  drv_usart_config.parity = uart_parity_mapping(obj->config.parity);
  drv_usart_config.stopbits = uart_stopbit_mapping(obj->config.stop_bits);
  drv_usart_config.databits = uart_databits_mapping(obj->config.data_bits);
  drv_usart_config.mode = SL_USART_MODE_ASYNCHRONOUS;
  drv_usart_config.hwflowcontrol = SL_USART_FLOW_CONTROL_NONE;
  drv_usart_config.usart_module = *(uint32_t *)obj->handle;

  is_rx_enable = false;
  ring_buf8_init(&ring_rx_handle, obj->rx_ring_buffer,
                 obj->config.rx_ring_size);

  // Initialize the UART
  status =
    sl_si91x_usart_init((usart_peripheral_t)drv_usart_config.usart_module,
                        &drv_usart_handle);
  if (status != SL_STATUS_OK) {
    return UART_ERROR;
  }
  // Configure the USART configurations
  status = sli_si91x_usart_set_non_uc_configuration(drv_usart_handle,
                                                    &drv_usart_config);
  if (status != SL_STATUS_OK) {
    return UART_ERROR;
  }
  // Register user callback function
  status =
    sl_si91x_usart_multiple_instance_register_event_callback(
      (usart_peripheral_t)drv_usart_config.usart_module,
      usart_callback_event);
  if (status != SL_STATUS_OK) {
    return UART_ERROR;
  }

  return UART_SUCCESS;
}

err_t uart_set_baud(uart_t *obj, uint32_t baud)
{
  if ((_acquire(obj, false) == ACQUIRE_FAIL) || (baud == 0)) {
    return UART_ERROR;
  }
  sl_status_t status;

  obj->config.baud = baud;
  drv_usart_config.baudrate = baud;
  // Configure the USART configurations
  status = sli_si91x_usart_set_non_uc_configuration(drv_usart_handle,
                                                    &drv_usart_config);
  if (status != SL_STATUS_OK) {
    return UART_ERROR;
  }
  return UART_SUCCESS;
}

err_t uart_set_parity(uart_t *obj, uart_parity_t parity)
{
  if ((_acquire(obj, false) == ACQUIRE_FAIL) || (parity > UART_PARITY_ODD)) {
    return UART_ERROR;
  }
  sl_status_t status;

  obj->config.parity = parity;
  drv_usart_config.parity = uart_parity_mapping(parity);
  // Configure the USART configurations
  status = sli_si91x_usart_set_non_uc_configuration(drv_usart_handle,
                                                    &drv_usart_config);
  if (status != SL_STATUS_OK) {
    return UART_ERROR;
  }
  return UART_SUCCESS;
}

err_t uart_set_stop_bits(uart_t *obj, uart_stop_bits_t stop)
{
  if ((_acquire(obj, false) == ACQUIRE_FAIL) || (stop > UART_STOP_BITS_TWO)) {
    return UART_ERROR;
  }
  sl_status_t status;

  obj->config.stop_bits = stop;
  drv_usart_config.stopbits = uart_stopbit_mapping(stop);
  // Configure the USART configurations
  status = sli_si91x_usart_set_non_uc_configuration(drv_usart_handle,
                                                    &drv_usart_config);
  if (status != SL_STATUS_OK) {
    return UART_ERROR;
  }
  return UART_SUCCESS;
}

err_t uart_set_data_bits(uart_t *obj, uart_data_bits_t bits)
{
  if ((_acquire(obj, false) == ACQUIRE_FAIL) || (bits > UART_DATA_BITS_9)) {
    return UART_ERROR;
  }
  sl_status_t status;

  obj->config.data_bits = bits;
  drv_usart_config.databits = uart_databits_mapping(bits);
  // Configure the USART configurations
  status = sli_si91x_usart_set_non_uc_configuration(drv_usart_handle,
                                                    &drv_usart_config);
  if (status != SL_STATUS_OK) {
    return UART_ERROR;
  }
  return UART_SUCCESS;
}

void uart_set_blocking(uart_t *obj, bool blocking)
{
  obj->is_blocking = blocking;
}

err_t uart_write(uart_t *obj, uint8_t *buffer, size_t size)
{
  if ((_acquire(obj, false) == ACQUIRE_FAIL)
      || (buffer == NULL) || (size == 0)) {
    return UART_ERROR;
  }

  sl_status_t status;

  // Transfer the char
  status = sl_si91x_usart_send_data(drv_usart_handle, buffer, size);
  if (status != SL_STATUS_OK) {
    return UART_ERROR;
  }

  while (!usart_send_complete) {}
  // Clear the variable to send next buffer
  usart_send_complete = false;

  return size;
}

err_t uart_read(uart_t *obj, uint8_t *buffer, size_t size)
{
  if ((_acquire(obj, false) == ACQUIRE_FAIL)
      || (buffer == NULL) || (size == 0)) {
    return UART_ERROR;
  }

  sl_status_t status;
  size_t data_read = 0;

  // Enable module rx, if it's disabled
  if (!is_rx_enable) {
    is_rx_enable = true;
    status = sl_si91x_usart_receive_data(drv_usart_handle, &rx_buffer, 1);
    if (status != SL_STATUS_OK) {
      is_rx_enable = false;
      return UART_ERROR;
    }
  }

  // Wait for some data to be received to the buffer if in blocking mode.
  while (ring_buf8_is_empty(&ring_rx_handle)) {
    if (!obj->is_blocking) {
      return 0;
    }
    Delay_1ms();
  }

  while ((size > 0) && !ring_buf8_is_empty(&ring_rx_handle)) {
    uint8_t data_byte;
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    data_byte = ring_buf8_pop(&ring_rx_handle);

    if (!is_rx_enable && !ring_buf8_is_full(&ring_rx_handle)) {
      is_rx_enable = true;
      status = sl_si91x_usart_receive_data(drv_usart_handle, &rx_buffer, 1);
      if (status != SL_STATUS_OK) {
        is_rx_enable = false;
        CORE_EXIT_CRITICAL();
        return UART_ERROR;
      }
    }
    CORE_EXIT_CRITICAL();

    buffer[data_read++] = data_byte;
    size--;
  }
  return data_read;
}

err_t uart_print(uart_t *obj, char *text)
{
  size_t data_written = 0;

  if ((_acquire(obj, false) == ACQUIRE_FAIL) || (text == NULL)) {
    return UART_ERROR;
  }
  while (text[data_written] != '\0') {
    if (uart_write(obj, (uint8_t *)&text[data_written], 1) != 1) {
      return data_written;
    }
    data_written++;
  }

  return data_written;
}

err_t uart_println(uart_t *obj, char *text)
{
  if ((_acquire(obj, false) == ACQUIRE_FAIL) || (text == NULL)) {
    return UART_ERROR;
  }

  err_t data_written = uart_print(obj, text);

  if (data_written < 0) {
    return UART_ERROR;
  }

  return data_written + uart_print(obj, "\r\n");
}

size_t uart_bytes_available(uart_t *obj)
{
  if (_acquire(obj, false) == ACQUIRE_FAIL) {
    return UART_ERROR;
  }

  return ring_buf8_size(&ring_rx_handle);
}

void uart_clear(uart_t *obj)
{
  ring_buf8_clear(&ring_rx_handle);
  (void) obj;
}

void uart_close(uart_t *obj)
{
  obj->handle = NULL;
  _owner = NULL;
}

static usart_parity_typedef_t uart_parity_mapping(uart_parity_t parity)
{
  usart_parity_typedef_t val;

  switch (parity) {
    default:
    case UART_PARITY_NONE:
      val = SL_USART_NO_PARITY;
      break;
    case UART_PARITY_EVEN:
      val = SL_USART_EVEN_PARITY;
      break;
    case UART_PARITY_ODD:
      val = SL_USART_ODD_PARITY;
      break;
  }
  return val;
}

static usart_stopbit_typedef_t uart_stopbit_mapping(uart_stop_bits_t stop_bits)
{
  usart_stopbit_typedef_t val;

  switch (stop_bits) {
    default:
    case UART_STOP_BITS_ONE:
      val = SL_USART_STOP_BITS_1;
      break;
    case UART_STOP_BITS_ONE_AND_A_HALF:
      val = SL_USART_STOP_BITS_1_5;
      break;
    case UART_STOP_BITS_TWO:
      val = SL_USART_STOP_BITS_2;
      break;
  }
  return val;
}

static usart_databits_typedef_t uart_databits_mapping(
  uart_data_bits_t data_bits)
{
  usart_databits_typedef_t val;

  switch (data_bits) {
    default:
    case UART_DATA_BITS_7:
      val = SL_USART_DATA_BITS_7;
      break;
    case UART_DATA_BITS_8:
      val = SL_USART_DATA_BITS_8;
      break;
  }
  return val;
}

/*******************************************************************************
 * Callback function triggered on data Transfer and reception
 ******************************************************************************/
void usart_callback_event(uint32_t event)
{
  switch (event) {
    case SL_USART_EVENT_SEND_COMPLETE:
      usart_send_complete = true;
      break;
    case SL_USART_EVENT_RECEIVE_COMPLETE:
      if (ring_buf8_is_full(&ring_rx_handle)) {
        is_rx_enable = false;
        return;
      }
      ring_buf8_push(&ring_rx_handle, rx_buffer);
      if (is_rx_enable) {
        sl_si91x_usart_receive_data(drv_usart_handle, &rx_buffer, 1);
      }
      break;
  }
}

// ------------------------------------------------------------------------- END
