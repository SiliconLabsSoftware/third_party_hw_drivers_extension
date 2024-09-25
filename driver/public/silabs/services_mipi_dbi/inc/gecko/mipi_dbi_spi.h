/***************************************************************************//**
 * @file tft_4wire_interface.h
 * @brief Adafruit ST7789 Color TFT SPI interface header file
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
#ifndef MIPI_DBI_4WIRE_DEVICE_H_
#define MIPI_DBI_4WIRE_DEVICE_H_

// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include "em_gpio.h"
#if defined(MIPI_DBI_SPIDRV)
#include "spidrv.h"
#endif
#include "sl_status.h"
#include "sl_component_catalog.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MIPI_DBI_SPIDRV)
#define MIPI_DBI_SPI_4WIRE_DMA_BUFFER_SIZE_MAX DMADRV_MAX_XFER_COUNT
#else
#define MIPI_DBI_SPI_4WIRE_DMA_BUFFER_SIZE_MAX (2048)
#endif

struct mipi_dbi_spi_gpio {
  GPIO_Port_TypeDef port;
  unsigned int pin;
};

#if defined(MIPI_DBI_SPIDRV)
struct mipi_dbi_spidrv_config {
  SPIDRV_Init_t *spidrv_init;
  const struct mipi_dbi_spi_gpio dc;
};

#endif

struct mipi_dbi_spi_usart_config {
  USART_TypeDef *usart;
  uint32_t peripheral_no;
  uint32_t bitrate;
  bool cs_control;
  uint8_t clock_mode;
  const struct mipi_dbi_spi_gpio clk;
  const struct mipi_dbi_spi_gpio tx;
  const struct mipi_dbi_spi_gpio rx;
  const struct mipi_dbi_spi_gpio cs;
  const struct mipi_dbi_spi_gpio dc;
};

struct mipi_dbi_spi_gecko_data {
};

#if defined(MIPI_DBI_SPIDRV)
#define MIPI_DBI_SPIDRV_CONFIG_S2(                       \
    name,                                                \
    usart_port, bitrate, frame_length, spi_type,         \
    bit_order, clock_mode, cs_control, slave_start_mode, \
    clk_port, clk_pin,                                   \
    tx_port, tx_pin,                                     \
    rx_port, rx_pin,                                     \
    cs_port, cs_pin)                                     \
  static SPIDRV_Init_t name = {                          \
    .port = usart_port,                                  \
    .portTx = tx_port,                                   \
    .portRx = rx_port,                                   \
    .portClk = clk_port,                                 \
    .portCs = cs_port,                                   \
    .pinTx = tx_pin,                                     \
    .pinRx = rx_pin,                                     \
    .pinClk = clk_pin,                                   \
    .pinCs = cs_pin,                                     \
    .bitRate = bitrate,                                  \
    .frameLength = frame_length,                         \
    .dummyTxValue = 0,                                   \
    .type = spi_type,                                    \
    .bitOrder = bit_order,                               \
    .clockMode = clock_mode,                             \
    .csControl = cs_control,                             \
    .slaveStartMode = slave_start_mode,                  \
  };

#define MIPI_DBI_SPI_INTERFACE_DEFINE(name,                           \
                                      usart_port,                     \
                                      usart_no,                       \
                                      spi_bitrate,                    \
                                      spi_clock_mode,                 \
                                      spi_cs_control,                 \
                                      clk_port, clk_pin,              \
                                      tx_port, tx_pin,                \
                                      rx_port, rx_pin,                \
                                      cs_port, cs_pin,                \
                                      dc_port, dc_pin)                \
  MIPI_DBI_SPIDRV_CONFIG_S2(                                          \
    name ## _spidrv,                                                  \
    usart_port, spi_bitrate, 8, spidrvMaster, spidrvBitOrderMsbFirst, \
    spi_clock_mode, spi_cs_control, spidrvSlaveStartImmediate,        \
    clk_port, clk_pin,                                                \
    tx_port, tx_pin,                                                  \
    rx_port, rx_pin,                                                  \
    cs_port, cs_pin);                                                 \
  static const struct mipi_dbi_spidrv_config name = {                 \
    .spidrv_init = &name ## _spidrv,                                  \
    .dc = { .port = dc_port, .pin = dc_pin },                         \
  };

#else
SL_ENUM(SPIDRV_ClockMode_t) {
  spidrvClockMode0 = 0,           ///< SPI mode 0: CLKPOL=0, CLKPHA=0.
  spidrvClockMode1 = 1,           ///< SPI mode 1: CLKPOL=0, CLKPHA=1.
  spidrvClockMode2 = 2,           ///< SPI mode 2: CLKPOL=1, CLKPHA=0.
  spidrvClockMode3 = 3            ///< SPI mode 3: CLKPOL=1, CLKPHA=1.
};
/// SPI master chip select (CS) control scheme.
SL_ENUM(SPIDRV_CsControl_t) {
  spidrvCsControlAuto = 0,        ///< CS controlled by the SPI driver.
  spidrvCsControlApplication = 1  ///< CS controlled by the application.
};
#define MIPI_DBI_SPI_INTERFACE_DEFINE(name,              \
                                      usart_port,        \
                                      usart_no,          \
                                      spi_bitrate,       \
                                      spi_clock_mode,    \
                                      spi_cs_control,    \
                                      clk_port, clk_pin, \
                                      tx_port, tx_pin,   \
                                      rx_port, rx_pin,   \
                                      cs_port, cs_pin,   \
                                      dc_port, dc_pin)   \
  static const struct mipi_dbi_spi_usart_config name = { \
    .usart = usart_port,                                 \
    .peripheral_no = usart_no,                           \
    .bitrate = spi_bitrate,                              \
    .cs_control = spi_cs_control,                        \
    .clock_mode = spi_clock_mode,                        \
    .clk = { .port = clk_port, .pin = clk_pin },         \
    .tx = { .port = tx_port, .pin = tx_pin },            \
    .rx = { .port = rx_port, .pin = rx_pin },            \
    .cs = { .port = cs_port, .pin = cs_pin },            \
    .dc = { .port = dc_port, .pin = dc_pin },            \
  }

#endif

#ifdef __cplusplus
}
#endif

#endif /* MIPI_DBI_4WIRE_DEVICE_H_ */
