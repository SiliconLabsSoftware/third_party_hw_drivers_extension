/***************************************************************************//**
 * @file mikroe_mm5d91_00_config.h
 * @brief SCL MM5D91_00 Configuration
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef MM5D91_00_CONFIG_H
#define MM5D91_00_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// <<< sl:start pin_tool >>>

// <gpio> CONFIG_MM5D91_00_RST
// $[GPIO_CONFIG_MM5D91_00_RST]
#define CONFIG_MM5D91_00_RST_PORT                  HP
#define CONFIG_MM5D91_00_RST_PIN                   10
// [GPIO_CONFIG_MM5D91_00_RST]$

// <gpio> CONFIG_MM5D91_00_GPIO0
// $[GPIO_CONFIG_MM5D91_00_GPIO0]
#define CONFIG_MM5D91_00_GPIO0_PORT                HP
#define CONFIG_MM5D91_00_GPIO0_PIN                 11
// [GPIO_CONFIG_MM5D91_00_GPIO0]$

// <gpio> CONFIG_MM5D91_00_GPIO1
// $[GPIO_CONFIG_MM5D91_00_GPIO1]
#define CONFIG_MM5D91_00_GPIO1_PORT                HP
#define CONFIG_MM5D91_00_GPIO1_PIN                 12
// [GPIO_CONFIG_MM5D91_00_GPIO1]$

// <gpio> CONFIG_MM5D91_00_GPIO2
// $[GPIO_CONFIG_MM5D91_00_GPIO2]
#define CONFIG_MM5D91_00_GPIO2_PORT                HP
#define CONFIG_MM5D91_00_GPIO2_PIN                 30
// [GPIO_CONFIG_MM5D91_00_GPIO2]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // MM5D91_00_CONFIG_H
