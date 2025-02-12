/***************************************************************************//**
 * @file sparkfun_weather_station_wind_direction.c
 * @brief Sparkfun Keypad source file.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "sparkfun_weather_station_wind_direction.h"
#include "sparkfun_weather_station_wind_direction_config.h"

#include "sl_si91x_adc.h"

#include "rsi_debug.h"

#define ADC_MAX_OP_VALUE 4095   // Maximum output value get from ADC data register
#define ADC_DATA_CLEAR   0x07FF // Clear the data if 12th bit is enabled
#define VREF_VALUE       3.3    // reference voltage

static sl_adc_channel_config_t adc_channel_cfg = {
  .input_type[SL_ADC_CHANNEL_1] = SL_ADC_CHANNEL_1_INPUT_TYPE,
  .pos_inp_sel[SL_ADC_CHANNEL_1] = SL_ADC_CHANNEL_1_POS_INPUT_CHNL_SEL,
  .neg_inp_sel[SL_ADC_CHANNEL_1] = SL_ADC_CHANNEL_1_NEG_INPUT_CHNL_SEL,
  .num_of_samples[SL_ADC_CHANNEL_1] = SL_ADC_CHANNEL_1_SAMPLE_LENGTH,
  .sampling_rate[SL_ADC_CHANNEL_1] = SL_ADC_CHANNEL_1_SAMPLING_RATE,
};

static sl_adc_config_t adc_cfg = {
  .num_of_channel_enable = 1,
  .operation_mode = SL_ADC_STATIC_MODE,
};

static volatile boolean_t data_sample_complete_flag = false;

typedef enum {
  WIND_DIRECTION_ANGLE_0 = 0,
  WIND_DIRECTION_ANGLE_22_5,
  WIND_DIRECTION_ANGLE_45,
  WIND_DIRECTION_ANGLE_67_5,
  WIND_DIRECTION_ANGLE_90,
  WIND_DIRECTION_ANGLE_112_5,
  WIND_DIRECTION_ANGLE_135,
  WIND_DIRECTION_ANGLE_157_5,
  WIND_DIRECTION_ANGLE_180,
  WIND_DIRECTION_ANGLE_202_5,
  WIND_DIRECTION_ANGLE_225,
  WIND_DIRECTION_ANGLE_247_5,
  WIND_DIRECTION_ANGLE_270,
  WIND_DIRECTION_ANGLE_292_5,
  WIND_DIRECTION_ANGLE_315,
  WIND_DIRECTION_ANGLE_337_5,
  WIND_DIRECTION_ANGLES_NUM
} windirection_index_t;

static uint32_t windirection[] =
{
  3160,
  3240,
  2135,
  2275,
  1725,
  1930,
  1495,
  1600,
  1565,
  2795,
  2640,
  3605,
  3415,
  3820,
  3695,
  3940
};

static sl_status_t winddirection_read(uint16_t *adc_value);
static void callback_event(uint8_t channel_no, uint8_t event);

/***************************************************************************//**
 * @brief This function initializes all necessary peripherals used
 ******************************************************************************/
sl_status_t sparkfun_weatherstation_winddirection_init(void)
{
  sl_status_t sc = SL_STATUS_OK;

  sc = sl_si91x_adc_init(adc_channel_cfg, adc_cfg, VREF_VALUE);

  /* Due to calling trim_efuse API on ADC init in driver it will change the
   *  clock frequency, if we don't initialize the debug again it will print
   *  the garbage data in console output. */
  DEBUGINIT();
  if (sc != SL_STATUS_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  sc = sl_si91x_adc_set_channel_configuration(adc_channel_cfg, adc_cfg);
  if (sc != SL_STATUS_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  sc = sl_si91x_adc_register_event_callback(callback_event);
  if (sc != SL_STATUS_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return sc;
}

/***************************************************************************//**
 * @brief Gets value of wind direction in degree
 ******************************************************************************/
sl_status_t sparkfun_weatherstation_winddirection_read_direction(
  float *wind_direction_degrees)
{
  uint16_t adc_value;
  uint16_t diff;
  uint16_t closest_diff = 0xFFF;
  windirection_index_t index = WIND_DIRECTION_ANGLE_0;

  if (wind_direction_degrees == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  if (winddirection_read(&adc_value) != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }

  for (uint8_t i = 0; i < SPARKFUN_WIND_DIRECTION_COUNT; i++) {
    if (windirection[i] > adc_value) {
      diff = windirection[i] - adc_value;
    } else {
      diff = adc_value - windirection[i];
    }
    if (diff < closest_diff) {
      closest_diff = diff;
      index = i;
    }
  }
  *wind_direction_degrees = SPARKFUN_WIND_DIRECTION_INCREMENTS * index;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Get ADC value
 ******************************************************************************/
static sl_status_t winddirection_read(uint16_t *adc_value)
{
  uint16_t data;
  if (sl_si91x_adc_start(adc_cfg) != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }

  // Wait until ADC data sample done
  while (data_sample_complete_flag != true) {}
  data_sample_complete_flag = false;
  if (sl_si91x_adc_stop(adc_cfg) != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  if (sl_si91x_adc_read_data_static(adc_channel_cfg,
                                    adc_cfg,
                                    &data) != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }

  if (data & SIGN_BIT) {
    *adc_value = (int16_t)(data & (ADC_DATA_CLEAR));
  } else {
    *adc_value = data | SIGN_BIT;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * Callback event function
 ******************************************************************************/
static void callback_event(uint8_t channel_no, uint8_t event)
{
  if (event == SL_INTERNAL_DMA) {
    if (channel_no == SL_ADC_CHANNEL_1) {
      data_sample_complete_flag = true;
    }
  } else if (event == SL_ADC_STATIC_MODE_EVENT) {
    data_sample_complete_flag = true;
  }
}
