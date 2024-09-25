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
#include "sl_si91x_clock_manager.h"
#include "sl_si91x_peripheral_gpio.h"
#include "rsi_egpio.h"
#include "sl_si91x_gspi.h"

#include "epaper_display.h"
#include "epaper_display_config.h"

#include "sl_sleeptimer.h"

#define DELAY_10US_COUNTER           46             // Delay count

#define SOC_PLL_CLK \
  ((uint32_t) (180000000L)) // 180MHz default SoC PLL Clock
                            // as source to Processor
#define INTF_PLL_CLK \
  ((uint32_t) (180000000L)) // 180MHz default Interface PLL Clock
                            // as source to all peripherals

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

#define si91x_get_port(num)           ((num) / 16)
#define si91x_get_pin(num)            ((num) % 16)
#define si91x_get_gpio_num(port, pin) (((port) * 16)  + pin)

static inline uint8_t si91x_gpio_get_pad(uint16_t gpio_num)
{
  if ((gpio_num >= 6) && (gpio_num <= 12)) {
    return gpio_num - 5;
  } else if (gpio_num == 15) {
    return 8;
  } else if ((gpio_num == 31)
             || (gpio_num == 32)
             || (gpio_num == 33)
             || (gpio_num == 34)) {
    return 9;
  } else if ((gpio_num >= 46) && (gpio_num <= 57)) {
    return gpio_num - 36;
  } else {
    return 0;
  }
}

static inline sl_status_t si91x_gpio_setup(uint16_t gpio_num,
                                           sl_gpio_mode_t mode,
                                           sl_si91x_gpio_direction_t direction,
                                           uint32_t output_value)
{
  sl_gpio_port_t port = si91x_get_port(gpio_num);
  uint8_t pin = si91x_get_pin(gpio_num);

  if (!SL_GPIO_VALIDATE_PORT(port)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (port == SL_ULP_GPIO_PORT) {
    if (!SL_GPIO_VALIDATE_ULP_PORT_PIN(port, pin)) {
      return SL_STATUS_INVALID_PARAMETER;
    }
  } else {
    if (!SL_GPIO_NDEBUG_PORT_PIN(port, pin)) {
      return SL_STATUS_INVALID_PARAMETER;
    }
  }

  if (port == SL_ULP_GPIO_PORT) {
    sl_si91x_gpio_enable_clock(ULPCLK_GPIO);
    sl_si91x_gpio_enable_ulp_pad_receiver(pin);
  } else {
    sl_si91x_gpio_enable_clock(M4CLK_GPIO);
    sl_si91x_gpio_enable_pad_receiver(gpio_num);
    uint8_t pad = si91x_gpio_get_pad(gpio_num);
    if (pad) {
      sl_si91x_gpio_enable_pad_selection(pad);
    }
  }

  sl_gpio_set_pin_mode(port, pin, mode, output_value);
  sl_si91x_gpio_set_pin_direction(port, pin, direction);
  return SL_STATUS_OK;
}

/*******************************************************************************
 * default_clock_configuration
 ******************************************************************************/
static void default_clock_configuration(void)
{
  // Core Clock runs at 180MHz SOC PLL Clock
  sl_si91x_clock_manager_m4_set_core_clk(M4_SOCPLLCLK, SOC_PLL_CLK);

  // All peripherals' source to be set to Interface PLL Clock
  // and it runs at 180MHz
  sl_si91x_clock_manager_set_pll_freq(INFT_PLL,
                                      INTF_PLL_CLK,
                                      PLL_REF_CLK_VAL_XTAL);
}

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

  default_clock_configuration();

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
  si91x_gpio_setup(RTE_GSPI_MASTER_CS0_PIN,
                   SL_GPIO_MODE_0,
                   GPIO_OUTPUT,
                   0);
  si91x_gpio_setup(RTE_GSPI_MASTER_CLK_PIN,
                   SL_GPIO_MODE_0,
                   GPIO_OUTPUT,
                   0);
}

static void spi_init(void)
{
  spi_config();
}

static uint8_t sspi_read(void)
{
  uint8_t value = 0;

  si91x_gpio_setup(RTE_GSPI_MASTER_CLK_PIN,
                   SL_GPIO_MODE_0,
                   GPIO_OUTPUT,
                   0);
  si91x_gpio_setup(RTE_GSPI_MASTER_MOSI_PIN,
                   SL_GPIO_MODE_0,
                   GPIO_INPUT,
                   0);

  for (uint8_t i = 0; i < 8; i++) {
    sl_gpio_set_pin_output(si91x_get_port(RTE_GSPI_MASTER_CLK_PIN),
                           si91x_get_pin(RTE_GSPI_MASTER_CLK_PIN));
    delay_10us(1);
    value |= (sl_gpio_get_pin_input(
                si91x_get_port(RTE_GSPI_MASTER_MOSI_PIN),
                si91x_get_pin(RTE_GSPI_MASTER_MOSI_PIN)) ? 1 : 0) << (7 - i);
    sl_gpio_clear_pin_output(si91x_get_port(RTE_GSPI_MASTER_CLK_PIN),
                             si91x_get_pin(RTE_GSPI_MASTER_CLK_PIN));
    delay_10us(1);
  }

  return value;
}

static void sspi_write(uint8_t value)
{
  si91x_gpio_setup(RTE_GSPI_MASTER_CLK_PIN,
                   SL_GPIO_MODE_0,
                   GPIO_OUTPUT,
                   0);
  si91x_gpio_setup(RTE_GSPI_MASTER_MOSI_PIN,
                   SL_GPIO_MODE_0,
                   GPIO_OUTPUT,
                   0);

  for (uint8_t i = 0; i < 8; i++) {
    if (!(value & (1 << (7 - i)))) {
      sl_gpio_clear_pin_output(si91x_get_port(RTE_GSPI_MASTER_MOSI_PIN),
                               si91x_get_pin(RTE_GSPI_MASTER_MOSI_PIN));
    } else {
      sl_gpio_set_pin_output(si91x_get_port(RTE_GSPI_MASTER_MOSI_PIN),
                             si91x_get_pin(RTE_GSPI_MASTER_MOSI_PIN));
    }
    delay_10us(1);

    sl_gpio_set_pin_output(si91x_get_port(RTE_GSPI_MASTER_CLK_PIN),
                           si91x_get_pin(RTE_GSPI_MASTER_CLK_PIN));
    delay_10us(1);
    sl_gpio_clear_pin_output(si91x_get_port(RTE_GSPI_MASTER_CLK_PIN),
                             si91x_get_pin(RTE_GSPI_MASTER_CLK_PIN));
    delay_10us(1);
  }
}

static void set_reset_pin(bool active)
{
  if (active) {
    sl_gpio_clear_pin_output(si91x_get_port(EPD_RST_PIN),
                             si91x_get_pin(EPD_RST_PIN));
  } else {
    sl_gpio_set_pin_output(si91x_get_port(EPD_RST_PIN),
                           si91x_get_pin(EPD_RST_PIN));
  }
}

static bool get_busy_pin(void)
{
  return (sl_gpio_get_pin_input(si91x_get_port(EPD_BUSY_PIN),
                                si91x_get_pin(EPD_BUSY_PIN)) == 0);
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

  sl_gpio_clear_pin_output(si91x_get_port(EPD_DC_PIN),
                           si91x_get_pin(EPD_DC_PIN));
  sl_gpio_clear_pin_output(si91x_get_port(RTE_GSPI_MASTER_CS0_PIN),
                           si91x_get_pin(RTE_GSPI_MASTER_CS0_PIN));

  while (num_cmds--) {
    sspi_write(*cmds++);
  }
  sl_sleeptimer_delay_millisecond(5);

  // Start OTP reading
  sl_gpio_set_pin_output(si91x_get_port(EPD_DC_PIN),
                         si91x_get_pin(EPD_DC_PIN));
  sspi_read(); // Dummy
  while (len--) {
    *response++ = sspi_read();
  }

  // End of OTP reading
  sl_gpio_set_pin_output(si91x_get_port(RTE_GSPI_MASTER_CS0_PIN),
                         si91x_get_pin(RTE_GSPI_MASTER_CS0_PIN));

  return SL_STATUS_OK;
}

static sl_status_t spi_command_write(struct epd *epd,
                                     uint8_t cmd,
                                     const uint8_t *data, size_t len)
{
  (void) epd;
  sl_status_t status;

  sl_gpio_clear_pin_output(si91x_get_port(EPD_DC_PIN),
                           si91x_get_pin(EPD_DC_PIN));
  sl_gpio_clear_pin_output(si91x_get_port(RTE_GSPI_MASTER_CS0_PIN),
                           si91x_get_pin(RTE_GSPI_MASTER_CS0_PIN));

  status = sl_si91x_gspi_send_data(spi_epd_handle, &cmd, 1);
  if (SL_STATUS_OK != status) {
    return status;
  }

  wait_spi_transfer_ready(spi_epd_handle);

  sl_gpio_set_pin_output(si91x_get_port(EPD_DC_PIN),
                         si91x_get_pin(EPD_DC_PIN));

  status = sl_si91x_gspi_send_data(spi_epd_handle, data, len);
  if (SL_STATUS_OK != status) {
    return status;
  }

  wait_spi_transfer_ready(spi_epd_handle);

  sl_gpio_set_pin_output(si91x_get_port(RTE_GSPI_MASTER_CS0_PIN),
                         si91x_get_pin(RTE_GSPI_MASTER_CS0_PIN));

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

  // Config pin
  si91x_gpio_setup(EPD_BUSY_PIN,
                   SL_GPIO_MODE_0,
                   GPIO_INPUT,
                   0);
  si91x_gpio_setup(EPD_DC_PIN,
                   SL_GPIO_MODE_0,
                   GPIO_OUTPUT,
                   1);
  si91x_gpio_setup(EPD_RST_PIN,
                   SL_GPIO_MODE_0,
                   GPIO_OUTPUT,
                   1);
  si91x_gpio_setup(RTE_GSPI_MASTER_CS0_PIN,
                   SL_GPIO_MODE_0,
                   GPIO_OUTPUT,
                   1);
}
