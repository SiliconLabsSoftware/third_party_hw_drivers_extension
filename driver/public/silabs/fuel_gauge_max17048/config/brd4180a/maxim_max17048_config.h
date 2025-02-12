/***************************************************************************//**
* @file max17048_config.h
* @brief MAX17048 Driver Configuration
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
#ifndef MAXIM_FUEL_GAUGE_MAX17048_CONFIG_H_
#define MAXIM_FUEL_GAUGE_MAX17048_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> MAX17048 I2C Configuration

// <e> MAX17048 I2C UC Configuration
// <i> Enable: Peripheral configuration is taken straight from the configuration set in the universal configuration (UC).
// <i> Disable: If the application demands it to be modified during runtime, use the default API to modify the peripheral configuration.
// <i> Default: 0
#define MAX17048_I2C_UC                         0

// <o MAX17048_I2C_SPEED_MODE> Speed mode
// <0=> Standard mode (100kbit/s)
// <1=> Fast mode (400kbit/s)
// <2=> Fast mode plus (1Mbit/s)
// <i> Default: 0
#define MAX17048_I2C_SPEED_MODE                 0

// <o MAX17048_I2C_ADDRESS> I2C Address <0x1..0x7E:0x1><f.h>
// <i> Default: 0x36
#define MAX17048_I2C_ADDRESS                    0x36

// </e>
// </h>

// <h> User-Settings

// <o MAX17048_STABILIZATION_DELAY> Stabilization delay (ms)
// <i> Default: 0
#define MAX17048_STABILIZATION_DELAY            0

// <o MAX17048_RCOMP_UPDATE_INTERVAL_MS> Rcomp update interval (ms)
// <i> Default: 60000
// <i> The driver calculates and updates the RCOMP factor at a rate of
// <i> 1000 ms <= MAX17048_RCOMP_UPDATE_INTERVAL_MS <= 60000 ms
// <i> and defaults to 1 minute (60000 ms = 1 minute).
#define MAX17048_RCOMP_UPDATE_INTERVAL_MS       60000

// <q MAX17048_ENABLE_HW_QSTRT> Enable battery quick start
// <i> Default: 1
#define MAX17048_ENABLE_HW_QSTRT                1

// <q MAX17048_ENABLE_POWER_MANAGER> Enable power manager
// <i> Default: 0
#define MAX17048_ENABLE_POWER_MANAGER           0

// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio optional=true> MAX17048_ALRT
// $[GPIO_MAX17048_ALRT]
#define MAX17048_ALRT_PORT                      SL_GPIO_PORT_C
#define MAX17048_ALRT_PIN                       0
// [GPIO_MAX17048_ALRT]$

// <gpio> MAX17048_ENABLE_QSTRT
// $[GPIO_MAX17048_ENABLE_QSTRT]
#define MAX17048_ENABLE_QSTRT_PORT              SL_GPIO_PORT_C
#define MAX17048_ENABLE_QSTRT_PIN               2
// [GPIO_MAX17048_ENABLE_QSTRT]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // MAXIM_FUEL_GAUGE_MAX17048_CONFIG_H_
