/***************************************************************************//**
 * @file sparkfun_weather_station_wind_direction_config.h
 * @brief Sparkfun Weather Station Wind Direction Driver Configuration
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef SPARKFUN_WEATHER_STATION_WIND_DIRECTION_CONFIG_H
#define SPARKFUN_WEATHER_STATION_WIND_DIRECTION_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>
// <h>ADC Channel Configuration

// <o SL_ADC_CHANNEL_1_INPUT_TYPE> Input Type
//   <SL_ADC_SINGLE_ENDED=>  Single ended
//   <SL_ADC_DIFFERENTIAL=> Differential
// <i> Selection of the ADC input type.
#define SL_ADC_CHANNEL_1_INPUT_TYPE SL_ADC_SINGLE_ENDED

// <o SL_ADC_CHANNEL_1_SAMPLING_RATE> Sampling Rate <1-2500000>
// <i> Default: 100000
#define SL_ADC_CHANNEL_1_SAMPLING_RATE 100000

// <o SL_ADC_CHANNEL_1_SAMPLE_LENGTH> Sample Length <1-1023>
// <i> Default: 1023
#define SL_ADC_CHANNEL_1_SAMPLE_LENGTH 1

// </h>
// <<< end of configuration section >>>

#ifdef SL_ADC_CH1_P_LOC
#define SL_ADC_CHANNEL_1_POS_INPUT_CHNL_SEL SL_ADC_CH1_P_LOC
#else
#define SL_ADC_CHANNEL_1_POS_INPUT_CHNL_SEL 10
#endif
#ifdef SL_ADC_CH1_N_LOC
#define SL_ADC_CHANNEL_1_NEG_INPUT_CHNL_SEL (SL_ADC_CH1_N_LOC - N1_START_LOCATION_PINTOOL)
#else
#define SL_ADC_CHANNEL_1_NEG_INPUT_CHNL_SEL 7
#endif

#endif // SPARKFUN_WEATHER_STATION_WIND_DIRECTION_CONFIG_H
