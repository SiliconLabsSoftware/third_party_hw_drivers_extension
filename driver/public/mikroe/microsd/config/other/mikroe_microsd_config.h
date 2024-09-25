/***************************************************************************//**
 * @file mikroe_microsd_config.h
 * @brief SCL microsd config file
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
#ifndef MIKROE_MICROSD_CONFIG_H_
#define MIKROE_MICROSD_CONFIG_H_

#ifndef SLI_SI917
#include "em_gpio.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>
// <h> SDcard clock settings

// <o MIKROE_MICROSD_MMC_SLOW_CLOCK> Set slow clock for card initialization <100000-400000>
// <i>
// <i> Default: 200000
#define MIKROE_MICROSD_MMC_SLOW_CLOCK         200000

// <o MIKROE_MICROSD_MMC_FAST_CLOCK> Set fast clock for card
// <i>
// <i> If set to 0, the default bitrate of SPI is used
// <i> Default: 0
#define MIKROE_MICROSD_MMC_FAST_CLOCK         0
// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MIKROE_MICROSD_MMC_CD
// $[GPIO_MIKROE_MICROSD_MMC_CD]
#warning "CD pin for SD card is not configured"
// #define MIKROE_MICROSD_MMC_CD_PORT            0
// #define MIKROE_MICROSD_MMC_CD_PIN             0
// [GPIO_MIKROE_MICROSD_MMC_CD]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // MIKROE_MICROSD_CONFIG_H_