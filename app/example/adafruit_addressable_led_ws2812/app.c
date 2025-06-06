/***************************************************************************//**
 * @file app.c
 * @brief Main application file for WS2812
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

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include "adafruit_ws2812.h"
#include "adafruit_ws2812_color.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_si91x_gspi.h"
#else
#include "sl_spidrv_instances.h"
#include "app_log.h"
#endif

#if defined(SLI_SI917)
#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...)              app_log(__VA_ARGS__)
#endif

static mikroe_spi_handle_t app_spi_instance = NULL;
#if defined(SLI_SI917)
static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#endif

void scanner_light_effect(ws2812_color_t color,
                          uint32_t wait_ms,
                          uint32_t bar_length);

void app_init(void)
{
  app_printf("========WS2812 Addressable LED Example========\n");
#if defined(SLI_SI917)
  app_spi_instance = &gspi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif
  ws2812_init(app_spi_instance);
  app_printf("WS2812: initialized 0x%02lX.\n", ws2812_init(app_spi_instance));
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  app_printf("WS2812: red scanner light effect.\n");
  scanner_light_effect(red, 20, 25);
  app_printf("WS2812: changing color...\n");

  app_printf("WS2812: green scanner light effect.\n");
  scanner_light_effect(green, 20, 25);
  app_printf("WS2812: changing color...\n");

  app_printf("WS2812: blue scanner light effect.\n");
  scanner_light_effect(blue, 20, 25);
  app_printf("WS2812: changing color...\n");
}

void scanner_light_effect(ws2812_color_t color,
                          uint32_t wait_ms,
                          uint32_t bar_length)
{
  for (uint32_t i = 0; i < bar_length; i++) {
    ws2812_set_specific_led(i, color);
    sl_sleeptimer_delay_millisecond(wait_ms);
  }

  for (uint32_t i = 0; i < WS2812_LED_NUMBER - bar_length; i++) {
    ws2812_set_specific_led(i, no_color);
    ws2812_set_specific_led(i + bar_length, color);
    sl_sleeptimer_delay_millisecond(wait_ms);
  }

  for (uint32_t i = WS2812_LED_NUMBER - (bar_length + 1); i < WS2812_LED_NUMBER;
       i++) {
    ws2812_set_specific_led(i, no_color);
    sl_sleeptimer_delay_millisecond(wait_ms);
  }

  for (uint32_t i = WS2812_LED_NUMBER - 1;
       i > WS2812_LED_NUMBER - (bar_length + 1); i--) {
    ws2812_set_specific_led(i, color);
    sl_sleeptimer_delay_millisecond(wait_ms);
  }

  for (uint32_t i = WS2812_LED_NUMBER - 1; i > (bar_length - 1); i--) {
    ws2812_set_specific_led(i, no_color);
    ws2812_set_specific_led(i - bar_length, color);
    sl_sleeptimer_delay_millisecond(wait_ms);
  }

  for (int i = (bar_length - 1); i >= 0; i--) {
    ws2812_set_specific_led(i, no_color);
    sl_sleeptimer_delay_millisecond(wait_ms);
  }
}
