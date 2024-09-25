/***************************************************************************//**
 * @file drv_ssi_master_si91x
 * @brief mikroSDK 2.0 Click Peripheral Drivers - SSI Master for Si91x
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "drv_spi_master.h"
#include "drv_digital_out.h"
#include "sl_si91x_ssi.h"
#include "sl_si91x_clock_manager.h"
#include "rsi_rom_clks.h"

#define SSI_MASTER_BUFFER_SIZE             1024      // Length of data to be sent through SPI
#define SSI_MASTER_DIVISION_FACTOR         0         // Division Factor
#define SSI_MASTER_INTF_PLL_CLK            180000000 // PLL Clock frequency
#define SSI_MASTER_INTF_PLL_REF_CLK        40000000  // PLL Ref Clock frequency
#define SSI_MASTER_SOC_PLL_CLK             20000000  // SOC PLL Clock frequency
#define SSI_MASTER_SOC_PLL_REF_CLK         40000000  // SOC PLL REFERENCE CLOCK frequency
#define SSI_MASTER_INTF_PLL_500_CTRL_VALUE 0xD900    // Interface PLL control value
#define SSI_MASTER_SOC_PLL_MM_COUNT_LIMIT  0xA4      // SOC PLL count limit
#define SSI_MASTER_BIT_WIDTH               8         // SSI bit width
#define SSI_MASTER_BAUDRATE                10000000  // SSI baudrate
#define SSI_MASTER_MAX_BIT_WIDTH           16        // Maximum Bit width
#define SSI_MASTER_RECEIVE_SAMPLE_DELAY    0         // By default sample delay is 0

#define SOC_PLL_CLK                        ((uint32_t)(180000000)) // 180MHz default SoC PLL Clock as source to Processor
#define INTF_PLL_CLK                       ((uint32_t)(180000000)) // 180MHz default Interface PLL Clock as source to all peripherals
#define QSPI_ODD_DIV_ENABLE                0         // Odd division enable for QSPI clock
#define QSPI_SWALLO_ENABLE                 0         // Swallo enable for QSPI clock
#define QSPI_DIVISION_FACTOR               0         // Division factor for QSPI clock

static spi_master_t *_owner = NULL;
static sl_ssi_handle_t ssi_driver_handle = NULL;
static uint32_t last_spi_speed_used;
static spi_master_mode_t last_spi_mode_used;

extern sl_ssi_control_config_t ssi_configuration;

static spi_master_chip_select_polarity_t spi_master_chip_select_polarity =
  SPI_MASTER_CHIP_SELECT_DEFAULT_POLARITY;

static void callback_event(uint32_t event);
static err_t _acquire(spi_master_t *obj, bool obj_open_state);
static void spi_master_configure_gpio_pin(digital_out_t *out, pin_name_t name);
static err_t spi_master_set_configuration(spi_master_t *obj);
static void default_clock_configuration(void);

void spi_master_configure_default(spi_master_config_t *config)
{
  config->default_write_data = 0;
  config->sck = 0xFFFFFFFF;
  config->miso = 0xFFFFFFFF;
  config->mosi = 0xFFFFFFFF;
  config->speed = 1000000;
  config->mode = SPI_MASTER_MODE_DEFAULT;
}

err_t spi_master_open(spi_master_t *obj, spi_master_config_t *config)
{
  sl_status_t status;
  spi_master_config_t *p_config = &obj->config;
  sl_ssi_clock_config_t ssi_clock_config = {
    .soc_pll_mm_count_value = SSI_MASTER_SOC_PLL_MM_COUNT_LIMIT,
    .intf_pll_500_control_value = SSI_MASTER_INTF_PLL_500_CTRL_VALUE,
    .intf_pll_clock = SSI_MASTER_INTF_PLL_CLK,
    .intf_pll_reference_clock = SSI_MASTER_INTF_PLL_REF_CLK,
    .soc_pll_clock = SSI_MASTER_SOC_PLL_CLK,
    .soc_pll_reference_clock = SSI_MASTER_SOC_PLL_REF_CLK,
    .division_factor = SSI_MASTER_DIVISION_FACTOR
  };

  memcpy(p_config, config, sizeof(spi_master_config_t));
  if (_acquire(obj, true) == ACQUIRE_FAIL) {
    return SPI_MASTER_ERROR;
  }

  // default clock configuration by application common for whole system
  default_clock_configuration();

  // Configuration of clock with the default clock parameters
  status = sl_si91x_ssi_configure_clock(&ssi_clock_config);
  if (status != SL_STATUS_OK) {
    return SPI_MASTER_ERROR;
  }
  // Initialize the SSI driver
  status = sl_si91x_ssi_init(SL_SSI_MASTER_ACTIVE, &ssi_driver_handle);
  if (status != SL_STATUS_OK) {
    return SPI_MASTER_ERROR;
  }

  if (spi_master_set_configuration(obj) != SPI_MASTER_SUCCESS) {
    return SPI_MASTER_ERROR;
  }
  // Register user callback function
  status = sl_si91x_ssi_register_event_callback(ssi_driver_handle,
                                                callback_event);
  if (status != SL_STATUS_OK) {
    return SPI_MASTER_ERROR;
  }

  // Validation for executing the API only once
  status = sl_si91x_ssi_set_slave_number(SSI_SLAVE_0);
  if (status != SL_STATUS_OK) {
    return SPI_MASTER_ERROR;
  }

  return SPI_MASTER_SUCCESS;
}

void spi_master_select_device(pin_name_t chip_select)
{
  digital_out_t struct_cs;

  if (chip_select == 0xFFFFFFFF) {
    return;
  }

  spi_master_configure_gpio_pin(&struct_cs, chip_select);

  if (spi_master_chip_select_polarity
      == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH) {
    digital_out_high(&struct_cs);
  } else {
    digital_out_low(&struct_cs);
  }
}

void spi_master_deselect_device(pin_name_t chip_select)
{
  digital_out_t struct_cs;

  if (chip_select == 0xFFFFFFFF) {
    return;
  }

  spi_master_configure_gpio_pin(&struct_cs, chip_select);

  if (spi_master_chip_select_polarity
      == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH) {
    digital_out_low(&struct_cs);
  } else {
    digital_out_high(&struct_cs);
  }
}

void spi_master_set_chip_select_polarity(
  spi_master_chip_select_polarity_t polarity)
{
  if (polarity == SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH) {
    spi_master_chip_select_polarity =
      SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_HIGH;
  } else {
    spi_master_chip_select_polarity =
      SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW;
  }
}

/***************************************************************************//**
 * Set SPI Master driver communication speed.
 ******************************************************************************/
err_t spi_master_set_speed(spi_master_t *obj, uint32_t speed)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  obj->config.speed = speed;

  return spi_master_set_configuration(obj);
}

/***************************************************************************//**
 * Set SPI Master driver communication mode.
 ******************************************************************************/
err_t spi_master_set_mode(spi_master_t *obj, spi_master_mode_t mode)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  obj->config.mode = mode;

  return spi_master_set_configuration(obj);
}

/***************************************************************************//**
 * Set SPI Master driver default ( dummy ) write data.
 ******************************************************************************/
err_t spi_master_set_default_write_data(spi_master_t *obj,
                                        uint8_t default_write_data)
{
  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  obj->config.default_write_data = default_write_data;
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Write byte to SPI bus.
 ******************************************************************************/
err_t spi_master_write(spi_master_t *obj,
                       uint8_t *write_data_buffer,
                       size_t write_data_length)
{
  sl_status_t status;

  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  if ((last_spi_speed_used != obj->config.speed)
      || (last_spi_mode_used != obj->config.mode)) {
    // Update the config
    spi_master_set_configuration(obj);
  }

  status = sl_si91x_ssi_send_data(ssi_driver_handle,
                                  write_data_buffer,
                                  write_data_length);
  if (status != SL_STATUS_OK) {
    return SPI_MASTER_ERROR;
  }
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Read byte from SPI bus.
 ******************************************************************************/
err_t spi_master_read(spi_master_t *obj,
                      uint8_t *read_data_buffer,
                      size_t read_data_length)
{
  sl_status_t status;
  uint8_t write_data_buffer[read_data_length];

  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  if ((last_spi_speed_used != obj->config.speed)
      || (last_spi_mode_used != obj->config.mode)) {
    // Update the config
    spi_master_set_configuration(obj);
  }

  // The data transmit buffer will be populated with user-defined data
  for (size_t i = 0; i < read_data_length; i++) {
    write_data_buffer[i] = _owner->config.default_write_data;
  }

  status = sl_si91x_ssi_transfer_data(ssi_driver_handle,
                                      write_data_buffer,
                                      read_data_buffer,
                                      read_data_length);
  if (status != SL_STATUS_OK) {
    return SPI_MASTER_ERROR;
  }
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Exchange bytes on SPI bus.
 ******************************************************************************/
err_t spi_master_exchange(spi_master_t *obj,
                          uint8_t *write_data_buffer,
                          uint8_t *read_data_buffer,
                          size_t exchange_data_length)
{
  sl_status_t status;

  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  if ((last_spi_speed_used != obj->config.speed)
      || (last_spi_mode_used != obj->config.mode)) {
    // Update the config
    spi_master_set_configuration(obj);
  }

  status = sl_si91x_ssi_transfer_data(ssi_driver_handle,
                                      write_data_buffer,
                                      read_data_buffer,
                                      exchange_data_length);
  if (status != SL_STATUS_OK) {
    return SPI_MASTER_ERROR;
  }
  return SPI_MASTER_SUCCESS;
}

/***************************************************************************//**
 * Perform a sequence of SPI Master writes
 * immediately followed by a SPI Master read.
 ******************************************************************************/
err_t spi_master_write_then_read(spi_master_t *obj,
                                 uint8_t *write_data_buffer,
                                 size_t length_write_data,
                                 uint8_t *read_data_buffer,
                                 size_t length_read_data)
{
  sl_status_t status;
  size_t tx_len = length_write_data + length_read_data;
  uint8_t tx_buffer[tx_len];
  uint8_t rx_buffer[tx_len];

  if (_acquire(obj, false) != ACQUIRE_SUCCESS) {
    return SPI_MASTER_ERROR;
  }

  if ((last_spi_speed_used != obj->config.speed)
      || (last_spi_mode_used != obj->config.mode)) {
    // Update the config
    spi_master_set_configuration(obj);
  }

  for (size_t i = 0; i < length_write_data; i++) {
    tx_buffer[i] = write_data_buffer[i];
  }
  // The data transmit buffer will be populated with user-defined data
  for (size_t i = length_write_data; i < tx_len; i++) {
    tx_buffer[i] = _owner->config.default_write_data;
  }

  status = sl_si91x_ssi_transfer_data(ssi_driver_handle,
                                      tx_buffer,
                                      rx_buffer,
                                      tx_len);
  if (status != SL_STATUS_OK) {
    return SPI_MASTER_ERROR;
  }

  for (size_t i = 0; i < length_read_data; i++) {
    read_data_buffer[i] = rx_buffer[i + length_write_data];
  }

  return SPI_MASTER_SUCCESS;
}

void spi_master_close(spi_master_t *obj)
{
  obj->handle = NULL;
  _owner = NULL;
}

static err_t _acquire(spi_master_t *obj, bool obj_open_state)
{
  if ((obj_open_state == true) && (_owner == obj)) {
    return ACQUIRE_FAIL;
  }

  if (_owner != obj) {
    _owner = obj;
  }

  return ACQUIRE_SUCCESS;
}

static void spi_master_configure_gpio_pin(digital_out_t *out, pin_name_t name)
{
  out->pin.base = hal_gpio_port_index(name);
  out->pin.mask = hal_gpio_pin_index(name);
}

static err_t spi_master_set_configuration(spi_master_t *obj)
{
  sl_ssi_control_config_t ssi_config = {
    .bit_width = SSI_MASTER_BIT_WIDTH,
    .device_mode = (uint32_t) SL_SSI_MASTER_ACTIVE,
    .baud_rate = obj->config.speed,
    .receive_sample_delay = SSI_MASTER_RECEIVE_SAMPLE_DELAY,
  };

  if (obj->config.mode == SPI_MASTER_MODE_0) {
    ssi_config.clock_mode = SL_SSI_PERIPHERAL_CPOL0_CPHA0;
  } else if (obj->config.mode == SPI_MASTER_MODE_1) {
    ssi_config.clock_mode = SL_SSI_PERIPHERAL_CPOL0_CPHA1;
  } else if (obj->config.mode == SPI_MASTER_MODE_2) {
    ssi_config.clock_mode = SL_SSI_PERIPHERAL_CPOL1_CPHA0;
  } else if (obj->config.mode == SPI_MASTER_MODE_3) {
    ssi_config.clock_mode = SL_SSI_PERIPHERAL_CPOL1_CPHA1;
  } else {
    return SPI_MASTER_ERROR;
  }

  last_spi_speed_used = obj->config.speed;
  last_spi_mode_used = obj->config.mode;

  // Overwrite gspi default
  ssi_configuration = ssi_config;

  /**
   * Configuration of all other parameters that are required by GSPI
   * control_configuration structure is from sl_si91x_ssi_init.h file.
   * The user can modify this structure with the configuration of
   * his choice by filling this structure.
   */
  if (sl_si91x_ssi_set_configuration(ssi_driver_handle,
                                     &ssi_config,
                                     SSI_SLAVE_0) != SL_STATUS_OK) {
    return SPI_MASTER_ERROR;
  }

  return SPI_MASTER_SUCCESS;
}

// Function to configure clock on powerup
static void default_clock_configuration(void)
{
  // Core Clock runs at 180MHz SOC PLL Clock
  sl_si91x_clock_manager_m4_set_core_clk(M4_SOCPLLCLK, SOC_PLL_CLK);

  // All peripherals' source to be set to Interface PLL Clock
  // and it runs at 180MHz
  sl_si91x_clock_manager_set_pll_freq(INFT_PLL,
                                      INTF_PLL_CLK,
                                      PLL_REF_CLK_VAL_XTAL);

  // Configure QSPI clock as input source
  ROMAPI_M4SS_CLK_API->clk_qspi_clk_config(M4CLK,
                                           QSPI_INTFPLLCLK,
                                           QSPI_SWALLO_ENABLE,
                                           QSPI_ODD_DIV_ENABLE,
                                           QSPI_DIVISION_FACTOR);
}

/*******************************************************************************
 * Callback event function
 * It is responsible for the event which are triggered by GSPI interface
 * It updates the respective member of the structure as the event is triggered.
 ******************************************************************************/
static void callback_event(uint32_t event)
{
  switch (event) {
    case SSI_EVENT_TRANSFER_COMPLETE:
      break;
    case SSI_EVENT_DATA_LOST:
      break;
    case SSI_EVENT_MODE_FAULT:
      break;
  }
}

// ------------------------------------------------------------------------- END
