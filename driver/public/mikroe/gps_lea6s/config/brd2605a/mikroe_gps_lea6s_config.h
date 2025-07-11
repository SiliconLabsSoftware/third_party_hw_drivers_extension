/***************************************************************************//**
 * @file mikroe_gps_lea6s_config.h
 * @brief mikroe_gps_lea6s_config.h
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
#ifndef MIKROE_GPS_LEA6S_CONFIG_H_
#define MIKROE_GPS_LEA6S_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< sl:start pin_tool >>>

// <gpio optional=true> MIKROE_LEAS6_RESET
// $[GPIO_MIKROE_LEAS6_RESET]
#define MIKROE_LEAS6_RESET_PORT                              HP
#define MIKROE_LEAS6_RESET_PIN                               10
// [GPIO_MIKROE_LEAS6_RESET]$

// <gpio optional=true> MIKROE_LEAS6_TIMEPULSE
// $[GPIO_MIKROE_LEAS6_TIMEPULSE]
#define MIKROE_LEAS6_TIMEPULSE_PORT                          HP
#define MIKROE_LEAS6_TIMEPULSE_PIN                           11
// [GPIO_MIKROE_LEAS6_TIMEPULSE]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // MIKROE_GPS_LEA6S_CONFIG_H_
