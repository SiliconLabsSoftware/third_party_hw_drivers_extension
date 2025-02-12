/***************************************************************************//**
 * @file mikroe_l9958.C
 * @brief SCL L9958 Driver
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

#include "mikroe_l9958.h"
#include "mikroe_l9958_config.h"

static dcmotor24_t dcmotor;
static dcmotor24_cfg_t dcmotor_cfg;

/***************************************************************************//**
 * @brief This function initializes all necessary pins and peripherals used
 * for this click board.
 ******************************************************************************/
sl_status_t mikroe_l9958_init(mikroe_spi_handle_t spi_instance,
                              mikroe_pwm_handle_t pwm_instance)
{
  if ((NULL == spi_instance) || (NULL == pwm_instance)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default spi instance
  dcmotor.spi.handle = spi_instance;
  dcmotor.pwm.handle = pwm_instance;

  // Call basic setup functions
  dcmotor24_cfg_setup(&dcmotor_cfg);
  dcmotor_cfg.spi_mode = SPI_MASTER_MODE_0;

#if defined(DCMOTOR24_ENABLE_PORT) && defined(DCMOTOR24_ENABLE_PIN)
  dcmotor_cfg.en = hal_gpio_pin_name(DCMOTOR24_ENABLE_PORT,
                                     DCMOTOR24_ENABLE_PIN);
#endif

#if defined(DCMOTOR24_DIRECTION_PORT) && defined(DCMOTOR24_DIRECTION_PIN)
  dcmotor_cfg.dir = hal_gpio_pin_name(DCMOTOR24_DIRECTION_PORT,
                                      DCMOTOR24_DIRECTION_PIN);
#endif

#if (MIKROE_L9958_SPI_UC == 1)
  dcmotor_cfg.spi_speed = MIKROE_L9958_SPI_BITRATE;
#endif

  if (dcmotor24_init(&dcmotor, &dcmotor_cfg) != SPI_MASTER_SUCCESS) {
    return SL_STATUS_INITIALIZATION;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief DC Motor 24 default configuration function.
 ******************************************************************************/
sl_status_t mikroe_l9958_default_cfg(void)
{
  return dcmotor24_default_cfg(&dcmotor);
}

/***************************************************************************//**
 * @brief DC Motor 24 write config function.
 ******************************************************************************/
sl_status_t mikroe_l9958_write_config(uint16_t config_word)
{
  return dcmotor24_write_config(&dcmotor, config_word);
}

/***************************************************************************//**
 * @brief DC Motor 24 read diag function.
 ******************************************************************************/
sl_status_t mikroe_l9958_read_diag(uint16_t *diag)
{
  if (NULL == diag) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return dcmotor24_read_diag(&dcmotor, diag);
}

/***************************************************************************//**
 * @brief DC Motor 24 set direction function.
 ******************************************************************************/
void mikroe_l9958_set_direction(uint8_t dir)
{
  dcmotor24_set_direction(&dcmotor, dir);
}

/***************************************************************************//**
 * @brief DC Motor 24 switch direction function.
 ******************************************************************************/
void mikroe_l9958_switch_direction(void)
{
  dcmotor24_switch_direction(&dcmotor);
}

/***************************************************************************//**
 * @brief DC Motor 24 enable output function.
 ******************************************************************************/
void mikroe_l9958_enable_output(void)
{
  dcmotor24_enable_output(&dcmotor);
}

/***************************************************************************//**
 * @brief DC Motor 24 disable output function.
 ******************************************************************************/
void mikroe_l9958_disable_output(void)
{
  dcmotor24_disable_output(&dcmotor);
}

/***************************************************************************//**
 * @brief DC Motor 24 sets PWM duty cycle.
 ******************************************************************************/
sl_status_t mikroe_l9958_set_duty_cycle(float duty_cycle)
{
  return dcmotor24_set_duty_cycle(&dcmotor, duty_cycle);
}

/***************************************************************************//**
 * @brief DC Motor 24 stop PWM module.
 ******************************************************************************/
sl_status_t mikroe_l9958_pwm_stop(void)
{
  return dcmotor24_pwm_stop(&dcmotor);
}

/***************************************************************************//**
 * @brief DC Motor 24 start PWM module.
 ******************************************************************************/
sl_status_t mikroe_l9958_pwm_start(void)
{
  return dcmotor24_pwm_start(&dcmotor);
}
