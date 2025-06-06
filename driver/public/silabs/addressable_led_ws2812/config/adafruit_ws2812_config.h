/***************************************************************************//**
 * @file adafruit_ws2812_config.h
 * @brief Configuration file for WS2812
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef ADAFRUIT_WS2812_CONFIG_H_
#define ADAFRUIT_WS2812_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WS2812_BIT_ONE           0xF8
#define WS2812_BIT_ZERO          0xC0
#define WS2812_BIT_RESET         0x00
#define WS2812_FRAME_LENGTH      24
#define WS2812_SPI_SPEED         6000000

// <<< Use Configuration Wizard in Context Menu >>>
// <h> WS2812 Setting

// <o WS2812_LED_NUMBER> Max number of used WS2812 LEDs
// <i> Default: 60
#define WS2812_LED_NUMBER        60

// </h>
// <<< end of configuration section >>>
#ifdef __cplusplus
}
#endif
#endif // ADAFRUIT_WS2812_CONFIG_H_
