/***************************************************************************//**
 * @file mikroe_lea6s.c
 * @brief mikroe_lea6s.c
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
#include "mikroe_lea6s.h"
#include "mikroe_gps_lea6s_config.h"

static gps_t mikroe_lea6s;
static gps_cfg_t mikroe_lea6s_cfg;

sl_status_t mikroe_lea6s_init(mikroe_uart_handle_t uart_handle, bool blocking)
{
  if (NULL == uart_handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  gps_cfg_setup(&mikroe_lea6s_cfg);

  mikroe_lea6s_cfg.reset = hal_gpio_pin_name(MIKROE_LEAS6_RESET_PORT,
                                             MIKROE_LEAS6_RESET_PIN);

  mikroe_lea6s_cfg.tmpls = hal_gpio_pin_name(MIKROE_LEAS6_TIMEPULSE_PORT,
                                             MIKROE_LEAS6_TIMEPULSE_PIN);

  mikroe_lea6s_cfg.uart_blocking = blocking;
  mikroe_lea6s.uart.handle = uart_handle;
  if (gps_init(&mikroe_lea6s, &mikroe_lea6s_cfg) != GPS_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

void mikroe_lea6s_wakeup(void)
{
  gps_module_wakeup(&mikroe_lea6s);
}

void mikroe_lea6s_set_rst_pin(uint8_t state)
{
  gps_set_rst_pin(&mikroe_lea6s, state);
}

uint8_t mikroe_lea6s_get_tmpls_pin(void)
{
  return gps_get_tmpls_pin(&mikroe_lea6s);
}

sl_status_t mikroe_lea6s_generic_write(uint8_t *data_buf, uint16_t len)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if (NULL != data_buf) {
    gps_generic_write(&mikroe_lea6s, (char *) data_buf, len);
    stt = SL_STATUS_OK;
  }

  return stt;
}

sl_status_t mikroe_lea6s_generic_read(uint8_t *data_buf,
                                      uint16_t max_len,
                                      uint16_t *actual_len)
{
  sl_status_t stt = SL_STATUS_INVALID_PARAMETER;

  if ((NULL != data_buf) && (NULL != actual_len)) {
    int32_t retval = gps_generic_read(&mikroe_lea6s,
                                      (char *) data_buf,
                                      max_len);
    if (UART_ERROR == retval) {
      stt = SL_STATUS_FAIL;
    } else {
      *actual_len = (uint16_t)retval;
      stt = SL_STATUS_OK;
    }
  }

  return stt;
}

mikroe_leas6_parser_result_t
mikroe_lea6s_generic_parser(uint8_t *raw_data_buffer,
                            mikroe_leas6_gps_command_t command,
                            mikroe_leas6_gpgga_command_elements_t element,
                            uint8_t *element_parser_buffer)
{
  mikroe_leas6_parser_result_t stt = gps_parser_invalid_input_parameter_e;

  if ((NULL != raw_data_buffer) && (NULL != element_parser_buffer)) {
    stt = (mikroe_leas6_parser_result_t)
          gps_generic_parser((char *)raw_data_buffer,
                             command,
                             element,
                             (char *)element_parser_buffer);
  }

  return stt;
}
