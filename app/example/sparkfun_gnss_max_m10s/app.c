/***************************************************************************//**
 * @file app.c
 * @brief Example application
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
#include "gnss_max_m10s_driver.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)                 DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED               SL_I2C2

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...)                 app_log(__VA_ARGS__)
#endif

///< Divisor value for latitude and longitude using UBX protocol
#define LAT_LONG_DIVISOR_UBX            10000000
///< Divisor value for latitude and longitude using NMEA protocol
#define LAT_LONG_DIVISOR_NMEA           1000000

///< Retry count for GNSS receiver
#define GNSS_RETRY_COUNT                10
///< Retry count to receive data from
#define GNSS_DATA_RETRY_COUNT           5
#define GNSS_DATA_TIMEOUT_RETRY_DELAY   200       ///< In ms
#define GNSS_PER_RETRY_DELAY            500       ///< In ms
#define GNSS_TASK_DELAY                 1000      ///< In ms

/// @brief Structure for MAX-M10s GNSS receiver data
typedef struct {
  int32_t   no_of_satellites; ///< no of satellites
  double    latitude; ///< latitude value
  double    longitude; ///< longitude value
  double    altitude; ///< altitude value
} sl_gnss_data_t;

sl_max_m10s_cfg_data_t gnss_cfg_data; ///< To store GNSS receiver configuration.
static mikroe_i2c_handle_t app_i2c_instance = NULL;

/**************************************************************************//**
 * Initialize application.
 *****************************************************************************/
void app_init(void)
{
  sl_status_t status;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  gnss_cfg_data.i2c_instance = app_i2c_instance;
  gnss_cfg_data.device_address = GNSS_ADDRESS;
  gnss_cfg_data.protocol_type = SL_MAX_M10S_PROTOCOL_UBX;

  status = gnss_max_m10s_begin(GNSS_POLL_MAX_TIMEOUT, &gnss_cfg_data);
  if (status != SL_STATUS_OK) {
    app_printf("\r\nmax m10s init failed, reason: 0x%lx\r\n", status);
    return;
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t status;
  sl_gnss_data_t gnss_data; ///< MAX-M10s GNSS receiver data
  uint8_t fix_type = 0;
  bool is_sensor_data_available;
  uint8_t retry_count = 0, data_retry_count = 0;

  is_sensor_data_available = false;

  while (GNSS_RETRY_COUNT > retry_count) {
    status = gnss_max_m10s_get_fix_type(&gnss_cfg_data,
                                        GNSS_POLL_MAX_TIMEOUT,
                                        &fix_type);
    if (status != SL_STATUS_OK) {
      fix_type = 0;
    }

    app_printf("\r\ngnss_receiver_task : fix type is: %d\r\n", fix_type);
    if (((SL_MAX_M10S_PROTOCOL_UBX == gnss_cfg_data.protocol_type)
         && ((3 == fix_type) || (2 == fix_type)))
        || ((SL_MAX_M10S_PROTOCOL_NMEA == gnss_cfg_data.protocol_type)
            && (1 == fix_type))) {
      for (data_retry_count = 0; data_retry_count < GNSS_DATA_RETRY_COUNT;
           ++data_retry_count) {
        status =
          gnss_max_m10s_get_nav_pvt(&gnss_cfg_data, GNSS_POLL_MAX_TIMEOUT);

        if (status == SL_STATUS_OK) {
          gnss_data.latitude = (double)gnss_cfg_data.packetUBXNAVPVT->data.lat;

          if (SL_MAX_M10S_PROTOCOL_UBX == gnss_cfg_data.protocol_type) {
            gnss_data.latitude /= LAT_LONG_DIVISOR_UBX;
          } else if (SL_MAX_M10S_PROTOCOL_NMEA == gnss_cfg_data.protocol_type) {
            gnss_data.latitude /= LAT_LONG_DIVISOR_NMEA;
          }
          app_printf("\r\ngnss_receiver_task : latitude is : %.7lf\r\n",
                     gnss_data.latitude);
          gnss_data.longitude = (double)gnss_cfg_data.packetUBXNAVPVT->data.lon;

          if (SL_MAX_M10S_PROTOCOL_UBX == gnss_cfg_data.protocol_type) {
            gnss_data.longitude /= LAT_LONG_DIVISOR_UBX;
          } else if (SL_MAX_M10S_PROTOCOL_NMEA == gnss_cfg_data.protocol_type) {
            gnss_data.longitude /= LAT_LONG_DIVISOR_NMEA;
          }
          app_printf("\r\ngnss_receiver_task : longitude is : %.7lf\r\n",
                     gnss_data.longitude);
          gnss_data.altitude = (double)gnss_cfg_data.packetUBXNAVPVT->data.hMSL;
          app_printf("\r\ngnss_receiver_task : altitude is : %lf\r\n",
                     gnss_data.altitude);
          gnss_data.no_of_satellites =
            (int32_t)gnss_cfg_data.packetUBXNAVPVT->data.numSV;
          app_printf("\r\ngnss_receiver_task : satellite is : %ld\r\n",
                     gnss_data.no_of_satellites);
          is_sensor_data_available = true;
          break;
        } else {
          gnss_max_m10s_delay(GNSS_DATA_TIMEOUT_RETRY_DELAY);
          app_printf("\r\ngnss_receiver_task :"
                     " Data is not received, retry-count: %d\r\n",
                     data_retry_count);
          continue;
        }
      }
    }
    if (true == is_sensor_data_available) {
      app_printf("\r\ngnss_receiver_task :"
                 " Data is received, break the fix-type retry loop\r\n");
      break;
    }
    retry_count++;
    gnss_max_m10s_delay(GNSS_PER_RETRY_DELAY);
  }
  /// If retry count reached to maximum value then send data as null
  if (GNSS_RETRY_COUNT == retry_count) {
    retry_count = 0;
    is_sensor_data_available = false;
    app_printf("\r\ngnss_receiver_task :"
               " GNSS receiver read failed due to fix type not found,"
               " fix_type:%u\n", fix_type);
  }
  gnss_max_m10s_delay(GNSS_TASK_DELAY);
}
