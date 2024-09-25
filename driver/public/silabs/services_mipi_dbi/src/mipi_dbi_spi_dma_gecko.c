/***************************************************************************//**
 * @file mipi_dbi_spi_dma_gecko.c
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
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "sl_component_catalog.h"
#include "spidrv.h"
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
static void dma_transfer_complete_callback(struct SPIDRV_HandleData *handle,
                                           Ecode_t transferStatus,
                                           int itemsTransferred);

static const struct mipi_dbi_api mipi_dbi_api = {
  .command_read = command_read,
  .command_write = command_write,
  .write_display = write_display,
  .reset = reset
};
static struct mipi_dbi_spi_gecko_data mipi_dbi_spi_gecko_data;

SPIDRV_HandleData_t spidrv_handle_data;
SPIDRV_Handle_t spidrv_handle = &spidrv_handle_data;

static void spi_select(SPIDRV_Handle_t handle)
{
  if (handle->initData.csControl == spidrvCsControlApplication) {
    GPIO_PinOutClear(handle->portCs, handle->pinCs);
  }
}

static void spi_deselect(SPIDRV_Handle_t handle)
{
  if (handle->initData.csControl == spidrvCsControlApplication) {
    GPIO_PinOutSet(handle->portCs, handle->pinCs);
  }
}

static void set_dc_mode(const struct mipi_dbi_device *device,
                        bool mode)
{
  struct mipi_dbi_spidrv_config *config =
    (struct mipi_dbi_spidrv_config *)device->config;
  if (mode) {
    GPIO_PinOutSet(config->dc.port, config->dc.pin);
  } else {
    GPIO_PinOutClear(config->dc.port, config->dc.pin);
  }
}

static sl_status_t spi_write_b(SPIDRV_Handle_t handle,
                               const void *buf,
                               int count)
{
  spi_select(handle);
  Ecode_t retVal = SPIDRV_MTransmitB(handle,
                                     buf,
                                     count);
  spi_deselect(handle);
  return ECODE_OK == retVal ? SL_STATUS_OK : SL_STATUS_IO;
}

static sl_status_t spi_write(
  SPIDRV_Handle_t handle,
  const void *buf,
  int count,
  mipi_dbi_transfer_complete_callback_t callback)
{
  transfer_complete_callback = callback;
  spi_select(handle);
  Ecode_t retVal = SPIDRV_MTransmit(handle,
                                    (uint8_t *)buf,
                                    count,
                                    dma_transfer_complete_callback);
  return ECODE_OK == retVal ? SL_STATUS_OK : SL_STATUS_IO;
}

static sl_status_t spi_read_b(
  SPIDRV_Handle_t handle,
  const void *buf,
  int count)
{
  spi_select(handle);
  Ecode_t retVal = SPIDRV_SReceiveB(handle,
                                    (uint8_t *)buf,
                                    count,
                                    1000);
  spi_deselect(handle);
  return ECODE_OK == retVal ? SL_STATUS_OK : SL_STATUS_IO;
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
  sl_status_t status;

  if (num_cmds > 0) {
    set_dc_mode(device, false);
    status = spi_write_b(spidrv_handle, cmds, num_cmds);
    if (SL_STATUS_OK != status) {
      return status;
    }
  }

  if (len > 0) {
    set_dc_mode(device, true);
    status = spi_read_b(spidrv_handle, response, len);
    if (SL_STATUS_OK != status) {
      return status;
    }
  }
  return SL_STATUS_OK;
}

static sl_status_t command_write(const struct mipi_dbi_device *device,
                                 uint8_t cmd,
                                 const uint8_t *data, size_t len)
{
  sl_status_t status;

  set_dc_mode(device, false);
  status = spi_write_b(spidrv_handle, &cmd, 1);
  if (SL_STATUS_OK != status) {
    return status;
  }

  if (len) {
    set_dc_mode(device, true);
    status = spi_write_b(spidrv_handle, data, len);
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
    status = spi_write(spidrv_handle, framebuf, desc->buf_size, callback);
  } else {
    status = spi_write_b(spidrv_handle, framebuf, desc->buf_size);
  }
  return status;
}

static void dma_transfer_complete_callback(struct SPIDRV_HandleData *handle,
                                           Ecode_t transferStatus,
                                           int itemsTransferred)
{
  (void) handle;
  (void) itemsTransferred;

  spi_deselect(handle);
  if (transferStatus == ECODE_OK) {
    if (transfer_complete_callback) {
      transfer_complete_callback();
    }
  }
}

sl_status_t mipi_dbi_device_init(struct mipi_dbi_device *device,
                                 const struct mipi_dbi_config *config)
{
  struct mipi_dbi_spidrv_config *device_config =
    (struct mipi_dbi_spidrv_config *)config;

  SPIDRV_Init(spidrv_handle, device_config->spidrv_init);
  if (device_config->spidrv_init->csControl == spidrvCsControlApplication) {
    GPIO_PinModeSet(device_config->spidrv_init->portCs,
                    device_config->spidrv_init->pinCs,
                    gpioModePushPull,
                    0);
  }
  GPIO_PinModeSet(device_config->dc.port,
                  device_config->dc.pin,
                  gpioModePushPull,
                  0);

  device->api = &mipi_dbi_api;
  device->config = config;
  device->data = (struct mipi_dbi_data *)&mipi_dbi_spi_gecko_data;
  return SL_STATUS_OK;
}
