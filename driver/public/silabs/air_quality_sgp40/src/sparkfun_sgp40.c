/***************************************************************************//**
 * @file sparkfun_sgp40.c
 * @brief sparkfun_sgp40 source file.
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
#include "sparkfun_sgp40.h"
#include "sl_sleeptimer.h"
#include "sparkfun_sgp40_config.h"

typedef i2c_master_t sgp40_handle_t;

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------
static uint8_t i2c_is_initialized = false;

/// @brief Specified SGP40 commands of Air Quality Sensor driver.
static uint8_t sgp40_measure_raw[2] = { 0x26, 0x0F };
static uint8_t sgp40_measure_test[2] = { 0x28, 0x0E };
static uint8_t sgp40_heater_off[2] = { 0x36, 0x15 };
static uint8_t sgp40_soft_reset[2] = { 0x00, 0x06 };

/// @brief  Storage for the Sensirion VOC Algorithm parameters
static voc_algorithm_params_t params;
static voc_algorithm_params_t *voc_algorithm_parameters = &params;

// -----------------------------------------------------------------------------
//                       Local Function declarations
// -----------------------------------------------------------------------------
static fix16_t voc_algorithm_mox_model_process(fix16_t sraw);

static fix16_t voc_algorithm_sigmoid_scaled_process(fix16_t sample);

static fix16_t voc_algorithm_adaptive_lowpass_process(fix16_t sample);

static fix16_t voc_algorithm_mean_variance_estimator_sigmoid_process(
  fix16_t sample);

static void voc_algorithm_mean_variance_estimator_calculate_gamma(
  fix16_t voc_index_from_prior);

static void voc_algorithm_mean_variance_estimator_process(fix16_t sraw,
                                                          fix16_t voc_index_from_prior);

static void voc_algorithm_mox_model_set_parameters(fix16_t sraw_std,
                                                   fix16_t sraw_mean);

static void voc_algorithm_mean_variance_estimator_set_parameters(
  fix16_t std_initial,
  fix16_t tau_mean_variance_hours,
  fix16_t gating_max_duration_minutes);

static void voc_algorithm_mean_variance_estimator_sigmoid_set_parameters(
  fix16_t l_val,
  fix16_t x0_val,
  fix16_t k_val);

static void voc_algorithm_mean_variance_estimator_sigmoid_init(void);

static void voc_algorithm_mean_variance_estimator_init_instances(void);

static void voc_algorithm_mean_variance_estimator_init(void);

static void voc_algorithm_mox_model_init(void);

static fix16_t voc_algorithm_mean_variance_estimator_get_std(void);

static fix16_t voc_algorithm_mean_variance_estimator_get_mean(void);

static void voc_algorithm_sigmoid_scaled_set_parameters(fix16_t offset);

static void voc_algorithm_sigmoid_scaled_init(void);

static void voc_algorithm_adaptive_lowpass_set_parameters(void);

static void voc_algorithm_adaptive_lowpass_init(void);

static void voc_algorithm_init_instances(void);

static fix16_t fix16_from_int(int32_t a);

static int32_t fix16_cast_to_int(fix16_t a);

static fix16_t fix16_mul(fix16_t a, fix16_t b);

static fix16_t fix16_div(fix16_t a, fix16_t b);

static fix16_t fix16_sqrt(fix16_t x);

static fix16_t fix16_exp(fix16_t x);

static sgp40_handle_t sgp40_handle;

static sl_status_t i2c_read_blocking(sgp40_handle_t *handle,
                                     uint8_t *dest,
                                     uint32_t len);
static sl_status_t i2c_write_blocking(sgp40_handle_t *handle,
                                      uint8_t *src,
                                      uint32_t len);

// -----------------------------------------------------------------------------
//                       Global Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 *  This function Initialize the sensor I2C instance.
 *****************************************************************************/
sl_status_t sparkfun_sgp40_init(mikroe_i2c_handle_t i2cspm_instance)
{
  i2c_master_config_t i2c_cfg;

  if (i2cspm_instance == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  // If already initialized, return status
  if (i2c_is_initialized == true) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  sgp40_handle.handle = i2cspm_instance;

  i2c_master_configure_default(&i2c_cfg);
  i2c_cfg.addr = SGP40_ADDRESS;
  i2c_cfg.timeout_pass_count = 0;

#if (SPARKFUN_SGP40_I2C_UC == 1)
  i2c_cfg.speed = SPARKFUN_SGP40_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&sgp40_handle, &i2c_cfg) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  i2c_is_initialized = true;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  This function triggers the built-in self-test checking of the sensor.
 *****************************************************************************/
sl_status_t sparkfun_sgp40_measure_test(uint16_t *results)
{
  uint8_t data_to_read[3];
  uint8_t check_sum;
  sl_status_t sc;

  sc = i2c_write_blocking(&sgp40_handle, sgp40_measure_test, 2);
  if (sc != SL_STATUS_OK) {
    return sc;
  }
  sl_sleeptimer_delay_millisecond(320);

  sc = i2c_read_blocking(&sgp40_handle, data_to_read, 3);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  *results = ((uint16_t)data_to_read[0]) << 8;
  *results |= data_to_read[1];
  check_sum = data_to_read[2];
  if (check_sum != sparkfun_sgp40_crc8(*results)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  This function general call resetting for the sensor.
 *****************************************************************************/
sl_status_t sparkfun_sgp40_soft_reset(void)
{
  sl_status_t sc;

  sc = i2c_write_blocking(&sgp40_handle, sgp40_soft_reset, 2);
  if (sc != SL_STATUS_OK) {
    return sc;
  }
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  This function turns the hotplate off and stops the measurement
 *****************************************************************************/
sl_status_t sparkfun_sgp40_heater_off(void)
{
  sl_status_t sc;

  sc = i2c_write_blocking(&sgp40_handle, sgp40_heater_off, 2);
  sl_sleeptimer_delay_millisecond(1);

  if (sc != SL_STATUS_OK) {
    return sc;
  }
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  This function reads a air quality SRAW data.
 *****************************************************************************/
sl_status_t sparkfun_sgp40_measure_raw(uint16_t *sraw_ticks, float rh, float t)
{
  uint16_t rh_ticks, t_ticks;
  uint8_t data_to_write[8];
  uint8_t data_to_read[3];
  uint16_t results;
  uint8_t check_sum;
  sl_status_t sc;

  if (rh < 0) {
    rh = 0;
  }

  if (rh > 100) {
    rh = 100;
  }

  if (t < -45) {
    t = -45;
  }

  if (t > 130) {
    t = 130;
  }

  rh_ticks = (uint16_t)(rh * 65535 / 100);
  t_ticks = (uint16_t)((t + 45) * 65535 / 175);

  // Perform a raw measurement
  sc = i2c_write_blocking(&sgp40_handle, sgp40_measure_raw, 2);

  data_to_write[0] = sgp40_measure_raw[0];
  data_to_write[1] = sgp40_measure_raw[1];
  data_to_write[2] = ((uint8_t)(rh_ticks >> 8));
  data_to_write[3] = ((uint8_t)(rh_ticks & 0xFF));
  data_to_write[4] = (sparkfun_sgp40_crc8(rh_ticks));
  data_to_write[5] = ((uint8_t)(t_ticks >> 8));
  data_to_write[6] = ((uint8_t)(t_ticks & 0xFF));
  data_to_write[7] = (sparkfun_sgp40_crc8(t_ticks));

  sc = i2c_write_blocking(&sgp40_handle, data_to_write, 8);
  if (sc != SL_STATUS_OK) {
    return sc;
  }
  sl_sleeptimer_delay_millisecond(30);

  sc = i2c_read_blocking(&sgp40_handle, data_to_read, 3);
  if (sc != SL_STATUS_OK) {
    return sc;
  }

  results = ((uint16_t)data_to_read[0]) << 8;
  results |= data_to_read[1];
  check_sum = data_to_read[2];

  if (check_sum != sparkfun_sgp40_crc8(results)) {
    return SL_STATUS_FAIL;
  }
  *sraw_ticks = results;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  This function measure VOC index with relative humidity and temperature.
 *****************************************************************************/
sl_status_t sparkfun_sgp40_get_voc_index(int32_t *voc_index, float rh, float t)
{
  uint16_t sraw_ticks;
  sl_status_t sc;

  sc = sparkfun_sgp40_measure_raw(&sraw_ticks, rh, t);
  if (sc != SL_STATUS_OK) {
    return sc;
  }
  sl_sleeptimer_delay_millisecond(10);
  sparkfun_sgp40_voc_algorithm_process(sraw_ticks, voc_index);

  return SL_STATUS_OK;
}

#ifndef SGP40_LOOKUP_TABLE

/**************************************************************************//**
 *  This function generates CRC8 if not defined SGP40_LOOKUP_TABLE.
 *****************************************************************************/
uint8_t sparkfun_sgp40_crc8(uint16_t data)
{
  uint8_t crc = 0xFF;

  crc ^= (data >> 8);

  for (uint8_t i = 0 ; i < 8 ; i++)
  {
    if ((crc & 0x80) != 0) {
      crc = (uint8_t)((crc << 1) ^ 0x31);
    } else {
      crc <<= 1;
    }
  }

  crc ^= (uint8_t)data;

  for (uint8_t i = 0 ; i < 8 ; i++)
  {
    if ((crc & 0x80) != 0) {
      crc = (uint8_t)((crc << 1) ^ 0x31);
    } else {
      crc <<= 1;
    }
  }

  return crc;
}

#else

/**************************************************************************//**
 *  This function generates CRC8 if defined SGP40_LOOKUP_TABLE.
 *****************************************************************************/
uint8_t sparkfun_sgp40_crc8(uint16_t data)
{
  uint8_t crc = 0xFF;
  crc ^= (uint8_t)(data >> 8);
  crc = CRC8LookupTable[crc >> 4][crc & 0xF];
  crc ^= (uint8_t)data;
  crc = CRC8LookupTable[crc >> 4][crc & 0xF];
  return crc;
}

#endif

/**************************************************************************//**
 *  This function Initialize the VOC algorithm parameters.
 *****************************************************************************/
void sparkfun_sgp40_voc_algorithm_init()
{
  voc_algorithm_parameters->mVoc_Index_Offset = F16(
    VOC_ALGORITHM_VOC_INDEX_OFFSET_DEFAULT);
  voc_algorithm_parameters->mTau_Mean_Variance_Hours = F16(
    VOC_ALGORITHM_TAU_MEAN_VARIANCE_HOURS);
  voc_algorithm_parameters->mGating_Max_Duration_Minutes = F16(
    VOC_ALGORITHM_GATING_MAX_DURATION_MINUTES);
  voc_algorithm_parameters->mSraw_Std_Initial = F16(
    VOC_ALGORITHM_SRAW_STD_INITIAL);
  voc_algorithm_parameters->mUptime = F16(0.);
  voc_algorithm_parameters->mSraw = F16(0.);
  voc_algorithm_parameters->mVoc_Index = 0;
  voc_algorithm_init_instances(  );
}

/**************************************************************************//**
 *  This function calculate the VOC index value from the raw sensor value.
 *****************************************************************************/
void sparkfun_sgp40_voc_algorithm_process(int32_t sraw, int32_t *voc_index)
{
  if ((voc_algorithm_parameters->mUptime
       <= F16(VOC_ALGORITHM_INITIAL_BLACKOUT))) {
    voc_algorithm_parameters->mUptime =
      (voc_algorithm_parameters->mUptime
       + F16(VOC_ALGORITHM_SAMPLING_INTERVAL));
  } else {
    if (((sraw > 0) && (sraw < 65000))) {
      if ((sraw < 20001)) {
        sraw = 20001;
      } else if ((sraw > 52767)) {
        sraw = 52767;
      }
      voc_algorithm_parameters->mSraw = (fix16_from_int((sraw - 20000)));
    }
    voc_algorithm_parameters->mVoc_Index = voc_algorithm_mox_model_process(
      voc_algorithm_parameters->mSraw);
    voc_algorithm_parameters->mVoc_Index = voc_algorithm_sigmoid_scaled_process(
      voc_algorithm_parameters->mVoc_Index);
    voc_algorithm_parameters->mVoc_Index =
      voc_algorithm_adaptive_lowpass_process(
        voc_algorithm_parameters->mVoc_Index);

    if ((voc_algorithm_parameters->mVoc_Index < F16(0.5))) {
      voc_algorithm_parameters->mVoc_Index = F16(0.5);
    }

    if ((voc_algorithm_parameters->mSraw > F16(0.))) {
      voc_algorithm_mean_variance_estimator_process(
        voc_algorithm_parameters->mSraw,
        voc_algorithm_parameters->mVoc_Index);
      voc_algorithm_mox_model_set_parameters(
        voc_algorithm_mean_variance_estimator_get_std(),
        voc_algorithm_mean_variance_estimator_get_mean(  ));
    }
  }

  *voc_index =
    (fix16_cast_to_int((voc_algorithm_parameters->mVoc_Index + F16(0.5))));
}

// -----------------------------------------------------------------------------
//                       Local Function definitions
// -----------------------------------------------------------------------------

static fix16_t voc_algorithm_mox_model_process(fix16_t sraw)
{
  return fix16_mul((fix16_div((sraw
                               - voc_algorithm_parameters->m_Mox_Model_Sraw_Mean),
                              (-(voc_algorithm_parameters->m_Mox_Model_Sraw_Std
                                 + F16(VOC_ALGORITHM_SRAW_STD_BONUS))))),
                   F16(VOC_ALGORITHM_VOC_INDEX_GAIN));
}

static fix16_t voc_algorithm_sigmoid_scaled_process(fix16_t sample)
{
  fix16_t x;
  fix16_t shift;

  x = fix16_mul(F16(VOC_ALGORITHM_SIGMOID_K),
                (sample - F16(VOC_ALGORITHM_SIGMOID_X0)));
  if ((x < F16(-50.))) {
    return F16(VOC_ALGORITHM_SIGMOID_L);
  } else if ((x > F16(50.))) {
    return F16(0.);
  } else {
    if ((sample >= F16(0.))) {
      shift = fix16_div(F16(VOC_ALGORITHM_SIGMOID_L)
                        - (fix16_mul(F16(5.),
                                     voc_algorithm_parameters->
                                     m_Sigmoid_Scaled_Offset)),
                        F16(4.));
      return ((fix16_div(F16(VOC_ALGORITHM_SIGMOID_L) + shift,
                         F16(1.) + fix16_exp(x))))
             - shift;
    } else {
      return fix16_mul(fix16_div(voc_algorithm_parameters->
                                 m_Sigmoid_Scaled_Offset,
                                 F16(VOC_ALGORITHM_VOC_INDEX_OFFSET_DEFAULT)),
                       fix16_div(F16(VOC_ALGORITHM_SIGMOID_L),
                                 F16(1.) + fix16_exp(x)));
    }
  }
}

static fix16_t voc_algorithm_adaptive_lowpass_process(fix16_t sample)
{
  fix16_t abs_delta;
  fix16_t F1;
  fix16_t tau_a;
  fix16_t a3;

  if ((voc_algorithm_parameters->m_Adaptive_Lowpass_Initialized == false)) {
    voc_algorithm_parameters->m_Adaptive_Lowpass_X1 = sample;
    voc_algorithm_parameters->m_Adaptive_Lowpass_X2 = sample;
    voc_algorithm_parameters->m_Adaptive_Lowpass_X3 = sample;
    voc_algorithm_parameters->m_Adaptive_Lowpass_Initialized = true;
  }
  voc_algorithm_parameters->m_Adaptive_Lowpass_X1 =
    fix16_mul(F16(1.) - voc_algorithm_parameters->m_Adaptive_Lowpass_A1,
              voc_algorithm_parameters->m_Adaptive_Lowpass_X1)
    + fix16_mul(voc_algorithm_parameters->m_Adaptive_Lowpass_A1, sample);
  voc_algorithm_parameters->m_Adaptive_Lowpass_X2 =
    (fix16_mul((F16(1.) - voc_algorithm_parameters->m_Adaptive_Lowpass_A2),
               voc_algorithm_parameters->m_Adaptive_Lowpass_X2))
    + fix16_mul(voc_algorithm_parameters->m_Adaptive_Lowpass_A2, sample);
  abs_delta = voc_algorithm_parameters->m_Adaptive_Lowpass_X1
              - voc_algorithm_parameters->m_Adaptive_Lowpass_X2;
  if ((abs_delta < F16(0.))) {
    abs_delta = (-abs_delta);
  }
  F1 = fix16_exp(fix16_mul(F16(VOC_ALGORITHM_LP_ALPHA), abs_delta));
  tau_a = fix16_mul(F16(VOC_ALGORITHM_LP_TAU_SLOW - VOC_ALGORITHM_LP_TAU_FAST),
                    F1)
          + F16(VOC_ALGORITHM_LP_TAU_FAST);
  a3 = fix16_div(F16(VOC_ALGORITHM_SAMPLING_INTERVAL),
                 F16(VOC_ALGORITHM_SAMPLING_INTERVAL) + tau_a);
  voc_algorithm_parameters->m_Adaptive_Lowpass_X3 =
    fix16_mul(F16(1.) - a3,
              voc_algorithm_parameters->m_Adaptive_Lowpass_X3)
    + fix16_mul(a3, sample);

  return voc_algorithm_parameters->m_Adaptive_Lowpass_X3;
}

static fix16_t voc_algorithm_mean_variance_estimator_sigmoid_process(
  fix16_t sample)
{
  fix16_t x;

  x = fix16_mul(voc_algorithm_parameters->m_Mean_Variance_Estimator_Sigmoid_K,
                sample
                - voc_algorithm_parameters->m_Mean_Variance_Estimator_Sigmoid_X0);
  if ((x < F16(-50.))) {
    return voc_algorithm_parameters->m_Mean_Variance_Estimator_Sigmoid_L;
  } else if ((x > F16(50.))) {
    return F16(0.);
  } else {
    return fix16_div(
      voc_algorithm_parameters->m_Mean_Variance_Estimator_Sigmoid_L,
      F16(1.) + fix16_exp(x));
  }
}

static void voc_algorithm_mean_variance_estimator_calculate_gamma(
  fix16_t voc_index_from_prior)
{
  fix16_t uptime_limit;
  fix16_t sigmoid_gamma_mean;
  fix16_t gamma_mean;
  fix16_t gating_threshold_mean;
  fix16_t sigmoid_gating_mean;
  fix16_t sigmoid_gamma_variance;
  fix16_t gamma_variance;
  fix16_t gating_threshold_variance;
  fix16_t sigmoid_gating_variance;

  uptime_limit =
    F16((VOC_ALGORITHM_MEAN_VARIANCE_ESTIMATOR_FIX16_MAX
         - VOC_ALGORITHM_SAMPLING_INTERVAL));
  if (voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gamma
      < uptime_limit) {
    voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gamma =
      voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gamma
      + F16(VOC_ALGORITHM_SAMPLING_INTERVAL);
  }
  if (voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gating
      < uptime_limit) {
    voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gating =
      voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gating
      + F16(VOC_ALGORITHM_SAMPLING_INTERVAL);
  }
  voc_algorithm_mean_variance_estimator_sigmoid_set_parameters(
    F16(1.),
    F16(VOC_ALGORITHM_INIT_DURATION_MEAN),
    F16(VOC_ALGORITHM_INIT_TRANSITION_MEAN));
  sigmoid_gamma_mean = voc_algorithm_mean_variance_estimator_sigmoid_process(
    voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gamma);
  gamma_mean = voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma
               + fix16_mul(voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma_Initial_Mean
                           - voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma,
                           sigmoid_gamma_mean);
  gating_threshold_mean = F16(VOC_ALGORITHM_GATING_THRESHOLD)
                          + fix16_mul(F16((
                                            VOC_ALGORITHM_GATING_THRESHOLD_INITIAL
                                            - VOC_ALGORITHM_GATING_THRESHOLD)),
                                      voc_algorithm_mean_variance_estimator_sigmoid_process(
                                        voc_algorithm_parameters->
                                        m_Mean_Variance_Estimator_Uptime_Gating));
  voc_algorithm_mean_variance_estimator_sigmoid_set_parameters(F16(
                                                                 1.),
                                                               gating_threshold_mean,
                                                               F16(
                                                                 VOC_ALGORITHM_GATING_THRESHOLD_TRANSITION));
  sigmoid_gating_mean =
    voc_algorithm_mean_variance_estimator_sigmoid_process(voc_index_from_prior);
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma_Mean =
    fix16_mul(sigmoid_gating_mean, gamma_mean);
  voc_algorithm_mean_variance_estimator_sigmoid_set_parameters(F16(1.),
                                                               F16(
                                                                 VOC_ALGORITHM_INIT_DURATION_VARIANCE),
                                                               F16(
                                                                 VOC_ALGORITHM_INIT_TRANSITION_VARIANCE));
  sigmoid_gamma_variance =
    voc_algorithm_mean_variance_estimator_sigmoid_process(
      voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gamma);
  gamma_variance =
    (voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma
     + fix16_mul((voc_algorithm_parameters->
                  m_Mean_Variance_Estimator_Gamma_Initial_Variance
                  - voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma),
                 sigmoid_gamma_variance - sigmoid_gamma_mean));
  gating_threshold_variance =
    F16(VOC_ALGORITHM_GATING_THRESHOLD)
    + fix16_mul(F16((VOC_ALGORITHM_GATING_THRESHOLD_INITIAL
                     - VOC_ALGORITHM_GATING_THRESHOLD)),
                voc_algorithm_mean_variance_estimator_sigmoid_process(
                  voc_algorithm_parameters->
                  m_Mean_Variance_Estimator_Uptime_Gating));
  voc_algorithm_mean_variance_estimator_sigmoid_set_parameters(F16(
                                                                 1.),
                                                               gating_threshold_variance,
                                                               F16(
                                                                 VOC_ALGORITHM_GATING_THRESHOLD_TRANSITION));
  sigmoid_gating_variance =
    voc_algorithm_mean_variance_estimator_sigmoid_process(voc_index_from_prior);
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma_Variance =
    fix16_mul(sigmoid_gating_variance, gamma_variance);
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Gating_Duration_Minutes =
    (voc_algorithm_parameters->m_Mean_Variance_Estimator_Gating_Duration_Minutes
     + fix16_mul(F16((VOC_ALGORITHM_SAMPLING_INTERVAL / 60.)),
                 ((fix16_mul((F16(1.) - sigmoid_gating_mean),
                             F16((1. + VOC_ALGORITHM_GATING_MAX_RATIO))))
                  - F16(VOC_ALGORITHM_GATING_MAX_RATIO))));
  if (voc_algorithm_parameters->
      m_Mean_Variance_Estimator_Gating_Duration_Minutes < F16(0.)) {
    voc_algorithm_parameters->m_Mean_Variance_Estimator_Gating_Duration_Minutes
      = F16(0.);
  }
  if (voc_algorithm_parameters->
      m_Mean_Variance_Estimator_Gating_Duration_Minutes
      > voc_algorithm_parameters->
      m_Mean_Variance_Estimator_Gating_Max_Duration_Minutes) {
    voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gating = F16(0.);
  }
}

static void voc_algorithm_mean_variance_estimator_process(fix16_t sraw,
                                                          fix16_t voc_index_from_prior)
{
  fix16_t delta_sgp;
  fix16_t c;
  fix16_t additional_scaling;

  if ((voc_algorithm_parameters->m_Mean_Variance_Estimator_Initialized
       == false)) {
    voc_algorithm_parameters->m_Mean_Variance_Estimator_Initialized = true;
    voc_algorithm_parameters->m_Mean_Variance_Estimator_Sraw_Offset = sraw;
    voc_algorithm_parameters->m_Mean_Variance_Estimator_Mean = F16(0.);
  } else {
    if ((voc_algorithm_parameters->m_Mean_Variance_Estimator_Mean
         >= F16(100.))
        || (voc_algorithm_parameters->m_Mean_Variance_Estimator_Mean
            <= F16(-100.))) {
      voc_algorithm_parameters->m_Mean_Variance_Estimator_Sraw_Offset =
        (voc_algorithm_parameters->m_Mean_Variance_Estimator_Sraw_Offset
         + voc_algorithm_parameters->m_Mean_Variance_Estimator_Mean);
      voc_algorithm_parameters->m_Mean_Variance_Estimator_Mean = F16(0.);
    }
    sraw =
      (sraw - voc_algorithm_parameters->m_Mean_Variance_Estimator_Sraw_Offset);
    voc_algorithm_mean_variance_estimator_calculate_gamma(voc_index_from_prior);
    delta_sgp =
      fix16_div(sraw - voc_algorithm_parameters->m_Mean_Variance_Estimator_Mean,
                F16(VOC_ALGORITHM_MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING));
    if (delta_sgp < F16(0.)) {
      c = voc_algorithm_parameters->m_Mean_Variance_Estimator_Std - delta_sgp;
    } else {
      c = voc_algorithm_parameters->m_Mean_Variance_Estimator_Std + delta_sgp;
    }
    additional_scaling = F16(1.);
    if (c > F16(1440.)) {
      additional_scaling = F16(4.);
    }
    voc_algorithm_parameters->m_Mean_Variance_Estimator_Std =
      fix16_mul(fix16_sqrt(fix16_mul(additional_scaling,
                                     F16(
                                       VOC_ALGORITHM_MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING)
                                     - voc_algorithm_parameters->
                                     m_Mean_Variance_Estimator_Gamma_Variance)),
                fix16_sqrt(fix16_mul(voc_algorithm_parameters->
                                     m_Mean_Variance_Estimator_Std,
                                     fix16_div(voc_algorithm_parameters->
                                               m_Mean_Variance_Estimator_Std,
                                               fix16_mul(F16(
                                                           VOC_ALGORITHM_MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING),
                                                         additional_scaling))
                                     + fix16_mul(fix16_div(fix16_mul(
                                                             voc_algorithm_parameters
                                                             ->
                                                             m_Mean_Variance_Estimator_Gamma_Variance,
                                                             delta_sgp),
                                                           additional_scaling),
                                                 delta_sgp))));
    voc_algorithm_parameters->m_Mean_Variance_Estimator_Mean =
      (voc_algorithm_parameters->m_Mean_Variance_Estimator_Mean
       + fix16_mul(voc_algorithm_parameters->
                   m_Mean_Variance_Estimator_Gamma_Mean,
                   delta_sgp));
  }
}

static void voc_algorithm_mox_model_set_parameters(fix16_t SRAW_STD,
                                                   fix16_t SRAW_MEAN)
{
  voc_algorithm_parameters->m_Mox_Model_Sraw_Std = SRAW_STD;
  voc_algorithm_parameters->m_Mox_Model_Sraw_Mean = SRAW_MEAN;
}

static void voc_algorithm_mean_variance_estimator_set_parameters(
  fix16_t std_initial,
  fix16_t tau_mean_variance_hours,
  fix16_t gating_max_duration_minutes)
{
  voc_algorithm_parameters->
  m_Mean_Variance_Estimator_Gating_Max_Duration_Minutes =
    gating_max_duration_minutes;
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Initialized = false;
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Mean = F16(0.);
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Sraw_Offset = F16(0.);
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Std = std_initial;
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma =
    fix16_div(F16((VOC_ALGORITHM_MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING
                   * (VOC_ALGORITHM_SAMPLING_INTERVAL / 3600.))),
              tau_mean_variance_hours
              + F16((VOC_ALGORITHM_SAMPLING_INTERVAL / 3600.)));
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma_Initial_Mean =
    F16(((VOC_ALGORITHM_MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING
          * VOC_ALGORITHM_SAMPLING_INTERVAL)
         / (VOC_ALGORITHM_TAU_INITIAL_MEAN + VOC_ALGORITHM_SAMPLING_INTERVAL)));
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma_Initial_Variance =
    F16(((VOC_ALGORITHM_MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING
          * VOC_ALGORITHM_SAMPLING_INTERVAL)
         / (VOC_ALGORITHM_TAU_INITIAL_VARIANCE
            + VOC_ALGORITHM_SAMPLING_INTERVAL)));
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma_Mean = F16(0.);
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Gamma_Variance = F16(0.);
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gamma = F16(0.);
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Uptime_Gating = F16(0.);
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Gating_Duration_Minutes =
    F16(0.);
}

static void voc_algorithm_mean_variance_estimator_sigmoid_set_parameters(
  fix16_t L,
  fix16_t X0,
  fix16_t K)
{
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Sigmoid_L = L;
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Sigmoid_K = K;
  voc_algorithm_parameters->m_Mean_Variance_Estimator_Sigmoid_X0 = X0;
}

static void voc_algorithm_mean_variance_estimator_sigmoid_init(void)
{
  voc_algorithm_mean_variance_estimator_sigmoid_set_parameters(F16(0.),
                                                               F16(0.),
                                                               F16(0.));
}

static void voc_algorithm_mean_variance_estimator_init_instances(void)
{
  voc_algorithm_mean_variance_estimator_sigmoid_init();
}

static void voc_algorithm_mean_variance_estimator_init(void)
{
  voc_algorithm_mean_variance_estimator_set_parameters(F16(0.),
                                                       F16(0.),
                                                       F16(0.));
  voc_algorithm_mean_variance_estimator_init_instances();
}

static void voc_algorithm_mox_model_init(void)
{
  voc_algorithm_mox_model_set_parameters(F16(1.), F16(0.));
}

static fix16_t voc_algorithm_mean_variance_estimator_get_std(void)
{
  return voc_algorithm_parameters->m_Mean_Variance_Estimator_Std;
}

static fix16_t voc_algorithm_mean_variance_estimator_get_mean(void)
{
  return (voc_algorithm_parameters->m_Mean_Variance_Estimator_Mean
          + voc_algorithm_parameters->m_Mean_Variance_Estimator_Sraw_Offset);
}

static void voc_algorithm_sigmoid_scaled_set_parameters(fix16_t offset)
{
  voc_algorithm_parameters->m_Sigmoid_Scaled_Offset = offset;
}

static void voc_algorithm_sigmoid_scaled_init(void)
{
  voc_algorithm_sigmoid_scaled_set_parameters(F16(0.));
}

static void voc_algorithm_adaptive_lowpass_set_parameters(void)
{
  voc_algorithm_parameters->m_Adaptive_Lowpass_A1 =
    F16((VOC_ALGORITHM_SAMPLING_INTERVAL
         / (VOC_ALGORITHM_LP_TAU_FAST + VOC_ALGORITHM_SAMPLING_INTERVAL)));
  voc_algorithm_parameters->m_Adaptive_Lowpass_A2 =
    F16((VOC_ALGORITHM_SAMPLING_INTERVAL
         / (VOC_ALGORITHM_LP_TAU_SLOW + VOC_ALGORITHM_SAMPLING_INTERVAL)));
  voc_algorithm_parameters->m_Adaptive_Lowpass_Initialized = false;
}

static void voc_algorithm_adaptive_lowpass_init(void)
{
  voc_algorithm_adaptive_lowpass_set_parameters();
}

static void voc_algorithm_init_instances(void)
{
  voc_algorithm_mean_variance_estimator_init();
  voc_algorithm_mean_variance_estimator_set_parameters(
    voc_algorithm_parameters->mSraw_Std_Initial,
    voc_algorithm_parameters->mTau_Mean_Variance_Hours,
    voc_algorithm_parameters->mGating_Max_Duration_Minutes);
  voc_algorithm_mox_model_init();
  voc_algorithm_mox_model_set_parameters(
    voc_algorithm_mean_variance_estimator_get_std(),
    voc_algorithm_mean_variance_estimator_get_mean());
  voc_algorithm_sigmoid_scaled_init();
  voc_algorithm_sigmoid_scaled_set_parameters(
    voc_algorithm_parameters->mVoc_Index_Offset);
  voc_algorithm_adaptive_lowpass_init();
  voc_algorithm_adaptive_lowpass_set_parameters();
}

static fix16_t fix16_from_int(int32_t a)
{
  return a * FIX16_ONE;
}

static int32_t fix16_cast_to_int(fix16_t a)
{
  return (a >> 16);
}

static fix16_t fix16_mul(fix16_t a, fix16_t b)
{
  int32_t A = (a >> 16), C = (b >> 16);
  uint32_t B = (a & 0xFFFF), D = (b & 0xFFFF);

  int32_t AC = A * C;
  int32_t AD_CB = A * D + C * B;
  uint32_t BD = B * D;

  int32_t product_hi = AC + (AD_CB >> 16);

  uint32_t ad_cb_temp = AD_CB << 16;
  uint32_t product_lo = BD + ad_cb_temp;
  if (product_lo < BD) {
    product_hi++;
  }

#ifndef FIXMATH_NO_OVERFLOW
  if (product_hi >> 31 != product_hi >> 15) {
    return FIX16_OVERFLOW;
  }
#endif

#ifdef FIXMATH_NO_ROUNDING
  return (product_hi << 16) | (product_lo >> 16);
#else
  uint32_t product_lo_tmp = product_lo;
  product_lo -= 0x8000;
  product_lo -= (uint32_t)product_hi >> 31;
  if (product_lo > product_lo_tmp) {
    product_hi--;
  }
  fix16_t result = (product_hi << 16) | (product_lo >> 16);
  result += 1;
  return result;
#endif
}

static fix16_t fix16_div(fix16_t a, fix16_t b)
{
  if (b == 0) {
    return FIX16_MINIMUM;
  }

  uint32_t remainder = (a >= 0) ? a : (-a);
  uint32_t divider = (b >= 0) ? b : (-b);

  uint32_t quotient = 0;
  uint32_t bit = 0x10000;

  while (divider < remainder) {
    divider <<= 1;
    bit <<= 1;
  }

#ifndef FIXMATH_NO_OVERFLOW
  if (!bit) {
    return FIX16_OVERFLOW;
  }
#endif

  if (divider & 0x80000000) {
    if (remainder >= divider) {
      quotient |= bit;
      remainder -= divider;
    }
    divider >>= 1;
    bit >>= 1;
  }

  while (bit && remainder) {
    if (remainder >= divider) {
      quotient |= bit;
      remainder -= divider;
    }
    remainder <<= 1;
    bit >>= 1;
  }

#ifndef FIXMATH_NO_ROUNDING
  if (remainder >= divider) {
    quotient++;
  }
#endif

  fix16_t result = quotient;

  if ((a ^ b) & 0x80000000) {
#ifndef FIXMATH_NO_OVERFLOW
    if (result == (fix16_t)FIX16_MINIMUM) {
      return FIX16_OVERFLOW;
    }
#endif
    result = -result;
  }
  return result;
}

static fix16_t fix16_sqrt(fix16_t x)
{
  uint32_t num = x;
  uint32_t result = 0;
  uint32_t bit;
  uint8_t n;

  bit = (uint32_t)1 << 30;
  while (bit > num) {
    bit >>= 2;
  }

  for (n = 0; n < 2; n++) {
    while (bit) {
      if (num >= result + bit) {
        num -= result + bit;
        result = (result >> 1) + bit;
      } else {
        result = (result >> 1);
      }
      bit >>= 2;
    }

    if (n == 0) {
      if (num > 65535) {
        num -= result;
        num = (num << 16) - 0x8000;
        result = (result << 16) + 0x8000;
      } else {
        num <<= 16;
        result <<= 16;
      }
      bit = 1 << 14;
    }
  }

#ifndef FIXMATH_NO_ROUNDING
  if (num > result) {
    result++;
  }
#endif
  return (fix16_t)result;
}

static fix16_t fix16_exp(fix16_t x)
{
#define NUM_EXP_VALUES 4
  static const fix16_t exp_pos_values[NUM_EXP_VALUES] = {
    F16(2.7182818), F16(1.1331485), F16(1.0157477), F16(1.0019550)
  };
  static const fix16_t exp_neg_values[NUM_EXP_VALUES] = {
    F16(0.3678794), F16(0.8824969), F16(0.9844964), F16(0.9980488)
  };
  const fix16_t *exp_values;

  fix16_t res, arg;
  uint16_t i;

  if (x >= F16(10.3972)) {
    return FIX16_MAXIMUM;
  }
  if (x <= F16(-11.7835)) {
    return 0;
  }

  if (x < 0) {
    x = -x;
    exp_values = exp_neg_values;
  } else {
    exp_values = exp_pos_values;
  }

  res = FIX16_ONE;
  arg = FIX16_ONE;
  for (i = 0; i < NUM_EXP_VALUES; i++) {
    while (x >= arg) {
      res = fix16_mul(res, exp_values[i]);
      x -= arg;
    }
    arg >>= 3;
  }
  return res;
}

static sl_status_t i2c_read_blocking(sgp40_handle_t *handle,
                                     uint8_t *dest,
                                     uint32_t len)
{
  if (I2C_MASTER_SUCCESS != i2c_master_read(handle,
                                            dest,
                                            len)) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

static sl_status_t i2c_write_blocking(sgp40_handle_t *handle,
                                      uint8_t *src,
                                      uint32_t len)
{
  if (I2C_MASTER_SUCCESS != i2c_master_write(handle,
                                             src,
                                             len)) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}
