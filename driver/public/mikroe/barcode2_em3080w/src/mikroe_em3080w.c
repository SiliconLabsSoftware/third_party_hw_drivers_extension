/***************************************************************************//**
 * @file mikroe_em3080w.c
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

#include "mikroe_em3080w_config.h"
#include "mikroe_em3080w.h"

sl_status_t mikroe_barcode2_init(mikroe_barcode2_t *barcode2,
                                 mikroe_uart_handle_t uart_handle,
                                 uint16_t baud_rate)
{
  barcode2_cfg_t cfg;

  if (!barcode2 || !uart_handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  barcode2_cfg_setup(&cfg);
  cfg.rst = hal_gpio_pin_name(CONFIG_EM3080W_RST_PORT, CONFIG_EM3080W_RST_PIN);
  cfg.trg = hal_gpio_pin_name(CONFIG_EM3080W_TRG_PORT, CONFIG_EM3080W_TRG_PIN);
  if (baud_rate > 0) {
    cfg.baud_rate = baud_rate;
  }
  barcode2->uart.handle = uart_handle;
  if (barcode2_init(barcode2, &cfg) != BARCODE2_OK) {
    return SL_STATUS_INITIALIZATION;
  }
  return SL_STATUS_OK;
}

sl_status_t mikroe_barcode2_generic_write(mikroe_barcode2_t *barcode2,
                                          char *data_buf,
                                          uint16_t len)
{
  if (!barcode2 || !data_buf || !len) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  barcode2_generic_write(barcode2, data_buf, len);
  return SL_STATUS_OK;
}

int32_t mikroe_barcode2_generic_read(mikroe_barcode2_t *barcode2,
                                     char *data_buf,
                                     uint16_t max_len)
{
  if (!barcode2 || !data_buf || !max_len) {
    return BARCODE2_INVALID_PARAMETER;
  }
  return barcode2_generic_read(barcode2, data_buf, max_len);
}

sl_status_t mikroe_barcode2_enable_scaning(mikroe_barcode2_t *barcode2,
                                           uint8_t en_state)
{
  if (!barcode2) {
    return BARCODE2_INVALID_PARAMETER;
  }
  barcode2_enable_scaning(barcode2, en_state);
  return SL_STATUS_OK;
}

sl_status_t mikroe_barcode2_enable_reset(mikroe_barcode2_t *barcode2,
                                         uint8_t en_state)
{
  if (!barcode2) {
    return BARCODE2_INVALID_PARAMETER;
  }
  barcode2_enable_reset(barcode2, en_state);
  return SL_STATUS_OK;
}
