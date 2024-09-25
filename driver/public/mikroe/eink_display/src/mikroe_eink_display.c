/***************************************************************************//**
 * @file mikroe_eink_display.c
 * @brief SCL Mikroe eINK display
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
//                               Includes
// -----------------------------------------------------------------------------

#include "mikroe_eink_display.h"

#if (CONFIG_EINK_DISPLAY_RESOLUTION == EINK_DISPLAY_1_54_INCH)
#define EINK154_DUMMY               0
// Macro for the ESL mode
#if ESL_MODE == 1
// Macro for the ESL display
#define EINK154_BUFFER_SIZE         200
#include "esl_tag_image_core.h"
#include "esl_tag_core.h"
#endif
#endif
// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

static eink_display_t eink_ctx;
static eink_display_cfg_t eink_cfg;
static bool initialized = false;

// -----------------------------------------------------------------------------
//                       Public Functions
// -----------------------------------------------------------------------------

/**************************************************************************//**
*  E-Paper initialization.
******************************************************************************/
sl_status_t mikroe_eink_display_init(mikroe_spi_handle_t spi_instance)
{
  if (spi_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (initialized) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  eink_ctx.spi.handle = spi_instance;
  eink_display_cfg_setup(&eink_cfg);

#if defined(EINK_DISPLAY_DC_PORT) && defined(EINK_DISPLAY_DC_PIN)
  eink_cfg.dc = hal_gpio_pin_name(EINK_DISPLAY_DC_PORT, EINK_DISPLAY_DC_PIN);
#endif

#if defined(EINK_DISPLAY_BSY_PORT) && defined(EINK_DISPLAY_BSY_PIN)
  eink_cfg.bsy = hal_gpio_pin_name(EINK_DISPLAY_BSY_PORT, EINK_DISPLAY_BSY_PIN);
#endif

#if defined(EINK_DISPLAY_RST_PORT) && defined(EINK_DISPLAY_RST_PIN)
  eink_cfg.rst = hal_gpio_pin_name(EINK_DISPLAY_RST_PORT, EINK_DISPLAY_RST_PIN);
#endif

#if defined(EINK_DISPLAY_CS_PORT) && defined(EINK_DISPLAY_CS_PIN)
  eink_cfg.cs = hal_gpio_pin_name(EINK_DISPLAY_CS_PORT, EINK_DISPLAY_CS_PIN);
#endif

#if (MIKROE_EINK_DISPLAY_SPI_UC == 1)
  eink_cfg.spi_speed = MIKROE_EINK_DISPLAY_SPI_UC_BITRATE;
#endif

  if (eink_display_init(&eink_ctx, &eink_cfg) != EINK_DISPLAY_OK) {
    return SL_STATUS_INITIALIZATION;
  }
  initialized = true;

  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper send command.
******************************************************************************/
sl_status_t mikroe_eink_display_send_command(uint8_t command)
{
  eink_display_send_command(&eink_ctx, command);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper send data.
******************************************************************************/
sl_status_t mikroe_eink_display_send_data(uint8_t data)
{
  eink_display_send_data(&eink_ctx, data);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper reset.
******************************************************************************/
sl_status_t mikroe_eink_display_reset(void)
{
  eink_display_reset(&eink_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper sleep mode.
******************************************************************************/
sl_status_t mikroe_eink_display_sleep_mode(void)
{
  eink_display_sleep_mode(&eink_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper start config.
******************************************************************************/
sl_status_t mikroe_eink_display_start_config(void)
{
  eink_display_start_config(&eink_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper set lut.
******************************************************************************/
sl_status_t mikroe_eink_display_set_lut(const uint8_t *lut, uint8_t n_bytes)
{
  eink_display_set_lut(&eink_ctx, lut, n_bytes);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper set memory pointer.
******************************************************************************/
sl_status_t mikroe_eink_display_set_memory_pointer(uint8_t x, uint8_t y)
{
  eink_display_set_mem_pointer(&eink_ctx, x, y);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper set memory area.
******************************************************************************/
sl_status_t mikroe_eink_display_set_memory_area(eink_display_xy_t *xy)
{
  eink_display_set_mem_area(&eink_ctx, xy);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper update display.
******************************************************************************/
sl_status_t mikroe_eink_display_update_display(void)
{
  eink_display_update_display(&eink_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper fill screen.
******************************************************************************/
sl_status_t mikroe_eink_display_fill_screen(uint8_t color)
{
  eink_display_fill_screen(&eink_ctx, color);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper display image.
******************************************************************************/
sl_status_t mikroe_eink_display_display_image(const uint8_t *image_buffer)
{
  eink_display_display_image(&eink_ctx, image_buffer);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  E-Paper display image for ESL.
******************************************************************************/
#if (CONFIG_EINK_DISPLAY_RESOLUTION == EINK_DISPLAY_1_54_INCH)
#if ESL_MODE == 1
enum eink154_states {
  EINK154_READY,
  EINK154_INIT,
  EINK154_WRITE,
  EINK154_DISPLAY
};

static enum eink154_states eink154_machine = EINK154_READY;

sl_status_t mikroe_eink_display_image_non_blocking(const uint8_t image_index)
{
  static uint16_t section = 0;
  static uint16_t offset = 0;
  static eink_display_xy_t xy;
  static uint16_t last_image = ESL_IMAGE_OBJECT_BASE;
  sl_status_t report_status = SL_STATUS_IN_PROGRESS;
  uint8_t buffer[EINK154_BUFFER_SIZE];

  switch (eink154_machine) {
    case EINK154_READY:
      // Check if image is available at requested slot
      report_status = esl_image_get_data(image_index,
                                         &offset,
                                         0,
                                         buffer);

      if (report_status == SL_STATUS_OK) {
        // Start actual image transfer on next call only if image exists
        eink154_machine = EINK154_INIT;
        last_image = (uint16_t)image_index;
        report_status = SL_STATUS_IN_PROGRESS;
      }
      break;

    case EINK154_INIT:
      xy.x_start = 0;
      xy.y_start = 0;
      xy.x_end = MIKROE_EINK_DISPLAY_WIDTH - 1;
      xy.y_end = MIKROE_EINK_DISPLAY_HEIGHT - 1;

      mikroe_eink_display_set_memory_area(&xy);
      mikroe_eink_display_set_memory_pointer(0, 0);

      mikroe_eink_display_send_command(EINK_DISPLAY_CMD_WRITE_RAM);

      offset = 0;
      section = 0;

      eink154_machine = EINK154_WRITE;
      break;

    case EINK154_WRITE:
      report_status = esl_image_get_data(last_image, &offset, 200, buffer);
      if (report_status == SL_STATUS_OK) {
        for (uint8_t i = 0; i < 200; i++) {
          mikroe_eink_display_send_data(buffer[i] ^ 0xFF);
        }
        report_status = SL_STATUS_IN_PROGRESS;
        section++;
        if (section >= 25) {
          eink154_machine = EINK154_DISPLAY;
        }
      } else {
        eink154_machine = EINK154_READY;
      }

      break;

    case EINK154_DISPLAY:
      report_status = SL_STATUS_OK;
      mikroe_eink_display_update_display();
      eink154_machine = EINK154_READY;

      break;
  }

  return report_status;
}

#endif
#endif
// End of the modification
