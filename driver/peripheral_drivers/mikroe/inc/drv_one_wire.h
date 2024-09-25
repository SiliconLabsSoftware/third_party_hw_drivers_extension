/***************************************************************************//**
 * @file drv_one_wire.h
 * @brief mikroSDK 2.0 Click Peripheral Drivers - One Wire
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

#ifndef _DRV_ONE_WIRE_H_
#define _DRV_ONE_WIRE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "drv_name.h"

#define ONE_WIRE_CMD_ROM_READ           (0x33)
#define ONE_WIRE_CMD_ROM_SKIP           (0xCC)
#define ONE_WIRE_CMD_ROM_MATCH          (0x55)
#define ONE_WIRE_CMD_ROM_SEARCH         (0xF0)
#define ONE_WIRE_CMD_ROM_READ_LEGACY    (0x0F)

/**
 * @brief One Wire Driver return values.
 */
typedef enum {
  ONE_WIRE_SUCCESS = 0,   /*!< Success. */
  ONE_WIRE_ERROR = (-1)   /*!< Error. */
} one_wire_error_t;

/**
 * @brief Structure for storing One Wire device address.
 * @details Unique One Wire address used for device addressing.
 */
typedef struct {
  uint8_t address[8];   /*!< One Wire address buffer. */
} one_wire_rom_address_t;

/**
 * @brief One Wire Driver initialization configuration structure.
 *
 * @details
 * - data_pin value - to be configured as desired.
 * - state value - NOTE must not be altered (it is supposed to be set automatically in Low Level Layer).
 */
typedef struct {
  pin_name_t data_pin;   /*!< One Wire pin - user is to configure it as desired. */
  bool state;   /*!< State of a pin. NOTE must not be altered. */
} one_wire_t;

err_t one_wire_open(one_wire_t *obj);
void one_wire_configure_default(one_wire_t *obj);
err_t one_wire_reset(one_wire_t *obj);
err_t one_wire_read_rom(one_wire_t *obj,
                        one_wire_rom_address_t *device_rom_address);
err_t one_wire_skip_rom(one_wire_t *obj);
err_t one_wire_match_rom(one_wire_t *obj,
                         one_wire_rom_address_t *device_rom_address);
err_t one_wire_search_first_device(one_wire_t *obj,
                                   one_wire_rom_address_t *one_wire_device_list);
err_t one_wire_search_next_device(one_wire_t *obj,
                                  one_wire_rom_address_t *one_wire_device_list);
err_t one_wire_write_byte(one_wire_t *obj,
                          uint8_t *write_data_buffer,
                          size_t write_data_length);
err_t one_wire_read_byte(one_wire_t *obj,
                         uint8_t *read_data_buffer,
                         size_t read_data_length);

#ifdef __cplusplus
}
#endif

#endif // _DRV_ONE_WIRE_H_
// ------------------------------------------------------------------------- END
