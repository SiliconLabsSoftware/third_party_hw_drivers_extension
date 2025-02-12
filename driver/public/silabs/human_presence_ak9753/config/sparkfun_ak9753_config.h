/***************************************************************************//**
 * @file sparkfun_ak9753_config.h
 * @brief AK9753 Configuration
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
 *    misrepresented as being the original software.s
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

#ifndef SPARKFUN_AK9753_CONFIG_H_
#define SPARKFUN_AK9753_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************************/
/* Address is changeable via CAD0/CAD1          */
/* Allowed settings are:                        */
/* 00 (0x64 default)                            */
/* 01 (0x65)                                    */
/* 10 (0x66)                                    */
/* 11 Not allowed - used for switch mode        */
/************************************************/

// <<< Use Configuration Wizard in Context Menu >>>

// <h> SPARKFUN AK9753 I2C Configuration

// <e> SPARKFUN AK9753 I2C UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define SPARKFUN_AK9753_I2C_UC                  0

// <o SPARKFUN_AK9753_I2C_SPEED_MODE> Speed mode
// <0=> Standard mode (100kbit/s)
// <1=> Fast mode (400kbit/s)
// <2=> Fast mode plus (1Mbit/s)
// <i> Default: 0
#define SPARKFUN_AK9753_I2C_SPEED_MODE          0

// </e>
// </h>

// <h>Address settings

// <o SPARKFUN_AK9753_ADDR> Address of AK9753 sensor.
// <0x64=> Address1 (0x64)
// <0x65=> Address2 (0x65)
// <0x66=> Address3 (0x66)
// <i> Default: 0x64
#define SPARKFUN_AK9753_ADDR             0x64

// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

/*
 * GPIO port/pin used for INT interrupt pin. This should typically
 * be a port A/B pin so that it can detect a falling edge while in
 * EM2/3. If the application firmware does not need to use EM2/3,
 * then a port C/D pin can be used.
 */
// <gpio optional=true> SPARKFUN_AK9753_CONFIG_INT
// $[GPIO_SPARKFUN_AK9753_CONFIG_INT]
// #define SPARKFUN_AK9753_CONFIG_INT_PORT   0
// #define SPARKFUN_AK9753_CONFIG_INT_PIN    0
// [GPIO_SPARKFUN_AK9753_CONFIG_INT]$

/*
 * GPIO port/pin used for PDN power down output pin.
 */
// <gpio optional=true> SPARKFUN_AK9753_CONFIG_PDN
// $[GPIO_SPARKFUN_AK9753_CONFIG_PDN]
// #define SPARKFUN_AK9753_CONFIG_PDN_PORT   0
// #define SPARKFUN_AK9753_CONFIG_PDN_PIN    0
// [GPIO_SPARKFUN_AK9753_CONFIG_PDN]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // SPARKFUN_AK9753_CONFIG_H_
