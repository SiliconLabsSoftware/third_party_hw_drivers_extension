/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
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
#include "sparkfun_ak9753.h"

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

static mikroe_i2c_handle_t app_i2c_instance = NULL;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  sparkfun_ak9753_config_t sparkfun_ak9753_cfg = {
    .I2C_address = SPARKFUN_AK9753_ADDR,
    .i2cspm_instance = app_i2c_instance,
    .cut_off_freq = SPARKFUN_AK975X_FREQ_8_8HZ,
    .mode = SPARKFUN_AK975X_MODE_0,
    .upper_threshold13 = 0x00,
    .lower_threshold13 = 0x00,
    .upper_threshold24 = 0x00,
    .lower_threshold24 = 0x00,
    .hysteresis_value24 = 0x00,
    .hysteresis_value13 = 0x00,
    .int_source_setting = 0x00,
    .int_present = false,
    .PDN_present = false
  };

  sparkfun_ak9753_init(&sparkfun_ak9753_cfg);
  sparkfun_ak9753_set_hysteresis_eeprom_ir13(10);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  bool data_ready = false;

  sparkfun_ak9753_is_data_ready(&data_ready);

  if (data_ready) {
    int16_t ret = 0;
    float tempC = 0;
    float tempF = 0;
    app_printf("1:DWN[");
    sparkfun_ak9753_get_ir1_data(&ret);
    app_printf("%d", ret);
    sparkfun_ak9753_get_ir2_data(&ret);
    app_printf("]\t2:LFT[");
    app_printf("%d", ret);
    sparkfun_ak9753_get_ir3_data(&ret);
    app_printf("]\t3:UP[");
    app_printf("%d", ret);
    sparkfun_ak9753_get_ir4_data(&ret);
    app_printf("]\t4:RGH[");
    app_printf("%d", ret);
    sparkfun_ak9753_get_tempC(&tempC);
    app_printf("]\ttempC[");
    app_printf("%f", tempC);
    sparkfun_ak9753_get_tempF(&tempF);
    app_printf("]\ttempF[");
    app_printf("%f", tempF);
    app_printf("]\r\n");
    sparkfun_ak9753_get_dummy();
  }
}
