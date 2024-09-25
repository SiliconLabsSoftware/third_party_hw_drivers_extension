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
#include "glib.h"
#include "adafruit_ili9341.h"

#define Y_OFFSET  (40)

void calib_screen_update(
  glib_context_t *g_context,
  int32_t x, int32_t y, int32_t rtouch,
  uint16_t x_raw, uint16_t y_raw, uint16_t z1_raw, uint16_t z2_raw)
{
  char string_buf[32];

  snprintf(string_buf, sizeof(string_buf), "X:  %3ld  ", x);
  glib_draw_string(g_context, string_buf, 0, Y_OFFSET + 20);
  snprintf(string_buf, sizeof(string_buf), "X_RAW:  %4d  ", x_raw);
  glib_draw_string(g_context, string_buf, 95, Y_OFFSET + 20);

  snprintf(string_buf, sizeof(string_buf), "Y:  %3ld", y);
  glib_draw_string(g_context, string_buf, 0, Y_OFFSET + 40);
  snprintf(string_buf, sizeof(string_buf), "Y_RAW:  %4d", y_raw);
  glib_draw_string(g_context, string_buf, 95, Y_OFFSET + 40);

  snprintf(string_buf, sizeof(string_buf), "RT: %3ld  ", rtouch);
  glib_draw_string(g_context, string_buf, 0, Y_OFFSET + 60);
  snprintf(string_buf, sizeof(string_buf), "Z1_RAW: %4d  ", z1_raw);
  glib_draw_string(g_context, string_buf, 95, Y_OFFSET + 60);
  snprintf(string_buf, sizeof(string_buf), "Z2_RAW: %4d  ", z2_raw);
  glib_draw_string(g_context, string_buf, 95, Y_OFFSET + 80);
}

void calib_screen_init(glib_context_t *g_context)
{
  glib_fill(g_context, ILI9341_BLACK);
  glib_draw_string(g_context, "~TOUCH SCREEN CALIB~", 0, Y_OFFSET);
  glib_fill_rect(g_context, 0, 0, 10, 10,
                 ILI9341_GREEN);
  glib_fill_rect(g_context,
                 g_context->width - 10,
                 g_context->height - 10,
                 10, 10,
                 ILI9341_YELLOW);
  calib_screen_update(g_context,
                      0, 0, 0,
                      0, 0, 0, 0);
}
