/***************************************************************************//**
 * @file drv_digital_in.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - Digital IN
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

#include "drv_digital_in.h"
#include "em_gpio.h"

static err_t drv_digital_in_init(digital_in_t *in,
                                 pin_name_t name,
                                 GPIO_Mode_TypeDef mode,
                                 unsigned int out);

err_t digital_in_init(digital_in_t *in, pin_name_t name)
{
  return drv_digital_in_init(in, name, gpioModeInput, 0);
}

err_t digital_in_pullup_init(digital_in_t *in, pin_name_t name)
{
  return drv_digital_in_init(in, name, gpioModeInputPull, 1);
}

err_t digital_in_pulldown_init(digital_in_t *in, pin_name_t name)
{
  return drv_digital_in_init(in, name, gpioModeInputPull, 0);
}

uint8_t digital_in_read(digital_in_t *in)
{
  if (GPIO_PORT_VALID(in->pin.base)) {
    return (uint8_t) ((GPIO_PortInGet(in->pin.base) & in->pin.mask) != 0);
  }
  return 0;
}

static err_t drv_digital_in_init(digital_in_t *in,
                                 pin_name_t name,
                                 GPIO_Mode_TypeDef mode,
                                 unsigned int out)
{
  GPIO_Port_TypeDef port_index;
  unsigned int pin_index;

  in->pin.base = (uint32_t) -1;
  in->pin.mask = 0;
  if (HAL_PIN_NC == name) {
    return DIGITAL_IN_UNSUPPORTED_PIN;
  }

  port_index = (GPIO_Port_TypeDef) hal_gpio_port_index(name);
  pin_index = hal_gpio_pin_index(name);

  if (!GPIO_PORT_PIN_VALID(port_index, pin_index)) {
    return DIGITAL_IN_UNSUPPORTED_PIN;
  }
  GPIO_PinModeSet(port_index, pin_index, mode, out);
  in->pin.base = port_index;
  in->pin.mask = 1 << pin_index;
  return DIGITAL_IN_SUCCESS;
}

// ------------------------------------------------------------------------- END
