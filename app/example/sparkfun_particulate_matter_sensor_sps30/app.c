/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
#include "sl_component_catalog.h"
#include "sl_sleeptimer.h"

#ifdef SL_CATALOG_PM_SPS30_I2C_PRESENT
#include "sparkfun_sps30_i2c.h"
#endif
#ifdef SL_CATALOG_PM_SPS30_UART_PRESENT
#include "sparkfun_sps30_uart.h"
#endif

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#define app_printf(...) DEBUGOUT(__VA_ARGS__)

#ifdef SL_CATALOG_PM_SPS30_I2C_PRESENT
#include "sl_i2c_instances.h"
#define I2C_INSTANCE_USED              SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif
#ifdef SL_CATALOG_PM_SPS30_UART_PRESENT
#include "sl_si91x_usart.h"
#define USART_INSTANCE_USED            UART_1
static usart_peripheral_t uart_instance = USART_INSTANCE_USED;
#endif

#else /* None Si91x device */
#include "app_log.h"
#define app_printf(...) app_log(__VA_ARGS__)

#ifdef SL_CATALOG_PM_SPS30_I2C_PRESENT
#include "sl_i2cspm_instances.h"
#endif
#ifdef SL_CATALOG_PM_SPS30_UART_PRESENT
#include "sl_iostream_init_usart_instances.h"
#include "sl_iostream_init_eusart_instances.h"
#endif
#endif

#define READING_INTERVAL_MSEC (3000)

static sl_sleeptimer_timer_handle_t app_periodic_timer;
static volatile bool enable_reading_data = false;
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data);

#ifdef SL_CATALOG_PM_SPS30_I2C_PRESENT
static mikroe_i2c_handle_t app_i2c_instance = NULL;
#endif
#ifdef SL_CATALOG_PM_SPS30_UART_PRESENT
mikroe_uart_handle_t app_uart_instance = NULL;
#endif

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  static sl_status_t  ret;

  /* Initialize SPS 30 */
#ifdef SL_CATALOG_PM_SPS30_I2C_PRESENT
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif
  app_printf("Hello world - SPS30 - Particulate Matter Sensor - I2C !!!\r\n");
  sps30_init(app_i2c_instance);
#endif

#ifdef SL_CATALOG_PM_SPS30_UART_PRESENT
#if (defined(SLI_SI917))
  app_uart_instance = &uart_instance;
#else
  app_uart_instance = sl_iostream_uart_mikroe_handle;

  sl_iostream_set_default(sl_iostream_vcom_handle);
  app_log_iostream_set(sl_iostream_vcom_handle);
#endif
  app_printf("Hello world - SPS30 - Particulate Matter Sensor - UART !!!\r\n");
  sps30_init(app_uart_instance);
#endif

  /* Busy loop for initialization */
  while (sps30_probe() != 0) {
    app_printf("SPS30 sensor probing failed\r\n");
    sl_sleeptimer_delay_millisecond(1000);
  }

  app_printf("SPS30 sensor probing successful\r\n");

#ifdef SL_CATALOG_PM_SPS30_I2C_PRESENT
  uint8_t fw_major;
  uint8_t fw_minor;
  ret = sps30_read_firmware_version(&fw_major, &fw_minor);
  if (ret) {
    app_printf("Error reading version\r\n");
  } else {
    app_printf("FW Ver: %u.%u\r\n", fw_major, fw_minor);
  }
#endif
#ifdef SL_CATALOG_PM_SPS30_UART_PRESENT
  sps30_version_information_t version;
  ret = sps30_read_version(&version);

  if (ret) {
    app_printf("Error reading version\r\n");
  } else {
    app_printf("FW Ver: %u.%u\r\n", version.firmware_major,
               version.firmware_minor);
    app_printf("SHDLC Ver: %u.%u\r\n", version.shdlc_major,
               version.shdlc_minor);
  }
#endif

  uint8_t serial_number[SPS30_MAX_SERIAL_LEN];
  ret = sps30_get_serial((char *)serial_number);
  if (ret) {
    app_printf("Error reading serial number\r\n");
  } else {
    app_printf("Serial Number: %s\r\n", serial_number);
  }

  ret = sps30_start_measurement();
  if (ret) {
    app_printf("Error starting measurement\r\n");
  } else {
    app_printf("Measurements started\r\n");
  }

  // Start timer used for periodic indications.
  sl_sleeptimer_start_periodic_timer_ms(&app_periodic_timer,
                                        READING_INTERVAL_MSEC,
                                        app_periodic_timer_cb,
                                        (void *) NULL,
                                        0,
                                        0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sps30_measurement_t measure;
  static sl_status_t ret;

  if (enable_reading_data) {
    enable_reading_data = false;

    ret = sps30_read_measurement(&measure);
    if (ret) {
      app_printf("Error reading measurement\r\n");
    } else {
      app_printf("<---------- Measured values ---------->\r\n"
                 "%0.2f pm1.0\r\n"
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
  }
}

/**************************************************************************//**
 * Timer callback
 * Called periodically to reading data from SPS30.
 *****************************************************************************/
static void app_periodic_timer_cb(sl_sleeptimer_timer_handle_t *timer,
                                  void *data)
{
  (void)data;
  (void)timer;

  enable_reading_data = true;
}
