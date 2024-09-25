/***************************************************************************//**
 * @file lvgl_input.h
 * @brief LVGL input driver interface
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

#ifndef LVGL_INPUT_H_
#define LVGL_INPUT_H_
#include <stdbool.h>
#include "touch_screen.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _lvgl_input_driver_api {
  sl_status_t (*init)(void);
  sl_status_t (*read_data)(void);
  bool (*get_touch)(void);
  sl_status_t (*get_xy)(int16_t *x, int16_t *y);
} lvgl_input_driver_api_t;

typedef struct _lvgl_input{
  const lvgl_input_driver_api_t *driver;
} lvgl_input_t;

sl_status_t lvgl_input_init(void);
lvgl_input_t *lvgl_input_get(void);

#ifdef __cplusplus
}
#endif

#endif /* LVGL_INPUT_H_ */
