/***************************************************************************//**
 * @file mikroe_stn1110.c
 * @brief SCL obdii Prototypes
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

#include "mikroe_stn1110.h"
#include "mikroe_stn1110_config.h"

static obdii_t obdii;
static obdii_cfg_t obdii_cfg;

sl_status_t mikroe_stn1110_init(mikroe_uart_handle_t handle)
{
  if (NULL == handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  obdii.uart.handle = handle;
  obdii_cfg_setup(&obdii_cfg);

#if defined (STN1110_RESET_PORT) && defined(STN1110_RESET_PIN)
  obdii_cfg.rst = hal_gpio_pin_name(STN1110_RESET_PORT,
                                    STN1110_RESET_PIN);
#endif

#if defined (STN1110_INT_PORT) && defined(STN1110_INT_PIN)
  obdii_cfg.int_pin = hal_gpio_pin_name(STN1110_INT_PORT,
                                        STN1110_INT_PIN);
#endif

  if (obdii_init(&obdii, &obdii_cfg) != UART_SUCCESS) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_stn1110_set_uart_instance(mikroe_uart_handle_t handle)
{
  if (NULL == handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  obdii.uart.handle = handle;

  return SL_STATUS_OK;
}

sl_status_t mikroe_stn1110_generic_write(uint8_t *data_in, uint16_t len)
{
  if (NULL == data_in) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return obdii_generic_write(&obdii, data_in, len);
}

sl_status_t mikroe_stn1110_generic_read(uint8_t *data_out, uint16_t len)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return obdii_generic_read(&obdii, data_out, len);
}

sl_status_t mikroe_stn1110_send_command(uint8_t *cmd)
{
  if (NULL == cmd) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  obdii_send_command(&obdii, cmd);

  return SL_STATUS_OK;
}

void mikroe_stn1110_reset_device(void)
{
  obdii_reset_device(&obdii);
}

void mikroe_stn1110_set_rst_pin(uint8_t state)
{
  obdii_set_rst_pin(&obdii, state);
}

uint8_t mikroe_stn1110_get_int_pin(void)
{
  return obdii_get_int_pin(&obdii);
}
