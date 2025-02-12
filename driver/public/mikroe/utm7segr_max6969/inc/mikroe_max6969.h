/***************************************************************************//**
 * @file mikroe_max6969.h
 * @brief SCL max6969 Prototypes
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef MIKROE_MAX6969_H_
#define MIKROE_MAX6969_H_

#include "sl_status.h"
#include "utm7segr.h"
#include "drv_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @addtogroup  max6969 MAX6969 - UT-M 7-SEG R click
 * @brief  Driver for the Mikroe UT-M 7-SEG R click
 *    @n @section max6969_example CAP1166 example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "mikroe_max6969.h"
 * #include "sl_spidrv_instances.h"
 * #include "sl_pwm_instances.h"
 *    int main(void)
 *    {
 *      ...
 *
 *      // Initialize led 7 seg
 *      mikroe_max6969_init(sl_spidrv_mikroe_handle, &sl_pwm_mikroe);
 *
 *      mikroe_max6969_display_number( 11, MIKROE_UTM7SEGR_DOT_LEFT );
 *
 *      while (true) {
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
 ******************************************************************************/

/**************************************************************************//**
 * @addtogroup mikroe_utm7segr_def Mikroe utm7segr definitions
 * @brief Dot and display definitions
 * @{
 ******************************************************************************/
#define MIKROE_UTM7SEGR_DOT_LEFT              0x02
#define MIKROE_UTM7SEGR_DOT_RIGHT             0x01
#define MIKROE_UTM7SEGR_DOT_LEFT_RIGHT        0x03
#define MIKROE_UTM7SEGR_NO_DOT                0x00

#define MIKROE_UTM7SEGR_DISPLAY_ON            0x01
#define MIKROE_UTM7SEGR_DISPLAY_OFF           0x00

#define MIKROE_UTM7SEGR_PWM_FREQUENCY         (10000)

/** @} (end addtogroup mikroe_buzz2_freq) */

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @param[in] pwm_instance
 *    PWM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_max6969_init(mikroe_spi_handle_t spi_instance,
                                mikroe_pwm_handle_t pwm_instance);

/***************************************************************************//**
 * @brief
 *    Set the light intensity of LED 7-segment.
 *
 * @param[in] percent
 *   Percent of the light intensity.
 *   0 <-> off
 *   100 <-> max of the light intensity
 *
 * @return
 *    @ref SL_STATUS_OK on success or @ref SL_STATUS_FAIL on failure
 ******************************************************************************/
void mikroe_max6969_set_contrast(uint8_t percent);

/***************************************************************************//**
 * @brief
 *    This function sets the SPI instance used by platform functions
 *
 * @param[in] spi_instance
 *    SPI instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors
 *    SL_STATUS_INVALID_PARAMETER if spi_instance is null
 ******************************************************************************/
sl_status_t mikroe_max6969_set_spi_instance(mikroe_spi_handle_t spi_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the PWM instance used by platform functions.
 *
 * @param[in] pwm_instance
 *    PWM instance, default: sl_pwm_mikroe
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if pwm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_max6969_set_pwm_instance(mikroe_pwm_handle_t pwm_instance);

/***************************************************************************//**
 * @brief
 *    Max6969 data writing function.
 *
 * @param[in] data_in
 *    Data to be written.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if data_in is null.
 *    SL_STATUS_FAIL otherwise
 ******************************************************************************/
sl_status_t mikroe_max6969_generic_write(uint8_t *data_in);

/***************************************************************************//**
 * @brief
 *    This function turns display on and off.
 *
 * @param[in] state
 *    @li @c 0 ( MIKROE_UTM7SEGR_DISPLAY_OFF ) - Display off,
 *    @li @c 1 ( MIKROE_UTM7SEGR_DISPLAY_ON  ) - Display on.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL otherwise
 ******************************************************************************/
void mikroe_max6969_display_state(uint8_t state);

/***************************************************************************//**
 * @brief
 *    This function is used to show the number on the display.
 *
 * @param[in] number
 *    Number from 0 to 99.
 *
 * @param[in] dot_pos
 *    Dot position
 *      No use dot           (MIKROE_UTM7SEGR_NO_DOT)
 *      Dot left             (MIKROE_UTM7SEGR_DOT_LEFT)
 *      Dot right            (MIKROE_UTM7SEGR_DOT_RIGHT)
 *      Dot left and right   (MIKROE_UTM7SEGR_DOT_LEFT_RIGHT)
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL otherwise
 ******************************************************************************/
sl_status_t mikroe_max6969_display_number(uint8_t number, uint8_t dot_pos);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup max6969) */
#endif /* MIKROE_MAX6969_H_ */
