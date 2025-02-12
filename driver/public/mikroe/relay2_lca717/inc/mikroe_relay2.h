/***************************************************************************//**
 * @file mikroe_relay2.h
 * @brief SCL Relay2 Prototypes
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

#ifndef MIKROE_RELAY2_H_
#define MIKROE_RELAY2_H_

#include "sl_status.h"
#include "relay2.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * \addtogroup mikroe_relay2 - RELAY2 Click
 * @{
 *
 * @brief
 *  The integration of the Mikroe SDK Click extension relay2 driver.
 *  (Relay 2 Click board.)
 *
 ******************************************************************************/

// -------------------------------------------------------------- PUBLIC MACROS

/**
 * \defgroup macros Macros
 * \{
 */

/**
 * \defgroup error_code Error Code
 * \{
 */
#define RELAY2_RETVAL                                     uint8_t

#define MIKROE_RELAY2_OK                                  0x70
#define MIKROE_RELAY2_INIT_ERROR                          0xFF

/** \} */

/** \} */   // End group macro

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_relay2_init(void);

/***************************************************************************//**
 * @brief
 *    Controls the Relay 1 pin
 *
 * @param[in] pin_state 1  = On, 0 = off
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 ******************************************************************************/
void mikroe_relay2_relay1_control(uint8_t pin_state);

/***************************************************************************//**
 * @brief
 *    Controls the Relay 2 pin
 *
 * @param[in] pin_state 1  = On, 0 = off
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 ******************************************************************************/
void mikroe_relay2_relay2_control (uint8_t pin_state);

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* MIKROE_RELAY2_H_ */
