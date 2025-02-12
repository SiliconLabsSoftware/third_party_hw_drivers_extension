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
#include "sl_gpio.h"

static err_t drv_digital_in_init(digital_in_t *in,
                                 pin_name_t name,
                                 sl_gpio_mode_t mode,
                                 bool output_value);

err_t digital_in_init(digital_in_t *in, pin_name_t name)
{
  return drv_digital_in_init(in, name, SL_GPIO_MODE_INPUT, 0);
}

err_t digital_in_pullup_init(digital_in_t *in, pin_name_t name)
{
  return drv_digital_in_init(in, name, SL_GPIO_MODE_INPUT_PULL, 1);
}

err_t digital_in_pulldown_init(digital_in_t *in, pin_name_t name)
{
  return drv_digital_in_init(in, name, SL_GPIO_MODE_INPUT_PULL, 0);
}

uint8_t digital_in_read(digital_in_t *in)
{
  bool pin_value;

  if (sl_gpio_get_pin_input((const sl_gpio_t *)in,
                            &pin_value) == SL_STATUS_OK) {
    return pin_value;
  }
  return 0;
}

static err_t drv_digital_in_init(digital_in_t *in,
                                 pin_name_t name,
                                 sl_gpio_mode_t mode,
                                 bool output_value)
{
  if (HAL_PIN_NC == name) {
    return DIGITAL_IN_UNSUPPORTED_PIN;
  }

  in->pin.base = hal_gpio_port_index(name);
  in->pin.mask = hal_gpio_pin_index(name);

  if (sl_gpio_set_pin_mode((const sl_gpio_t *)in,
                           mode,
                           output_value) != SL_STATUS_OK) {
    in->pin.base = (uint8_t) -1;
    in->pin.mask = 0;
    return DIGITAL_IN_UNSUPPORTED_PIN;
  }

  return DIGITAL_IN_SUCCESS;
}

// ------------------------------------------------------------------------- END
