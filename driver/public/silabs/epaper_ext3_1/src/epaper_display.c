/***************************************************************************//**
 * @file epaper_display.c
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
#include <string.h>

#include "sl_sleeptimer.h"

#include "epaper_display.h"
#include "epaper_display_config.h"

static struct epd epd;

// Internal variables
static uint32_t image_data_size = 0;
static uint8_t otp_data[128];

// SPI transfer protocol for index and data transmit to COG
static void send_index_data(uint8_t index, uint8_t *data, uint32_t len);
static void send_command_data(uint8_t index, uint8_t data);

// 2 format to perform soft reset
static void soft_start_format1(uint8_t offset, uint8_t iREPEAT);
static void soft_start_format2(uint8_t offset, uint8_t iREPEAT);

void epd_init(void)
{
  // Actually for 1 color, BWR requires 2 pages.
  image_data_size = (uint32_t)EPD_VERTICAL * (uint32_t)(EPD_HORIZONTAL / 8);

  epd_driver_init(&epd);
}

void cog_initial(uint8_t *image1, uint8_t *image2)
{
  uint8_t tmp[2] = {};

  cog_power_on();

  cog_get_data_otp();

  epd.drv->spi_init();

  tmp[0] = otp_data[0x10];
  tmp[1] = 0x00;
  send_index_data(0x01, tmp, 2);

  cog_send_image(image1, image2);

  send_command_data(0x05, 0x7d);
  sl_sleeptimer_delay_millisecond(50);
  send_command_data(0x05, 0x00);
  sl_sleeptimer_delay_millisecond(1);

  send_command_data(0xd8, otp_data[0x1c]); // MS_SYNC
  send_command_data(0xd6, otp_data[0x1d]); // BVSS

  send_command_data(0xa7, 0x10);
  sl_sleeptimer_delay_millisecond(2);
  send_command_data(0xa7, 0x00);
  sl_sleeptimer_delay_millisecond(10);

  send_command_data(0x44, 0x00);
  send_command_data(0x45, 0x80);

  send_command_data(0xa7, 0x10);
  sl_sleeptimer_delay_millisecond(2);
  send_command_data(0xa7, 0x00);
  sl_sleeptimer_delay_millisecond(10);

  send_command_data(0x44, 0x06);

  // check temperature, use normal mode
  // uint8_t indexTemperature = u_temperature * 2 + 0x50;
  send_command_data(0x45, 0x28);

  send_command_data(0xa7, 0x10);
  sl_sleeptimer_delay_millisecond(2);
  send_command_data(0xa7, 0x00);
  sl_sleeptimer_delay_millisecond(10);

  send_command_data(0x60, otp_data[0x0b]); // TCON
  send_command_data(0x61, otp_data[0x1b]); // STV_DIR
  send_command_data(0x02, otp_data[0x11]); // VCOM

  cog_soft_start();

  cog_display_refresh();

  cog_power_off();
}

void cog_power_on(void)
{
  epd.drv->set_reset_pin(false);
  sl_sleeptimer_delay_millisecond(2);

  epd.drv->set_reset_pin(true);
  sl_sleeptimer_delay_millisecond(4);

  epd.drv->set_reset_pin(false);
  sl_sleeptimer_delay_millisecond(20);
}

void cog_power_off(void)
{
  while (epd.drv->get_busy_pin()) {
    // wait busy high
  }

  send_command_data(0x09, 0x7b);
  send_command_data(0x05, 0x5d);
  send_command_data(0x09, 0x7a);
  sl_sleeptimer_delay_millisecond(15);
  send_command_data(0x09, 0x00);

  while (epd.drv->get_busy_pin()) {
    // wait busy high
  }

//  GPIO_PinOutClear(EPD_BUSY_PORT, EPD_BUSY_PIN);
//  GPIO_PinOutClear(EPD_DC_PORT, EPD_DC_PIN);
//  GPIO_PinOutClear(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN);
//  GPIO_PinOutClear(EPD_RST_PORT, EPD_RST_PIN);
}

void cog_get_data_otp(void)
{
  uint8_t cmd = 0xb9;

  epd.drv->sspi_init();

  epd.drv->sspi_command_read(&epd,
                             &cmd, 1,
                             otp_data, 128);
}

void cog_send_image(uint8_t *dram1, uint8_t *dram2)
{
  send_index_data(0x13, &otp_data[0x15], 6); // DUW
  send_index_data(0x90, &otp_data[0x0C], 4); // DRFW
  send_index_data(0x12, &otp_data[0x12], 3); // RAM_RW

  // first frame
  send_index_data(0x10, dram1, image_data_size); // First frame

  send_index_data(0x12, &otp_data[0x12], 3); // RAM_RW

  // second frame
  send_index_data(0x11, dram2, image_data_size); // Second frame
}

void cog_soft_start(void)
{
  // Application note § 3.3 DC/DC soft-start
  uint8_t offsetFrame = 0x28;

  for (uint8_t stage = 0; stage < 4; stage++) {
    uint8_t offset = offsetFrame + 0x08 * stage;

    uint8_t REPEAT = otp_data[offset];
    uint8_t iREPEAT = otp_data[offset] & 0x7f;

    if ((REPEAT & 0x80) == 0) {
      soft_start_format1(offset, iREPEAT);
    } else {
      soft_start_format2(offset, iREPEAT);
    }
  }
}

// EPD Screen refresh function
void cog_display_refresh(void)
{
  while (epd.drv->get_busy_pin()) {
    sl_sleeptimer_delay_millisecond(32);
  }
  send_command_data(0x15, 0x3c);
}

// Implements SPI transfer of index and data
static void send_index_data(uint8_t index, uint8_t *data, uint32_t len)
{
  epd.drv->spi_command_write(&epd, index, data, len);
}

static void send_command_data(uint8_t index, uint8_t data)
{
  epd.drv->spi_command_write(&epd, index, &data, 1);
}

static void soft_start_format1(uint8_t offset, uint8_t iREPEAT)
{
  uint8_t BST_SW_a = otp_data[offset + 1];
  uint8_t BST_SW_b = otp_data[offset + 2];
  uint8_t DELAY_a_SCALE = otp_data[offset + 3] & 0x80;
  uint8_t DELAY_a_VALUE = otp_data[offset + 3] & 0x7f;
  uint8_t DELAY_b_SCALE = otp_data[offset + 4] & 0x80;
  uint8_t DELAY_b_VALUE = otp_data[offset + 4] & 0x7f;

  for (uint8_t i = 0; i < iREPEAT; i++) {
    send_command_data(0x09, BST_SW_a);

    if (DELAY_a_SCALE > 0) {
      sl_sleeptimer_delay_millisecond(DELAY_a_VALUE);
    } else {
      epd.drv->delay_10us(DELAY_a_VALUE);
    }

    send_command_data(0x09, BST_SW_b);

    if (DELAY_b_SCALE > 0) {
      sl_sleeptimer_delay_millisecond(DELAY_b_VALUE);
    } else {
      epd.drv->delay_10us(DELAY_b_VALUE);
    }
  }
}

static void soft_start_format2(uint8_t offset, uint8_t iREPEAT)
{
  uint8_t reg51[2];
  uint8_t iPHL_INI = otp_data[offset + 1]; // PHL_INI
  uint8_t iPHH_INI = otp_data[offset + 2]; // PHH_INI
  uint8_t PHL_VAR = otp_data[offset + 3];
  uint8_t PHH_VAR = otp_data[offset + 4];
  uint8_t BST_SW_a = otp_data[offset + 5];
  uint8_t BST_SW_b = otp_data[offset + 6];
  uint8_t DELAY_SCALE = otp_data[offset + 7] & 0x80;
  uint8_t DELAY_VALUE = otp_data[offset + 7] & 0x7f;

  for (uint8_t i = 0; i < iREPEAT; i++) {
    send_command_data(0x09, BST_SW_a);

    reg51[0] = iPHL_INI + PHL_VAR;
    reg51[1] = iPHH_INI + PHH_VAR;
    send_index_data(0x51, reg51, 2);

    send_command_data(0x09, BST_SW_b);

    if (DELAY_SCALE > 0) {
      sl_sleeptimer_delay_millisecond(DELAY_VALUE); // ms
    } else {
      epd.drv->delay_10us(DELAY_VALUE); // 10 us
    }
  }
}
