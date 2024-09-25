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
#include "mikroe_ssd1351_image.h"
#include "mikroe_ssd1351.h"
#include "glib.h"
#include "app_assert.h"
#include "sl_sleeptimer.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_si91x_gspi.h"
static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#define app_printf(...) DEBUGOUT(__VA_ARGS__)

#else /* None Si91x device */
#include "sl_spidrv_instances.h"
#include "app_log.h"
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#define APP_STATE_DISPLAY_RECTANGLE   0
#define APP_STATE_DISPLAY_LINE        1
#define APP_STATE_DISPLAY_IMAGE       2

#define TIMEOUT_MSEC                  500

static glib_context_t glib_context;
static volatile uint8_t state = APP_STATE_DISPLAY_RECTANGLE;
static volatile bool timer_is_expire = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;
mikroe_spi_handle_t app_spi_instance = NULL;
static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);
static void app_task(void);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;

#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

  app_printf("---- Application Init ----\r\n");
  //  Click initialization.
  sc = mikroe_ssd1351_init(app_spi_instance);
  app_assert_status(sc);
  glib_init(&glib_context);
  glib_set_bg_color(&glib_context, 0xFFFF);
  glib_set_text_color(&glib_context, 0x0000);
  glib_enable_display(true);

  // demo with API of GLIB
  glib_clear(&glib_context);
  glib_fill(&glib_context, 0xFF00);
  glib_update_display();

  glib_fill(&glib_context, 0xFFFF);
  glib_update_display();

  glib_draw_string(&glib_context, "SILICON LABS", 15, 25);
  glib_draw_string(&glib_context, "DEMO", 35, 45);
  glib_draw_string(&glib_context, "OLED C CLICK", 10, 65);
  glib_update_display();

  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        TIMEOUT_MSEC,
                                        app_timer_cb,
                                        (void *) NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (timer_is_expire == false) {
    return;
  }
  timer_is_expire = false;
  app_task();
}

// Timer callback function
static void app_task(void)
{
  switch (state)
  {
    case APP_STATE_DISPLAY_RECTANGLE:
      mikroe_ssd1351_rectangle(0, 0, 96, 96, 0xF000);
      mikroe_ssd1351_rectangle(5, 5, 91, 91, 0xFF00);
      mikroe_ssd1351_rectangle(10, 10, 86, 86, 0x00F0);
      mikroe_ssd1351_rectangle(15, 15, 81, 81, 0x0F0F);
      mikroe_ssd1351_rectangle(20, 20, 76, 76, 0xF000);
      mikroe_ssd1351_rectangle(25, 25, 71, 71, 0xFF00);
      state = APP_STATE_DISPLAY_LINE;
      break;

    case APP_STATE_DISPLAY_LINE:
      mikroe_ssd1351_rectangle(25, 25, 71, 27, 0);
      mikroe_ssd1351_rectangle(25, 71, 71, 73, 0);
      mikroe_ssd1351_rectangle(25, 25, 27, 71, 0);
      mikroe_ssd1351_rectangle(68, 25, 71, 71, 0);
      state = APP_STATE_DISPLAY_IMAGE;
      break;

    case APP_STATE_DISPLAY_IMAGE:
      mikroe_ssd1351_fill_screen(0xFFFF);
      mikroe_ssd1351_image(aclogo, 0, 0);
      mikroe_ssd1351_image(mikroe_with_slogan, 0, 62);
      state = APP_STATE_DISPLAY_RECTANGLE;
      break;
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  timer_is_expire = true;
}
