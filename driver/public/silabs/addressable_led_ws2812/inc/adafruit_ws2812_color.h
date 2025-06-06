/***************************************************************************//**
 * @file adafruit_ws2812.h
 * @brief Header file for WS2812
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
#ifndef ADAFRUIT_WS2812_COLOR_H_
#define ADAFRUIT_WS2812_COLOR_H_

#include "adafruit_ws2812.h"

#ifdef __cplusplus
extern "C" {
#endif

ws2812_color_t red = {
  .red = 255,
  .green = 0,
  .blue = 0,
  .brightness = 100
};

ws2812_color_t green = {
  .red = 0,
  .green = 255,
  .blue = 0,
  .brightness = 100
};

ws2812_color_t blue = {
  .red = 0,
  .green = 0,
  .blue = 255,
  .brightness = 100
};

ws2812_color_t no_color = {
  .red = 0,
  .green = 0,
  .blue = 0,
  .brightness = 0
};

#ifdef __cplusplus
}
#endif

#endif // ADAFRUIT_WS2812_COLOR_H_
