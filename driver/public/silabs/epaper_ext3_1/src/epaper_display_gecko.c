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

#include "epaper_display.h"
#include "epaper_display_config.h"

#include "spidrv.h"
#include "em_gpio.h"
#include "sl_sleeptimer.h"
#include "sl_udelay.h"

#define DELAY_10US_COUNTER             46             // Delay count

static SPIDRV_HandleData_t spi_epd_handle_data;
static SPIDRV_Handle_t spi_epd_handle = &spi_epd_handle_data;

static SPIDRV_Init_t spi_epd_init = {
  .port = SPI_EPD_PERIPHERAL,
  .portTx = SPI_EPD_TX_PORT,
  .portRx = SPI_EPD_RX_PORT,
  .portClk = SPI_EPD_CLK_PORT,
  .portCs = SPI_EPD_CS_PORT,
  .pinTx = SPI_EPD_TX_PIN,
  .pinRx = SPI_EPD_RX_PIN,
  .pinClk = SPI_EPD_CLK_PIN,
  .pinCs = SPI_EPD_CS_PIN,
  .bitRate = SPI_EPD_BITRATE,
  .frameLength = SPI_EPD_FRAME_LENGTH,
  .dummyTxValue = 0,
  .type = SPI_EPD_TYPE,
  .bitOrder = SPI_EPD_BIT_ORDER,
  .clockMode = SPI_EPD_CLOCK_MODE,
  .csControl = SPI_EPD_CS_CONTROL,
  .slaveStartMode = SPI_EPD_SLAVE_START_MODE,
};

static void sspi_init(void);
static void spi_init(void);

static void set_reset_pin(bool active);
static bool get_busy_pin(void);

static void delay_10us(uint32_t idelay);

static sl_status_t sspi_command_read(struct epd *epd,
                                     uint8_t *cmds, size_t num_cmds,
                                     uint8_t *response, size_t len);
static sl_status_t spi_command_write(struct epd *epd,
                                     uint8_t cmd,
                                     const uint8_t *data, size_t len);

static const struct epd_driver epd_driver = {
  .sspi_init = sspi_init,
  .spi_init = spi_init,
  .set_reset_pin = set_reset_pin,
  .get_busy_pin = get_busy_pin,
  .delay_10us = delay_10us,
  .sspi_command_read = sspi_command_read,
  .spi_command_write = spi_command_write,
};

static void sspi_init(void)
{
  GPIO_PinModeSet(SPI_EPD_CLK_PORT,
                  SPI_EPD_CLK_PIN,
                  gpioModePushPull, 0);
  GPIO_PinModeSet(SPI_EPD_TX_PORT,
                  SPI_EPD_TX_PIN,
                  gpioModePushPull, 0);
}

static void spi_init(void)
{
  GPIO_PinModeSet(SPI_EPD_CLK_PORT,
                  SPI_EPD_CLK_PIN,
                  gpioModePushPull, 0);
  GPIO_PinModeSet(SPI_EPD_TX_PORT,
                  SPI_EPD_TX_PIN,
                  gpioModePushPull, 0);

  GPIO_PinModeSet(SPI_EPD_RX_PORT,
                  SPI_EPD_RX_PIN,
                  gpioModeInput, 0);
  // Init SPI
  SPIDRV_Init(spi_epd_handle, &spi_epd_init);
  sl_sleeptimer_delay_millisecond(100);
}

static void set_reset_pin(bool active)
{
  if (active) {
    GPIO_PinOutClear(EPD_RST_PORT, EPD_RST_PIN);
  } else {
    GPIO_PinOutSet(EPD_RST_PORT, EPD_RST_PIN);
  }
}

static bool get_busy_pin(void)
{
  return (GPIO_PinInGet(EPD_BUSY_PORT, EPD_BUSY_PIN) == 0);
}

static void delay_10us(uint32_t idelay)
{
  sl_udelay_wait(10 * idelay);
}

static uint8_t sspi_read(void)
{
  uint8_t value = 0;

  GPIO_PinModeSet(SPI_EPD_TX_PORT,
                  SPI_EPD_TX_PIN,
                  gpioModeInput, 0);

  for (uint8_t i = 0; i < 8; i++) {
    GPIO_PinOutSet(SPI_EPD_CLK_PORT,
                   SPI_EPD_CLK_PIN);
    sl_udelay_wait(10);
    value |= GPIO_PinInGet(SPI_EPD_TX_PORT,
                           SPI_EPD_TX_PIN) << (7 - i);
    GPIO_PinOutClear(SPI_EPD_CLK_PORT,
                     SPI_EPD_CLK_PIN);
    sl_udelay_wait(10);
  }

  return value;
}

static void sspi_write(uint8_t value)
{
  GPIO_PinModeSet(SPI_EPD_TX_PORT,
                  SPI_EPD_TX_PIN,
                  gpioModePushPull, 0);

  for (uint8_t i = 0; i < 8; i++) {
    if (!(value & (1 << (7 - i)))) {
      GPIO_PinOutClear(SPI_EPD_TX_PORT,
                       SPI_EPD_TX_PIN);
    } else {
      GPIO_PinOutSet(SPI_EPD_TX_PORT,
                     SPI_EPD_TX_PIN);
    }
    sl_udelay_wait(10);

    GPIO_PinOutSet(SPI_EPD_CLK_PORT,
                   SPI_EPD_CLK_PIN);
    sl_udelay_wait(10);
    GPIO_PinOutClear(SPI_EPD_CLK_PORT,
                     SPI_EPD_CLK_PIN);
    sl_udelay_wait(10);
  }
}

static sl_status_t sspi_command_read(struct epd *epd,
                                     uint8_t *cmds, size_t num_cmds,
                                     uint8_t *response, size_t len)
{
  (void) epd;

  GPIO_PinOutClear(EPD_DC_PORT, EPD_DC_PIN);
  GPIO_PinOutClear(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN);

  while (num_cmds--) {
    sspi_write(*cmds++);
  }
  sl_sleeptimer_delay_millisecond(5);

  // Start OTP reading
  GPIO_PinOutSet(EPD_DC_PORT, EPD_DC_PIN);
  sspi_read(); // Dummy
  while (len--) {
    *response++ = sspi_read();
  }

  // End of OTP reading
  GPIO_PinOutSet(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN);

  return SL_STATUS_OK;
}

static sl_status_t spi_command_write(struct epd *epd,
                                     uint8_t cmd,
                                     const uint8_t *data, size_t len)
{
  (void) epd;

  GPIO_PinOutClear(EPD_DC_PORT, EPD_DC_PIN);
  GPIO_PinOutClear(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN);

  SPIDRV_MTransmitB(spi_epd_handle, &cmd, 1);

  GPIO_PinOutSet(EPD_DC_PORT, EPD_DC_PIN);

  while (len) {
    if (len < DMADRV_MAX_XFER_COUNT) {
      SPIDRV_MTransmitB(spi_epd_handle, data, len);
      len = 0;
    } else {
      SPIDRV_MTransmitB(spi_epd_handle, data, DMADRV_MAX_XFER_COUNT);
      len -= DMADRV_MAX_XFER_COUNT;
      data += DMADRV_MAX_XFER_COUNT;
    }
  }

  GPIO_PinOutSet(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN);

  return SL_STATUS_OK;
}

void epd_driver_init(struct epd *epd)
{
  epd->drv = &epd_driver;

  // Config pin
  GPIO_PinModeSet(EPD_BUSY_PORT, EPD_BUSY_PIN, gpioModeInput, 0);

  GPIO_PinModeSet(EPD_DC_PORT, EPD_DC_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet(EPD_RST_PORT, EPD_RST_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet(SPI_EPD_CS_PORT, SPI_EPD_CS_PIN, gpioModePushPull, 1);
}
