/***************************************************************************//**
 * @file mikroe_mq131.c
 * @brief Mikroe Ozone 2 Click Driver API
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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

#include "mikroe_mq131.h"
#include "drv_digital_in.h"
#include "drv_digital_out.h"

#ifdef SLI_SI917
#include "RTE_Device_917.h"
#else
#include "spidrv.h"
#endif

static ozone2_t ctx;
static ozone2_cfg_t cfg;

sl_status_t mikroe_ozone2_init(mikroe_spi_handle_t spi,
                               mikroe_adc_handle_t adc)
{
  ozone2_cfg_setup(&cfg);

#if MIKROE_OZONE2_ADC_SEL == 0

  if (adc == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  (void)spi;

#if defined(MIKROE_OZONE2_AN_PORT) && defined(MIKROE_OZONE2_AN_PIN)
  cfg.an = hal_gpio_pin_name(MIKROE_OZONE2_AN_PORT, MIKROE_OZONE2_AN_PIN);
#endif

  cfg.adc_sel = OZONE2_ADC_SEL_AN;
  cfg.vref = 3.3;
  ctx.an.handle = adc;

#else
  if (spi == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  (void)adc;

  cfg.adc_sel = OZONE2_ADC_SEL_SPI;

#ifdef SLI_SI917
  cfg.miso = hal_gpio_pin_name(RTE_GSPI_MASTER_MISO_PORT,
                               RTE_GSPI_MASTER_MISO_PIN);
#else
  const SPIDRV_Handle_t ptr = (SPIDRV_Handle_t)spi;
  cfg.miso = hal_gpio_pin_name(ptr->initData.portRx, ptr->initData.pinRx);
#endif

#if defined(MIKROE_OZONE2_CS_PORT) && defined(MIKROE_OZONE2_CS_PIN)
  cfg.cs = hal_gpio_pin_name(MIKROE_OZONE2_CS_PORT, MIKROE_OZONE2_CS_PIN);
  // CS pin need to init here since the mikroe_sdk_v2 missed this step
  digital_out_t struct_cs;
  digital_out_init(&struct_cs, cfg.cs);
#endif

#if (MIKROE_OZONE2_CLICK_SPI_UC == 1)
  cfg.spi_speed = MIKROE_OZONE2_CLICK_SPI_UC_BITRATE;
#endif

  ctx.spi.handle = spi;
#endif

  if (OZONE2_OK == ozone2_init(&ctx, &cfg)) {
    return SL_STATUS_OK;
  }

  return SL_STATUS_INITIALIZATION;
}

sl_status_t mikroe_ozone2_read_signal_voltage(float *data_out)
{
  if (data_out == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  if (OZONE2_OK == ozone2_read_signal_voltage(&ctx, data_out)) {
    return SL_STATUS_OK;
  }
  return SL_STATUS_FAIL;
}

sl_status_t mikroe_ozone2_read_measurement(uint16_t *data_out)
{
  if (data_out == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  if (OZONE2_OK == ozone2_read_measurement(&ctx, data_out)) {
    return SL_STATUS_OK;
  }
  return SL_STATUS_FAIL;
}
