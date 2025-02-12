/***************************************************************************//**
 * @file epd_config.h
 * @brief E-Paper Display Configuration
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

#ifndef EPD_CONFIG_H_
#define EPD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define EPD_TYPE                    0x580B
#define EPD_VERTICAL                256
#define EPD_HORIZONTAL              720
#define EPD_DIAGONAL                581

// <<< Use Configuration Wizard in Context Menu >>>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio> EPD_DC
// $[GPIO_EPD_DC]
#ifndef EPD_DC_PORT
#define EPD_DC_PORT                              HP
#endif
#ifndef EPD_DC_PIN
#define EPD_DC_PIN                               10
#endif
// [GPIO_EPD_DC]$

// <gpio> EPD_RST
// $[GPIO_EPD_RST]
#ifndef EPD_RST_PORT
#define EPD_RST_PORT                             HP
#endif
#ifndef EPD_RST_PIN
#define EPD_RST_PIN                              12
#endif
// [GPIO_EPD_RST]$

// <gpio> EPD_BUSY
// $[GPIO_EPD_BUSY]
#ifndef EPD_BUSY_PORT
#define EPD_BUSY_PORT                            HP
#endif
#ifndef EPD_BUSY_PIN
#define EPD_BUSY_PIN                             11
#endif
// [GPIO_EPD_BUSY]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // EPD_CONFIG_H_
