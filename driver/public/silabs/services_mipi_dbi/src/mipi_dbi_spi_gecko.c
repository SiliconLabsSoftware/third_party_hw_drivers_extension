/***************************************************************************//**
 * @file mipi_dbi_spi_gecko.c
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
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "sl_component_catalog.h"
#include "mipi_dbi.h"
#include "mipi_dbi_spi.h"

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------
const struct adafruit_st7789_spi_gecko_config *spi_config = NULL;
static volatile mipi_dbi_transfer_complete_callback_t transfer_complete_callback
  = NULL;

static sl_status_t command_read(const struct mipi_dbi_device *device,
                                uint8_t *cmds, size_t num_cmds,
                                uint8_t *response, size_t len);
static sl_status_t command_write(const struct mipi_dbi_device *device,
                                 uint8_t cmd,
                                 const uint8_t *data, size_t len);
static sl_status_t write_display(
  const struct mipi_dbi_device *device,
  const uint8_t *framebuf,
  struct mipi_dbi_display_buffer_descriptor *desc,
  enum mipi_dbi_display_pixel_format pixfmt,
  mipi_dbi_transfer_complete_callback_t callback);
static sl_status_t reset(const struct mipi_dbi_device *device,
                         uint32_t delay);

static const struct mipi_dbi_api mipi_dbi_api = {
  .command_read = command_read,
  .command_write = command_write,
  .write_display = write_display,
  .reset = reset
};
static struct mipi_dbi_spi_gecko_data mipi_dbi_spi_gecko_data;

static void spi_select(const struct mipi_dbi_device *device)
{
  struct mipi_dbi_spi_usart_config *config =
    (struct mipi_dbi_spi_usart_config *)device->config;

  if (config->cs_control == spidrvCsControlAuto) {
    GPIO_PinOutClear(config->cs.port, config->cs.pin);
  }
}

static void spi_deselect(const struct mipi_dbi_device *device)
{
  struct mipi_dbi_spi_usart_config *config =
    (struct mipi_dbi_spi_usart_config *)device->config;
  if (config->cs_control == spidrvCsControlAuto) {
    GPIO_PinOutSet(config->cs.port, config->cs.pin);
  }
}

static void set_dc_mode(const struct mipi_dbi_device *device,
                        bool mode)
{
  struct mipi_dbi_spi_usart_config *config =
    (struct mipi_dbi_spi_usart_config *)device->config;
  if (mode) {
    GPIO_PinOutSet(config->dc.port, config->dc.pin);
  } else {
    GPIO_PinOutClear(config->dc.port, config->dc.pin);
  }
}

static sl_status_t spi_write_b(const struct mipi_dbi_device *device,
                               const uint8_t *buf,
                               int count)
{
  struct mipi_dbi_spi_usart_config *config =
    (struct mipi_dbi_spi_usart_config *)device->config;
  spi_select(device);
  while (count--) {
    USART_SpiTransfer(config->usart, *buf++);
  }
  spi_deselect(device);
  return SL_STATUS_OK;
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

static sl_status_t command_write(const struct mipi_dbi_device *device,
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
  (void) pixfmt;
  (void) callback;

  set_dc_mode(device, true);
  return spi_write_b(device, framebuf, desc->buf_size);
}

sl_status_t mipi_dbi_device_init(struct mipi_dbi_device *device,
                                 const struct mipi_dbi_config *config)
{
  struct mipi_dbi_spi_usart_config *device_config =
    (struct mipi_dbi_spi_usart_config *)config;
  USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;

  usartInit.msbf = true;
  usartInit.clockMode = usartClockMode0;
  usartInit.master = true;
  usartInit.baudrate = device_config->bitrate;
  usartInit.clockMode = device_config->clock_mode;
  if (device_config->cs_control == spidrvCsControlAuto) {
    usartInit.autoCsEnable = true;
  }

#if defined(_CMU_HFPERCLKEN0_MASK)
  CMU_ClockEnable(cmuClock_HFPER, true);
#endif
  CMU_ClockEnable(cmuClock_GPIO, true);
#if ADAFRUIT_HXD8357D_PERIPHERAL_NO == 0
  CMU_ClockEnable(cmuClock_USART0, true);
#elif ADAFRUIT_HXD8357D_PERIPHERAL_NO == 1
  CMU_ClockEnable(cmuClock_USART1, true);
#elif ADAFRUIT_HXD8357D_PERIPHERAL_NO == 2
  CMU_ClockEnable(cmuClock_USART2, true);
#endif

  usartInit.databits = usartDatabits8;
  USART_InitSync(device_config->usart, &usartInit);

#if defined(USART_ROUTEPEN_TXPEN)
  ADAFRUIT_HXD8357D_PERIPHERAL->ROUTELOC0 =
    (ADAFRUIT_HXD8357D_PERIPHERAL->ROUTELOC0
     & ~(_USART_ROUTELOC0_TXLOC_MASK
         | _USART_ROUTELOC0_RXLOC_MASK
         | _USART_ROUTELOC0_CLKLOC_MASK))
    | (ADAFRUIT_HXD8357D_TX_LOC  << _USART_ROUTELOC0_TXLOC_SHIFT)
    | (ADAFRUIT_HXD8357D_RX_LOC  << _USART_ROUTELOC0_RXLOC_SHIFT)
    | (ADAFRUIT_HXD8357D_CLK_LOC << _USART_ROUTELOC0_CLKLOC_SHIFT);

  ADAFRUIT_HXD8357D_PERIPHERAL->ROUTEPEN = USART_ROUTEPEN_TXPEN
                                           | USART_ROUTEPEN_RXPEN
                                           | USART_ROUTEPEN_CLKPEN
                                           | USART_ROUTEPEN_CSPEN;
#else
  GPIO->USARTROUTE[device_config->peripheral_no].ROUTEEN =
    GPIO_USART_ROUTEEN_TXPEN
    | GPIO_USART_ROUTEEN_RXPEN
    | GPIO_USART_ROUTEEN_CLKPEN;
  GPIO->USARTROUTE[device_config->peripheral_no].TXROUTE =
    ((uint32_t)device_config->tx.port << _GPIO_USART_TXROUTE_PORT_SHIFT)
    | ((uint32_t)device_config->tx.pin << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[device_config->peripheral_no].RXROUTE =
    ((uint32_t)device_config->rx.port << _GPIO_USART_RXROUTE_PORT_SHIFT)
    | ((uint32_t)device_config->rx.pin << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[device_config->peripheral_no].CLKROUTE =
    ((uint32_t)device_config->clk.port << _GPIO_USART_CLKROUTE_PORT_SHIFT)
    | ((uint32_t)device_config->clk.pin << _GPIO_USART_CLKROUTE_PIN_SHIFT);
#endif

  GPIO_PinModeSet(device_config->clk.port,
                  device_config->clk.pin,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(device_config->tx.port,
                  device_config->tx.pin,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(device_config->rx.port,
                  device_config->rx.pin,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(device_config->cs.port,
                  device_config->cs.pin,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(device_config->dc.port,
                  device_config->dc.pin,
                  gpioModePushPull,
                  0);

  device->api = &mipi_dbi_api;
  device->config = config;
  device->data = (struct mipi_dbi_data *)&mipi_dbi_spi_gecko_data;
  return SL_STATUS_OK;
}
