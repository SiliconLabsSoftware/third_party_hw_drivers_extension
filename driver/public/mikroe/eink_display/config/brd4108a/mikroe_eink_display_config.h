/***************************************************************************//**
 * @file mikroe_eink_diplay_config.h
 * @brief Mikroe eINK Display Configuration
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
#ifndef MIKROE_EINK_DISPLAY_CONFIG_H_
#define MIKROE_EINK_DISPLAY_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define EINK_DISPLAY_1_54_INCH              0
#define EINK_DISPLAY_2_13_INCH              1
#define EINK_DISPLAY_2_90_INCH              2

// A CMSIS annotation block starts with the following line:
// <<< Use Configuration Wizard in Context Menu >>>

// <h> MIKROE EINK DISPLAY SPI Configuration

// <e> MIKROE EINK DISPLAY SPI UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_EINK_DISPLAY_SPI_UC              0

// <o MIKROE_EINK_DISPLAY_SPI_UC_BITRATE> Bit Rate (Bits/Second) <1-116000000>
// <i> Default: 10000000
#define MIKROE_EINK_DISPLAY_SPI_UC_BITRATE      10000000

// </e>
// </h>

// <h> Screen

// <o CONFIG_EINK_DISPLAY_RESOLUTION> eINK Display Resolution
// <EINK_DISPLAY_1_54_INCH=> 200x200
// <EINK_DISPLAY_2_13_INCH=> 122x250
// <EINK_DISPLAY_2_90_INCH=> 296x128
// <i> Default: EINK_DISPLAY_1_54_INCH
// <i> The screen width parameter is used
// <i> to configure display driver.
#define CONFIG_EINK_DISPLAY_RESOLUTION  EINK_DISPLAY_1_54_INCH

// </h>

// <h> ESL MODE setting
// <e> ESL_MODE> Enable Peripheral
// <i> Macro for the ESL mode
#define ESL_MODE                               0
// </e>
// </h>

// The block ends with the following line or at the end of the file:
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> EINK_DISPLAY_DC
// $[GPIO_EINK_DISPLAY_DC]
#define EINK_DISPLAY_DC_PORT                        SL_GPIO_PORT_B
#define EINK_DISPLAY_DC_PIN                         4
// [GPIO_EINK_DISPLAY_DC]$

// <gpio optional=true> EINK_DISPLAY_RST
// $[GPIO_EINK_DISPLAY_RST]
#define EINK_DISPLAY_RST_PORT                       SL_GPIO_PORT_C
#define EINK_DISPLAY_RST_PIN                        6
// [GPIO_EINK_DISPLAY_RST]$

// <gpio optional=true> EINK_DISPLAY_BSY
// $[GPIO_EINK_DISPLAY_BSY]
#define EINK_DISPLAY_BSY_PORT                       SL_GPIO_PORT_B
#define EINK_DISPLAY_BSY_PIN                        3
// [GPIO_EINK_DISPLAY_BSY]$

// <<< sl:end pin_tool >>>

#if (CONFIG_EINK_DISPLAY_RESOLUTION == EINK_DISPLAY_1_54_INCH)
#define MIKROE_EINK_DISPLAY_WIDTH                   200
#define MIKROE_EINK_DISPLAY_HEIGHT                  200
#elif (CONFIG_EINK_DISPLAY_RESOLUTION == EINK_DISPLAY_2_13_INCH)
#define MIKROE_EINK_DISPLAY_WIDTH                   128
#define MIKROE_EINK_DISPLAY_HEIGHT                  250
#elif (CONFIG_EINK_DISPLAY_RESOLUTION == EINK_DISPLAY_2_90_INCH)
#define MIKROE_EINK_DISPLAY_WIDTH                   128
#define MIKROE_EINK_DISPLAY_HEIGHT                  296
#endif

#ifdef __cplusplus
}
#endif

#endif // MIKROE_EINK_DISPLAY_CONFIG_H_
