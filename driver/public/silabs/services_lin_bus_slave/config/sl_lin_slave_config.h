/***************************************************************************//**
 * @file sl_sdc_platform_spi.h
 * @brief Storage Device Controls SD Card platform include header
 *******************************************************************************
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
 ******************************************************************************/
#ifndef SL_LIN_SLAVE_CONFIG_H
#define SL_LIN_SLAVE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> DMA Channel configuration
// <o LIN_DMA_CH_USART_TX> USART Tx DMA Channel <0-7>
// <i> Default: 4
#define LIN_DMA_CH_USART_TX            4

// <o LIN_DMA_CH_USART_TX> USART Rx DMA Channel <0-7>
// <i> Default: 5
#define LIN_DMA_CH_USART_RX            5
// </h> end DMA Channel configuration

// <h> TIMER configuration
// <o LIN_DELAY_TIMER_NUM> Select timer for microsecond delay
// <0=> TIMER0
// <1=> TIMER1
// <2=> TIMER2
// <3=> TIMER3
// <4=> TIMER4
// <i> Select timer for microsecond delay.
// <i> Default: 3
#define LIN_DELAY_TIMER_NUM  3
// </h> end TIMER configuration

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <usart signal=TX,RX> LIN
// $[USART_LIN]
#define LIN_PERIPHERAL                            USART1
#define LIN_PERIPHERAL_NO                         1

// USART1 TX on PB02
#define LIN_TX_PORT                               SL_GPIO_PORT_B
#define LIN_TX_PIN                                2

// USART1 RX on PB03
#define LIN_RX_PORT                               SL_GPIO_PORT_B
#define LIN_RX_PIN                                3
// [USART_LIN]$

// <gpio optional=true> LIN_DMA_ACT
// $[GPIO_LIN_DMA_ACT]
#define LIN_DMA_ACT_PORT                          SL_GPIO_PORT_C
#define LIN_DMA_ACT_PIN                           6
// [GPIO_LIN_DMA_ACT]$

// <gpio optional=true> LIN_TIMEOUT_ACT
// $[GPIO_LIN_TIMEOUT_ACT]
#define LIN_TIMEOUT_ACT_PORT                      SL_GPIO_PORT_A
#define LIN_TIMEOUT_ACT_PIN                       8
// [GPIO_LIN_TIMEOUT_ACT]$

// <gpio optional=true> LIN_UART_ACT
// $[GPIO_LIN_UART_ACT]
#define LIN_UART_ACT_PORT                         SL_GPIO_PORT_A
#define LIN_UART_ACT_PIN                          0
// [GPIO_LIN_UART_ACT]$

// <gpio optional=true> LIN_WAKEUP_ACT
// $[GPIO_LIN_WAKEUP_ACT]
#define LIN_WAKEUP_ACT_PORT                       SL_GPIO_PORT_A
#define LIN_WAKEUP_ACT_PIN                        7
// [GPIO_LIN_WAKEUP_ACT]$

// <gpio optional=true> LIN_CHECKSUM_ERR
// $[GPIO_LIN_CHECKSUM_ERR]
#define LIN_CHECKSUM_ERR_PORT                     SL_GPIO_PORT_C
#define LIN_CHECKSUM_ERR_PIN                      1
// [GPIO_LIN_CHECKSUM_ERR]$

// <gpio optional=true> LIN_CONFLICT_ERR
// $[GPIO_LIN_CONFLICT_ERR]
#define LIN_CONFLICT_ERR_PORT                     SL_GPIO_PORT_C
#define LIN_CONFLICT_ERR_PIN                      3
// [GPIO_LIN_CONFLICT_ERR]$

// <gpio optional=true> LIN_GENERIC_ERR
// $[GPIO_LIN_GENERIC_ERR]
#define LIN_GENERIC_ERR_PORT                      SL_GPIO_PORT_A
#define LIN_GENERIC_ERR_PIN                       5
// [GPIO_LIN_GENERIC_ERR]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // SL_LIN_SLAVE_CONFIG_H
