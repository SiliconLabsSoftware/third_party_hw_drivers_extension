/***************************************************************************//**
 * @file sparkfun_ccs811_config.c
 * @brief CCS811 Driver Configuration
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SPARKFUN_CCS811_CONFIG_H
#define SPARKFUN_CCS811_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> SPARKFUN CCS811 I2C Configuration

// <o SPARKFUN_CCS811_ADDR> SPARKFUN CCS811 Adress
// <0x5B=>    0x5B
// <0x5A=>    0x5A
// <i> Default: 0x5B
#define SPARKFUN_CCS811_ADDR                 0x5B

// <e> SPARKFUN CCS811 I2C UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define SPARKFUN_CCS811_I2C_UC               0

// <o SPARKFUN_CCS811_I2C_SPEED_MODE> Speed mode
// <0=> Standard mode (100kbit/s)
// <1=> Fast mode (400kbit/s)
// <2=> Fast mode plus (1Mbit/s)
// <i> Default: 0
#define SPARKFUN_CCS811_I2C_SPEED_MODE       0

// </e>
// </h>

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio> SPARKFUN_CCS811_WAKE
// $[GPIO_SPARKFUN_CCS811_WAKE]
#warning "WAKE pin for CCS811 is not configured"
// #define SPARKFUN_CCS811_WAKE_PORT            0
// #define SPARKFUN_CCS811_WAKE_PIN             0
// [GPIO_SPARKFUN_CCS811_WAKE]$

// <<< sl:end pin_tool >>>

#endif // SPARKFUN_CCS811_CONFIG_H
