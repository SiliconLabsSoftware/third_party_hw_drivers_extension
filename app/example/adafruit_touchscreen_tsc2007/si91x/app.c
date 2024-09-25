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
#include "app_assert.h"
#include "rsi_debug.h"
#include "sl_si91x_i2c.h"
#include "adafruit_ili9341.h"
#include "adafruit_tsc2007.h"
#include "adafruit_ili9341_spi_config.h"
#include "glib.h"

#define PRESSURE_THRESH                 500

static glib_context_t g_context;

static sl_i2c_instance_t i2c_instance = SL_I2C2;
static const struct adafruit_tsc2007_config adafruit_tsc2007_config = {
  .mikroe_drv_i2c_handle = (void *)&i2c_instance
};

MIPI_DBI_SPI_INTERFACE_DEFINE(ili9341_config,
                              ADAFRUIT_ILI9341_BITRATE,
                              ADAFRUIT_ILI9341_CLOCK_MODE,
                              ADAFRUIT_ILI9341_CS_CONTROL,
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
                              ADAFRUIT_ILI9341_DC_PORT,
                              ADAFRUIT_ILI9341_DC_PIN,
                              SL_GPIO_MODE_0);

static void screen_print(
  int16_t glib_y_ofset,
  int32_t x, int32_t y, int32_t rtouch,
  uint16_t x_raw, uint16_t y_raw, uint16_t z1_raw, uint16_t z2_raw);

void app_init(void)
{
  sl_status_t sc;

  sc = adafruit_ili9341_init(&ili9341_config);
  app_assert_status(sc);
  DEBUGOUT("\rAdafruit ILI9341 Init done\n");

  sc = adafruit_tsc2007_init(&adafruit_tsc2007_config);
  app_assert_status(sc);
  DEBUGOUT("\rTouch Screen Init done\n");

  glib_init(&g_context);
  app_assert_status(sc);
  DEBUGOUT("\rGLIB Init done\n");
  glib_set_bg_color(&g_context, ILI9341_BLACK);
  glib_set_text_color(&g_context, ILI9341_WHITE);
  glib_set_text_size(&g_context, 2, 2);
  glib_enable_wrap(&g_context, false);

  glib_fill(&g_context, ILI9341_BLACK);
  glib_draw_string(&g_context, "~TSC2007~", 65, 10);
  screen_print(40,
               0, 0, 0,
               0, 0, 0, 0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  int32_t x, y, rtouch;
  uint16_t x_raw, y_raw, z1_raw, z2_raw;

  adafruit_tsc2007_read_touch();
  adafruit_tsc2007_get_touch(&x_raw, &y_raw, &z1_raw, &z2_raw);
  adafruit_tsc2007_get_touch_point(&x, &y, &rtouch);
  DEBUGOUT("x: %ld, y: %ld, rtouch: %ld\r\n", x, y, rtouch);
  if (rtouch < PRESSURE_THRESH) {
    screen_print(40,
                 x, y, rtouch,
                 x_raw, y_raw, z1_raw, z2_raw);
  }
}

static void screen_print(
  int16_t glib_y_ofset,
  int32_t x, int32_t y, int32_t rtouch,
  uint16_t x_raw, uint16_t y_raw, uint16_t z1_raw, uint16_t z2_raw)
{
  char string_buf[32];

  snprintf(string_buf, sizeof(string_buf), "X:  %3ld  ", x);
  glib_draw_string(&g_context, string_buf, 0, glib_y_ofset);
  snprintf(string_buf, sizeof(string_buf), "X_RAW:  %4d  ", x_raw);
  glib_draw_string(&g_context, string_buf, 95, glib_y_ofset);

  snprintf(string_buf, sizeof(string_buf), "Y:  %3ld", y);
  glib_draw_string(&g_context, string_buf, 0, glib_y_ofset + 20);
  snprintf(string_buf, sizeof(string_buf), "Y_RAW:  %4d", y_raw);
  glib_draw_string(&g_context, string_buf, 95, glib_y_ofset + 20);

  snprintf(string_buf, sizeof(string_buf), "RT: %3ld  ", rtouch);
  glib_draw_string(&g_context, string_buf, 0, glib_y_ofset + 40);
  snprintf(string_buf, sizeof(string_buf), "Z1_RAW: %4d  ", z1_raw);
  glib_draw_string(&g_context, string_buf, 95, glib_y_ofset + 40);
  snprintf(string_buf, sizeof(string_buf), "Z2_RAW: %4d  ", z2_raw);
  glib_draw_string(&g_context, string_buf, 95, glib_y_ofset + 60);
}
