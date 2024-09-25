/***************************************************************************//**
 * @file adafruit_hxd8357d.h
 * @brief Adafruit HXD8357D TFT LCD with Touchscreen Header File
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
#ifndef _ADAFRUIT_HXD8357D_H
#define _ADAFRUIT_HXD8357D_H

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

#define HXD8357D           0xD ///< Our internal const for D type
#define HXD8357B           0xB ///< Our internal const for B type

#define HXD8357D_TFTWIDTH  320 ///< 320 pixels wide
#define HXD8357D_TFTHEIGHT 480 ///< 480 pixels tall

#define HXD8357D_NOP       0x00 ///< No op
#define HXD8357D_SWRESET   0x01 ///< software reset
#define HXD8357D_RDDID     0x04 ///< Read ID
#define HXD8357D_RDDST     0x09 ///< (unknown)

#define HXD8357D_RDPOWMODE 0x0A ///< Read power mode Read power mode
#define HXD8357D_RDMADCTL  0x0B ///< Read MADCTL
#define HXD8357D_RDCOLMOD  0x0C ///< Column entry mode
#define HXD8357D_RDDIM     0x0D ///< Read display image mode
#define HXD8357D_RDDSDR    0x0F ///< Read dosplay signal mode

#define HXD8357D_SLPIN     0x10 ///< Enter sleep mode
#define HXD8357D_SLPOUT    0x11 ///< Exit sleep mode

#define HXD8357D_INVOFF    0x20 ///< Turn off invert
#define HXD8357D_INVON     0x21 ///< Turn on invert
#define HXD8357D_DISPOFF   0x28 ///< Display on
#define HXD8357D_DISPON    0x29 ///< Display off

#define HXD8357D_CASET     0x2A ///< Column addr set
#define HXD8357D_PASET     0x2B ///< Page addr set
#define HXD8357D_RAMWR     0x2C ///< Write VRAM
#define HXD8357D_RAMRD     0x2E ///< Read VRAm

#define HXD8357D_TEON      0x35 ///< Tear enable on
#define HXD8357D_TEARLINE  0x44 ///< (unknown)
#define HXD8357D_MADCTL    0x36 ///< Memory access control
#define HXD8357D_COLMOD    0x3A ///< Color mode

#define HXD8357D_SETOSC    0xB0   ///< Set oscillator
#define HXD8357D_SETPWR1   0xB1   ///< Set power control
#define HXD8357D_SETRGB    0xB3   ///< Set RGB interface
#define HXD8357D_SETCOM    0xB6  ///< Set VCOM voltage

#define HXD8357D_SETCYC    0xB4   ///< Set display cycle reg
#define HXD8357D_SETC      0xB9   ///< Enable extension command

#define HXD8357D_SETSTBA   0xC0         ///< Set source option
#define HXD8357D_SETPANEL  0xCC          ///< Set Panel

#define HXD8357D_SETGAMMA  0xE0 ///< Set Gamma

// Plan is to move this to GFX header (with different prefix), though
// defines will be kept here for existing code that might be referencing
// them. Some additional ones are in the ILI9341 lib -- add all in GFX!
// Color definitions
#define HXD8357D_BLACK     0x0000 ///< BLACK color for drawing graphics
#define HXD8357D_BLUE      0x001F ///< BLUE color for drawing graphics
#define HXD8357D_RED       0xF800 ///< RED color for drawing graphics
#define HXD8357D_GREEN     0x07E0 ///< GREEN color for drawing graphics
#define HXD8357D_CYAN      0x07FF ///< CYAN color for drawing graphics
#define HXD8357D_MAGENTA   0xF81F ///< MAGENTA color for drawing graphics
#define HXD8357D_YELLOW    0xFFE0 ///< YELLOW color for drawing graphics
#define HXD8357D_WHITE     0xFFFF ///< WHITE color for drawing graphics

typedef void (*adafruit_hxd8357d_dma_complete_callback_t)(sl_status_t status);
// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *  Initialize HXD8357D chip and connects to the HXD8357D over SPI and sends
 *  initialization procedure commands.
 *
 * @param[in] spidrv_handle
 *  SPI Driver Handle.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_hxd8357d_spi_init(const struct mipi_dbi_config *config);

#define adafruit_hxd8357d_init(config) \
  adafruit_hxd8357d_spi_init((const struct mipi_dbi_config *)config)

/***************************************************************************//**
 * @brief
 *
 * @param[in] rotate
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_hxd8357d_set_rotation(uint8_t rotate);

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
sl_status_t adafruit_hxd8357d_invert_display(bool invert);

/***************************************************************************//**
 * @brief
 *  Set the "address window" - the rectangle we will write to RAM with the next
 *  chunk of SPI data writes. The hxd8357d will automatically wrap the data as
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
sl_status_t adafruit_hxd8357d_set_addr_window(uint16_t x1, uint16_t y1,
                                              uint16_t w, uint16_t h);

/***************************************************************************//**
 * @brief
 *  Read 8 bits of data from HX8357 configuration memory. NOT from RAM!.
 *
 * @param[out] result
 *  Command value read from HX8357.
 * @param[in] command
 *  The command register to read data from.
 * @param[in] index
 *  Unsigned 8-bit data read from HX8357 register.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_hxd8357d_read_command(uint8_t *result,
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
sl_status_t adafruit_hxd8357d_draw_pixel(int16_t x, int16_t y, uint16_t color);

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
sl_status_t adafruit_hxd8357d_fill_screen(uint16_t color);

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
sl_status_t adafruit_hxd8357d_draw_rgb_bitmap(int16_t x,
                                              int16_t y,
                                              uint16_t *color,
                                              int16_t w,
                                              int16_t h);

sl_status_t adafruit_hxd8357d_flush_area_rgb565(int16_t x1, int16_t y1,
                                                int16_t x2, int16_t y2,
                                                uint8_t *pcolor,
                                                bool color_swap,
                                                void (*callback)(void *arg),
                                                void *callback_arg);

#ifdef __cplusplus
}
#endif

#endif /* ADAFRUIT_HXD8357D_H_ */
