/***************************************************************************//**
 * @file mikroe_pl_n823_01.h
 * @brief SCL Mikroe PIR PL_N823_01 header file
 * @version 1.2.0
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "mikroe_ra_08.h"
#include "mikroe_ra_08_config.h"

static lr9_t lr;
static lr9_cfg_t lr_cfg;

sl_status_t mikroe_ra_08_init(mikroe_uart_handle_t instance)
{
  if (instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  lr.uart.handle = instance;
  lr9_cfg_setup(&lr_cfg);

#if defined (LR9_RESET_PORT) && defined(LR9_RESET_PIN)
  lr_cfg.rst = hal_gpio_pin_name(LR9_RESET_PORT,
                                 LR9_RESET_PIN);
#endif

#if defined (LR9_BT_PORT) && defined(LR9_BT_PIN)
  lr_cfg.bt = hal_gpio_pin_name(LR9_BT_PORT,
                                LR9_BT_PIN);
#endif

  if (LR9_OK != lr9_init(&lr, &lr_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

void mikroe_ra_08_hw_reset(void)
{
  lr9_hw_reset(&lr);
}

void mikroe_ra_08_set_boot_pin(uint8_t state)
{
  lr9_set_boot_pin(&lr, state);
}

sl_status_t mikroe_ra_08_generic_write(uint8_t *data_in, uint16_t len)
{
  if (data_in == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (lr9_generic_write(&lr, data_in, len) == LR9_OK) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_ra_08_generic_read(uint8_t *data_out,
                                      uint16_t len,
                                      int32_t *rx_size)
{
  if (data_out == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *rx_size = lr9_generic_read(&lr, data_out, len);
  return (*rx_size > 0) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_ra_08_write_command(uint8_t *command, uint8_t *param)
{
  if ((command == NULL) || (param == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (lr9_write_command(&lr, command, param) >= 0) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_ra_08_inquire_command(uint8_t *command,
                                         mikroe_ra_08_en_set_t en_set)
{
  if (command == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (lr9_inquire_command(&lr, command,
                              (lr9_en_set_t)en_set) >= 0) ? SL_STATUS_OK
         : SL_STATUS_FAIL;
}

sl_status_t mikroe_ra_08_send_data_frame(uint8_t confirm, uint8_t nbtrials,
                                         uint8_t *data_frame)
{
  if (data_frame == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (lr9_send_data_frame(&lr, confirm, nbtrials, data_frame) == LR9_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}
