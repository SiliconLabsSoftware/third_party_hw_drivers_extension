/***************************************************************************//**
 * @file adafruit_neotrellis.h
 * @brief adafruit_neotrellis header file for Adafruit NeoTrellis 4x4 keypad.
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
#ifndef _ADAFRUIT_NEOTRELLIS_H_
#define _ADAFRUIT_NEOTRELLIS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "adafruit_seesaw.h"
#include "adafruit_neopixel.h"

/***************************************************************************//**
 *    //                       Macro
 ******************************************************************************/
#define NEO_TRELLIS_ADDR          0x2E

#define NEO_TRELLIS_NEOPIX_PIN    3

#define NEO_TRELLIS_NUM_ROWS      4
#define NEO_TRELLIS_NUM_COLS      4
#define NEO_TRELLIS_NUM_KEYS      (NEO_TRELLIS_NUM_ROWS * NEO_TRELLIS_NUM_COLS)

#define NEO_TRELLIS_MAX_CALLBACKS 32

#define NEO_TRELLIS_KEY(x)        (((x) / 4) * 8 + ((x) % 4))
#define NEO_TRELLIS_SEESAW_KEY(x) (((x) / 8) * 4 + ((x) % 8))

#define NEO_TRELLIS_X(k)          ((k) % 4)
#define NEO_TRELLIS_Y(k)          ((k) / 4)

#define NEO_TRELLIS_XY(x, y)      ((y) * NEO_TRELLIS_NUM_COLS + (x))

typedef void (*TrellisCallback)(keyEvent evt);

typedef struct neotrellis_t {
  ///< the I2C address of this board
  uint8_t addr;
  ///< the array of callback functions
  TrellisCallback (*callbacks[NEO_TRELLIS_NUM_KEYS])(keyEvent);
  neopixel_t pixel;
}neotrellis_t;

/***************************************************************************//**
 *    //                       Prototype
 ******************************************************************************/

/**************************************************************************/

/*!
 *     @brief  Neotrellis keypad initialization function.
 *     @param  i2cspm I2C instance.
 *     @param  trellis_addr list of address for all trellis board in order from
 *             top left to bottom right.
 *     @param  addr_list_len lenght of address list.
 */

/**************************************************************************/
sl_status_t adafruit_neotrellis_init(mikroe_i2c_handle_t i2cspm,
                                     uint8_t *trellis_addr,
                                     uint8_t addr_list_len);

/**************************************************************************/

/*!
 *     @brief  register a callback for a key addressed by key number.
 *     @param  num the keynumber to set the color of. Key 0 is in the top left
 *             corner of the trellis matrix, key 1 is directly to the right of
 *   it,
 *             and the last key number is in the bottom righthand corner.
 *     @param  cb the function to be called when an event from the specified key
 *   is
 *             detected.
 */

/**************************************************************************/
void adafruit_neotrellis_registerCallback(uint16_t num,
                                          TrellisCallback (*cb)(keyEvent));

/**************************************************************************/

/*!
 *     @brief  register a callback for a key addressed by key index.
 *     @param  x the column index of the key. column 0 is on the lefthand side
 *   of
 *    the matix.
 *     @param  y the row index of the key. row 0 is at the top of the matrix and
 *    the numbers increase downwards.
 *     @param  cb the function to be called when an event from the specified key
 *   is
 *             detected.
 */

/**************************************************************************/

void adafruit_neotrellis_registerCallback_xy(uint16_t x,
                                             uint16_t y,
                                             TrellisCallback (*cb)(keyEvent));

/**************************************************************************/

/*!
 *     @brief  Unregister a callback for a key addressed by key number.
 *     @param  num the keynumber to set the color of. Key 0 is in the top left
 *             corner of the trellis matrix, key 1 is directly to the right of
 *   it,
 *             and the last key number is in the bottom righthand corner.
 */

/**************************************************************************/

void adafruit_neotrellis_unregisterCallback(uint16_t num);

/**************************************************************************/

/*!
 *     @brief  Unregister a callback for a key addressed by key index.
 *     @param  x the column index of the key. column 0 is on the lefthand side
 *   of
 *    the matix.
 *     @param  y the row index of the key. row 0 is at the top of the matrix and
 *    the numbers increase downwards.
 */

/**************************************************************************/

void adafruit_neotrellis_unregisterCallback_xy(uint16_t x, uint16_t y);

/**************************************************************************/

/*!
 *     @brief  Activate or deactivate a key by number.
 *     @param  num the keynumber to set the color of. Key 0 is in the top left
 *             corner of the trellis matrix, key 1 is directly to the right of
 *   it,
 *             and the last key number is in the bottom righthand corner.
 *     @param  edge the edge that the key triggers an event on. Use
 *    SEESAW_KEYPAD_EDGE_FALLING or SEESAW_KEYPAD_EDGE_RISING.
 *     @param  enable pass true to enable the key on the passed edge, false to
 *    disable.
 */

/**************************************************************************/

sl_status_t adafruit_neotrellis_activateKey(uint16_t num,
                                            uint8_t edge,
                                            bool enable);

/**************************************************************************/

/*!
 *     @brief  Activate or deactivate a key by number.
 *     @param  x the column index of the key. column 0 is on the lefthand side
 *   of
 *    the matix.
 *     @param  y the row index of the key. row 0 is at the top of the matrix and
 *    the numbers increase downwards.
 *     @param  edge the edge that the key triggers an event on. Use
 *    SEESAW_KEYPAD_EDGE_FALLING or SEESAW_KEYPAD_EDGE_RISING.
 *     @param  enable pass true to enable the key on the passed edge, false to
 *    disable.
 */

/**************************************************************************/

sl_status_t adafruit_neotrellis_activateKey_xy(uint16_t x,
                                               uint16_t y,
                                               uint8_t edge,
                                               bool enable);

/**************************************************************************/

/*!
 *     @brief  set the color of a neopixel at a key index.
 *     @param  x the column index of the key. column 0 is on the lefthand side
 *   of
 *    the matix.
 *     @param  y the row index of the key. row 0 is at the top of the matrix and
 *    the numbers increase downwards.
 *     @param  color the color to set the pixel to. This is a 24 bit RGB value.
 *             for example, full brightness red would be 0xFF0000, and full
 *    brightness blue would be 0x0000FF.
 */

/**************************************************************************/

sl_status_t adafruit_neotrellis_setPixelColor_xy(uint16_t x,
                                                 uint16_t y,
                                                 uint32_t color);

/**************************************************************************/

/*!
 *     @brief  set the color of a neopixel at a key number.
 *     @param  num the keynumber to set the color of. Key 0 is in the top left
 *             corner of the trellis matrix, key 1 is directly to the right of
 *   it,
 *             and the last key number is in the bottom righthand corner.
 *     @param  color the color to set the pixel to. This is a 24 bit RGB value.
 *             for example, full brightness red would be 0xFF0000, and full
 *    brightness blue would be 0x0000FF.
 */

/**************************************************************************/

sl_status_t adafruit_neotrellis_setPixelColor(uint16_t num, uint32_t color);

/**************************************************************************/

/*!
 *     @brief  call show for all connected neotrellis boards to show all
 *   neopixels
 */

/**************************************************************************/

void adafruit_neotrellis_show();

/**************************************************************************/

/*!
 *     @brief  read all events currently stored in the seesaw fifo and call any
 *    callbacks.
 */

/**************************************************************************/

void adafruit_neotrellis_read();

#ifdef __cplusplus
}
#endif

#endif // _ADAFRUIT_NEOTRELLIS_H_
