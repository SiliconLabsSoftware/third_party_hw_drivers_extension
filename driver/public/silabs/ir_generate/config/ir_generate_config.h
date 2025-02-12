/***************************************************************************//**
* @file ir_generate_config.h
* @brief IR Generate Driver Configuration
********************************************************************************
* # License
* <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
********************************************************************************
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
*******************************************************************************/
#ifndef IR_GENERATE_CONFIG_H_
#define IR_GENERATE_CONFIG_H_

// <<< sl:start pin_tool >>>

// <gpio> IR_GENERATE_BSP_CARRIER
// $[GPIO_IR_GENERATE_BSP_CARRIER]
#define IR_GENERATE_BSP_CARRIER_PORT                 SL_GPIO_PORT_D
#define IR_GENERATE_BSP_CARRIER_PIN                  2
// [GPIO_IR_GENERATE_BSP_CARRIER]$

// <gpio> IR_GENERATE_BSP_MODULATION
// $[GPIO_IR_GENERATE_BSP_MODULATION]
#define IR_GENERATE_BSP_MODULATION_PORT              SL_GPIO_PORT_D
#define IR_GENERATE_BSP_MODULATION_PIN               3
// [GPIO_IR_GENERATE_BSP_MODULATION]$

// <<< sl:end pin_tool >>>

#endif // IR_GENERATE_CONFIG_H_
