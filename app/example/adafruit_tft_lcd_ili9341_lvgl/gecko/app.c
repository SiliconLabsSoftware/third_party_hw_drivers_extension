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
#include "sl_component_catalog.h"
#include "adafruit_ili9341_spi_config.h"
#include "adafruit_ili9341.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "ui.h"

#define LVGL_TIMER_PERIOD     1
#define LVGL_LOADER_DELAY     3000

MIPI_DBI_SPI_INTERFACE_DEFINE(ili9341_config,
                              ADAFRUIT_ILI9341_PERIPHERAL,
                              ADAFRUIT_ILI9341_PERIPHERAL_NO,
                              ADAFRUIT_ILI9341_BITRATE,
                              ADAFRUIT_ILI9341_CLOCK_MODE,
                              ADAFRUIT_ILI9341_CS_CONTROL,
                              ADAFRUIT_ILI9341_CLK_PORT,
                              ADAFRUIT_ILI9341_CLK_PIN,
                              ADAFRUIT_ILI9341_TX_PORT,
                              ADAFRUIT_ILI9341_TX_PIN,
                              ADAFRUIT_ILI9341_RX_PORT,
                              ADAFRUIT_ILI9341_RX_PIN,
                              ADAFRUIT_ILI9341_CS_PORT,
                              ADAFRUIT_ILI9341_CS_PIN,
                              ADAFRUIT_ILI9341_DC_PORT,
                              ADAFRUIT_ILI9341_DC_PIN);

static volatile bool app_timer_expire = false;
static sl_sleeptimer_timer_handle_t main_timer;
static sl_sleeptimer_timer_handle_t lvgl_tick_timer;

static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data);
static void app_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                    void *data);

void app_init(void)
{
  adafruit_ili9341_init(&ili9341_config);
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  sl_sleeptimer_start_periodic_timer_ms(&lvgl_tick_timer,
                                        LVGL_TIMER_PERIOD,
                                        lvgl_tick_timer_callback,
                                        NULL,
                                        0,
                                        0);

  ui_init();
  sl_sleeptimer_start_timer_ms(&main_timer,
                               LVGL_LOADER_DELAY,
                               app_sleeptimer_callback,
                               NULL,
                               0,
                               0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  lv_timer_handler_run_in_period(5);

  if (app_timer_expire == true) {
    app_timer_expire = false;
    lv_disp_load_scr(ui_main);
  }
}

static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data)
{
  (void)timer;
  (void)data;

  lv_tick_inc(LVGL_TIMER_PERIOD);
}

static void app_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                    void *data)
{
  (void)timer;
  (void)data;

  app_timer_expire = true;
}
