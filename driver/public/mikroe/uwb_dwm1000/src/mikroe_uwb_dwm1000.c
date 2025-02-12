/***************************************************************************//**
 * @file mikroe_uwb_dwm1000.c
 * @brief mikroe uwb dwm1000 source file
 * @version 1.0.0
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

#include "mikroe_uwb_dwm1000.h"
#include "mikroe_uwb_dwm1000_config.h"

#if (defined(SLI_SI917))
#else
#include "sl_spidrv_mikroe_config.h"
#endif
// ------------------------------------------------------------- PRIVATE MACROS

#define MIKROE_DWM1000_DUMMY 0

// ------------------------------------------------------------------ CONSTANTS

const uint8_t MIKROE_DWM1000_TMODE_LONGDATA_RANGE_LOWPOWER[3]
  = { MIKROE_DWM1000_DATA_RATE_110KBSPS,
      MIKROE_DWM1000_PULSTE_FREQ_16MHZ,
      MIKROE_DWM1000_PREAMBLE_LEN_2048 };
const uint8_t MIKROE_DWM1000_TMODE_LONGDATA_RANGE_ACCURACY[3]
  = { MIKROE_DWM1000_DATA_RATE_110KBSPS,
      MIKROE_DWM1000_PULSTE_FREQ_64MHZ,
      MIKROE_DWM1000_PREAMBLE_LEN_2048 };
const uint8_t MIKROE_DWM1000B_TMODE_LONGDATA_FAST_LOWPOWER[3]
  = { MIKROE_DWM1000_DATA_RATE_6800KBSPS,
      MIKROE_DWM1000_PULSTE_FREQ_16MHZ,
      MIKROE_DWM1000_PREAMBLE_LEN_1024 };
const uint8_t MIKROE_DWM1000_TMODE_LONGDATA_FAST_ACCURACY[3]
  = { MIKROE_DWM1000_DATA_RATE_6800KBSPS,
      MIKROE_DWM1000_PULSTE_FREQ_64MHZ,
      MIKROE_DWM1000_PREAMBLE_LEN_1024 };
const uint8_t MIKROE_DWM1000_TMODE_SHORTDATA_FAST_LOWPOWER[3]
  = { MIKROE_DWM1000_DATA_RATE_6800KBSPS,
      MIKROE_DWM1000_PULSTE_FREQ_16MHZ,
      MIKROE_DWM1000_PREAMBLE_LEN_128 };
const uint8_t MIKROE_DWM1000_TMODE_SHORTDATA_FAST_ACCURACY[3]
  = { MIKROE_DWM1000_DATA_RATE_6800KBSPS,
      MIKROE_DWM1000_PULSTE_FREQ_64MHZ,
      MIKROE_DWM1000_PREAMBLE_LEN_128 };

static uwb_t uwb;
static uwb_cfg_t uwb_cfg;
static bool initialized = false;
static digital_out_t cs_out;

sl_status_t mikroe_dwm1000_init(mikroe_spi_handle_t spi_instance)
{
  if ((NULL == spi_instance)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (initialized) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  // Configure default spi instance
  uwb.spi.handle = spi_instance;

  uwb.offset = UWB_SUB_NO;

  // Call basic setup functions
  uwb_cfg_setup(&uwb_cfg);

#if (UWB_DWM1000_SPI_UC == 1)
  uwb_cfg.spi_speed = UWB_DWM1000_SPI_BITRATE;
#endif

#if defined(DWM1000_RESET_PORT) && defined(DWM1000_RESET_PIN)
  uwb_cfg.rst = hal_gpio_pin_name(DWM1000_RESET_PORT,
                                  DWM1000_RESET_PIN);
#endif

#if defined(DWM1000_INT_PORT) && defined(DWM1000_INT_PIN)
  uwb_cfg.irq = hal_gpio_pin_name(DWM1000_INT_PORT,
                                  DWM1000_INT_PIN);
#endif

#if (defined(SLI_SI917))
#if defined(DWM1000_SPI_CS_PORT) && defined(DWM1000_SPI_CS_PIN)
  uwb_cfg.cs = hal_gpio_pin_name(DWM1000_SPI_CS_PORT,
                                 DWM1000_SPI_CS_PIN);
#endif
#else
#if defined(SL_SPIDRV_MIKROE_CS_PORT) && defined(SL_SPIDRV_MIKROE_CS_PIN)
  uwb_cfg.cs = hal_gpio_pin_name(SL_SPIDRV_MIKROE_CS_PORT,
                                 SL_SPIDRV_MIKROE_CS_PIN);
#endif
#endif

  digital_out_init(&cs_out, uwb_cfg.cs);
  digital_out_high(&cs_out);

  if (uwb_init(&uwb, &uwb_cfg) != UWB_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  initialized = true;

  return SL_STATUS_OK;
}

sl_status_t mikroe_dwm1000_generic_write(
  uint8_t reg_adr,
  uint8_t *tx_buf,
  uint16_t buf_len)
{
  return uwb_generic_write(&uwb, reg_adr, tx_buf, buf_len);
}

sl_status_t mikroe_dwm1000_generic_read(
  uint8_t reg_adr,
  uint8_t *rx_buf,
  uint16_t buf_len)
{
  return uwb_generic_read(&uwb, reg_adr, rx_buf, buf_len);
}

void mikroe_dwm1000_set_rst_pin_status(uint8_t status)
{
  uwb_set_rst_pin_status(&uwb, status);
}

uint8_t mikroe_dwm1000_get_qint_pin_status(void)
{
  return uwb_get_qint_pin_status(&uwb);
}

void mikroe_dwm1000_read_otp(uint16_t otp_adr, uint8_t *rx_buf)
{
  uwb_read_otp(&uwb, otp_adr, rx_buf);
}

void mikroe_dwm1000_set_bit(uint8_t reg_adr, uint16_t bit_num,
                            uint8_t bit_state)
{
  uwb_set_bit(&uwb, reg_adr, bit_num, bit_state);
}

uint8_t mikroe_dwm1000_get_bit(uint8_t reg_adr, uint16_t bit_num)
{
  return uwb_get_bit(&uwb, reg_adr, bit_num);
}

uint16_t mikroe_dwm1000_reg_data_len(uint8_t reg_adr)
{
  return uwb_reg_data_len(reg_adr);
}

void mikroe_dwm1000_set_dev_adr_n_network_id(uint16_t dev_adr, uint16_t net_id)
{
  uwb_set_dev_adr_n_network_id(&uwb, dev_adr, net_id);
}

sl_status_t mikroe_dwm1000_set_data_rate(uint8_t rate)
{
  if (SL_STATUS_OK != uwb_set_data_rate(&uwb, rate)) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

void mikroe_dwm1000_set_pulse_freq(uint8_t freq)
{
  uwb_set_pulse_freq(&uwb, freq);
}

void mikroe_dwm1000_set_preamble_len(uint8_t prealen)
{
  uwb_set_preamble_len(&uwb, prealen);
}

void mikroe_dwm1000_set_preable_code(uint8_t preamble)
{
  uwb_set_preable_code(&uwb, preamble);
}

void mikroe_dwm1000_set_channel(uint8_t channel)
{
  uwb_set_channel(&uwb, channel);
}

void mikroe_dwm1000_set_transmit_type(const uint8_t *t_t)
{
  uwb_set_transmit_type(&uwb, t_t);
}

void mikroe_dwm1000_get_transmit(uint8_t *rx_buf, uint16_t len_buf)
{
  uwb_get_transmit(&uwb, rx_buf, len_buf);
}

uint8_t mikroe_dwm1000_get_transmit_len(void)
{
  return uwb_get_transmit_len(&uwb);
}

void mikroe_dwm1000_set_transmit(uint8_t *tx_buf, uint16_t len_buf)
{
  uwb_set_transmit(&uwb, tx_buf, len_buf);
}

void mikroe_dwm1000_use_smart_power(uint8_t smart_power)
{
  uwb_use_smart_power(&uwb, smart_power);
}

void mikroe_dwm1000_frame_check(uint8_t fc_val)
{
  uwb_frame_check(fc_val);
}

void mikroe_dwm1000_frame_filter(uint8_t ff_val)
{
  uwb_frame_filter(&uwb, ff_val);
}

void mikroe_dwm1000_set_mode(uint8_t mode)
{
  uwb_set_mode(&uwb, mode);
}

void mikroe_dwm1000_start_transceiver(void)
{
  uwb_start_transceiver(&uwb);
}

uint8_t mikroe_dwm1000_get_transmit_status(void)
{
  return uwb_get_transmit_status(&uwb);
}

void mikroe_dwm1000_clear_status(void)
{
  uwb_int_mask_set(&uwb);
}

void mikroe_dwm1000_int_mask_set(void)
{
  uwb_int_mask_set(&uwb);
}

void mikroe_dwm1000_tune_config(void)
{
  uwb_tune_config(&uwb);
}

void mikroe_dwm1000_enable(void)
{
  uwb_enable(&uwb);
}

void mikroe_dwm1000_dev_reset(void)
{
  uwb_dev_reset(&uwb);
}
