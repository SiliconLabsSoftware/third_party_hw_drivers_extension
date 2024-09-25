/***************************************************************************//**
 * @file adafruit_is31fl3741_config.h
 * @brief Configuration file for Adafruit RGB LED Driver board.
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

#ifndef ADAFRUIT_IS31FL3741_CONFIG_H_
#define ADAFRUIT_IS31FL3741_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define IS31FL3741_DEFAULT_I2C_ADDR                 0x30

// <<< Use Configuration Wizard in Context Menu >>>

//<h>ADAFRUIT IS31FL3741 I2C Configuration

//  <e>ADAFRUIT IS31FL3741 I2C UC Configuration
//  <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
//  <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
//  <i> Default: 0
#define ADAFRUIT_IS31FL3741_I2C_UC                  0

// <o ADAFRUIT_IS31FL3741_I2C_SPEED_MODE> Speed mode
// <0=> Standard mode (100kbit/s)
// <1=> Fast mode (400kbit/s)
// <2=> Fast mode plus (1Mbit/s)
// <i> Default: 0
#define ADAFRUIT_IS31FL3741_I2C_SPEED_MODE          0

// </e>
// </h>

// <h> Display Configuration
// <o IS31FL3741_DISPLAY_LAYOUT> Display Layout
// <0=> 1x1
// <1=> 1x2
// <2=> 1x3
// <3=> 1x4
// <4=> 2x1
// <5=> 2x2
// <i> Default: 0
#define IS31FL3741_DISPLAY_LAYOUT                   0
// </h>

// <h> Connection
// <o DISPLAY_1_I2C_ADDRESS> DISPLAY 1 I2C ADDRESS
// <0=> None
// <0x30=> 0x30
// <0x31=> 0x31
// <0x32=> 0x32
// <0x33=> 0x33
// <i> Default: 0x30
#define DISPLAY_1_I2C_ADDRESS                       0x30

// <o DISPLAY_2_I2C_ADDRESS> DISPLAY 2 I2C ADDRESS
// <0=> None
// <0x30=> 0x30
// <0x31=> 0x31
// <0x32=> 0x32
// <0x33=> 0x33
// <i> Default: 0
#define DISPLAY_2_I2C_ADDRESS                       0

// <o DISPLAY_3_I2C_ADDRESS> DISPLAY 3 I2C ADDRESS
// <0=> None
// <0x30=> 0x30
// <0x31=> 0x31
// <0x32=> 0x32
// <0x33=> 0x33
// <i> Default: 0
#define DISPLAY_3_I2C_ADDRESS                       0

// <o DISPLAY_4_I2C_ADDRESS> DISPLAY 4 I2C ADDRESS
// <0=> None
// <0x30=> 0x30
// <0x31=> 0x31
// <0x32=> 0x32
// <0x33=> 0x33
// <i> Default: 0
#define DISPLAY_4_I2C_ADDRESS                       0
// </h>

// <<< end of configuration section >>>

#ifdef __cplusplus
}
#endif

#endif /* ADAFRUIT_IS31FL3741_CONFIG_H_ */
