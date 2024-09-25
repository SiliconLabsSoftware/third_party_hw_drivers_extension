/***************************************************************************//**
 * @file app.c
 * @brief Example application
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
#include <stdbool.h>
#include "maxim_fuel_gauge_max17048.h"
#include "app_assert.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

extern bool update_rcom;
static bool handle_interrupt = false;
static mikroe_i2c_handle_t app_i2c_instance = NULL;
static void max17048_soc_callback(sl_max17048_irq_source_t source,
                                  void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t status;
  uint32_t vcell, cb_data, soc;
  float crate;
  uint8_t id;
  uint16_t version;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_fuel_gauge;
#endif

  // Initialize the max17048
  status = max17048_init(app_i2c_instance);
  app_assert_status(status);

#if (defined(SLI_SI917))

  /**
   * Due to calling trim_efuse API on ADC init in driver
   * it will change the clock frequency,
   * if we are not initialize the debug again
   * it will print the garbage data in console output.
   */
  DEBUGINIT();
#endif

  // Read and print the device ID
  status = max17048_get_id(&id);
  app_printf("status: 0x%x      ", (int)status);
  app_printf("ID: 0x%x\r\n", id);

  // Read and print the device version
  status = max17048_get_production_version(&version);
  app_printf("status: 0x%x      ", (int)status);
  app_printf("Device version: 0x%x\r\n", version);

  // Read and print the battery voltage
  status = max17048_get_vcell(&vcell);
  app_printf("status: 0x%x      ", (int)status);
  app_printf("vcell: %ld mV\r\n", vcell);

  // Read and print the battery state of charge
  status = max17048_get_soc(&soc);
  app_printf("status: 0x%x      ", (int)status);
  app_printf("soc: %ld%%\r\n", soc);

  // Read and print the average SOC rate of change
  status = max17048_get_crate(&crate);
  app_printf("status: 0x%x      ", (int)status);
  app_printf("crate: %f%%\r\n", crate);

  // Enables the MAX17048 state-of-charge (SOC) interrupt
  status = max17048_enable_soc_interrupt(max17048_soc_callback, &cb_data);
  app_printf("status: 0x%x      ", (int)status);
  app_printf("enable SOC interrupt\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  // Update rcom value
  if (update_rcom) {
    max17048_update_rcom();
    update_rcom = false;
  }

  // Handle SoC interrupt
  if (handle_interrupt) {
    sl_status_t status;
    uint32_t soc, vcell;
    float crate;

    app_printf("\nSOC interrupt >\r\n");
    // Read and print the battery state of charge
    status = max17048_get_soc(&soc);
    app_printf("status: 0x%x      ", (int)status);
    app_printf("SOC is now: %ld%%\r\n", soc);

    // Read and print the battery voltage
    status = max17048_get_vcell(&vcell);
    app_printf("status: 0x%x      ", (int)status);
    app_printf("vcell is now: %ld mV\r\n", vcell);

    // Read and print the average SOC rate of change
    status = max17048_get_crate(&crate);
    app_printf("status: 0x%x      ", (int)status);
    app_printf("crate is now: %f%%\r\n", crate);

    handle_interrupt = false;
  }
}

static void max17048_soc_callback(sl_max17048_irq_source_t source,
                                  void *data)
{
  (void)source;
  (void)data;

  handle_interrupt = true;
}
