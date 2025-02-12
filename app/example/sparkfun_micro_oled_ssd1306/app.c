/***************************************************************************//**
 * @file
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

#include "glib.h"
#include "glib_font.h"
#include "micro_oled_ssd1306.h"
#include "sl_sleeptimer.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "sl_si91x_button_instances.h"

#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_simple_button_instances.h"
#include "sl_i2cspm_instances.h"
#endif

#define DISPLAY_MODE_COUNT           (12)

static glib_context_t glib_context;
static volatile uint8_t display_mode;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static const unsigned char silicon_labs_logo_64x23[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x80, 0xff, 0x1f, 0x00, 0x1e, 0x00, 0x00, 0x00, 0xf0,
  0xff, 0x7f, 0x00, 0x3e, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x7f, 0x00, 0x7f,
  0x00, 0x00, 0x00, 0xff, 0xff, 0x3f, 0x80, 0x7f, 0x00, 0x06, 0x80, 0xff,
  0xff, 0xff, 0xe0, 0xff, 0x80, 0x03, 0xc0, 0xff, 0x01, 0xf8, 0xff, 0x7f,
  0xe0, 0x01, 0xc0, 0x03, 0x00, 0x00, 0xfe, 0x7f, 0xf0, 0x01, 0x00, 0x01,
  0x00, 0x00, 0xf8, 0x3f, 0xf8, 0x03, 0x00, 0xf0, 0x07, 0x00, 0xe0, 0x3f,
  0xfc, 0x03, 0x00, 0x00, 0x20, 0x00, 0xe0, 0x1f, 0xfe, 0x0f, 0x00, 0x00,
  0xc0, 0x00, 0xe0, 0x07, 0xfe, 0xff, 0x00, 0x00, 0xfc, 0x00, 0xe0, 0x03,
  0xff, 0xc3, 0xff, 0xff, 0xff, 0x00, 0xf0, 0x00, 0x7e, 0x00, 0xfe, 0xff,
  0x7f, 0x00, 0x38, 0x00, 0x3e, 0x00, 0xff, 0xff, 0x3f, 0x00, 0x0c, 0x00,
  0x1c, 0x80, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x18, 0x00, 0xff, 0xff,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif
  // Initialize the display
  ssd1306_init(app_i2c_instance);
  glib_init(&glib_context);

  // Fill lcd with background color
  glib_clear(&glib_context);

  display_mode = 0;
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint8_t i;
  switch (display_mode) {
    case 0:
      glib_clear(&glib_context);
      glib_draw_string(&glib_context, "SILABS", 0, 2);

      glib_draw_string(&glib_context, "OLED", 0, 12);

      glib_draw_string(&glib_context, "Sparkfun", 0, 22);

      glib_draw_string(&glib_context, "PressBTN0", 0, 32);

      glib_update_display();
      break;

    case 1:
      glib_clear(&glib_context);
      glib_draw_rect(&glib_context,
                     5, 5, 40, 20,
                     GLIB_WHITE);
      glib_draw_string(&glib_context, "rect", 15, 35);
      glib_update_display();
      break;

    case 2:
      glib_clear(&glib_context);
      glib_fill_rect(&glib_context,
                     5, 5, 55, 25,
                     GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 6, 7);
      glib_draw_string(&glib_context, "rectangle", 6, 18);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 3:
      glib_clear(&glib_context);
      glib_draw_round_rect(&glib_context,
                           2, 5, 60, 40,
                           10,
                           GLIB_WHITE);
      glib_draw_string(&glib_context, "round", 6, 14);
      glib_draw_string(&glib_context, "rectangle", 6, 25);
      glib_update_display();
      break;

    case 4:
      glib_clear(&glib_context);
      glib_fill_round_rect(&glib_context,
                           2, 5, 60, 40,
                           10,
                           GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 6, 7);
      glib_draw_string(&glib_context, "round", 6, 18);
      glib_draw_string(&glib_context, "rectangle", 6, 29);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 5:
      glib_clear(&glib_context);
      glib_draw_triangle(&glib_context,
                         2, 2,
                         40, 3,
                         5, 40,
                         GLIB_WHITE);
      glib_draw_string(&glib_context, "triangle", 15, 31);
      glib_update_display();
      break;

    case 6:
      glib_clear(&glib_context);
      glib_fill_triangle(&glib_context,
                         2, 2,
                         40, 3,
                         5, 40,
                         GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 40, 20);
      glib_draw_string(&glib_context, "triangle", 15, 31);
      glib_update_display();
      break;

    case 7:
      glib_clear(&glib_context);
      glib_draw_circle(&glib_context, 32, 24, 20,
                       GLIB_WHITE);
      glib_draw_string(&glib_context, "circle", 15, 18);
      glib_update_display();
      break;

    case 8:
      glib_clear(&glib_context);
      glib_fill_circle(&glib_context, 32, 24, 20,
                       GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "fill", 16, 12);
      glib_draw_string(&glib_context, "circle", 16, 23);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 9:
      glib_clear(&glib_context);
      glib_fill_circle_helper(&glib_context, 40, 20, 15,
                              2, 10, GLIB_WHITE);
      glib_update_display();
      break;

    case 10:
      glib_clear(&glib_context);
      glib_set_font(&glib_context, &glib_font_free_sans_9pt7b);
      glib_draw_string(&glib_context, "Custom", 1, 14);
      glib_draw_string(&glib_context, "font", 1, 30);
      glib_set_font(&glib_context, NULL);
      glib_update_display();
      break;

    case 11:
      glib_clear(&glib_context);
      glib_fill(&glib_context, GLIB_WHITE);
      glib_set_color(&glib_context, GLIB_BLACK, GLIB_WHITE);
      glib_draw_string(&glib_context, "FILL", 16, 23);
      glib_set_color(&glib_context, GLIB_WHITE, GLIB_WHITE);
      glib_update_display();
      break;

    case 12:
      glib_clear(&glib_context);
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_64x23,
                        64, 23, GLIB_WHITE);
      glib_draw_string(&glib_context, "XBITMAP", 10, 30);
      glib_update_display();

      sl_sleeptimer_delay_millisecond(500);
      glib_set_invert_color();
      sl_sleeptimer_delay_millisecond(500);
      glib_set_normal_color();
      sl_sleeptimer_delay_millisecond(500);
      glib_set_invert_color();
      sl_sleeptimer_delay_millisecond(500);
      glib_set_normal_color();

      for (i = 0x8F; i > 0x00; i--) {
        glib_set_contrast(i);
        sl_sleeptimer_delay_millisecond(5);
      }

      for (i = 0x00; i < 0x8F; i++) {
        glib_set_contrast(i);
        sl_sleeptimer_delay_millisecond(5);
      }

      glib_scroll_right(0x00, 0x06);
      sl_sleeptimer_delay_millisecond(1000);
      glib_stop_scroll();
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_64x23,
                        64, 23, GLIB_WHITE);
      glib_draw_string(&glib_context, "XBITMAP", 10, 30);
      glib_update_display();

      glib_scroll_left(0x00, 0x06);
      sl_sleeptimer_delay_millisecond(1000);
      glib_stop_scroll();
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_64x23,
                        64, 23, GLIB_WHITE);
      glib_draw_string(&glib_context, "XBITMAP", 10, 30);
      glib_update_display();

      glib_scroll_diag_right(0x00, 0x06);
      sl_sleeptimer_delay_millisecond(1000);
      glib_stop_scroll();
      glib_draw_xbitmap(&glib_context,
                        0, 0, silicon_labs_logo_64x23,
                        64, 23, GLIB_WHITE);
      glib_draw_string(&glib_context, "XBITMAP", 10, 30);
      glib_update_display();

      glib_scroll_diag_left(0x00, 0x06);
      sl_sleeptimer_delay_millisecond(1000);
      glib_stop_scroll();
      break;
  }
}

/***************************************************************************//**
 * Callback on button change.
 *
 * This function overrides a weak implementation defined in the simple_button
 * module. It is triggered when the user activates one of the buttons.
 *
 ******************************************************************************/
#if (defined(SLI_SI917))
void sl_si91x_button_isr(uint8_t pin, int8_t state)
{
  if (pin == button_btn0.pin) {
    if (state == BUTTON_PRESSED) {
#else
void sl_button_on_change(const sl_button_t *handle)
{
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
    if (&sl_button_btn0 == handle) {
#endif
      if (display_mode < DISPLAY_MODE_COUNT) {
        display_mode++;
      } else {
        display_mode = 0;
      }
    }
  }
}
