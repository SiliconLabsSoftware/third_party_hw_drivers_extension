/***************************************************************************//**
 * @file mikroe_thunder.h
 * @brief SCL Thunder click Prototypes
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
#ifndef MIKROE_THUNDER_AS3935_H_
#define MIKROE_THUNDER_AS3935_H_

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------
#include "sl_status.h"
#include "thunder.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
//                       Public Functions
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] spi_instance
 *    SPI instance.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_thunder_as3935_init(mikroe_spi_handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    Thunder click set instance.
 *
 * @param[in] spi_instance
 *    SPI instance.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_thunder_as3935_set_spi_instance(
  mikroe_spi_handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    Click default configuration function.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_thunder_as3935_default_cfg(void);

/***************************************************************************//**
 * @brief
 *    Write reg function.
 *
 * @param[in] reg
 *    Address where data be written.
 * @param[in] data_in
 *    Data to be written.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_thunder_as3935_write_reg(uint8_t reg, uint8_t data_in);

/***************************************************************************//**
 * @brief
 *    Read reg function.
 *
 * @param[in] reg
 *    Address where data be written.
 * @param[out] data_out
 *    Buffer where data be stored.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_thunder_as3935_read_reg(uint8_t reg, uint8_t *data_out);

/***************************************************************************//**
 * @brief
 *    Command Send function
 *
 * @param[in] command
 *    0 - Reset command, Value different from 0 - RCO calibrate command.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_thunder_as3935_send_command(uint8_t command);

/***************************************************************************//**
 * @brief
 *    Interrupt check function.
 *
 * @return
 *    0x00 - No interrupt.
 *    0x01 - Noise level too high.
 *    0x04 - Disturber detected.
 *    0x08 - Lightning interrupt.
 ******************************************************************************/
uint8_t mikroe_thunder_as3935_check_int(void);

/***************************************************************************//**
 * @brief
 *    Storm information get function.
 *
 * @param[out] energy_out
 *    Energy of the single lightning.
 * @param[out] distance_out
 *    Distance estimation for the head of the storm.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_thunder_as3935_get_storm_info(uint32_t *energy_out,
                                                 uint8_t *distance_out);

/***************************************************************************//**
 * @brief
 *    IRQ pin Check function.
 *
 * @return
 *    Value of the IRQ pin (INT).
 ******************************************************************************/
uint8_t mikroe_thunder_as3935_check_irq_pin(void);

#ifdef __cplusplus
}
#endif

#endif /* MIKROE_THUNDER_AS3935_H_ */
