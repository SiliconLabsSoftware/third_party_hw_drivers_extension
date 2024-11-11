/***************************************************************************//**
 * @file mikroe_e_paper_config.h
 * @brief Mikroe E-Paper Configuration
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
#ifndef MIKROE_E_PAPER_154_INCH_CONFIG_H_
#define MIKROE_E_PAPER_154_INCH_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define MIKROE_E_PAPER_WIDTH                   200
#define MIKROE_E_PAPER_HEIGHT                  200

// A CMSIS annotation block starts with the following line:
// <<< Use Configuration Wizard in Context Menu >>>

// <h> ESL MODE setting
// <e> ESL_MODE> Enable Peripheral
// <i> Macro for the ESL mode
#define ESL_MODE 0
// </e>
// </h>

// The block ends with the following line or at the end of the file:
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> E_PAPER_DC
// $[GPIO_E_PAPER_DC]
#ifndef E_PAPER_DC_PORT                         
#define E_PAPER_DC_PORT                          gpioPortD
#endif
#ifndef E_PAPER_DC_PIN                          
#define E_PAPER_DC_PIN                           2
#endif
// [GPIO_E_PAPER_DC]$

// <gpio optional=true> E_PAPER_RST
// $[GPIO_E_PAPER_RST]
#ifndef E_PAPER_RST_PORT                        
#define E_PAPER_RST_PORT                         gpioPortD
#endif
#ifndef E_PAPER_RST_PIN                         
#define E_PAPER_RST_PIN                          3
#endif
// [GPIO_E_PAPER_RST]$

// <gpio optional=true> E_PAPER_BSY
// $[GPIO_E_PAPER_BSY]
#ifndef E_PAPER_BSY_PORT                        
#define E_PAPER_BSY_PORT                         gpioPortB
#endif
#ifndef E_PAPER_BSY_PIN                         
#define E_PAPER_BSY_PIN                          1
#endif
// [GPIO_E_PAPER_BSY]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_E_PAPER_154_INCH_CONFIG_H_ */
