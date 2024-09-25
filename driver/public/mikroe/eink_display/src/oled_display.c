/***************************************************************************//**
 * @file oled_display.c
 * @brief OLED Driver API interface
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include <string.h>
#include "mikroe_eink_display_config.h"
#include "mikroe_eink_display.h"
#include "oled_display.h"

/* This oled_frame_buffer is large enough to store one full frame. */
static uint8_t oled_frame_buffer[(MIKROE_EINK_DISPLAY_WIDTH
                                  * MIKROE_EINK_DISPLAY_HEIGHT) / 8];

static sl_status_t driver_init(void);
static sl_status_t draw_pixel(int16_t x, int16_t y, uint16_t color);
static sl_status_t fill_screen(uint16_t color);
static sl_status_t update_display(void);

static oled_display_t oled_display_instance;
static const oled_display_driver_api_t sl_memlcd_driver_api =
{
  .init = driver_init,
  .draw_pixel = draw_pixel,
  .get_raw_pixel = NULL,
  .fill_screen = fill_screen,
  .update_display = update_display,
  .enable_display = NULL,
  .set_invert_color = NULL,
  .set_normal_color = NULL,
  .set_contrast = NULL,
  .scroll_right = NULL,
  .scroll_left = NULL,
  .scroll_diag_right = NULL,
  .scroll_diag_left = NULL,
  .stop_scroll = NULL,
};

/** Flag to monitor is this driver has been initialized. The
 *   oled_display_instance
 *  is only valid after initialized=true. */
static bool initialized = false;

sl_status_t oled_display_init(void)
{
  oled_display_instance.width = MIKROE_EINK_DISPLAY_WIDTH;
  oled_display_instance.height = MIKROE_EINK_DISPLAY_HEIGHT;
  oled_display_instance.driver = &sl_memlcd_driver_api;
  initialized = true;
  return SL_STATUS_OK;
}

static sl_status_t draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  uint16_t off;
  uint16_t pos;

  if ((x >= 0) && (x < MIKROE_EINK_DISPLAY_WIDTH) && (y >= 0)
      && (y < MIKROE_EINK_DISPLAY_HEIGHT)) {
    pos = (y * (MIKROE_EINK_DISPLAY_WIDTH / 8)) + (x / 8);
    off = 7 - (x % 8);
    oled_frame_buffer[pos] &= ~(0x01 << off);
    oled_frame_buffer[pos] |= ((color & 0x01) << off);
  }
  return SL_STATUS_OK;
}

static sl_status_t fill_screen(uint16_t color)
{
  uint16_t i;

  /* Fill the display with the background color of the glib_context_t  */
  for (i = 0; i < sizeof(oled_frame_buffer); i++) {
    oled_frame_buffer[i] = (uint8_t)color;
  }
  return SL_STATUS_OK;
}

static sl_status_t update_display(void)
{
  eink_display_xy_t xy;

  xy.x_start = 0;
  xy.y_start = 0;
  xy.x_end = MIKROE_EINK_DISPLAY_WIDTH - 1;
  xy.y_end = MIKROE_EINK_DISPLAY_HEIGHT - 1;

  mikroe_eink_display_set_memory_area(&xy);
  mikroe_eink_display_set_memory_pointer(0, 0);
  mikroe_eink_display_send_command(EINK_DISPLAY_CMD_WRITE_RAM);

  for (uint16_t i = 0; i < sizeof(oled_frame_buffer); i++ )
  {
    mikroe_eink_display_send_data(oled_frame_buffer[i]);
  }
  return mikroe_eink_display_update_display();
}

const oled_display_t *oled_display_get(void)
{
  if (initialized) {
    return &oled_display_instance;
  } else {
    return NULL;
  }
}

static sl_status_t driver_init(void)
{
  return SL_STATUS_OK;
}
