/***************************************************************************//**
 * @file adafruit_neopixel.h
 * @brief adafruit_neopixel header file for Adafruit NeoTrellis 4x4 keypad.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#ifndef _ADAFRUIT_NEOPIXEL_H_
#define _ADAFRUIT_NEOPIXEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "sl_status.h"
#include "adafruit_seesaw.h"

/***************************************************************************//**
 *    //                       Macro
 ******************************************************************************/

// RGB NeoPixel permutations; white and red offsets are always same
// Offset:         W          R          G          B
#define NEO_RGB  ((0 << 6) | (0 << 4) | (1 << 2) | (2))
#define NEO_RBG  ((0 << 6) | (0 << 4) | (2 << 2) | (1))
#define NEO_GRB  ((1 << 6) | (1 << 4) | (0 << 2) | (2))
#define NEO_GBR  ((2 << 6) | (2 << 4) | (0 << 2) | (1))
#define NEO_BRG  ((1 << 6) | (1 << 4) | (2 << 2) | (0))
#define NEO_BGR  ((2 << 6) | (2 << 4) | (1 << 2) | (0))

// RGBW NeoPixel permutations; all 4 offsets are distinct
// Offset:         W          R          G          B
#define NEO_WRGB ((0 << 6) | (1 << 4) | (2 << 2) | (3))
#define NEO_WRBG ((0 << 6) | (1 << 4) | (3 << 2) | (2))
#define NEO_WGRB ((0 << 6) | (2 << 4) | (1 << 2) | (3))
#define NEO_WGBR ((0 << 6) | (3 << 4) | (1 << 2) | (2))
#define NEO_WBRG ((0 << 6) | (2 << 4) | (3 << 2) | (1))
#define NEO_WBGR ((0 << 6) | (3 << 4) | (2 << 2) | (1))

#define NEO_RWGB ((1 << 6) | (0 << 4) | (2 << 2) | (3))
#define NEO_RWBG ((1 << 6) | (0 << 4) | (3 << 2) | (2))
#define NEO_RGWB ((2 << 6) | (0 << 4) | (1 << 2) | (3))
#define NEO_RGBW ((3 << 6) | (0 << 4) | (1 << 2) | (2))
#define NEO_RBWG ((2 << 6) | (0 << 4) | (3 << 2) | (1))
#define NEO_RBGW ((3 << 6) | (0 << 4) | (2 << 2) | (1))

#define NEO_GWRB ((1 << 6) | (2 << 4) | (0 << 2) | (3))
#define NEO_GWBR ((1 << 6) | (3 << 4) | (0 << 2) | (2))
#define NEO_GRWB ((2 << 6) | (1 << 4) | (0 << 2) | (3))
#define NEO_GRBW ((3 << 6) | (1 << 4) | (0 << 2) | (2))
#define NEO_GBWR ((2 << 6) | (3 << 4) | (0 << 2) | (1))
#define NEO_GBRW ((3 << 6) | (2 << 4) | (0 << 2) | (1))

#define NEO_BWRG ((1 << 6) | (2 << 4) | (3 << 2) | (0))
#define NEO_BWGR ((1 << 6) | (3 << 4) | (2 << 2) | (0))
#define NEO_BRWG ((2 << 6) | (1 << 4) | (3 << 2) | (0))
#define NEO_BRGW ((3 << 6) | (1 << 4) | (2 << 2) | (0))
#define NEO_BGWR ((2 << 6) | (3 << 4) | (1 << 2) | (0))
#define NEO_BGRW ((3 << 6) | (2 << 4) | (1 << 2) | (0))

// If 400 KHz support is enabled, the third parameter to the constructor
// requires a 16-bit value (in order to select 400 vs 800 KHz speed).
// If only 800 KHz is enabled (as is default on ATtiny), an 8-bit value
// is sufficient to encode pixel color order, saving some space.

#define NEO_KHZ800 0x0000 // 800 KHz datastream
#define NEO_KHZ400 0x0100 // 400 KHz datastream

typedef uint16_t neoPixelType;

typedef struct neopixel_t {
  bool is800KHz;   // ...true if 800 KHz pixels
  bool begun;          // true if begin() previously called
  uint16_t numLEDs;   // Number of RGB LEDs in strip
  uint16_t numBytes;       // Size of 'pixels' buffer below (3 or 4 bytes/pixel)
  int8_t pin;
  uint8_t brightness;
  uint8_t *pixels;        // Holds LED color values (3 or 4 bytes each)
  uint8_t rOffset;        // Index of red byte within each 3- or 4-byte pixel
  uint8_t gOffset;        // Index of green byte
  uint8_t bOffset;        // Index of blue byte
  uint8_t wOffset;        // Index of white byte (same as rOffset if no white)
  uint32_t endTime;   // Latch timing reference
  uint16_t type;
  seesaw_t adafruit_seesaw;
} neopixel_t;

/***************************************************************************//**
 *    //                       Prototype
 ******************************************************************************/

/**************************************************************************/

/*!
 *     @brief  NeoPixel Initialization.
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_neopixel_init(neopixel_t *pixel,
                                   uint16_t num_led,
                                   uint8_t pixel_pin,
                                   neoPixelType pixel_type,
                                   uint8_t i2c_addr,
                                   mikroe_i2c_handle_t i2c_inst);

/**************************************************************************/

/*!
 *     @brief  show all neopixels colors
 *     @param  pixel point to pixel instance.
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_neopixel_show(neopixel_t *pixel);

/**************************************************************************/

/*!
 *     @brief  Set the output pin number.
 *     @param  pixel point to pixel instance.
 *     @param  p     pin number to set.
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_neopixel_set_pin(neopixel_t *pixel, uint8_t p);

/**************************************************************************/

/*!
 *     @brief  Set pixel color with r, g, b components.
 *     @param  pixel point to pixel instance.
 *     @param  n     pixel number to set.
 *     @param  r     r value of rgb code.
 *     @param  g     g value of rgb code.
 *     @param  b     b value of rgb code.
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_neopixel_set_pixelColor_rgb(neopixel_t *pixel,
                                                 uint16_t n,
                                                 uint8_t r,
                                                 uint8_t g,
                                                 uint8_t b);

/**************************************************************************/

/*!
 *     @brief  Set pixel color with r, g, b, w components.
 *     @param  pixel point to pixel instance.
 *     @param  n     pixel number to set.
 *     @param  r     r value of rgb code.
 *     @param  g     g value of rgb code.
 *     @param  b     b value of rgb code.
 *     @param  w     w value of rgb code.
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_neopixel_set_pixelColor_rgbw(neopixel_t *pixel,
                                                  uint16_t n,
                                                  uint8_t r,
                                                  uint8_t g,
                                                  uint8_t b,
                                                  uint8_t w);

/**************************************************************************/

/*!
 *     @brief  Set pixel color with rgb color code.
 *     @param  pixel point to pixel instance.
 *     @param  n     pixel number to set.
 *     @param  c     rgb color code.
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_neopixel_set_pixelColor(neopixel_t *pixel,
                                             uint16_t n,
                                             uint32_t c);

/**************************************************************************/

/*!
 *     @brief  Clear Neopixel.
 *     @param  pixel point to pixel instance.
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_neopixel_clear(neopixel_t *pixel);

/**************************************************************************/

/*!
 *     @brief  Update length pixel for Neopixel.
 *     @param  pixel point to pixel instance.
 *     @param  n     pixel number.
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_neopixel_update_length(neopixel_t *pixel, uint16_t n);

/**************************************************************************/

/*!
 *     @brief  Update type pixel for Neopixel.
 *     @param  pixel point to pixel instance.
 *     @param  t     pixel type.
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_neopixel_update_type(neopixel_t *pixel, neoPixelType t);

/**************************************************************************/

/*!
 *     @brief  convert r, g, b components to rgb color code.
 *     @param  r     r value of rgb code.
 *     @param  g     g value of rgb code.
 *     @param  b     b value of rgb code.
 *     @return rgb code value.
 */

/**************************************************************************/

uint32_t adafruit_neopixel_color_rgb(uint8_t r, uint8_t g, uint8_t b);

/**************************************************************************/

/*!
 *     @brief  convert r, g, b, w components to rgbw color code.
 *     @param  r     r value of rgb code.
 *     @param  g     g value of rgb code.
 *     @param  b     b value of rgb code.
 *     @param  w     w value of rgb code.
 *     @return rgb code value.
 */

/**************************************************************************/

uint32_t adafruit_neopixel_color_rgbw(uint8_t r,
                                      uint8_t g,
                                      uint8_t b,
                                      uint8_t w);

/**************************************************************************/

/*!
 *     @brief  Get current color of specified NeoPixel.
 *     @param  pixel point to pixel instance.
 *     @param  n     pixel number.
 *     @return rgb color code.
 */

/**************************************************************************/

uint32_t adafruit_neopixel_getPixelColor(neopixel_t *pixel, uint16_t n);

/**************************************************************************/

/*!
 *     @brief  Set brightness for NeoPixel.
 *     @param  pixel point to pixel instance.
 *     @param  b     brightness value.
 *     @return none.
 */

/**************************************************************************/

void adafruit_neopixel_set_brightness(neopixel_t *pixel, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif // _ADAFRUIT_NEOPIXEL_H_
