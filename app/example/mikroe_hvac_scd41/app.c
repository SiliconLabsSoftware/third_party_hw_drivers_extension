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

#include "mikroe_scd41.h"
#include "sl_sleeptimer.h"

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

#define READING_INTERVAL_MSEC        5000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool trigger_process = false;
static uint16_t ser_num[3];
static mikroe_scd41_measuremen_data_t hvac_data;
static mikroe_scd41_feature_data_t version_data;
static mikroe_i2c_handle_t app_i2c_instance = NULL;
void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

void app_init(void)
{
  sl_status_t stt;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  app_printf("Hello World - Mikroe HVAC Click example\r\n");

  stt = mikroe_scd41_init(app_i2c_instance);
  app_printf("Mikroe SCD41 Init = 0x%x\r\n", (uint16_t)stt);

  mikroe_scd41_send_cmd(MIKROE_SCD41_PERFORM_FACTORY_RESET);
  app_printf("Perform Factory Reset \r\n");
  app_printf("-----------------------------------------\r\n");

  sl_sleeptimer_delay_millisecond(2000);

  stt = mikroe_scd41_get_serial_number(ser_num);
  app_printf("mikroe_scd41_get_serial_number = 0x%x\r\n", (uint16_t)stt);
  app_printf("SCD41 - Serial Number: %.4d-%.4d-%.4d\r\n",
             ( uint16_t ) ser_num[0],
             ( uint16_t ) ser_num[1],
             ( uint16_t ) ser_num[2]);
  app_printf("-----------------------------------------\r\n");

  stt = mikroe_scd41_get_feature_set_version(&version_data);
  app_printf("mikroe_scd41_get_feature_set_version = 0x%x\r\n", (uint16_t)stt);
  app_printf("SCD41 - Features\r\n");
  app_printf("Product Type    : %d \r\n",
             ( uint16_t ) version_data.product_type);
  app_printf("Platform Type   : %d \r\n",
             ( uint16_t ) version_data.platform_type);
  app_printf("Product Version : %d.%d \r\n",
             ( uint16_t ) version_data.product_major_version,
             ( uint16_t ) version_data.product_minor_version);
  app_printf("-----------------------------------------\r\n");

  // Trigger measure single shot */
  mikroe_scd41_send_cmd(MIKROE_SCD41_MEASURE_SINGLE_SHOT);
  app_printf("sl_sleeptimer_start_periodic = 0x%lx\r\n",
             sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                                   READING_INTERVAL_MSEC,
                                                   app_timer_callback,
                                                   NULL,
                                                   0,
                                                   0));
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (trigger_process) {
    trigger_process = false;
    sl_status_t stt = mikroe_scd41_read_measurement(&hvac_data);

    if (SL_STATUS_OK == stt) {
      app_printf("CO2 Concent = %dppm\r\n", hvac_data.co2_concent);
      app_printf("Temperature = %.2f%cC \r\n", hvac_data.temperature, 176);
      app_printf("R.Humidity = %.2f %% \r\n", hvac_data.r_humidity);
      app_printf("-----------------------------------------\r\n");

      // Continue trigger measure single shot
      mikroe_scd41_send_cmd(MIKROE_SCD41_MEASURE_SINGLE_SHOT);
    } else {
      app_printf("Error, code = 0x%lx\r\n", stt);
      while (1) {}
    }
  }
}

void app_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  trigger_process = true;
}
