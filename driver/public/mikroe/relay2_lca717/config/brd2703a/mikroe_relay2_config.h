/***************************************************************************//**
 * @file mikroe_relay2_config.h
 * @brief SCL Relay2 Configuration
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

#ifndef SCL_RELAY2_CONFIG_H_
#define SCL_RELAY2_CONFIG_H_

#include "em_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

// <<< sl:start pin_tool >>>

// <gpio optional=true> RELAY2_RL1
// $[GPIO_RELAY2_RL1]
#define RELAY2_RL1_PORT                        gpioPortA
#define RELAY2_RL1_PIN                         0
// [GPIO_RELAY2_RL1]$

// <gpio optional=true> RELAY2_RL2
// $[GPIO_RELAY2_RL2]
#define RELAY2_RL2_PORT                        gpioPortB
#define RELAY2_RL2_PIN                         0
// [GPIO_RELAY2_RL2]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif /* SCL_RELAY2_CONFIG_H_ */
