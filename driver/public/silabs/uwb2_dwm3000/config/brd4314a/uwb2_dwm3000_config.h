/***************************************************************************//**
 * @file  mikroe_uwb2_dwm3000_config.h
 * @brief DWM3000 UWB config.
 * @version 0.0.1
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef DWM3000_CONFIG_H_
#define DWM3000_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu
// <h> SPI Driver settings

// <o DWM3000_FREQ_MIN> SPI FREQUENCY MIN (Hz)
// <d> 2000000
#define DWM3000_FREQ_MIN 2000000

// <o DWM3000_FREQ_MAX> SPI FREQUENCY MAX (Hz)
// <d> 19500000
#define DWM3000_FREQ_MAX 19500000

// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <usart signal=TX,RX,CLK,(CS)> DWM3000
// $[USART_DWM3000]
#define DWM3000_PERIPHERAL              USART0
#define DWM3000_PERIPHERAL_NO           0

// USART0 TX on PC00
#define DWM3000_TX_PORT                 SL_GPIO_PORT_C
#define DWM3000_TX_PIN                  0

// USART0 RX on PC01
#define DWM3000_RX_PORT                 SL_GPIO_PORT_C
#define DWM3000_RX_PIN                  1

// USART0 CLK on PC02
#define DWM3000_CLK_PORT                SL_GPIO_PORT_C
#define DWM3000_CLK_PIN                 2

// USART0 CS on PC03
#define DWM3000_CS_PORT                 SL_GPIO_PORT_C
#define DWM3000_CS_PIN                  3

// [USART_DWM3000]$

// <gpio optional=true> DWM3000_RESET
// $[GPIO_DWM3000_RESET]
#ifndef DWM3000_RESET_PORT
#define DWM3000_RESET_PORT                       SL_GPIO_PORT_C
#endif
#ifndef DWM3000_RESET_PIN
#define DWM3000_RESET_PIN                        6
#endif
// [GPIO_DWM3000_RESET]$

// <gpio optional=true> DWM3000_INT
// $[GPIO_DWM3000_INT]
#ifndef DWM3000_INT_PORT
#define DWM3000_INT_PORT                         SL_GPIO_PORT_B
#endif
#ifndef DWM3000_INT_PIN
#define DWM3000_INT_PIN                          3
#endif
// [GPIO_DWM3000_INT]$

// <gpio optional=true> DWM3000_WAKE
// $[GPIO_DWM3000_WAKE]
// #ifndef DWM3000_WAKE_PORT
// #define DWM3000_WAKE_PORT                        SL_GPIO_PORT_B
// #endif
// #ifndef DWM3000_WAKE_PIN
// #define DWM3000_WAKE_PIN                         0
// #endif
// [GPIO_DWM3000_WAKE]$

// <gpio optional=true> DWM3000_ON
// $[GPIO_DWM3000_ON]
// #ifndef DWM3000_ON_PORT
// #define DWM3000_ON_PORT                          SL_GPIO_PORT_B
// #endif
// #ifndef DWM3000_ON_PIN
// #define DWM3000_ON_PIN                           4
// #endif
// [GPIO_DWM3000_ON]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // DWM3000_CONFIG_H_
