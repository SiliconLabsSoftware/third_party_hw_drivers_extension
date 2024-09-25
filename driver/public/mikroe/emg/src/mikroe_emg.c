/***************************************************************************//**
 * @file mirkroe_emg
 * @brief SCL EMG Click Source code file.
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
#include "emg.h"
#include "mikroe_emg.h"
#include "mikroe_emg_config.h"

static emg_t emg;
static emg_cfg_t emg_cfg;

sl_status_t mikroe_emg_init(mikroe_adc_handle_t handle)
{
  if (NULL == handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  emg.adc.handle = handle;
  emg_cfg_setup(&emg_cfg);

#if defined(EMG_ANALOG_OUTPUT_PORT) && defined(EMG_ANALOG_OUTPUT_PIN)
  emg_cfg.an = hal_gpio_pin_name(EMG_ANALOG_OUTPUT_PORT,
                                 EMG_ANALOG_OUTPUT_PIN);
#endif

  if (emg_init(&emg, &emg_cfg) != EMG_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_emg_read_an_pin_value(uint16_t *data_out)
{
  err_t status;

  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  status = emg_read_an_pin_value(&emg, data_out);
  if (status != ADC_SUCCESS) {
    *data_out = 0;
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

sl_status_t mikroe_emg_read_an_pin_voltage(float *data_out)
{
  err_t status;

  if (NULL == data_out) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  status = emg_read_an_pin_voltage(&emg, data_out);
  if (status != ADC_SUCCESS) {
    *data_out = 0;
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}
