/***************************************************************************//**
 * @file sl_lin_test_master.c
 * @brief Simple LIN bus implementation for master devices, for testing purposes
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

#pragma GCC optimize "O2"
#pragma GCC optimize "fast-math"
#pragma GCC optimize "gcse-after-reload"
#pragma GCC optimize "gcse-las"
#pragma GCC optimize "gcse-sm"
#pragma GCC optimize "ira-loop-pressure"
#pragma GCC optimize "ivopts"
#pragma GCC optimize "loop-interchange"
#pragma GCC optimize "modulo-sched"
#pragma GCC optimize "modulo-sched-allow-regmoves"
#pragma GCC optimize "no-delete-null-pointer-checks"
#pragma GCC optimize "no-tree-loop-distribute-patterns"
#pragma GCC optimize "omit-frame-pointer"
#pragma GCC optimize "peel-loops"
#pragma GCC optimize "predictive-commoning"
#pragma GCC optimize "rename-registers"
#pragma GCC optimize "split-loops"
#pragma GCC optimize "split-paths"
#pragma GCC optimize "stdarg-opt"
#pragma GCC optimize "tree-loop-distribution"
#pragma GCC optimize "tree-partial-pre"
#pragma GCC optimize "unswitch-loops"
#pragma GCC optimize "web"

#include "sl_lin_test_master.h"

#include <em_emu.h>
#include <em_cmu.h>
#include <em_gpio.h>
#include <em_prs.h>
#include <em_usart.h>
#include <em_timer.h>
#include <em_letimer.h>
#include <em_burtc.h>
#include <em_ldma.h>
#include <em_ramfunc.h>

#include <stddef.h>
#include <inttypes.h>
#include <string.h>

#include <sl_gpio.h>
#include "sl_component_catalog.h"
#include "sl_sleeptimer.h"
#include "sl_power_manager.h"

#include "system.h"

#define SL_LIN_MAX_ENDPOINT 59
#define CURRENT_IDX         2

// the baud rate is (HFXO * 2) / USART_CLKDIV_DIV (fractional as-is)
// N bits of time is (500000 * N * USART_CLKDIV_DIV) / HFXO usecs
// which converts to (N * USART_CLKDIV_DIV * 16384) / HFXO ticks
// with N=34 and HFXO=38400000 that translates to
// (136 * USART_CLKDIV_DIV) /  9375
// (the raw register has an implicit multiplication of 8)
#define SL_BREAKDELIMSYNCPID_TICKS(reg) \
  (((reg & _USART_CLKDIV_DIV_MASK) * 17) / 9375)

#define SL_LIN_SYNC_BYTE    0x55
#define SL_LIN_DIAG_REQUEST 0x3c

// indifferent, better kept high
// must be settable for TIMER_TEST if it's enabled
// for PC03 the range is 6..11
#define PRS_TIMER_CC0       11 // master-only

// can be 0..5
#define PRS_USART_TX        5
#define PRS_USART_RX        4

#define _GPIO_PRS_ASYNCH_ROUTE(n)      ASYNCH ## n ## ROUTE
#define _GPIO_PRS_ASYNCH_ROUTEEN(n)    GPIO_PRS_ROUTEEN_ASYNCH ## n ## PEN
#define _GPIO_PRS_ASYNCH_PORT_SHIFT(n) _GPIO_PRS_ASYNCH ## n ## ROUTE_PORT_SHIFT
#define _GPIO_PRS_ASYNCH_PIN_SHIFT(n)  _GPIO_PRS_ASYNCH ## n ## ROUTE_PIN_SHIFT

#define GPIO_PRS_ASYNCH_ROUTE(n)       _GPIO_PRS_ASYNCH_ROUTE(n)
#define GPIO_PRS_ASYNCH_ROUTEEN(n)     _GPIO_PRS_ASYNCH_ROUTEEN(n)
#define GPIO_PRS_ASYNCH_PORT_SHIFT(n)  _GPIO_PRS_ASYNCH_PORT_SHIFT(n)
#define GPIO_PRS_ASYNCH_PIN_SHIFT(n)   _GPIO_PRS_ASYNCH_PIN_SHIFT(n)

#define USART_TX_ROUTE                 GPIO_PRS_ASYNCH_ROUTE(PRS_USART_TX)
#define PRS_USART_TX_ROUTEEN           GPIO_PRS_ASYNCH_ROUTEEN(PRS_USART_TX)
#define USART_TX_ROUTE_PORT_SHIFT      GPIO_PRS_ASYNCH_PORT_SHIFT(PRS_USART_TX)
#define USART_TX_ROUTE_PIN_SHIFT       GPIO_PRS_ASYNCH_PIN_SHIFT(PRS_USART_TX)

#if defined(TIMER_TEST_PORT)
#define TIMER_CC0_ROUTE                GPIO_PRS_ASYNCH_ROUTE(PRS_TIMER_CC0)
#define PRS_TIMER_CC0_ROUTEEN          GPIO_PRS_ASYNCH_ROUTEEN(PRS_TIMER_CC0)
#define TIMER_CC0_PORT_SHIFT           GPIO_PRS_ASYNCH_PORT_SHIFT(PRS_TIMER_CC0)
#define TIMER_CC0_PIN_SHIFT            GPIO_PRS_ASYNCH_PIN_SHIFT(PRS_TIMER_CC0)
#endif

// only one of them is active at a time, so one channel would be enough
// but it's easier to register separate callbacks just once,
// than switching one depending on the transfer
#define DMA_CH_USART_TX                4
#define DMA_CH_USART_RX                5

// PB3, the LIN_RX GPIO is the wakeup signal GPIO_EM4WU
#define WAKEUP_SIGNAL                  4U

#if defined(DO_WAKEUPS)
static volatile bool need_wakeup = true;
#endif

static unsigned int bittime;

static uint8_t sleep_data[8] =
{ 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, };

volatile uint32_t sl_lin_counter_master_checksum = 0;
volatile uint32_t sl_lin_counter_master_conflict = 0;
volatile uint32_t sl_lin_counter_master_generic = 0;
volatile uint32_t sl_lin_counter_master_timeout = 0;

volatile uint32_t sl_lin_counter_slave1_checksum = 0;
volatile uint32_t sl_lin_counter_slave1_conflict = 0;
volatile uint32_t sl_lin_counter_slave1_generic = 0;

volatile uint32_t sl_lin_counter_slave2_checksum = 0;
volatile uint32_t sl_lin_counter_slave2_conflict = 0;
volatile uint32_t sl_lin_counter_slave2_generic = 0;

// TODO: update master to use RTCC
#if defined(DO_WAKEUPS)
void BURTC_IRQHandler(void)
{
  // setting this over again without clearing does not affect anything
  need_wakeup = true;
  __DSB();
}

#endif

#define SLAVE1_CHECKSUM_MASK (1U << SLAVE1_CHECKSUM_PIN) // 1
#define SLAVE1_CONFLICT_MASK (1U << SLAVE1_CONFLICT_PIN) // 3
#define SLAVE1_GENERIC_MASK  (1U << SLAVE1_GENERIC_PIN)  // 5
#define SLAVE2_CHECKSUM_MASK (1U << SLAVE2_CHECKSUM_PIN) // 0
#define SLAVE2_CONFLICT_MASK (1U << SLAVE2_CONFLICT_PIN) // 2
#define SLAVE2_GENERIC_MASK  (1U << SLAVE2_GENERIC_PIN)  // 6

// EXTI 0, 1, 2, 3, 5, 6 are consumed
// EXTI 4, 7, 8, 9, 10, 11 are free
// handle signals of slave1 & slave2, all of them are routed to odd numbered pins
static void gpio_int_cb(uint8_t int_no, void *ctx)
{
  (void)ctx;

  if (int_no & SLAVE1_CHECKSUM_MASK) {
    sl_lin_counter_slave1_checksum++;
  }

  if (int_no & SLAVE1_CONFLICT_MASK) {
    sl_lin_counter_slave1_conflict++;
  }

  if (int_no & SLAVE1_GENERIC_MASK) {
    sl_lin_counter_slave1_generic++;
  }

  if (int_no & SLAVE2_CHECKSUM_MASK) {
    sl_lin_counter_slave2_checksum++;
  }

  if (int_no & SLAVE2_CONFLICT_MASK) {
    sl_lin_counter_slave2_conflict++;
  }

  if (int_no & SLAVE2_GENERIC_MASK) {
    sl_lin_counter_slave2_generic++;
  }
}

// blocking, use sparingly on the slave!
// might add a couple usecs of latency
static void sl_lin_delay(int usecs)
{
  usecs = (usecs - 17) / 5;

  TIMER_Enable(TIMER3, false);
  TIMER_TopSet(TIMER3, usecs);
  TIMER_CounterSet(TIMER3, usecs);
  TIMER_IntClear(TIMER3, TIMER_IF_UF);
  TIMER_Enable(TIMER3, true);
  while ((TIMER3->IF & TIMER_IF_UF) == 0) {}
}

// both for master and slave
void sl_lin_bus_wakeup(void)
{
  USART1->CTRL_SET = USART_CTRL_TXINV;
  sl_lin_delay(250);
  USART1->CTRL_CLR = USART_CTRL_TXINV;
}

// take care, input is not masked to gain speed
__attribute__((__pure__))
static uint8_t sl_lin_frame_id_to_pid(uint8_t frame_id)
{
  return frame_id
         | ((((frame_id >> 0U) & 1U)
             ^ ((frame_id >> 1U) & 1U)
             ^ ((frame_id >> 2U) & 1U)
             ^ ((frame_id >> 4U) & 1U)) << 6U)
         | ((((frame_id >> 1U) & 1U)
             ^ ((frame_id >> 3U) & 1U)
             ^ ((frame_id >> 4U) & 1U)
             ^ ((frame_id >> 5U) & 1U)
             ^ 1U) << 7U);
}

__attribute__((__pure__))
static uint8_t sl_lin_calc_checksum(uint8_t init, const uint8_t *data, int len)
{
  uint32_t checksum = init;

  if (likely(data != NULL)) {
    for (int i = 0; i < len; i++)
    {
      checksum += *data++;
      checksum += checksum >> 8;
      checksum &= 0xff;
    }
  }

  return checksum ^ 0xff;
}

void sl_lin_master_init(int baud)
{
  sl_gpio_t gpio_port_pin;
  int32_t int_no;
  USART_InitAsync_TypeDef uart = USART_INITASYNC_DEFAULT;
  LETIMER_Init_TypeDef letimer = LETIMER_INIT_DEFAULT;
  TIMER_Init_TypeDef breakTimer = TIMER_INIT_DEFAULT;
  TIMER_InitCC_TypeDef breakTimerCC0 = TIMER_INITCC_DEFAULT;
  TIMER_Init_TypeDef wakeTimer = TIMER_INIT_DEFAULT;
#if defined(DO_WAKEUPS)
  BURTC_Init_TypeDef burtc = BURTC_INIT_DEFAULT;
#endif

  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_PRS, true);
  CMU_ClockEnable(cmuClock_LETIMER0, true);
  // the clock source of the TIMER is assumed to be HFXO
  CMU_ClockEnable(cmuClock_TIMER3, true);
  CMU_ClockEnable(cmuClock_TIMER4, true);
#if defined(DO_WAKEUPS)
  CMU_ClockEnable(cmuClock_BURTC, true);
#endif

  CMU_ClockSelectSet(cmuClock_USART1, cmuSelect_EM01GRPACLK);
  CMU_ClockEnable(cmuClock_USART1, true);

  NVIC_DisableIRQ(LETIMER0_IRQn);
  LETIMER_IntDisable(LETIMER0, _LETIMER_IEN_MASK);
  LETIMER_IntClear(LETIMER0, _LETIMER_IF_MASK);
  NVIC_ClearPendingIRQ(LETIMER0_IRQn);

  NVIC_DisableIRQ(TIMER3_IRQn);
  TIMER_IntDisable(TIMER3, _TIMER_IEN_MASK);
  TIMER_IntClear(TIMER3, _TIMER_IF_MASK);
  NVIC_ClearPendingIRQ(TIMER3_IRQn);

  NVIC_DisableIRQ(TIMER4_IRQn);
  TIMER_IntDisable(TIMER4, _TIMER_IEN_MASK);
  TIMER_IntClear(TIMER4, _TIMER_IF_MASK);
  NVIC_ClearPendingIRQ(TIMER4_IRQn);

#if defined(DO_WAKEUPS)
  NVIC_DisableIRQ(BURTC_IRQn);
  BURTC_IntDisable(_BURTC_IEN_MASK);
  BURTC_IntClear(_BURTC_IF_MASK);
  NVIC_ClearPendingIRQ(BURTC_IRQn);
#endif

  NVIC_DisableIRQ(USART1_TX_IRQn);
  NVIC_DisableIRQ(USART1_RX_IRQn);
  USART_IntDisable(USART1, _USART_IEN_MASK);
  USART_IntClear(USART1, _USART_IF_MASK);
  NVIC_ClearPendingIRQ(USART1_TX_IRQn);
  NVIC_ClearPendingIRQ(USART1_RX_IRQn);

  GPIO->P_SET[LIN_TX_PORT].CTRL = GPIO_P_CTRL_DINDISALT;

  GPIO_PinModeSet(LIN_TX_PORT, LIN_TX_PIN, gpioModeWiredAndAlternate, 1);
  GPIO_PinModeSet(LIN_RX_PORT, LIN_RX_PIN, gpioModeInput, 0);

#if defined(TIMER_TEST_PORT)
  GPIO_PinModeSet(TIMER_TEST_PORT, TIMER_TEST_PIN, gpioModePushPull, 0);
#endif

#if defined(CHECKSUM_ERR_PORT) && defined(CHECKSUM_ERR_PIN)
  GPIO_PinModeSet(CHECKSUM_ERR_PORT, CHECKSUM_ERR_PIN, gpioModePushPull, 0);
#endif

#if defined(CONFLICT_ERR_PORT) && defined(CONFLICT_ERR_PIN)
  GPIO_PinModeSet(CONFLICT_ERR_PORT, CONFLICT_ERR_PIN, gpioModePushPull, 0);
#endif

#if defined(GENERIC_ERR_PORT) && defined(GENERIC_ERR_PIN)
  GPIO_PinModeSet(GENERIC_ERR_PORT, GENERIC_ERR_PIN, gpioModePushPull, 0);
#endif

#if defined(TIMEOUT_PORT) && defined(TIMEOUT_PIN)
  GPIO_PinModeSet(TIMEOUT_PORT, TIMEOUT_PIN, gpioModePushPull, 0);
#endif

#if defined(SLAVE1_CHECKSUM_PORT) && defined(SLAVE1_CHECKSUM_PIN)
  GPIO_PinModeSet(SLAVE1_CHECKSUM_PORT, SLAVE1_CHECKSUM_PIN, gpioModeInput, 0);
  gpio_port_pin.port = SLAVE1_CHECKSUM_PORT;
  gpio_port_pin.pin = SLAVE1_CHECKSUM_PIN;
  int_no = SLAVE1_CHECKSUM_PIN;
  sl_gpio_configure_external_interrupt(&gpio_port_pin,
                                       &int_no,
                                       SL_GPIO_INTERRUPT_RISING_FALLING_EDGE,
                                       gpio_int_cb,
                                       NULL);
#endif

#if defined(SLAVE1_CONFLICT_PORT) && defined(SLAVE1_CONFLICT_PIN)
  GPIO_PinModeSet(SLAVE1_CONFLICT_PORT, SLAVE1_CONFLICT_PIN, gpioModeInput, 0);
  gpio_port_pin.port = SLAVE1_CONFLICT_PORT;
  gpio_port_pin.pin = SLAVE1_CONFLICT_PIN;
  int_no = SLAVE1_CONFLICT_PIN;
  sl_gpio_configure_external_interrupt(&gpio_port_pin,
                                       &int_no,
                                       SL_GPIO_INTERRUPT_RISING_FALLING_EDGE,
                                       gpio_int_cb,
                                       NULL);
#endif

#if defined(SLAVE1_GENERIC_PORT) && defined(SLAVE1_GENERIC_PIN)
  GPIO_PinModeSet(SLAVE1_GENERIC_PORT, SLAVE1_GENERIC_PIN, gpioModeInput, 0);
  gpio_port_pin.port = SLAVE1_GENERIC_PORT;
  gpio_port_pin.pin = SLAVE1_GENERIC_PIN;
  int_no = SLAVE1_GENERIC_PIN;
  sl_gpio_configure_external_interrupt(&gpio_port_pin,
                                       &int_no,
                                       SL_GPIO_INTERRUPT_RISING_FALLING_EDGE,
                                       gpio_int_cb,
                                       NULL);
#endif

#if defined(SLAVE2_CHECKSUM_PORT) && defined(SLAVE2_CHECKSUM_PIN)
  GPIO_PinModeSet(SLAVE2_CHECKSUM_PORT, SLAVE2_CHECKSUM_PIN, gpioModeInput, 0);
  gpio_port_pin.port = SLAVE2_CHECKSUM_PORT;
  gpio_port_pin.pin = SLAVE2_CHECKSUM_PIN;
  int_no = SLAVE2_CHECKSUM_PIN;
  sl_gpio_configure_external_interrupt(&gpio_port_pin,
                                       &int_no,
                                       SL_GPIO_INTERRUPT_RISING_FALLING_EDGE,
                                       gpio_int_cb,
                                       NULL);
#endif

#if defined(SLAVE2_CONFLICT_PORT) && defined(SLAVE2_CONFLICT_PIN)
  GPIO_PinModeSet(SLAVE2_CONFLICT_PORT, SLAVE2_CONFLICT_PIN, gpioModeInput, 0);
  gpio_port_pin.port = SLAVE2_CONFLICT_PORT;
  gpio_port_pin.pin = SLAVE2_CONFLICT_PIN;
  int_no = SLAVE2_CONFLICT_PIN;
  sl_gpio_configure_external_interrupt(&gpio_port_pin,
                                       &int_no,
                                       SL_GPIO_INTERRUPT_RISING_FALLING_EDGE,
                                       gpio_int_cb,
                                       NULL);
#endif

#if defined(SLAVE2_GENERIC_PORT) && defined(SLAVE2_GENERIC_PIN)
  GPIO_PinModeSet(SLAVE2_GENERIC_PORT, SLAVE2_GENERIC_PIN, gpioModeInput, 0);
  gpio_port_pin.port = SLAVE2_GENERIC_PORT;
  gpio_port_pin.pin = SLAVE2_GENERIC_PIN;
  int_no = SLAVE2_GENERIC_PIN;
  sl_gpio_configure_external_interrupt(&gpio_port_pin,
                                       &int_no,
                                       SL_GPIO_INTERRUPT_RISING_FALLING_EDGE,
                                       gpio_int_cb,
                                       NULL);
#endif

  GPIO->USARTROUTE[1].ROUTEEN = 0;
  GPIO->USARTROUTE[1].RXROUTE = (LIN_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
                                | (LIN_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[1].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN;

  letimer.enable = false;
  letimer.comp0Top = true;
  LETIMER_Init(LETIMER0, &letimer);

  wakeTimer.enable = false;
  // configure a 5-usec timer
  wakeTimer.prescale = (CMU_ClockFreqGet(cmuClock_EM01GRPACLK) / 200000) - 1;
  wakeTimer.mode = timerModeDown;
  wakeTimer.oneShot = true;
  wakeTimer.disSyncOut = false;
  TIMER_Init(TIMER3, &wakeTimer);

  // configure a microsecond-timer
  breakTimer.enable = false;
  // timer.prescale = (CMU_ClockFreqGet(cmuClock_EM01GRPACLK) / 1000000) - 1;
  breakTimer.prescale = 1;
  breakTimer.oneShot = true;
  breakTimer.disSyncOut = false;
  breakTimerCC0.cofoa = timerOutputActionToggle; // L->H transition, starts
                                                 //   RX&TX
  breakTimerCC0.cmoa = timerOutputActionToggle;  // H->L transition just to be
                                                 //   sure
  breakTimerCC0.mode = timerCCModeCompare;
  breakTimerCC0.coist = true;
  breakTimerCC0.prsOutput = timerPrsOutputLevel;
  TIMER_InitCC(TIMER4, 0, &breakTimerCC0);
  TIMER_Init(TIMER4, &breakTimer);

  bittime = CMU_ClockFreqGet(cmuClock_EM01GRPACLK)
            / ((breakTimer.prescale + 1) * baud);

  TIMER_TopSet(TIMER4, 14 * bittime - 1); // 13+1 bit time, wraparound & stop
  TIMER_CompareSet(TIMER4, 0, 1 * bittime - 1); // 1 bit time before H->L
                                                //   transition
  TIMER_CounterSet(TIMER4, 0);

  GPIO->PRSROUTE_CLR[0].ROUTEEN = PRS_USART_TX_ROUTEEN;
#if defined(TIMER_TEST_PORT)
  GPIO->PRSROUTE_CLR[0].ROUTEEN = PRS_TIMER_CC0_ROUTEEN;
#endif

  PRS_ConnectSignal(PRS_TIMER_CC0, prsTypeAsync, PRS_TIMER4_CC0);
  PRS_ConnectSignal(PRS_USART_TX, prsTypeAsync, PRS_USART1_TX);
  PRS_Combine(PRS_USART_TX, PRS_TIMER_CC0, prsLogic_A_AND_B);

  // the trigger is a L->H transition
  PRS_ConnectConsumer(PRS_TIMER_CC0, prsTypeAsync, prsConsumerUSART1_TRIGGER);

  GPIO->PRSROUTE[0].USART_TX_ROUTE =
    (LIN_TX_PORT << USART_TX_ROUTE_PORT_SHIFT)
    | (LIN_TX_PIN << USART_TX_ROUTE_PIN_SHIFT);

  GPIO->PRSROUTE_SET[0].ROUTEEN = PRS_USART_TX_ROUTEEN;

#if defined(TIMER_TEST_PORT)
  GPIO->PRSROUTE[0].TIMER_CC0_ROUTE =
    (TIMER_TEST_PORT << TIMER_CC0_PORT_SHIFT)
    | (TIMER_TEST_PIN << TIMER_CC0_PIN_SHIFT);

  GPIO->PRSROUTE_SET[0].ROUTEEN = PRS_TIMER_CC0_ROUTEEN;
#endif

#if defined(DO_WAKEUPS)
  burtc.start = false;
  burtc.compare0Top = true;
  BURTC_Init(&burtc);

  // slaves might enter sleep mode after 4..10 seconds of inactivity
  // so after at most 4 seconds of inactivity, a wakeup shall be triggered
  BURTC_CompareSet(0, 4 * 32768);
  NVIC_EnableIRQ(BURTC_IRQn);
  BURTC_IntEnable(BURTC_IEN_OF);
  BURTC_Start();
#endif

  uart.baudrate = baud;
  USART_InitAsync(USART1, &uart);

  // the PRS trigger will enable the TX and the RX
  USART1->CMD = USART_CMD_TXDIS
                | USART_CMD_RXDIS
                | USART_CMD_RXBLOCKDIS
                | USART_CMD_CLEARTX
                | USART_CMD_CLEARRX;

  USART1->TIMING_CLR = _USART_TIMING_TXDELAY_MASK;
  USART1->TIMING_SET = USART_TIMING_TXDELAY_ONE;
  USART1->CTRL_SET = USART_CTRL_TXBIL | USART_CTRL_ERRSDMA
                     | USART_CTRL_CCEN /* | USART_CTRL_LOOPBK_ENABLE */;
  USART1->TRIGCTRL = USART_TRIGCTRL_TXTEN | USART_TRIGCTRL_RXTEN;

  while ((USART1->STATUS & (_USART_STATUS_TXBUFCNT_MASK
                            | USART_STATUS_TXIDLE
                            | USART_STATUS_RXDATAV
                            | USART_STATUS_RXBLOCK
                            | USART_STATUS_TXENS
                            | USART_STATUS_RXENS)) != USART_STATUS_TXIDLE) {}

  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}

sl_status_t sl_lin_master_transmit(uint8_t frame_id,
                                   const uint8_t *data,
                                   int len,
                                   bool enhanced_checksum,
                                   bool inject_checksum_error)
{
  uint32_t cmd = USART_CMD_TXDIS | USART_CMD_RXBLOCKDIS;
  uint32_t mask = USART_STATUS_TXENS | USART_STATUS_RXBLOCK;
  uint32_t res = 0;
  uint8_t pid, checksum;

  if (unlikely((data == NULL) ^ (len == 0))) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (unlikely(frame_id >= 0x3e)) {
    return SL_STATUS_INVALID_RANGE;
  }

  if (unlikely(frame_id >= 0x3c)) {
    enhanced_checksum = false;
  }

  pid = sl_lin_frame_id_to_pid(frame_id);
  checksum = sl_lin_calc_checksum((enhanced_checksum) ? pid : 0x00,
                                  data, len);
  if (unlikely(inject_checksum_error)) {
    checksum ^= 0xff;
  }

  // RX & TX is assumed to be disabled and will be enabled by PRS
  // have to block RX for collision detection
  USART1->CMD = USART_CMD_RXBLOCKEN
                | USART_CMD_CLEARTX
                | USART_CMD_CLEARRX;

  while ((USART1->STATUS & (_USART_STATUS_TXBUFCNT_MASK
                            | USART_STATUS_TXIDLE
                            | USART_STATUS_RXDATAV
                            | USART_STATUS_RXBLOCK))
         != (USART_STATUS_TXIDLE
             | USART_STATUS_RXBLOCK)) {}

  USART_IntClear(USART1, _USART_IF_MASK);

  // the FIFO is assumed to be at least 2-level deep
  USART1->TXDATA = SL_LIN_SYNC_BYTE;
  USART1->TXDATA = pid;

#if defined(DO_WAKEUPS)
  if (unlikely(need_wakeup)) {
    sl_lin_bus_wakeup();
    sl_lin_delay(100000);
  }

  BURTC_CounterReset();
  BURTC_IntClear(_BURTC_IF_MASK);
  NVIC_ClearPendingIRQ(BURTC_IRQn);
  need_wakeup = false;
#endif

  TIMER_Enable(TIMER4, true);

  // EUSART is expected to start transmitting from the FIFO, refill it
  if (data) {
    while (len)
    {
      while ((USART1->STATUS & USART_STATUS_TXBL) == 0) {}
      USART1->TXDATA = *data++;
      len--;
    }

    while ((USART1->STATUS & USART_STATUS_TXBL) == 0) {}
    USART1->TXDATA = checksum;
  }

  while ((USART1->STATUS & USART_STATUS_TXC) == 0) {}

  if (data) {
    cmd |= USART_CMD_RXDIS;
    mask |= USART_STATUS_RXENS;
  }

  USART1->CMD = cmd;
  while ((USART1->STATUS & mask) != res) {}

  if (unlikely((USART1->IF & USART_IF_CCF) != 0)) {
#if defined(CONFLICT_ERR_PORT) && defined(CONFLICT_ERR_PIN)
    GPIO_PinOutToggle(CONFLICT_ERR_PORT, CONFLICT_ERR_PIN);
#endif
    sl_lin_counter_master_conflict++;
  }

  return ((USART1->IF & USART_IF_CCF) == 0) ? SL_STATUS_OK : SL_STATUS_ABORT;
}

sl_status_t sl_lin_master_bus_sleep(bool force)
{
  (void)force;
  sl_status_t ret;

#if defined(DO_WAKEUPS)
  if (!force && need_wakeup) {
    return SL_STATUS_OK;
  }
#endif

  ret = sl_lin_master_transmit(SL_LIN_DIAG_REQUEST, sleep_data,
                               sizeof(sleep_data), false, false);

#if defined(DO_WAKEUPS)
  need_wakeup = true;
#endif

  return ret;
}

sl_status_t sl_lin_master_request(uint8_t frame_id,
                                  uint8_t *data,
                                  int len,
                                  bool enhanced_checksum)
{
  uint32_t checksum = 0;
  uint8_t pid;
  sl_status_t ret;

  if (unlikely((data == NULL) || (len == 0))) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (unlikely(frame_id >= 0x3e)) {
    return SL_STATUS_INVALID_RANGE;
  }

  if (unlikely(frame_id >= 0x3c)) {
    enhanced_checksum = false;
  }

  pid = sl_lin_frame_id_to_pid(frame_id);

  // the specification defines some timeouts,
  // which SHALL NOT be applied by any nodes
  // a slave MAY postpone the bytes of the answer as long as it wants
  // but a slave SHALL finish its answer before the next BREAK
  // a master is expected to send frames with some constant period
  // (not defined by the spec, 5 or 10 msec in example)
  // so in reality there IS a timeout for each frame
  // except... blah blah blah
  LETIMER0->TOP = SL_LIN_TIMEOUT;
  while (LETIMER0->SYNCBUSY != 0) {}
  LETIMER0->CMD = LETIMER_CMD_CLEAR | LETIMER_CMD_START;
  while (LETIMER0->SYNCBUSY != 0) {}
  LETIMER_IntClear(LETIMER0, LETIMER_IF_UF);

  ret = sl_lin_master_transmit(frame_id, NULL, 0, false, false);
  if (unlikely(ret != SL_STATUS_OK)) {
    goto error;
  }

  if (enhanced_checksum) {
    checksum += pid;
  }

  while (len)
  {
    uint8_t ch;

    while (((USART1->STATUS & USART_STATUS_RXDATAV) == 0)
           && ((USART1->IF & (USART_IF_FERR | USART_IF_RXOF)) == 0)
           && ((LETIMER0->IF & LETIMER_IF_UF) == 0)) {}

    if (unlikely((LETIMER0->IF & LETIMER_IF_UF) != 0)) {
#if defined(TIMEOUT_PORT) && defined(TIMEOUT_PIN)
      GPIO_PinOutToggle(TIMEOUT_PORT, TIMEOUT_PIN);
#endif
      sl_lin_counter_master_timeout++;

      ret = SL_STATUS_TIMEOUT;
      break;
    }

    if (unlikely((USART1->IF & (USART_IF_FERR | USART_IF_RXOF)) != 0)) {
#if defined(GENERIC_ERR_PORT) && defined(GENERIC_ERR_PIN)
      GPIO_PinOutToggle(GENERIC_ERR_PORT, GENERIC_ERR_PIN);
#endif
      sl_lin_counter_master_generic++;

      ret = SL_STATUS_IO;
      break;
    }

    ch = USART1->RXDATA;
    *data++ = ch;
    checksum += ch;
    len--;
  }

  // all data bytes have been received, fetch and validate the checksum
  if (likely(len == 0)) {
    while (((USART1->STATUS & USART_STATUS_RXDATAV) == 0)
           && ((USART1->IF & (USART_IF_FERR | USART_IF_RXOF)) == 0)
           && ((LETIMER0->IF & LETIMER_IF_UF) == 0)) {}

    if (unlikely((LETIMER0->IF & LETIMER_IF_UF) != 0)) {
#if defined(TIMEOUT_PORT) && defined(TIMEOUT_PIN)
      GPIO_PinOutToggle(TIMEOUT_PORT, TIMEOUT_PIN);
#endif
      sl_lin_counter_master_timeout++;

      ret = SL_STATUS_TIMEOUT;
    } else if (unlikely((USART1->IF & (USART_IF_FERR | USART_IF_RXOF)) != 0)) {
#if defined(GENERIC_ERR_PORT) && defined(GENERIC_ERR_PIN)
      GPIO_PinOutToggle(GENERIC_ERR_PORT, GENERIC_ERR_PIN);
#endif
      sl_lin_counter_master_generic++;

      ret = SL_STATUS_IO;
    } else {
      uint8_t ch;

      ch = USART1->RXDATA;
      checksum += ch;

      while (checksum > 0xff)
      {
        checksum -= 0xff;
      }

      if (unlikely(checksum != 0xff)) {
#if defined(CHECKSUM_ERR_PORT) && defined(CHECKSUM_ERR_PIN)
        GPIO_PinOutToggle(CHECKSUM_ERR_PORT, CHECKSUM_ERR_PIN);
#endif
        sl_lin_counter_master_checksum++;
      }

      ret = (checksum == 0xff) ? SL_STATUS_OK : SL_STATUS_IO;
    }
  }

  while (LETIMER0->SYNCBUSY != 0) {}
  LETIMER0->CMD = LETIMER_CMD_STOP;
  while (LETIMER0->SYNCBUSY != 0) {}

  error:
  USART1->CMD = USART_CMD_RXDIS;
  while ((USART1->STATUS & USART_STATUS_RXENS) != 0) {}

  return ret;
}

void sl_lin_master_reset_counters(void)
{
  sl_lin_counter_master_checksum = 0;
  sl_lin_counter_master_conflict = 0;
  sl_lin_counter_master_generic = 0;
  sl_lin_counter_master_timeout = 0;

  sl_lin_counter_slave1_checksum = 0;
  sl_lin_counter_slave1_conflict = 0;
  sl_lin_counter_slave1_generic = 0;

  sl_lin_counter_slave2_checksum = 0;
  sl_lin_counter_slave2_conflict = 0;
  sl_lin_counter_slave2_generic = 0;
}
