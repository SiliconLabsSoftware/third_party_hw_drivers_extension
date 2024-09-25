/***************************************************************************//**
 * @file mipi_dbi_spi_4wire.h
 * @brief Adafruit ST7789 Color TFT SPI interface header file
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
#ifndef GPIO_HELPER_SI91X
#define GPIO_HELPER_SI91X

// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------
#include "sl_si91x_peripheral_gpio.h"
#include "rsi_egpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define si91x_get_port(num) ((num) / 16)
#define si91x_get_pin(num)  ((num) % 16)

static inline uint8_t si91x_gpio_get_pad(uint16_t gpio_num)
{
  if ((gpio_num >= 6) && (gpio_num <= 12)) {
    return gpio_num - 5;
  } else if (gpio_num == 15) {
    return 8;
  } else if ((gpio_num == 31)
             || (gpio_num == 32)
             || (gpio_num == 33)
             || (gpio_num == 34)) {
    return 9;
  } else if ((gpio_num >= 46) && (gpio_num <= 57)) {
    return gpio_num - 36;
  } else {
    return 0;
  }
}

static inline sl_status_t si91x_gpio_setup(uint16_t gpio_num,
                                           sl_gpio_mode_t mode,
                                           sl_si91x_gpio_direction_t direction,
                                           uint32_t output_value)
{
  sl_gpio_port_t port = si91x_get_port(gpio_num);
  uint8_t pin = si91x_get_pin(gpio_num);

  if (!SL_GPIO_VALIDATE_PORT(port)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (port == SL_ULP_GPIO_PORT) {
    if (!SL_GPIO_VALIDATE_ULP_PORT_PIN(port, pin)) {
      return SL_STATUS_INVALID_PARAMETER;
    }
  } else {
    if (!SL_GPIO_NDEBUG_PORT_PIN(port, pin)) {
      return SL_STATUS_INVALID_PARAMETER;
    }
  }

  if (port == SL_ULP_GPIO_PORT) {
    sl_si91x_gpio_enable_clock(ULPCLK_GPIO);
    sl_si91x_gpio_enable_ulp_pad_receiver(pin);
  } else {
    sl_si91x_gpio_enable_clock(M4CLK_GPIO);
    sl_si91x_gpio_enable_pad_receiver(gpio_num);
    uint8_t pad = si91x_gpio_get_pad(gpio_num);
    if (pad) {
      sl_si91x_gpio_enable_pad_selection(pad);
    }
  }

  sl_gpio_set_pin_mode(port, pin, mode, output_value);
  sl_si91x_gpio_set_pin_direction(port, pin, direction);
  return SL_STATUS_OK;
}

static inline sl_status_t si91x_ulp_gpio_pin_setup(
  uint16_t ulp_pin,
  sl_gpio_mode_t mode,
  uint8_t ulp_soc_mode,
  sl_si91x_gpio_direction_t direction,
  uint32_t output_value)
{
  if (!SL_GPIO_VALIDATE_ULP_PIN(ulp_pin)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  sl_si91x_gpio_enable_clock(ULPCLK_GPIO);
  sl_si91x_gpio_enable_ulp_pad_receiver(ulp_pin);

  sl_gpio_set_pin_mode(SL_ULP_GPIO_PORT,
                       ulp_pin,
                       mode,
                       output_value);
  sl_si91x_gpio_ulp_soc_mode(ulp_pin, ulp_soc_mode);
  sl_si91x_gpio_set_pin_direction(SL_ULP_GPIO_PORT,
                                  ulp_pin,
                                  direction);
  return SL_STATUS_OK;
}

#ifdef __cplusplus
}
#endif

#endif /* GPIO_HELPER_SI91X */
