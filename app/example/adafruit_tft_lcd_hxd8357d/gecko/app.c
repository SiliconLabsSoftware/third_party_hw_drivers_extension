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
#include "adafruit_hxd8357d.h"
#include "adafruit_hxd8357d_spi_config.h"
#include "touch_screen.h"
#include "glib.h"

#define CALIB_ENABLED                   0
#define PRESSURE_THRESH                 500

void main_screen_init(glib_context_t *g_context);
void main_screen_update(glib_context_t *g_context,
                        int32_t x, int32_t y);

void calib_screen_init(glib_context_t *g_context);
void calib_screen_update(
  glib_context_t *g_context,
  int32_t x, int32_t y, int32_t rtouch,
  uint16_t x_raw, uint16_t y_raw, uint16_t z1_raw, uint16_t z2_raw);

struct touch_screen touch_screen;
static touch_point_t point;
static glib_context_t g_context;

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

void app_init(void)
{
  sl_status_t sc;

  sc = adafruit_hxd8357d_init(&hx8357d_config);
  app_assert_status(sc);
  app_log("\rAdafruit HXD8357D Init done\n");

  sc = touch_screen_init(&touch_screen);
  app_assert_status(sc);
  app_log("\rTouch Screen Init done\n");

  glib_init(&g_context);
  app_assert_status(sc);
  app_log("\rGLIB Init done\n");
  g_context.bg_color = HXD8357D_BLACK;
  g_context.textsize_x = 2;
  g_context.textsize_y = 2;
  g_context.text_color = HXD8357D_WHITE;
  g_context.wrap = false;

#if CALIB_ENABLED
  calib_screen_init(&g_context);
#else
  main_screen_init(&g_context);
#endif
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  touch_screen_get_point(&touch_screen, &point);
  if (point.r_touch < PRESSURE_THRESH) {
#if CALIB_ENABLED
    calib_screen_update(&g_context,
                        point.x, point.y, point.r_touch,
                        touch_screen.t_x, touch_screen.t_y,
                        touch_screen.t_z1, touch_screen.t_z2);
#else
    main_screen_update(&g_context, point.x, point.y);
#endif
  }
}
