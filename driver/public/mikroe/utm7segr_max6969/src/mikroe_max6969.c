/***************************************************************************//**
 * @file mikroe_max6969.c
 * @brief SCL max6969 Source File
 * @version 1.0.0
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
 * # Experimental Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#include "utm7segr.h"
#include "mikroe_max6969.h"
#include "utm7segr_max6969_config.h"

static utm7segr_t utm7segr;
static utm7segr_cfg_t utm7segr_cfg;
static pwm_t utm7segr_pwm_instance;

sl_status_t mikroe_max6969_init(mikroe_spi_handle_t spi_instance,
                                mikroe_pwm_handle_t pwm_instance)
{
  sl_status_t stt = SL_STATUS_OK;

  if ((NULL == spi_instance) || (NULL == pwm_instance)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // update instance for pwm
  utm7segr_pwm_instance.handle = pwm_instance;

  // Configure default spi instance
  utm7segr.spi.handle = spi_instance;

  // Call basic setup functions
  utm7segr_cfg_setup(&utm7segr_cfg);

#if (MIKROE_MAX6969_SPI_UC == 1)
  utm7segr_cfg.spi_speed = MIKROE_MAX6969_SPI_BITRATE;
#endif

#if defined(MIKROE_MAX6969_CS_PORT) && defined(MIKROE_MAX6969_CS_PIN)
  utm7segr_cfg.cs = hal_gpio_pin_name(MIKROE_MAX6969_CS_PORT,
                                      MIKROE_MAX6969_CS_PIN);
  // CS pin need to init here since the mikroe_sdk_v2 missed this step
  digital_out_t struct_cs;
  digital_out_init(&struct_cs, utm7segr_cfg.cs);
#endif

  if (utm7segr_init(&utm7segr, &utm7segr_cfg) != UTM7SEGR_OK) {
    stt = SL_STATUS_INITIALIZATION;
  }

  pwm_config_t cfg_pwm;
  pwm_configure_default(&cfg_pwm);

  pwm_open(&utm7segr_pwm_instance, &cfg_pwm);
  pwm_set_freq(&utm7segr_pwm_instance, MIKROE_UTM7SEGR_PWM_FREQUENCY);

  // Start pwm
  pwm_start(&utm7segr_pwm_instance);

  // Set the light intensity of LED 7-segment to 50%
  mikroe_max6969_set_contrast(50);

  return stt;
}

sl_status_t mikroe_max6969_set_spi_instance(mikroe_spi_handle_t spi_instance)
{
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  utm7segr.spi.handle = spi_instance;

  return SL_STATUS_OK;
}

sl_status_t mikroe_max6969_set_pwm_instance(mikroe_pwm_handle_t pwm_instance)
{
  if (NULL == pwm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // update instance for pwm
  utm7segr_pwm_instance.handle = pwm_instance;

  return SL_STATUS_OK;
}

void mikroe_max6969_set_contrast(uint8_t percent)
{
  // Set duty cycle
  pwm_set_duty(&utm7segr_pwm_instance, (float)(percent / 100.0));
}

sl_status_t mikroe_max6969_generic_write(uint8_t *data_in)
{
  sl_status_t ret;
  if (data_in == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  ret = utm7segr_generic_write(&utm7segr, data_in);

  return ret;
}

void mikroe_max6969_display_state(uint8_t state)
{
  utm7segr_display_state(&utm7segr, state);
}

sl_status_t mikroe_max6969_display_number(uint8_t number, uint8_t dot_pos)
{
  sl_status_t ret;

  ret = utm7segr_display_number(&utm7segr, number, dot_pos);

  return ret;
}
