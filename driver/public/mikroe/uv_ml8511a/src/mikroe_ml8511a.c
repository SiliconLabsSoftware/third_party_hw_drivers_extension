/***************************************************************************//**
 * @file mikroe_ml8511a.c
 * @brief SCL ML8511A Source File
 * @version 1.0.0
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

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include "mikroe_ml8511a.h"
#include "mikroe_ml8511a_config.h"

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

static uv_t uv_ctx;
static uv_cfg_t uv_cfg;
static bool initialized = false;

// -----------------------------------------------------------------------------
//                       Public Functions
// -----------------------------------------------------------------------------

/**************************************************************************//**
 *  ML8511A initialization.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_init(mikroe_spi_handle_t spi_instance,
                                mikroe_adc_handle_t adc_instance)
{
  if (spi_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (adc_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (initialized) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  uv_ctx.spi.handle = spi_instance;
  uv_ctx.an.handle = adc_instance;
  uv_cfg_setup(&uv_cfg);

#if defined(ML8511A_AN_PORT) && defined(ML8511A_AN_PIN)
  uv_cfg.an = hal_gpio_pin_name(ML8511A_AN_PORT, ML8511A_AN_PIN);
#endif // ML8511A_AN_PORT & ML8511A_AN_PIN

#if MIKROE_ML8511A_OPERATION_MODE == ADC_MODE
#if (MIKROE_ML8511A_SPI_UC == 1)
  uv_cfg.spi_speed = MIKROE_ML8511A_SPI_BITRATE;
#endif // MIKROE_ML8511A_SPI_UC
#endif // MIKROE_ML8511A_OPERATION_MODE

  uv_cfg.en = hal_gpio_pin_name(ML8511A_EN_PORT, ML8511A_EN_PIN);

  if (uv_init(&uv_ctx, &uv_cfg) != SPI_MASTER_SUCCESS) {
    return SL_STATUS_INITIALIZATION;
  }
  initialized = true;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  ML8511A sets callback.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_set_callback_handler(uv_callback_t handler)
{
  uv_set_callback_handler(&uv_ctx, handler);

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  ML8511A reads adc value.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_read_adc_value(uint16_t *data_out)
{
  return uv_read_adc_value(&uv_ctx, data_out);
}

/**************************************************************************//**
 *  ML8511A reads adc voltage.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_read_adc_voltage(float *data_out)
{
  return uv_read_adc_voltage(&uv_ctx, data_out);
}

/**************************************************************************//**
 *  ML8511A reads an pin value.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_read_an_pin_value(uint16_t *data_out)
{
  return uv_read_an_pin_value(&uv_ctx, data_out);
}

/**************************************************************************//**
 *  ML8511A reads an pin voltage.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_read_an_pin_voltage(float *data_out)
{
  return uv_read_an_pin_voltage(&uv_ctx, data_out);
}

/**************************************************************************//**
 *  ML8511A calculates index.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_calc_index(float data_in, uint8_t *data_out)
{
  uv_calc_index(&uv_ctx, data_in, data_out);

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  ML8511A enables device.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_device_enable(void)
{
  uv_device_enable(&uv_ctx);

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  ML8511A disables device.
 ******************************************************************************/
sl_status_t mikroe_ml8511a_device_disable(void)
{
  uv_device_disable(&uv_ctx);

  return SL_STATUS_OK;
}
