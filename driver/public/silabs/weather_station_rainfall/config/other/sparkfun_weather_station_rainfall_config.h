/***************************************************************************//**
 * @file sparkfun_weather_station_rainfall_config.h
 * @brief Sparkfun Weather Station Rainfall Driver Configuration
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

#ifndef SPARKFUN_WEATHER_STATION_RAINFALL_CONFIG_H
#define SPARKFUN_WEATHER_STATION_RAINFALL_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <o SPARKFUN_WINDSPEED_CHECK_PERIOD_MS> Windspeed check period (ms)
// <0-4294967295:1>
// <i> Default: 1000 [0-4294967295]
#define SPARKFUN_WINDSPEED_CHECK_PERIOD_MS                 1000

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio> SPARKFUN_WEATHER_STATION_RAINFALL_INT
// $[GPIO_SPARKFUN_WEATHER_STATION_RAINFALL_INT]
#warning \
  "SPARKFUN_WEATHER_STATION_RAINFALL_INT pin for rain detection is not configured"
// [GPIO_SPARKFUN_WEATHER_STATION_RAINFALL_INT]$

// <<< sl:end pin_tool >>>

#endif // SPARKFUN_WEATHER_STATION_RAINFALL_CONFIG_H
