/***************************************************************************//**
 * @file mikroe_lb11685.h
 * @brief mikroe lb11685 Prototypes
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
#ifndef MIKROE_LB11685AV_H_
#define MIKROE_LB11685AV_H_

#include "sl_status.h"
#include "brushless16.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup mikroe_lb11685 LB11685AV - BRUSHLESS 16 Click
 * @brief
 *  The integration of the Mikroe SDK Click extension LB11685AV driver.
 *  (BRUSHLESS 16 Click)
 *
 *    @n @section lb11685_example lb11685 example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "mikroe_lb11685.h"
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize sensor and set mode
 *      mikroe_lb11685av_init();
 *      mikroe_lb11685av_get_rd();
 *      mikroe_lb11685av_get_fg();
 *      while (true);
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/**************************************************************************//**
 *  @brief
 *   Brushless 16 initialization function.
 *
 *  @return
 *   SL_STATUS_OK if success.
 *   SL_STATUS_FAIL if fail.
 ******************************************************************************/
sl_status_t mikroe_lb11685av_init(void);

/**************************************************************************//**
 *  @brief
 *   Set en pin state.
 *
 *  @param[in] state
 *   State of en pin .
 ******************************************************************************/
void mikroe_lb11685av_set_en(uint8_t state);

/**************************************************************************//**
 *  @brief
 *   Get rd pin state.
 *
 *  @return
 *   The state of the rd pin 1 for high and 0 for low.
 ******************************************************************************/
uint8_t mikroe_lb11685av_get_rd(void);

/**************************************************************************//**
 *  @brief
 *   Get fg pin state.
 *
 *  @return
 *   The state of the fg pin 1 for high and 0 for low.
 ******************************************************************************/
uint8_t mikroe_lb11685av_get_fg(void);

/** @} (end addtogroup mikroe_lb11685) */

#ifdef __cplusplus
}
#endif

#endif // MIKROE_LB11685AV_H_
