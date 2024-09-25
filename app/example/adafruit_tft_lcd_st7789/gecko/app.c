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
#include "app_log.h"
#include "app_assert.h"
#include "glib.h"
#include "sl_sleeptimer.h"
#include "adafruit_st7789_spi_config.h"
#include "adafruit_st7789.h"

#define APP_TIMER_SMALL        500
#define APP_TIMER_MEDIUM       2000
#define APP_TIMER_LARGE        4000

static glib_context_t g_context;
static uint8_t demo_index = 0;

MIPI_DBI_SPI_INTERFACE_DEFINE(st7789_config,
                              ADAFRUIT_ST7789_PERIPHERAL,
                              ADAFRUIT_ST7789_PERIPHERAL_NO,
                              ADAFRUIT_ST7789_BITRATE,
                              ADAFRUIT_ST7789_CLOCK_MODE,
                              ADAFRUIT_ST7789_CS_CONTROL,
                              ADAFRUIT_ST7789_CLK_PORT,
                              ADAFRUIT_ST7789_CLK_PIN,
                              ADAFRUIT_ST7789_TX_PORT,
                              ADAFRUIT_ST7789_TX_PIN,
                              ADAFRUIT_ST7789_RX_PORT,
                              ADAFRUIT_ST7789_RX_PIN,
                              ADAFRUIT_ST7789_CS_PORT,
                              ADAFRUIT_ST7789_CS_PIN,
                              ADAFRUIT_ST7789_DC_PORT,
                              ADAFRUIT_ST7789_DC_PIN);

static volatile bool app_timer_expire = false;
static sl_sleeptimer_timer_handle_t app_sleep_timer;
static void app_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                    void *data);

extern const unsigned char gImage_cat_image[64808];
extern const unsigned char gImage_bird_image[64808];
extern const unsigned char gImage_cute_image[64808];
extern const unsigned char gImage_cactus_plants[64808];
extern const unsigned char gImage_nature_image[64808];

const uint8_t *img_arr[5] = {
  gImage_cat_image,
  gImage_bird_image,
  gImage_cute_image,
  gImage_cactus_plants,
  gImage_nature_image
};

void app_init(void)
{
  sl_status_t sc = SL_STATUS_OK;

  sc = adafruit_st7789_init(&st7789_config);
  app_assert_status(sc);

  adafruit_st7789_set_rotation(adafruit_st7789_rotation_none);

  sc = glib_init(&g_context);
  app_assert_status(sc);

  app_log("Adafruit ST7789 TFT init done\r\n");
  app_log("GLIB init done\r\n");

  sl_sleeptimer_restart_periodic_timer_ms(&app_sleep_timer,
                                          APP_TIMER_SMALL,
                                          app_sleeptimer_callback,
                                          NULL,
                                          0,
                                          0);
  app_log("Start sleep timer %dms\r\n", APP_TIMER_SMALL);

  g_context.textsize_x = 2;
  g_context.textsize_y = 2;
  g_context.wrap = false;

  glib_set_color(&g_context, ST7789_WHITE, ST7789_BLACK);
  glib_fill(&g_context, ST7789_BLACK);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (app_timer_expire) {
    app_timer_expire = false;

    switch (demo_index) {
      case 0:
        glib_fill(&g_context, ST7789_WHITE);
        app_log("glib_fill --> WHITE color\r\n");
        sl_sleeptimer_restart_periodic_timer_ms(&app_sleep_timer,
                                                APP_TIMER_SMALL,
                                                app_sleeptimer_callback,
                                                NULL,
                                                0,
                                                0);
        app_log("Start sleep timer %dms\r\n", APP_TIMER_SMALL);
        break;
      case 1:
        glib_fill(&g_context, ST7789_RED);
        app_log("glib_fill --> RED color\r\n");
        break;
      case 2:
        glib_fill(&g_context, ST7789_GREEN);
        app_log("glib_fill --> GREEN color\r\n");
        break;
      case 3:
        glib_fill(&g_context, ST7789_BLUE);
        app_log("glib_fill --> BLUE color\r\n");
        break;
      case 4:
        glib_fill(&g_context, ST7789_CYAN);
        app_log("glib_fill --> CYAN color\r\n");
        break;
      case 5:
        glib_fill(&g_context, ST7789_MAGENTA);
        app_log("glib_fill --> MAGENTA color\r\n");
        break;
      case 6:
        glib_fill(&g_context, ST7789_YELLOW);
        app_log("glib_fill --> YELLOW color\r\n");
        break;
      case 7:
        glib_fill(&g_context, ST7789_ORANGE);
        app_log("glib_fill --> ORANGE color\r\n");
        break;
      case 8:
        glib_fill(&g_context, ST7789_BLACK);
        app_log("glib_fill --> BLACK color\r\n");
        break;
      case 9:

        glib_set_color(&g_context, ST7789_GREEN, ST7789_BLACK);
        glib_draw_string(&g_context, "HELLO WORLD", 0, 0);
        glib_set_color(&g_context, ST7789_RED, ST7789_BLACK);
        glib_draw_string(&g_context, "Adafruit", 0, 25);
        glib_set_color(&g_context, ST7789_YELLOW, ST7789_BLACK);
        glib_draw_string(&g_context, "1.14\" TFT", 0, 50);

        app_log("glib_draw_string --> HELLO WORLD\r\n");
        app_log("glib_draw_string --> Adafruit\r\n");
        app_log("glib_draw_string --> 1.14\" TFT\r\n");

        sl_sleeptimer_restart_periodic_timer_ms(&app_sleep_timer,
                                                APP_TIMER_MEDIUM,
                                                app_sleeptimer_callback,
                                                NULL,
                                                0,
                                                0);
        app_log("Start sleep timer %dms\r\n", APP_TIMER_MEDIUM);
        break;
      case 10:
        glib_set_invert_color();
        app_log("glib_set_invert_color --> true\r\n");
        break;
      case 11:
        glib_set_normal_color();
        app_log("glib_set_invert_color --> false\r\n");
        break;
      case 12:
        glib_enable_display(false);
        app_log("glib_enable_display --> false\r\n");
        break;
      case 13:
        glib_enable_display(true);
        app_log("glib_enable_display --> true\r\n");
        break;
      case 14:
        adafruit_st7789_draw_rgb_bitmap(0, 0, (uint16_t *)img_arr[0], 135, 240);
        app_log("adafruit_st7789_draw_rgb_bitmap idx = 0\r\n");
        sl_sleeptimer_restart_periodic_timer_ms(&app_sleep_timer,
                                                APP_TIMER_LARGE,
                                                app_sleeptimer_callback,
                                                NULL,
                                                0,
                                                0);
        app_log("Start sleep timer %dms\r\n", APP_TIMER_LARGE);
        break;
      case 15:
        adafruit_st7789_draw_rgb_bitmap(0, 0, (uint16_t *)img_arr[1], 135, 240);
        app_log("adafruit_st7789_draw_rgb_bitmap idx = 1\r\n");
        break;
      case 16:
        adafruit_st7789_draw_rgb_bitmap(0, 0, (uint16_t *)img_arr[2], 135, 240);
        app_log("adafruit_st7789_draw_rgb_bitmap idx = 2\r\n");
        break;
      case 17:
        adafruit_st7789_draw_rgb_bitmap(0, 0, (uint16_t *)img_arr[3], 135, 240);
        app_log("adafruit_st7789_draw_rgb_bitmap idx = 3\r\n");
        break;
      case 18:
        adafruit_st7789_draw_rgb_bitmap(0, 0, (uint16_t *)img_arr[4], 135, 240);
        app_log("adafruit_st7789_draw_rgb_bitmap idx = 4\r\n");
        break;
      case 19:
        glib_fill(&g_context, ST7789_BLACK);

        glib_draw_circle(&g_context, 25, 25, 25, ST7789_MAGENTA);
        glib_fill_circle(&g_context, 90, 25, 25, ST7789_BLUE);
        glib_draw_triangle(&g_context, 80, 60, 130, 60, 130, 90, ST7789_YELLOW);
        glib_fill_triangle(&g_context, 0, 60, 80, 80, 45, 116, ST7789_ORANGE);
        glib_draw_round_rect(&g_context, 0, 125, 50, 65, 10, ST7789_CYAN);
        glib_fill_rect(&g_context, 70, 125, 50, 65, ST7789_GREEN);
        app_log("glib_draw_circle, glib_fill_circle\r\n");
        app_log("glib_draw_triangle, glib_fill_triangle\r\n");
        app_log("glib_draw_round_rect, glib_fill_rect\r\n");
        break;
      default:
        break;
    }

    if (demo_index++ > 19) {
      demo_index = 0;
    }
  }
}

static void app_sleeptimer_callback(sl_sleeptimer_timer_handle_t *timer,
                                    void *data)
{
  (void)timer;
  (void)data;

  app_timer_expire = true;
}
