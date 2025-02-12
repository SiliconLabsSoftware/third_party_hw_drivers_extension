/***************************************************************************//**
 * @file sparkfun_bmi270.h
 * @brief sparkfun bmi270 header file
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#ifndef SPARKFUN_BMI270_H_
#define SPARKFUN_BMI270_H_

#ifdef __cplusplus
extern "C" {
#endif

//// Includes
#include "sl_status.h"
#include "drv_i2c_master.h"

/******************************************************************************/

/*******************************************************************************
 ***************************  Defines / Macros  ********************************
 ******************************************************************************/
///< Value used to clear BMI270_INIT_CTRL_REG
#define CLR_INIT_CTRL_REG       0x00

///< Value used to clear BMI270_POWER_CFG_REG to disable advance power mode
#define DISABLE_ADV_POWER       0x00

///< Value used to compare BMI270_INIT_STATUS_REG for successful initialization
#define BMI270_INIT_SUCCESSFUL  0x01

///< Value used to set BMI270_POWER_CFG_REG to disable advance power mode
#define ENABLE_ADV_POWER        0x01

///< Value used to set BMI270_INIT_CTRL_REG
#define SET_INIT_CTRL_REG       0x01

///< Value of gravitational force of earth in meter per second square (m/s^2)
#define GRAVITY_EARTH           9.80665

///< Value of chip-id of bmi270 sensor
#define BMI270_CHIP_ID          0x24

///< Value of slave address of bmi270 sensor
#define BMI270_ADDR             0x68

///< Value used to set BMI270_CMD_REG for soft reset of bmi270 sensor.
#define SOFT_RESET_CMD          0xB6

///< Value of chunk size of a configuration file for burst write
#define CHUNK_SIZE              1024

///< Value of delay counter for milli seconds delay
#define MS_DELAY_COUNTER        4600

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/
/// @brief Enum for bmi270 commands
typedef enum bmi270_commands {
  ///< Register used to read chip ID of bmi270 sensor
  BMI270_CHIP_IDENTIFY_CODE_REG  = 0x00,

  ///< Register used to read the accelerator reading
  BMI270_ACC_X_LSB_ADDR          = 0x0C,

  ///< Register used to read the gyroscope reading
  BMI270_GYR_X_LSB_ADDR          = 0x12,

  ///< Register used to read status of initialization of bmi270 sensor
  BMI270_INIT_STATUS_REG         = 0x21,

  ///< Register used to configure accelerometer
  BMI270_ACC_CONF_ADDR           = 0x40,

  ///< Register used to configure accelerometer range
  BMI270_ACC_RANGE_REG           = 0x41,

  ///< Register used to configure gyroscope
  BMI270_GYR_CONF_ADDR           = 0x42,

  ///< Register used to configure gyroscope range
  BMI270_GYRO_RANGE_REG          = 0x43,

  ///< Register used to make bmi270 sensor ready to load configuration file
  BMI270_INIT_CTRL_REG           = 0x59,

  ///< Register used to write Base address_0 of the initialization data
  BMI270_INIT_ADDR_0_REG         = 0x5B,

  ///< Register used to write Base address_1 of the initialization data
  BMI270_INIT_ADDR_1_REG         = 0x5C,

  ///< Register used to write configuration file in chunks
  BMI270_INIT_DATA_REG           = 0x5E,

  ///< Register used to configuration advance power mode of bmi270 sensor
  BMI270_POWER_CFG_REG           = 0x7C,

  ///< Register used to enable features of bmi270 sensor
  BMI270_PWR_CTRL_ADDR           = 0x7D,

  ///< Register used to soft reset bmi270 sensor
  BMI270_CMD_REG                 = 0x7E,

  ///< Register used to read the temperature LSB reading
  BMI270_TEMP0_REG               = 0x22,

  ///< Register used to read the temperature MSB reading
  BMI270_TEMP1_REG               = 0x23,
} bmi270_commands_t;

/// @brief Enum for bmi270 features
typedef enum {
  ///< Used to enable accelerometer feature
  BMI270_FEATURE_GYRO = 0x02,

  ///< Used to enable gyroscope feature
  BMI270_FEATURE_ACC  = 0x04,

  ///< Used to enable temperature feature
  BMI270_FEATURE_TEMP = 0x08
} bmi270_feature_t;

/// @brief Enum for bmi270 different accelerometer ranges
typedef enum {
  ///< Used to enable +/- 2G accelerometer range
  BMI270_ACCEL_RANGE_2G  = 0x00,

  ///< Used to enable +/- 4G accelerometer range
  BMI270_ACCEL_RANGE_4G  = 0x01,

  ///< Used to enable +/- 8G accelerometer range
  BMI270_ACCEL_RANGE_8G  = 0x02,

  ///< Used to enable +/- 16G accelerometer range
  BMI270_ACCEL_RANGE_16G = 0x03,
} bmi270_accel_range_t;

/// @brief Enum for bmi270 different accelerometer output data rate
/// Bits 0-3 of ACC_CONF register address 0x40 is used for accelerometer ODR
typedef enum {
  ///< Used to enable 25/32 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_25_32_HZ = 0x01,

  ///< Used to enable 25/16 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_25_16_HZ = 0x02,

  ///< Used to enable 25/8 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_25_8_HZ  = 0x03,

  ///< Used to enable 25/4 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_25_4_HZ  = 0x04,

  ///< Used to enable 25/2 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_25_2_HZ  = 0x05,

  ///< Used to enable 25 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_25HZ     = 0x06,

  ///< Used to enable 50 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_50HZ     = 0x07,

  ///< Used to enable 100 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_100HZ    = 0x08,

  ///< Used to enable 200 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_200HZ    = 0x09,

  ///< Used to enable 400 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_400HZ    = 0x0A,

  ///< Used to enable 800 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_800HZ    = 0x0B,

  ///< Used to enable 1600 Hz accelerometer output data rate
  BMI270_ACCEL_ODR_1600HZ   = 0x0C,
} bmi270_accel_odr_t;

/// @brief Enum for bmi270 different accelerometer bandwidth
/// Bits 4-6 of ACC_CONF register address 0x40 is used for accelerometer
///   bandwidth
typedef enum {
  BMI270_ACCEL_OSR4_AVG1  = 0x00,
  BMI270_ACCEL_OSR2_AVG2  = 0x01,
  BMI270_ACCEL_NORM_AVG4  = 0x02,
  BMI270_ACCEL_CIC_AVG8   = 0x03,
  BMI270_ACCEL_RES_AVG16  = 0x04,
  BMI270_ACCEL_RES_AVG32  = 0x05,
  BMI270_ACCEL_RES_AVG64  = 0x06,
  BMI270_ACCEL_RES_AVG128 = 0x07,
}bmi270_accel_bwp_t;

/// @brief Enum for bmi270 different accelerometer filter
/// Bit 7 of ACC_CONF register address 0x40 is used for accelerometer filter
typedef enum {
  /// For Bit : 7 of ACC_CONF register address 0x40

  ///< Used to enable accelerometer power optimize mode
  BMI270_ACCEL_POWER_OPT  = 0x00,

  ///< Used to enable accelerometer performance optimize mode
  BMI270_ACCEL_PERFO_OPT  = 0x01,
}bmi270_accel_filter_t;

/// @brief Enum for bmi270 different gyroscope range in degree per second
/// Bits 0-2 of GYR_RANGE register address 0x43 is used for gyroscope range
typedef enum {
  /// For Bit : 2...0 of GYR_RANGE register address 0x43

  ///< Used to enable +/- 2000 DPS gyroscope range
  BMI270_GYRO_RANGE_2000DPS = 0x00,

  ///< Used to enable +/- 1000 DPS gyroscope range
  BMI270_GYRO_RANGE_1000DPS = 0x01,

  ///< Used to enable +/- 500 DPS gyroscope range
  BMI270_GYRO_RANGE_500DPS  = 0x02,

  ///< Used to enable +/- 250 DPS gyroscope range
  BMI270_GYRO_RANGE_250DPS  = 0x03,

  ///< Used to enable +/- 125 DPS gyroscope range
  BMI270_GYRO_RANGE_125DPS  = 0x04,
} bmi270_gyro_range_t;

/// @brief Enum for bmi270 different gyroscope output data rate
/// Bits 0-3 of GYR_CONF register address 0x42 is used for gyroscope output data
///   rate
typedef enum {
  ///< Used to enable 25Hz gyroscope output data rate
  BMI270_GYRO_ODR_25HZ   = 0x06,

  ///< Used to enable 50Hz gyroscope output data rate
  BMI270_GYRO_ODR_50HZ   = 0x07,

  ///< Used to enable 100Hz gyroscope output data rate
  BMI270_GYRO_ODR_100HZ  = 0x08,

  ///< Used to enable 200Hz gyroscope output data rate
  BMI270_GYRO_ODR_200HZ  = 0x09,

  ///< Used to enable 400Hz gyroscope output data rate
  BMI270_GYRO_ODR_400HZ  = 0x0a,

  ///< Used to enable 800Hz gyroscope output data rate
  BMI270_GYRO_ODR_800HZ  = 0x0b,

  ///< Used to enable 1600Hz gyroscope output data rate
  BMI270_GYRO_ODR_1600HZ = 0x0c,

  ///< Used to enable 3200Hz gyroscope output data rate
  BMI270_GYRO_ODR_3200HZ = 0x0d,
} bmi270_gyro_odr_t;

/// @brief Enum for bmi270 different gyroscope bandwidth
/// Bits 4-5 of GYR_CONF register address 0x42 is used for gyroscope bandwidth
typedef enum {
  BMI270_GYRO_BWP_OSR4   = 0x00,
  BMI270_GYRO_BWP_OSR2   = 0x01,
  BMI270_GYRO_BWP_NORMAL = 0x02,
} bmi270_gyro_bwp_t;

/// @brief Enum for bmi270 different gyroscope noise mode
/// Bit 6 of GYR_CONF register address 0x42 is used for noise mode
typedef enum {
  ///< Used to enable power optimized gyroscope noise mode
  BMI270_GYRO_N_POWER_OPT  = 0x00,

  ///< Used to enable performance optimized gyroscope noise mode
  BMI270_GYRO_N_PERFO_OPT  = 0x01,
}bmi270_gyro_noise_t;

/// @brief Enum for bmi270 different gyroscope filter mode
/// Bit 7 of GYR_CONF register address 0x42 is used for filter mode
typedef enum {
  ///< Used to enable power optimized gyroscope filter mode
  BMI270_GYRO_F_POWER_OPT  = 0x00,

  ///< Used to enable performance optimized gyroscope filter mode
  BMI270_GYRO_F_PERFO_OPT   = 0x01,
}bmi270_gyro_filter_t;

/// @brief Accelerometer Configuration Structure
typedef struct {
  ///< Used to set output data rate of accelerometer
  bmi270_accel_odr_t odr;

  ///< Used to set range of accelerometer
  bmi270_accel_range_t range;

  ///< Used to set bandwidth of accelerometer
  bmi270_accel_bwp_t bw;

  ///< Used to set filter mode of accelerometer
  bmi270_accel_filter_t filter;
} bmi270_accel_config_t;

/// @brief Gyroscope Configuration Structure
typedef struct {
  ///< Used to set output data rate of gyroscope
  bmi270_gyro_odr_t odr;

  ///< Used to set measurement range of gyroscope
  bmi270_gyro_range_t range;

  ///< Used to set bandwidth of gyroscope
  bmi270_gyro_bwp_t bw;

  ///< Used to set power mode of gyroscope
  bmi270_gyro_noise_t noise;

  ///< Used to set filter mode of gyroscope
  bmi270_gyro_filter_t filter;
} bmi270_gyro_config_t;

/// @brief Enable features of bmi270 sensor Structure
typedef struct {
  ///< Used to set features of bmi270 sensor (set using ORing)
  uint8_t features;
} bmi270_enable_features_t;

/// @brief to store the BMI270 sensor configuration data
typedef struct {
  ///< Used to enables features of bmi270 sensor
  bmi270_enable_features_t enable_features;

  ///< Used to set the configuration of accelerometer
  bmi270_accel_config_t acc_config;

  ///< Used to set the configuration of gyroscope
  bmi270_gyro_config_t gyro_config;

  ///< Used to set I2C instance used by the user
  mikroe_i2c_handle_t i2c_instance;

  ///< Used to set Value of slave address of bmi270 sensor
  uint8_t bmi270_slave_address;
} bmi270_cfg_data_t;

// -----------------------------------------------------------------------------
// Prototypes

/*******************************************************************************
 * @brief Initialize and probe BMI270 sensor.
 * @param[in] bmi_cfg_data : pointer to store the BMI270 sensor configuration
 *   data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 ******************************************************************************/
sl_status_t sparkfun_bmi270_init(bmi270_cfg_data_t *bmi_cfg_data);

/*******************************************************************************
 * @brief Reset BMI270 sensor.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 ******************************************************************************/
sl_status_t sparkfun_bmi270_reset(void);

/*******************************************************************************
 * @brief Check presence of BMI270 sensor on I2C bus.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * - \ref SL_STATUS_NOT_AVAILABLE on device not available
 ******************************************************************************/
sl_status_t sparkfun_bmi270_is_present(void);

/*******************************************************************************
 * @brief Enable and disable advance power mode of BMI270 sensor.
 * @param[in] value : data to be write.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 ******************************************************************************/
sl_status_t sparkfun_bmi270_update_advance_power_mode(uint8_t value);

/*******************************************************************************
 * @brief Enable and configure default features of BMI270 sensor.
 * @param[in] bmi_cfg_data : pointer to store the BMI270 sensor configuration
 *   data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note This API must be called after 'sparkfun_bmi270_init'.
 ******************************************************************************/
sl_status_t sparkfun_bmi270_enable_and_config_features(
  bmi270_cfg_data_t *bmi_cfg_data);

/*******************************************************************************
 * @brief Read readings of accelerometer in BMI270 sensor.
 * @param[in] bmi_cfg_data : pointer to store the BMI270 sensor configuration
 *   data.
 * @param[out] acc_buffer : A pointer to accelerometer buffer.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * - \ref SL_STATUS_NOT_AVAILABLE on feature not available due to software
 * configuration.
 * @note This API must be called after
 *   'sparkfun_bmi270_enable_and_config_features'.
 ******************************************************************************/
sl_status_t sparkfun_bmi270_read_acc_reading(bmi270_cfg_data_t *bmi_cfg_data,
                                             double *acc_buffer);

/*******************************************************************************
 * @brief Read readings of gyroscope in BMI270 sensor.
 * @param[in] bmi_cfg_data : pointer to store the BMI270 sensor configuration
 *   data.
 * @param[out] gyro_buffer : A pointer to gyroscope buffer.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * - \ref SL_STATUS_NOT_AVAILABLE on feature not available due to software
 * configuration.
 * @note This API must be called after
 *   'sparkfun_bmi270_enable_and_config_features'.
 ******************************************************************************/
sl_status_t sparkfun_bmi270_read_gyro_reading(bmi270_cfg_data_t *bmi_cfg_data,
                                              double *gyro_buffer);

/*******************************************************************************
 * @brief Read readings of temperature in BMI270 sensor.
 * @param[in] bmi_cfg_data : pointer to store the BMI270 sensor configuration
 *   data.
 * @param[out] temperature_reading : A pointer to store temperature data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * - \ref SL_STATUS_NOT_AVAILABLE on feature not available due to software
 * configuration.
 * @note
 * 1. If the gyroscope is enabled, the temperature reading is updated every
 * 10 ms (+/- 12%). Otherwise, the temperature is updated every 1.28 s.
 * 2. This API must be called after
 *   'sparkfun_bmi270_enable_and_config_features'.
 ******************************************************************************/
sl_status_t sparkfun_bmi270_read_temp_reading(bmi270_cfg_data_t *bmi_cfg_data,
                                              double *temperature_reading);

#ifdef __cplusplus
}
#endif

#endif /* SPARKFUN_BMI270_H_ */

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
