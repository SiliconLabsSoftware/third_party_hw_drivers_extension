
/***************************************************************************/ /**
 * @file mikroe ph2 config.h
 * @brief mikroe ph2 configuration
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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

#ifndef MIKROE_PH2_CONFIG_H
#define MIKROE_PH2_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> MIKROE PH2 I2C Configuration

// <e> MIKROE PH2 I2C UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_PH2_I2C_UC                  0

// <o MIKROE_PH2_I2C_SPEED_MODE> Speed mode
// <0=> Standard mode (100kbit/s)
// <1=> Fast mode (400kbit/s)
// <2=> Fast mode plus (1Mbit/s)
// <i> Default: 0
#define MIKROE_PH2_I2C_SPEED_MODE          0

// </e>
// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MIKROE_PH2_ST1
#warning "MIKROE_PH2_ST1 is not configured"
// $[GPIO_MIKROE_PH2_ST1]
// #ifndef MIKROE_PH2_ST1_PORT
// #define MIKROE_PH2_ST1_PORT                         0
// #endif
// #ifndef MIKROE_PH2_ST1_PIN
// #define MIKROE_PH2_ST1_PIN                          0
// #endif
// [GPIO_MIKROE_PH2_ST1]$

// <gpio optional=true> MIKROE_PH2_ST2
#warning "MIKROE_PH2_ST2 is not configured"
// $[GPIO_MIKROE_PH2_ST2]
// #ifndef MIKROE_PH2_ST2_PORT
// #define MIKROE_PH2_ST2_PORT                         0
// #endif
// #ifndef MIKROE_PH2_ST2_PIN
// #define MIKROE_PH2_ST2_PIN                          0
// #endif
// [GPIO_MIKROE_PH2_ST2]$

// <gpio optional=true> MIKROE_DS18B20_DQ
#warning "MIKROE_DS18B20_DQ is not configured"
// $[GPIO_MIKROE_DS18B20_DQ]
// #ifndef MIKROE_DS18B20_DQ_PORT
// #define MIKROE_DS18B20_DQ_PORT                      0
// #endif
// #ifndef MIKROE_DS18B20_DQ_PIN
// #define MIKROE_DS18B20_DQ_PIN                       0
// #endif
// [GPIO_MIKROE_DS18B20_DQ]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // MIKROE_PH2_CONFIG_H