/***************************************************************************//**
 * @file adafruit_hxd8357d_spi_config.h
 * @brief Configuration file for Adafruit TFT Display Driver.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#ifndef ADAFRUIT_HXD8357D_CONFIG_H_
#define ADAFRUIT_HXD8357D_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>
// <h> ST7789 SPI settings

// <o ADAFRUIT_HXD8357D_BITRATE> ST7789 SPI bitrate
// <i> Default: 20000000
#define ADAFRUIT_HXD8357D_BITRATE                      20000000

// <o ADAFRUIT_HXD8357D_CLOCK_MODE> SPI clock mode
// <SL_GSPI_MODE_0=> SPI mode 0: CLKPOL=0, CLKPHA=0
// <SL_GSPI_MODE_3=> SPI mode 3: CLKPOL=1, CLKPHA=1
#define ADAFRUIT_HXD8357D_CLOCK_MODE                   SL_GSPI_MODE_0

// <o ADAFRUIT_HXD8357D_CS_CONTROL> SPI master chip select (CS) control scheme.
// <SL_GSPI_MASTER_SW=> CS controlled by the SPI driver
// <SL_GSPI_MASTER_HW_OUTPUT=> CS controlled by the application
#define ADAFRUIT_HXD8357D_CS_CONTROL                   SL_GSPI_MASTER_HW_OUTPUT
// </h>
// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio> ADAFRUIT_HXD8357D_DC
// $[GPIO_ADAFRUIT_HXD8357D_DC]
#ifndef ADAFRUIT_HXD8357D_DC_PORT
#define ADAFRUIT_HXD8357D_DC_PORT                HP
#endif
#ifndef ADAFRUIT_HXD8357D_DC_PIN
#define ADAFRUIT_HXD8357D_DC_PIN                 10
#endif
// [GPIO_ADAFRUIT_HXD8357D_DC]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // ADAFRUIT_HXD8357D_CONFIG_H_
