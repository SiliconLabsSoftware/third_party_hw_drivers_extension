/***************************************************************************//**
 * @file mikroe_stretch.c
 * @brief SCL mikroe stretch Prototypes
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#include <stddef.h>
#include "mikroe_stretch.h"
#include "mikroe_stretch_config.h"

static stretch_t stretch;
static stretch_cfg_t stretch_cfg;

sl_status_t mikroe_stretch_init(mikroe_adc_handle_t handle)
{
  if (NULL == handle) {
    return SL_STATUS_INVALID_HANDLE;
  }
  stretch.adc.handle = handle;
  stretch_cfg_setup(&stretch_cfg);

#if defined(STRETCH_ANALOG_OUTPUT_PORT) && defined(STRETCH_ANALOG_OUTPUT_PIN)
  stretch_cfg.an_pin = hal_gpio_pin_name(STRETCH_ANALOG_OUTPUT_PORT,
                                         STRETCH_ANALOG_OUTPUT_PIN);
#endif

#if defined(STRETCH_LED_PORT) && defined(STRETCH_LED_PIN)
  stretch_cfg.pwm = hal_gpio_pin_name(STRETCH_LED_PORT,
                                      STRETCH_LED_PIN);
#endif

  if (stretch_init(&stretch, &stretch_cfg) != STRETCH_OK) {
    return SL_STATUS_INITIALIZATION;
  }
  return SL_STATUS_OK;
}

sl_status_t mikroe_stretch_generic_read(uint16_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  *data_out = stretch_generic_read(&stretch);

  return SL_STATUS_OK;
}

void mikroe_stretch_turn_on_led(void)
{
  stretch_turn_on_led(&stretch);
}

void mikroe_stretch_turn_off_led(void)
{
  stretch_turn_off_led(&stretch);
}
