/***************************************************************************//**
 * @file drv_digital_out.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - Digital OUT
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

#include "drv_digital_out.h"
#include "em_gpio.h"

err_t digital_out_init(digital_out_t *out, pin_name_t name)
{
  GPIO_Port_TypeDef port_index;
  unsigned int pin_index;

  out->pin.base = (uint32_t) -1;
  out->pin.mask = 0;
  if (HAL_PIN_NC == name) {
    return DIGITAL_OUT_UNSUPPORTED_PIN;
  }

  port_index = (GPIO_Port_TypeDef) hal_gpio_port_index(name);
  pin_index = hal_gpio_pin_index(name);
  if (!GPIO_PORT_PIN_VALID(port_index, pin_index)) {
    return DIGITAL_OUT_UNSUPPORTED_PIN;
  }
  GPIO_PinModeSet(port_index, pin_index, gpioModePushPull, 0);
  out->pin.base = port_index;
  out->pin.mask = 1 << pin_index;
  return DIGITAL_OUT_SUCCESS;
}

void digital_out_high(digital_out_t *out)
{
  if (GPIO_PORT_VALID(out->pin.base)) {
    GPIO_PortOutSetVal((GPIO_Port_TypeDef) out->pin.base, out->pin.mask,
                       out->pin.mask);
  }
}

void digital_out_low(digital_out_t *out)
{
  if (GPIO_PORT_VALID(out->pin.base)) {
    GPIO_PortOutSetVal((GPIO_Port_TypeDef) out->pin.base, 0, out->pin.mask);
  }
}

void digital_out_toggle(digital_out_t *out)
{
  if (GPIO_PORT_VALID(out->pin.base)) {
    GPIO_PortOutToggle((GPIO_Port_TypeDef) out->pin.base, out->pin.mask);
  }
}

void digital_out_write(digital_out_t *out, uint8_t value)
{
  if (GPIO_PORT_VALID(out->pin.base)) {
    if (value) {
      GPIO_PortOutSetVal((GPIO_Port_TypeDef) out->pin.base, out->pin.mask,
                         out->pin.mask);
    } else {
      GPIO_PortOutSetVal((GPIO_Port_TypeDef) out->pin.base, 0, out->pin.mask);
    }
  }
}

// ------------------------------------------------------------------------- END
