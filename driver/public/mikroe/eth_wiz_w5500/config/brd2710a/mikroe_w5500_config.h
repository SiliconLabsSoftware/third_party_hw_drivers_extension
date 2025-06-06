/***************************************************************************//**
 * @file w5x00_config.h
 * @brief Wiznet w5x00 config.
 * @version 0.0.1
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
#ifndef Wx500_CONFIG_H_
#define Wx500_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>
// <h> MIKROE_W5500 SPI Configuration

// <e> MIKROE_W5500 SPI UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_W5500_SPI_UC                  0

// <o MIKROE_W5500_SPI_BITRATE> Bit Rate (Bits/Second) <1-116000000>
// <i> Default: 10000000
#define MIKROE_W5500_SPI_BITRATE             10000000

// </e>
// </h>

// <q W5x00_ETHERNET_LARGE_BUFFERS_ENABLE> Use ethernet large buffer
// <i>
// <i> By default, each socket uses 2K buffers inside the Wiznet chip.
// <i> If W5x00_MAX_SOCK_NUM is set to fewer than the chip's maximum, uncommenting
// <i> this will use larger buffers within the Wiznet chip.  Large buffers
// <i> can really help with UDP protocols like Artnet.  In theory larger
// <i> buffers should allow faster TCP over high-latency links, but this
// <i> does not always seem to work in practice (maybe Wiznet bugs?)
// <i> Default: 0
#define W5x00_ETHERNET_LARGE_BUFFERS_ENABLE        0

// <q W5x00_HTTP_SERVER_DEBUG_ENABLE> HTTP Server debug message enable
// <i>
// <i> Default: 0
#define W5x00_HTTP_SERVER_DEBUG_ENABLE             0

// <o W5x00_HTTP_SERVER_BUFFER_SIZE> HTTP server buffer size configuration <1024-10240>
// <i> Default: 1024
#define W5x00_HTTP_SERVER_BUFFER_SIZE              1024

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> W5500_RESET
// $[GPIO_W5500_RESET]
#ifndef W5500_RESET_PORT
#define W5500_RESET_PORT                         SL_GPIO_PORT_C
#endif
#ifndef W5500_RESET_PIN
#define W5500_RESET_PIN                          6
#endif
// [GPIO_W5500_RESET]$

// <gpio optional=true> MIKROE_W5500_CS
// $[GPIO_MIKROE_W5500_CS]
#ifndef MIKROE_W5500_CS_PORT
#define MIKROE_W5500_CS_PORT                    SL_GPIO_PORT_C
#endif
#ifndef MIKROE_W5500_CS_PIN
#define MIKROE_W5500_CS_PIN                     3
#endif
// [GPIO_MIKROE_W5500_CS]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // Wx500_CONFIG_H_
