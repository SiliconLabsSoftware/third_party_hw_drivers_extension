/***************************************************************************//**
 * @file epd_config.h
 * @brief E-Paper Display Configuration
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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

// A CMSIS annotation block starts with the following line:
// <<< Use Configuration Wizard in Context Menu >>>
// <h> SPIDRV settings

// <o SPI_EPD_BITRATE> SPI bitrate
// <i> Default: 1000000
#define SPI_EPD_BITRATE           8000000

// <o SPI_EPD_FRAME_LENGTH> SPI frame length <4-16>
// <i> Default: 8
#define SPI_EPD_FRAME_LENGTH      8

// <o SPI_EPD_TYPE> SPI mode
// <spidrvMaster=> Master
// <spidrvSlave=> Slave
#define SPI_EPD_TYPE              spidrvMaster

// <o SPI_EPD_BIT_ORDER> Bit order on the SPI bus
// <spidrvBitOrderLsbFirst=> LSB transmitted first
// <spidrvBitOrderMsbFirst=> MSB transmitted first
#define SPI_EPD_BIT_ORDER         spidrvBitOrderMsbFirst

// <o SPI_EPD_CLOCK_MODE> SPI clock mode
// <spidrvClockMode0=> SPI mode 0: CLKPOL=0, CLKPHA=0
// <spidrvClockMode1=> SPI mode 1: CLKPOL=0, CLKPHA=1
// <spidrvClockMode2=> SPI mode 2: CLKPOL=1, CLKPHA=0
// <spidrvClockMode3=> SPI mode 3: CLKPOL=1, CLKPHA=1
#define SPI_EPD_CLOCK_MODE        spidrvClockMode0

// <o SPI_EPD_CS_CONTROL> SPI master chip select (CS) control scheme.
// <spidrvCsControlAuto=> CS controlled by the SPI driver
// <spidrvCsControlApplication=> CS controlled by the application
#define SPI_EPD_CS_CONTROL        spidrvCsControlAuto

// <o SPI_EPD_SLAVE_START_MODE> SPI slave transfer start scheme
// <spidrvSlaveStartImmediate=> Transfer starts immediately
// <spidrvSlaveStartDelayed=> Transfer starts when the bus is idle
// <i> Only applies if instance type is spidrvSlave
#define SPI_EPD_SLAVE_START_MODE  spidrvSlaveStartImmediate
// </h>
// The block ends with the following line or at the end of the file:
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>
// <usart signal=TX,RX,CLK,(CS)> SPI_EPD
// $[USART_SPI_EPD]
#warning "E_PAPER_154_INCH_DC is not configured"
// #ifndef SPI_EPD_PERIPHERAL
// #define SPI_EPD_PERIPHERAL              USART0
// #endif
// #ifndef SPI_EPD_PERIPHERAL_NO
// #define SPI_EPD_PERIPHERAL_NO           0
// #endif

// <gpio optional=true> SPI_EPD_TX
// $[SPI_EPD_TX]
#warning "SPI_EPD_TX is not configured"
// #ifndef SPI_EPD_TX_PORT
// #define SPI_EPD_TX_PORT                 SL_GPIO_PORT_C
// #endif
// #ifndef SPI_EPD_TX_PIN
// #define SPI_EPD_TX_PIN                  0
// #endif

// <gpio optional=true> SPI_EPD_RX
// $[SPI_EPD_RX]
#warning "SPI_EPD_RX is not configured"
// #ifndef SPI_EPD_RX_PORT
// #define SPI_EPD_RX_PORT                 SL_GPIO_PORT_C
// #endif
// #ifndef SPI_EPD_RX_PIN
// #define SPI_EPD_RX_PIN                  1
// #endif

// <gpio optional=true> SPI_EPD_CLK
// $[SPI_EPD_CLK]
#warning "SPI_EPD_CLK is not configured"
// #ifndef SPI_EPD_CLK_PORT
// #define SPI_EPD_CLK_PORT                SL_GPIO_PORT_C
// #endif
// #ifndef SPI_EPD_CLK_PIN
// #define SPI_EPD_CLK_PIN                 2
// #endif

// <gpio optional=true> SPI_EPD_CS
// $[SPI_EPD_CS]
#warning "SPI_EPD_CS is not configured"
// #ifndef SPI_EPD_CS_PORT
// #define SPI_EPD_CS_PORT                 SL_GPIO_PORT_C
// #endif
// #ifndef SPI_EPD_CS_PIN
// #define SPI_EPD_CS_PIN                  3
// #endif
// [USART_SPI_EPD]$

// <gpio optional=true> EPD_DC
// $[GPIO_EPD_DC]
#warning "EPD_DC is not configured"
// #ifndef EPD_DC_PORT
// #define EPD_DC_PORT                     SL_GPIO_PORT_B
// #endif
// #ifndef EPD_DC_PIN
// #define EPD_DC_PIN                      4
// #endif
// [GPIO_EPD_DC]$

// <gpio optional=true> EPD_RST
// $[GPIO_EPD_RST]
#warning "EPD_RST is not configured"
// #ifndef EPD_RST_PORT
// #define EPD_RST_PORT                    SL_GPIO_PORT_B
// #endif
// #ifndef EPD_RST_PIN
// #define EPD_RST_PIN                     2
// #endif
// [GPIO_EPD_RST]$

// <gpio optional=true> EPD_BUSY
// $[GPIO_EPD_BUSY]
#warning "EPD_BUSY is not configured"
// #ifndef EPD_BUSY_PORT
// #define EPD_BUSY_PORT                   SL_GPIO_PORT_B
// #endif
// #ifndef EPD_BUSY_PIN
// #define EPD_BUSY_PIN                    3
// #endif
// [GPIO_EPD_BUSY]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // EPD_CONFIG_H_
