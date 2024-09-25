/***************************************************************************//**
 * @file mikroe_eink_display.h
 * @brief SCL Mikroe eINK
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
#ifndef MIKROE_EINK_DISPLAY_H_
#define MIKROE_EINK_DISPLAY_H_

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#include "sl_status.h"
#include "mikroe_eink_display_config.h"

#if (CONFIG_EINK_DISPLAY_RESOLUTION == EINK_DISPLAY_1_54_INCH)
#include "eink154inch.h"

#define eink_display_cfg_setup(cfg) \
  eink154inch_cfg_setup(cfg)
#define eink_display_init(ctx, cfg) \
  eink154inch_init(ctx, cfg)
#define eink_display_send_command(ctx, command) \
  eink154inch_send_cmd(ctx, command)
#define eink_display_send_data(ctx, data) \
  eink154inch_send_data(ctx, data)
#define eink_display_reset(ctx) \
  eink154inch_reset(ctx)
#define eink_display_sleep_mode(ctx) \
  eink154inch_sleep_mode(ctx)
#define eink_display_set_lut(ctx, lut, n_bytes) \
  eink154inch_set_lut(ctx, lut, n_bytes)
#define eink_display_start_config(ctx) \
  eink154inch_start_config(ctx)
#define eink_display_set_mem_pointer(ctx, x, y) \
  eink154inch_set_mem_pointer(ctx, x, y)
#define eink_display_set_mem_area(ctx, xy) \
  eink154inch_set_mem_area(ctx, xy)
#define eink_display_update_display(ctx) \
  eink154inch_update_display(ctx)
#define eink_display_fill_screen(ctx, color) \
  eink154inch_fill_screen(ctx, color)
#define eink_display_display_image(ctx, image_buffer) \
  eink154inch_image(ctx, image_buffer)

typedef eink154inch_t eink_display_t;
typedef eink154inch_cfg_t eink_display_cfg_t;
typedef eink154inch_xy_t eink_display_xy_t;
#elif (CONFIG_EINK_DISPLAY_RESOLUTION == EINK_DISPLAY_2_13_INCH)
#include "eink213inch.h"

#define eink_display_cfg_setup(cfg) \
  eink213inch_cfg_setup(cfg)
#define eink_display_init(ctx, cfg) \
  eink213inch_init(ctx, cfg)
#define eink_display_send_command(ctx, command) \
  eink213inch_send_cmd(ctx, command)
#define eink_display_send_data(ctx, data) \
  eink213inch_send_data(ctx, data)
#define eink_display_reset(ctx) \
  eink213inch_reset(ctx)
#define eink_display_sleep_mode(ctx) \
  eink213inch_sleep_mode(ctx)
#define eink_display_set_lut(ctx, lut, n_bytes) \
  eink213inch_set_lut(ctx, lut, n_bytes)
#define eink_display_start_config(ctx) \
  eink213inch_start_config(ctx)
#define eink_display_set_mem_pointer(ctx, x, y) \
  eink213inch_set_mem_pointer(ctx, x, y)
#define eink_display_set_mem_area(ctx, xy) \
  eink213inch_set_mem_area(ctx, xy)
#define eink_display_update_display(ctx) \
  eink213inch_update_display(ctx)
#define eink_display_fill_screen(ctx, color) \
  eink213inch_fill_screen(ctx, color)
#define eink_display_display_image(ctx, image_buffer) \
  eink213inch_display_image(ctx, image_buffer)

typedef eink213inch_t eink_display_t;
typedef eink213inch_cfg_t eink_display_cfg_t;
typedef eink213inch_xy_t eink_display_xy_t;
#elif (CONFIG_EINK_DISPLAY_RESOLUTION == EINK_DISPLAY_2_90_INCH)
#include "eink290inch.h"

#define eink_display_cfg_setup(cfg) \
  eink290inch_cfg_setup(cfg)
#define eink_display_init(ctx, cfg) \
  eink290inch_init(ctx, cfg)
#define eink_display_send_command(ctx, command) \
  eink290inch_send_cmd(ctx, command)
#define eink_display_send_data(ctx, data) \
  eink290inch_send_data(ctx, data)
#define eink_display_reset(ctx) \
  eink290inch_reset(ctx)
#define eink_display_sleep_mode(ctx) \
  eink290inch_sleep_mode(ctx)
#define eink_display_set_lut(ctx, lut, n_bytes) \
  eink290inch_set_lut(ctx, lut, n_bytes)
#define eink_display_start_config(ctx) \
  eink290inch_start_config(ctx)
#define eink_display_set_mem_pointer(ctx, x, y) \
  eink290inch_set_mem_pointer(ctx, x, y)
#define eink_display_set_mem_area(ctx, xy) \
  eink290inch_set_mem_area(ctx, xy)
#define eink_display_update_display(ctx) \
  eink290inch_update_display(ctx)
#define eink_display_fill_screen(ctx, color) \
  eink290inch_fill_screen(ctx, color)
#define eink_display_display_image(ctx, image_buffer) \
  eink290inch_display_image(ctx, image_buffer)

typedef eink290inch_t eink_display_t;
typedef eink290inch_cfg_t eink_display_cfg_t;
typedef eink290inch_xy_t eink_display_xy_t;
#endif

// -----------------------------------------------------------------------------
//                               Macros
// -----------------------------------------------------------------------------

#define EINK_DISPLAY_OK                                                0
#define EINK_DISPLAY_ERROR                                             -1

/* Screen color */
#define EINK_DISPLAY_SCREEN_COLOR_WHITE                                0xFF
#define EINK_DISPLAY_SCREEN_COLOR_BLACK                                0x00

/* Display Command */
#define EINK_DISPLAY_CMD_DRIVER_OUTPUT_CONTROL                         0x01
#define EINK_DISPLAY_CMD_BOOSTER_SOFT_START_CONTROL                    0x0C
#define EINK_DISPLAY_CMD_GATE_SCAN_START_POSITION                      0x0F
#define EINK_DISPLAY_CMD_DEEP_SLEEP_MODE                               0x10
#define EINK_DISPLAY_CMD_DATA_ENTRY_MODE_SETTING                       0x11
#define EINK_DISPLAY_CMD_SW_RESET                                      0x12
#define EINK_DISPLAY_CMD_TEMPERATURE_SENSOR_CONTROL                    0x1A
#define EINK_DISPLAY_CMD_MASTER_ACTIVATION                             0x20
#define EINK_DISPLAY_CMD_DISPLAY_UPDATE_CONTROL_1                      0x21
#define EINK_DISPLAY_CMD_DISPLAY_UPDATE_CONTROL_2                      0x22
#define EINK_DISPLAY_CMD_WRITE_RAM                                     0x24
#define EINK_DISPLAY_CMD_WRITE_VCOM_REGISTER                           0x2C
#define EINK_DISPLAY_CMD_WRITE_LUT_REGISTER                            0x32
#define EINK_DISPLAY_CMD_SET_DUMMY_LINE_PERIOD                         0x3A
#define EINK_DISPLAY_CMD_SET_GATE_TIME                                 0x3B
#define EINK_DISPLAY_CMD_BORDER_WAVEFORM_CONTROL                       0x3C
#define EINK_DISPLAY_CMD_SET_RAM_X_ADDRESS_START_END_POSITION          0x44
#define EINK_DISPLAY_CMD_SET_RAM_Y_ADDRESS_START_END_POSITION          0x45
#define EINK_DISPLAY_CMD_SET_RAM_X_ADDRESS_COUNTER                     0x4E
#define EINK_DISPLAY_CMD_SET_RAM_Y_ADDRESS_COUNTER                     0x4F
#define EINK_DISPLAY_CMD_TERMINATE_FRAME_READ_WRITE                    0xFF

// -----------------------------------------------------------------------------
//                       Public Functions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_init(mikroe_spi_handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    Generic transfer function.
 *
 * @param[in] wr_buf
 *    Write data buffer
 * @param[in] wr_len
 *    Number of byte in write data buffer
 * @param[in] rd_buf
 *    Read data buffer
 * @param[in] rd_len
 *    Number of byte in read data buffer
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_generic_transfer(uint8_t *wr_buf,
                                                 uint16_t wr_len,
                                                 uint8_t *rd_buf,
                                                 uint16_t rd_len);

/***************************************************************************//**
 * @brief
 *    Sending command function.
 *
 * @param[in] command
 *    The command to be sent
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_send_command(uint8_t command);

/***************************************************************************//**
 * @brief
 *    Sending data function.
 *
 * @param[in] data
 *    The data to be sent
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_send_data(uint8_t data);

/***************************************************************************//**
 * @brief
 *    Reset chip function.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_reset(void);

/***************************************************************************//**
 * @brief
 *    Sleep mode function.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_sleep_mode(void);

/***************************************************************************//**
 * @brief
 *    Configuration display function.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_start_config(void);

/***************************************************************************//**
 * @brief
 *    Set LUT table function.
 *
 * @param[in] lut
 *    Lut table
 * @param[in] n_bytes
 *    Number of bytes in Lut table
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_set_lut(const uint8_t *lut,
                                        uint8_t n_bytes);

/***************************************************************************//**
 * @brief
 *    Setting pointers in memory.
 *
 * @param[in] x
 *    x position
 * @param[in] y
 *    y position
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_set_memory_pointer(uint8_t x, uint8_t y);

/***************************************************************************//**
 * @brief
 *    Setting area in memory.
 *
 * @param[in] xy
 *    Struct object
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_set_memory_area(eink_display_xy_t *xy);

/***************************************************************************//**
 * @brief
 *    Update display function.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_update_display();

/***************************************************************************//**
 * @brief
 *    Function that fills the screen.
 *
 * @param[in] lut
 *    Lut table
 * @param[in] color
 *    The color to which the screen will be colored.
 *    Options :
 *      EINK_DISPLAY_SCREEN_COLOR_WHITE
 *      EINK_DISPLAY_SCREEN_COLOR_BLACK
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_fill_screen(uint8_t color);

/***************************************************************************//**
 * @brief
 *    Displays image function.
 *
 * @param[in] image_buffer
 *    Buffer containing the image.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_display_image(const uint8_t *image_buffer);

/***************************************************************************//**
 * @brief
 *    Displays image function for ESL.
 *
 * @param[in] image_index
 *    Buffer containing the image index.
 *
 * @return
 *    SL_STATUS_OK if Succeed .
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_eink_display_image_non_blocking(const uint8_t image_index);

#ifdef __cplusplus
}
#endif

#endif // MIKROE_EINK_DISPLAY_H_
