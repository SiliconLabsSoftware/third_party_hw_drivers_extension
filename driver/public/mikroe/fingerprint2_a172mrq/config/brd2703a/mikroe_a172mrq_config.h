/***************************************************************************//**
 * @file mikroe_a172mrq_config.h
 * @brief SCL A172MRQ Configuration
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SCL_A172MRQ_CONFIG_H_
#define SCL_A172MRQ_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< sl:start pin_tool >>>

// <gpio optional=true> A172MRQ_RESET
// $[GPIO_A172MRQ_RESET]
#define A172MRQ_RESET_PORT       SL_GPIO_PORT_C
#define A172MRQ_RESET_PIN        8
// [GPIO_A172MRQ_RESET]$

// <gpio optional=true> A172MRQ_LD1
// $[GPIO_A172MRQ_LD1]
#define A172MRQ_LD1_PORT         SL_GPIO_PORT_B
#define A172MRQ_LD1_PIN          0
// [GPIO_A172MRQ_LD1]$

// <gpio optional=true> A172MRQ_LD2
// $[GPIO_A172MRQ_LD2]
#define A172MRQ_LD2_PORT         SL_GPIO_PORT_B
#define A172MRQ_LD2_PIN          1
// [GPIO_A172MRQ_LD2]$

// <gpio optional=true> A172MRQ_GP1
// $[GPIO_A172MRQ_GP1]
#define A172MRQ_GP1_PORT         SL_GPIO_PORT_C
#define A172MRQ_GP1_PIN          0
// [GPIO_A172MRQ_GP1]$

// <gpio optional=true> A172MRQ_GP2
// $[GPIO_A172MRQ_GP2]
#define A172MRQ_GP2_PORT         SL_GPIO_PORT_A
#define A172MRQ_GP2_PIN          0
// [GPIO_A172MRQ_GP2]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // SCL_A172MRQ_CONFIG_H_
