/***************************************************************************//**
 * @file mikroe_relay2.c
 * @brief SCL RELAY2 Source File
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

#include <stdbool.h>
#include "mikroe_relay2.h"
#include "mikroe_relay2_config.h"

static relay2_t relay2;
static relay2_cfg_t relay2_cfg;

sl_status_t mikroe_relay2_init(void)
{
  relay2_cfg_setup(&relay2_cfg);

#if defined(RELAY2_RL1_PORT) && defined(RELAY2_RL1_PIN)
  relay2_cfg.an = hal_gpio_pin_name(RELAY2_RL1_PORT, RELAY2_RL1_PIN);
#endif

#if defined(RELAY2_RL2_PORT) && defined(RELAY2_RL2_PIN)
  relay2_cfg.pwm = hal_gpio_pin_name(RELAY2_RL2_PORT, RELAY2_RL2_PIN);
#endif

  if (relay2_init(&relay2, &relay2_cfg) != RELAY2_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

void mikroe_relay2_relay1_control(uint8_t pin_state)
{
  relay2_relay1_control(&relay2, pin_state);
}

void mikroe_relay2_relay2_control(uint8_t pin_state)
{
  relay2_relay2_control(&relay2, pin_state);
}
