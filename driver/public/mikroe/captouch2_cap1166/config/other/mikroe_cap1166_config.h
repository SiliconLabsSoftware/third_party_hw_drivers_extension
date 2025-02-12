/***************************************************************************//**
 * @file mikroe_cap1166_config.h
 * @brief SCL CAP1166 Configuration
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

#ifndef SCL_CAP1166_CONFIG_H_
#define SCL_CAP1166_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> MIKROE CAP1166 SPI Configuration

// <e> MIKROE CAP1166 SPI UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_CAP1166_SPI_UC                  0

// <o MIKROE_CAP1166_SPI_BITRATE> Bit Rate (Bits/Second) <1-116000000>
// <i> Default: 10000000
#define MIKROE_CAP1166_SPI_BITRATE             1000000

// </e>
// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> CAP1166_RESET
// $[GPIO_CAP1166_RESET]
#warning "Reset pin for CAP1166 is not configured"
// #define CAP1166_RESET_PORT                   0
// #define CAP1166_RESET_PIN                    0
// [GPIO_CAP1166_RESET]$

// <gpio optional=true> CAP1166_ALERT
// $[GPIO_CAP1166_ALERT]
#warning "ALT pin for CAP1166 is not configured"
// #define CAP1166_ALERT_PORT                   0
// #define CAP1166_ALERT_PIN                    0
// [GPIO_CAP1166_ALERT]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // SCL_CAP1166_CONFIG_H_
