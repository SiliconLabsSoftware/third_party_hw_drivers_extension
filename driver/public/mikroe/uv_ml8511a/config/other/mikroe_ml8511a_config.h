/***************************************************************************//**
 * @file mikroe_ml8511a_config.h
 * @brief SCL ML8511A Configuration File
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
#ifndef MIKROE_ML8511A_CONFIG_H_
#define MIKROE_ML8511A_CONFIG_H_

#ifndef SLI_SI917
#include "em_gpio.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ADC_MODE                                 0
#define ANALOG_MODE                              1

// <<< Use Configuration Wizard in Context Menu >>>

// <h> MIKROE_ML8511A SPI Configuration

// <e>MIKROE_ML8511A SPI UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_ML8511A_SPI_UC                    0

// <o MIKROE_ML8511A_SPI_BITRATE> Bit Rate (Bits/Second) <1-116000000>
// <i> Default: 10000000
#define MIKROE_ML8511A_SPI_BITRATE               10000000

// </e>
// </h>

//<h>MIKROE ML8511A Configuration
// <o MIKROE_ML8511A_OPERATION_MODE> Operation mode
// <AN_MODE=> Analog mode
// <ADC_MODE=> ADC mode
// <i> Default: AN_MODE
#define MIKROE_ML8511A_OPERATION_MODE            AN_MODE

// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> ML8511A_CS
// $[GPIO_ML8511A_CS]
// #define ML8511A_CS_PORT                       0
// #define ML8511A_CS_PIN                        0
// [GPIO_ML8511A_CS]$

// <gpio optional=true> ML8511A_AN
#warning "GPIO_ML8511A_AN is not configured"
// $[GPIO_ML8511A_AN]
// #define ML8511A_AN_PORT                       gpioPortB
// #define ML8511A_AN_PIN                        0
// [GPIO_ML8511A_AN]$

// <gpio optional=true> ML8511A_EN
// $[GPIO_ML8511A_EN]
#warning "GPIO_ML8511A_EN is not configured"
// #define ML8511A_EN_PORT                       gpioPortC
// #define ML8511A_EN_PIN                        6
// [GPIO_ML8511A_EN]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_ML8511A_CONFIG_H_ */
