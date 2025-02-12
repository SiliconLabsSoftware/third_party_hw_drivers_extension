/***************************************************************************//**
 * @file hal_gpio.h
 * @brief mikroSDK 2.0 Click Peripheral Drivers
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

#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#include <stdint.h>

#ifndef SLI_SI917
// Defines the macros for GPIO
#include "sl_device_gpio.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_PIN_NC                           0xffffffff
#define HAL_PORT_NC                          0xffffffff

#define PORT_SIZE                            (16)

#ifndef HP
#define HP                                   0
#endif

#ifndef ULP
#define ULP                                  4
#endif

#ifndef UULP_VBAT
#define UULP_VBAT                            5
#endif

#define hal_gpio_pin_index(pin_name) \
  ((pin_name) % PORT_SIZE)

#define hal_gpio_port_index(pin_name) \
  ((pin_name) / PORT_SIZE)

#define hal_gpio_pin_mask(pin_name) \
  (0x01 << gpio_pin_index(pin_name))

#define hal_gpio_pin_name(port_index, pin_index) \
  ((pin_name_t)(port_index) * PORT_SIZE + (pin_index))

#define hal_gpio_port_name(port_index) \
  ((port_name_t)(port_index) * PORT_SIZE)

typedef struct {
  uint8_t base;
  uint8_t mask;
} hal_gpio_t;

typedef hal_gpio_t hal_gpio_port_t;

#ifdef __cplusplus
}
#endif

#endif // _HAL_TARGET_H_
// ------------------------------------------------------------------------- END
