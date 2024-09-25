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

#include <string.h>
#include "app_assert.h"
#include "sl_sleeptimer.h"
#include "adafruit_is31fl3741.h"
#include "glib.h"
#include "glib_font.h"

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

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

char *str = "Silicon Labs - Third Party Hardware Drivers Extension";
static glib_context_t glib_context;
static uint16_t color[] = { 0xf800, 0xffe0, 0x07e0, 0x07ff, 0x001f, 0xf81f,
                            0xffff, 0x73c0 };
static volatile bool app_timer_expire = false;
static sl_sleeptimer_timer_handle_t app_timer;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

// -----------------------------------------------------------------------------
//                       Local Functions
// -----------------------------------------------------------------------------

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);
static void app_task(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t ret;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  ret = adafruit_is31fl3741_init(app_i2c_instance);
  app_assert(ret == SL_STATUS_OK, "\rAdafruit RGB LED initialization fail\n");
  app_printf("\rAdafruit RGB LED initialization done\n");

  // GLIB initialization
  glib_init(&glib_context);
  glib_set_rotation(&glib_context, 0);
  glib_set_contrast(0xFF);
  glib_set_bg_color(&glib_context, 0x0000);
  glib_enable_wrap(&glib_context, false);
  glib_enable_display(true);
  app_printf("\rGLIB Initialization done\n");

  ret = sl_sleeptimer_start_periodic_timer_ms(&app_timer,
                                              50,
                                              app_timer_cb,
                                              NULL,
                                              0,
                                              0);
  app_assert(ret == SL_STATUS_OK, "\rStart displaying text fail\n");
  app_printf("\rStart displaying text\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (app_timer_expire == false) {
    return;
  }
  app_timer_expire = false;
  app_task();
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)data;
  (void)handle;

  app_timer_expire = true;
}

static void app_task(void)
{
  char *ptr = str;
  static int16_t cursor_x = 0;
  int length = (int)strlen(str) * 6 - 1;
  int i = 0;

  glib_set_cursor(&glib_context, cursor_x, 1);
  glib_fill(&glib_context, 0);
  while (*ptr) {
    glib_set_text_color(&glib_context, color[i++]);
    glib_write_char(&glib_context, *ptr++);
    if (i == 8) {
      i = 0;
    }
  }
  glib_update_display();
  if (cursor_x > -length) {
    cursor_x--;
  } else {
    cursor_x = glib_context.width;
  }
}
