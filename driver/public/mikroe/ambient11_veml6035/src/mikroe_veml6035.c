/***************************************************************************/ /**
 * @file mikroe_veml6035.c
 * @brief veml6035 Source File
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "mikroe_veml6035.h"

static ambient11_t ambient11;
static ambient11_cfg_t ambient11_cfg;

sl_status_t mikroe_ambient11_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  ambient11.i2c.handle = i2cspm_instance;

  ambient11_cfg_setup(&ambient11_cfg);

  if (AMBIENT11_OK != ambient11_init(&ambient11, &ambient11_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }

  ambient11_default_cfg(&ambient11);

  return SL_STATUS_OK;
}

void mikroe_ambient11_generic_write(uint8_t wr_cmd, uint16_t wr_data)
{
  ambient11_generic_write(&ambient11, wr_cmd, wr_data);
}

uint16_t mikroe_ambient11_generic_read(uint8_t reg_addr)
{
  return ambient11_generic_read(&ambient11, reg_addr);
}

float mikroe_ambient11_calc_illumination(float typical_res)
{
  return ambient11_calc_illumination(&ambient11, typical_res);
}
