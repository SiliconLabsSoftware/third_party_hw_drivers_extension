/***************************************************************************//**
 * @file sparkfun_weather_station_wind_direction_config.h
 * @brief Sparkfun Weather Station Wind Direction Driver Configuration
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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
