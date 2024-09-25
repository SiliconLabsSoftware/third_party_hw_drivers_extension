/***************************************************************************//**
 * @file mikroe_ssd1306.c
 * @brief SCL SSD1306 Source File
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

#include "mikroe_ssd1306_config.h"
#include "mikroe_ssd1306.h"

#define SSD1306_HORIZONTAL_ADDRESSING_MODE    0x00
#define SSD1306_VERTICAL_ADDRESSING_MODE      0x01
#define SSD1306_PAGE_ADDRESSING_MODE          0x02
#define SSD1306_NUM_PAGES                     ((SSD1306_DISPLAY_HEIGHT + 7) / 8)

static oledw_t oledw;
static oledw_cfg_t oledw_cfg;
static bool initialized = false;

sl_status_t mikroe_ssd1306_init(mikroe_spi_handle_t spi_instance)
{
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default spi instance
  oledw.spi.handle = spi_instance;

  // Call basic setup functions
  oledw_cfg_setup(&oledw_cfg);
  oledw_cfg.sel = OLEDW_MASTER_SPI;

#if defined(SSD1306_DC_PORT) && defined(SSD1306_DC_PIN)
  oledw_cfg.dc = hal_gpio_pin_name(SSD1306_DC_PORT, SSD1306_DC_PIN);
#endif

#if defined(SSD1306_RST_PORT) && defined(SSD1306_RST_PIN)
  oledw_cfg.rst = hal_gpio_pin_name(SSD1306_RST_PORT, SSD1306_RST_PIN);
#endif

#if defined(SSD1306_CS_PORT) && defined(SSD1306_CS_PIN)
  oledw_cfg.cs = hal_gpio_pin_name(SSD1306_CS_PORT, SSD1306_CS_PIN);
#endif

#if (MIKROE_SSD1306_SPI_UC == 1)
  oledw_cfg.spi_speed = MIKROE_SSD1306_SPI_BITRATE;
#endif

  if (oledw_init(&oledw, &oledw_cfg) != OLEDW_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  initialized = true;
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_set_spi_instance(mikroe_spi_handle_t spi_instance)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  if ((NULL == spi_instance)
      || (oledw_cfg.sel != OLEDW_MASTER_SPI)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  oledw.spi.handle = spi_instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_send(oledw_data_t tx_data,
                                oledw_data_mode_t data_mode)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  if (oledw_send(&oledw, tx_data, data_mode) != OLEDW_OK) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_default_cfg(void)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  // 0xAE Set OLED Display Off
  oledw_send(&oledw, OLEDW_DISPLAYOFF, OLEDW_COMMAND);
  // 0xD5 Set Display Clock Divide Ratio/Oscillator Frequency
  oledw_send(&oledw, OLEDW_SETDISPLAYCLOCKDIV, OLEDW_COMMAND);
  oledw_send(&oledw, 0x80, OLEDW_COMMAND);
  // 0xD3 Set Display Offset
  oledw_send(&oledw, OLEDW_SETDISPLAYOFFSET, OLEDW_COMMAND);
  oledw_send(&oledw, 0x00, OLEDW_COMMAND);
  // 0x40 Set Display Start Line
  oledw_send(&oledw, OLEDW_SETSTARTLINE, OLEDW_COMMAND);
  // 0x8D Set Charge Pump
  oledw_send(&oledw, OLEDW_CHARGEPUMP, OLEDW_COMMAND);
  // 0x14 Enable Charge Pump
  oledw_send(&oledw, 0x14, OLEDW_COMMAND);
  // 0xC8 Set COM Output Scan Direction
  oledw_send(&oledw, OLEDW_COMSCANDEC, OLEDW_COMMAND);
  // 0xA8 Set Multiplex Ratio
  oledw_send(&oledw, OLEDW_SETMULTIPLEX, OLEDW_COMMAND);
  // OLED_HEIGHT - 1
  oledw_send(&oledw, SSD1306_DISPLAY_HEIGHT - 1, OLEDW_COMMAND);
  // 0xA1 set segment re-map 0 to 127
  oledw_send(&oledw, OLEDW_SETSEGMENTREMAP, OLEDW_COMMAND);

#ifdef CONFIG_OLED_DISPLAY_64_48
  // 0x20 Set memory mode
  oledw_send(&oledw, OLEDW_MEMORYMODE, OLEDW_COMMAND);
  oledw_send(&oledw, SSD1306_PAGE_ADDRESSING_MODE, OLEDW_COMMAND);

  // Set COM Pins Hardware Configuration
  oledw_send(&oledw, OLEDW_SETCOMPINS, OLEDW_COMMAND);
  oledw_send(&oledw, 0x12, OLEDW_COMMAND);

  // 0x81 Set Contrast Control
  oledw_send(&oledw, OLEDW_SETCONTRAST, OLEDW_COMMAND);
  oledw_send(&oledw, 0x8F, OLEDW_COMMAND);

  // 0xD9 Set Pre-Charge Period
  oledw_send(&oledw, OLEDW_SETPRECHARGE, OLEDW_COMMAND);
  oledw_send(&oledw, 0xF1, OLEDW_COMMAND);

  // 0xDB Set VCOMH Detect Level
  oledw_send(&oledw, OLEDW_SETVCOMDETECT, OLEDW_COMMAND);
  oledw_send(&oledw, 0x40, OLEDW_COMMAND);
#endif

#ifdef CONFIG_OLED_DISPLAY_96_39
  // 0x20 Set memory mode
  oledw_send(&oledw, OLEDW_MEMORYMODE, OLEDW_COMMAND);
  oledw_send(&oledw, SSD1306_PAGE_ADDRESSING_MODE, OLEDW_COMMAND);

  // Set COM Pins Hardware Configuration
  oledw_send(&oledw, OLEDW_SETCOMPINS, OLEDW_COMMAND);
  oledw_send(&oledw, 0x12, OLEDW_COMMAND);

  // 0x81 Set Contrast Control
  oledw_send(&oledw, OLEDW_SETCONTRAST, OLEDW_COMMAND);
  oledw_send(&oledw, 0xAF, OLEDW_COMMAND);

  // 0xD9 Set Pre-Charge Period
  oledw_send(&oledw, OLEDW_SETPRECHARGE, OLEDW_COMMAND);
  oledw_send(&oledw, 0x25, OLEDW_COMMAND);

  // 0xDB Set VCOMH Detect Level
  oledw_send(&oledw, OLEDW_SETVCOMDETECT, OLEDW_COMMAND);
  oledw_send(&oledw, 0x20, OLEDW_COMMAND);
#endif

#ifdef CONFIG_OLED_DISPLAY_128_32
  // 0x20 Set memory mode
  oledw_send(&oledw, OLEDW_MEMORYMODE, OLEDW_COMMAND);
  oledw_send(&oledw, SSD1306_HORIZONTAL_ADDRESSING_MODE, OLEDW_COMMAND);

  // Set COM Pins Hardware Configuration
  oledw_send(&oledw, OLEDW_SETCOMPINS, OLEDW_COMMAND);
  oledw_send(&oledw, 0x02, OLEDW_COMMAND);

  // 0x81 Set Contrast Control
  oledw_send(&oledw, OLEDW_SETCONTRAST, OLEDW_COMMAND);
  oledw_send(&oledw, 0x7F, OLEDW_COMMAND);

  // 0xD9 Set Pre-Charge Period
  oledw_send(&oledw, OLEDW_SETPRECHARGE, OLEDW_COMMAND);
  oledw_send(&oledw, 0x22, OLEDW_COMMAND);

  // 0xDB Set VCOMH Detect Level
  oledw_send(&oledw, OLEDW_SETVCOMDETECT, OLEDW_COMMAND);
  oledw_send(&oledw, 0x40, OLEDW_COMMAND);
#endif

#ifdef CONFIG_OLED_DISPLAY_128_64
  // 0x20 Set memory mode
  oledw_send(&oledw, OLEDW_MEMORYMODE, OLEDW_COMMAND);
  oledw_send(&oledw, SSD1306_HORIZONTAL_ADDRESSING_MODE, OLEDW_COMMAND);

  // Set COM Pins Hardware Configuration
  oledw_send(&oledw, OLEDW_SETCOMPINS, OLEDW_COMMAND);
  oledw_send(&oledw, 0x12, OLEDW_COMMAND);

  // 0x81 Set Contrast Control
  oledw_send(&oledw, OLEDW_SETCONTRAST, OLEDW_COMMAND);
  oledw_send(&oledw, 0x7F, OLEDW_COMMAND);

  // 0xD9 Set Pre-Charge Period
  oledw_send(&oledw, OLEDW_SETPRECHARGE, OLEDW_COMMAND);
  oledw_send(&oledw, 0x22, OLEDW_COMMAND);

  // 0xDB Set VCOMH Detect Level
  oledw_send(&oledw, OLEDW_SETVCOMDETECT, OLEDW_COMMAND);
  oledw_send(&oledw, 0x20, OLEDW_COMMAND);
#endif

  // 0xA4 Set Entire Display On/Off
  oledw_send(&oledw, OLEDW_DISPLAYALLON_RESUME, OLEDW_COMMAND);
  // 0xA6 Set Normal/Inverse Display
  oledw_send(&oledw, OLEDW_NORMALDISPLAY, OLEDW_COMMAND);
  oledw_send(&oledw, OLEDW_DISPLAYON, OLEDW_COMMAND);

  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_set_page(oledw_data_t page_addr)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledw_set_page(&oledw, page_addr);
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_set_column(oledw_data_t col_addr)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledw_set_column(&oledw, col_addr);
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_display_picture(oledw_resources_t *pic)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  if (pic == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  } else {
    oledw_display_picture(&oledw, pic);
    return SL_STATUS_OK;
  }
}

sl_status_t mikroe_ssd1306_draw(const uint8_t *data)
{
  uint16_t i;
  static const uint8_t command_table[] = {
#ifdef SSD1306_USE_PAGE_ADDRESSING_MODE

    // Set Lower Column Start Address for Page Addressing Mode
    0x00,

    // Set Higher Column Start Address for Page Addressing Mode
    0x12
#else

    // Set page start, end address + set page pointer to page start address
    OLEDW_PAGEADDR, 0, (SSD1306_NUM_PAGES - 1),

    // Set page start, end address + set page pointer to page start address
    OLEDW_COLUMNADDR, 0, (SSD1306_DISPLAY_WIDTH - 1),
#endif
  };

  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }

#ifdef SSD1306_USE_PAGE_ADDRESSING_MODE

  // Get start address to draw from
  for (i = 0; i < SSD1306_NUM_PAGES; i++) {
    /**
     * Send update command and first line address
     * Set the current RAM page address.
     */
    oledw_send(&oledw, 0xB0 + i, OLEDW_COMMAND);

    for (uint16_t j = 0; j < sizeof(command_table); j++) {
      oledw_send(&oledw, command_table[j], OLEDW_COMMAND);
    }

    // Send pixels for this page
    for (uint16_t j = 0; j < MIKROE_SSD1306_LCDWIDTH; j++) {
      oledw_send(&oledw, *data++, OLEDW_DATA);
    }
  }
#else

  // Send commands to prepare data transfer from frame buffer
  for (i = 0; i < sizeof(command_table); i++) {
    oledw_send(&oledw, command_table[i], OLEDW_COMMAND);
  }

  // Send frame buffer data
  for (i = 0; i < (SSD1306_DISPLAY_WIDTH * SSD1306_NUM_PAGES); i++) {
    oledw_send(&oledw, *data++, OLEDW_DATA);
  }
#endif
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_enable_display(bool on)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  if (on) {
    // 0xA7 - set inverse color
    oledw_send(&oledw, OLEDW_DISPLAYON, OLEDW_COMMAND);
  } else {
    // 0xA7 - set inverse color
    oledw_send(&oledw, OLEDW_DISPLAYOFF, OLEDW_COMMAND);
  }
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_set_invert_color(void)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  // 0xA7 - set inverse color
  oledw_send(&oledw, OLEDW_INVERTDISPLAY, OLEDW_COMMAND);
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_set_normal_color(void)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  // 0xA6 - set normal color
  oledw_send(&oledw, OLEDW_NORMALDISPLAY, OLEDW_COMMAND);
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_set_contrast(oledw_data_t value)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledw_set_contrast(&oledw, value);
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_scroll_right(oledw_data_t start_page_addr,
                                        oledw_data_t end_page_addr)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledw_scroll_right(&oledw, start_page_addr, end_page_addr);
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_scroll_left(oledw_data_t start_page_addr,
                                       oledw_data_t end_page_addr)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledw_scroll_left(&oledw, start_page_addr, end_page_addr);
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_scroll_diag_right(oledw_data_t start_page_addr,
                                             oledw_data_t end_page_addr)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledw_scroll_diag_right(&oledw, start_page_addr, end_page_addr);
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_scroll_diag_left(oledw_data_t start_page_addr,
                                            oledw_data_t end_page_addr)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledw_scroll_diag_left(&oledw, start_page_addr, end_page_addr);
  return SL_STATUS_OK;
}

sl_status_t mikroe_ssd1306_stop_scroll(void)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  oledw_stop_scroll(&oledw);
  return SL_STATUS_OK;
}
