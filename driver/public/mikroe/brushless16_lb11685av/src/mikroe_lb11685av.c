/***************************************************************************//**
 * @file mikroe_lb11685av.c
 * @brief Mikroe lb11685 Source File
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

#include "mikroe_lb11685av_config.h"
#include "mikroe_lb11685av.h"
#include "brushless16.h"

static brushless16_t brushless16;
static brushless16_cfg_t brushless16_cfg;

/**************************************************************************//**
*  Brushless 16 initialization.
******************************************************************************/
sl_status_t mikroe_lb11685av_init(void)
{
  brushless16_cfg_setup(&brushless16_cfg);

#if defined(BRUSHLESS16_EN_PORT) && defined(BRUSHLESS16_EN_PIN)
  brushless16_cfg.en = hal_gpio_pin_name(BRUSHLESS16_EN_PORT,
                                         BRUSHLESS16_EN_PIN);
#endif

#if defined(BRUSHLESS16_FG_PORT) && defined(BRUSHLESS16_FG_PIN)
  brushless16_cfg.fg = hal_gpio_pin_name(BRUSHLESS16_FG_PORT,
                                         BRUSHLESS16_FG_PIN);
#endif

#if defined(BRUSHLESS16_RD_PORT) && defined(BRUSHLESS16_RD_PIN)
  brushless16_cfg.rd = hal_gpio_pin_name(BRUSHLESS16_RD_PORT,
                                         BRUSHLESS16_RD_PIN);
#endif

  if (brushless16_init(&brushless16, &brushless16_cfg) != BRUSHLESS16_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

/**************************************************************************//**
*  Set en pin state.
******************************************************************************/
void mikroe_lb11685av_set_en(uint8_t state)
{
  brushless16_set_en(&brushless16, state);
}

/**************************************************************************//**
*  Get rd pin state.
******************************************************************************/
uint8_t mikroe_lb11685av_get_rd(void)
{
  return brushless16_get_rd(&brushless16);
}

/**************************************************************************//**
*  Get fg pin state.
******************************************************************************/
uint8_t mikroe_lb11685av_get_fg(void)
{
  return brushless16_get_fg(&brushless16);
}
