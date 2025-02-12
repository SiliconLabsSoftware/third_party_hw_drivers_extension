/***************************************************************************//**
 * @file mikroe_l9958.h
 * @brief SCL L9958 Prototypes
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

#ifndef _MIKROE_L9958_H_
#define _MIKROE_L9958_H_

#include <stdint.h>
#include "sl_status.h"
#include "dcmotor24.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup MIKROE L9958 CLICK CONFIGURATION DEFINITION
 * @brief Define configurations for registers of L9958
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief L9958 configuration protocol (CFG_REG) setting.
 * @details Specified setting for configuration protocol (CFG_REG) of L9958.
 ******************************************************************************/
#define MIKROE_L9958_DR                        0x0002u
#define MIKROE_L9958_CL_2p5A                   0x0000u
#define MIKROE_L9958_CL_4A                     0x0004u
#define MIKROE_L9958_CL_6p6A                   0x0008u
#define MIKROE_L9958_CL_8p6A                   0x000Cu
#define MIKROE_L9958_CL_MASK                   0x000Cu
#define MIKROE_L9958_VSR                       0x0100u
#define MIKROE_L9958_ISR                       0x0200u
#define MIKROE_L9958_ISR_DIS                   0x0400u
#define MIKROE_L9958_OL_ON                     0x0800u

/***************************************************************************//**
 * @brief L9958 diagnostics protocol (DIA_REG) setting.
 * @details Specified setting for diagnostics protocol (DIA_REG) of L9958.
 ******************************************************************************/
#define MIKROE_L9958_DIA_OL_OFF                0x0001u
#define MIKROE_L9958_DIA_OL_ON                 0x0002u
#define MIKROE_L9958_DIA_VS_UV                 0x0004u
#define MIKROE_L9958_DIA_VDD_OV                0x0008u
#define MIKROE_L9958_DIA_ILIM                  0x0010u
#define MIKROE_L9958_DIA_TWARN                 0x0020u
#define MIKROE_L9958_DIA_TSD                   0x0040u
#define MIKROE_L9958_DIA_ACT                   0x0080u
#define MIKROE_L9958_DIA_OC_LS1                0x0100u
#define MIKROE_L9958_DIA_OC_LS2                0x0200u
#define MIKROE_L9958_DIA_OC_HS1                0x0400u
#define MIKROE_L9958_DIA_OC_HS2                0x0800u
#define MIKROE_L9958_DIA_SGND_OFF              0x4000u
#define MIKROE_L9958_DIA_SBAT_OFF              0x8000u

/***************************************************************************//**
 * @brief L9958 direction setting.
 * @details Specified setting for direction of L9958 driver.
 ******************************************************************************/
#define MIKROE_L9958_DIR_REVERSE               0
#define MIKROE_L9958_DIR_FORWARD               1

/***************************************************************************//**
 * @brief L9958 default PWM frequency.
 * @details Specified setting for default PWM frequency of L9958 driver.
 ******************************************************************************/
#define MIKROE_L9958_DEF_FREQ                  5000

/*! @} */ // mikroe_l9958_set

/*!
 * @addtogroup MIKROE L9958 CLICK PROTOTYPES
 * @brief API for configuring and manipulating DC Motor 24 Click driver.
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *    This function initializes all necessary pins and peripherals used
 * for this click board.
 * @param[in]
 *    spi_instance : An SPI driver instance handle.
 *    pwm_instance : An PWM driver instance handle
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL Initialization failed.
 *    SL_STATUS_INVALID_PARAMETER if spi_instance is null.
 ******************************************************************************/
sl_status_t mikroe_l9958_init (mikroe_spi_handle_t spi_instance,
                               mikroe_pwm_handle_t pwm_instance);

/***************************************************************************//**
 * @brief DC Motor 24 default configuration function.
 * @details This function executes a default configuration of DC Motor 24
 * click board.
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL if failed.
 ******************************************************************************/
sl_status_t mikroe_l9958_default_cfg (void);

/***************************************************************************//**
 * @brief DC Motor 24 write config function.
 * @details This function writes a desired configuration word by using SPI
 *   serial interface.
 * @param[in] config_word : Configuration word for CFG_REG.
 * @return SL_STATUS_OK.
 ******************************************************************************/
sl_status_t mikroe_l9958_write_config (uint16_t config_word);

/***************************************************************************//**
 * @brief DC Motor 24 read diag function.
 * @details This function reads a diagnostics word by using SPI serial
 *   interface.
 * @param[out] diag : Diagnostics word from DIA_REG.
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if diag is null.
 *    error code if errors.
 ******************************************************************************/
sl_status_t mikroe_l9958_read_diag (uint16_t *diag);

/***************************************************************************//**
 * @brief DC Motor 24 set direction function.
 * @details This function sets the motor direction.
 * @param[in] dir @li @c 0 - Reverse,
 *                @li @c 1 - Forward.
 * @return none.
 ******************************************************************************/
void mikroe_l9958_set_direction (uint8_t dir);

/***************************************************************************//**
 * @brief DC Motor 24 switch direction function.
 * @details This function switches the direction by toggling the DIR pin state.
 * @return none.
 ******************************************************************************/
void mikroe_l9958_switch_direction (void);

/***************************************************************************//**
 * @brief DC Motor 24 enable output function.
 * @details This function enables the output by setting the EN pin to high logic
 *   state.
 * @return none.
 ******************************************************************************/
void mikroe_l9958_enable_output (void);

/***************************************************************************//**
 * @brief DC Motor 24 disable output function.
 * @details This function disables the output by setting the EN pin to low logic
 *   state.
 * @return none.
 ******************************************************************************/
void mikroe_l9958_disable_output (void);

/***************************************************************************//**
 * @brief DC Motor 24 sets PWM duty cycle.
 * @details This function sets the PWM duty cycle in percentages ( Range[ 0..1 ]
 *   ).
 * @param[in] duty_cycle : PWM duty cycle.
 * @return SL_STATUS_OK or error code if errors.
 ******************************************************************************/
sl_status_t mikroe_l9958_set_duty_cycle (float duty_cycle);

/***************************************************************************//**
 * @brief DC Motor 24 stop PWM module.
 * @details This function stops the PWM module output.
 * @return SL_STATUS_OK or error code if errors.
 ******************************************************************************/
sl_status_t mikroe_l9958_pwm_stop (void);

/***************************************************************************//**
 * @brief DC Motor 24 start PWM module.
 * @details This function starts the PWM module output.
 * @return SL_STATUS_OK or error code if errors.
 ******************************************************************************/
sl_status_t mikroe_l9958_pwm_start (void);

#ifdef __cplusplus
}
#endif
#endif // MIKROE_L9958_H
