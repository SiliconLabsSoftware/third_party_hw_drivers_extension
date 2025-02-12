/***************************************************************************//**
 * @file
 * @brief Top level application functions
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

#include <string.h>
#include "sl_sleeptimer.h"
#include "adafruit_neotrellis.h"
#include "adafruit_neotrellis_config.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED            SL_I2C2

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

/***************************************************************************//**
 *    //                       Definition
 ******************************************************************************/

#define NUMBER_NEOTRELLIS_BOARD      (NEOTRELLIS_NUM_COLUMN_BOARDS \
                                      * NEOTRELLIS_NUM_ROW_BOARDS)

/***************************************************************************//**
 *    //                       Global Variables
 ******************************************************************************/

static uint8_t keypad_addr_list[NUMBER_NEOTRELLIS_BOARD] = { 0x2E };
static uint32_t color_list[8] = { 0xff0000, 0xff00ff, 0xffff00, 0x00ff00,
                                  0x0000ff, 0x00ffff, 0xffffff, 0xc0c0c0 };

/***************************************************************************//**
 *    //                       Static Functions
 ******************************************************************************/

static TrellisCallback blink(keyEvent evt);

static mikroe_i2c_handle_t app_i2c_instance = NULL;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc = SL_STATUS_OK;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  app_printf("Adafruit NeoTrellis 4x4 Keypad Initializing....\r\n");

  sc = adafruit_neotrellis_init(app_i2c_instance,
                                keypad_addr_list,
                                NUMBER_NEOTRELLIS_BOARD);

  if (sc != SL_STATUS_OK) {
    app_printf("Keypad Initilization fail\r\n");
    while (1) {}
  }

  app_printf("number keypad: %d\r\n", NUMBER_NEOTRELLIS_BOARD);

  for (uint8_t i = 0; i < 16; i++) {
    sc |= adafruit_neotrellis_activateKey(i, SEESAW_KEYPAD_EDGE_FALLING, true);
    sc |= adafruit_neotrellis_activateKey(i, SEESAW_KEYPAD_EDGE_RISING, true);
    adafruit_neotrellis_registerCallback(i, blink);
  }

  for (uint8_t i = 0; i < 16; i++) {
    adafruit_neotrellis_setPixelColor(i, 0xff00ff);
    adafruit_neotrellis_show();
    sl_sleeptimer_delay_millisecond(100);
    adafruit_neotrellis_setPixelColor(i, 0x000000);
    adafruit_neotrellis_show();
  }

  if (sc != SL_STATUS_OK) {
    app_printf("\rApplication Initialization fail.\r\n");
    while (1) {}
  } else {
    app_printf("\rApplication Initialization done.\r\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  adafruit_neotrellis_read();
}

static TrellisCallback blink(keyEvent evt)
{
  static uint8_t i = 0;
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    adafruit_neotrellis_setPixelColor(evt.bit.NUM, color_list[i]);
    adafruit_neotrellis_show();
  } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    adafruit_neotrellis_setPixelColor(evt.bit.NUM, 0x000000);
    adafruit_neotrellis_show();
  }

  i = (i < 8) ? (i + 1) : 0;
  return 0;
}
