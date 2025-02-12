/***************************************************************************//**
 * @file
 * @brief Top level application functions
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

#include "sl_sleeptimer.h"
#include "app_assert.h"
#include "mikroe_ssd1306.h"
#include "mikroe_resources.h"

#if (defined(SLI_SI917))
#include "sl_si91x_gspi.h"

static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#else
#include "sl_spidrv_instances.h"
#endif

mikroe_spi_handle_t app_spi_instance = NULL;

void app_init(void)
{
  sl_status_t ret_code;

#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

  ret_code = mikroe_ssd1306_init(app_spi_instance);
  app_assert_status(ret_code);

  mikroe_ssd1306_default_cfg();

  // Change segment remap for Mikroe Bitmap
  mikroe_ssd1306_send(OLEDW_SEGREMAP, MIKROE_SSD1306_COMMAND);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint8_t i;

  mikroe_ssd1306_display_picture(oledw_img);
  sl_sleeptimer_delay_millisecond(500);
  mikroe_ssd1306_send(MIKROE_SSD1306_INVERTDISPLAY, MIKROE_SSD1306_COMMAND);
  sl_sleeptimer_delay_millisecond(500);
  mikroe_ssd1306_send(MIKROE_SSD1306_NORMALDISPLAY, MIKROE_SSD1306_COMMAND);
  sl_sleeptimer_delay_millisecond(500);
  mikroe_ssd1306_send(MIKROE_SSD1306_INVERTDISPLAY, MIKROE_SSD1306_COMMAND);
  sl_sleeptimer_delay_millisecond(500);
  mikroe_ssd1306_send(MIKROE_SSD1306_NORMALDISPLAY, MIKROE_SSD1306_COMMAND);
  sl_sleeptimer_delay_millisecond(300);

  for (i = 0xAF; i > 0x00; i--) {
    mikroe_ssd1306_set_contrast(i);
    sl_sleeptimer_delay_millisecond(5);
  }

  for (i = 0x00; i < 0xAF; i++) {
    mikroe_ssd1306_set_contrast(i);
    sl_sleeptimer_delay_millisecond(5);
  }

  mikroe_ssd1306_scroll_right(0x00, 0x05);
  sl_sleeptimer_delay_millisecond(1000);
  mikroe_ssd1306_stop_scroll();
  mikroe_ssd1306_display_picture(oledw_img);

  mikroe_ssd1306_scroll_left(0x00, 0x05);
  sl_sleeptimer_delay_millisecond(1000);
  mikroe_ssd1306_stop_scroll();
  mikroe_ssd1306_display_picture(oledw_img);

  mikroe_ssd1306_scroll_diag_right(0x00, 0x05);
  sl_sleeptimer_delay_millisecond(1000);
  mikroe_ssd1306_stop_scroll();
  mikroe_ssd1306_display_picture(oledw_img);

  mikroe_ssd1306_scroll_diag_left(0x00, 0x05);
  sl_sleeptimer_delay_millisecond(1000);
  mikroe_ssd1306_stop_scroll();
}
