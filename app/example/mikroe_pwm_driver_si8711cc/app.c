/***************************************************************************//**
 * @file app.c
 * @brief Top level application functions
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
 * # Experimental Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include "mikroe_si8711cc.h"
#include "sl_pwm_instances.h"
#include "sl_sleeptimer.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else /* None Si91x device */
#include "app_log.h"
#define app_printf(...) app_log(__VA_ARGS__)
#endif

static const float DUTY_CYCLE_STEP = 0.1;
static const uint16_t FREQUENCY = 1000;
static const uint16_t TIMER_PERIOD = 2000;

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool timer_trigger = false;
static uint8_t step_count = 1;
static float duty_cycle;
static mikroe_pwm_handle_t app_pwm_instance = NULL;
static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                               void *data);

void app_init(void)
{
#if (defined(SLI_SI917))
  app_pwm_instance = &sl_pwm_channel_0_config;
#else
  app_pwm_instance = &sl_pwm_mikroe;
#endif

  app_printf("Hello World - PWM Driver Click !!!\r\n");

  if (mikroe_si8711cc_init(app_pwm_instance) == SL_STATUS_OK) {
    app_printf("PWM Driver Click board driver init successfully\r\n");

    sl_status_t stt = sl_sleeptimer_restart_periodic_timer_ms(&app_timer_handle,
                                                              TIMER_PERIOD,
                                                              app_timer_callback,
                                                              NULL,
                                                              0,
                                                              0);

    app_printf("Start periodic timer %d(ms), status = 0x%lx\r\n",
               TIMER_PERIOD,
               stt);
    duty_cycle = step_count * DUTY_CYCLE_STEP;
    mikroe_si8711cc_set_frequency(FREQUENCY);
    mikroe_si8711cc_pwm_start();

    app_printf("mikroe_si8711cc_pwm_start \r\n");
  } else {
    app_printf("PWM Driver Click board driver init failed," \
               " please run program again.\r\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (timer_trigger) {
    timer_trigger = false;

    mikroe_si8711cc_set_duty_cycle(duty_cycle);
    app_printf("PWM Driver set duty = %d%%\r\n", (uint16_t)(duty_cycle * 100));

    step_count++;
    duty_cycle = step_count * DUTY_CYCLE_STEP;

    if (step_count >= 10) {
      step_count = 1;
      duty_cycle = 0.1;
    }
  }
}

static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;

  timer_trigger = true;
}
