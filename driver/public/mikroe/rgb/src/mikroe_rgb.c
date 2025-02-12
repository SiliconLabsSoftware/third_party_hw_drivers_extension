/***************************************************************************/ /**
 * @file mikroe_rgbdriver.c
 * @brief mikroe_rgbdriver.c Source File
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

#include "mikroe_rgb.h"

static rgbdriver_t rgbdriver;
static rgbdriver_cfg_t rgbdriver_cfg;

sl_status_t mikroe_rgbdriver_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (NULL == i2cspm_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  rgbdriver.i2c.handle = i2cspm_instance;

  rgbdriver_cfg_setup(&rgbdriver_cfg);

  if (RGBDRIVER_OK != rgbdriver_init(&rgbdriver, &rgbdriver_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }

  if (RGBDRIVER_OK != rgbdriver_default_cfg(&rgbdriver)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

sl_status_t mikroe_rgbdriver_generic_write(uint8_t *tx_buf, uint8_t tx_len)
{
  sl_status_t stt = SL_STATUS_FAIL;

  if ((NULL != tx_buf)) {
    rgbdriver_generic_write(&rgbdriver, tx_buf, tx_len);
    stt = SL_STATUS_OK;
  }
  return stt;
}

void mikroe_rgbdriver_set_rgb_color(uint8_t red, uint8_t green, uint8_t blue)
{
  blue = ((uint16_t)blue * (0x5f - 0x40) / 255) + 0x40;
  green = ((uint16_t)green * (0x9f - 0x80) / 255) + 0x80;
  red = ((uint16_t)red * (0x7f - 0x60) / 255) + 0x60;

  rgbdriver_set_rgb_color(&rgbdriver, red, green, blue);
}

void mikroe_rgbdriver_set_color(uint32_t color)
{
  rgbdriver_set_color(&rgbdriver, color);
}

void mikroe_rgbdriver_shut_down(void)
{
  rgbdriver_shut_down(&rgbdriver);
}
