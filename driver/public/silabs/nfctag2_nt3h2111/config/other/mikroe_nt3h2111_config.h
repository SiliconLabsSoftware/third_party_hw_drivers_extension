/***************************************************************************//**
 * @file mikroe_nt3h2111_config.h
 * @brief SCL MIKROE NT3H2111 Configuration
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

#ifndef MIKROE_NT3H211_CONFIG_H_
#define MIKROE_NT3H211_CONFIG_H_

// A CMSIS annotation block starts with the following line:
// <<< Use Configuration Wizard in Context Menu >>>

// <h> MIKROE_NT3H211 I2C Configuration

// <e> MIKROE_NT3H211 I2C UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_NT3H211_I2C_UC                  0

// <o MIKROE_NT3H211_I2C_SPEED_MODE> Speed mode
// <0=> Standard mode (100kbit/s)
// <1=> Fast mode (400kbit/s)
// <2=> Fast mode plus (1Mbit/s)
// <i> Default: 0
#define MIKROE_NT3H211_I2C_SPEED_MODE          0

// </e>
// </h>

// <h> Connection
// <o MIKROE_NT3H211_ADDR> I2C Address <0-0xFF><f.h>
// <i> Default: 0x55
#define MIKROE_NT3H211_ADDR                            0x55
// </h>

// The block ends with the following line or at the end of the file:
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MIKROE_NT3H211_FD
// $[GPIO_MIKROE_NT3H211_FD]
#warning "FD GPIO not configured"
// #define MIKROE_NT3H211_FD_PORT                        0
// #define MIKROE_NT3H211_FD_PIN                         0
// [GPIO_MIKROE_NT3H211_FD]$

// <<< sl:end pin_tool >>>

#endif // MIKROE_NT3H211_CONFIG_H_
