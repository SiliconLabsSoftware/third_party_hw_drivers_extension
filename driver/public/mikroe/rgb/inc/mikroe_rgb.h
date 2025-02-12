/***************************************************************************//**
 * @file mikroe_rgbdriver.h
 * @brief mikroe_rgbdriver.h Prototypes
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef MIKROE_RGB_H_
#define MIKROE_RGB_H_

#include "sl_status.h"
#include "rgbdriver.h"

#ifdef __cplusplus
extern "C" {
#endif
enum mikroe_rgbdriver_color {
  mikroe_rgbdriver_red_low_intensity    = RGBDRIVER_COLOR_RED_LOW_INTENSITY,
  mikroe_rgbdriver_orange_low_intensity = RGBDRIVER_COLOR_ORANGE_LOW_INTENSITY,
  mikroe_rgbdriver_yellow_low_intensity = RGBDRIVER_COLOR_YELLOW_LOW_INTENSITY,
  mikroe_rgbdriver_green_low_intensity  = RGBDRIVER_COLOR_GREEN_LOW_INTENSITY,
  mikroe_rgbdriver_blue_low_intensity   = RGBDRIVER_COLOR_BLUE_LOW_INTENSITY,
  mikroe_rgbdriver_white_low_intensity  = RGBDRIVER_COLOR_WHITE_LOW_INTENSITY,
  mikroe_rgbdriver_purple_low_intensity = RGBDRIVER_COLOR_PURPLE_LOW_INTENSITY
};

/***************************************************************************//**
 * @addtogroup mikroe_rgbdriver - RGB Driver Click
 * @brief
 *  The integration of the Mikroe SDK Click extension rgbdriver driver.
 *  (RGB Driver Click board.)
 *
 *    @n @section rgbdriver_example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 *    #include "mikroe_rgbdriver.h"
 *    #include "sl_i2cspm_instances.h"
 *
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize sensor and set mode
 *      mikroe_rgbdriver_init(sl_i2cspm_mikroe);
 *
 *      while (true) {
 *
 *        mikroe_rgbdriver_set_color(color_table[idx]);
 *        sl_sleeptimer_delay_millisecond(2000);
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_rgbdriver_init(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    This function writes a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 *
 * @param[in] tx_buf : Data to be written.
 * @param[in] tx_len : Number of bytes to be written.
 *
 * @return
 *    SL_STATUS_INVALID_PARAMETER if tx_buf is null
 *    SL_STATUS_OK - Success
 *    SL_STATUS_FAIL - Error.
 *
 ******************************************************************************/
sl_status_t mikroe_rgbdriver_generic_write(uint8_t *tx_buf, uint8_t tx_len);

/***************************************************************************//**
 * @brief
 *    This function sets colors of the rgb LEDs through the
 *  the parameters for red, green and blue.
 *
 * @param[in] red : Red led.
 * @param[in] green : Green led.
 * @param[in] blue : Blue led.
 *
 * @return void
 *
 *
 ******************************************************************************/
void mikroe_rgbdriver_set_rgb_color (uint8_t red, uint8_t green,
                                     uint8_t blue);

/***************************************************************************//**
 * @brief
 *    This function sets the color.
 *
 * @param[in] color : Color to set.
 *
 * @return void
 *
 * @note
 * Set color:
 *    RED,
 *    GREEN,
 *    BLUE,
 *    YELLOW,
 *    WHITE,
 *    PURPLE,
 *    ORANGE and OFF.
 *
 ******************************************************************************/
void mikroe_rgbdriver_set_color (uint32_t color);

/***************************************************************************//**
 * @brief
 * This function shuts down device.
 *
 * @return void
 *
 ******************************************************************************/
void mikroe_rgbdriver_shut_down(void);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_RGB_H_ */
