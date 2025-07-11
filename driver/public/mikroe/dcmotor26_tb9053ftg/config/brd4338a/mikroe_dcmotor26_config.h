/***************************************************************************//**
 * @file mikroe_dcmotor26_config.h
 * @brief Mikroe DC Motor 26 Click Configuration
 * @version 1.2.0
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

#ifndef MIKROE_DCMOTOR26_CONFIG_H_
#define MIKROE_DCMOTOR26_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> MIKROE DCMOTOR26 SPI Configuration
// <e> MIKROE DCMOTOR26 SPI UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_DCMOTOR26_SPI_UC                  0

// <o MIKROE_DCMOTOR26_SPI_BITRATE> Bit Rate (Bits/Second) <1-116000000>
// <i> Default: 10000000
#define MIKROE_DCMOTOR26_SPI_BITRATE             10000000

// </e>
// </h>

// <h> MIKROE DCMOTOR26 I2C Configuration
// <e> MIKROE DCMOTOR26 I2C UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MIKROE_DCMOTOR26_I2C_UC                  0

// <o MIKROE_DCMOTOR26_I2C_SPEED_MODE> Speed mode
// <0=> Standard mode (100kbit/s)
// <1=> Fast mode (400kbit/s)
// <2=> Fast mode plus (1Mbit/s)
// <i> Default: 0
#define MIKROE_DCMOTOR26_I2C_SPEED_MODE          0

// </e>
// </h>

// <h> I2C address
// <o DCMOTOR26_PCA9538A_ADDRESS> DCMOTOR26 PCA9538A ADDRESS
// <0=> None
// <0x70=> 0x70
// <0x71=> 0x71
// <0x72=> 0x72
// <0x73=> 0x73
// <i> Default: 0x70
#define DCMOTOR26_PCA9538A_ADDRESS                       0x70

// </h>

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> DCMOTOR26_CS
// $[GPIO_DCMOTOR26_CS]
#define DCMOTOR26_CS_PORT                                HP
#define DCMOTOR26_CS_PIN                                 49
// [GPIO_DCMOTOR26_CS]$

// <gpio optional=true> DCMOTOR26_SLEEP
// $[GPIO_DCMOTOR26_SLEEP]
#define DCMOTOR26_SLEEP_PORT                             HP
#define DCMOTOR26_SLEEP_PIN                              46
// [GPIO_DCMOTOR26_SLEEP]$

// <gpio optional=true> DCMOTOR26_INT
// $[GPIO_DCMOTOR26_INT]
#define DCMOTOR26_INT_PORT                               HP
#define DCMOTOR26_INT_PIN                                47
// [GPIO_DCMOTOR26_INT]$

// <gpio optional=true> DCMOTOR26_PWM
// $[GPIO_DCMOTOR26_PWM]
#define DCMOTOR26_PWM_PORT                               HP
#define DCMOTOR26_PWM_PIN                                48
// [GPIO_DCMOTOR26_PWM]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // MIKROE_DCMOTOR26_CONFIG_H_
