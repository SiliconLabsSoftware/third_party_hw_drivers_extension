/***************************************************************************//**
 * @file epaper_display.h
 * @brief E-Paper Display Header File
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

#ifndef EPAPER_DISPLAY
#define EPAPER_DISPLAY

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct epd_driver;

// E-Paper Display (EPD)
struct epd {
  const struct epd_driver *drv;
};

struct epd_driver {
  void (*set_reset_pin)(bool active);
  bool (*get_busy_pin)(void);
  void (*sspi_init)(void);
  void (*spi_init)(void);
  void (*delay_10us)(uint32_t idelay);
  sl_status_t (*sspi_command_read)(struct epd *epd,
                                   uint8_t *cmds, size_t num_cmds,
                                   uint8_t *response, size_t len);
  sl_status_t (*spi_command_write)(struct epd *epd,
                                   uint8_t cmd,
                                   const uint8_t *data, size_t len);
};

/***************************************************************************//**
 * @brief  EPD Initialization function.
 ******************************************************************************/
void epd_init(void);

void epd_driver_init(struct epd *epd);

// -----------------------------------------------------------------------------
//                       COG related Function
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *    COG Initialization function.
 *
 * @param[in] image1 first image data frame
 * @param[in] image2 second image data frame
 ******************************************************************************/
void cog_initial(uint8_t *image1, uint8_t *image2);

/***************************************************************************//**
 * @brief  Power on COG driver.
 ******************************************************************************/
void cog_power_on(void);

/***************************************************************************//**
 * @brief  COG shutdown.
 ******************************************************************************/
void cog_power_off(void);

/***************************************************************************//**
 * @brief  Read OTP data.
 ******************************************************************************/
void cog_get_data_otp(void);

/***************************************************************************//**
 * @brief  DC-DC soft-start command.
 ******************************************************************************/
void cog_soft_start(void);

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] dram1 data is the new image data that you want to display
 * @param[in] dram2 The data definition of DRAM2 is different between
 *                  "Normal update" and "Fast update"
 *           1. Normal update: DRAM2 image is dummy data
 *           2. Fast update: DRAM2 image is the OLD image data
 ******************************************************************************/
void cog_send_image(uint8_t *dram1, uint8_t *dram2);

/***************************************************************************//**
 * @brief  Send updating command.
 ******************************************************************************/
void cog_display_refresh(void);

#ifdef __cplusplus
}
#endif

#endif // EPAPER_DISPLAY
