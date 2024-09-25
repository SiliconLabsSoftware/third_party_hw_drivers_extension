/***************************************************************************//**
 * @file lvgl_input.c
 * @brief LVGL input driver source file
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
#include "sl_status.h"
#include "lvgl_input.h"
// -----------------------------------------------------------------------------
//                       Local Function
// -----------------------------------------------------------------------------
static sl_status_t lvgl_input_driver_init(void);
static sl_status_t lvgl_input_driver_read_data (void);
static sl_status_t lvgl_input_driver_get_xy(int16_t *x, int16_t *y);
static bool lvgl_input_driver_get_touch(void);

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------
#define PRESSURE_THRESH                 500

static lvgl_input_t lvgl_input_instance;
static bool initialized = false;

static int32_t _x, _y, _rtouch;
static uint16_t x_raw, y_raw, z1_raw, z2_raw;

static const lvgl_input_driver_api_t lvgl_input_driver_api =
{
  .init = lvgl_input_driver_init,
  .read_data = lvgl_input_driver_read_data,
  .get_touch = lvgl_input_driver_get_touch,
  .get_xy = lvgl_input_driver_get_xy
};

sl_status_t lvgl_input_init(void)
{
  lvgl_input_instance.driver = &lvgl_input_driver_api;
  initialized = true;
  return SL_STATUS_OK;
}

lvgl_input_t *lvgl_input_get(void)
{
  if (initialized) {
    return &lvgl_input_instance;
  } else {
    return NULL;
  }
}

static sl_status_t lvgl_input_driver_init(void)
{
  return SL_STATUS_OK;
}

static sl_status_t lvgl_input_driver_read_data(void)
{
  sl_status_t stt1 = adafruit_tsc2007_read_touch();

  if (SL_STATUS_OK == stt1) {
    adafruit_tsc2007_get_touch(&x_raw, &y_raw, &z1_raw, &z2_raw);

    sl_status_t stt2 = adafruit_tsc2007_get_touch_point(&_x, &_y, &_rtouch);
    return stt2;
  } else {
    return stt1;
  }
}

static sl_status_t lvgl_input_driver_get_xy(int16_t *x, int16_t *y)
{
  sl_status_t stt = SL_STATUS_OK;

  if ((NULL != x) && (NULL != y)) {
    (*x) = _x;
    (*y) = _y;
  } else {
    stt = SL_STATUS_INVALID_PARAMETER;
  }

  return stt;
}

static bool lvgl_input_driver_get_touch(void)
{
  bool retVal = false;

  if (_rtouch < PRESSURE_THRESH) {
    retVal = true;
  }
  return retVal;
}
