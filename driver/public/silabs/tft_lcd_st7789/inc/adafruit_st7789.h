/***************************************************************************//**
 * @file adafruit_st7789.h
 * @brief Adafruit ST7789 Color TFT Display Driver header file
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
#ifndef ADAFRUIT_ST7789_H_
#define ADAFRUIT_ST7789_H_

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
#define ST7789_TFTWIDTH    135 ///< ST7789 max TFT width
#define ST7789_TFTHEIGHT   240 ///< ST7789 max TFT height

#define ST_CMD_DELAY       0x80 // special signifier for command lists

#define ST7789_NOP         0x00
#define ST7789_SWRESET     0x01
#define ST7789_RDDID       0x04
#define ST7789_RDDST       0x09

#define ST7789_SLPIN       0x10
#define ST7789_SLPOUT      0x11
#define ST7789_PTLON       0x12
#define ST7789_NORON       0x13

#define ST7789_INVON       0x20
#define ST7789_INVOFF      0x21
#define ST7789_DISPOFF     0x28
#define ST7789_DISPON      0x29
#define ST7789_CASET       0x2A
#define ST7789_RASET       0x2B
#define ST7789_RAMWR       0x2C
#define ST7789_RAMRD       0x2E

#define ST7789_PTLAR       0x30
#define ST7789_TEOFF       0x34
#define ST7789_TEON        0x35
#define ST7789_MADCTL      0x36
#define ST7789_COLMOD      0x3A

#define ST7789_MADCTL_MY   0x80
#define ST7789_MADCTL_MX   0x40
#define ST7789_MADCTL_MV   0x20
#define ST7789_MADCTL_ML   0x10
#define ST7789_MADCTL_RGB  0x00

#define ST7789_RDID1       0xDA
#define ST7789_RDID2       0xDB
#define ST7789_RDID3       0xDC
#define ST7789_RDID4       0xDD

// Some ready-made 16-bit ('565') color settings:
#define ST7789_BLACK       0x0000
#define ST7789_WHITE       0xFFFF
#define ST7789_RED         0xF800
#define ST7789_GREEN       0x07E0
#define ST7789_BLUE        0x001F
#define ST7789_CYAN        0x07FF
#define ST7789_MAGENTA     0xF81F
#define ST7789_YELLOW      0xFFE0
#define ST7789_ORANGE      0xFC00

typedef enum
{
  adafruit_st7789_rotation_none = 0,
  adafruit_st7789_rotation_90,
  adafruit_st7789_rotation_180,
  adafruit_st7789_rotation_270,
} adafruit_st7789_rotation_e;

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *  Initialize ST7789 chip and connects to the ST7789 over SPI and sends
 *  initialization procedure commands.
 *
 * @param[in] spidrv_handle
 *  SPI Driver Handle.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_st7789_spi_init(const struct mipi_dbi_config *config);

#define adafruit_st7789_init(spi_handle) \
  adafruit_st7789_spi_init((const struct mipi_dbi_config *)spi_handle);

/***************************************************************************//**
 * @brief
 *  Get the current display width. This value will be changed when the screen is
 *  rotated.
 *
 * @param[in] none
 *
 * @return
 *  The current display width.
 ******************************************************************************/
uint16_t adafruit_st7789_get_display_width(void);

/***************************************************************************//**
 * @brief
 *  Get the current display height. This value will be changed when the screen
 *  is rotated.
 *
 * @param[in] none
 *
 * @return
 *  The current display height.
 ******************************************************************************/
uint16_t adafruit_st7789_get_display_height(void);

/***************************************************************************//**
 * @brief
 *  Enable/Disable display color inversion.
 *
 * @param[in] invert
 *  True to invert, False to have normal color.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_IO or SL_STATUS_INVALID_PARAMETER if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_st7789_invert_display(bool invert);

/***************************************************************************//**
 * @brief
 *  Enable/Disable display
 *
 * @param[in] enable
 *  True to turn on display, False to turn off display.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_IO or SL_STATUS_INVALID_PARAMETER if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_st7789_enable_display(bool enable);

/***************************************************************************//**
 * @brief
 *  Change whether TE pin output is on or off
 *
 * @param[in] enable
 *  True if you want the TE pin ON, FALSE to TE pin OFF
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_IO or SL_STATUS_INVALID_PARAMETER if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_st7789_enable_tearing(bool enable);

/***************************************************************************//**
 * @brief
 *  Change whether sleep mode is on or off
 *
 * @param[in] enable
 *  True if you want sleep mode ON, false to sleep mode OFF
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_IO or SL_STATUS_INVALID_PARAMETER if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_st7789_enableSleep(bool enable);

/***************************************************************************//**
 * @brief
 *  Set the "address window" - the rectangle we will write to RAM with the next
 *  chunk of SPI data writes. The ST7789 will automatically wrap the data as
 *  each row is filled.
 *
 * @param[in] x
 *  TFT memory 'x' origin.
 * @param[in] y
 *  TFT memory 'y' origin.
 * @param[in] w
 *  Width of rectangle.
 * @param[in] h
 *  Height of rectangle.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_IO if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_st7789_set_addr_window(uint16_t x, uint16_t y,
                                            uint16_t w, uint16_t h);

/***************************************************************************//**
 * @brief
 *  Change rotation of the TFT
 *
 * @param[in] rotation
 *  Value from 0x00 -> 0x03 coresponding rotation 0 - degree, 90 - degree,
 *  180 - degree, 270 - degree.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_IO or SL_STATUS_INVALID_PARAMETER if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_st7789_set_rotation(adafruit_st7789_rotation_e rotation);

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
 *  SL_STATUS_INVALID_PARAMETER if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_st7789_draw_pixel(int16_t x, int16_t y, uint16_t color);

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
sl_status_t adafruit_st7789_fill_screen(uint16_t color);

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
sl_status_t adafruit_st7789_draw_rgb_bitmap(int16_t x,
                                            int16_t y,
                                            uint16_t *color,
                                            int16_t w,
                                            int16_t h);

/***************************************************************************//**
 * @brief
 * Fill the screen area  with color. After the process is complete call the user
 * callback to notify the higher layer.
 *
 * @param[in] (x1, y1): The first point coordinate.
 * @param[in] (x2, y2): The second point coordinate.
 * @param[in] pcolor: The pointer to array, which store color to fill to the
 *   area
 * @param[in] color_swap:
 * True - The "pcolor" contains pre-swap 16 bit data, the driver pushes the data
 * directly to the TFT.
 * False - The "pcolor" contains 16 bit raw data, the driver need swap 2 bytes
 * before pushes the data to the TFT.
 * @param[in] callback: The "callback" function will be called after the fill
 *   area
 * process is done.
 * @param[in] callback_arg: The argument is passed to the "callback" function.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_st7789_flush_area_rgb565(int16_t x1, int16_t y1,
                                              int16_t x2, int16_t y2,
                                              uint8_t *pcolor,
                                              bool color_swap,
                                              void (*callback)(void *arg),
                                              void *callback_arg);

#ifdef __cplusplus
}
#endif

#endif /* ADAFRUIT_ST7789_H_ */
