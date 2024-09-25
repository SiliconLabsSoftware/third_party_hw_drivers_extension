/***************************************************************************//**
 * @file mipi_dbi_spi_4wire.h
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
#ifndef MIPI_DBI_H_
#define MIPI_DBI_H_

// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

struct mipi_dbi_api;
struct mipi_dbi_config;
struct mipi_dbi_data;

struct mipi_dbi_device {
  const struct mipi_dbi_api *api;
  const struct mipi_dbi_config *config;
  struct mipi_dbi_data *data;
};

struct mipi_dbi_display_buffer_descriptor {
  /** Data buffer size in bytes */
  uint32_t buf_size;

  /** Data buffer row width in pixels */
  uint16_t width;

  /** Data buffer column height in pixels */
  uint16_t height;

  /** Number of pixels between consecutive rows in the data buffer */
  uint16_t pitch;
};

/**
 * @brief Display pixel formats
 *
 * Display pixel format enumeration.
 *
 * In case a pixel format consists out of multiple bytes the byte order is
 * big endian.
 */
enum mipi_dbi_display_pixel_format {
  PIXEL_FORMAT_RGB_888, /**< 24-bit RGB */
  PIXEL_FORMAT_MONO01, /**< Monochrome (0=Black 1=White) */
  PIXEL_FORMAT_MONO10, /**< Monochrome (1=Black 0=White) */
  PIXEL_FORMAT_ARGB_8888, /**< 32-bit ARGB */
  PIXEL_FORMAT_RGB_565, /**< 16-bit RGB */
  PIXEL_FORMAT_BGR_565, /**< 16-bit BGR */
};

typedef void (*mipi_dbi_transfer_complete_callback_t)(void);

struct mipi_dbi_api {
  sl_status_t (*command_read)(const struct mipi_dbi_device *device,
                              uint8_t *cmds, size_t num_cmds, uint8_t *response,
                              size_t len);
  sl_status_t (*command_write)(const struct mipi_dbi_device *device,
                               uint8_t cmd,
                               const uint8_t *data, size_t len);
  sl_status_t (*write_display)(
    const struct mipi_dbi_device *device,
    const uint8_t *framebuf,
    struct mipi_dbi_display_buffer_descriptor *desc,
    enum mipi_dbi_display_pixel_format pixfmt,
    mipi_dbi_transfer_complete_callback_t callback);
  sl_status_t (*reset)(const struct mipi_dbi_device *device,
                       uint32_t delay);
};

sl_status_t mipi_dbi_device_init(struct mipi_dbi_device *device,
                                 const struct mipi_dbi_config *config);

#ifdef __cplusplus
}
#endif

#endif /* MIPI_DBI_H_ */
