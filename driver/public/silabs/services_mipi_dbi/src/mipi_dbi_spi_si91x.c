/***************************************************************************//**
 * @file mipi_dbi_spi_si91x.c
 * @brief TFT Display MIPI_DBI Interface Driver source file.
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
//                       Includes
// -----------------------------------------------------------------------------
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "sl_component_catalog.h"
#include "sl_si91x_clock_manager.h"
#include "sl_si91x_peripheral_gpio.h"
#include "gpio_helper_si91x.h"
#include "sl_si91x_gspi.h"
#include "mipi_dbi.h"
#include "mipi_dbi_spi.h"

#define SOC_PLL_CLK \
  ((uint32_t) (180000000L)) // 180MHz default SoC PLL Clock
                            // as source to Processor
#define INTF_PLL_CLK \
  ((uint32_t) (180000000L)) // 180MHz default Interface PLL Clock
                            // as source to all peripherals

#define wait_spi_transfer_ready(handle)               \
  do {                                                \
    sl_gspi_status_t gspi_status;                     \
    do {                                              \
      gspi_status = sl_si91x_gspi_get_status(handle); \
    } while (gspi_status.busy);                       \
  } while (0)

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------
static volatile mipi_dbi_transfer_complete_callback_t transfer_complete_callback
  = NULL;

static sl_status_t reset(const struct mipi_dbi_device *device,
                         uint32_t delay);
static sl_status_t command_read(
  const struct mipi_dbi_device *device,
  uint8_t *cmds, size_t num_cmds,
  uint8_t *response, size_t len);
static sl_status_t command_write(
  const struct mipi_dbi_device *device,
  uint8_t cmd,
  const uint8_t *data, size_t len);
static sl_status_t write_display(
  const struct mipi_dbi_device *device,
  const uint8_t *framebuf,
  struct mipi_dbi_display_buffer_descriptor *desc,
  enum mipi_dbi_display_pixel_format pixfmt,
  mipi_dbi_transfer_complete_callback_t callback);
static void event_callback(uint32_t event);

static const struct mipi_dbi_api mipi_dbi_api = {
  .command_read = command_read,
  .command_write = command_write,
  .write_display = write_display,
  .reset = reset
};

static sl_gspi_handle_t spi_handle = NULL;

static void spi_select(const struct mipi_dbi_device *device)
{
  struct mipi_dbi_gspi_config *config =
    (struct mipi_dbi_gspi_config *)device->config;

  if (config->control_config->slave_select_mode == SL_GSPI_MASTER_SW) {
    sl_gpio_clear_pin_output(si91x_get_port(config->cs.num),
                             si91x_get_pin(config->cs.num));
  }
}

static void spi_deselect(const struct mipi_dbi_device *device)
{
  struct mipi_dbi_gspi_config *config =
    (struct mipi_dbi_gspi_config *)device->config;

  if (config->control_config->slave_select_mode == SL_GSPI_MASTER_SW) {
    sl_gpio_set_pin_output(si91x_get_port(config->cs.num),
                           si91x_get_pin(config->cs.num));
  }
}

static void set_dc_mode(const struct mipi_dbi_device *device, bool mode)
{
  struct mipi_dbi_gspi_config *config =
    (struct mipi_dbi_gspi_config *)device->config;

  if (mode) {
    sl_gpio_set_pin_output(si91x_get_port(config->dc.num),
                           si91x_get_pin(config->dc.num));
  } else {
    sl_gpio_clear_pin_output(si91x_get_port(config->dc.num),
                             si91x_get_pin(config->dc.num));
  }
}

static sl_status_t spi_write_b(const struct mipi_dbi_device *device,
                               const void *buf,
                               int count)
{
  sl_status_t status;

  wait_spi_transfer_ready(spi_handle);
  spi_select(device);
  status = sl_si91x_gspi_send_data(spi_handle,
                                   buf,
                                   count);
  wait_spi_transfer_ready(spi_handle);
  spi_deselect(device);
  return status;
}

static sl_status_t spi_write(
  const struct mipi_dbi_device *device,
  const void *buf,
  int count,
  mipi_dbi_transfer_complete_callback_t callback)
{
  sl_status_t status;

  wait_spi_transfer_ready(spi_handle);
  transfer_complete_callback = callback;

  spi_select(device);
  status = sl_si91x_gspi_send_data(spi_handle,
                                   buf,
                                   count);
  return status;
}

static sl_status_t reset(const struct mipi_dbi_device *device,
                         uint32_t delay)
{
  (void) device;
  (void) delay;
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t command_read(const struct mipi_dbi_device *device,
                                uint8_t *cmds, size_t num_cmds,
                                uint8_t *response, size_t len)
{
  (void) device;
  (void) cmds;
  (void) num_cmds;
  (void) response;
  (void) len;
  return SL_STATUS_NOT_SUPPORTED;
}

static sl_status_t command_write(
  const struct mipi_dbi_device *device,
  uint8_t cmd,
  const uint8_t *data, size_t len)
{
  sl_status_t status;

  set_dc_mode(device, false);
  status = spi_write_b(device, &cmd, 1);
  if (SL_STATUS_OK != status) {
    return status;
  }

  if (len) {
    set_dc_mode(device, true);
    status = spi_write_b(device, data, len);
  }
  return status;
}

static sl_status_t write_display(
  const struct mipi_dbi_device *device,
  const uint8_t *framebuf,
  struct mipi_dbi_display_buffer_descriptor *desc,
  enum mipi_dbi_display_pixel_format pixfmt,
  mipi_dbi_transfer_complete_callback_t callback)
{
  sl_status_t status;
  (void)pixfmt;

  set_dc_mode(device, true);
  if (callback) {
    status = spi_write(device, framebuf, desc->buf_size, callback);
  } else {
    status = spi_write_b(device, framebuf, desc->buf_size);
  }
  return status;
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

  if (transfer_complete_callback) {
    mipi_dbi_transfer_complete_callback_t callback =
      transfer_complete_callback;
    transfer_complete_callback = NULL;
    callback();
  }
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

sl_status_t mipi_dbi_device_init(struct mipi_dbi_device *device,
                                 const struct mipi_dbi_config *config)
{
  sl_status_t status;
  sl_gspi_clock_config_t clock_config;
  sl_gspi_control_config_t control_config;
//  extern sl_gspi_control_config_t gspi_configuration;
  struct mipi_dbi_gspi_config *si91x_config =
    (struct mipi_dbi_gspi_config *)config;

  default_clock_configuration();

  memcpy(&clock_config, si91x_config->clock_config, sizeof(clock_config));
  memcpy(&control_config, si91x_config->control_config, sizeof(control_config));
//  gspi_configuration = control_config;
  // Configuration of clock with the default clock parameters
  status = sl_si91x_gspi_configure_clock(&clock_config);
  if (status != SL_STATUS_OK) {
    return status;
  }
  // Pass the address of void pointer, it will be updated with the address
  // of GSPI instance which can be used in other APIs.
  status = sl_si91x_gspi_init(SL_GSPI_MASTER, &spi_handle);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Fetching the status of GSPI i.e., busy, data lost and mode fault
  // Configuration of all other parameters that are required by GSPI
  // gspi_configuration structure is from sl_si91x_gspi_init.h file.
  // The user can modify this structure with the configuration of
  // his choice by filling this structure.
  status = sl_si91x_gspi_set_configuration(spi_handle,
                                           &control_config);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Register user callback function
  status = sl_si91x_gspi_register_event_callback(spi_handle,
                                                 event_callback);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Validation for executing the API only once
  status = sl_si91x_gspi_set_slave_number(GSPI_SLAVE_0);
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = si91x_gpio_setup(si91x_config->cs.num,
                            si91x_config->cs.mode,
                            GPIO_OUTPUT,
                            1);
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = si91x_gpio_setup(si91x_config->dc.num,
                            si91x_config->dc.mode,
                            GPIO_OUTPUT,
                            1);
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = si91x_gpio_setup(si91x_config->clk.num,
                            si91x_config->clk.mode,
                            GPIO_OUTPUT,
                            0);
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = si91x_gpio_setup(si91x_config->tx.num,
                            si91x_config->tx.mode,
                            GPIO_OUTPUT,
                            0);
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = si91x_gpio_setup(si91x_config->rx.num,
                            si91x_config->rx.mode,
                            GPIO_OUTPUT,
                            0);
  if (status != SL_STATUS_OK) {
    return status;
  }

  device->api = &mipi_dbi_api;
  device->config = config;

  return SL_STATUS_OK;
}
