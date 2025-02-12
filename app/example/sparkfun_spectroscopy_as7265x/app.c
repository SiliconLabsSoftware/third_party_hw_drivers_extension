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

#include "sl_sleeptimer.h"
#include "sparkfun_as7265x.h"
#include "app_assert.h"

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

// Choose which type of test you want by uncommenting the #define

#define TEST_BASIC_READING
// #define TEST_BASIC_READING_WITH_LED
// #define TEST_READ_RAW_VALUE
// #define TEST_MAX_READ_RATE

static mikroe_i2c_handle_t app_i2c_instance = NULL;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t status;
  uint8_t temperature = 0;
  uint8_t hardware_version_data = 0;
  sparkfun_as7265x_firmware_version_t firmware_version_data;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  status = sparkfun_as7265x_init(app_i2c_instance);
  app_assert_status(status);
  app_printf("\nInit as7265x sensor successfully\r\n");

  // Get sensor temperature.
  app_printf("\n-------------Sensor temperature-------------\r\n");
  status = sparkfun_as7265x_get_temperature(SPARKFUN_AS72651_NIR, &temperature);
  app_assert_status(status);
  app_printf("AS72651 Temperature = %d\r\n", temperature);

  status = sparkfun_as7265x_get_temperature(SPARKFUN_AS72652_VISIBLE,
                                            &temperature);
  app_assert_status(status);
  app_printf("AS72652 Temperature = %d\r\n", temperature);

  status = sparkfun_as7265x_get_temperature(SPARKFUN_AS72653_UV, &temperature);
  app_assert_status(status);
  app_printf("AS72653 Temperature = %d\r\n", temperature);

  // Get hardware and firmware version.
  app_printf("\n---------Hardware and firmware version--------\r\n");
  status = sparkfun_as7265x_get_device_type(&hardware_version_data);
  app_assert_status(status);
  app_printf("AMS Device Type: 0x%x\r\n", hardware_version_data);

  status = sparkfun_as7265x_get_hardware_version(&hardware_version_data);
  app_assert_status(status);
  app_printf("AMS Hardware Version: 0x%x\r\n", hardware_version_data);

  status = sparkfun_as7265x_get_firmware_version(&firmware_version_data);
  app_assert_status(status);
  app_printf("Major Firmware Version: 0x%x\r\n",
             firmware_version_data.major_firmware_version);
  app_printf("Patch Firmware Version: 0x%x\r\n",
             firmware_version_data.patch_firmware_version);
  app_printf("Build Firmware Version: 0x%x\r\n",
             firmware_version_data.build_firmware_version);

#if defined(TEST_BASIC_READING_WITH_LED) \
  || defined(TEST_READ_RAW_VALUE)        \
  || defined(TEST_MAX_READ_RATE)
  status = sparkfun_as7265x_disable_indicator();
  app_assert_status(status);
#endif

#ifdef TEST_MAX_READ_RATE
  // Rather than toggle the LEDs with each measurement, turn on LEDs all the
  // time
  status = sparkfun_as7265x_enable_bulb(SPARKFUN_AS7265x_LED_WHITE);
  app_assert_status(status);
  status = sparkfun_as7265x_enable_bulb(SPARKFUN_AS7265x_LED_IR);
  app_assert_status(status);
  status = sparkfun_as7265x_enable_bulb(SPARKFUN_AS7265x_LED_UV);
  app_assert_status(status);

  // Integration time = 1 * 2.8(ms) = 2.8(ms)
  status = sparkfun_as7265x_set_integration_cycles(1);
  app_assert_status(status);
#endif

#if defined(TEST_BASIC_READING)           \
  || defined(TEST_BASIC_READING_WITH_LED) \
  || defined(TEST_READ_RAW_VALUE)         \
  || defined(TEST_MAX_READ_RATE)

  app_printf("\n----Spectral data in counts/microwatt/cm2----\r\n");
  app_printf(
    "|    A    |    B    |    C    |    D    |    E    |    F    |\
    G    |    H    |    R    |    I    |    S    |    J    |    T    |\
    U    |    V    |    W    |    K    |   L\r\n");

#endif
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t status;

#if defined(TEST_BASIC_READING) || defined(TEST_BASIC_READING_WITH_LED)
  sparkfun_as7265x_calibrated_data_t cal_value;
#ifdef TEST_BASIC_READING_WITH_LED
  status = sparkfun_as7265x_take_measurements_with_bulb();
#else
  status = sparkfun_as7265x_take_measurements();
#endif

  if (status == SL_STATUS_OK) {
    status = sparkfun_as7265x_get_all_calibrated_value(&cal_value);
    app_assert_status(status);

    app_printf("|%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_A]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_B]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_C]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_D]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_E]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_F]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_G]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_H]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_R]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_I]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_S]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_J]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_T]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_U]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_V]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_W]);
    app_printf("%9.3f|", cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_K]);
    app_printf("%9.3f\r\n",
               cal_value.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_L]);
  }
#endif

#if defined(TEST_READ_RAW_VALUE) || defined(TEST_MAX_READ_RATE)

#ifdef TEST_MAX_READ_RATE
  uint32_t start_time = 0;
  uint32_t end_time = 0;

  start_time = sl_sleeptimer_tick_to_ms(sl_sleeptimer_get_tick_count());
#endif

  sparkfun_as7265x_color_data_t color_data;
  status = sparkfun_as7265x_take_measurements();

#ifdef TEST_MAX_READ_RATE
  end_time = sl_sleeptimer_tick_to_ms(sl_sleeptimer_get_tick_count());
  app_printf("Data rate = %.2f\r\n", (float)1000 / (end_time - start_time));
#endif

  if (status == SL_STATUS_OK) {
    status = sparkfun_as7265x_get_all_color_channel(&color_data);
    app_assert_status(status);

    app_printf("|%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_A]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_B]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_C]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_D]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_E]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_F]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_G]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_H]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_R]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_I]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_S]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_J]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_T]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_U]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_V]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_W]);
    app_printf("%9d|", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_K]);
    app_printf("%9d\r\n", color_data.channel[SPARKFUN_AS7265x_COLOR_CHANNEL_L]);
  }
#endif
}
