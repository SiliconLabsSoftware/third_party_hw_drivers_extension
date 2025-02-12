/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "app.h"

#include "sparkfun_weather_station_rainfall.h"
#include "sparkfun_weather_station_wind_direction.h"
#include "sparkfun_weather_station_wind_speed.h"

#include "sl_sleeptimer.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#else
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#define APP_MEASUREMENT_PERIOD_MS 500

static volatile bool measurement_trigger = false;
static sl_sleeptimer_timer_handle_t app_timer_handle;

static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                               void *data);
static void weather_measurement();

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc = SL_STATUS_OK;

  app_printf("Sparkfun Weather Station, example application\n");

  sparkfun_weatherstation_rainfall_init();
  sparkfun_weatherstation_windspeed_init();
  sc = sparkfun_weatherstation_winddirection_init();

  if (sc != SL_STATUS_OK) {
    app_printf("Initializing Error. Please check again ...\n");
    return;
  }

  sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                        APP_MEASUREMENT_PERIOD_MS,
                                        app_timer_callback,
                                        (void *)NULL, 2, 0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (measurement_trigger) {
    measurement_trigger = false;
    weather_measurement();
  }
}

/***************************************************************************//**
 * Static function implementation.
 ******************************************************************************/
static void app_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                               void *data)
{
  (void) handle;
  (void) data;

  measurement_trigger = true;
}

static void weather_measurement()
{
  float rainfall;
  float winddirection;
  float windspeed;

  sl_status_t sc = SL_STATUS_OK;

  sparkfun_weatherstation_rainfall_get_rainfall_amount(&rainfall);
  sparkfun_weatherstation_winddirection_read_direction(&winddirection);
  sc = sparkfun_weatherstation_windspeed_get(&windspeed);
  if (sc != SL_STATUS_OK) {
    app_printf("Error reading wind speed sensor\n");
  }
  app_printf("\r\nRain : %.2f mm\n", rainfall);
  app_printf("Wind Direction : %.2f" "\xB0" "\n", winddirection);
  app_printf("Wind Speed : %.2f km/h\n", windspeed);
}
