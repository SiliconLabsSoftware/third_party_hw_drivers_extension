/***************************************************************************//**
 * @file app.c
 * @brief Example application
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

#include "app_assert.h"
#include "sl_sleeptimer.h"
#include "mikroe_scd41.h"
#include "sparkfun_sps30_i2c.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...) app_log(__VA_ARGS__)
#endif

#define READING_INTERVAL_MSEC    5000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool trigger_process = false;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t stt;
  uint16_t ser_num[3];
  mikroe_scd41_feature_data_t version_data;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  app_printf("Hello World - Mikroe HVAC Click Bundle example\r\n");
  app_printf("-----------------------------------------\r\n");

  // SCD41 Initialization
  stt = mikroe_scd41_init(app_i2c_instance);
  app_assert_status(stt);
  app_printf("Mikroe SCD41\r\n\nInit Done\r\n");

  mikroe_scd41_send_cmd(MIKROE_SCD41_PERFORM_FACTORY_RESET);
  app_printf("Perform Factory Reset Done\r\n");

  sl_sleeptimer_delay_millisecond(2000);

  stt = mikroe_scd41_get_serial_number(ser_num);
  app_assert_status(stt);
  app_printf("Get Serial Number Done\r\n");
  app_printf("Serial Number: %.4d-%.4d-%.4d\r\n",
             ( uint16_t ) ser_num[0],
             ( uint16_t ) ser_num[1],
             ( uint16_t ) ser_num[2]);

  stt = mikroe_scd41_get_feature_set_version(&version_data);
  app_assert_status(stt);
  app_printf("Get Feature Set Version Done\r\n");
  app_printf("SCD41 - Features\r\n");
  app_printf("Product Type    : %d \r\n",
             ( uint16_t ) version_data.product_type);
  app_printf("Platform Type   : %d \r\n",
             ( uint16_t ) version_data.platform_type);
  app_printf("Product Version : %d.%d \r\n",
             ( uint16_t ) version_data.product_major_version,
             ( uint16_t ) version_data.product_minor_version);

  // Trigger measure single shot */
  mikroe_scd41_send_cmd(MIKROE_SCD41_MEASURE_SINGLE_SHOT);
  app_printf("-----------------------------------------\r\n");

  // SPS30 Initialization
  stt = sps30_init(app_i2c_instance);
  app_assert_status(stt);
  app_printf("Sparkfun SPS30\r\nInit Done\r\n");

  while (sps30_probe() != 0) {
    app_printf("SPS30 Sensor Probing Failed\r\n");
    sl_sleeptimer_delay_millisecond(1000);
  }

  app_printf("SPS30 Sensor Probing Successful\r\n");

  uint8_t fw_major;
  uint8_t fw_minor;
  stt = sps30_read_firmware_version(&fw_major, &fw_minor);
  app_assert_status(stt);
  app_printf("FW Version: %u.%u\r\n", fw_major, fw_minor);

  uint8_t serial_number[SPS30_MAX_SERIAL_LEN];
  stt = sps30_get_serial((char *)serial_number);
  app_assert_status(stt);
  app_printf("Serial Number: %s\r\n", serial_number);

  stt = sps30_start_measurement();
  app_assert_status(stt);
  app_printf("Measurements Started\r\n");

  app_printf("-----------------------------------------\r\n\n");

  // Start periodic measurement
  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        READING_INTERVAL_MSEC,
                                        app_timer_callback,
                                        NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t stt;
  sps30_measurement_t measure;
  mikroe_scd41_measuremen_data_t hvac_data;

  if (trigger_process) {
    trigger_process = false;
    stt = mikroe_scd41_read_measurement(&hvac_data);

    if (SL_STATUS_OK == stt) {
      app_printf("<----------- Measured Values ----------->\r\n");
      app_printf("CO2 Concent = %dppm\r\n", hvac_data.co2_concent);
      app_printf("Temperature = %.2f%cC \r\n", hvac_data.temperature, 176);
      app_printf("R.Humidity = %.2f %% \r\n", hvac_data.r_humidity);

      // Continue trigger measure single shot
      mikroe_scd41_send_cmd(MIKROE_SCD41_MEASURE_SINGLE_SHOT);
    } else {
      app_printf("Error, code = 0x%lx\r\n", stt);
      while (1) {}
    }

    stt = sps30_read_measurement(&measure);
    if (stt != SL_STATUS_OK) {
      app_printf("Error Reading Measurement\r\n");
    } else {
      app_printf("%0.2f pm1.0\r\n"
                 "%0.2f pm2.5\r\n"
                 "%0.2f pm4.0\r\n"
                 "%0.2f pm10.0\r\n"
                 "%0.2f nc0.5\r\n"
                 "%0.2f nc1.0\r\n"
                 "%0.2f nc2.5\r\n"
                 "%0.2f nc4.5\r\n"
                 "%0.2f nc10.0\r\n"
                 "%0.2f typical particle size\r\n",
                 measure.mc_1p0,
                 measure.mc_2p5,
                 measure.mc_4p0,
                 measure.mc_10p0,
                 measure.nc_0p5,
                 measure.nc_1p0,
                 measure.nc_2p5,
                 measure.nc_4p0,
                 measure.nc_10p0,
                 measure.typical_particle_size);
    }
    app_printf("-----------------------------------------\r\n\n");
  }
}

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
