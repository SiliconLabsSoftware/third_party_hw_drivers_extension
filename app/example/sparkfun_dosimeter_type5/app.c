/***************************************************************************//**
 * @file app.c
 * @brief Example application
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
#include "sparkfun_type5.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"

#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else
#include "app_log.h"

#define app_printf(...) app_log(__VA_ARGS__)
#endif

static void noise_callback(void);
static void radiation_callback(void);

void app_init(void)
{
  sparkfun_type5_init();
  sparkfun_type5_register_noise_callback(noise_callback);
  sparkfun_type5_register_radiation_callback(radiation_callback);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sparkfun_type5_process();
}

/**************************************************************************//**
 *  Callback function for radiation detection.
 *****************************************************************************/
void radiation_callback(void)
{
  app_printf("A wild gamma ray appeared\r\n");
  app_printf("%2f uSv/h +/- %.2f\r\n",
             sparkfun_type5_get_usvh(),
             sparkfun_type5_get_usvh_error());
}

/**************************************************************************//**
 *  Callback function for noise detection.
 *****************************************************************************/
void noise_callback(void)
{
  app_printf("Noise! Please stop moving\r\n");
}
