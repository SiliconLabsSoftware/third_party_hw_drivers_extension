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
#include "lv_port_disp.h"
#include "app_ui/ui.h"
#include "sl_sleeptimer.h"
#include "sl_si91x_led.h"
#include "sl_si91x_button.h"
#include "sl_si91x_button_pin_config.h"
#include "sl_si91x_led_config.h"
#include "sl_si91x_button_instances.h"
#include "sl_si91x_led_instances.h"
#include "adafruit_st7789_spi_config.h"
#include "adafruit_st7789.h"

#define LVGL_TIMER_PERIOD     1
static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data);

MIPI_DBI_SPI_INTERFACE_DEFINE(st7789_config,
                              ADAFRUIT_ST7789_BITRATE,
                              ADAFRUIT_ST7789_CLOCK_MODE,
                              ADAFRUIT_ST7789_CS_CONTROL,
                              RTE_GSPI_MASTER_CLK_PORT,
                              RTE_GSPI_MASTER_CLK_PIN,
                              SL_GPIO_MODE_4,
                              RTE_GSPI_MASTER_MOSI_PORT,
                              RTE_GSPI_MASTER_MOSI_PIN,
                              SL_GPIO_MODE_4,
                              RTE_GSPI_MASTER_MISO_PORT,
                              RTE_GSPI_MASTER_MISO_PIN,
                              SL_GPIO_MODE_4,
                              RTE_GSPI_MASTER_CS0_PORT,
                              RTE_GSPI_MASTER_CS0_PIN,
                              SL_GPIO_MODE_4,
                              ADAFRUIT_ST7789_DC_PORT,
                              ADAFRUIT_ST7789_DC_PIN,
                              SL_GPIO_MODE_0);

static sl_sleeptimer_timer_handle_t lvgl_tick_timer;

static bool led0_state = false;
static bool led1_state = false;

static bool btn0_trigger = false;
static bool btn1_trigger = false;

void app_init(void)
{
  printf("Hello World Adafruit 1.14\" TFT LCD with LVGL Demo\r\n");
  adafruit_st7789_init(&st7789_config);
  adafruit_st7789_set_rotation(adafruit_st7789_rotation_90);

  printf("adafruit_st7789_init done\r\n");

  lv_init();
  lv_port_disp_init();

  sl_sleeptimer_start_periodic_timer_ms(&lvgl_tick_timer,
                                        LVGL_TIMER_PERIOD,
                                        lvgl_tick_timer_callback,
                                        NULL,
                                        0,
                                        0);
  printf("lvgl init done\r\n");

  ui_init();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  lv_timer_handler_run_in_period(5);

  if (btn0_trigger) {
    btn0_trigger = false;

    if (sl_si91x_button_state_get(button_btn0.pin)) {
      led0_state = !led0_state;
      printf("led0 state = %d\r\n", (uint8_t)led0_state);
      if (led0_state) {
        sl_si91x_led_set(led_led0.pin);
        lv_obj_add_flag(ui_imgled0off, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_imgled0on, LV_OBJ_FLAG_HIDDEN);
      } else {
        sl_si91x_led_clear(led_led0.pin);
        lv_obj_add_flag(ui_imgled0on, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_imgled0off, LV_OBJ_FLAG_HIDDEN);
      }
    }
  }

  if (btn1_trigger) {
    btn1_trigger = false;

    if (sl_si91x_button_state_get(button_btn1.pin)) {
      led1_state = !led1_state;
      printf("led1 state = %d\r\n", (uint8_t)led1_state);
      if (led1_state) {
        sl_si91x_led_set(led_led1.pin);
        lv_obj_add_flag(ui_imgled1off, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_imgled1on, LV_OBJ_FLAG_HIDDEN);
      } else {
        sl_si91x_led_clear(led_led1.pin);
        lv_obj_add_flag(ui_imgled1on, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_imgled1off, LV_OBJ_FLAG_HIDDEN);
      }
    }
  }
}

static void lvgl_tick_timer_callback(sl_sleeptimer_timer_handle_t *timer,
                                     void *data)
{
  (void)timer;
  (void)data;

  lv_tick_inc(LVGL_TIMER_PERIOD);
}

void sl_si91x_button_isr(uint8_t pin, int8_t state)
{
  if (pin == button_btn0.pin) {
    if (state == BUTTON_PRESSED) {
      btn0_trigger = true;
    }
  } else if (pin == button_btn1.pin) {
    if (state == BUTTON_PRESSED) {
      btn1_trigger = true;
    }
  }
}
