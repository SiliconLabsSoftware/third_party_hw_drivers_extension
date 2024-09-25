/***************************************************************************//**
 * @file sparkfun_as7265x.h
 * @brief AS7265X Prototypes
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

#ifndef SPARKFUN_AS7265X_H_
#define SPARKFUN_AS7265X_H_

#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "drv_i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup as7265x AS7265X Driver API*/

#define SPARKFUN_AS7265X_ADDR                0x49 // Device I2C address

// Gets current tick count.
#define sparkfun_as7265x_get_tick_count() sl_sleeptimer_get_tick_count64()

// Gets current millisecond count.
#define sparkfun_as7265x_get_ms() \
  sl_sleeptimer_tick_to_ms(sl_sleeptimer_get_tick_count64())

// Delay 'ms' milliseconds
#define sparkfun_as7265x_delay_ms(ms)     sl_sleeptimer_delay_millisecond(ms)

/***************************************************************************//**
 * @addtogroup as7265x
 * @brief  AS7265X Driver API
 * @details
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *    Typedef for color channel data.
 ******************************************************************************/
typedef enum SPARKFUN_AS7265x_COLOR_CHANNEL {
  SPARKFUN_AS7265x_COLOR_CHANNEL_A = 0,
  SPARKFUN_AS7265x_COLOR_CHANNEL_B,
  SPARKFUN_AS7265x_COLOR_CHANNEL_C,
  SPARKFUN_AS7265x_COLOR_CHANNEL_D,
  SPARKFUN_AS7265x_COLOR_CHANNEL_E,
  SPARKFUN_AS7265x_COLOR_CHANNEL_F,
  SPARKFUN_AS7265x_COLOR_CHANNEL_G,
  SPARKFUN_AS7265x_COLOR_CHANNEL_H,
  SPARKFUN_AS7265x_COLOR_CHANNEL_I,
  SPARKFUN_AS7265x_COLOR_CHANNEL_J,
  SPARKFUN_AS7265x_COLOR_CHANNEL_K,
  SPARKFUN_AS7265x_COLOR_CHANNEL_L,
  SPARKFUN_AS7265x_COLOR_CHANNEL_R,
  SPARKFUN_AS7265x_COLOR_CHANNEL_S,
  SPARKFUN_AS7265x_COLOR_CHANNEL_T,
  SPARKFUN_AS7265x_COLOR_CHANNEL_U,
  SPARKFUN_AS7265x_COLOR_CHANNEL_V,
  SPARKFUN_AS7265x_COLOR_CHANNEL_W,
  SPARKFUN_AS7265x_COLOR_CHANNEL_MAX
} sparkfun_as7265x_color_channel_t;

/***************************************************************************//**
 * @brief
 *    Typedef for calibration data.
 ******************************************************************************/
typedef struct sparkfun_as7265x_calibrated_data {
  /*
   * Access to a channel:
   *  {
   *    sparkfun_as7265x_calibrated_data_t cal_data;
   *    uint8_t c_i;
   *
   *    c_i = SPARKFUN_AS7265x_COLOR_CHANNEL_A;
   *
   *    if (c_i < SPARKFUN_AS7265x_COLOR_CHANNEL_MAX) {
   *      cal_data.channel[c_i] = 0.1;
   *    }
   *  }
   */
  float channel[18]; ///< value of calibration data.
} sparkfun_as7265x_calibrated_data_t;

/***************************************************************************//**
 * @brief
 *    Typedef for color channel.
 ******************************************************************************/
typedef struct sparkfun_as7265x_color_data {
  /*
   * Access to a channel:
   *  {
   *    sparkfun_as7265x_color_data_t color_data;
   *    uint8_t c_i;
   *
   *    c_i = SPARKFUN_AS7265x_COLOR_CHANNEL_A;
   *
   *    if (c_i < SPARKFUN_AS7265x_COLOR_CHANNEL_MAX) {
   *      cal_data.channel[c_i] = 1;
   *    }
   *  }
   */
  uint16_t channel[18]; ///< value of color channel.
} sparkfun_as7265x_color_data_t;

/***************************************************************************//**
 * @brief
 *    Typedef for device type.
 ******************************************************************************/
typedef enum sparkfun_as7265x_device_type {
  SPARKFUN_AS72651_NIR = 0, ///< SPARKFUN_AS72651
  SPARKFUN_AS72652_VISIBLE = 1, ///< SPARKFUN_AS72652
  SPARKFUN_AS72653_UV = 2, ///< SPARKFUN_AS72653
} sparkfun_as7265x_device_type_t;

/***************************************************************************//**
 * @brief
 *    Typedef for led type.
 ******************************************************************************/
typedef enum sparkfun_as7265x_led_type {
  SPARKFUN_AS7265x_LED_WHITE = 0, ///< White LED is connected to x51
  SPARKFUN_AS7265x_LED_IR = 1, ///< IR LED is connected to x52
  SPARKFUN_AS7265x_LED_UV = 2, ///< UV LED is connected to x53
} sparkfun_as7265x_led_type_t;

/***************************************************************************//**
 * @brief
 *    Typedef for device mode.
 ******************************************************************************/
typedef enum sparkfun_as7265x_device_mode {
  SPARKFUN_AS7265X_MEASUREMENT_MODE_4CHAN = 0b00, ///< Mode 0: 4 channels
  SPARKFUN_AS7265X_MEASUREMENT_MODE_4CHAN_2 = 0b01, ///< Mode 1: 4 channels
  SPARKFUN_AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS = 0b10, ///< Mode 2: all 6
                                                             ///<   channels
  SPARKFUN_AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT = 0b11 ///< Mode 3: One-Shot
                                                          ///<   of mode 2
} sparkfun_as7265x_device_mode_t;

/***************************************************************************//**
 * @brief
 *    Typedef for gain.
 ******************************************************************************/
typedef enum sparkfun_as7265x_gain {
  SPARKFUN_AS7265X_GAIN_1X = 0b00, ///< Gain 1x
  SPARKFUN_AS7265X_GAIN_37X = 0b01, ///< Gain 3.7x
  SPARKFUN_AS7265X_GAIN_16X = 0b10, ///< Gain 16x
  SPARKFUN_AS7265X_GAIN_64X = 0b11 ///< Gain 64x
} sparkfun_as7265x_gain_t;

/***************************************************************************//**
 * @brief
 *    Typedef for indicator LED current.
 ******************************************************************************/
typedef enum sparkfun_as7265x_indicator_led_current {
  SPARKFUN_AS7265X_INDICATOR_CURRENT_LIMIT_1MA = 0b00, ///< 1mA
  SPARKFUN_AS7265X_INDICATOR_CURRENT_LIMIT_2MA = 0b01, ///< 2mA
  SPARKFUN_AS7265X_INDICATOR_CURRENT_LIMIT_4MA = 0b10, ///< 4mA
  SPARKFUN_AS7265X_INDICATOR_CURRENT_LIMIT_8MA = 0b11 ///< 8mA
} sparkfun_as7265x_indicator_led_current_t;

/***************************************************************************//**
 * @brief
 *    Typedef for LED current.
 ******************************************************************************/
typedef enum sparkfun_as7265x_led_current {
  SPARKFUN_AS7265X_LED_CURRENT_LIMIT_12_5MA = 0b00, ///< 12.5mA
  SPARKFUN_AS7265X_LED_CURRENT_LIMIT_25MA = 0b01, ///< 25mA
  SPARKFUN_AS7265X_LED_CURRENT_LIMIT_50MA = 0b10, ///< 50mA
  SPARKFUN_AS7265X_LED_CURRENT_LIMIT_100MA = 0b11 ///< 100mA
} sparkfun_as7265x_led_current_t;

/***************************************************************************//**
 * @brief
 *    Typedef for firmware version.
 ******************************************************************************/
typedef struct sparkfun_as7265x_firmware_version {
  uint8_t major_firmware_version;
  uint8_t patch_firmware_version;
  uint8_t build_firmware_version;
} sparkfun_as7265x_firmware_version_t;

/***************************************************************************//**
 * @brief
 *    This function does a soft reset.
 *    Give sensor at least 1000ms to reset.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_soft_reset(void);

/***************************************************************************//**
 * @brief
 *    This function get the temperature of a given device in C.
 *
 * @param[in] device_number
 *    Number of device.
 * @param[out] temp_val
 *    Temperature value.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_get_temperature(
  sparkfun_as7265x_device_type_t device,
  uint8_t *temp_val);

/***************************************************************************//**
 * @brief
 *    This function set the current limit of onboard LED.
 *    Default is max 8mA = 0b11.
 *
 * @param[in] current
 *            SPARKFUN_AS7265X_INDICATOR_CURRENT_LIMIT_1MA.
 *            SPARKFUN_AS7265X_INDICATOR_CURRENT_LIMIT_2MA.
 *            SPARKFUN_AS7265X_INDICATOR_CURRENT_LIMIT_4MA.
 *            SPARKFUN_AS7265X_INDICATOR_CURRENT_LIMIT_8MA.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_set_indicator_current(
  sparkfun_as7265x_indicator_led_current_t current);

/***************************************************************************//**
 * @brief
 *    Disable the onboard indicator LED.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_disable_indicator(void);

/***************************************************************************//**
 * @brief
 *    Enable the onboard indicator LED.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_enable_indicator(void);

/***************************************************************************//**
 * @brief
 *    Set the current limit of bulb/LED.
 *
 * @param[in] current
 *            SPARKFUN_AS7265X_LED_CURRENT_LIMIT_12_5MA.
 *            SPARKFUN_AS7265X_LED_CURRENT_LIMIT_25MA.
 *            SPARKFUN_AS7265X_LED_CURRENT_LIMIT_50MA.
 *            SPARKFUN_AS7265X_LED_CURRENT_LIMIT_100MA.
 * @param[in] led_type
 *            SPARKFUN_AS7265x_LED_WHITE.
 *            SPARKFUN_AS7265x_LED_IR.
 *            SPARKFUN_AS7265x_LED_UV.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_set_bulb_current(
  sparkfun_as7265x_led_current_t current,
  sparkfun_as7265x_led_type_t led_type);

/***************************************************************************//**
 * @brief
 *    Disable the LED or bulb on a given device.
 *
 * @param[in] led_type
 *            SPARKFUN_AS7265x_LED_WHITE.
 *            SPARKFUN_AS7265x_LED_IR.
 *            SPARKFUN_AS7265x_LED_UV.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_disable_bulb(sparkfun_as7265x_led_type_t led_type);

/***************************************************************************//**
 * @brief
 *    Enable the LED or bulb on a given device.
 *
 * @param[in] led_type
 *            SPARKFUN_AS7265x_LED_WHITE.
 *            SPARKFUN_AS7265x_LED_IR.
 *            SPARKFUN_AS7265x_LED_UV.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_enable_bulb(sparkfun_as7265x_led_type_t led_type);

/***************************************************************************//**
 * @brief
 *    Checks to see if DRDY flag is set in the control setup register.
 *
 * @param[out] is_data_ready
 *    true if data ready otherwise false.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_data_available(bool *is_data_ready);

/***************************************************************************//**
 * @brief
 *    Disables the interrupt pin.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_disable_interrupt(void);

/***************************************************************************//**
 * @brief
 *    Enable the interrupt pin.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_enable_interrupt(void);

/***************************************************************************//**
 * @brief
 *    Sets the integration cycle amount.
 *    Give this function a byte from 0 to 255.
 *    Time will be 2.8ms * [integration cycles + 1].
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_set_integration_cycles(uint8_t cycle_value);

/***************************************************************************//**
 * @brief
 *    Sets the gain value
 *
 * @param[in] gain
 *            SPARKFUN_AS7265X_GAIN_1X.
 *            SPARKFUN_AS7265X_GAIN_37X.
 *            SPARKFUN_AS7265X_GAIN_16X.
 *            SPARKFUN_AS7265X_GAIN_64X.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_set_gain(sparkfun_as7265x_gain_t gain);

/***************************************************************************//**
 * @brief
 *    Sets the measurement mode.
 *    Mode 0: 4 channels out of 6 channels.
 *    Mode 1: Different 4 channels out of 6 channels.
 *    Mode 2: All 6 channels continuously.
 *    Mode 3: One-shot reading of all channels.
 *
 * @param[in] mode
 *            SPARKFUN_AS7265X_MEASUREMENT_MODE_4CHAN.
 *            SPARKFUN_AS7265X_MEASUREMENT_MODE_4CHAN_2.
 *            SPARKFUN_AS7265X_MEASUREMENT_MODE_6CHAN_CONTINUOUS.
 *            SPARKFUN_AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_set_measurement_mode(
  sparkfun_as7265x_device_mode_t mode);

/***************************************************************************//**
 * @brief
 *    Given an address, read four bytes and return the floating point
 *    calibrated value.
 *
 * @param[in] channel
 *    Calibrated address.
 * @param[out] cal_val
 *    Calibrated value.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_get_calibrated_value(
  sparkfun_as7265x_color_channel_t channel,
  float *cal_val);

/***************************************************************************//**
 * @brief
 *    Get calibrated value of all channels.
 *
 * @param[out] cal_val
 *    All calibrated value.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_get_all_calibrated_value(
  sparkfun_as7265x_calibrated_data_t *cal_val);

/***************************************************************************//**
 * @brief
 *    Get the 16-bit value stored in a given channel register.
 *
 * @param[in] channel
 *    Color channel.
 * @param[out] color_data
 *    Color data.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_get_channel(
  sparkfun_as7265x_color_channel_t channel,
  uint16_t *color_data);

/***************************************************************************//**
 * @brief
 *    Get color data of all channels.
 *
 * @param[out] color_data
 *    All color data.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_get_all_color_channel(
  sparkfun_as7265x_color_data_t *color_data);

/***************************************************************************//**
 * @brief
 *    This function takes measurements of all channels with bulbs.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_take_measurements_with_bulb(void);

/***************************************************************************//**
 * @brief
 *    This function takes all channel measurements and polls for data ready
 *    flag.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_take_measurements(void);

/***************************************************************************//**
 * @brief
 *    This function get firmware version.
 *
 * @param[out] firm_ver
 *    Firmware version.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_get_firmware_version(
  sparkfun_as7265x_firmware_version_t *version);

/***************************************************************************//**
 * @brief
 *    This function get hardware version.
 *
 * @param[out] hw_ver
 *    Hardware version.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_get_hardware_version(uint8_t *hw_ver);

/***************************************************************************//**
 * @brief
 *    This function get device type.
 *
 * @param[out] dev_type
 *    Device type.
 *
 * @return sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 *
 ******************************************************************************/
sl_status_t sparkfun_as7265x_get_device_type(uint8_t *dev_type);

/***************************************************************************//**
 * @brief
 *    Initializes the sensor with basic settings.
 *
 * @param[in] i2cspm
 *    The I2C peripheral to use.
 *
 * @return
 *    sl_status_t SL_STATUS_OK on success or SL_STATUS_FAIL on failure.
 ******************************************************************************/
sl_status_t sparkfun_as7265x_init(mikroe_i2c_handle_t i2cspm);

/** @} (end addtogroup as7265x) */

#ifdef __cplusplus
}
#endif

#endif // SPARKFUN_AS7265X_H_
