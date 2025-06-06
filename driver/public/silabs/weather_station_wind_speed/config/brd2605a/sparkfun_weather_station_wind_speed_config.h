/***************************************************************************//**
 * @file sparkfun_weather_station_wind_speed_config.h
 * @brief Sparkfun Weather Station Wind Speed Driver Configuration
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
#ifndef SPARKFUN_WEATHER_STATION_WIND_SPEED_CONFIG_H
#define SPARKFUN_WEATHER_STATION_WIND_SPEED_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <o SPARKFUN_WINDSPEED_CHECK_PERIOD_MS> Windspeed check period (ms)
// <0-4294967295:1>
// <i> Default: 1000 [0-4294967295]
#define SPARKFUN_WINDSPEED_CHECK_PERIOD_MS            1000

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <gpio> SPARKFUN_WEATHER_STATION_WIND_SPEED
// $[GPIO_SPARKFUN_WEATHER_STATION_WIND_SPEED]
#ifndef SPARKFUN_WEATHER_STATION_WIND_SPEED_PORT
#define SPARKFUN_WEATHER_STATION_WIND_SPEED_PORT HP
#endif
#ifndef SPARKFUN_WEATHER_STATION_WIND_SPEED_PIN 
#define SPARKFUN_WEATHER_STATION_WIND_SPEED_PIN  11
#endif
// [GPIO_SPARKFUN_WEATHER_STATION_WIND_SPEED]$

// <<< sl:end pin_tool >>>

#endif // SPARKFUN_WEATHER_STATION_WIND_SPEED_CONFIG_H
