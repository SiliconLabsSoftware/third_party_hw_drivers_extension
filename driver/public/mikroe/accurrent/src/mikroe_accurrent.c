/***************************************************************************//**
 * @file mikroe_accurrent.c
 * @brief SCL ACCURRENT Driver
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

#include "accurrent.h"
#include "mikroe_accurrent.h"
#include "mikroe_accurrent_config.h"

static accurrent_t ctx;
static accurrent_cfg_t ctx_cfg;

/***************************************************************************//**
 * @brief This function initializes all necessary pins and peripherals used
 * for this click board.
 ******************************************************************************/
sl_status_t mikroe_accurrent_init(mikroe_spi_handle_t spi_instance)
{
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  accurrent_cfg_setup(&ctx_cfg);

  // Configure default spi instance
  ctx.spi.handle = spi_instance;

#if defined(ACCURRENT_CS_PORT) && defined(ACCURRENT_CS_PIN)
  ctx_cfg.cs = hal_gpio_pin_name(ACCURRENT_CS_PORT,
                                 ACCURRENT_CS_PIN);
#endif

#if (MIKROE_ACCURRENT_SPI_UC == 1)
  ctx_cfg.spi_speed = MIKROE_ACCURRENT_SPI_BITRATE;
#endif

  if (accurrent_init(&ctx, &ctx_cfg) != ACCURRENT_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief This function sets the SPI instance.
 ******************************************************************************/
sl_status_t mikroe_accurrent_set_spi_instance(mikroe_spi_handle_t spi_instance)
{
  if (NULL == spi_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  ctx.spi.handle = spi_instance;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief This function use the SPI to perform generic transfer.
 ******************************************************************************/
void mikroe_accurrent_generic_transfer(uint8_t *wr_buf, uint16_t wr_len,
                                       uint8_t *rd_buf, uint16_t rd_len)
{
  return accurrent_generic_transfer(&ctx, wr_buf, wr_len, rd_buf, rd_len);
}

/***************************************************************************//**
 * @brief Gets current value of AC Current in A.
 ******************************************************************************/
float mikroe_accurrent_get_a(void)
{
  return accurrent_get_a(&ctx);
}

/***************************************************************************//**
 * @brief Gets current value of AC Current in mA.
 ******************************************************************************/
float mikroe_accurrent_get_ma(void)
{
  return accurrent_get_ma(&ctx);
}
