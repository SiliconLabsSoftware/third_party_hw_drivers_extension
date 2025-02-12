/***************************************************************************//**
 * @file mikroe_water_detect.c
 * @brief SCL Water Detect Source File
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

#include "mikroe_water_detect_config.h"
#include "mikroe_water_detect.h"

static waterdetect_t waterdetect;
static waterdetect_cfg_t waterdetect_cfg;

// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

sl_status_t mikroe_water_detect_init(void)
{
  waterdetect_cfg_setup(&waterdetect_cfg);
#if defined(WATER_DETECT_INT_PORT) && defined(WATER_DETECT_INT_PIN)
  waterdetect_cfg.int_pin = hal_gpio_pin_name(WATER_DETECT_INT_PORT,
                                              WATER_DETECT_INT_PIN);
#endif

  if (waterdetect_init(&waterdetect, &waterdetect_cfg) != WATERDETECT_OK) {
    return SL_STATUS_INITIALIZATION;
  }
  return SL_STATUS_OK;
}

uint8_t mikroe_water_detect_get_status(void)
{
  return waterdetect_get_status(&waterdetect);
}

// ------------------------------------------------------------------------- END
