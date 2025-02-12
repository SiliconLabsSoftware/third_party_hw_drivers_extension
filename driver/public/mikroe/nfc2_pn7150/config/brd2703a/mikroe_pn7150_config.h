/***************************************************************************//**
 * @file mikroe_pn7150_config.h
 * @brief SCL MIKROE PN7150 Configuration
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

#ifndef MIKROE_PN7150_CONFIG_H_
#define MIKROE_PN7150_CONFIG_H_

// A CMSIS annotation block starts with the following line:
// <<< Use Configuration Wizard in Context Menu >>>

// <h> MIKROE PN7150 I2C Configuration

// <e> MIKROE PN7150 I2C UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_PN7150_I2C_UC                  0

// <o MIKROE_PN7150_I2C_SPEED_MODE> Speed mode
// <0=> Standard mode (100kbit/s)
// <1=> Fast mode (400kbit/s)
// <2=> Fast mode plus (1Mbit/s)
// <i> Default: 0
#define MIKROE_PN7150_I2C_SPEED_MODE          0

// </e>
// </h>

// <o MIKROE_PN7150_ADDR> PN7150 I2C ADDRESS
// <0x28=> I2C_ADDR_0
// <0x29=> I2C_ADDR_1
// <0x2A=> I2C_ADDR_2
// <0x2B=> I2C_ADDR_3
// <i> Default: 0x28
#define MIKROE_PN7150_ADDR                            0x28

// The block ends with the following line or at the end of the file:
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MIKROE_PN7150_INT
// $[GPIO_MIKROE_PN7150_INT]
#define MIKROE_PN7150_INT_PORT                        SL_GPIO_PORT_B
#define MIKROE_PN7150_INT_PIN                         1
// [GPIO_MIKROE_PN7150_INT]$

// <gpio optional=true> MIKROE_PN7150_RESET
// $[GPIO_MIKROE_PN7150_RESET]
#define MIKROE_PN7150_RESET_PORT                      SL_GPIO_PORT_C
#define MIKROE_PN7150_RESET_PIN                       8
// [GPIO_MIKROE_PN7150_RESET]$

// <<< sl:end pin_tool >>>

#endif // MIKROE_PN7150_CONFIG_H_
