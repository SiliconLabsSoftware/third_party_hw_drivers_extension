/***************************************************************************//**
 * @file adafruit_seesaw.c
 * @brief adafruit_seesaw source file for Adafruit NeoTrellis 4x4 keypad.
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
#include <stdlib.h>
#include "sl_sleeptimer.h"
#include "adafruit_seesaw.h"

sl_status_t adafruit_seesaw_i2c_register_read(seesaw_t *seesaw_dev,
                                              uint8_t reg_high,
                                              uint8_t reg_low,
                                              uint8_t *pdata,
                                              uint8_t len,
                                              uint16_t delay_ms)
{
  uint8_t pos = 0;
  uint8_t i2c_write_data[2];
  uint8_t read_now;

  while (pos < len) {
    if ((pos + 32) < len) {
      read_now = 32;
    } else {
      read_now = len;
    }

    i2c_write_data[0] = reg_high;
    i2c_write_data[1] = reg_low;

    if (I2C_MASTER_SUCCESS != i2c_master_write(&seesaw_dev->i2c_instance,
                                               i2c_write_data,
                                               2)) {
      return SL_STATUS_TRANSMIT;
    }

    sl_sleeptimer_delay_millisecond(delay_ms);

    if (I2C_MASTER_SUCCESS != i2c_master_read(&seesaw_dev->i2c_instance,
                                              pdata,
                                              read_now)) {
      return SL_STATUS_TRANSMIT;
    }

    pos += read_now;
    pdata += pos;
  }

  return SL_STATUS_OK;
}

sl_status_t adafruit_seesaw_i2c_register_write(seesaw_t *seesaw_dev,
                                               uint8_t reg_high,
                                               uint8_t reg_low,
                                               uint8_t *pdata,
                                               uint16_t len)
{
  uint8_t data_buffer[len + 2];
  data_buffer[0] = reg_high;
  data_buffer[1] = reg_low;

  if ((NULL != pdata) && (len > 0)) {
    for (uint8_t i = 0; i < len; i++)
    {
      data_buffer[i + 2] = pdata[i];
    }
  }

  err_t stt = i2c_master_write(&seesaw_dev->i2c_instance,
                               data_buffer,
                               len + 2
                               );

  if (I2C_MASTER_SUCCESS != stt) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

int8_t adafruit_seesaw_get_keypad_count(seesaw_t *seesaw_dev)
{
  sl_status_t sc;
  int8_t count = 0;

  sc = adafruit_seesaw_i2c_register_read(seesaw_dev,
                                         SEESAW_KEYPAD_BASE,
                                         SEESAW_KEYPAD_COUNT,
                                         (uint8_t *)&count,
                                         1, 1);

  if (sc != SL_STATUS_OK) {
    return -1;
  }

  return count;
}

/**
 *****************************************************************************
 *  @brief      Read all keyEvents into the passed buffer
 *
 *  @param      buf pointer to where the keyEvents should be stored
 *  @param		count the number of events to read
 *  @returns    True on I2C read success
 *****************************************************************************/
sl_status_t adafruit_seesaw_read_keypad(seesaw_t *seesaw_dev,
                                        keyEventRaw *buf,
                                        uint8_t count)
{
  return adafruit_seesaw_i2c_register_read(seesaw_dev,
                                           SEESAW_KEYPAD_BASE,
                                           SEESAW_KEYPAD_FIFO,
                                           (uint8_t *)buf,
                                           count,
                                           1);
}
