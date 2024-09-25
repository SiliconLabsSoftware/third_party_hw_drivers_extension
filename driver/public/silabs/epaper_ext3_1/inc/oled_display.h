/***************************************************************************//**
 * @file oled_display.h
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
#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <stdbool.h>
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct oled_display_driver_api {
  sl_status_t (*init)(void);
  sl_status_t (*draw_pixel)(int16_t x, int16_t y, uint16_t color);
  uint16_t (*get_raw_pixel)(int16_t x, int16_t y);
  sl_status_t (*fill_screen)(uint16_t color);
  sl_status_t (*update_display)(void);
  sl_status_t (*set_invert_color)(void);
  sl_status_t (*set_normal_color)(void);
  sl_status_t (*set_contrast)(uint8_t);
  sl_status_t (*scroll_right)(uint8_t, uint8_t);
  sl_status_t (*scroll_left)(uint8_t, uint8_t);
  sl_status_t (*scroll_diag_right)(uint8_t, uint8_t);
  sl_status_t (*scroll_diag_left)(uint8_t, uint8_t);
  sl_status_t (*stop_scroll)(void);
  sl_status_t (*enable_display)(bool);
} oled_display_driver_api_t;

/**
 * General memory LCD data structure.
 */
typedef struct oled_display {
  unsigned short width;       ///< Display pixel width
  unsigned short height;      ///< Display pixel height
  const oled_display_driver_api_t *driver;
} oled_display_t;

/**************************************************************************//**
 * @brief
 *   Initialization function for device driver.
 *
 * @return
 *   If all operations completed sucessfully SL_STATUS_OK is returned. On
 *   failure a different status code is returned specifying the error.
 *****************************************************************************/
sl_status_t oled_display_init(void);

const oled_display_t *oled_display_get(void);

#ifdef __cplusplus
}
#endif

#endif
