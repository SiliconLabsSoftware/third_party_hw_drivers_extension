/***************************************************************************//**
 * @file mirkroe_mic2
 * @brief SCL MIC 2 Click Source code file.
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

#include "mikroe_mic2.h"
#include "mikroe_mic2_config.h"

static mic2_t mic2;
static mic2_cfg_t mic2_cfg;

sl_status_t mikroe_mic2_init(mikroe_i2c_handle_t i2c_instance,
                             mikroe_adc_handle_t adc)
{
  if ((NULL == i2c_instance) || (NULL == adc)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  mic2.i2c.handle = i2c_instance;
  mic2.slave_address = MIC2_I2C_ADDRESS;
  mic2.adc.handle = adc;

  mic2_cfg_setup(&mic2_cfg);

#if defined(MIC2_ANALOG_OUTPUT_PORT) && defined(MIC2_ANALOG_OUTPUT_PIN)
  mic2_cfg.an_pin = hal_gpio_pin_name(MIC2_ANALOG_OUTPUT_PORT,
                                      MIC2_ANALOG_OUTPUT_PIN);
#endif

#if (MIKROE_MIC2_I2C_UC == 1)
  mic2_cfg.i2c_speed = MIKROE_MIC2_I2C_SPEED_MODE;
#endif

  if (mic2_init(&mic2, &mic2_cfg) != MIC2_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

void mikroe_mic2_set_potentiometer(uint8_t ptt_value)
{
  mic2_set_potentiometer(&mic2, ptt_value);
}

sl_status_t mikroe_mic2_generic_read(mikroe_mic2_data_t *data)
{
  if (data == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *data = mic2_generic_read(&mic2);

  return SL_STATUS_OK;
}

sl_status_t mikroe_mic2_set_i2c_instance(mikroe_i2c_handle_t i2c_instance)
{
  if (NULL == i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  mic2.i2c.handle = i2c_instance;

  return SL_STATUS_OK;
}
