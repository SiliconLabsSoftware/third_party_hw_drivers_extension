/***************************************************************************//**
 * @file sparkfun_type5_config.h
 * @brief Configuration file for Sparkfun dosimeter type5.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#ifndef SPARKFUN_TYPE5_CONFIG_H_
#define SPARKFUN_TYPE5_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> SPARKFUN TYPE5 configuration

// <o SPARKFUN_TYPE5_PROCESS_PERIOD> Process period [MS]
// <i> Default: 160
#define SPARKFUN_TYPE5_PROCESS_PERIOD      160

// <o SPARKFUN_TYPE5_HISTORY_LENGTH> History length
// <i> Default: 200
#define SPARKFUN_TYPE5_HISTORY_LENGTH      200

// <o SPARKFUN_TYPE5_HISTORY_UNIT> History unit
// <i> Default: 6
#define SPARKFUN_TYPE5_HISTORY_UNIT        6

// </h> end SPARKFUN TYPE5 configuration

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> SPARKFUN_TYPE5_SIG
// $[GPIO_SPARKFUN_TYPE5_SIG]
#define SPARKFUN_TYPE5_SIG_PORT                  SL_GPIO_PORT_B
#define SPARKFUN_TYPE5_SIG_PIN                   1
// [GPIO_SPARKFUN_TYPE5_SIG]$

// <gpio optional=true> SPARKFUN_TYPE5_NS
// $[GPIO_SPARKFUN_TYPE5_NS]
#define SPARKFUN_TYPE5_NS_PORT                   SL_GPIO_PORT_D
#define SPARKFUN_TYPE5_NS_PIN                    5
// [GPIO_SPARKFUN_TYPE5_NS]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // SPARKFUN_TYPE5_CONFIG_H_
