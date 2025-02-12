/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif // SL_CATALOG_POWER_MANAGER_PRESENT
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT

#include "sl_lin_test_master.h"

#include <stdio.h>
#include <em_cmu.h>
#include <em_timer.h>

#define RUNS_PER_TEST 500

static uint8_t buf[] = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, };

static uint8_t incoming[8];

#define ESCAPE        "\x1b"
#define SET_RED       ESCAPE "[31m"
#define SET_GREEN     ESCAPE "[32m"
#define RESET_COLOR   ESCAPE "[39m"

static const char * const result[2] =
{ SET_RED "FAILED" RESET_COLOR, SET_GREEN "PASSED" RESET_COLOR };

static void dump_counters(void)
{
  printf(
    "    MASTER checksum: %4lu conflict: %4lu generic: %4lu timeout: %4lu\n",
    sl_lin_counter_master_checksum,
    sl_lin_counter_master_conflict,
    sl_lin_counter_master_generic,
    sl_lin_counter_master_timeout);

  printf("    SLAVE1 checksum: %4lu conflict: %4lu generic: %4lu\n",
         sl_lin_counter_slave1_checksum,
         sl_lin_counter_slave1_conflict,
         sl_lin_counter_slave1_generic);

  printf("    SLAVE2 checksum: %4lu conflict: %4lu generic: %4lu\n",
         sl_lin_counter_slave2_checksum,
         sl_lin_counter_slave2_conflict,
         sl_lin_counter_slave2_generic);
}

static void master_to_slave(uint8_t address, bool inject_checksum_error)
{
  sl_lin_master_reset_counters();

  for (unsigned int i = 0; i < RUNS_PER_TEST; i++)
  {
    TIMER_CounterSet(TIMER0, 0);
    TIMER_IntClear(TIMER0, TIMER_IF_CC0);

    sl_lin_master_transmit(address,
                           buf,
                           sizeof(buf),
                           true,
                           inject_checksum_error);

    while (((TIMER0->IF & TIMER_IF_CC0)) == 0) {}
  }

  TIMER_CounterSet(TIMER0, 0);
  TIMER_IntClear(TIMER0, TIMER_IF_CC0);

  while (((TIMER0->IF & TIMER_IF_CC0)) == 0) {}
}

static void slave_to_master(uint8_t address)
{
  sl_lin_master_reset_counters();

  for (unsigned int i = 0; i < RUNS_PER_TEST; i++)
  {
    TIMER_CounterSet(TIMER0, 0);
    TIMER_IntClear(TIMER0, TIMER_IF_CC0);

    sl_lin_master_request(address, incoming, 8, true);

    while (((TIMER0->IF & TIMER_IF_CC0)) == 0) {}
  }

  TIMER_CounterSet(TIMER0, 0);
  TIMER_IntClear(TIMER0, TIMER_IF_CC0);

  while (((TIMER0->IF & TIMER_IF_CC0)) == 0) {}
}

static bool master_to_slave_to_master(uint8_t address)
{
  bool passed = true;
  uint8_t workbuf[8];

  sl_lin_master_reset_counters();

  for (unsigned int i = 0; i < RUNS_PER_TEST; i++)
  {
    for (unsigned int j = 0; j < sizeof(workbuf); j++)
    {
      workbuf[j] = rand() & 0xff;
    }

    TIMER_CounterSet(TIMER0, 0);
    TIMER_IntClear(TIMER0, TIMER_IF_CC0);

    sl_lin_master_transmit(address, workbuf, sizeof(workbuf), true, false);

    while (((TIMER0->IF & TIMER_IF_CC0)) == 0) {}

    TIMER_CounterSet(TIMER0, 0);
    TIMER_IntClear(TIMER0, TIMER_IF_CC0);

    sl_lin_master_request(address + 1, incoming, 8, true);

    for (unsigned int j = 0; j < sizeof(workbuf); j++)
    {
      if (incoming[j] != workbuf[j]) {
        passed = false;
      }
    }

    while (((TIMER0->IF & TIMER_IF_CC0)) == 0) {}
  }

  TIMER_CounterSet(TIMER0, 0);
  TIMER_IntClear(TIMER0, TIMER_IF_CC0);

  while (((TIMER0->IF & TIMER_IF_CC0)) == 0) {}

  return passed;
}

void app_init(void)
{
  TIMER_Init_TypeDef periodTimer = TIMER_INIT_DEFAULT;
  TIMER_InitCC_TypeDef periodTimerCC0 = TIMER_INITCC_DEFAULT;
  bool matches;

  srand(DEVINFO->EUI48L);

  CMU_ClockEnable(cmuClock_TIMER0, true);
  periodTimerCC0.mode = timerCCModeCompare;
  TIMER_InitCC(TIMER0, 0, &periodTimerCC0);
  TIMER_Init(TIMER0, &periodTimer);
  TIMER_TopSet(TIMER0, 0xffffffff);
  // 10 msec in 38.4 MHz ticks (100 Hz)
  TIMER_CompareSet(TIMER0, 0, 384000);

  sl_lin_master_init(20000);

  // while (1)
  {
    puts(
      "\n\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");

    // normal write, no errors should happen
    printf(" 1) Writing %u packets to slave1 (11) => ", RUNS_PER_TEST);
    master_to_slave(11, false);
    puts(result[sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0]);
    dump_counters();

    // normal write, no errors should happen
    printf(" 2) Writing %u packets to slave2 (21) => ", RUNS_PER_TEST);
    master_to_slave(21, false);
    puts(result[sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0]);
    dump_counters();

    // normal read, no errors should happen
    printf(" 3) Reading %u packets from slave1 (12) => ", RUNS_PER_TEST);
    slave_to_master(12);
    puts(result[sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0]);
    dump_counters();

    // normal read, no errors should happen
    printf(" 4) Reading %u packets from slave2 (22) => ", RUNS_PER_TEST);
    slave_to_master(22);
    puts(result[sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0]);
    dump_counters();

    // normal write to two devices at once, no errors should happen
    printf(" 5) Writing %u packets to slave1 and slave2 (31) => ",
           RUNS_PER_TEST);
    master_to_slave(31, false);
    puts(result[sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0]);
    dump_counters();

    // writing to a read endpoint, conflict detection on either master or slave
    //   (but not both of them)
    printf(" 6) Writing %u packets to slave1 (12) (conflict) => ",
           RUNS_PER_TEST);
    master_to_slave(12, false);
    puts(result[sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0
                && (sl_lin_counter_master_conflict
                    + sl_lin_counter_slave1_conflict) == RUNS_PER_TEST]);
    dump_counters();

    // writing to a read endpoint, conflict detection on either master or slave
    //   (but not both of them)
    printf(" 7) Writing %u packets to slave2 (22) (conflict) => ",
           RUNS_PER_TEST);
    master_to_slave(22, false);
    puts(result[sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_generic == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && (sl_lin_counter_master_conflict
                    + sl_lin_counter_slave2_conflict) == RUNS_PER_TEST]);
    dump_counters();

    // normal read from two devices at once, conflict detection on one slave
    //   device (but not both of them)
    printf(
      " 8) Reading %u packets from slave1 and slave2 (32) (conflict or checksum) => ",
      RUNS_PER_TEST);
    slave_to_master(32);
    puts(result[sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_generic == 0
                && (sl_lin_counter_slave1_conflict
                    + sl_lin_counter_slave2_conflict) == RUNS_PER_TEST]);
    dump_counters();

    // reading checkum error-injected packets, checksum error detection on
    //   master
    printf(" 9) Reading %u packets from slave1 (15) (checksum) => ",
           RUNS_PER_TEST);
    slave_to_master(15);
    puts(result[sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0
                && sl_lin_counter_master_checksum == RUNS_PER_TEST]);
    dump_counters();

    // reading checksum error-injected packets, checksum error detection on
    //   master
    printf("10) Reading %u packets from slave2 (25) (checksum) => ",
           RUNS_PER_TEST);
    slave_to_master(25);
    puts(result[sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0
                && sl_lin_counter_master_checksum == RUNS_PER_TEST]);
    dump_counters();

    // writing checksum error-injected packets to two devices, checksum error
    //   detection on both of them
    printf("11) Writing %u packets to slave1 and slave2 (35) (checksum) => ",
           RUNS_PER_TEST);
    master_to_slave(35, true);
    puts(result[sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0
                && sl_lin_counter_slave1_checksum == RUNS_PER_TEST
                && sl_lin_counter_slave2_checksum == RUNS_PER_TEST]);
    dump_counters();

    // writing packet to a slave device which is a read on the other one,
    //   conflict detection on either master or the other slave (but not both of
    //   them)
    printf("12) Writing %u packets to slave1 (36) (conflict or checksum) => ",
           RUNS_PER_TEST);
    master_to_slave(36, false);
    puts(result[sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_generic == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && (sl_lin_counter_master_conflict
                    + sl_lin_counter_slave2_conflict) == RUNS_PER_TEST]);
    dump_counters();

    // writing packet to a slave device and reading back, data should match
    printf("13) Writing & reading %u packets to slave1 (13) => ",
           RUNS_PER_TEST);
    matches = master_to_slave_to_master(13);
    puts(result[sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0
                && matches]);
    dump_counters();

    // writing packet to a slave device and reading back, data should match
    printf("14) Writing & reading %u packets to slave2 (23) => ",
           RUNS_PER_TEST);
    matches = master_to_slave_to_master(23);
    puts(result[sl_lin_counter_master_conflict == 0
                && sl_lin_counter_master_checksum == 0
                && sl_lin_counter_master_generic == 0
                && sl_lin_counter_master_timeout == 0
                && sl_lin_counter_slave1_checksum == 0
                && sl_lin_counter_slave1_conflict == 0
                && sl_lin_counter_slave1_generic == 0
                && sl_lin_counter_slave2_checksum == 0
                && sl_lin_counter_slave2_conflict == 0
                && sl_lin_counter_slave2_generic == 0
                && matches]);
    dump_counters();
  }

  while (1) {}
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
