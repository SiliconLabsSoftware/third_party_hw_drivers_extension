/***************************************************************************//**
 * @file mikroe_cap1166.c
 * @brief SCL CAP1166 Source File
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

#include "mikroe_cap1166.h"
#include "mikroe_cap1166_config.h"

static captouch2_t captouch2;
static captouch2_cfg_t captouch2_cfg;

void mikroe_cap1166_default_cfg(void)
{
  captouch2_default_cfg(&captouch2);
}

sl_status_t mikroe_cap1166_init(mikroe_spi_handle_t spi_instance)
{
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default spi instance
  captouch2.spi.handle = spi_instance;

  // Call basic setup functions
  captouch2_cfg_setup(&captouch2_cfg);

#if defined(CAP1166_RESET_PORT) && defined(CAP1166_RESET_PIN)
  captouch2_cfg.rst = hal_gpio_pin_name(CAP1166_RESET_PORT, CAP1166_RESET_PIN);
#endif

#if defined(CAP1166_ALERT_PORT) && defined(CAP1166_ALERT_PIN)
  captouch2_cfg.alt = hal_gpio_pin_name(CAP1166_ALERT_PORT, CAP1166_ALERT_PIN);
#endif

#if defined(CAP1166_CS_PORT) && defined(CAP1166_CS_PIN)
  captouch2_cfg.cs = hal_gpio_pin_name(CAP1166_CS_PORT, CAP1166_CS_PIN);
#endif

#if (MIKROE_CAP1166_SPI_UC == 1)
  captouch2_cfg.spi_speed = MIKROE_CAP1166_SPI_BITRATE;
#endif

  if (captouch2_init(&captouch2, &captouch2_cfg) != CAPTOUCH2_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_cap1166_set_spi_instance(mikroe_spi_handle_t spi_instance)
{
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  captouch2.spi.handle = spi_instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_cap1166_read_reg(uint8_t reg_addr,
                                    uint8_t *rx_data,
                                    uint8_t n_bytes)
{
  if ((rx_data == NULL) || (n_bytes == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  captouch2_read_reg(&captouch2, reg_addr, rx_data, n_bytes);

  return SL_STATUS_OK;
}

void mikroe_cap1166_write_reg(uint8_t reg_addr, uint8_t tx_data)
{
  captouch2_write_reg(&captouch2, reg_addr, tx_data);
}

sl_status_t mikroe_cap1166_generic_transfer(uint8_t *wr_buf,
                                            uint16_t wr_len,
                                            uint8_t *rd_buf,
                                            uint16_t rd_len)
{
  if ((wr_buf == NULL) || (rd_buf == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  captouch2_generic_transfer(&captouch2, wr_buf, wr_len, rd_buf, rd_len);

  return SL_STATUS_OK;
}

sl_status_t mikroe_cap1166_detect_touch(uint8_t *in_sens)
{
  if (in_sens == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  captouch2_detect_touch(&captouch2, in_sens);

  return SL_STATUS_OK;
}

void mikroe_cap1166_set_active_mode(uint8_t analog_gain, uint8_t en_input)
{
  captouch2_set_active_mode(&captouch2, analog_gain, en_input);
}

void mikroe_cap1166_set_standby_mode(uint8_t analog_gain, uint8_t en_input)
{
  captouch2_set_standby_mode(&captouch2, analog_gain, en_input);
}

void mikroe_cap1166_set_deep_sleep_mode(void)
{
  captouch2_set_deep_sleep_mode(&captouch2);
}

uint8_t mikroe_cap1166_check_interrupt(void)
{
  return captouch2_check_interrupt(&captouch2);
}

void mikroe_cap1166_reset()
{
  captouch2_reset(&captouch2);
}
