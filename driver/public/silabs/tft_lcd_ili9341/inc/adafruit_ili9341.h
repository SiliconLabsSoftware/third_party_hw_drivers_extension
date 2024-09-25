/***************************************************************************//**
 * @file adafruit_ili9341.h
 * @brief Adafruit ILI9341 TFT LCD with Touchscreen Header File
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
#ifndef ADAFRUIT_ILI9341_H_
#define ADAFRUIT_ILI9341_H_

// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include "sl_status.h"
#include "sl_component_catalog.h"
#include "mipi_dbi.h"
#include "mipi_dbi_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
//                       Macros
// -----------------------------------------------------------------------------
#define ILI9341_TFTWIDTH    240 ///< ILI9341 max TFT width
#define ILI9341_TFTHEIGHT   320 ///< ILI9341 max TFT height

#define ILI9341_NOP         0x00 ///< No-op register
#define ILI9341_SWRESET     0x01 ///< Software reset register
#define ILI9341_RDDID       0x04 ///< Read display identification information
#define ILI9341_RDDST       0x09 ///< Read Display Status

#define ILI9341_SLPIN       0x10 ///< Enter Sleep Mode
#define ILI9341_SLPOUT      0x11 ///< Sleep Out
#define ILI9341_PTLON       0x12 ///< Partial Mode ON
#define ILI9341_NORON       0x13 ///< Normal Display Mode ON

#define ILI9341_RDMODE      0x0A ///< Read Display Power Mode
#define ILI9341_RDMADCTL    0x0B ///< Read Display MADCTL
#define ILI9341_RDPIXFMT    0x0C ///< Read Display Pixel Format
#define ILI9341_RDIMGFMT    0x0D ///< Read Display Image Format
#define ILI9341_RDSELFDIAG  0x0F ///< Read Display Self-Diagnostic Result

#define ILI9341_INVOFF      0x20 ///< Display Inversion OFF
#define ILI9341_INVON       0x21 ///< Display Inversion ON
#define ILI9341_GAMMASET    0x26 ///< Gamma Set
#define ILI9341_DISPOFF     0x28 ///< Display OFF
#define ILI9341_DISPON      0x29 ///< Display ON

#define ILI9341_CASET       0x2A ///< Column Address Set
#define ILI9341_PASET       0x2B ///< Page Address Set
#define ILI9341_RAMWR       0x2C ///< Memory Write
#define ILI9341_RAMRD       0x2E ///< Memory Read

#define ILI9341_PTLAR       0x30 ///< Partial Area
#define ILI9341_VSCRDEF     0x33 ///< Vertical Scrolling Definition
#define ILI9341_MADCTL      0x36 ///< Memory Access Control
#define ILI9341_VSCRSADD    0x37 ///< Vertical Scrolling Start Address
#define ILI9341_PIXFMT      0x3A ///< COLMOD: Pixel Format Set

///< Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR1     0xB1
#define ILI9341_FRMCTR2     0xB2 ///< Frame Rate Control (In Idle Mode/8 colors)
///< Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_FRMCTR3     0xB3
#define ILI9341_INVCTR      0xB4 ///< Display Inversion Control
#define ILI9341_DFUNCTR     0xB6 ///< Display Function Control

#define ILI9341_PWCTR1      0xC0 ///< Power Control 1
#define ILI9341_PWCTR2      0xC1 ///< Power Control 2
#define ILI9341_PWCTR3      0xC2 ///< Power Control 3
#define ILI9341_PWCTR4      0xC3 ///< Power Control 4
#define ILI9341_PWCTR5      0xC4 ///< Power Control 5
#define ILI9341_VMCTR1      0xC5 ///< VCOM Control 1
#define ILI9341_VMCTR2      0xC7 ///< VCOM Control 2

#define ILI9341_RDID1       0xDA ///< Read ID 1
#define ILI9341_RDID2       0xDB ///< Read ID 2
#define ILI9341_RDID3       0xDC ///< Read ID 3
#define ILI9341_RDID4       0xDD ///< Read ID 4

#define ILI9341_GMCTRP1     0xE0 ///< Positive Gamma Correction
#define ILI9341_GMCTRN1     0xE1 ///< Negative Gamma Correction
// #define ILI9341_PWCTR6     0xFC

// Color definitions
#define ILI9341_BLACK       0x0000 ///<   0,   0,   0
#define ILI9341_NAVY        0x000F ///<   0,   0, 123
#define ILI9341_DARKGREEN   0x03E0 ///<   0, 125,   0
#define ILI9341_DARKCYAN    0x03EF ///<   0, 125, 123
#define ILI9341_MAROON      0x7800 ///< 123,   0,   0
#define ILI9341_PURPLE      0x780F ///< 123,   0, 123
#define ILI9341_OLIVE       0x7BE0 ///< 123, 125,   0
#define ILI9341_LIGHTGREY   0xC618 ///< 198, 195, 198
#define ILI9341_DARKGREY    0x7BEF ///< 123, 125, 123
#define ILI9341_BLUE        0x001F ///<   0,   0, 255
#define ILI9341_GREEN       0x07E0 ///<   0, 255,   0
#define ILI9341_CYAN        0x07FF ///<   0, 255, 255
#define ILI9341_RED         0xF800 ///< 255,   0,   0
#define ILI9341_MAGENTA     0xF81F ///< 255,   0, 255
#define ILI9341_YELLOW      0xFFE0 ///< 255, 255,   0
#define ILI9341_WHITE       0xFFFF ///< 255, 255, 255
#define ILI9341_ORANGE      0xFD20 ///< 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define ILI9341_PINK        0xFC18 ///< 255, 130, 198

typedef void (*adafruit_ili9341_dma_complete_callback_t)(sl_status_t status);
// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *  Initialize ILI9341 chip and connects to the ILI9341 over SPI and sends
 *  initialization procedure commands.
 *
 * @param[in] spidrv_handle
 *  SPI Driver Handle.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_ili9341_device_init(const struct mipi_dbi_config *config);

#define adafruit_ili9341_init(config) \
  adafruit_ili9341_device_init((const struct mipi_dbi_config *)config)

/***************************************************************************//**
 * @brief
 *  Enable/Disable display color inversion.
 *
 * @param[in] invert
 *  True to invert, False to have normal color.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_ili9341_invert_display(bool invert);

/***************************************************************************//**
 * @brief
 *  Scroll display memory.
 *
 * @param[in] y
 *  How many pixels to scroll display by.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_ili9341_scroll_to(uint16_t y);

/***************************************************************************//**
 * @brief
 *  Set the height of the Top and Bottom Scroll Margins.
 *
 * @param[in] top
 *  The height of the Top scroll margin.
 * @param[in] bottom
 *  The height of the Bottom scroll margin
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_ili9341_set_scroll_margins(uint16_t top, uint16_t bottom);

/***************************************************************************//**
 * @brief
 *  Set the "address window" - the rectangle we will write to RAM with the next
 *  chunk of SPI data writes. The ILI9341 will automatically wrap the data as
 *  each row is filled.
 *
 * @param[in] x1
 *  TFT memory 'x' origin.
 * @param[in] y1
 *  TFT memory 'y' origin.
 * @param[in] w
 *  Width of rectangle.
 * @param[in] h
 *  Height of rectangle.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_ili9341_set_addr_window(uint16_t x1, uint16_t y1,
                                             uint16_t w, uint16_t h);

/***************************************************************************//**
 * @brief
 *  Read 8 bits of data from ILI9341 configuration memory. NOT from RAM!.
 *
 * @param[out] result
 *  Command value read from ILI9341.
 * @param[in] command
 *  The command register to read data from.
 * @param[in] index
 *  Unsigned 8-bit data read from ILI9341 register.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_ili9341_read_command(uint8_t *result,
                                          uint8_t command,
                                          uint8_t index);

/***************************************************************************//**
 * @brief
 *  Draw a single pixel to the display at requested coordinates.
 *
 * @param[in] x
 *  Horizontal position (0 = left).
 * @param[in] y
 *  Vertical position   (0 = top).
 * @param[in] color
 *  16-bit pixel color in '565' RGB format.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_ili9341_draw_pixel(int16_t x, int16_t y, uint16_t color);

/***************************************************************************//**
 * @brief
 *  Fill the screen completely with one color.
 *
 * @param[in] color
 *  16-bit pixel color in '565' RGB format.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_ili9341_fill_screen(uint16_t color);

/***************************************************************************//**
 * @brief
 *  Draw a 16-bit image (565 RGB) at the specified (x,y) position.
 *
 * @param[in] x
 *  Top left corner horizontal coordinate.
 * @param[in] y
 *  Top left corner vertical coordinate.
 * @param[in] color
 *  Pointer to 16-bit array of pixel values.
 * @param[in] w
 *  Width of bitmap in pixels.
 * @param[in] h
 *  Height of bitmap in pixels.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_ili9341_draw_rgb_bitmap(int16_t x,
                                             int16_t y,
                                             uint16_t *color,
                                             int16_t w,
                                             int16_t h);

sl_status_t adafruit_ili9341_flush_area_rgb565(int16_t x1, int16_t y1,
                                               int16_t x2, int16_t y2,
                                               uint8_t *pcolor,
                                               bool color_swap,
                                               void (*callback)(void *arg),
                                               void *callback_arg);

#ifdef __cplusplus
}
#endif

#endif /* ADAFRUIT_ILI9341_H_ */
