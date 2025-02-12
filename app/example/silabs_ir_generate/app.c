/*******************************************************************************
 * @file app.c
 * @brief Example application
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

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_timer.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "app_log.h"
#include "ir_generate.h"

volatile uint16_t timer_1ms = 0;
static bool repeat_flag = false;
static uint8_t restart_conter = 0;
void TIMER2_init(void);
void TIMER2_IRQHandler(void);

void TIMER2_IRQHandler(void)
{
  // Acknowledge the interrupt
  uint32_t flags = TIMER_IntGet(TIMER2);
  TIMER_IntClear(TIMER2, flags);
  timer_1ms++;
}

void TIMER2_init(void)
{
  uint32_t timerFreq = 0;

  CMU_ClockEnable(cmuClock_TIMER2, true);

  // Initialize the timer
  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
  timerInit.prescale = timerPrescale2;
  timerInit.enable = false;

  // Configure but do not start the timer
  TIMER_Init(TIMER2, &timerInit);

  timerFreq = CMU_ClockFreqGet(cmuClock_TIMER2) / (timerInit.prescale + 1);
  TIMER_TopSet(TIMER2, (timerFreq / 1000));// 1000 for 1ms, 200 for 5ms

  // Start the timer
  TIMER_Enable(TIMER2, true);

  // Enable TIMER2 compare event interrupts to update the duty cycle
  TIMER_IntEnable(TIMER2, TIMER_IF_OF);
  NVIC_EnableIRQ(TIMER2_IRQn);
}

/**
 * @brief ir sent callback
 *
 * @param none
 *
 * @return none
 *
 */
void app_ir_complete(void)
{
  app_log("ir complete\r\n");
}

void app_init(void)
{
  TIMER2_init();
  app_log("test ir\r\n");

  ir_generate_init(CODE_NEC, app_ir_complete);

  // ir_generate_stream(0xFF, 0xFF, false); // for test
}

void app_process_action(void)
{
  uint8_t frame_interval[2] = { 108, 45 }; // 108 for NEC, 45 for SONY

  if (timer_1ms >= frame_interval[CODE_NEC]) {
    timer_1ms -= frame_interval[CODE_NEC];

    ir_generate_stream(0xFF, 0xFF, repeat_flag);   // for test

    // for NEC protocol, change after 4th repeat frame.
    if (restart_conter++ % 5 == 0) {
      repeat_flag = false;
    } else {
      repeat_flag = true;
    }
  }
}
