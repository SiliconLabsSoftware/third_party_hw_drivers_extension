/***************************************************************************//**
 * @file adafruit_ws2812.h
 * @brief Header file for WS2812
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
#ifndef ADAFRUIT_WS2812_H_
#define ADAFRUIT_WS2812_H_

#include "adafruit_ws2812_config.h"
#include "drv_digital_out.h"
#include "drv_digital_in.h"
#include "drv_spi_master.h"
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t brightness;
} ws2812_color_t;

typedef struct {
  spi_master_t spi;
  uint32_t num_leds;
} ws2812_device_t;

typedef struct {
  uint8_t ws2812_frame[WS2812_FRAME_LENGTH];
} ws2812_data_transmission_t;

/*******************************************************************************
 * @brief
 *  Initialize the WS2812.
 *
 * @param[in] spi_master_instance   Working SPI instance.
 *
 * @return                          SL_STATUS_OK if no error,
 *                                  otherwise error code.
 ******************************************************************************/
sl_status_t ws2812_init(mikroe_spi_handle_t spi_master_instance);

/*******************************************************************************
 * @brief
 *  Set a number of LEDs consecutively.
 *
 * @param[in] first_pos             First LED position to be set.
 *
 * @param[in] second_pos            Second LED position to be set.
 *
 * @param[in] color                 Color to be changed into.
 *
 * @return                          SL_STATUS_OK if no error, otherwise error code.
 ******************************************************************************/
sl_status_t ws2812_set_consecutive_leds(uint32_t first_pos,
                                        uint32_t second_pos,
                                        ws2812_color_t color);

/*******************************************************************************
 * @brief
 *  Set a specific LED.
 *
 * @param[in] pos                   Position of LED to be set.
 *
 * @param[in] color                 Color to be changed into.
 *
 * @return                          SL_STATUS_OK if no error, otherwise error code.
 ******************************************************************************/
sl_status_t ws2812_set_specific_led(uint32_t pos,
                                    ws2812_color_t color);

/*******************************************************************************
 * @brief
 *  Set all of the available LEDs.
 *
 * @param[in] color                 Color to be changed into.
 ******************************************************************************/
void ws2812_set_all(ws2812_color_t color);

/*******************************************************************************
 * @brief
 *  Consecutively clear a number of LEDs.
 *
 * @param[in] first_pos             First LED position to be set.
 *
 * @param[in] second_pos            Second LED position to be set.
 *
 * @return                          SL_STATUS_OK if no error, otherwise error code.
 ******************************************************************************/
sl_status_t ws2812_clear_consecutive_led(uint32_t first_pos,
                                         uint32_t second_pos);

/*******************************************************************************
 * @brief
 *  Clear a specific LED.
 *
 * @param[in]  pos                  Position of LED to be cleared.
 *
 * @return                          SL_STATUS_OK if no error, otherwise error code.
 ******************************************************************************/
sl_status_t ws2812_clear_specific_led(uint32_t pos);

/*******************************************************************************
 * @brief
 *  Clear all available LEDs.
 ******************************************************************************/
void ws2812_clear_all(void);

/*******************************************************************************
 * @brief
 *  End the transaction process.
 ******************************************************************************/
void ws2812_end(void);

/*******************************************************************************
 * @brief
 *  Set the number of LED to be interacted with.
 *
 * @param[in]  led_num         The number of LEDs to be interacted with
 ******************************************************************************/
void ws2812_set_led_num(uint32_t led_num);

#ifdef __cplusplus
}
#endif

#endif // ADAFRUIT_WS2812_H_
