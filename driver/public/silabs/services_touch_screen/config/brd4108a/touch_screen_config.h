/***************************************************************************//**
 * @file touch_screen_config.h
 * @brief Configuration file for touch screen driver.
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
#ifndef TOUCH_SCREEN_CONFIG_H_
#define TOUCH_SCREEN_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Touch settings

// <o TOUCHSCREEN_HOR_RES> Touch Screen Horizontal Resolution
// <i> Default: 240
#define TOUCHSCREEN_HOR_RES              240

// <o TOUCHSCREEN_VER_RES> Touch Screen Vertical Resolution
// <i> Default: 320
#define TOUCHSCREEN_VER_RES              320

// <o TOUCHSCREEN_XPLATE_RES> X-plate resistance
// <i> Default: 200
// <d> 200
#define TOUCHSCREEN_XPLATE_RES              200

// <q TOUCHSCREEN_X_INV> Invert X-axis
// <i> Default: 0
// <d> 0
#define TOUCHSCREEN_X_INV                   1

// <q TOUCHSCREEN_Y_INV> Invert Y-axis
// <i> Default: 0
// <d> 0
#define TOUCHSCREEN_Y_INV                   1

// <q TOUCHSCREEN_XY_SWAP> XY Swap
// <i> Default: 0
// <d> 0
#define TOUCHSCREEN_XY_SWAP             0

// <o TOUCHSCREEN_X_MIN> Calib X-min
// <i> Default: 460
#define TOUCHSCREEN_X_MIN              460

// <o TOUCHSCREEN_X_MAX> Calib X-max
// <i> Default: 3620
#define TOUCHSCREEN_X_MAX              3620

// <o TOUCHSCREEN_Y_MIN> Calib Y-min
// <i> Default: 340
#define TOUCHSCREEN_Y_MIN              340

// <o TOUCHSCREEN_Y_MAX> Calib Y-max
// <i> Default: 3580
#define TOUCHSCREEN_Y_MAX              3580

// </h>

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>
// <gpio optional=true> TOUCHSCREEN_XP
// $[GPIO_TOUCHSCREEN_XP]
#define TOUCHSCREEN_XP_PORT                            SL_GPIO_PORT_B
#define TOUCHSCREEN_XP_PIN                             2
// [GPIO_TOUCHSCREEN_XP]$

// <gpio optional=true> TOUCHSCREEN_YP
// $[GPIO_TOUCHSCREEN_YP]
#define TOUCHSCREEN_YP_PORT                            SL_GPIO_PORT_D
#define TOUCHSCREEN_YP_PIN                             2
// [GPIO_TOUCHSCREEN_YP]$

// <gpio optional=true> TOUCHSCREEN_XM
// $[GPIO_TOUCHSCREEN_XM]
#define TOUCHSCREEN_XM_PORT                            SL_GPIO_PORT_B
#define TOUCHSCREEN_XM_PIN                             4
// [GPIO_TOUCHSCREEN_XM]$

// <gpio optional=true> TOUCHSCREEN_YM
// $[GPIO_TOUCHSCREEN_YM]
#define TOUCHSCREEN_YM_PORT                            SL_GPIO_PORT_B
#define TOUCHSCREEN_YM_PIN                             3
// [GPIO_TOUCHSCREEN_YM]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
extern "C"
}
#endif

#endif // TOUCH_SCREEN_CONFIG_H_
