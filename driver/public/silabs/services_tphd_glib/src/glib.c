/***************************************************************************//**
 * @file glib.c
 * @brief Silicon Labs Graphics Library
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

/* Standard C header files */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "glib.h"
#include "sl_sleeptimer.h"
#include "oled_display.h"

#define swap_int16_t(a, b) \
  {                        \
    int16_t t = a;         \
    a = b;                 \
    b = t;                 \
  }
#define draw_vline(g, x, y, h, color) \
  glib_draw_line(g, x, y, x, y + h - 1, color)

#define draw_hline(g, x, y, w, color) \
  glib_draw_line(g, x, y, x + w - 1, y, color);

#define write_string(ptr)  { while (*ptr) {                                   \
                               glib_write_char(shift_data.context, *ptr++); } \
                             glib_update_display(); }                         \

typedef struct {
  glib_context_t *context;
  const char *data;
  glib_shifting_direction_t direction;
  uint32_t rate_ms;
  uint32_t ticks;
  uint16_t string_length;
  uint8_t max_char_height;
  uint8_t addition_pixel;
  int16_t y;
} glib_shift_string_data_t;

const oled_display_t *oled_display = NULL;
static glib_shift_string_data_t shift_data;

/***************************************************************************//**
 *     @brief
 *       Initialization function for the glib.
 *
 *     @return
 *      Returns GLIB_OK on success, or else error code
 *******************************************************************************/
glib_status_t glib_init(glib_context_t *g_context)
{
  sl_status_t sc;

  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  oled_display_init();

  if (oled_display == NULL) {
    oled_display = oled_display_get();
    sc = oled_display->driver->init();
    if (SL_STATUS_OK != sc) {
      return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
    }
  }

  g_context->font = NULL;
  g_context->width = oled_display->width;
  g_context->height = oled_display->height;
  g_context->cursor_y = g_context->cursor_x = 0;
  g_context->textsize_x = g_context->textsize_y = 1;
  g_context->rotation = 0;
  g_context->text_color = g_context->bg_color = 0xFFFF;
  g_context->wrap = true;
  g_context->cp437 = false;

  return GLIB_OK;
}

/***************************************************************************//**
 *  @brief
 *  Clears the display with the background color of the glib_context_t
 *
 *  @param g_context
 *  Pointer to a GLIB_Context_t which holds the background color.
 *
 *  @return
 *  Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_clear(glib_context_t *g_context)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  if (SL_STATUS_OK == oled_display->driver->fill_screen(0)) {
    return GLIB_OK;
  }
  return GLIB_ERROR_IO;
}

/***************************************************************************//**
 *  @brief      Set rotation setting for display
 *  @param  x   0 thru 3 corresponding to 4 cardinal rotations
 ******************************************************************************/
glib_status_t glib_set_rotation(glib_context_t *g_context, uint8_t x)
{
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  g_context->rotation = (x & 3);
  switch (g_context->rotation) {
    case 0:
    case 2:
      g_context->width = oled_display->width;
      g_context->height = oled_display->height;
      break;
    case 1:
    case 3:
      g_context->width = oled_display->height;
      g_context->height = oled_display->width;
      break;
  }
  return GLIB_OK;
}

/***************************************************************************//**
 *  @brief
 *  Fill the display with the color
 *
 *  @param g_context
 *  Pointer to a GLIB_Context_t which holds the background color.
 *
 *  @return
 *  Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_fill(glib_context_t *g_context, uint16_t color)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  if (SL_STATUS_OK == oled_display->driver->fill_screen(color)) {
    return GLIB_OK;
  }
  return GLIB_ERROR_IO;
}

/***************************************************************************//**
 *  @brief
 *  Draws a pixel at x, y using text_color defined in the glib_context_t.
 *
 *  @param g_context
 *  Pointer to a glib_context_t which holds the foreground color.
 *  @param x
 *  X-coordinate
 *  @param y
 *  Y-coordinate
 *
 *  @return
 *  Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_draw_pixel(glib_context_t *g_context,
                              int16_t x,
                              int16_t y,
                              uint16_t color)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  if ((x < 0) || (y < 0)
      || (x >= g_context->width) || (y >= g_context->height)) {
    return GLIB_OUT_OF_BOUNDS;
  }

  int16_t t;
  switch (g_context->rotation) {
    case 1:
      t = x;
      x = oled_display->width - 1 - y;
      y = t;
      break;
    case 2:
      x = oled_display->width - 1 - x;
      y = oled_display->height - 1 - y;
      break;
    case 3:
      t = x;
      x = y;
      y = oled_display->height - 1 - t;
      break;
  }

  if (SL_STATUS_OK == oled_display->driver->draw_pixel(x, y, color)) {
    return GLIB_OK;
  }
  return GLIB_ERROR_IO;
}

/***************************************************************************//**
 *  @brief
 *  Draws a pixel at x, y using text_color defined in the glib_context_t.
 *
 *  @param g_context
 *  Pointer to a glib_context_t which holds the foreground color.
 *  @param x
 *  X-coordinate
 *  @param y
 *  Y-coordinate
 *
 *  @return
 *  Returns GLIB_OK on success, or else error code
 ******************************************************************************/
uint16_t glib_get_pixel(glib_context_t *g_context, int16_t x, int16_t y)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  if ((x < 0) || (y < 0)
      || (x >= g_context->width) || (y >= g_context->height)) {
    return GLIB_OUT_OF_BOUNDS;
  }

  int16_t t;
  switch (g_context->rotation) {
    case 1:
      t = x;
      x = oled_display->width - 1 - y;
      y = t;
      break;
    case 2:
      x = oled_display->width - 1 - x;
      y = oled_display->height - 1 - y;
      break;
    case 3:
      t = x;
      x = y;
      y = oled_display->height - 1 - t;
      break;
  }
  return oled_display->driver->get_raw_pixel(x, y);
}

/***************************************************************************//**
 *    @brief    Write a line.  Bresenham's algorithm - thx wikpedia
 *     @param    x0  Start point x coordinate
 *     @param    y0  Start point y coordinate
 *     @param    x1  End point x coordinate
 *     @param    y1  End point y coordinate
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_line(glib_context_t *g_context,
                             int16_t x0, int16_t y0,
                             int16_t x1, int16_t y1,
                             uint16_t color)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap_int16_t(x0, y0);
    swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    swap_int16_t(x0, x1);
    swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      glib_draw_pixel(g_context, y0, x0, color);
    } else {
      glib_draw_pixel(g_context, x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
  return GLIB_OK;
}

/***************************************************************************//**
 *    @brief    Draw a circle outline
 *     @param    x0   Center-point x coordinate
 *     @param    y0   Center-point y coordinate
 *     @param    r   Radius of circle
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_circle(glib_context_t *g_context,
                               int16_t x0, int16_t y0, int16_t r,
                               uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  glib_draw_pixel(g_context, x0, y0 + r, color);
  glib_draw_pixel(g_context, x0, y0 - r, color);
  glib_draw_pixel(g_context, x0 + r, y0, color);
  glib_draw_pixel(g_context, x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    glib_draw_pixel(g_context, x0 + x, y0 + y, color);
    glib_draw_pixel(g_context, x0 - x, y0 + y, color);
    glib_draw_pixel(g_context, x0 + x, y0 - y, color);
    glib_draw_pixel(g_context, x0 - x, y0 - y, color);
    glib_draw_pixel(g_context, x0 + y, y0 + x, color);
    glib_draw_pixel(g_context, x0 - y, y0 + x, color);
    glib_draw_pixel(g_context, x0 + y, y0 - x, color);
    glib_draw_pixel(g_context, x0 - y, y0 - x, color);
  }
  return GLIB_OK;
}

/***************************************************************************//**
 *     @brief    Quarter-circle drawer, used to do circles and roundrects
 *     @param    x0   Center-point x coordinate
 *     @param    y0   Center-point y coordinate
 *     @param    r   Radius of circle
 *     @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of
 *    the circle we're doing
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_circle_helper(glib_context_t *g_context,
                                      int16_t x0, int16_t y0, int16_t r,
                                      uint8_t cornername, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4) {
      glib_draw_pixel(g_context, x0 + x, y0 + y, color);
      glib_draw_pixel(g_context, x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      glib_draw_pixel(g_context, x0 + x, y0 - y, color);
      glib_draw_pixel(g_context, x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      glib_draw_pixel(g_context, x0 - y, y0 + x, color);
      glib_draw_pixel(g_context, x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      glib_draw_pixel(g_context, x0 - y, y0 - x, color);
      glib_draw_pixel(g_context, x0 - x, y0 - y, color);
    }
  }
  return GLIB_OK;
}

/***************************************************************************//**
 *     @brief  Quarter-circle drawer with fill, used for circles and roundrects
 *     @param  x0       Center-point x coordinate
 *     @param  y0       Center-point y coordinate
 *     @param  r        Radius of circle
 *     @param  corners  Mask bits indicating which quarters we're doing
 *     @param  delta    Offset from center-point, used for round-rects
 *     @param  color    16-bit 5-6-5 Color to fill with
 ******************************************************************************/
glib_status_t glib_fill_circle_helper(glib_context_t *g_context,
                                      int16_t x0, int16_t y0, int16_t r,
                                      uint8_t corners, int16_t delta,
                                      uint16_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  delta++; // Avoid some +1's in the loop

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // These checks avoid double-drawing certain lines, important
    // for the SSD1306 library which has an INVERT drawing mode.
    if (x < (y + 1)) {
      if (corners & 1) {
        draw_vline(g_context, x0 + x, y0 - y, 2 * y + delta, color);
      }
      if (corners & 2) {
        draw_vline(g_context, x0 - x, y0 - y, 2 * y + delta, color);
      }
    }
    if (y != py) {
      if (corners & 1) {
        draw_vline(g_context, x0 + py, y0 - px, 2 * px + delta, color);
      }
      if (corners & 2) {
        draw_vline(g_context, x0 - py, y0 - px, 2 * px + delta, color);
      }
      py = y;
    }
    px = x;
  }
  return GLIB_OK;
}

/***************************************************************************//**
 *    @brief    Draw a circle with filled color
 *     @param    x0   Center-point x coordinate
 *     @param    y0   Center-point y coordinate
 *     @param    r   Radius of circle
 *     @param    color 16-bit 5-6-5 Color to fill with
 ******************************************************************************/
glib_status_t glib_fill_circle(glib_context_t *g_context,
                               int16_t x0, int16_t y0, int16_t r,
                               uint16_t color)
{
  glib_status_t status = GLIB_OK;

  status |= draw_vline(g_context,
                       x0,
                       y0 > r ? (y0 - r) : 0,
                       2 * r + 1,
                       color);
  status |= glib_fill_circle_helper(g_context, x0, y0, r, 3, 0, color);
  return status;
}

/***************************************************************************//**
 *    @brief   Draw a rectangle with no fill color
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    w   Width in pixels
 *     @param    h   Height in pixels
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_rect(glib_context_t *g_context,
                             int16_t x, int16_t y, int16_t w, int16_t h,
                             uint16_t color)
{
  glib_status_t status = GLIB_OK;

  status |= draw_hline(g_context, x, y, w, color);
  status |= draw_hline(g_context, x, y + h - 1, w, color);
  status |= draw_vline(g_context, x, y, h, color);
  status |= draw_vline(g_context, x + w - 1, y, h, color);
  return status;
}

/***************************************************************************//**
 *    @brief    Fill a rectangle completely with one color. Update in subclasses
 *   if
 *    desired!
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    w   Width in pixels
 *     @param    h   Height in pixels
 *    @param    color 16-bit 5-6-5 Color to fill with
 ******************************************************************************/
glib_status_t glib_fill_rect(glib_context_t *g_context,
                             int16_t x, int16_t y,
                             int16_t w, int16_t h,
                             uint16_t color)
{
  glib_status_t status = GLIB_OK;

  for (int16_t i = x; i < x + w; i++) {
    status |= draw_vline(g_context, i, y, h, color);
  }
  return status;
}

/***************************************************************************//**
 *    @brief   Draw a rounded rectangle with no fill color
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    w   Width in pixels
 *     @param    h   Height in pixels
 *     @param    r   Radius of corner rounding
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_round_rect(glib_context_t *g_context,
                                   int16_t x, int16_t y, int16_t w, int16_t h,
                                   int16_t r, uint16_t color)
{
  glib_status_t status = GLIB_OK;
  int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
  if (r > max_radius) {
    r = max_radius;
  }
  // smarter version
  status |= draw_hline(g_context, x + r, y, w - 2 * r, color);         // Top
  status |= draw_hline(g_context, x + r, y + h - 1, w - 2 * r, color); // Bottom
  status |= draw_vline(g_context, x, y + r, h - 2 * r, color);         // Left
  status |= draw_vline(g_context, x + w - 1, y + r, h - 2 * r, color); // Right
  // draw four corners
  status |= glib_draw_circle_helper(g_context,
                                    x + r, y + r,
                                    r,
                                    1,
                                    color);
  status |= glib_draw_circle_helper(g_context,
                                    x + w - r - 1, y + r,
                                    r,
                                    2,
                                    color);
  status |= glib_draw_circle_helper(g_context,
                                    x + w - r - 1, y + h - r - 1,
                                    r,
                                    4,
                                    color);
  status |= glib_draw_circle_helper(g_context,
                                    x + r, y + h - r - 1,
                                    r,
                                    8,
                                    color);
  return status;
}

/***************************************************************************//**
 *    @brief   Draw a rounded rectangle with fill color
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    w   Width in pixels
 *     @param    h   Height in pixels
 *     @param    r   Radius of corner rounding
 *     @param    color 16-bit 5-6-5 Color to draw/fill with
 ******************************************************************************/
glib_status_t glib_fill_round_rect(glib_context_t *g_context,
                                   int16_t x, int16_t y, int16_t w, int16_t h,
                                   int16_t r, uint16_t color)
{
  glib_status_t status = GLIB_OK;
  int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
  if (r > max_radius) {
    r = max_radius;
  }
  // smarter version
  status |= glib_fill_rect(g_context,
                           x + r, y,
                           w - 2 * r, h,
                           color);
  // draw four corners
  status |= glib_fill_circle_helper(g_context,
                                    x + w - r - 1, y + r,
                                    r,
                                    1,
                                    h - 2 * r - 1,
                                    color);
  status |= glib_fill_circle_helper(g_context,
                                    x + r, y + r,
                                    r,
                                    2,
                                    h - 2 * r - 1,
                                    color);
  return status;
}

/***************************************************************************//**
 *    @brief   Draw a triangle with no fill color
 *     @param    x0  Vertex #0 x coordinate
 *     @param    y0  Vertex #0 y coordinate
 *     @param    x1  Vertex #1 x coordinate
 *     @param    y1  Vertex #1 y coordinate
 *     @param    x2  Vertex #2 x coordinate
 *     @param    y2  Vertex #2 y coordinate
 *     @param    color 16-bit 5-6-5 Color to draw with
 ******************************************************************************/
glib_status_t glib_draw_triangle(glib_context_t *g_context,
                                 int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                 int16_t x2, int16_t y2, uint16_t color)
{
  glib_status_t status = GLIB_OK;

  status |= glib_draw_line(g_context, x0, y0, x1, y1, color);
  status |= glib_draw_line(g_context, x1, y1, x2, y2, color);
  status |= glib_draw_line(g_context, x2, y2, x0, y0, color);
  return status;
}

/***************************************************************************//**
 *    @brief     Draw a triangle with color-fill
 *     @param    x0  Vertex #0 x coordinate
 *     @param    y0  Vertex #0 y coordinate
 *     @param    x1  Vertex #1 x coordinate
 *     @param    y1  Vertex #1 y coordinate
 *     @param    x2  Vertex #2 x coordinate
 *     @param    y2  Vertex #2 y coordinate
 *     @param    color 16-bit 5-6-5 Color to fill/draw with
 ******************************************************************************/
glib_status_t glib_fill_triangle(glib_context_t *g_context,
                                 int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                 int16_t x2, int16_t y2, uint16_t color)
{
  glib_status_t status = GLIB_OK;
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap_int16_t(y0, y1);
    swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    swap_int16_t(y2, y1);
    swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    swap_int16_t(y0, y1);
    swap_int16_t(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a) {
      a = x1;
    } else if (x1 > b) {
      b = x1;
    }
    if (x2 < a) {
      a = x2;
    } else if (x2 > b) {
      b = x2;
    }
    return draw_hline(g_context, a, y0, b - a + 1, color);
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2) {
    last = y1; // Include y1 scanline
  } else {
    last = y1 - 1; // Skip it
  }
  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    /* longhand:
     *    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
     *    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
     */
    if (a > b) {
      swap_int16_t(a, b);
    }
    status |= draw_hline(g_context, a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    /* longhand:
     *    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
     *    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
     */
    if (a > b) {
      swap_int16_t(a, b);
    }
    status |= draw_hline(g_context, a, y, b - a + 1, color);
  }
  return status;
}

// BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------

/***************************************************************************//**
 *    @brief      Draw a RAM-resident 1-bit image at the specified (x,y)
 *   position,
 *    using the specified foreground (for set bits) and background (unset bits)
 *    colors.
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    bitmap  byte array with monochrome bitmap
 *     @param    w   Width of bitmap in pixels
 *     @param    h   Height of bitmap in pixels
 *     @param    color 16-bit 5-6-5 Color to draw pixels with
 *     @param    bg 16-bit 5-6-5 Color to draw background with
 ******************************************************************************/
glib_status_t glib_draw_bitmap(glib_context_t *g_context,
                               int16_t x, int16_t y, uint8_t *bitmap, int16_t w,
                               int16_t h, uint16_t color, uint16_t bg)
{
  glib_status_t status = GLIB_OK;
  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7) {
        b <<= 1;
      } else {
        b = bitmap[j * byteWidth + i / 8];
      }
      status |= glib_draw_pixel(g_context, x + i, y, (b & 0x80) ? color : bg);
    }
  }
  return status;
}

/***************************************************************************//**
 *    @brief      Draw PROGMEM-resident XBitMap Files (*.xbm), exported from
 *   GIMP.
 *    Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
 *    C Array can be directly used with this function.
 *    There is no RAM-resident version of this function; if generating bitmaps
 *    in RAM, use the format defined by drawBitmap() and call that instead.
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    bitmap  byte array with monochrome bitmap
 *     @param    w   Width of bitmap in pixels
 *     @param    h   Height of bitmap in pixels
 *     @param    color 16-bit 5-6-5 Color to draw pixels with
 ******************************************************************************/
glib_status_t glib_draw_xbitmap(glib_context_t *g_context,
                                int16_t x, int16_t y, const uint8_t *bitmap,
                                int16_t w, int16_t h, uint16_t color)
{
  glib_status_t status = GLIB_OK;
  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7) {
        b >>= 1;
      } else {
        b = bitmap[j * byteWidth + i / 8];
      }
      // Nearly identical to drawBitmap(), only the bit order
      // is reversed here (left-to-right = LSB to MSB):
      if (b & 0x01) {
        status |= glib_draw_pixel(g_context, x + i, y, color);
      }
    }
  }
  return status;
}

/***************************************************************************//**
 *    @brief   Draw a RAM-resident 8-bit image (grayscale) at the specified
 *   (x,y)
 *    pos. Specifically for 8-bit display devices such as IS31FL3731; no color
 *    reduction/expansion is performed.
 *     @param    x   Top left corner x coordinate
 *     @param    y   Top left corner y coordinate
 *     @param    bitmap  byte array with grayscale bitmap
 *     @param    w   Width of bitmap in pixels
 *     @param    h   Height of bitmap in pixels
 ******************************************************************************/
glib_status_t glib_draw_grayscale_bitmap(glib_context_t *g_context,
                                         int16_t x, int16_t y, uint8_t *bitmap,
                                         int16_t w, int16_t h)
{
  glib_status_t status = GLIB_OK;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      status |= glib_draw_pixel(g_context, x + i, y, bitmap[j * w + i]);
    }
  }
  return status;
}

// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

// Standard ASCII 5x7 font
static const unsigned char font_5x7[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x5B, 0x4F, 0x5B, 0x3E, 0x3E, 0x6B,
  0x4F, 0x6B, 0x3E, 0x1C, 0x3E, 0x7C, 0x3E, 0x1C, 0x18, 0x3C, 0x7E, 0x3C,
  0x18, 0x1C, 0x57, 0x7D, 0x57, 0x1C, 0x1C, 0x5E, 0x7F, 0x5E, 0x1C, 0x00,
  0x18, 0x3C, 0x18, 0x00, 0xFF, 0xE7, 0xC3, 0xE7, 0xFF, 0x00, 0x18, 0x24,
  0x18, 0x00, 0xFF, 0xE7, 0xDB, 0xE7, 0xFF, 0x30, 0x48, 0x3A, 0x06, 0x0E,
  0x26, 0x29, 0x79, 0x29, 0x26, 0x40, 0x7F, 0x05, 0x05, 0x07, 0x40, 0x7F,
  0x05, 0x25, 0x3F, 0x5A, 0x3C, 0xE7, 0x3C, 0x5A, 0x7F, 0x3E, 0x1C, 0x1C,
  0x08, 0x08, 0x1C, 0x1C, 0x3E, 0x7F, 0x14, 0x22, 0x7F, 0x22, 0x14, 0x5F,
  0x5F, 0x00, 0x5F, 0x5F, 0x06, 0x09, 0x7F, 0x01, 0x7F, 0x00, 0x66, 0x89,
  0x95, 0x6A, 0x60, 0x60, 0x60, 0x60, 0x60, 0x94, 0xA2, 0xFF, 0xA2, 0x94,
  0x08, 0x04, 0x7E, 0x04, 0x08, 0x10, 0x20, 0x7E, 0x20, 0x10, 0x08, 0x08,
  0x2A, 0x1C, 0x08, 0x08, 0x1C, 0x2A, 0x08, 0x08, 0x1E, 0x10, 0x10, 0x10,
  0x10, 0x0C, 0x1E, 0x0C, 0x1E, 0x0C, 0x30, 0x38, 0x3E, 0x38, 0x30, 0x06,
  0x0E, 0x3E, 0x0E, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5F,
  0x00, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00, 0x14, 0x7F, 0x14, 0x7F, 0x14,
  0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x23, 0x13, 0x08, 0x64, 0x62, 0x36, 0x49,
  0x56, 0x20, 0x50, 0x00, 0x08, 0x07, 0x03, 0x00, 0x00, 0x1C, 0x22, 0x41,
  0x00, 0x00, 0x41, 0x22, 0x1C, 0x00, 0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 0x08,
  0x08, 0x3E, 0x08, 0x08, 0x00, 0x80, 0x70, 0x30, 0x00, 0x08, 0x08, 0x08,
  0x08, 0x08, 0x00, 0x00, 0x60, 0x60, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02,
  0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x42, 0x7F, 0x40, 0x00, 0x72, 0x49,
  0x49, 0x49, 0x46, 0x21, 0x41, 0x49, 0x4D, 0x33, 0x18, 0x14, 0x12, 0x7F,
  0x10, 0x27, 0x45, 0x45, 0x45, 0x39, 0x3C, 0x4A, 0x49, 0x49, 0x31, 0x41,
  0x21, 0x11, 0x09, 0x07, 0x36, 0x49, 0x49, 0x49, 0x36, 0x46, 0x49, 0x49,
  0x29, 0x1E, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x40, 0x34, 0x00, 0x00,
  0x00, 0x08, 0x14, 0x22, 0x41, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x41,
  0x22, 0x14, 0x08, 0x02, 0x01, 0x59, 0x09, 0x06, 0x3E, 0x41, 0x5D, 0x59,
  0x4E, 0x7C, 0x12, 0x11, 0x12, 0x7C, 0x7F, 0x49, 0x49, 0x49, 0x36, 0x3E,
  0x41, 0x41, 0x41, 0x22, 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x7F, 0x49, 0x49,
  0x49, 0x41, 0x7F, 0x09, 0x09, 0x09, 0x01, 0x3E, 0x41, 0x41, 0x51, 0x73,
  0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x41, 0x7F, 0x41, 0x00, 0x20, 0x40,
  0x41, 0x3F, 0x01, 0x7F, 0x08, 0x14, 0x22, 0x41, 0x7F, 0x40, 0x40, 0x40,
  0x40, 0x7F, 0x02, 0x1C, 0x02, 0x7F, 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x3E,
  0x41, 0x41, 0x41, 0x3E, 0x7F, 0x09, 0x09, 0x09, 0x06, 0x3E, 0x41, 0x51,
  0x21, 0x5E, 0x7F, 0x09, 0x19, 0x29, 0x46, 0x26, 0x49, 0x49, 0x49, 0x32,
  0x03, 0x01, 0x7F, 0x01, 0x03, 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x1F, 0x20,
  0x40, 0x20, 0x1F, 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x63, 0x14, 0x08, 0x14,
  0x63, 0x03, 0x04, 0x78, 0x04, 0x03, 0x61, 0x59, 0x49, 0x4D, 0x43, 0x00,
  0x7F, 0x41, 0x41, 0x41, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x41, 0x41,
  0x41, 0x7F, 0x04, 0x02, 0x01, 0x02, 0x04, 0x40, 0x40, 0x40, 0x40, 0x40,
  0x00, 0x03, 0x07, 0x08, 0x00, 0x20, 0x54, 0x54, 0x78, 0x40, 0x7F, 0x28,
  0x44, 0x44, 0x38, 0x38, 0x44, 0x44, 0x44, 0x28, 0x38, 0x44, 0x44, 0x28,
  0x7F, 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x08, 0x7E, 0x09, 0x02, 0x18,
  0xA4, 0xA4, 0x9C, 0x78, 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x44, 0x7D,
  0x40, 0x00, 0x20, 0x40, 0x40, 0x3D, 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,
  0x00, 0x41, 0x7F, 0x40, 0x00, 0x7C, 0x04, 0x78, 0x04, 0x78, 0x7C, 0x08,
  0x04, 0x04, 0x78, 0x38, 0x44, 0x44, 0x44, 0x38, 0xFC, 0x18, 0x24, 0x24,
  0x18, 0x18, 0x24, 0x24, 0x18, 0xFC, 0x7C, 0x08, 0x04, 0x04, 0x08, 0x48,
  0x54, 0x54, 0x54, 0x24, 0x04, 0x04, 0x3F, 0x44, 0x24, 0x3C, 0x40, 0x40,
  0x20, 0x7C, 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x3C, 0x40, 0x30, 0x40, 0x3C,
  0x44, 0x28, 0x10, 0x28, 0x44, 0x4C, 0x90, 0x90, 0x90, 0x7C, 0x44, 0x64,
  0x54, 0x4C, 0x44, 0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x77, 0x00,
  0x00, 0x00, 0x41, 0x36, 0x08, 0x00, 0x02, 0x01, 0x02, 0x04, 0x02, 0x3C,
  0x26, 0x23, 0x26, 0x3C, 0x1E, 0xA1, 0xA1, 0x61, 0x12, 0x3A, 0x40, 0x40,
  0x20, 0x7A, 0x38, 0x54, 0x54, 0x55, 0x59, 0x21, 0x55, 0x55, 0x79, 0x41,
  0x22, 0x54, 0x54, 0x78, 0x42,   // a-umlaut
  0x21, 0x55, 0x54, 0x78, 0x40, 0x20, 0x54, 0x55, 0x79, 0x40, 0x0C, 0x1E,
  0x52, 0x72, 0x12, 0x39, 0x55, 0x55, 0x55, 0x59, 0x39, 0x54, 0x54, 0x54,
  0x59, 0x39, 0x55, 0x54, 0x54, 0x58, 0x00, 0x00, 0x45, 0x7C, 0x41, 0x00,
  0x02, 0x45, 0x7D, 0x42, 0x00, 0x01, 0x45, 0x7C, 0x40, 0x7D, 0x12, 0x11,
  0x12, 0x7D,   // A-umlaut
  0xF0, 0x28, 0x25, 0x28, 0xF0, 0x7C, 0x54, 0x55, 0x45, 0x00, 0x20, 0x54,
  0x54, 0x7C, 0x54, 0x7C, 0x0A, 0x09, 0x7F, 0x49, 0x32, 0x49, 0x49, 0x49,
  0x32, 0x3A, 0x44, 0x44, 0x44, 0x3A,   // o-umlaut
  0x32, 0x4A, 0x48, 0x48, 0x30, 0x3A, 0x41, 0x41, 0x21, 0x7A, 0x3A, 0x42,
  0x40, 0x20, 0x78, 0x00, 0x9D, 0xA0, 0xA0, 0x7D, 0x3D, 0x42, 0x42, 0x42,
  0x3D,   // O-umlaut
  0x3D, 0x40, 0x40, 0x40, 0x3D, 0x3C, 0x24, 0xFF, 0x24, 0x24, 0x48, 0x7E,
  0x49, 0x43, 0x66, 0x2B, 0x2F, 0xFC, 0x2F, 0x2B, 0xFF, 0x09, 0x29, 0xF6,
  0x20, 0xC0, 0x88, 0x7E, 0x09, 0x03, 0x20, 0x54, 0x54, 0x79, 0x41, 0x00,
  0x00, 0x44, 0x7D, 0x41, 0x30, 0x48, 0x48, 0x4A, 0x32, 0x38, 0x40, 0x40,
  0x22, 0x7A, 0x00, 0x7A, 0x0A, 0x0A, 0x72, 0x7D, 0x0D, 0x19, 0x31, 0x7D,
  0x26, 0x29, 0x29, 0x2F, 0x28, 0x26, 0x29, 0x29, 0x29, 0x26, 0x30, 0x48,
  0x4D, 0x40, 0x20, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
  0x38, 0x2F, 0x10, 0xC8, 0xAC, 0xBA, 0x2F, 0x10, 0x28, 0x34, 0xFA, 0x00,
  0x00, 0x7B, 0x00, 0x00, 0x08, 0x14, 0x2A, 0x14, 0x22, 0x22, 0x14, 0x2A,
  0x14, 0x08, 0x55, 0x00, 0x55, 0x00, 0x55,   // #176 (25% block) missing in old
                                              // code
  0xAA, 0x55, 0xAA, 0x55, 0xAA,               // 50% block
  0xFF, 0x55, 0xFF, 0x55, 0xFF,               // 75% block
  0x00, 0x00, 0x00, 0xFF, 0x00, 0x10, 0x10, 0x10, 0xFF, 0x00, 0x14, 0x14,
  0x14, 0xFF, 0x00, 0x10, 0x10, 0xFF, 0x00, 0xFF, 0x10, 0x10, 0xF0, 0x10,
  0xF0, 0x14, 0x14, 0x14, 0xFC, 0x00, 0x14, 0x14, 0xF7, 0x00, 0xFF, 0x00,
  0x00, 0xFF, 0x00, 0xFF, 0x14, 0x14, 0xF4, 0x04, 0xFC, 0x14, 0x14, 0x17,
  0x10, 0x1F, 0x10, 0x10, 0x1F, 0x10, 0x1F, 0x14, 0x14, 0x14, 0x1F, 0x00,
  0x10, 0x10, 0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x10, 0x10,
  0x10, 0x1F, 0x10, 0x10, 0x10, 0x10, 0xF0, 0x10, 0x00, 0x00, 0x00, 0xFF,
  0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0x10, 0x00,
  0x00, 0x00, 0xFF, 0x14, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x1F,
  0x10, 0x17, 0x00, 0x00, 0xFC, 0x04, 0xF4, 0x14, 0x14, 0x17, 0x10, 0x17,
  0x14, 0x14, 0xF4, 0x04, 0xF4, 0x00, 0x00, 0xFF, 0x00, 0xF7, 0x14, 0x14,
  0x14, 0x14, 0x14, 0x14, 0x14, 0xF7, 0x00, 0xF7, 0x14, 0x14, 0x14, 0x17,
  0x14, 0x10, 0x10, 0x1F, 0x10, 0x1F, 0x14, 0x14, 0x14, 0xF4, 0x14, 0x10,
  0x10, 0xF0, 0x10, 0xF0, 0x00, 0x00, 0x1F, 0x10, 0x1F, 0x00, 0x00, 0x00,
  0x1F, 0x14, 0x00, 0x00, 0x00, 0xFC, 0x14, 0x00, 0x00, 0xF0, 0x10, 0xF0,
  0x10, 0x10, 0xFF, 0x10, 0xFF, 0x14, 0x14, 0x14, 0xFF, 0x14, 0x10, 0x10,
  0x10, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x10, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x38, 0x44, 0x44,
  0x38, 0x44, 0xFC, 0x4A, 0x4A, 0x4A, 0x34,   // sharp-s or beta
  0x7E, 0x02, 0x02, 0x06, 0x06, 0x02, 0x7E, 0x02, 0x7E, 0x02, 0x63, 0x55,
  0x49, 0x41, 0x63, 0x38, 0x44, 0x44, 0x3C, 0x04, 0x40, 0x7E, 0x20, 0x1E,
  0x20, 0x06, 0x02, 0x7E, 0x02, 0x02, 0x99, 0xA5, 0xE7, 0xA5, 0x99, 0x1C,
  0x2A, 0x49, 0x2A, 0x1C, 0x4C, 0x72, 0x01, 0x72, 0x4C, 0x30, 0x4A, 0x4D,
  0x4D, 0x30, 0x30, 0x48, 0x78, 0x48, 0x30, 0xBC, 0x62, 0x5A, 0x46, 0x3D,
  0x3E, 0x49, 0x49, 0x49, 0x00, 0x7E, 0x01, 0x01, 0x01, 0x7E, 0x2A, 0x2A,
  0x2A, 0x2A, 0x2A, 0x44, 0x44, 0x5F, 0x44, 0x44, 0x40, 0x51, 0x4A, 0x44,
  0x40, 0x40, 0x44, 0x4A, 0x51, 0x40, 0x00, 0x00, 0xFF, 0x01, 0x03, 0xE0,
  0x80, 0xFF, 0x00, 0x00, 0x08, 0x08, 0x6B, 0x6B, 0x08, 0x36, 0x12, 0x36,
  0x24, 0x36, 0x06, 0x0F, 0x09, 0x0F, 0x06, 0x00, 0x00, 0x18, 0x18, 0x00,
  0x00, 0x00, 0x10, 0x10, 0x00, 0x30, 0x40, 0xFF, 0x01, 0x01, 0x00, 0x1F,
  0x01, 0x01, 0x1E, 0x00, 0x19, 0x1D, 0x17, 0x12, 0x00, 0x3C, 0x3C, 0x3C,
  0x3C, 0x00, 0x00, 0x00, 0x00, 0x00   // #255 NBSP
};

/***************************************************************************//**
 *  @brief
 *  Draws a char using the font supplied with the library.
 *
 *  @param g_context
 *  Pointer to the glib_context_t
 *
 *  @param my_char
 *  Char to be drawn
 *
 *  @param x
 *  Start x-coordinate for the char (Upper left corner)
 *
 *  @param y
 *  Start y-coordinate for the char (Upper left corner)
 *
 *  @return
 *  Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_draw_char(glib_context_t *g_context,
                             int16_t x, int16_t y,
                             unsigned char c,
                             uint16_t color, uint16_t bg,
                             uint8_t size_x, uint8_t size_y)
{
  glib_status_t status = GLIB_OK;

  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  if (!g_context->font) { // 'Classic' built-in font
    if ((x >= oled_display->width)                 // Clip right
        || (y >= oled_display->height)             // Clip bottom
        || ((x + 6 * size_x - 1) < 0) // Clip left
        || ((y + 8 * size_y - 1) < 0)) { // Clip top
      return GLIB_ERROR_INVALID_REGION;
    }

    if (c >= 176) {
      c++; // Handle 'classic' charset behavior
    }
    for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
      uint8_t line = font_5x7[c * 5 + i];
      for (int8_t j = 0; j < 8; j++, line >>= 1) {
        if (line & 1) {
          if ((size_x == 1) && (size_y == 1)) {
            status |= glib_draw_pixel(g_context, x + i, y + j, color);
          } else {
            glib_fill_rect(g_context,
                           x + i * size_x, y + j * size_y,
                           size_x, size_y,
                           color);
          }
        } else if (bg != color) {
          if ((size_x == 1) && (size_y == 1)) {
            status |= glib_draw_pixel(g_context, x + i, y + j, bg);
          } else {
            status |= glib_fill_rect(g_context,
                                     x + i * size_x, y + j * size_y,
                                     size_x, size_y,
                                     bg);
          }
        }
      }
    }
    if (bg != color) { // If opaque, draw vertical line for last column
      if ((size_x == 1) && (size_y == 1)) {
        status |= draw_vline(g_context,
                             x + 5, y, 8,
                             bg);
      } else {
        status |= glib_fill_rect(g_context,
                                 x + 5 * size_x, y,
                                 size_x, 8 * size_y,
                                 bg);
      }
    }
  } else { // Custom font
    c -= (uint8_t)g_context->font->first;
    glib_gfx_glyph_t *glyph = &(g_context->font->glyph[c]);
    uint8_t *bitmap = g_context->font->bitmap;

    uint16_t bo = glyph->bitmap_offset;
    uint8_t w = glyph->width, h = glyph->height;
    int8_t xo = glyph->x_offset,
           yo = glyph->y_offset;
    uint8_t xx, yy, bits = 0, bit = 0;
    int16_t xo16 = 0, yo16 = 0;

    if ((size_x > 1) || (size_y > 1)) {
      xo16 = xo;
      yo16 = yo;
    }

    for (yy = 0; yy < h; yy++) {
      for (xx = 0; xx < w; xx++) {
        if (!(bit++ & 7)) {
          bits = bitmap[bo++];
        }
        if (bits & 0x80) {
          if ((size_x == 1) && (size_y == 1)) {
            status |= glib_draw_pixel(g_context,
                                      x + xo + xx, y + yo + yy,
                                      color);
          } else {
            status |= glib_fill_rect(g_context,
                                     x + (xo16 + xx) * size_x,
                                     y + (yo16 + yy) * size_y,
                                     size_x,
                                     size_y,
                                     bg);
          }
        }
        bits <<= 1;
      }
    }
  }
  return status;
}

/***************************************************************************//**
 *  @brief
 *  Print one byte/character of data.
 *
 *  @param g_context
 *  Pointer to the glib_context_t
 *
 *  @param c
 *  The 8-bit ascii character to write
 *
 *  @return
 *  Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_write_char(glib_context_t *g_context, unsigned char c)
{
  glib_status_t status = GLIB_ERROR_NOTHING_TO_DRAW;

  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  if (!g_context->font) { // 'Classic' built-in font
    if (c == '\n') { // Newline?
      g_context->cursor_x = 0; // Reset x to zero,
      g_context->cursor_y += g_context->textsize_y * 8; // advance y one line
    } else if (c != '\r') { // Ignore carriage returns
      if (g_context->wrap
          && ((g_context->cursor_x + g_context->textsize_x * 6)
              > g_context->width)) { // Off right?
        g_context->cursor_x = 0; // Reset x to zero,
        g_context->cursor_y += g_context->textsize_y * 8; // advance y one line
      }

      status = glib_draw_char(g_context,
                              g_context->cursor_x, g_context->cursor_y,
                              c,
                              g_context->text_color, g_context->bg_color,
                              g_context->textsize_x, g_context->textsize_y);
      g_context->cursor_x += g_context->textsize_x * 6; // Advance x one char
    }
  } else { // Custom font
    if (c == '\n') {
      g_context->cursor_x = 0;
      g_context->cursor_y +=
        (int16_t)g_context->textsize_y * (uint8_t)g_context->font->y_advance;
    } else if (c != '\r') {
      uint8_t first = g_context->font->first;
      if ((c >= first) && (c <= (uint8_t)g_context->font->last)) {
        glib_gfx_glyph_t *glyph = &(g_context->font->glyph[c - first]);
        uint8_t w = glyph->width,
                h = glyph->height;
        if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
          int16_t xo = (int8_t)glyph->x_offset; // sic
          if (g_context->wrap
              && ((g_context->cursor_x + g_context->textsize_x * (xo + w))
                  > g_context->width)) {
            g_context->cursor_x = 0;
            g_context->cursor_y += (int16_t)g_context->textsize_y
                                   * (uint8_t)g_context->font->y_advance;
          }
          status = glib_draw_char(g_context,
                                  g_context->cursor_x, g_context->cursor_y,
                                  c,
                                  g_context->text_color, g_context->bg_color,
                                  g_context->textsize_x, g_context->textsize_y);
        } else {
          // Glyph is whitespace character, just leave it blank
          status = GLIB_OK;
        }
        g_context->cursor_x +=
          (uint8_t)glyph->x_advance * (int16_t)g_context->textsize_x;
      }
    }
  }
  return status;
}

/***************************************************************************//**
 *  @brief
 *  Draws a string using the font supplied with the library.
 *
 *  @param g_context
 *  Pointer to a glib_context_t
 *
 *  @param str
 *  Pointer to the string that is drawn
 *
 *  @param x0
 *  Start x-coordinate for the string (Upper left corner)
 *
 *  @param y0
 *  Start y-coordinate for the string (Upper left corner)
 *
 *  @return
 *  Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_draw_string(glib_context_t *g_context, const char *str,
                               int16_t x0, int16_t y0)
{
  glib_status_t status;

  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }

  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  g_context->cursor_x = x0;
  g_context->cursor_y = y0;

  /* Loops through the string and prints char for char */
  while (*str) {
    status = glib_write_char(g_context, *str);
    if (status != GLIB_OK) {
      // Char could not be written
      return status;
    }

    // Next char
    str++;
  }
  return GLIB_OK;
}

/***************************************************************************//**
 *  @brief
 *  Set new font for the library. Note that GLIB defines a default font in
 *   glib.c.
 *  Redefine GLIB_DEFAULT_FONT to change the default font.
 *
 *  @param g_context
 *  Pointer to the glib_context_t
 *
 *  @param pFont
 *  Pointer to the new font
 *
 *  @return
 *  Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_set_font(glib_context_t *g_context, const glib_gfx_font_t *f)
{
  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  if (f) {          // Font struct pointer passed in?
    if (!g_context->font) { // And no current font struct?
      // Switching from classic to new font behavior.
      // Move cursor pos down 6 pixels so it's on baseline.
      g_context->cursor_y += 6;
    }
  } else if (g_context->font) { // NULL passed.  Current font struct defined?
    // Switching from new to classic font behavior.
    // Move cursor pos up 6 pixels so it's at top-left of char.
    g_context->cursor_y -= 6;
  }
  g_context->font = (glib_gfx_font_t *)f;

  return GLIB_OK;
}

/***************************************************************************//**
 *     @brief       Set text 'magnification' size.
 *                  Each increase in s makes 1 pixel that much bigger.
 *                  For example: 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
 *
 *     @param  s_x  Desired text width magnification level in X-axis.
 *                  1 is default
 *
 *     @param  s_y  Desired text width magnification level in Y-axis.
 *                  1 is default
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_set_text_size(glib_context_t *g_context,
                                 uint8_t s_x,
                                 uint8_t s_y)
{
  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  g_context->textsize_x = s_x;
  g_context->textsize_y = s_y;

  return GLIB_OK;
}

/***************************************************************************//**
 *     @brief
 *     Set text cursor location
 *
 *     @param g_context
 *     Pointer to the glib_context_t
 *
 *     @param x
 *     X-coordinate in pixels
 *
 *     @param y
 *     Y-coordinate in pixels
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_set_cursor(glib_context_t *g_context, int16_t x, int16_t y)
{
  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  g_context->cursor_x = x;
  g_context->cursor_y = y;

  return GLIB_OK;
}

/***************************************************************************//**
 *     @brief   Set text font color with custom background color
 *
 *     @param   c   16-bit 5-6-5 Color to draw text with
 *
 *     @param   bg  16-bit 5-6-5 Color to draw background/fill with
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_set_color(glib_context_t *g_context,
                             uint16_t c,
                             uint16_t bg)
{
  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  g_context->text_color = c;
  g_context->bg_color = bg;
  return GLIB_OK;
}

/***************************************************************************//**
 *     @brief   Set text font color
 *
 *     @param   c   16-bit 5-6-5 Color to draw text with
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_set_text_color(glib_context_t *g_context, uint16_t c)
{
  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  g_context->text_color = c;
  return GLIB_OK;
}

/***************************************************************************//**
 *     @brief      Set background setting for display
 *
 *     @param  bg  16-bit 5-6-5 Color to draw background/fill with
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_set_bg_color(glib_context_t *g_context, uint16_t bg)
{
  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  g_context->bg_color = bg;
  return GLIB_OK;
}

/***************************************************************************//**
 *     @brief      Set whether text that is too long for the screen width should
 *                 automatically wrap around to the next line (else clip right).
 *
 *     @param  wr  true = wrapping, false = clipping
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_enable_wrap(glib_context_t *g_context, bool wr)
{
  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  g_context->wrap = wr;
  return GLIB_OK;
}

/***************************************************************************//**
 *     @brief      Set 'cp437' to use correct CP437 charset (default is off)
 *
 *     @param  cp  true = Enable cp437; false = Disable cp437
 *
 *     @return
 *     Returns GLIB_OK on success, or else error code
 ******************************************************************************/
glib_status_t glib_enable_cp437(glib_context_t *g_context, bool cp)
{
  /* Check arguments */
  if (g_context == NULL) {
    return GLIB_ERROR_INVALID_ARGUMENT;
  }

  g_context->cp437 = cp;
  return GLIB_OK;
}

/***************************************************************************//**
 *    @brief
 *    Update the display device with contents of active glib_frame_buffer.
 *
 *    @return
 *    Returns GLIB_OK is successful, error otherwise.
 ******************************************************************************/
glib_status_t glib_update_display(void)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }
  return ((oled_display->driver->update_display()
           == SL_STATUS_OK) ? GLIB_OK : GLIB_ERROR_OUT_OF_MEMORY);
}

/***************************************************************************//**
 * @brief
 *   Set a inversion color to glib.
 *
 * @return
 *   GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_set_invert_color(void)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }
  return ((oled_display->driver->set_invert_color()
           == SL_STATUS_OK) ? GLIB_OK : GLIB_ERROR_IO);
}

/***************************************************************************//**
 *    @brief
 *      Set a normal color to glib.
 *
 *    @return
 *      GLIB_OK if there are no errors.
 *****************************************************************************/
glib_status_t glib_set_normal_color(void)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }
  return ((oled_display->driver->set_normal_color()
           == SL_STATUS_OK) ? GLIB_OK : GLIB_ERROR_IO);
}

/***************************************************************************//**
 *    @brief
 *      Set a contrast to glib.
 *
 *    @param[in] value
 *      value to set contrast. Select 1 out of 256 contrast steps.
 *      Contrast increases as the value increases
 *
 *    @return
 *      GLIB_OK if there are no errors.
 *****************************************************************************/
glib_status_t glib_set_contrast(uint8_t value)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }
  return ((oled_display->driver->set_contrast(value)
           == SL_STATUS_OK) ? GLIB_OK : GLIB_ERROR_IO);
}

/***************************************************************************//**
 *    @brief
 *      Set a Right Horizontal Scroll to glib.
 *
 *    @param[in] start_page_addr
 *      Start page address
 *
 *    @param[in] end_page_addr
 *      End page address
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_scroll_right(uint8_t start_page_addr, uint8_t end_page_addr)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }
  return ((oled_display->driver->scroll_right(start_page_addr,
                                              end_page_addr)
           == SL_STATUS_OK) ? GLIB_OK : GLIB_ERROR_IO);
}

/***************************************************************************//**
 *    @brief
 *      Set a Left Horizontal Scroll to glib.
 *
 *    @param[in] start_page_addr
 *      Start page address
 *
 *    @param[in] end_page_addr
 *      End page address
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_scroll_left(uint8_t start_page_addr, uint8_t end_page_addr)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }
  return ((oled_display->driver->scroll_left(start_page_addr,
                                             end_page_addr)
           == SL_STATUS_OK) ? GLIB_OK : GLIB_ERROR_IO);
}

/***************************************************************************//**
 *    @brief
 *      Set a Vertical and Right Horizontal Scroll to glib.
 *
 *    @param[in] start_page_addr
 *      Start page address
 *
 *    @param[in] end_page_addr
 *      End page address
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_scroll_diag_right(uint8_t start_page_addr,
                                     uint8_t end_page_addr)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }
  return ((oled_display->driver->scroll_diag_right(start_page_addr,
                                                   end_page_addr)
           == SL_STATUS_OK) ? GLIB_OK : GLIB_ERROR_IO);
}

/***************************************************************************//**
 *    @brief
 *      Set a Vertical and Left Horizontal Scroll to glib.
 *
 *    @param[in] start_page_addr
 *      Start page address
 *
 *    @param[in] end_page_addr
 *      End page address
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_scroll_diag_left(uint8_t start_page_addr,
                                    uint8_t end_page_addr)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }
  return ((oled_display->driver->scroll_diag_left(start_page_addr,
                                                  end_page_addr)
           == SL_STATUS_OK) ? GLIB_OK : GLIB_ERROR_IO);
}

/***************************************************************************//**
 *    @brief
 *      Stop scroll to glib.
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_stop_scroll(void)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }
  return ((oled_display->driver->stop_scroll()
           == SL_STATUS_OK) ? GLIB_OK : GLIB_ERROR_IO);
}

/***************************************************************************//**
 *    @brief
 *      Initialize for shifting text along the horizontal axis of the screen.
 *
 *    @param[in] string
 *      The string that shifted along the axis.
 *
 *    @param[in] rate_ms
 *      The moving speed of the string.
 *
 *    @param[in] direction
 *      The moving direction of the string.
 *
 *    @param[in] y
 *      The distance between the text and the upper boundary of the display.
 ******************************************************************************/
void glib_shift_text_init(glib_context_t *g_context, const char *string,
                          uint32_t rate_ms,
                          glib_shifting_direction_t direction, int16_t y)
{
  shift_data.context = g_context;
  shift_data.data = string;
  shift_data.direction = direction;
  shift_data.string_length = 0;
  shift_data.rate_ms = rate_ms;
  shift_data.max_char_height = 0;
  shift_data.addition_pixel = 0;
  shift_data.ticks = sl_sleeptimer_get_tick_count();
  shift_data.y = y;

  if (!shift_data.context->font) {
    shift_data.max_char_height = 8;
    shift_data.string_length = (int)strlen(shift_data.data) * 6 - 1;
  } else {
    glib_gfx_font_t *font = shift_data.context->font;
    for (int i = 0; i < (int)strlen(shift_data.data); i++) {
      unsigned char c = shift_data.data[i] - font->first;
      if (font->glyph[c].height > shift_data.max_char_height) {
        shift_data.max_char_height = font->glyph[c].height;
      }
      if (font->glyph[c].y_offset <= 0) {
        if (font->glyph[c].height + font->glyph[c].y_offset
            > shift_data.addition_pixel) {
          shift_data.addition_pixel =
            font->glyph[c].height + font->glyph[c].y_offset;
        }
      }
      shift_data.string_length += font->glyph[c].x_advance;
    }
  }
}

/***************************************************************************//**
 *    @brief
 *      Shifting string along the axis of the screen. Call it in the process
 *      action funtion of the application.
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_shift_text(void)
{
  glib_status_t status = GLIB_OK;
  static int16_t cursor_x = 0;

  char *ptr = (char *)shift_data.data;

  if (sl_sleeptimer_get_tick_count() - shift_data.ticks
      < shift_data.rate_ms) {
    return GLIB_ERROR_NOTHING_TO_DRAW;
  }
  switch (shift_data.direction) {
    case LEFT:
      if (!shift_data.context->font) {
        status |= glib_fill_rect(shift_data.context, 0, shift_data.y,
                                 shift_data.context->width,
                                 shift_data.max_char_height, 0);
      } else {
        status |= glib_fill_rect(
          shift_data.context,
          0,
          shift_data.y - shift_data.max_char_height,
          shift_data.context->width,
          shift_data.max_char_height + shift_data.addition_pixel,
          0);
      }
      status |= glib_set_cursor(shift_data.context, cursor_x--, shift_data.y);
      write_string(ptr);
      if (cursor_x == -shift_data.string_length) {
        cursor_x = shift_data.context->width;
      }
      break;

    case RIGHT:
      if (!shift_data.context->font) {
        status |= glib_fill_rect(shift_data.context, 0, shift_data.y,
                                 shift_data.context->width,
                                 shift_data.max_char_height, 0);
      } else {
        status |= glib_fill_rect(
          shift_data.context,
          0,
          shift_data.y - shift_data.max_char_height,
          shift_data.context->width,
          shift_data.max_char_height + shift_data.addition_pixel,
          0);
      }
      status |= glib_set_cursor(shift_data.context, cursor_x++, shift_data.y);
      write_string(ptr);
      if (cursor_x == shift_data.context->width) {
        cursor_x = -shift_data.string_length;
      }
      break;
  }
  return status;
}

/***************************************************************************//**
 *    @brief
 *      Set the display ON/OFF to glib.
 *
 *    @param[in] on
 *      State of OLED
 *
 *    @return
 *      GLIB_OK if there are no errors.
 ******************************************************************************/
glib_status_t glib_enable_display(bool on)
{
  // Check oled driver
  if (oled_display == NULL) {
    return GLIB_ERROR_DRIVER_NOT_INITIALIZED;
  }
  return ((oled_display->driver->enable_display(on)
           == SL_STATUS_OK) ? GLIB_OK : GLIB_ERROR_IO);
}
