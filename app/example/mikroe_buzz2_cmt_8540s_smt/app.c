/***************************************************************************//**
 * @file app.c
 * @brief Example application
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "sl_sleeptimer.h"
#include "mikroe_cmt_8540s_smt.h"
#include "sl_pwm_instances.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"

#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#include "app_log.h"

#define app_printf(...) app_log(__VA_ARGS__)
#endif

mikroe_pwm_handle_t app_pwm_instance = NULL;

#define W                            4 * Q // Whole 4/4 - 4 Beats
#define H                            2 * Q // Half 2/4 - 2 Beats
#define Q                            250 // Quarter 1/4 - 1 Beat
#define E                            Q / 2 // Eighth 1/8 - 1/2 Beat
#define S                            Q / 4 // Sixteenth 1/16 - 1/4 Beat

#define VOLUME                       100 // goes up to 1000

static void imperial_march(void);

void app_init(void)
{
  app_printf("- Buzzer 2 Click board driver, example application. -\n");

#if (defined(SLI_SI917))
  app_pwm_instance = &sl_pwm_channel_0_config;
#else
  app_pwm_instance = &sl_pwm_mikroe;
#endif

  if (SL_STATUS_OK != mikroe_cmt_8540s_smt_init(app_pwm_instance)) {
    app_printf(
      "> Buzzer 2 Click board driver init failed, please run program again.\n");
    while (1) {}
  } else {
    app_printf("> Buzzer 2 Click board driver init successfully.\n");
    mikroe_cmt_8540s_smt_set_duty_cycle(0.0);
    mikroe_cmt_8540s_smt_pwm_start();
    sl_sleeptimer_delay_millisecond(100);
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  app_printf("Playing the Imperial March melody ...\r\n");
  imperial_march( );
  sl_sleeptimer_delay_millisecond(10000);
}

static void imperial_march(void)
{
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, H);
  sl_sleeptimer_delay_millisecond(1 + H);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F7, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, H);
  sl_sleeptimer_delay_millisecond(1 + H);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab7, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_G7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Gb7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F7, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Bb6, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Eb7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_D7, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Db7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_B6, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, H);
  sl_sleeptimer_delay_millisecond(1 + H);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab7, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_G7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Gb7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_E7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F7, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Bb6, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Eb7, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_D7, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Db7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_B6, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E);
  sl_sleeptimer_delay_millisecond(1 + E);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);

  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_A6, VOLUME, Q);
  sl_sleeptimer_delay_millisecond(1 + Q);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_F6, VOLUME, E + S);
  sl_sleeptimer_delay_millisecond(1 + E + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_C7, VOLUME, S);
  sl_sleeptimer_delay_millisecond(1 + S);
  mikroe_cmt_8540s_smt_play_sound(MIKROE_BUZZ2_NOTE_Ab6, VOLUME, H);
  sl_sleeptimer_delay_millisecond(1 + H);
}
