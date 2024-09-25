/***************************************************************************//**
 * @file mikroe_a172mrq.c
 * @brief SCL fingerprint2 Prototypes
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

#include "fingerprint2.h"
#include "mikroe_a172mrq.h"
#include "mikroe_a172mrq_config.h"
#include "drv_digital_out.h"
#include "drv_digital_in.h"

static fingerprint2_t fingerprint2;
static fingerprint2_cfg_t fingerprint2_cfg;
static bool initialized = false;

void mikroe_a172mrq_cfg_setup(void)
{
  fingerprint2_cfg_setup(&fingerprint2_cfg);
}

sl_status_t mikroe_a172mrq_init(mikroe_uart_handle_t handle)
{
  if (NULL == handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  fingerprint2.uart.handle = handle;
  fingerprint2_cfg_setup(&fingerprint2_cfg);

#if defined (A172MRQ_RESET_PORT) && defined(A172MRQ_RESET_PIN)
  fingerprint2_cfg.rst = hal_gpio_pin_name(A172MRQ_RESET_PORT,
                                           A172MRQ_RESET_PIN);
#endif

#if defined (A172MRQ_LD1_PORT) && defined(A172MRQ_LD1_PIN)
  fingerprint2_cfg.ld1 = hal_gpio_pin_name(A172MRQ_LD1_PORT,
                                           A172MRQ_LD1_PIN);
#endif

#if defined (A172MRQ_LD2_PORT) && defined(A172MRQ_LD2_PIN)
  fingerprint2_cfg.ld2 = hal_gpio_pin_name(A172MRQ_LD2_PORT,
                                           A172MRQ_LD2_PIN);
#endif

#if defined (A172MRQ_GP1_PORT) && defined(A172MRQ_GP1_PIN)
  fingerprint2_cfg.gp1 = hal_gpio_pin_name(A172MRQ_GP1_PORT,
                                           A172MRQ_GP1_PIN);
#endif

#if defined (A172MRQ_GP2_PORT) && defined(A172MRQ_GP2_PIN)
  fingerprint2_cfg.gp2 = hal_gpio_pin_name(A172MRQ_GP2_PORT,
                                           A172MRQ_GP2_PIN);
#endif

  if (fingerprint2_init(&fingerprint2, &fingerprint2_cfg) != FINGERPRINT2_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  initialized = true;
  return SL_STATUS_OK;
}

sl_status_t mikroe_a172mrq_set_uart_instance(mikroe_uart_handle_t handle)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  if (NULL == handle) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  fingerprint2.uart.handle = handle;

  return SL_STATUS_OK;
}

sl_status_t mikroe_a172mrq_generic_write(char *data_buf, uint16_t len)
{
  if (NULL == data_buf) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  fingerprint2_generic_write(&fingerprint2, data_buf, len);

  return SL_STATUS_OK;
}

sl_status_t mikroe_a172mrq_generic_read(char *data_buf,
                                        uint16_t max_len,
                                        int32_t *rsp_size)
{
  if (NULL == data_buf) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  *rsp_size = fingerprint2_generic_read(&fingerprint2, data_buf, max_len);

  return SL_STATUS_OK;
}

uint8_t mikroe_a172mrq_get_ld1_status(void)
{
  return fingerprint2_get_ld1_status(&fingerprint2);
}

uint8_t mikroe_a172mrq_get_ld2_status(void)
{
  return fingerprint2_get_ld2_status(&fingerprint2);
}

void mikroe_a172mrq_set_rst_status(uint8_t status)
{
  fingerprint2_set_rst_status(&fingerprint2, status);
}

void mikroe_a172mrq_set_gp1_status(uint8_t status)
{
  fingerprint2_set_gp1_status(&fingerprint2, status);
}

void mikroe_a172mrq_set_gp2_status(uint8_t status)
{
  fingerprint2_set_gp2_status(&fingerprint2, status);
}

void mikroe_a172mrq_reset(void)
{
  fingerprint2_reset(&fingerprint2);
}

void mikroe_a172mrq_reg_one_fp(uint8_t fp_index)
{
  fingerprint2_reg_one_fp(&fingerprint2, fp_index);
}

void mikroe_a172mrq_delete_one_fp(uint8_t fp_index)
{
  fingerprint2_delete_one_fp(&fingerprint2, fp_index);
}
