/***************************************************************************//**
 * @file adafruit_seesaw.h
 * @brief adafruit_seesaw header file for Adafruit NeoTrellis 4x4 keypad.
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
#ifndef _ADAFRUIT_SEESAW_H_
#define _ADAFRUIT_SEESAW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "sl_status.h"
#include "drv_i2c_master.h"

/***************************************************************************//**
 *    //                       Macro
 ******************************************************************************/
/** Module Base Addreses
 *  The module base addresses for keypad and neopixel seesaw modules.
 */
enum {
  SEESAW_NEOPIXEL_BASE = 0x0E,
  SEESAW_KEYPAD_BASE = 0x10,
};

/** neopixel module function address registers
 */
enum {
  SEESAW_NEOPIXEL_STATUS = 0x00,
  SEESAW_NEOPIXEL_PIN = 0x01,
  SEESAW_NEOPIXEL_SPEED = 0x02,
  SEESAW_NEOPIXEL_BUF_LENGTH = 0x03,
  SEESAW_NEOPIXEL_BUF = 0x04,
  SEESAW_NEOPIXEL_SHOW = 0x05,
};

/** touch module function address registers
 */
enum {
  SEESAW_TOUCH_CHANNEL_OFFSET = 0x10,
};

/** keypad module function address registers
 */
enum {
  SEESAW_KEYPAD_STATUS = 0x00,
  SEESAW_KEYPAD_EVENT = 0x01,
  SEESAW_KEYPAD_INTENSET = 0x02,
  SEESAW_KEYPAD_INTENCLR = 0x03,
  SEESAW_KEYPAD_COUNT = 0x04,
  SEESAW_KEYPAD_FIFO = 0x10,
};

/** keypad module edge definitions
 */
enum {
  SEESAW_KEYPAD_EDGE_HIGH = 0,
  SEESAW_KEYPAD_EDGE_LOW,
  SEESAW_KEYPAD_EDGE_FALLING,
  SEESAW_KEYPAD_EDGE_RISING,
};

/** raw key event stucture for keypad module */
typedef union keyEventRaw {
  struct {
    uint8_t EDGE : 2; ///< the edge that was triggered
    uint8_t NUM : 6;  ///< the event number
  } bit;              ///< bitfield format
  uint8_t reg;        ///< register format
}keyEventRaw;

/** extended key event stucture for keypad module */
typedef union keyEvent {
  struct {
    uint8_t EDGE : 2;  ///< the edge that was triggered
    uint16_t NUM : 14; ///< the event number
  } bit;               ///< bitfield format
  uint16_t reg;        ///< register format
}keyEvent;

/** key state struct that will be written to seesaw chip keypad module */
typedef union keyState {
  struct {
    uint8_t STATE : 1;  ///< the current state of the key
    uint8_t ACTIVE : 4; ///< the registered events for that key
  } bit;                ///< bitfield format
  uint8_t reg;          ///< register format
}keyState;

typedef struct seesaw_t {
  i2c_master_t  i2c_instance;
}seesaw_t;

/**************************************************************************/

/*!
 *     @brief  I2C Read function.
 *     @param  seesaw_dev seesaw instance
 *     @param  reg_high   Module base address
 *     @param  reg_low    Module function address
 *     @param  pdata      Buffer to store data
 *     @param  len        Length of data to be read
 *     @param  delay      Time to delay ms
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_seesaw_i2c_register_read(seesaw_t *seesaw_dev,
                                              uint8_t reg_high,
                                              uint8_t reg_low,
                                              uint8_t *pdata,
                                              uint8_t len,
                                              uint16_t delay_ms);

/**************************************************************************/

/*!
 *     @brief  I2C write function.
 *     @param  seesaw_dev seesaw instance
 *     @param  reg_high   Module base address
 *     @param  reg_low    Module function address
 *     @param  pdata      Data to be written
 *     @param  len        Length of data to be written
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_seesaw_i2c_register_write(seesaw_t *seesaw_dev,
                                               uint8_t reg_high,
                                               uint8_t reg_low,
                                               uint8_t *pdata,
                                               uint16_t len);

/**************************************************************************/

/*!
 *     @brief  Get keypad count from keypad module.
 *     @param  seesaw_dev seesaw instance
 *     @return keypad count.
 */

/**************************************************************************/

int8_t adafruit_seesaw_get_keypad_count(seesaw_t *seesaw_dev);

/**************************************************************************/

/*!
 *     @brief  Seesaw chip Initialization.
 *     @param  seesaw_dev seesaw instance
 *     @param  buf keypad buffer
 *     @param  count number event
 *     @return SL_STATUS_OK if success, Error code if not.
 */

/**************************************************************************/

sl_status_t adafruit_seesaw_read_keypad(seesaw_t *seesaw_dev,
                                        keyEventRaw *buf,
                                        uint8_t count);

#ifdef __cplusplus
}
#endif

#endif // _ADAFRUIT_SEESAW_H_
