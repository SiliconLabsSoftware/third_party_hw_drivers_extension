/***************************************************************************//**
 * @file w5x00_platform.c
 * @brief Wiznet w5x00 platform implementation.
 * @version 0.0.1
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
#include <stdint.h>
#include <stdlib.h>
#include "w5x00_platform.h"
#include "drv_digital_out.h"

typedef struct {
  spi_master_t spi;
  digital_out_t rst_pin;
  digital_out_t cs_pin;
} w5x00_handle_t;

static w5x00_handle_t w5x00;

/***************************************************************************//**
 * Reset Chip.
 ******************************************************************************/
void w5x00_reset(void)
{
  digital_out_low(&w5x00.rst_pin);
  w5x00_delay_ms(100);
  digital_out_high(&w5x00.rst_pin);
}

/***************************************************************************//**
 * Initialize Bus IO.
 ******************************************************************************/
void w5x00_bus_init(mikroe_spi_handle_t handle)
{
  w5x00.spi.handle = handle;

  spi_master_config_t spi_cfg;
  spi_master_configure_default(&spi_cfg);

#if (MIKROE_W5500_SPI_UC == 1)
  spi_cfg.speed = MIKROE_W5500_SPI_BITRATE;
#endif

  spi_master_open(&w5x00.spi, &spi_cfg);

  pin_name_t reset = hal_gpio_pin_name(W5500_RESET_PORT,
                                       W5500_RESET_PIN);
  digital_out_init(&w5x00.rst_pin, reset);

  pin_name_t cs = hal_gpio_pin_name(MIKROE_W5500_CS_PORT,
                                    MIKROE_W5500_CS_PIN);
  digital_out_init(&w5x00.cs_pin, cs);
  digital_out_high(&w5x00.cs_pin);
}

/***************************************************************************//**
 * Select Chip.
 ******************************************************************************/
void w5x00_bus_select(void)
{
  if (w5x00.spi.handle == NULL) {
    return;
  }
  digital_out_low(&w5x00.cs_pin);
}

/***************************************************************************//**
 * Deselect Chip.
 ******************************************************************************/
void w5x00_bus_deselect(void)
{
  if (w5x00.spi.handle == NULL) {
    return;
  }
  digital_out_high(&w5x00.cs_pin);
}

/***************************************************************************//**
 * Read Chip Data From SPI Interface.
 ******************************************************************************/
uint32_t w5x00_bus_write_then_read(uint8_t *write_data_buffer,
                                   size_t length_write_data,
                                   uint8_t *read_data_buffer,
                                   size_t length_read_data)
{
  if (w5x00.spi.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  if (spi_master_write_then_read(&w5x00.spi,
                                 write_data_buffer,
                                 length_write_data,
                                 read_data_buffer,
                                 length_read_data) != SPI_MASTER_SUCCESS) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Write Data To Chip.
 ******************************************************************************/
uint32_t w5x00_bus_write(const uint8_t *buf, uint16_t len)
{
  if (w5x00.spi.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  if (spi_master_write(&w5x00.spi, (uint8_t *)buf,
                       len) != SPI_MASTER_SUCCESS) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Generate Random Number In Range.
 ******************************************************************************/
long w5x00_random(long howbig)
{
  uint32_t x = (uint32_t)rand();
  uint64_t m = (uint64_t)x * (uint64_t)howbig;
  uint32_t l = (uint32_t)m;

  if (l < (uint32_t)howbig) {
    uint32_t t = (uint32_t)-howbig;
    if (t >= (uint32_t)howbig) {
      t -= howbig;
      if (t >= (uint32_t)howbig) {
        t %= howbig;
      }
    }
    while (l < t) {
      x = (uint32_t)rand();
      m = (uint64_t)x * (uint64_t)howbig;
      l = (uint32_t)m;
    }
  }
  return (long)(m >> 32);
}

/***************************************************************************//**
 * Generate Random Number.
 ******************************************************************************/
long w5x00_random2(long howsmall, long howbig)
{
  long diff = howbig - howsmall;

  if (howsmall >= howbig) {
    return howsmall;
  }

  return w5x00_random(diff) + howsmall;
}
