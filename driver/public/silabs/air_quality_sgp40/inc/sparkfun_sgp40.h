/***************************************************************************//**
 * @file sparkfun_sgp40.h
 * @brief sparkfun_sgp40 header file.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#ifndef SPARKFUN_SGP40_H_
#define SPARKFUN_SGP40_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include "sl_status.h"
#include "drv_i2c_master.h"

/***************************************************************************//**
 * @addtogroup SGP40 - Air Quality Sensor
 * @brief This file contains API for Air Quality Sensor SGP40 driver.
 ******************************************************************************/
#define SGP40_ADDRESS                                     0x59

/// @brief GSP40 Lookup table for CRC8.
#define SGP40_LOOKUP_TABLE

/// @brief GSP40 fixed point arithmetic parts.
#define F16(x) \
  ((fix16_t)(((x) >= 0) ? ((x) * 65536.0 + 0.5) : ((x) * 65536.0 - 0.5)))
#define VOC_ALGORITHM_SAMPLING_INTERVAL                        (1.)
#define VOC_ALGORITHM_INITIAL_BLACKOUT                         (45.)
#define VOC_ALGORITHM_VOC_INDEX_GAIN                           (230.)
#define VOC_ALGORITHM_SRAW_STD_INITIAL                         (50.)
#define VOC_ALGORITHM_SRAW_STD_BONUS                           (220.)
#define VOC_ALGORITHM_TAU_MEAN_VARIANCE_HOURS                  (12.)
#define VOC_ALGORITHM_TAU_INITIAL_MEAN                         (20.)
#define VOC_ALGORITHM_INIT_DURATION_MEAN                       (3600. * 0.75)
#define VOC_ALGORITHM_INIT_TRANSITION_MEAN                     (0.01)
#define VOC_ALGORITHM_TAU_INITIAL_VARIANCE                     (2500.)
#define VOC_ALGORITHM_INIT_DURATION_VARIANCE                   (3600. * 1.45)
#define VOC_ALGORITHM_INIT_TRANSITION_VARIANCE                 (0.01)
#define VOC_ALGORITHM_GATING_THRESHOLD                         (340.)
#define VOC_ALGORITHM_GATING_THRESHOLD_INITIAL                 (510.)
#define VOC_ALGORITHM_GATING_THRESHOLD_TRANSITION              (0.09)
#define VOC_ALGORITHM_GATING_MAX_DURATION_MINUTES              ((60. * 3.))
#define VOC_ALGORITHM_GATING_MAX_RATIO                         (0.3)
#define VOC_ALGORITHM_SIGMOID_L                                (500.)
#define VOC_ALGORITHM_SIGMOID_K                                (-0.0065)
#define VOC_ALGORITHM_SIGMOID_X0                               (213.)
#define VOC_ALGORITHM_VOC_INDEX_OFFSET_DEFAULT                 (100.)
#define VOC_ALGORITHM_LP_TAU_FAST                              (20.0)
#define VOC_ALGORITHM_LP_TAU_SLOW                              (500.0)
#define VOC_ALGORITHM_LP_ALPHA                                 (-0.2)
#define VOC_ALGORITHM_PERSISTENCE_UPTIME_GAMMA                 (3. * 3600.)
#define VOC_ALGORITHM_MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING    (64.)
#define VOC_ALGORITHM_MEAN_VARIANCE_ESTIMATOR_FIX16_MAX        (32767.)

/// @brief GSP40 description setting.
#define SPARKFUN_SGP40_TEST_PASSED                             0xD400
#define SPARKFUN_SGP40_TEST_FAILED                             0x4B00

/// @brief GSP40 configuration object.
#define FIX16_MAXIMUM                                          0x7FFFFFFF
#define FIX16_MINIMUM                                          0x80000000
#define FIX16_OVERFLOW                                         0x80000000
#define FIX16_ONE                                              0x00010000

typedef int32_t fix16_t;

/// @brief GSP40  VOC algorithm object.
typedef struct {
  fix16_t mVoc_Index_Offset;
  fix16_t mTau_Mean_Variance_Hours;
  fix16_t mGating_Max_Duration_Minutes;
  fix16_t mSraw_Std_Initial;
  fix16_t mUptime;
  fix16_t mSraw;
  fix16_t mVoc_Index;
  fix16_t m_Mean_Variance_Estimator_Gating_Max_Duration_Minutes;
  bool    m_Mean_Variance_Estimator_Initialized;
  fix16_t m_Mean_Variance_Estimator_Mean;
  fix16_t m_Mean_Variance_Estimator_Sraw_Offset;
  fix16_t m_Mean_Variance_Estimator_Std;
  fix16_t m_Mean_Variance_Estimator_Gamma;
  fix16_t m_Mean_Variance_Estimator_Gamma_Initial_Mean;
  fix16_t m_Mean_Variance_Estimator_Gamma_Initial_Variance;
  fix16_t m_Mean_Variance_Estimator_Gamma_Mean;
  fix16_t m_Mean_Variance_Estimator_Gamma_Variance;
  fix16_t m_Mean_Variance_Estimator_Uptime_Gamma;
  fix16_t m_Mean_Variance_Estimator_Uptime_Gating;
  fix16_t m_Mean_Variance_Estimator_Gating_Duration_Minutes;
  fix16_t m_Mean_Variance_Estimator_Sigmoid_L;
  fix16_t m_Mean_Variance_Estimator_Sigmoid_K;
  fix16_t m_Mean_Variance_Estimator_Sigmoid_X0;
  fix16_t m_Mox_Model_Sraw_Std;
  fix16_t m_Mox_Model_Sraw_Mean;
  fix16_t m_Sigmoid_Scaled_Offset;
  fix16_t m_Adaptive_Lowpass_A1;
  fix16_t m_Adaptive_Lowpass_A2;
  bool    m_Adaptive_Lowpass_Initialized;
  fix16_t m_Adaptive_Lowpass_X1;
  fix16_t m_Adaptive_Lowpass_X2;
  fix16_t m_Adaptive_Lowpass_X3;
} voc_algorithm_params_t;

#ifdef SGP40_LOOKUP_TABLE
// Lookup table for CRC8
static const uint8_t CRC8LookupTable[16][16] = {
  { 0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97, 0xB9, 0x88, 0xDB, 0xEA,
    0x7D, 0x4C, 0x1F, 0x2E },
  { 0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4, 0xFA, 0xCB, 0x98, 0xA9,
    0x3E, 0x0F, 0x5C, 0x6D },
  { 0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11, 0x3F, 0x0E, 0x5D, 0x6C,
    0xFB, 0xCA, 0x99, 0xA8 },
  { 0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52, 0x7C, 0x4D, 0x1E, 0x2F,
    0xB8, 0x89, 0xDA, 0xEB },
  { 0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA, 0x84, 0xB5, 0xE6, 0xD7,
    0x40, 0x71, 0x22, 0x13 },
  { 0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9, 0xC7, 0xF6, 0xA5, 0x94,
    0x03, 0x32, 0x61, 0x50 },
  { 0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C, 0x02, 0x33, 0x60, 0x51,
    0xC6, 0xF7, 0xA4, 0x95 },
  { 0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F, 0x41, 0x70, 0x23, 0x12,
    0x85, 0xB4, 0xE7, 0xD6 },
  { 0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED, 0xC3, 0xF2, 0xA1, 0x90,
    0x07, 0x36, 0x65, 0x54 },
  { 0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE, 0x80, 0xB1, 0xE2, 0xD3,
    0x44, 0x75, 0x26, 0x17 },
  { 0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B, 0x45, 0x74, 0x27, 0x16,
    0x81, 0xB0, 0xE3, 0xD2 },
  { 0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28, 0x06, 0x37, 0x64, 0x55,
    0xC2, 0xF3, 0xA0, 0x91 },
  { 0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0, 0xFE, 0xCF, 0x9C, 0xAD,
    0x3A, 0x0B, 0x58, 0x69 },
  { 0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93, 0xBD, 0x8C, 0xDF, 0xEE,
    0x79, 0x48, 0x1B, 0x2A },
  { 0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56, 0x78, 0x49, 0x1A, 0x2B,
    0xBC, 0x8D, 0xDE, 0xEF },
  { 0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15, 0x3B, 0x0A, 0x59, 0x68,
    0xFF, 0xCE, 0x9D, 0xAC }
};
#endif

/***************************************************************************//**
 * @brief
 *    Initialize the SGP40 I2CSPM instance
 *
 * @param[in] i2cspm
 *    The I2C peripheral to use.
 *
 * @return
 *   @li @ref SL_STATUS_OK Success.
 *
 *   @li @ref SL_STATUS_FAIL Failure.
 ******************************************************************************/
sl_status_t sparkfun_sgp40_init(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *  Sparkfun SGP40 measurement test function.
 *
 * @details
 *  This function triggers the built-in self-test checking for integrity of
 *  both hotplate and MOX materia of the Sparkfun SGP40 Air Quality Sensor
 *  for VOC Measurements by using I2C serial interface.
 *
 * @param[out] results
 *   @li @ref 0xD400 (SPARKFUN_SGP40_TEST_PASSED) - Tests passed successfully.
 *
 *   @li @ref 0x4B00 (SPARKFUN_SGP40_TEST_FAILED) - Tests have failed.
 *
 * @return
 *  Result of this test:
 *   @li @ref SL_STATUS_OK Success.
 *
 *   @li @ref SL_STATUS_FAIL Failure.
 ******************************************************************************/
sl_status_t sparkfun_sgp40_measure_test(uint16_t *results);

/***************************************************************************//**
 * @brief
 *  Sparkfun SGP40 soft reset function.
 *
 * @details
 *  This function general call resetting all devices connected to the same
 *  I2C bus of Sparkfun SGP40 Air Quality Sensor for VOC Measurements.
 *
 * @return
 *  Result of this test:
 *   @li @ref SL_STATUS_OK Success.
 *
 *   @li @ref SL_STATUS_FAIL Failure.
 ******************************************************************************/
sl_status_t sparkfun_sgp40_soft_reset(void);

/***************************************************************************//**
 * @brief
 *  Sparkfun SGP40 heater off function.
 *
 * @details
 *  This function turns the hotplate off and stops the measurement of the
 *  Sparkfun SGP40 Air Quality Sensor for VOC Measurements.
 *
 * @return
 *  Result of this test:
 *   @li @ref SL_STATUS_OK Success.
 *
 *   @li @ref SL_STATUS_FAIL Failure.
 ******************************************************************************/
sl_status_t sparkfun_sgp40_heater_off(void);

/***************************************************************************//**
 * @brief
 *  Sparkfun SGP40 get air quality data function.
 *
 * @details
 *  This function reads a air quality SRAW data from the
 *  Sparkfun SGP40 Air Quality Sensor for VOC Measurements.
 *
 * @param[out] sraw_ticks
 *  The air quality SRAW data value.
 * @param[in] rh
 *  The Relative Humidity
 * @param[in] t
 *  The Temperature
 *
 * @return
 *  Result of this test:
 *   @li @ref SL_STATUS_OK Success.
 *
 *   @li @ref SL_STATUS_FAIL Failure.
 ******************************************************************************/
sl_status_t sparkfun_sgp40_measure_raw(uint16_t *sraw_ticks, float rh, float t);

/***************************************************************************//**
 * @brief
 *  Sparkfun SGP40 measure VOC index with relative humidity and temperature.
 *
 * @details
 *  This function measure the humidity-compensated VOC Index,
 *  ambient temperature and relative humidity.
 *
 * @param[out] voc_index
 *  The VOC index value.
 * @param[in] rh
 *  Relative Humidity in [milli %RH].
 * @param[in] t
 *  Temperature in [milli degree Celsius].
 * @return
 *  Result of this test:
 *   @li @ref SL_STATUS_OK Success.
 *
 *   @li @ref SL_STATUS_FAIL Failure.
 ******************************************************************************/
sl_status_t sparkfun_sgp40_get_voc_index(int32_t *voc_index, float rh, float t);

/***************************************************************************//**
 * @brief
 *  Sparkfun SGP40 VOC algorithm initialize function.
 *
 * @details
 *  This function Initialize the VOC algorithm parameters.
 *
 * @return
 *  Nothing.
 ******************************************************************************/
void sparkfun_sgp40_voc_algorithm_init(void);

/***************************************************************************//**
 * @brief
 *  Sparkfun SGP40 VOC algorithm process function.
 *
 * @details
 *  This function calculate the VOC index value from the raw sensor value.
 *
 * @param[in] sraw
 *  Air quality SRAW data
 * @param[out] voc_index
 *  Calculated VOC index value from the raw sensor value.
 * @return
 *  Nothing.
 ******************************************************************************/
void sparkfun_sgp40_voc_algorithm_process(int32_t sraw, int32_t *voc_index);

/***************************************************************************//**
 * @brief
 *  This function generates CRC8 .
 *
 * @param[in] data
 *  The data to calculate CRC8
 *
 * @return
 *  The number CRC8.
 ******************************************************************************/
uint8_t sparkfun_sgp40_crc8(uint16_t data);

#ifdef __cplusplus
}
#endif

#endif
