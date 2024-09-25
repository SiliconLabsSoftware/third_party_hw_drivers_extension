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

#include "sl_sleeptimer.h"
#include "mikroe_maxm86161.h"

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
#define I2C_INSTANCE_USED             SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

bool sensor_init_ok = false;
static uint16_t counter = 1000;
static mikroe_i2c_handle_t app_i2c_instance = NULL;

void app_init(void)
{
  uint8_t rd_stat;
  app_printf(
    "- MAXM86161 - Heartrate 2 Click board driver, example application. -\n");

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  if (SL_STATUS_OK != mikroe_maxm86161_init(app_i2c_instance)) {
    app_printf("> MAXM86161 - Heartrate 2 Click board driver init failed.\n");
  } else {
    sensor_init_ok = true;
  }

  if (sensor_init_ok) {
    sl_sleeptimer_delay_millisecond(1000);

    mikroe_maxm86161_set_en(MAXM86161_PIN_HIGH);
    sl_sleeptimer_delay_millisecond(100);
    mikroe_maxm86161_soft_reset();
    rd_stat = mikroe_maxm86161_generic_read(MIKROE_MAXM86161_REG_PART_ID);

    if (rd_stat != MIKROE_MAXM86161_DEV_ID) {
      app_printf("---- WRONG ID ----\n");
      app_printf("Please restart your system.\n");
      for ( ; ; ) {}
    }

    if (mikroe_maxm86161_default_cfg(MAXM86161_CONFIG_GREEN) == SL_STATUS_OK) {
      app_printf("The module has been configured!\n");
    }
    sl_sleeptimer_delay_millisecond(1000);

    app_printf("> App init done.\n");
    app_printf("> Starting measurement.\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  mikroe_maxm86161_fifo_data_t fifo_object;

  mikroe_maxm86161_read_fifo(&fifo_object);

  if (fifo_object.tag == MIKROE_MAXM86161_FIFO_TAG_PPG1_LEDC1) {
    counter++;
    if (fifo_object.data_val > 1000) {
      app_printf("%lu;\r\n", fifo_object.data_val);
      counter = 1000;
    } else if (counter > 1000) {
      app_printf("Please place your index finger on the sensor.\r\n");
      counter = 0;
    }
  }
}
