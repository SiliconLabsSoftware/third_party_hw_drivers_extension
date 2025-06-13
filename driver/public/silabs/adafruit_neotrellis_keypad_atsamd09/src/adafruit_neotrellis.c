/***************************************************************************//**
 * @file adafruit_neotrellis.c
 * @brief adafruit_neotrellis source file for Adafruit NeoTrellis 4x4 keypad.
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
#include "adafruit_neotrellis.h"
#include "adafruit_neotrellis_config.h"
#include "sl_sleeptimer.h"

#define TRELLIS_MAX_DEVICE_SUPPORT 31
#define NUM_NEOTRELLIS_DEVICE      (NEOTRELLIS_NUM_COLUMN_BOARDS \
                                    * NEOTRELLIS_NUM_ROW_BOARDS)

static neotrellis_t trellis_dev[NUM_NEOTRELLIS_DEVICE];

sl_status_t adafruit_neotrellis_init(mikroe_i2c_handle_t i2cspm,
                                     uint8_t *trellis_addr,
                                     uint8_t addr_list_len)
{
  sl_status_t sc;

  if ((i2cspm == NULL) || (trellis_addr == NULL)
      || (addr_list_len < NUM_NEOTRELLIS_DEVICE)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  for (uint8_t i = 0; i < addr_list_len; i++) {
    if ((trellis_addr[i] < 0x2E) || (trellis_addr[i] > 0x4D)) {
      return SL_STATUS_FAIL;
    }
  }

  for (uint8_t i = 0; i < NUM_NEOTRELLIS_DEVICE; i++) {
    sc = adafruit_neopixel_init(&(trellis_dev[i].pixel),
                                NEO_TRELLIS_NUM_KEYS,
                                NEO_TRELLIS_NEOPIX_PIN,
                                NEO_GRB + NEO_KHZ800,
                                trellis_addr[i],
                                i2cspm);

    if (sc != SL_STATUS_OK) {
      return SL_STATUS_FAIL;
    }

    for (uint8_t keycount = 0; keycount < NEO_TRELLIS_NUM_KEYS; keycount++) {
      trellis_dev[i].callbacks[keycount] = NULL;
    }

    uint8_t enable_interrupt = 0x01;
    // enable keypad interrupt
    sc = adafruit_seesaw_i2c_register_write(&trellis_dev->pixel.adafruit_seesaw,
                                            SEESAW_KEYPAD_BASE,
                                            SEESAW_KEYPAD_INTENSET,
                                            &enable_interrupt, 1);
    if (sc != SL_STATUS_OK) {
      return SL_STATUS_FAIL;
    }
  }

  return SL_STATUS_OK;
}

void adafruit_neotrellis_registerCallback(uint16_t num,
                                          TrellisCallback (*cb)(keyEvent))
{
  uint8_t x = num % (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);
  uint8_t y = num / (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);

  adafruit_neotrellis_registerCallback_xy(x, y, cb);
}

void adafruit_neotrellis_registerCallback_xy(uint16_t x,
                                             uint16_t y,
                                             TrellisCallback (*cb)(keyEvent))
{
  uint8_t index = (y / NEO_TRELLIS_NUM_ROWS * NEOTRELLIS_NUM_COLUMN_BOARDS)
                  + (x / NEO_TRELLIS_NUM_COLS);
  int xkey = NEO_TRELLIS_X(x);
  int ykey = NEO_TRELLIS_Y(y % NEO_TRELLIS_NUM_ROWS * NEO_TRELLIS_NUM_COLS);
  int key = NEO_TRELLIS_XY(xkey, ykey);

  printf("register_Callback: index %d key %d\r\n", index, key);

  trellis_dev[index].callbacks[key] = cb;
}

void adafruit_neotrellis_unregisterCallback(uint16_t num)
{
  uint8_t x = num % (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);
  uint8_t y = num / (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);

  adafruit_neotrellis_unregisterCallback_xy(x, y);
}

void adafruit_neotrellis_unregisterCallback_xy(uint16_t x, uint16_t y)
{
  uint8_t index = (y / NEO_TRELLIS_NUM_ROWS * NEOTRELLIS_NUM_COLUMN_BOARDS)
                  + (x / NEO_TRELLIS_NUM_COLS);
  int xkey = NEO_TRELLIS_X(x);
  int ykey = NEO_TRELLIS_Y(y % NEO_TRELLIS_NUM_ROWS * NEO_TRELLIS_NUM_COLS);
  int key = NEO_TRELLIS_XY(xkey, ykey);

  trellis_dev[index].callbacks[key] = NULL;
}

sl_status_t adafruit_neotrellis_activateKey(uint16_t num,
                                            uint8_t edge,
                                            bool enable)
{
  uint8_t x = num % (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);
  uint8_t y = num / (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);

  return adafruit_neotrellis_activateKey_xy(x, y, edge, enable);
}

sl_status_t adafruit_neotrellis_activateKey_xy(uint16_t x, uint16_t y,
                                               uint8_t edge,
                                               bool enable)
{
  uint8_t index = (y / NEO_TRELLIS_NUM_ROWS * NEOTRELLIS_NUM_COLUMN_BOARDS)
                  + (x / NEO_TRELLIS_NUM_COLS);
  int xkey = NEO_TRELLIS_X(x);
  int ykey = NEO_TRELLIS_Y(y % NEO_TRELLIS_NUM_ROWS * NEO_TRELLIS_NUM_COLS);
  int key = NEO_TRELLIS_KEY(NEO_TRELLIS_XY(xkey, ykey));
  keyState ks;

  ks.bit.STATE = enable;
  ks.bit.ACTIVE = (1 << edge);
  uint8_t cmd[] = { key, ks.reg };

  return adafruit_seesaw_i2c_register_write(
    &trellis_dev[index].pixel.adafruit_seesaw,
    SEESAW_KEYPAD_BASE,
    SEESAW_KEYPAD_EVENT,
    cmd,
    2);
}

sl_status_t adafruit_neotrellis_setPixelColor_xy(uint16_t x,
                                                 uint16_t y,
                                                 uint32_t color)
{
  uint8_t index = (y / NEO_TRELLIS_NUM_ROWS * NEOTRELLIS_NUM_COLUMN_BOARDS)
                  + (x / NEO_TRELLIS_NUM_COLS);
  int xkey = NEO_TRELLIS_X(x);
  int ykey = NEO_TRELLIS_Y(y % NEO_TRELLIS_NUM_ROWS * NEO_TRELLIS_NUM_COLS);
  int key = NEO_TRELLIS_XY(xkey, ykey);
  printf("pixel color: index %d key %d\r\n", index, key);
  return adafruit_neopixel_set_pixelColor(&(trellis_dev[index].pixel),
                                          key,
                                          color);
}

sl_status_t adafruit_neotrellis_setPixelColor(uint16_t num, uint32_t color)
{
  uint8_t x = num % (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);
  uint8_t y = num / (NEO_TRELLIS_NUM_COLS * NEOTRELLIS_NUM_COLUMN_BOARDS);

  return adafruit_neotrellis_setPixelColor_xy(x, y, color);
}

void adafruit_neotrellis_show()
{
  for (uint8_t i = 0; i < NUM_NEOTRELLIS_DEVICE; i++) {
    adafruit_neopixel_show(&(trellis_dev[i].pixel));
  }
}

void adafruit_neotrellis_read()
{
  for (uint8_t n = 0; n < NUM_NEOTRELLIS_DEVICE; n++) {
    int8_t count = adafruit_seesaw_get_keypad_count(
      &trellis_dev[n].pixel.adafruit_seesaw);

    sl_sleeptimer_delay_millisecond(1);

    if (count > 0) {
      count = count + 2;
      keyEventRaw e[count];
      adafruit_seesaw_read_keypad(&trellis_dev[n].pixel.adafruit_seesaw, e,
                                  count);
      for (int i = 0; i < count; i++) {
        // call any callbacks associated with the key
        e[i].bit.NUM = NEO_TRELLIS_SEESAW_KEY(e[i].bit.NUM);
        if ((e[i].bit.NUM < NEO_TRELLIS_NUM_KEYS)
            && (trellis_dev[n].callbacks[e[i].bit.NUM] != NULL)) {
          // update the event with the multitrellis number
          keyEvent evt = { { e[i].bit.EDGE, e[i].bit.NUM } };
          int x = NEO_TRELLIS_X(e[i].bit.NUM);
          int y = NEO_TRELLIS_Y(e[i].bit.NUM);

          x = x + (n % NEOTRELLIS_NUM_COLUMN_BOARDS) * NEO_TRELLIS_NUM_COLS;
          y = y + (n / NEOTRELLIS_NUM_COLUMN_BOARDS) * NEO_TRELLIS_NUM_ROWS;

          evt.bit.NUM = (y * NEO_TRELLIS_NUM_COLS
                         * NEOTRELLIS_NUM_COLUMN_BOARDS) + x;

          trellis_dev[n].callbacks[e[i].bit.NUM](evt);
        }
      }
    }
  }
}
