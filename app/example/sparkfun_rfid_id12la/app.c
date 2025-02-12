/***************************************************************************//**
 * @file app.c
 * @brief Top level application functions
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

#include "app_assert.h"
#include "sparkfun_rfid_id12la.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

static id12la_tag_list_t id12la_all_tag_data;
static uint8_t count_tag = 0;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t ret;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  ret = sparkfun_id12la_init(app_i2c_instance);

  if (ret == SL_STATUS_NOT_AVAILABLE) {
    app_printf(
      "Device is not present on the I2C bus or I2C address has been changed before\n");
    ret = sparkfun_id12la_scan_address();
    app_assert_status(ret);
    app_printf("I2C address is: 0x%02X\n", sparkfun_id12la_get_i2c_address());
  } else if (ret != SL_STATUS_OK) {
    app_printf("Failed to initialize RFID\r\n");
    return;
  }

  app_printf("RFID inits successfully, ready scans some tags\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (sparkfun_id12la_get_all_tag(&id12la_all_tag_data,
                                  &count_tag) != SL_STATUS_OK) {
    app_printf("error while scanning tags, check connection!!!\n");
  }

  if (count_tag > 0) {
    app_printf("count tag: %d\n", count_tag);

    for (uint8_t i = 0; i < count_tag; i++) {
      if (id12la_all_tag_data.id12la_data[i].checksum_valid == true) {
        app_printf(
          "ID (last byte is checksum): 0x%02X 0x%02X 0x%02X 0x%02X \
                   0x%02X 0x%02X\n",
          id12la_all_tag_data.id12la_data[i].id_tag[0], \
          id12la_all_tag_data.id12la_data[i].id_tag[1], \
          id12la_all_tag_data.id12la_data[i].id_tag[2], \
          id12la_all_tag_data.id12la_data[i].id_tag[3], \
          id12la_all_tag_data.id12la_data[i].id_tag[4], \
          id12la_all_tag_data.id12la_data[i].id_tag[5]);
        app_printf("Scan time: %ld\n\n",
                   id12la_all_tag_data.id12la_data[i].time);
      } else {
        app_printf("Tag %d : checksum error, please scan the tag again\n", i);
      }
    }
  }
}
