/***************************************************************************//**
 * @file mipi_dbi_spi_4wire_device.h
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
#ifndef MIPI_DBI_SPI_DEVICE_H_
#define MIPI_DBI_SPI_DEVICE_H_

// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include "sl_si91x_peripheral_gpio.h"
#include "sl_si91x_gspi.h"
#include "sl_status.h"
#include "sl_component_catalog.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIPI_DBI_SPI_4WIRE_DMA_BUFFER_SIZE_MAX (4096)

struct mipi_dbi_gspi_gpio {
  uint16_t num;
  sl_gpio_mode_t mode;
};

struct mipi_dbi_gspi_config {
  const sl_gspi_clock_config_t *clock_config;
  const sl_gspi_control_config_t *control_config;
  struct mipi_dbi_gspi_gpio clk;
  struct mipi_dbi_gspi_gpio tx;
  struct mipi_dbi_gspi_gpio rx;
  struct mipi_dbi_gspi_gpio cs;
  struct mipi_dbi_gspi_gpio dc;
};

#define SI91X_INTF_PLL_CLK            180000000 // Intf pll clock frequency
#define SI91X_INTF_PLL_REF_CLK        40000000  // Intf pll reference clock freq
#define SI91X_SOC_PLL_CLK             20000000  // Soc pll clock frequency
#define SI91X_SOC_PLL_REF_CLK         40000000  // Soc pll reference clock freq
#define SI91X_INTF_PLL_500_CTRL_VALUE 0xD900    // Intf pll control value
#define SI91X_SOC_PLL_MM_COUNT_LIMIT  0xA4      // Soc pll count limit
#define SI91X_DVISION_FACTOR          0         // Division factor

#define MIPI_DBI_SPI_INTERFACE_DEFINE(name,                         \
                                      bit_rate,                     \
                                      spi_clock_mode,               \
                                      spi_slave_select_mode,        \
                                      clk_port, clk_pin, clk_mode,  \
                                      tx_port, tx_pin, tx_mode,     \
                                      rx_port, rx_pin, rx_mode,     \
                                      cs_port, cs_pin, cs_mode,     \
                                      dc_port, dc_pin, dc_mode)     \
  static const sl_gspi_clock_config_t name ## _clock_config = {     \
    .soc_pll_mm_count_value = SI91X_SOC_PLL_MM_COUNT_LIMIT,         \
    .intf_pll_500_control_value = SI91X_INTF_PLL_500_CTRL_VALUE,    \
    .intf_pll_clock = SI91X_INTF_PLL_CLK,                           \
    .intf_pll_reference_clock = SI91X_INTF_PLL_REF_CLK,             \
    .soc_pll_clock = SI91X_SOC_PLL_CLK,                             \
    .soc_pll_reference_clock = SI91X_SOC_PLL_REF_CLK,               \
    .division_factor = SI91X_DVISION_FACTOR                         \
  };                                                                \
  static const sl_gspi_control_config_t name ## _control_config = { \
    .bit_width = 8,                                                 \
    .bitrate = bit_rate,                                            \
    .clock_mode = spi_clock_mode,                                   \
    .slave_select_mode = spi_slave_select_mode,                     \
    .swap_read = 1,                                                 \
    .swap_write = 0                                                 \
  };                                                                \
  const struct mipi_dbi_gspi_config name = {                        \
    .clock_config = &name ## _clock_config,                         \
    .control_config = &name ## _control_config,                     \
    .clk = { .num = clk_pin, .mode = clk_mode },                    \
    .tx = { .num = tx_pin, .mode = tx_mode },                       \
    .rx = { .num = rx_pin, .mode = rx_mode },                       \
    .cs = { .num = cs_pin, .mode = cs_mode },                       \
    .dc = { .num = dc_pin, .mode = dc_mode },                       \
  }

#ifdef __cplusplus
}
#endif

#endif /* MIPI_DBI_SPI_DEVICE_H_ */
