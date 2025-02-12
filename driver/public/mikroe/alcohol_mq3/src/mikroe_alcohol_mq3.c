/***************************************************************************//**
 * @file mikroe_alcohol_mq3.c
 * @brief SCL mikroe alcohol mq3 Prototypes
 * @version 1.2.0
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

#include <stddef.h>
#include "mikroe_alcohol_mq3.h"
#include "mikroe_alcohol_mq3_config.h"

static alcohol_t alcohol;
static alcohol_cfg_t alcohol_cfg;

void mikroe_mq3_setup(void)
{
  alcohol_cfg_setup(&alcohol_cfg);
}

sl_status_t mikroe_mq3_init(mikroe_adc_handle_t handle)
{
  if (NULL == handle) {
    return SL_STATUS_INVALID_HANDLE;
  }
  alcohol.adc.handle = handle;

  alcohol_cfg_setup(&alcohol_cfg);

#if defined(MQ3_ANALOG_OUTPUT_PORT) && defined(MQ3_ANALOG_OUTPUT_PIN)
  alcohol_cfg.an = hal_gpio_pin_name(MQ3_ANALOG_OUTPUT_PORT,
                                     MQ3_ANALOG_OUTPUT_PIN);
#endif

  if (alcohol_init(&alcohol, &alcohol_cfg) != ALCOHOL_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_mq3_read_an_pin_value(uint16_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (alcohol_read_an_pin_value(&alcohol, data_out) != ALCOHOL_OK) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_mq3_read_an_pin_voltage(float *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (alcohol_read_an_pin_voltage(&alcohol, data_out) != ALCOHOL_OK) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}
