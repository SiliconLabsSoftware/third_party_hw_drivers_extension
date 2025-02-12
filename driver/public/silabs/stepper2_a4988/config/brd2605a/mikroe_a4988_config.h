/***************************************************************************//**
 * @file mikroe_a4988_config.h
 * @brief Stepper motor driver configuration.
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
#ifndef MIKROE_A4988_CONFIG_H_
#define MIKROE_A4988_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu

// <h> Stepper Motor settings
// <o MIKROE_A4988_STEP_FREQUENCY> Stepper Motor Step Frequency
// <i> Default: 300
#define MIKROE_A4988_STEP_FREQUENCY             300
// </h> end Stepper Motor config

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MIKROE_A4988_STEP
// $[GPIO_MIKROE_A4988_STEP]
#ifndef MIKROE_A4988_STEP_PORT
#define MIKROE_A4988_STEP_PORT                  HP
#endif
#ifndef MIKROE_A4988_STEP_PIN
#define MIKROE_A4988_STEP_PIN                   12
#endif
// [GPIO_MIKROE_A4988_STEP]$

// <gpio optional=true> MIKROE_A4988_DIR
// $[GPIO_MIKROE_A4988_DIR]
#ifndef MIKROE_A4988_DIR_PORT
#define MIKROE_A4988_DIR_PORT                   HP
#endif
#ifndef MIKROE_A4988_DIR_PIN
#define MIKROE_A4988_DIR_PIN                    11
#endif
// [GPIO_MIKROE_A4988_DIR]$

// <gpio optional=true> MIKROE_A4988_ENABLE
// $[GPIO_MIKROE_A4988_ENABLE]
#ifndef MIKROE_A4988_ENABLE_PORT
#define MIKROE_A4988_ENABLE_PORT                HP
#endif
#ifndef MIKROE_A4988_ENABLE_PIN
#define MIKROE_A4988_ENABLE_PIN                 7
#endif
// [GPIO_MIKROE_A4988_ENABLE]$

// <gpio optional=true> MIKROE_A4988_RST
// $[GPIO_MIKROE_A4988_RST]
#ifndef MIKROE_A4988_RST_PORT
#define MIKROE_A4988_RST_PORT                   HP
#endif
#ifndef MIKROE_A4988_RST_PIN
#define MIKROE_A4988_RST_PIN                    6
#endif
// [GPIO_MIKROE_A4988_RST]$

// <gpio optional=true> MIKROE_A4988_SLEEP
// $[GPIO_MIKROE_A4988_SLEEP]
#ifndef MIKROE_A4988_SLEEP_PORT
#define MIKROE_A4988_SLEEP_PORT                 HP
#endif
#ifndef MIKROE_A4988_SLEEP_PIN
#define MIKROE_A4988_SLEEP_PIN                  10
#endif
// [GPIO_MIKROE_A4988_SLEEP]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // MIKROE_A4988_CONFIG_H_
