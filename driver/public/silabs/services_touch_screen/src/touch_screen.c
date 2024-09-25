/***************************************************************************//**
 * @file adafruit_ili9341_touch.h
 * @brief Adafruit ILI9341 TFT LCD with Touchscreen Header File
 * @version 1.0.0
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
// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------
#include "touch_screen.h"

// -----------------------------------------------------------------------------
//                       Macros
// -----------------------------------------------------------------------------

#define NUMSAMPLES               6

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                       Local Functions
// -----------------------------------------------------------------------------

/**************************************************************************//**
 * Insert sort function.
 *****************************************************************************/
static void insert_sort(int32_t *array, uint8_t size)
{
  uint8_t j;
  int32_t save;

  for (uint8_t i = 1; i < size; i++) {
    save = array[i];
    for (j = i; j >= 1 && save < array[j - 1]; j--) {
      array[j] = array[j - 1];
    }
    array[j] = save;
  }
}

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 * Initialize Touch Screen feature.
 *****************************************************************************/
sl_status_t touch_screen_init(struct touch_screen *ts)
{
  return touch_screen_interface_init(ts);
}

/**************************************************************************//**
 * Set config.
 *****************************************************************************/
void touch_screen_set_config(struct touch_screen *ts,
                             const struct touch_screen_config *config)
{
  ts->config = config;
}

/**************************************************************************//**
 * Get touch points whenever a touch is detected on the screen.
 *****************************************************************************/
sl_status_t touch_screen_read_touch(struct touch_screen *ts)
{
  int32_t samples[NUMSAMPLES];
  int32_t ts_data;
  int i;

  // XM = 0, XP = 1, YM & YP as input
  ts->aif->set_output(TOUCH_SCREEN_CHANNEL_XM, 0);
  ts->aif->set_output(TOUCH_SCREEN_CHANNEL_XP, 1);
  ts->aif->set_input(TOUCH_SCREEN_CHANNEL_YM);
  ts->aif->set_input(TOUCH_SCREEN_CHANNEL_YP);

  // Fast ARM chips need to allow voltages to settle
  ts->aif->delay_10us(2);

  ts->aif->adc_start_read(TOUCH_SCREEN_CHANNEL_YP);
  for (i = 0; i < NUMSAMPLES; i++) {
    samples[i] = ts->aif->adc_read_u12();
  }
  ts->aif->adc_stop();

  insert_sort(samples, NUMSAMPLES);
  ts_data = samples[NUMSAMPLES / 2];
  if (ts_data > 4095) {
    return SL_STATUS_INVALID_RANGE;
  }

  ts->t_x = (4096 - ts_data);

  // YM = 0, YP = 1, XM & XP as input
  ts->aif->set_output(TOUCH_SCREEN_CHANNEL_YM, 0);
  ts->aif->set_output(TOUCH_SCREEN_CHANNEL_YP, 1);
  ts->aif->set_input(TOUCH_SCREEN_CHANNEL_XM);
  ts->aif->set_input(TOUCH_SCREEN_CHANNEL_XP);

  // Fast ARM chips need to allow voltages to settle
  ts->aif->delay_10us(2);

  ts->aif->adc_start_read(TOUCH_SCREEN_CHANNEL_XM);
  for (i = 0; i < NUMSAMPLES; i++) {
    samples[i] = ts->aif->adc_read_u12();
  }
  ts->aif->adc_stop();

  insert_sort(samples, NUMSAMPLES);
  ts_data = samples[NUMSAMPLES / 2];
  if (ts_data > 4095) {
    return SL_STATUS_INVALID_RANGE;
  }

  ts->t_y = (4096 - ts_data);

  // XP = 0, YM = 1, XM = Hi-Z& YP as input
  ts->aif->set_output(TOUCH_SCREEN_CHANNEL_XP, 0);
  ts->aif->set_output(TOUCH_SCREEN_CHANNEL_YM, 1);
  ts->aif->set_input(TOUCH_SCREEN_CHANNEL_XM);
  ts->aif->set_input(TOUCH_SCREEN_CHANNEL_YP);

  ts->aif->adc_start_read(TOUCH_SCREEN_CHANNEL_XM);
  ts->t_z1 = ts->aif->adc_read_u12();
  ts->aif->adc_stop();

  ts->aif->adc_start_read(TOUCH_SCREEN_CHANNEL_YP);
  ts->t_z2 = ts->aif->adc_read_u12();
  ts->aif->adc_stop();

  return SL_STATUS_OK;
}

sl_status_t touch_screen_get_point(struct touch_screen *ts,
                                   touch_point_t *ts_point)
{
  float r_touch;
  uint16_t x, y;
  sl_status_t status;

  status = touch_screen_read_touch(ts);
  if (SL_STATUS_OK != status) {
    return status;
  }

  if (ts->config->xy_swap) {
    x = ts->t_y;
    y = ts->t_x;
  } else {
    x = ts->t_x;
    y = ts->t_y;
  }

  if (x > ts->config->x_min) {
    x -= ts->config->x_min;
  } else {
    x = 0;
  }

  if (y > ts->config->y_min) {
    y -= ts->config->y_min;
  } else {
    y = 0;
  }

  ts_point->x = (uint32_t)((uint32_t)x * ts->config->h_res)
                / (ts->config->x_max - ts->config->x_min);

  ts_point->y = (uint32_t)((uint32_t)y * ts->config->v_res)
                / (ts->config->y_max - ts->config->y_min);

  // now read the x
  r_touch = ts->t_z2;
  r_touch /= ts->t_z1;
  r_touch -= 1;
  r_touch *= ts->t_x;
  r_touch *= ts->config->xplate_res;
  r_touch /= 4096;

  ts_point->r_touch = r_touch;

  return SL_STATUS_OK;
}
