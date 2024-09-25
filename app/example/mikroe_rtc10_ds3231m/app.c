/***************************************************************************//**
 * @file app.c
 * @brief Example application
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
#include "mikroe_ds3231m.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#define READING_INTERVAL_MSEC        1000

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

static mikroe_i2c_handle_t app_i2c_instance = NULL;
static volatile bool flag = false;
static uint8_t sec_flag;
static sl_sleeptimer_timer_handle_t app_timer_handle;

/***************************************************************************//**
 * Display day of week function.
 ******************************************************************************/
static void display_day_of_the_week(uint8_t day_of_the_week)
{
  switch (day_of_the_week) {
    case 1:
      app_printf("      Monday    \r\n\n ");
      break;
    case 2:
      app_printf("      Tuesday   \r\n\n ");
      break;
    case 3:
      app_printf("     Wednesday  \r\n\n ");
      break;
    case 4:
      app_printf("     Thursday   \r\n\n ");
      break;
    case 5:
      app_printf("      Friday    \r\n\n ");
      break;
    case 6:
      app_printf("      Sunday    \r\n\n ");
      break;
    case 7:
      app_printf("     Saturday   \r\n\n ");
      break;
    default:
      break;
  }
}

/***************************************************************************//**
 * Application sleeptimer callback function.
 ******************************************************************************/
static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  flag = true;
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  app_printf("---- Application Init ----");

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  //  Click initialization.
  mikroe_ds3231m_init(app_i2c_instance);

  sl_sleeptimer_delay_millisecond(1000);

  sec_flag = 0xFF;

  app_printf("------------------- \r\n");
  app_printf("  Hardware  Reset   \r\n");
  mikroe_ds3231m_hw_reset();
  sl_sleeptimer_delay_millisecond(1000);

  // Set Time: 18h, 00 min and 00 sec
  mikroe_ds3231m_set_time(18, 0, 0);
  sl_sleeptimer_delay_millisecond(10);

  // Set Date: 6 ( Day of the week: Wednesday ), 6 ( day ), 12 ( month ) and
  //   2023 ( year )
  mikroe_ds3231m_set_date(3, 6, 12, 2023);
  sl_sleeptimer_delay_millisecond(100);

  app_printf("-------------------  \r\n");
  app_printf("  Enable Counting  \r\n");
  app_printf("------------------- \r\n");
  app_printf("     Start RTC      \r\n");
  app_printf("------------------- \r\n");
  mikroe_ds3231m_enable_counting();
  sl_sleeptimer_delay_millisecond(100);
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_cb,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (flag) {
    uint8_t time_hours = 0;
    uint8_t time_minutes = 0;
    uint8_t time_seconds = 0;

    uint8_t day_of_the_week = 0;
    uint8_t date_day = 0;
    uint8_t date_month = 0;
    uint8_t date_year = 0;

    float temperature;

    mikroe_ds3231m_get_time(&time_hours, &time_minutes, &time_seconds);
    sl_sleeptimer_delay_millisecond(100);

    mikroe_ds3231m_get_date(&day_of_the_week, &date_day, &date_month,
                            &date_year);
    sl_sleeptimer_delay_millisecond(100);

    if (sec_flag != time_seconds) {
      app_printf(" \r\n\n Time: %02u:%02u:%02u  ",
                 (uint16_t)time_hours,
                 (uint16_t)time_minutes,
                 (uint16_t)time_seconds);

      app_printf("Date: %u. %u. 20%u. ",
                 (uint16_t)date_day,
                 (uint16_t)date_month,
                 (uint16_t)date_year);
      display_day_of_the_week(day_of_the_week);

      if (time_seconds == 0) {
        temperature = mikroe_ds3231m_get_temperature();

        app_printf("\r\n\n Temp: %.2f C", temperature);
      }
      app_printf("--------------------------------------------");

      sec_flag = time_seconds;
    }
    flag = false;
  }
}
