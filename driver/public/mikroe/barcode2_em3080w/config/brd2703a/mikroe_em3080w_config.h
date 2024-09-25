/***************************************************************************//**
 * @file mikroe_em3080w_config.h
 * @brief Micro EM3080W Configuration
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef MIKROE_EM3080W_CONFIG_H_
#define MIKROE_EM3080W_CONFIG_H_

#include "em_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

// <<< sl:start pin_tool >>>

// <gpio> CONFIG_EM3080W_RST
// $[GPIO_CONFIG_EM3080W_RST]
#ifndef CONFIG_EM3080W_RST_PORT                 
#define CONFIG_EM3080W_RST_PORT                  gpioPortC
#endif
#ifndef CONFIG_EM3080W_RST_PIN                  
#define CONFIG_EM3080W_RST_PIN                   8
#endif
// [GPIO_CONFIG_EM3080W_RST]$

// <gpio> CONFIG_EM3080W_TRG
// $[GPIO_CONFIG_EM3080W_TRG]
#ifndef CONFIG_EM3080W_TRG_PORT                 
#define CONFIG_EM3080W_TRG_PORT                  gpioPortA
#endif
#ifndef CONFIG_EM3080W_TRG_PIN                  
#define CONFIG_EM3080W_TRG_PIN                   0
#endif
// [GPIO_CONFIG_EM3080W_TRG]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_EM3080W_CONFIG_H_ */
