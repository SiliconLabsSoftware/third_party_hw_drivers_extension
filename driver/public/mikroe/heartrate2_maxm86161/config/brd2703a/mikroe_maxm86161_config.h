/***************************************************************************//**
 * @file mikroe_maxm86161_config.h
 * @brief SCL MAXM86161 Configuration
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

#ifndef SCL_MAXM86161_CONFIG_H_
#define SCL_MAXM86161_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< sl:start pin_tool >>>

// <gpio optional=true> MAXM86161_GP
// $[GPIO_MAXM86161_GP]
#define MAXM86161_GP_PORT                        SL_GPIO_PORT_B
#define MAXM86161_GP_PIN                         0
// [GPIO_MAXM86161_GP]$

// <gpio optional=true> MAXM86161_EN
// $[GPIO_MAXM86161_EN]
#define MAXM86161_EN_PORT                        SL_GPIO_PORT_C
#define MAXM86161_EN_PIN                         0
// [GPIO_MAXM86161_EN]$

// <gpio optional=true> MAXM86161_INT
// $[GPIO_MAXM86161_INT]
#define MAXM86161_INT_PORT                       SL_GPIO_PORT_B
#define MAXM86161_INT_PIN                        1
// [GPIO_MAXM86161_INT]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // SCL_MAXM86161_CONFIG_H_
