/***************************************************************************//**
 * @file mikroe_mic2_config.h
 * @brief Mikroe MIC 2 Click Configuration
 * @version 1.2.0
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

#ifndef MIKORE_MIC2_CONFIG_H_
#define MIKORE_MIC2_CONFIG_H_

#ifndef SLI_SI917
#include "em_gpio.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> MIKROE MIC2 I2C Configuration

// <e> MIKROE MIC2 I2C UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_MIC2_I2C_UC                  0

// <o MIKROE_MIC2_I2C_SPEED_MODE> Speed mode
// <0=> Standard mode (100kbit/s)
// <1=> Fast mode (400kbit/s)
// <2=> Fast mode plus (1Mbit/s)
// <i> Default: 0
#define MIKROE_MIC2_I2C_SPEED_MODE          0

// </e>
// </h>

// <o MIC2_I2C_ADDRESS> MIC 2 I2C ADDRESS
// <0x2C=> AD0_LOW
// <0x2D=> AD0_HIGH
// <i> Default: 0x2C
#define MIC2_I2C_ADDRESS                          0x2C

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MIC2_ANALOG_OUTPUT
// $[GPIO_MIC2_ANALOG_OUTPUT]
#warning "AN pin for MIC 2 is not configured"
// #define MIC2_ANALOG_OUTPUT_PORT                   gpioPortB
// #define MIC2_ANALOG_OUTPUT_PIN                    0
// [GPIO_MIC2_ANALOG_OUTPUT]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}

#endif
#endif // MIKORE_MIC2_CONFIG_H_