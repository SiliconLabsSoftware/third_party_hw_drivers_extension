/***************************************************************************//**
 * @file mikroe_water_detect.h
 * @brief SCL Water Detect Prototypes
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

#ifndef MIKROE_WATER_DETECT_H_
#define MIKROE_WATER_DETECT_H_

#include "sl_status.h"
#include "waterdetect.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @addtogroup  water_detect - Water Detect Click
 * @brief  Driver for the Mikroe Water Detect Click
 *    @n @section water_detect_example example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "mikroe_water_detect.h"
 * #include "app_log.h"
 *
 * uint8_t wd_state = 0;
 * uint8_t wd_state_old = 0;
 *
 *    int main(void)
 *    {
 *      ...
 *
 *      mikroe_water_detect_init();
 *
 *      while (true) {
 *
 *        ...
 *
 *        wd_state = mikroe_water_detect_get_status( );
 *
 *        if ( wd_state > wd_state_old ) {
 *          app_log(" >  Water is detected  < \r\n" );
 *          app_log("------------------------- \r\n" );
 *          wd_state_old = 1;
 *        }
 *
 *      if ( wd_state < wd_state_old ) {
 *          app_log(" There is no water \r\n" );
 *          app_log("------------------- \r\n" );
 *          wd_state_old = 0;
 *      }
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL otherwise
 ******************************************************************************/
sl_status_t mikroe_water_detect_init(void);

/***************************************************************************//**
 * @brief
 *    Get the status of the water detection function.
 *
 * @return
 *    water detect status:
 *    - 0 : water is detected;
 *    - 1 : water not detected
 ******************************************************************************/
uint8_t mikroe_water_detect_get_status(void);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup water_detect) */
#endif // MIKROE_WATER_DETECT_H_
