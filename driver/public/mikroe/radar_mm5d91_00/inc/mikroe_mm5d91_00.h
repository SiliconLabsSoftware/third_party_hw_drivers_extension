/***************************************************************************//**
 * @file mikroe_mm5d91_00.h
 * @brief SCL MM5D91_00 Prototypes
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef MIKROE_MM5D91_00
#define MIKROE_MM5D91_00

#include "sl_status.h"
#include "radar.h"

typedef radar_t mikroe_radar_t;

/***************************************************************************//**
 * @brief Radar initialization function.
 * @details This function initializes all necessary pins and peripherals used
 *          for this click board.
 * @param[in] handle : Click context object.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * See #err_t definition for detailed explanation.
 * @note None.
 ******************************************************************************/
sl_status_t mikroe_radar_init(mikroe_uart_handle_t handle);

/***************************************************************************//**
 * @brief This function sets the UART instance used by platform functions
 * @param[in] handle: UART instance
 * @return
 *    SL_STATUS_OK if there are no errors
 *    SL_STATUS_INVALID_PARAMETER if spi_instance is null
 ******************************************************************************/
sl_status_t mikroe_radar_set_uart_instance(mikroe_uart_handle_t handle);

/***************************************************************************//**
 * @brief Radar data writing function.
 * @details This function writes a desired number of data bytes by using UART
 *          serial interface.
 * @param[in] data_buf : Data buffer for sending.
 * @param[in] len : Number of bytes for sending.
 * @return @li @c  >=0 - Success,
 *         @li @c   <0 - Error.
 * See #err_t definition for detailed explanation.
 * @note None.
 ******************************************************************************/
int32_t mikroe_radar_generic_write(uint8_t *data_buf,
                                   uint16_t len);

/***************************************************************************//**
 * @brief Radar data reading function.
 * @details This function reads a desired number of data bytes by using UART
 *          serial interface.
 * @param[out] data_buf : Output read data.
 * @param[in] max_len : Number of bytes to be read.
 * @return @li @c  >0 - Number of data bytes read,
 *         @li @c <=0 - Error/Empty Ring buffer.
 * See #err_t definition for detailed explanation.
 * @note None.
 ******************************************************************************/
int32_t mikroe_radar_generic_read(uint8_t *data_buf,
                                  uint16_t max_len);

/***************************************************************************//**
 * @brief Radar enable device function.
 * @details This function enables the device by setting the RST pin to low logic
 *          state.
 ******************************************************************************/
void mikroe_radar_enable_device(void);

/***************************************************************************//**
 * @brief Radar disable device function.
 * @details This function disables the device by setting the RST pin to high
 *          logic state.
 ******************************************************************************/
void mikroe_radar_disable_device(void);

/***************************************************************************//**
 * @brief Radar get gpio 2 pin function.
 * @details This function returns the GPIO 2 (bootloader mode indication) pin
 *          logic state.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * @note None.
 ******************************************************************************/
sl_status_t mikroe_radar_get_gpio2_pin(uint8_t *gpio_val);

/***************************************************************************//**
 * @brief Radar get gpio 1 pin function.
 * @details This function returns the GPIO 1 (presence detected indication) pin
 *          logic state.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * @note None.
 ******************************************************************************/
sl_status_t mikroe_radar_get_gpio1_pin(uint8_t *gpio_val);

/***************************************************************************//**
 * @brief Radar get gpio 0 pin function.
 * @details This function returns the GPIO 0 (no presence indication) pin logic
 *          state.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * @note None.
 ******************************************************************************/
sl_status_t mikroe_radar_get_gpio0_pin(uint8_t *gpio_val);

/***************************************************************************//**
 * @brief Radar set command function.
 * @details This function sends a desired set command with payload to the module
 *          and waits for an ACK response.
 * @param[in] cmd_id : Set command ID.
 * @param[in] payload : Command payload content.
 * @param[in] payload_size : Command payload size in bytes.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * See #err_t definition for detailed explanation.
 * @note None.
 ******************************************************************************/
sl_status_t mikroe_radar_set_command(uint8_t cmd_id,
                                     uint8_t *payload,
                                     uint8_t payload_size);

/***************************************************************************//**
 * @brief Radar get command function.
 * @details This function sends a desired get command to the module and waits
 *          for a response.
 * @param[in] cmd_id : Get command ID.
 * @param[out] payload : Command payload content.
 * @param[out] payload_size : Command payload size in bytes.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * See #err_t definition for detailed explanation.
 * @note None.
 ******************************************************************************/
sl_status_t mikroe_radar_get_command(uint8_t cmd_id,
                                     uint8_t *payload,
                                     uint8_t *payload_size);

/***************************************************************************//**
 * @brief Radar get event function.
 * @details This function waits for an IN/OUT event or ACK command response.
 * @param[out] evt_id : Event ID.
 * @param[out] payload : Event payload content.
 * @param[out] payload_size : Event payload size in bytes.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * See #err_t definition for detailed explanation.
 * @note None.
 ******************************************************************************/
sl_status_t mikroe_radar_get_event(uint8_t *evt_id,
                                   uint8_t *payload,
                                   uint8_t *payload_size);

/***************************************************************************//**
 * @brief Radar get temperature function.
 * @details This function reads the chip internal temperature.
 * @param[out] temperature : Chip internal temperature in Celsius.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * See #err_t definition for detailed explanation.
 * @note None.
 ******************************************************************************/
sl_status_t mikroe_radar_get_temperature(float *temperature);

/***************************************************************************//**
 * @brief Radar set detection range function.
 * @details This function sets the min and max presence detection values.
 * @param[in] min : Minimal detection level in meters [0.0 - 10.0].
 * @param[in] max : Maximal detection level in meters [0.2 - 10.0] (Must be >=
 *   min).
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * See #err_t definition for detailed explanation.
 * @note None.
 ******************************************************************************/
sl_status_t mikroe_radar_set_detection_range(float min,
                                             float max);

/***************************************************************************//**
 * @brief Radar reset config function.
 * @details This function resets all configuration to default and performs a
 *   system reset.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * See #err_t definition for detailed explanation.
 * @note None.
 ******************************************************************************/
sl_status_t mikroe_radar_reset_config(void);

/***************************************************************************//**
 * @brief Radar float ieee to mchip function.
 * @details This function converts the float IEEE-754 to Microchip
 *   representation.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * @note This is an empty function unless AI for PIC compiler is selected.
 ******************************************************************************/
sl_status_t mikroe_radar_float_ieee_to_mchip(float *f_data);

/***************************************************************************//**
 * @brief Radar float ieee to mchip function.
 * @details This function converts the float IEEE-754 to Microchip
 *   representation.
 * @return @li @c SL_STATUS_OK - Success,
 *         @li @c SL_STATUS_INVALID_PARAMETER - Invalid parameter.
 * @note This is an empty function unless AI for PIC compiler is selected.
 ******************************************************************************/
sl_status_t mikroe_radar_float_mchip_to_ieee(float *f_data);

#endif // MIKROE_MM5D91_00
