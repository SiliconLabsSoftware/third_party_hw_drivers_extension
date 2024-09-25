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
#include "adafruit_hxd8357d_spi_config.h"
#include "adafruit_hxd8357d.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "ui.h"

#include "sl_simple_led_instances.h"
#include "sl_simple_button_instances.h"

#define LVGL_TIMER_PERIOD     1

static bool btn0_pressed = 0;
static bool btn0_released = 0;
static sl_sleeptimer_timer_handle_t lvgl_tick_timer;

MIPI_DBI_SPI_INTERFACE_DEFINE(hx8357d_config,
                              ADAFRUIT_HXD8357D_PERIPHERAL,
                              ADAFRUIT_HXD8357D_PERIPHERAL_NO,
                              ADAFRUIT_HXD8357D_BITRATE,
                              ADAFRUIT_HXD8357D_CLOCK_MODE,
                              ADAFRUIT_HXD8357D_CS_CONTROL,
                              ADAFRUIT_HXD8357D_CLK_PORT,
                              ADAFRUIT_HXD8357D_CLK_PIN,
                              ADAFRUIT_HXD8357D_TX_PORT,
                              ADAFRUIT_HXD8357D_TX_PIN,
                              ADAFRUIT_HXD8357D_RX_PORT,
                              ADAFRUIT_HXD8357D_RX_PIN,
                              ADAFRUIT_HXD8357D_CS_PORT,
                              ADAFRUIT_HXD8357D_CS_PIN,
                              ADAFRUIT_HXD8357D_DC_PORT,
                              ADAFRUIT_HXD8357D_DC_PIN);

static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data);

void app_init(void)
{
  adafruit_hxd8357d_init(&hx8357d_config);
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

  // init status led 0
  lv_obj_add_flag(ui_led0ONSymbol, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_led0OnLabel, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_led0OffSymbol, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_led0OffLabel, LV_OBJ_FLAG_HIDDEN);

  // init status Led 1
  lv_obj_add_flag(ui_led1ONSymbol, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_led1OnLabel, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_led1OffSymbol, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_led1OffLabel, LV_OBJ_FLAG_HIDDEN);

  // init status button 0
  lv_obj_add_flag(ui_buttonOnSymbol, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_buttonOffSymbol, LV_OBJ_FLAG_HIDDEN);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  lv_timer_handler_run_in_period(5);

  if (btn0_pressed) {
    lv_obj_add_flag(ui_buttonOffSymbol, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_buttonOnSymbol, LV_OBJ_FLAG_HIDDEN);
    btn0_pressed = false;
  }
  if (btn0_released) {
    lv_obj_add_flag(ui_buttonOnSymbol, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_buttonOffSymbol, LV_OBJ_FLAG_HIDDEN);
    btn0_released = false;
  }
}

static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data)
{
  (void)timer;
  (void)data;

  lv_tick_inc(LVGL_TIMER_PERIOD);
}

void sl_button_on_change(const sl_button_t *handle)
{
  if (handle == &sl_button_btn0) {
    if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
      btn0_pressed = true;
    } else {
      btn0_released = true;
    }
  }
}
