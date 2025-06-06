/***************************************************************************//**
 * @file adafruit_ws2812.c
 * @brief Source file for WS2812
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
#include "adafruit_ws2812.h"

/*******************************************************************************
 * @brief
 *  Set the frame before sending it.
 *
 * @param[in] pos                   First LED position to be set.
 *
 * @param[in] input_color           Second LED position to be set.
 ******************************************************************************/
static void ws2812_set_frame(uint32_t pos, ws2812_color_t input_color);

/*******************************************************************************
 * @brief
 *  Send the frame.
 ******************************************************************************/
static void ws2812_send_frame(void);

static ws2812_data_transmission_t ws2812_data_to_send[WS2812_LED_NUMBER] =
{ 0 };
static ws2812_device_t this_ws2812_device = {
  .num_leds = WS2812_LED_NUMBER
};

/*******************************************************************************
 * @brief
 *  Initialize the WS2812.
 ******************************************************************************/
sl_status_t ws2812_init(mikroe_spi_handle_t spi_master_instance)
{
  if (NULL == spi_master_instance) {
    return SL_STATUS_NULL_POINTER;
  }

  spi_master_config_t ws2812_spi_configuration;

  this_ws2812_device.spi.handle = spi_master_instance;

  spi_master_configure_default(&ws2812_spi_configuration);

  ws2812_spi_configuration.speed = WS2812_SPI_SPEED;

  if (spi_master_open(&this_ws2812_device.spi,
                      &ws2812_spi_configuration) == SPI_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  memset(ws2812_data_to_send, WS2812_BIT_ZERO, sizeof(ws2812_data_to_send));

  return SL_STATUS_OK;
}

/*******************************************************************************
 * @brief
 *  Set a number of LEDs consecutively.
 ******************************************************************************/
sl_status_t ws2812_set_consecutive_leds(uint32_t first_pos,
                                        uint32_t second_pos,
                                        ws2812_color_t color)
{
  int32_t pos_length = second_pos - first_pos;

  if ((pos_length <= 0) || (second_pos >= this_ws2812_device.num_leds)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  for (uint32_t led_index = first_pos; led_index <= second_pos; led_index++) {
    ws2812_set_frame(led_index, color);
  }

  ws2812_send_frame();
  return SL_STATUS_OK;
}

/*******************************************************************************
 * @brief
 *  Set a specific LED.
 ******************************************************************************/
sl_status_t ws2812_set_specific_led(uint32_t pos,
                                    ws2812_color_t color)
{
  if (pos >= this_ws2812_device.num_leds) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  ws2812_set_frame(pos, color);
  ws2812_send_frame();
  return SL_STATUS_OK;
}

/*******************************************************************************
 * @brief
 *  Set all of the available LEDs.
 ******************************************************************************/
void ws2812_set_all(ws2812_color_t color)
{
  for (uint32_t led_index = 0; led_index < this_ws2812_device.num_leds;
       led_index++) {
    ws2812_set_frame(led_index, color);
  }

  ws2812_send_frame();
}

/*******************************************************************************
 * @brief
 *  Consecutively clear a number of LEDs.
 ******************************************************************************/
sl_status_t ws2812_clear_consecutive_led(uint32_t first_pos,
                                         uint32_t second_pos)
{
  ws2812_color_t no_color = {
    .red = 0,
    .green = 0,
    .blue = 0,
    .brightness = 0
  };

  return ws2812_set_consecutive_leds(first_pos, second_pos, no_color);
}

/*******************************************************************************
 * @brief
 *  Clear a specific LED.
 ******************************************************************************/
sl_status_t ws2812_clear_specific_led(uint32_t pos)
{
  ws2812_color_t no_color = {
    .red = 0,
    .green = 0,
    .blue = 0,
    .brightness = 0
  };
  return ws2812_set_specific_led(pos, no_color);
}

/*******************************************************************************
 * @brief
 *  Clear all available LEDs.
 ******************************************************************************/
void ws2812_clear_all(void)
{
  ws2812_color_t no_color = {
    .red = 0,
    .green = 0,
    .blue = 0,
    .brightness = 0
  };
  ws2812_set_all(no_color);
}

/*******************************************************************************
 * @brief
 *  End the transaction process.
 ******************************************************************************/
void ws2812_end(void)
{
  memset(ws2812_data_to_send, WS2812_BIT_RESET, sizeof(ws2812_data_to_send));
  spi_master_close(&this_ws2812_device.spi);
}

/*******************************************************************************
 * @brief
 *  Set the number of LED to be interacted with.
 ******************************************************************************/
void ws2812_set_led_num(uint32_t led_num)
{
  if (led_num > WS2812_LED_NUMBER) {
    led_num = WS2812_LED_NUMBER;
  }
  this_ws2812_device.num_leds = led_num;
}

static void ws2812_set_frame(uint32_t pos, ws2812_color_t input_color)
{
  uint8_t index = 0;

  uint8_t colors[3] = { input_color.green, input_color.red, input_color.blue };
  if ((input_color.brightness > 0) && (input_color.brightness <= 100)) {
    for (uint8_t c = 0; c < 3; c++) {
      colors[c] = (uint8_t)((uint32_t)colors[c] * input_color.brightness / 100);
    }
  } else {
    // If brightness is zero - set all color components to zero
    memset(colors, 0, sizeof(colors));
  }

  for (uint8_t c = 0; c < 3; c++) {
    for (int8_t i = 7; i >= 0; i--) {
      if ((colors[c] >> i) & 1) {
        ws2812_data_to_send[pos].ws2812_frame[index++] = WS2812_BIT_ONE;
      } else {
        ws2812_data_to_send[pos].ws2812_frame[index++] = WS2812_BIT_ZERO;
      }
    }
  }
}

static void ws2812_send_frame(void)
{
  uint8_t data_to_send[WS2812_FRAME_LENGTH * WS2812_LED_NUMBER] = { 0 };
  memcpy(data_to_send, ws2812_data_to_send, sizeof(ws2812_data_to_send));
  spi_master_write(&this_ws2812_device.spi, data_to_send, sizeof(data_to_send));
}
