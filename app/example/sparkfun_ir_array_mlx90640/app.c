/***************************************************************************//**
 * @file app.c
 * @brief Top level application functions
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
 * # Experimental Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#include "sl_sleeptimer.h"
#include "app_assert.h"

#include "sparkfun_mlx90640.h"
#include "sparkfun_mlx90640_config.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED            SL_I2C2

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...) app_log(__VA_ARGS__)
#endif

static mikroe_i2c_handle_t app_i2c_instance = NULL;
static float mlx90640_image[SPARKFUN_MLX90640_NUM_OF_PIXELS];
static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool app_timer_expire = false;

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t sc;
  uint16_t refrate = 0;

#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  sc = sparkfun_mlx90640_init(app_i2c_instance,
                              SPARKFUN_MLX90640_I2C_ADDRESS);

  if (sc == SL_STATUS_OK) {
    app_printf("\nMLX90640 initialized successfully\n");
    sc = sparkfun_mlx90640_set_refresh_rate(0x03);
    app_assert_status(sc);

    sc = sparkfun_mlx90640_get_refresh_rate(&refrate);
    app_assert_status(sc);

    app_printf("RefreshRate: %x\n", refrate);

    sc = sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                               250,
                                               app_timer_cb,
                                               (void *) NULL,
                                               0,
                                               0);
    app_assert_status(sc);
  } else {
    app_printf("\nMLX90640 initialization failed!\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (app_timer_expire == true) {
    app_timer_expire = false;
    sparkfun_mlx90640_get_image_array(mlx90640_image);

    for (int i = 0; i < SPARKFUN_MLX90640_NUM_OF_PIXELS; i++)
    {
      app_printf("%3.2f,", mlx90640_image[i]);
    }
    app_printf("\n");
  }
}

static void app_timer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) handle;
  (void) data;

  app_timer_expire = true;
}
