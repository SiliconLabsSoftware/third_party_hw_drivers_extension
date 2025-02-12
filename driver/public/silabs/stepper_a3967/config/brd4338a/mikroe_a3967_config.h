/***************************************************************************//**
 * @file mikroe_a3967_config.h
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
#ifndef MIKROE_A3967_CONFIG_H_
#define MIKROE_A3967_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu

// <o MIKROE_A3967_STEP_FREQUENCY> Stepper Motor Step Frequency
// <i> Default: 500
#define MIKROE_A3967_STEP_FREQUENCY             500
// </h> end Stepper Motor config

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MIKROE_A3967_STEP
// $[GPIO_MIKROE_A3967_STEP]
#define MIKROE_A3967_STEP_PORT                  HP
#define MIKROE_A3967_STEP_PIN                   46
// [GPIO_MIKROE_A3967_STEP]$

// <gpio optional=true> MIKROE_A3967_DIR
// $[GPIO_MIKROE_A3967_DIR]
#define MIKROE_A3967_DIR_PORT                   HP
#define MIKROE_A3967_DIR_PIN                    47
// [GPIO_MIKROE_A3967_DIR]$

// <gpio optional=true> MIKROE_A3967_MS1
// $[GPIO_MIKROE_A3967_MS1]
#define MIKROE_A3967_MS1_PORT                   HP
#define MIKROE_A3967_MS1_PIN                    48
// [GPIO_MIKROE_A3967_MS1]$

// <gpio optional=true> MIKROE_A3967_MS2
// $[GPIO_MIKROE_A3967_MS2]
#define MIKROE_A3967_MS2_PORT                   HP
#define MIKROE_A3967_MS2_PIN                    49
// [GPIO_MIKROE_A3967_MS2]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // MIKROE_A3967_CONFIG_H_
