/***************************************************************************//**
 * @file
 * @brief Top level application functions
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
#include "app_assert.h"
#include "sl_sleeptimer.h"
#include "mikroe_lb11685av.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#else
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

void app_init(void)
{
  sl_status_t ret;

  ret = mikroe_lb11685av_init();
  app_assert(ret == SL_STATUS_OK, "\rMikroe LB11685AV init fail\n");
  app_printf("\rMikroe LB11685AV init done");

  sl_sleeptimer_delay_millisecond(500);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static uint16_t timer = 5000;
  static uint8_t state = 1;

  if (mikroe_lb11685av_get_rd()) {
    app_printf("\rMotor lock\n");
    sl_sleeptimer_delay_millisecond(500);
  }
  if (mikroe_lb11685av_get_fg()) {
    app_printf("\rFG\n");
    sl_sleeptimer_delay_millisecond(500);
  }
  if (!(timer--)) {
    timer = 5000;

    if (state) {
      app_printf("\rMotor stop\n");
    } else {
      app_printf("\rMotor rotating\n");
    }
    mikroe_lb11685av_set_en(state);
    state = !state;
  }
  sl_sleeptimer_delay_millisecond(1);
}
