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
#include "sl_sleeptimer.h"
#include "mikroe_adpd188bi_i2c.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED            SL_I2C2

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

#define SMOKE_THRESHOLD_VALUE        500

#define EXAMPLE_MODE_SMOKE           0
#define EXAMPLE_MODE_PROXIMITY       1
#define EXAMPLE_MODE                 EXAMPLE_MODE_SMOKE

static mikroe_i2c_handle_t app_i2c_instance = NULL;

#if (EXAMPLE_MODE == EXAMPLE_MODE_SMOKE)
static void smoke_example (void);

#elif (EXAMPLE_MODE == EXAMPLE_MODE_PROXIMITY)
static void proximity_example (void);

#endif

void app_init(void)
{
  sl_status_t stt;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  app_printf("Mikroe Smoke 2 Click - I2C Interface example\r\n");
  stt = mikroe_adpd188bi_init(app_i2c_instance);
  app_printf("mikroe_adpd188bi_init = 0x%lx\r\n", stt);

  mikroe_adpd188bi_soft_reset();
  app_printf("mikroe_adpd188bi_soft_reset\r\n");

  mikroe_adpd188bi_set_mode(MIKROE_ADPD188BI_MODE_IDLE);
  app_printf("mikroe_adpd188bi_set_mode MIKROE_ADPD188BI_MODE_IDLE\r\n");

  uint16_t devid = mikroe_adpd188bi_read_data(MIKROE_ADPD188BI_REG_DEVID);

  app_printf("Smoke2 Click ID:  0x%.2X\r\n", ( uint16_t ) (devid & 0xFF));
  app_printf("Smoke2 Click REV: 0x%.2X\r\n",
             ( uint16_t ) ((devid >> 8) & 0xFF));
  sl_sleeptimer_delay_millisecond(1000);

  mikroe_adpd188bi_default_cfg();
  app_printf("mikroe_adpd188bi_default_cfg\r\n");

#if (EXAMPLE_MODE == EXAMPLE_MODE_SMOKE)
  app_printf("SMOKE MODE \r\n");
  app_printf("Calibration Start\r\n");
  uint16_t calib_data = mikroe_adpd188bi_smoke_calibration(
    SMOKE_THRESHOLD_VALUE);
  app_printf("Calibration data: %u\r\n", calib_data);

#elif (EXAMPLE_MODE == EXAMPLE_MODE_PROXIMITY)
  app_printf("PROXIMITY MODE \r\n");
#endif
  app_printf("Application Task \r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
#if (EXAMPLE_MODE == EXAMPLE_MODE_SMOKE)
  smoke_example();
#elif (EXAMPLE_MODE == EXAMPLE_MODE_PROXIMITY)
  proximity_example();
#endif
}

#if (EXAMPLE_MODE == EXAMPLE_MODE_SMOKE)
static void smoke_example(void)
{
  static uint8_t last_state = 0;
  uint8_t smoke_detected = mikroe_adpd188bi_check_smoke();

  if (last_state != smoke_detected) {
    if (MIKROE_ADPD188BI_SMOKE_DETECTED == smoke_detected) {
      app_printf("SMOKE_DETECTED <<\r\n");
    } else if (MIKROE_ADPD188BI_SMOKE_NOT_DETECTED == smoke_detected) {
      app_printf("SMOKE_CLEAR <<\r\n");
    } else {
      app_printf("ERROR <<\r\n");
      for ( ; ; ) {}
    }
    last_state = smoke_detected;
  }
}

#elif (EXAMPLE_MODE == EXAMPLE_MODE_PROXIMITY)
static void proximity_example(void)
{
  uint16_t sens_data = 0;
  uint8_t fifo_int = 0;
  uint8_t slot_a_int = 0;
  uint8_t slot_b_int = 0;

  if (0 == mikroe_adpd188bi_get_int_pin()) {
    mikroe_adpd188bi_get_int(&fifo_int, &slot_a_int, &slot_b_int);
    if (0 != slot_a_int) {
      sens_data = mikroe_adpd188bi_read_sens_data(MIKROE_ADPD188BI_SLOT_A,
                                                  MIKROE_ADPD188BI_CHN_1);
      app_printf("DATA A: %d\r\n", sens_data);
      slot_a_int = 0;
      sl_sleeptimer_delay_millisecond(100);
    }
    if (0 != slot_b_int) {
      sens_data = mikroe_adpd188bi_read_sens_data(MIKROE_ADPD188BI_SLOT_B,
                                                  MIKROE_ADPD188BI_CHN_1);
      app_printf("DATA B: %d\r\n", sens_data);
      slot_b_int = 0;
      sl_sleeptimer_delay_millisecond(100);
    }
    if (0 != fifo_int) {
      app_printf("Fifo int: --------------------\r\n");
      for ( uint8_t fifo_cnt = 0; fifo_cnt < fifo_int; fifo_cnt++ )
      {
        sens_data = mikroe_adpd188bi_read_data(
          MIKROE_ADPD188BI_REG_FIFO_ACCESS);
        app_printf("FIFO: %d\r\n", sens_data);
      }
      sl_sleeptimer_delay_millisecond(100);
    }
  }
}

#endif
