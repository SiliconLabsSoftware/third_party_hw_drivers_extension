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
#include "sl_si91x_peripheral_gpio.h"
#include "sl_si91x_gspi_common_config.h"
#include "sl_si91x_gspi.h"

#include "epaper_display.h"
#include "epaper_display_config.h"
#include "drv_digital_in.h"
#include "drv_digital_out.h"

#include "sl_sleeptimer.h"

#define DELAY_10US_COUNTER           46             // Delay count

#define GSPI_BUFFER_SIZE             1024      // Size of buffer
#define GSPI_INTF_PLL_CLK            180000000 // Intf pll clock frequency
#define GSPI_INTF_PLL_REF_CLK        40000000  // Intf pll reference clock frequency
#define GSPI_SOC_PLL_CLK             20000000  // Soc pll clock frequency
#define GSPI_SOC_PLL_REF_CLK         40000000  // Soc pll reference clock frequency
#define GSPI_INTF_PLL_500_CTRL_VALUE 0xD900    // Intf pll control value
#define GSPI_SOC_PLL_MM_COUNT_LIMIT  0xA4      // Soc pll count limit
#define GSPI_DVISION_FACTOR          0         // Division factor
#define GSPI_SWAP_READ_DATA          1         // true to enable and false to disable swap read
#define GSPI_SWAP_WRITE_DATA         0         // true to enable and false to disable swap write
#define GSPI_BITRATE                 10000000  // Bitrate for setting the clock division factor
#define GSPI_BIT_WIDTH               8         // Default Bit width
#define GSPI_MAX_BIT_WIDTH           16        // Maximum Bit width

static sl_gspi_handle_t spi_epd_handle = NULL;

static digital_in_t busy_pin;
static digital_out_t dc_pin;
static digital_out_t rst_pin;
static digital_out_t cs_pin;
static digital_out_t sck_pin;
static digital_out_t mosi_pin;

// read/write for OTP
static uint8_t sspi_read();
static void sspi_write(uint8_t value);

static void event_callback(uint32_t event);

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

#define wait_spi_transfer_ready(handle)               \
  do {                                                \
    sl_gspi_status_t gspi_status;                     \
    do {                                              \
      gspi_status = sl_si91x_gspi_get_status(handle); \
    } while (gspi_status.busy);                       \
  } while (0)

static sl_status_t spi_config(void)
{
  sl_status_t status;
  sl_gspi_clock_config_t clock_config = {
    .soc_pll_mm_count_value = GSPI_SOC_PLL_MM_COUNT_LIMIT,
    .intf_pll_500_control_value = GSPI_INTF_PLL_500_CTRL_VALUE,
    .intf_pll_clock = GSPI_INTF_PLL_CLK,
    .intf_pll_reference_clock = GSPI_INTF_PLL_REF_CLK,
    .soc_pll_clock = GSPI_SOC_PLL_CLK,
    .soc_pll_reference_clock = GSPI_SOC_PLL_REF_CLK,
    .division_factor = GSPI_DVISION_FACTOR
  };
  sl_gspi_control_config_t control_config = {
    .bit_width = GSPI_BIT_WIDTH,
    .bitrate = GSPI_BITRATE,
    .clock_mode = SL_GSPI_MODE_0,
    .slave_select_mode = SL_GSPI_MASTER_HW_OUTPUT,
    .swap_read = GSPI_SWAP_READ_DATA,
    .swap_write = GSPI_SWAP_WRITE_DATA,
  };

//  gspi_configuration = control_config;
  // Configuration of clock with the default clock parameters
  status = sl_si91x_gspi_configure_clock(&clock_config);
  if (status != SL_STATUS_OK) {
    return status;
  }
  // Pass the address of void pointer, it will be updated with the address
  // of GSPI instance which can be used in other APIs.
  status = sl_si91x_gspi_init(SL_GSPI_MASTER, &spi_epd_handle);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Fetching the status of GSPI i.e., busy, data lost and mode fault
  // Configuration of all other parameters that are required by GSPI
  // gspi_configuration structure is from sl_si91x_gspi_init.h file.
  // The user can modify this structure with the configuration of
  // his choice by filling this structure.
  status = sl_si91x_gspi_set_configuration(spi_epd_handle,
                                           &control_config);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Register user callback function
  status = sl_si91x_gspi_register_event_callback(spi_epd_handle,
                                                 event_callback);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Validation for executing the API only once
  status = sl_si91x_gspi_set_slave_number(GSPI_SLAVE_0);
  if (status != SL_STATUS_OK) {
    return status;
  }
  return SL_STATUS_OK;
}

static void sspi_init(void)
{
  pin_name_t pin = hal_gpio_pin_name(SL_GSPI_MASTER_MOSI__PORT,
                                     SL_GSPI_MASTER_MOSI__PIN);
  digital_out_init(&mosi_pin, pin);
  digital_out_low(&mosi_pin);
  pin = hal_gpio_pin_name(SL_GSPI_MASTER_SCK__PORT, SL_GSPI_MASTER_SCK__PIN);
  digital_out_init(&sck_pin, pin);
  digital_out_low(&sck_pin);
}

static void spi_init(void)
{
  spi_config();
}

static uint8_t sspi_read(void)
{
  uint8_t value = 0;

  digital_out_low(&sck_pin);
  sl_si91x_gpio_set_pin_direction(mosi_pin.pin.base,
                                  mosi_pin.pin.mask,
                                  (sl_si91x_gpio_direction_t)GPIO_INPUT);

  for (uint8_t i = 0; i < 8; i++) {
    digital_out_high(&sck_pin);
    delay_10us(1);
    value |= digital_in_read((digital_in_t *)&mosi_pin) << (7 - i);
    digital_out_low(&sck_pin);
    delay_10us(1);
  }

  return value;
}

static void sspi_write(uint8_t value)
{
  digital_out_low(&sck_pin);
  sl_si91x_gpio_set_pin_direction(mosi_pin.pin.base,
                                  mosi_pin.pin.mask,
                                  (sl_si91x_gpio_direction_t)GPIO_OUTPUT);

  for (uint8_t i = 0; i < 8; i++) {
    if (!(value & (1 << (7 - i)))) {
      digital_out_low(&mosi_pin);
    } else {
      digital_out_high(&mosi_pin);
    }
    delay_10us(1);

    digital_out_high(&sck_pin);
    delay_10us(1);
    digital_out_low(&sck_pin);
    delay_10us(1);
  }
}

static void set_reset_pin(bool active)
{
  if (active) {
    digital_out_low(&rst_pin);
  } else {
    digital_out_high(&rst_pin);
  }
}

static bool get_busy_pin(void)
{
  return (digital_in_read(&busy_pin) == 0);
}

static void delay_10us(uint32_t idelay)
{
  for (uint32_t x = 0; x < DELAY_10US_COUNTER * idelay; x++) {
    __NOP();
  }
}

static sl_status_t sspi_command_read(struct epd *epd,
                                     uint8_t *cmds, size_t num_cmds,
                                     uint8_t *response, size_t len)
{
  (void) epd;

  digital_out_low(&dc_pin);
  digital_out_low(&cs_pin);

  while (num_cmds--) {
    sspi_write(*cmds++);
  }
  sl_sleeptimer_delay_millisecond(5);

  // Start OTP reading
  digital_out_high(&dc_pin);
  sspi_read(); // Dummy
  while (len--) {
    *response++ = sspi_read();
  }

  // End of OTP reading
  digital_out_high(&cs_pin);

  return SL_STATUS_OK;
}

static sl_status_t spi_command_write(struct epd *epd,
                                     uint8_t cmd,
                                     const uint8_t *data, size_t len)
{
  (void) epd;
  sl_status_t status;

  digital_out_low(&dc_pin);
  digital_out_low(&cs_pin);

  status = sl_si91x_gspi_send_data(spi_epd_handle, &cmd, 1);
  if (SL_STATUS_OK != status) {
    return status;
  }

  wait_spi_transfer_ready(spi_epd_handle);

  digital_out_high(&dc_pin);

  status = sl_si91x_gspi_send_data(spi_epd_handle, data, len);
  if (SL_STATUS_OK != status) {
    return status;
  }

  wait_spi_transfer_ready(spi_epd_handle);

  digital_out_high(&cs_pin);

  return SL_STATUS_OK;
}

static void event_callback(uint32_t event)
{
  switch (event) {
    case SL_GSPI_TRANSFER_COMPLETE:
      break;
    case SL_GSPI_DATA_LOST:
      break;
    case SL_GSPI_MODE_FAULT:
      break;
  }
}

void epd_driver_init(struct epd *epd)
{
  epd->drv = &epd_driver;

  pin_name_t pin = hal_gpio_pin_name(EPD_BUSY_PORT, EPD_BUSY_PIN);
  digital_in_init(&busy_pin, pin);

  pin = hal_gpio_pin_name(EPD_DC_PORT, EPD_DC_PIN);
  digital_out_init(&dc_pin, pin);
  digital_out_high(&dc_pin);

  pin = hal_gpio_pin_name(EPD_RST_PORT, EPD_RST_PIN);
  digital_out_init(&rst_pin, pin);
  digital_out_high(&rst_pin);

  pin = hal_gpio_pin_name(SL_GSPI_MASTER_CS0__PORT, SL_GSPI_MASTER_CS0__PIN);
  digital_out_init(&cs_pin, pin);
  digital_out_high(&cs_pin);
}
