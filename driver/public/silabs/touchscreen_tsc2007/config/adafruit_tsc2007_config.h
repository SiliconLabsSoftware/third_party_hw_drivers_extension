/***************************************************************************//**
 * @file adafruit_tsc2007_config.h
 * @brief Configuration file for Adafruit Touch Screen Controller TSC2007.
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
#ifndef ADAFRUIT_TSC2007_CONFIG_H_
#define ADAFRUIT_TSC2007_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>
// <h> TSC2007 I2C address

// <o ADAFRUIT_TSC2007_I2CADDR> TSC2007 i2c address <f.h>
// <i> Default: 0x48
// <d> 0x48
#define ADAFRUIT_TSC2007_I2CADDR                 0x48
// </h>

// <h> Touch Screen Configuration
// <o ADAFRUIT_TSC2007_HOR_RES> Touch Screen Horizontal Resolution
// <i> Default: 320
// <d> 320
#define ADAFRUIT_TSC2007_HOR_RES                 240

// <o ADAFRUIT_TSC2007_VER_RES> Touch Screen Vertical Resolution
// <i> Default: 240
// <d> 240
#define ADAFRUIT_TSC2007_VER_RES                 320

// <o ADAFRUIT_TSC2007_X_MIN> Touch Screen X-axis Minimum Value
// <i> Default: 200
// <d> 200
#define ADAFRUIT_TSC2007_X_MIN                   328

// <o ADAFRUIT_TSC2007_Y_MIN> Touch Screen Y-axis Minimum Value
// <i> Default: 200
// <d> 200
#define ADAFRUIT_TSC2007_Y_MIN                   431

// <o ADAFRUIT_TSC2007_X_MAX> Touch Screen X-axis Maximum Value
// <i> Default: 3800
// <d> 3800
#define ADAFRUIT_TSC2007_X_MAX                   3788

// <o ADAFRUIT_TSC2007_Y_MAX> Touch Screen Y-axis Maximum Value
// <i> Default: 3800
// <d> 3800
#define ADAFRUIT_TSC2007_Y_MAX                   3821

// <o ADAFRUIT_TSC2007_XPLATE_RES> X-plate resistance
// <i> Default: 200
// <d> 200
#define ADAFRUIT_TSC2007_XPLATE_RES              200

// <q ADAFRUIT_TSC2007_X_INV> Invert X-axis
// <i> Default: 0
// <d> 0
#define ADAFRUIT_TSC2007_X_INV                   1

// <q ADAFRUIT_TSC2007_Y_INV> Invert Y-axis
// <i> Default: 0
// <d> 0
#define ADAFRUIT_TSC2007_Y_INV                   1

// <q ADAFRUIT_TSC2007_XY_SWAP> XY Swap
// <i> Default: 0
// <d> 0
#define ADAFRUIT_TSC2007_XY_SWAP                 0

// </h>

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> ADAFRUIT_TSC2007_INT
// $[GPIO_ADAFRUIT_TSC2007_INT]
#define ADAFRUIT_TSC2007_INT_PORT                SL_GPIO_PORT_B
#define ADAFRUIT_TSC2007_INT_PIN                 0
// [GPIO_ADAFRUIT_TSC2007_INT]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // ADAFRUIT_TSC2007_CONFIG_H_
