/***************************************************************************//**
 * @file mikroe_mq7.c
 * @brief SCL MQ7 Source File
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

#include <stddef.h>
#include "sl_status.h"
#include "co.h"
#include "mikroe_mq7_config.h"

static co_t co;
static co_cfg_t co_cfg;

sl_status_t mikroe_mq7_init(mikroe_adc_handle_t handle)
{
  if (NULL == handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  co.adc.handle = handle;
  // Call basic setup functions
  co_cfg_setup(&co_cfg);

#if defined(MQ7_AN_PORT) && defined(MQ7_AN_PIN)
  co_cfg.an = hal_gpio_pin_name(MQ7_AN_PORT, MQ7_AN_PIN);
#endif

  if (co_init(&co, &co_cfg) != CO_OK) {
    return SL_STATUS_INITIALIZATION;
  }
  return SL_STATUS_OK;
}

sl_status_t mikroe_mq7_set_adc_instance(mikroe_adc_handle_t handle)
{
  if (NULL == handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  co.adc.handle = handle;

  return SL_STATUS_OK;
}

sl_status_t mikroe_mq7_read_an_pin_value(uint16_t *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (co_read_an_pin_value(&co, data_out) != ADC_SUCCESS) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_mq7_read_an_pin_voltage(float *data_out)
{
  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (co_read_an_pin_voltage(&co, data_out) != ADC_SUCCESS) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}
