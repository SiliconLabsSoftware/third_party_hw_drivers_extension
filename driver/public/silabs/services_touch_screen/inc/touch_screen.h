/***************************************************************************//**
 * @file touch_screen.h
 * @brief Touchscreen Header File
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
#ifndef TOUCH_SCREEN_H_
#define TOUCH_SCREEN_H_

// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------
#include <stdbool.h>
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
//                       Typedefs
// -----------------------------------------------------------------------------

struct touch_screen_config {
  // Horizontal Resolution
  uint32_t h_res;

  // Vertical Resolution
  uint32_t v_res;

  // X-plate resistance
  uint16_t xplate_res;

  // Calibration value: X-min
  uint16_t x_min;

  // Calibration value: X-max
  uint16_t x_max;

  // Calibration value: Y-min
  uint16_t y_min;

  // Calibration value: Y-max
  uint16_t y_max;

  // Invert X-axis
  bool x_inv;

  // Invert Y-axis
  bool y_inv;

  // Invert Y-axis
  bool xy_swap;
};

enum TOUCH_SCREEN_CHANNEL {
  TOUCH_SCREEN_CHANNEL_XM,
  TOUCH_SCREEN_CHANNEL_XP,
  TOUCH_SCREEN_CHANNEL_YM,
  TOUCH_SCREEN_CHANNEL_YP,
};

struct touch_screen_analog_interface {
  void (*delay_10us)(uint32_t idelay);
  void (*set_input)(enum TOUCH_SCREEN_CHANNEL channel);
  void (*set_output)(enum TOUCH_SCREEN_CHANNEL channel, uint8_t value);
  sl_status_t (*adc_start_read)(enum TOUCH_SCREEN_CHANNEL channel);
  uint16_t (*adc_read_u12)(void);
  void (*adc_stop)(void);
};

struct touch_screen {
  const struct touch_screen_config *config;
  const struct touch_screen_analog_interface *aif;
  uint16_t t_x;
  uint16_t t_y;
  uint16_t t_z1;
  uint16_t t_z2;
};

// Touch point properties struct
typedef struct
{
  int32_t x; /* X coordinate of touch point */
  int32_t y; /* Y coordinate of touch point */
  float r_touch; /* Pressure value when touch point detected */
} touch_point_t;

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *  Initialize Touch Screen feature.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t touch_screen_init(struct touch_screen *ts);

/***************************************************************************//**
 * @brief
 *  Initialize Touch Screen HAL.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t touch_screen_interface_init(struct touch_screen *ts);

/***************************************************************************//**
 * @brief
 *  Set config.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
void touch_screen_set_config(struct touch_screen *ts,
                             const struct touch_screen_config *config);

/***************************************************************************//**
 * @brief
 *  Get touch points whenever a touch is detected on the screen.
 *
 * @param[in] rxplate
 *  Resistance threshold value.
 * @param[out] ts_point
 *  Touch point detected coordinate.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t touch_screen_get_point(struct touch_screen *ts,
                                   touch_point_t *ts_point);

#ifdef __cplusplus
extern "C"
}
#endif

#endif /* TOUCH_SCREEN_H_ */
