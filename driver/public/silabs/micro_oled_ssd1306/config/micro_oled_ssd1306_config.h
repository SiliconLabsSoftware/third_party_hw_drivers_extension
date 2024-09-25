/***************************************************************************//**
 * @file micro_oled_ssd1306_config.h
 * @brief Micro OLED SSD1306 Configuration
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
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/

#ifndef SSD1306_I2C_CONFIG_H_
#define SSD1306_I2C_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define OLED_DISPLAY_64_48               0
#define OLED_DISPLAY_96_39               1
#define OLED_DISPLAY_128_32              2
#define OLED_DISPLAY_128_64              3

// <<< Use Configuration Wizard in Context Menu >>>

// <h> MICRO OLED I2C Configuration

// <e> MICRO OLED I2C UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MICRO_OLED_I2C__UC                  0

// <o MICRO_OLED_I2C_SPEED_MODE> Speed mode
// <0=> Standard mode (100kbit/s)
// <1=> Fast mode (400kbit/s)
// <2=> Fast mode plus (1Mbit/s)
// <i> Default: 0
#define MICRO_OLED_I2C_SPEED_MODE          0

// </e>
// </h>

// <h> Connection
// <o CONFIG_OLED_DISPLAY_ADDRESS> I2C Address <0x1..0x7E:0x1><f.h>
// <i> Default: 0x3D
#define CONFIG_OLED_DISPLAY_ADDRESS      0x3D
// </h>
// <h> Screen

// <o CONFIG_OLED_DISPLAY_RESOLUTION> OLED Screen Resolution
// <OLED_DISPLAY_64_48=> 64x48
// <OLED_DISPLAY_96_39=> 96x39
// <OLED_DISPLAY_128_32=> 128x32
// <OLED_DISPLAY_128_64=> 128x64
// <i> Default: 64x48
// <i> The screen width parameter is used
// <i> to configure display driver.
#define CONFIG_OLED_DISPLAY_RESOLUTION  OLED_DISPLAY_64_48

// </h>
// <<< end of configuration section >>>

#if (CONFIG_OLED_DISPLAY_RESOLUTION == OLED_DISPLAY_64_48)
#define CONFIG_OLED_DISPLAY_64_48
#define SSD1306_DISPLAY_WIDTH                         64
#define SSD1306_DISPLAY_HEIGHT                        48
#define SSD1306_USE_PAGE_ADDRESSING_MODE
#elif (CONFIG_OLED_DISPLAY_RESOLUTION == OLED_DISPLAY_96_39)
#define CONFIG_OLED_DISPLAY_96_39
#define SSD1306_DISPLAY_WIDTH                         96
#define SSD1306_DISPLAY_HEIGHT                        39
#define SSD1306_USE_PAGE_ADDRESSING_MODE
#elif (CONFIG_OLED_DISPLAY_RESOLUTION == OLED_DISPLAY_128_32)
#define CONFIG_OLED_DISPLAY_128_32
#define SSD1306_DISPLAY_WIDTH                         128
#define SSD1306_DISPLAY_HEIGHT                        32
#elif (CONFIG_OLED_DISPLAY_RESOLUTION == OLED_DISPLAY_128_64)
#define CONFIG_OLED_DISPLAY_128_64
#define SSD1306_DISPLAY_WIDTH                         128
#define SSD1306_DISPLAY_HEIGHT                        64
#endif

#ifdef __cplusplus
}
#endif

#endif // SSD1306_I2C_CONFIG_H_
