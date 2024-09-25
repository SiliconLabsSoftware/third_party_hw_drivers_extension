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

static uint16_t color = ILI9341_YELLOW;
static uint8_t rotation = 0;

void main_screen_init(glib_context_t *g_context)
{
  glib_set_rotation(g_context, rotation);

  glib_fill(g_context, ILI9341_BLACK);
  glib_fill_rect(g_context, 0, g_context->height - 30, 30, 30, ILI9341_YELLOW);
  glib_fill_rect(g_context, 30, g_context->height - 30, 30, 30, ILI9341_BLUE);
  glib_fill_rect(g_context, 60, g_context->height - 30, 30, 30, ILI9341_GREEN);
  glib_fill_rect(g_context, 90, g_context->height - 30, 30, 30, ILI9341_RED);
  glib_fill_rect(g_context, 120, g_context->height - 30, 30, 30, ILI9341_WHITE);
  glib_fill_rect(g_context, 150, g_context->height - 30, 30, 30, ILI9341_PINK);
  glib_fill_rect(g_context, 180, g_context->height - 30, 30, 30,
                 ILI9341_PURPLE);
  glib_fill_rect(g_context, 210, g_context->height - 30, 30, 30,
                 ILI9341_ORANGE);

  glib_fill_rect(g_context, 0, 0, 10, 10, color);
  glib_draw_string(g_context, "CLR", g_context->width - 37, 5);
  glib_draw_rect(g_context, g_context->width - 40, 0, 40, 25, ILI9341_WHITE);
}

void main_screen_update(glib_context_t *g_context,
                        int32_t x, int32_t y)
{
  int temp;
  switch (rotation) {
    case 1:
      temp = x;
      x = y;
      y = g_context->height - temp;
      break;
    case 2:
      x = g_context->width - x;
      y = g_context->height - y;
      break;
    case 3:
      temp = x;
      x = g_context->width - y;
      y = g_context->height - temp;
      break;
    default:
      break;
  }
  if ((y >= (g_context->height - 30)) && (x < 240)) {
    if ((x >= 0) && (x < 30)) {
      color = ILI9341_YELLOW;
      glib_fill_rect(g_context, 0, 0, 10, 10, color);
    } else if ((x >= 30) && (x < 60)) {
      color = ILI9341_BLUE;
      glib_fill_rect(g_context, 0, 0, 10, 10, color);
    } else if ((x >= 60) && (x < 90)) {
      color = ILI9341_GREEN;
      glib_fill_rect(g_context, 0, 0, 10, 10, color);
    } else if ((x >= 90) && (x < 120)) {
      color = ILI9341_RED;
      glib_fill_rect(g_context, 0, 0, 10, 10, color);
    } else if ((x >= 120) && (x < 150)) {
      color = ILI9341_WHITE;
      glib_fill_rect(g_context, 0, 0, 10, 10, color);
    } else if ((x >= 150) && (x < 180)) {
      color = ILI9341_PINK;
      glib_fill_rect(g_context, 0, 0, 10, 10, color);
    } else if ((x >= 180) && (x < 210)) {
      color = ILI9341_PURPLE;
      glib_fill_rect(g_context, 0, 0, 10, 10, color);
    } else if ((x >= 210) && (x < 240)) {
      color = ILI9341_ORANGE;
      glib_fill_rect(g_context, 0, 0, 10, 10, color);
    }
  } else if ((x >= (g_context->width - 40)) && (y < 25)) {
    main_screen_init(g_context);
  } else {
    glib_draw_pixel(g_context, x, y, color);
  }
}
