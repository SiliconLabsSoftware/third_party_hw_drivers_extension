/***************************************************************************//**
 * @file mikroe_ssd1351.c
 * @brief SSD1351 driver source file
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

#include "mikroe_ssd1351.h"
#include "mikroe_ssd1351_config.h"
#include "stdio.h"

static oledc_t oledc;
static oledc_cfg_t oledc_cfg;
static bool initialized = false;

// -----------------------------------------------------------------------------
// Public Function Definition
// -----------------------------------------------------------------------------

/***************************************************************************//**
 *    Initialization function.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_init(mikroe_spi_handle_t spi_instance)
{
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default spi instance
  oledc.spi.handle = spi_instance;

  // Call basic setup functions
  oledc_cfg_setup(&oledc_cfg);

#if (MIKROE_SSD1351_SPI_UC == 1)
  oledc_cfg.spi_speed = MIKROE_SSD1351_SPI_BITRATE;
#endif
#if defined(SSD1351_CS_PORT) && defined(SSD1351_CS_PIN)
  oledc_cfg.cs = hal_gpio_pin_name(SSD1351_CS_PORT, SSD1351_CS_PIN);
#endif

  oledc_cfg.dc = hal_gpio_pin_name(SSD1351_DC_PORT, SSD1351_DC_PIN);
  oledc_cfg.en = hal_gpio_pin_name(SSD1351_EN_PORT, SSD1351_EN_PIN);
  oledc_cfg.rw = hal_gpio_pin_name(SSD1351_RW_PORT, SSD1351_RW_PIN);

#if defined(SSD1351_RST_PORT) && defined(SSD1351_RST_PIN)
  oledc_cfg.rst = hal_gpio_pin_name(SSD1351_RST_PORT, SSD1351_RST_PIN);
#endif

  if (oledc_init(&oledc, &oledc_cfg) != OLEDC_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  initialized = true;
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    This function sets the SPI instance used by platform functions.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_set_spi_instance(mikroe_spi_handle_t spi_instance)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  oledc.spi.handle = spi_instance;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Default Configuration function.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_default_cfg(void)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_default_cfg(&oledc);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Enable/Disable display.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_enable(bool state)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_enable(&oledc, state);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Hardware Reset.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_reset(void)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_reset(&oledc);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Command Send with more argument.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_more_arg_commands(uint8_t command,
                                             uint8_t *args,
                                             uint16_t args_len)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_more_arg_commands(&oledc, command, args, args_len);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Command Send with one argument.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_one_arg_commands(uint8_t command, uint8_t args)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_one_arg_commands(&oledc, command, args);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Fill Screen.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_fill_screen(uint16_t color)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_fill_screen(&oledc, color);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Draw Rectangle.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_rectangle(uint8_t col_off,
                                     uint8_t row_off,
                                     uint8_t col_end,
                                     uint8_t row_end,
                                     uint16_t color)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_rectangle(&oledc, col_off, row_off, col_end, row_end, color);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Draw BMP Image.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_image(const uint8_t *img,
                                 uint8_t col_off,
                                 uint8_t row_off)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_image(&oledc, img, col_off, row_off);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Draw Text.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_text(uint8_t *text, uint16_t x, uint16_t y)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_text(&oledc, text, x, y);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Font Setup.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_set_font(const uint8_t *font_s, uint16_t color)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledc_set_font(&oledc, font_s, color);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Set Contrast.
 ******************************************************************************/
sl_status_t mikroe_ssd1351_set_contrast(uint8_t contrast)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  uint8_t oledc_contrast[3] = { 0x8A, contrast, 0x8A };
  oledc_more_arg_commands(&oledc, OLEDC_CONTRAST, oledc_contrast, 3);
  return SL_STATUS_OK;
}
