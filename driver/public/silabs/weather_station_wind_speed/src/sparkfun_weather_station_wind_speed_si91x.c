/***************************************************************************//**
 * @file sparkfun_weather_station_wind_speed.c
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
#include "sparkfun_weather_station_wind_speed.h"
#include "sparkfun_weather_station_wind_speed_config.h"

#include "sl_sleeptimer.h"
#include "sl_si91x_driver_gpio.h"

#ifndef HP
#define HP                                   0
#endif

#ifndef ULP
#define ULP                                  4
#endif

#ifndef UULP_VBAT
#define UULP_VBAT                            5
#endif

#define INT_NO                               0 // available interrupt number
#define INT_CH                               0 // GPIO Pin interrupt
#define FILTER_MS                            8

static sl_si91x_gpio_pin_config_t sparkfun_windspeed_cfg = {
  .port_pin = {
    SPARKFUN_WEATHER_STATION_WIND_SPEED_PORT == HP
    ?SPARKFUN_WEATHER_STATION_WIND_SPEED_PIN / 16
    : (SPARKFUN_WEATHER_STATION_WIND_SPEED_PORT == ULP ? 4 : 5),
    SPARKFUN_WEATHER_STATION_WIND_SPEED_PIN % 16
  },
  .direction = GPIO_INPUT
};

static volatile uint32_t sparkfun_windspeed_last_count = 0;
static volatile uint32_t sparkfun_windspeed_pulse_count = 0;

static sl_sleeptimer_timer_handle_t measurement_timer_handle;
static sl_sleeptimer_timer_handle_t filter_timer_handle;

static void measurement_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                                       void *data);
static void filter_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                                  void *data);
static void gpio_int_callback(uint32_t intNo);

/************************************************************************
 *    Init function
 *****************************************************************************/
void sparkfun_weatherstation_windspeed_init(void)
{
  sl_gpio_set_configuration(sparkfun_windspeed_cfg);
  sl_gpio_driver_configure_interrupt(&sparkfun_windspeed_cfg.port_pin,
                                     INT_CH,
                                     SL_GPIO_INTERRUPT_FALLING_EDGE,
                                     (void *)&gpio_int_callback,
                                     INT_NO);

  sl_sleeptimer_start_periodic_timer_ms(&measurement_timer_handle,
                                        SPARKFUN_WINDSPEED_CHECK_PERIOD_MS,
                                        measurement_timer_callback,
                                        (void *)NULL, 0, 0);
}

/************************************************************************
 *    Get the latest windspeed
 *****************************************************************************/
sl_status_t sparkfun_weatherstation_windspeed_get(float *wind_speed)
{
  if (wind_speed == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  *wind_speed = 2.4 * sparkfun_windspeed_last_count * 1000
                / SPARKFUN_WINDSPEED_CHECK_PERIOD_MS;

  return SL_STATUS_OK;
}

/************************************************************************
 *    Callback to update the wind speed periodically
 *****************************************************************************/
static void measurement_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                                       void *data)
{
  (void) data;
  (void) handle;

  sparkfun_windspeed_last_count = sparkfun_windspeed_pulse_count;
  sparkfun_windspeed_pulse_count = 0;
}

/************************************************************************
 *    Callback to filter
 *****************************************************************************/
static void filter_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                                  void *data)
{
  (void) data;
  (void) handle;

  if (!sl_gpio_get_pin_input(sparkfun_windspeed_cfg.port_pin.port,
                             sparkfun_windspeed_cfg.port_pin.pin)) {
    // Increment counter
    sparkfun_windspeed_pulse_count++;
  }
}

/************************************************************************
 *    Callback for pulse event
 *****************************************************************************/
static void gpio_int_callback(uint32_t intNo)
{
  (void) intNo;

  sl_sleeptimer_start_timer_ms(&filter_timer_handle,
                               FILTER_MS,
                               filter_timer_callback,
                               (void *)NULL, 0, 0);
}
