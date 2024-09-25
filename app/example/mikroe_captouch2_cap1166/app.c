/***************************************************************************//**
 * @file
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
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/

#include "sl_status.h"
#include "app_assert.h"
#include "sl_sleeptimer.h"
#include "mikroe_cap1166.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_si91x_gspi.h"
#else
#include "app_log.h"
#include "sl_spidrv_instances.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#if (defined(SLI_SI917))
static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#endif

static mikroe_spi_handle_t app_spi_instance = NULL;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t ret_code;

#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

  ret_code = mikroe_cap1166_init(app_spi_instance);
  app_assert_status(ret_code);

  mikroe_cap1166_reset();

  sl_sleeptimer_delay_millisecond(200);

  mikroe_cap1166_default_cfg();
  sl_sleeptimer_delay_millisecond(100);

  app_printf("\r\n>> Cap Touch 2 is initialized\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint8_t cnt;
  uint8_t sensor_results[6];

  mikroe_cap1166_detect_touch(sensor_results);
  for (cnt = 0; cnt < 6; cnt++ ) {
    if (sensor_results[cnt] == 1) {
      app_printf("\r\nInput %d is touched\r\n", cnt + 1);
    } else if (sensor_results[cnt] == 2) {
      app_printf("\r\nInput %d is released\r\n", cnt + 1);
    }
  }
}
