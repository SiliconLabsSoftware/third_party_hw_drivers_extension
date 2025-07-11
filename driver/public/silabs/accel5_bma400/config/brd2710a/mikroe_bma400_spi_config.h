/***************************************************************************//**
 * @file mikroe_bma400_spi_config.h
 * @brief Configuration file for BMA400
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

#ifndef MIKROE_BMA400_CONFIG_H_
#define MIKROE_BMA400_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> MIKROE BMA400 SPI Configuration

// <e> MIKROE BMA400 SPI UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_BMA400_SPI_UC                  0

// <o MIKROE_BMA400_SPI_BITRATE> Bit Rate (Bits/Second) <1-116000000>
// <i> Default: 10000000
#define MIKROE_BMA400_SPI_BITRATE             10000000

// </e>
// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MIKROE_BMA400_INT1
// $[GPIO_MIKROE_BMA400_INT1]
#define MIKROE_BMA400_INT1_PORT                  SL_GPIO_PORT_B
#define MIKROE_BMA400_INT1_PIN                   4
// [GPIO_MIKROE_BMA400_INT1]$

// <gpio optional=true> MIKROE_BMA400_INT2
// $[GPIO_MIKROE_BMA400_INT2]
#define MIKROE_BMA400_INT2_PORT                  SL_GPIO_PORT_B
#define MIKROE_BMA400_INT2_PIN                   3
// [GPIO_MIKROE_BMA400_INT2]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // MIKROE_BMA400_CONFIG_H_
