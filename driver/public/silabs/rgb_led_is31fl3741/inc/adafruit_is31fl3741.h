/***************************************************************************//**
 * @file adafruit_is31fl3741.h
 * @brief Adafruit RGB LED header file.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#ifndef ADAFRUIT_IS31FL3741_H_
#define ADAFRUIT_IS31FL3741_H_

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include "adafruit_is31fl3741_i2c.h"

// -----------------------------------------------------------------------------
//                       Public Functions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *  This function initializes the RGB LED IS31FL3741.
 *
 * @param[in] i2cspm
 *  The I2CSPM instance..
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_ALREADY_INITIALIZED if the module is already initialized.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_init(mikroe_i2c_handle_t i2cspm);

/***************************************************************************//**
 * @brief
 *  This function perfoms a software reset on RGB LED IS31FL3741 module then
 *  update all the register to POR values.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_reset(void);

/***************************************************************************//**
 * @brief
 *  This function helps to enable/disable output via the shutdown register bit.
 *
 * @param[in] en
 *  Enable/Disable status, true to enable, false to disable.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_enable(bool en);

/***************************************************************************//**
 * @brief
 *  This function sets global current-mirror from 0 (off) to 255 (brightest).
 *
 * @param[in] current
 *  The vaue of the current that set to RGB LED IS31FL3741 module.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_set_global_current(uint8_t current);

/***************************************************************************//**
 * @brief
 *  This function Gets the global current-mirror register setting.
 *
 * @param[out] current
 *  The vaue of the current that set to RGB LED IS31FL3741 module.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_get_global_current(uint8_t *current);

/***************************************************************************//**
 * @brief
 *  This function sets the scaling level for a single LED of the RGB LED
 *  IS31FL3741 module.
 *
 * @param[in] led_num
 *  The index of the led that want to control.
 * @param[in] scale
 *  Scaling level.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_set_led_scaling(uint16_t led_num,
                                                uint8_t scale);

/***************************************************************************//**
 * @brief
 *  This function sets the PWM level for a single LED of the RGB LED IS31FL3741
 *  module.
 *
 * @param[in] led_num
 *  The index of the led that want to control.
 * @param[in] pwm
 *  PWM level.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_set_led_pwm(uint16_t led_num, uint8_t pwm);

/***************************************************************************//**
 * @brief
 *  This function sets the PWM level for all LEDs of the RGB LED IS31FL3741
 *  module.
 *
 * @param[in] pwm
 *  PWM level.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_set_global_led_pwm(uint8_t pwm);

/***************************************************************************//**
 * @brief
 *  This function sets the scaling level for all LEDs of the RGB LED IS31FL3741
 *  module.
 *
 * @param[in] scaling
 *  Scaling level.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_set_global_led_scaling(uint8_t scaling);

/***************************************************************************//**
 * @brief
 *  This function draws a single pixel on the RGB LED IS31FL3741 with specific
 *  16-bit RGB565 packed color. It will push data into a buffer then user needs
 *  to call the adafruit_is31fl3741_show() function to display the pixel on the
 *  RGB LED IS31FL3741 module.
 *
 * @param[in] x
 *  Coordinator in x axis.
 * @param[in] y
 *  Coordinator in y axis.
 * @param[in] color
 *  RGB color 565 packed format.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_draw_pixel(int16_t x, int16_t y,
                                           uint16_t color);

/***************************************************************************//**
 * @brief
 *  This function fill the whole RGB LED IS31FL3741's pixels with the specific
 *  color.
 *
 * @param[in] color
 *  RGB color 565 packed format.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_fill(uint16_t color);

/***************************************************************************//**
 * @brief
 *  This function push the data from buffer to RGB LED IS31FL3741 module.
 *
 * @return
 *  SL_STATUS_OK if there are no errors.
 *  SL_STATUS_FAIL if the process is failed.
 ******************************************************************************/
sl_status_t adafruit_is31fl3741_show(void);

#endif /* ADAFRUIT_IS31FL3741_H_ */
