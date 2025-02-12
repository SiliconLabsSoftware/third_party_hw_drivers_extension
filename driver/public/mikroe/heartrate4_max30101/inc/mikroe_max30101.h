/***************************************************************************//**
 * @file mikroe_max30101.h
 * @brief SCL MAX30101 Prototypes
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

#ifndef MIKROE_MAX30101_H_
#define MIKROE_MAX30101_H_

#include "sl_status.h"
#include "heartrate4.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup mikroe_MAX30101 - HEARTRATE4 Click
 * @brief  The integration of the Mikroe SDK Click extension MAX30101 driver.
 *         (HEARTRATE 4 Click board.)
 *
 *    @n @section max30101_example MAX30101 example
 *
 *      Basic example @n @n
 *      @code{.c}
 *
 * #include "mikroe_max30101.h"
 * #include "sl_i2cspm_instances.h"
 * #include "app_log.h"
 * #include "sl_sleeptimer.h"
 *    int main(void)
 *    {
 *      ...
 *
 *      if (mikroe_max30101_init(sl_i2cspm_mikroe) == SL_STATUS_OK) {
 *          app_log("max 30101 init successfully\n");
 *      }
 *      sl_sleeptimer_delay_millisecond(200);
 *
 *      while (true) {
 *
 *        if ( mikroe_max30101_get_intrrupt( 1 ) & 0x40 ) {
 *        red_samp = mikroe_max30101_get_red_val( );
 *        counter++;
 *
 *     // If sample pulse amplitude is not under threshold value 0x8000
 *     if ( red_samp > 0x8000 ) {
 *         app_log( "%lu\r\n", red_samp );
 *         sl_sleeptimer_delay_millisecond(1);
 *         counter = 200;
 *     } else if ( counter > 200 ) {
 *         app_log("Place Finger On Sensor\r\n" );
 *         sl_sleeptimer_delay_millisecond(100);
 *         counter = 0;
 *     }
 *
 *
 *        ...
 *
 *      }
 *
 *    } @endcode
 *
 * @{
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
#define MIKROE_MAX30101_REG_CHIP_ID      0xFF
#define MIKROE_MAX30101_CHIP_ID          0x15

/** \} */

/** \} */   // End group macro

/**
 * \defgroup max30101_typedefs Types
 * \{
 */

/***************************************************************************//**
 * @brief
 *    Click registers object definition.
 ******************************************************************************/
typedef struct
{
  // Structure fields

  uint8_t avg_samp;
  uint8_t mode;
  uint8_t adc_range;
  uint8_t samp_rate;
  uint8_t pulse_width;
  uint8_t led_pow_lvl;
} mikroe_max30101_set_registers_t;

/** \} */

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_max30101_init(mikroe_i2c_handle_t i2c_instance);

/***************************************************************************//**
 * @brief
 *    This function sets the IC2SPM instance used by platform functions.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_max30101_set_i2c_instance(mikroe_i2c_handle_t i2c_instance);

/***************************************************************************//**
 * @brief
 *    This function executes default configuration for HeartRate4 click
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_default_cfg(void);

/***************************************************************************//**
 * @brief
 *  Check whether a special MAX30101 is present on the bus or not.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 ******************************************************************************/
sl_status_t mikroe_max30101_present(void);

/***************************************************************************//**
 * @brief
 *    Generic write function.
 *
 * @param[in] wr_addr
 *    Register address
 *
 * @param[in] wr_data
 *    Data to be written.
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_write_data(uint8_t wr_addr,
                                uint8_t wr_data);

/***************************************************************************//**
 * @brief
 *    Generic read function.
 *
 * @param[in] rd_addr
 *    Register address
 *
 * @return
 *    8-bit data from address specified register
 ******************************************************************************/
uint8_t mikroe_max30101_read_data(uint8_t rd_addr);

/***************************************************************************//**
 * @brief
 *    Multi read function.
 *
 * @param[in] rd_addr
 *    Register address
 *
 * @param[in] buffer
 *    Data buf to read.
 *
 * @param[in] cnt
 *    Number of bytes to read.
 *
 * @return
 *    8-bit data from address specified register
 ******************************************************************************/
sl_status_t mikroe_max30101_multi_read(uint8_t rd_addr,
                                       uint8_t *buffer,
                                       uint8_t cnt);

/***************************************************************************//**
 * @brief
 *    Set new value function.
 *
 * @param[in] reg
 *    8-bit register address
 *
 * @param[in] mask
 *    8-bit value used as a mask
 *
 * @param[in] value
 *    8-bit data to be written into register
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_set_new_value(uint8_t reg, uint8_t mask, uint8_t value);

/***************************************************************************//**
 * @brief
 *    Function is used to read desired interrupt specified by flag.
 *
 * @note Use datasheet to discern what value from register represents.
 *
 * @param[in] flag
 *    1 - Get the main interrupt group; 2 - Get the temporary ready interrupt
 *
 * @return
 *    8-bit data representing the desired interrupt.
 ******************************************************************************/
uint8_t mikroe_max30101_get_intrrupt(uint8_t flag);

/***************************************************************************//**
 * @brief
 *    Function is used to enable or disable specified interupt.
 *
 * @note
 * <pre>
 *  interrupt_flag state:
 *        0 - A full
 *        1 - FIFO data ready interrupt
 *        2 - Ambient Light Cancellation Overflow
 *        3 - Proximity Threshold
 *        4 - Temperature conversion interrupt
 *  enable_flag state:
 *         1 - enable;
 *         0 - disable;
 *  One may consult datasheet in order to discern what to write into register or
 *  use predefined values.
 *  </pre>
 * Consult datasheet in order to discern what to write into register or use
 * predefined values.
 *
 * @param[in] interrupt_flag
 *    Flag that specifie interrupt
 *
 * @param[in] enable_flag
 *    1 - enable; 0 - disable
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_enable_disable_interrupts(uint8_t interrupt_flag,
                                               uint8_t enable_flag);

/***************************************************************************//**
 * @brief
 *    Function is used to write specified mode into corresponding configuration
 *    register.
 *
 * @note
 * <pre>
 * flag state:
 *       0 - Soft shut down
 *       1 - Soft wake up
 *       2 - Set LED Mode For Sampling
 *       3 - Set ADC Range
 *       4 - Set sample rate
 *       5 - Set pulse width
 * One may consult datasheet in order to discern what to write into register or
 *   use predefined values.
 * </pre>
 *
 * @param[in] flag
 *    Flag that specifie interrupt
 *
 * @param[in] mode
 *    8-bit data representing desired mode
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_setting_function(uint8_t flag, uint8_t mode);

/***************************************************************************//**
 * @brief
 *    Function is used to set specified amplitude or proximity threshold.
 *
 * @note
 * <pre>
 * flag state:
 *     0 - Set RED LED Pulse Amplitude
 *     1 - Set IR LED Pulse Amplitude
 *     2 - Set GREEN LED Pulse Amplitude
 *     3 - Set Proximity Pulse Amplitude
 *     4 - Set Proximity Threshold
 * One may consult datasheet in order to discern what to write into register or
 *   use predefined values.
 * </pre>
 *
 * @param[in] flag
 *    Flag that specifie amplitude or proximity threshold
 *
 * @param[in] write_data
 *    8-bit data representing amplitude or the proximity mode interrupt
 *    threshold
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_setting_prox_and_amp(uint8_t flag, uint8_t write_data);

/***************************************************************************//**
 * @brief
 *    Function is used to determine which LED is active in each time slot.
 *
 * @param[in] slot_num
 *    8-bit data representing the slot number
 *
 * @param[in] dev
 *    8-bit data representing the device(LED)
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_enable_slot(uint8_t slot_num, uint8_t dev);

/***************************************************************************//**
 * @brief
 *    Function is used to disable all LED-s in each time slot.
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_disable_slots(void);

/***************************************************************************//**
 * @brief
 *    Function is used to clear FIFO pointers.t.
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_clear_fifo(void);

/***************************************************************************//**
 * @brief
 *    Function is used to set number of samples by writning into FIFO
 *    configuration register. Or to enable or disable FIFO Rolls on Full
 *
 * @note
 * <pre>
 * flag state:
 *     0 - Disable roll over
 *     1 - Enable roll over
 *     2 - Set sample average
 *     3 - Set number of trigger samples
 * One may consult datasheet in order to discern what to write into register or
 *   use predefined values.
 * </pre>
 *
 * @param[in] flag
 *    Flag that specifie amplitude or proximity threshold
 *
 * @param[in] samp_num
 *    8-bit data representing the number of samples
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_fifo_setting(uint8_t flag, uint8_t samp_num);

/***************************************************************************//**
 * @brief
 *    Function is used to check write pointer.
 *
 * @note
 *    The write pointer increments every time a new sample is added to the FIFO.
 *
 * @return
 *    result 8-bit data that points to the location where the MAX30105 writes
 *    the next sample
 ******************************************************************************/
uint8_t mikroe_max30101_get_write_ptr(void);

/***************************************************************************//**
 * @brief
 *    Function is used to check read pointer.
 *
 * @note
 *    The read pointer is incremented every time a sample is read from the FIFO.
 *
 * @return
 *    result 8-bit data that points to the location from where the processor
 *    gets the next sample from the FIFO
 ******************************************************************************/
uint8_t mikroe_max30101_get_read_ptr(void);

/***************************************************************************//**
 * @brief
 *    Read temperature in Centigrade function
 *
 * @return
 *    result float value of temperature in Celsius
 ******************************************************************************/
float mikroe_max30101_read_temp_c(void);

/***************************************************************************//**
 * @brief
 *    Read temperature in Fahrenheit function
 *
 * @return
 *    result float value of temperature in Fahrenheit
 ******************************************************************************/
float mikroe_max30101_read_temp_f(void);

/***************************************************************************//**
 * @brief
 *    Function is used to apply software reset.
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_reset(void);

/***************************************************************************//**
 * @brief
 *    Function is used to check the state of INT pin.
 *
 * @return
 *    State of interrupt pin.
 ******************************************************************************/
uint8_t mikroe_max30101_check_int(void);

/***************************************************************************//**
 * @brief
 *    Read oldest FIFO sample functionn.
 *
 * @description
 *    Function is used to read the oldest RED value from FIFO.
 *
 * @return
 *    32-bit value representing the oldest RED value
 ******************************************************************************/
uint32_t mikroe_max30101_read_fifo_oldest_sample(void);

/***************************************************************************//**
 * @brief
 *    Function is used to check if desired interrupt, power ready or internal
 *    temperature ready flag has occured
 *
 * @note
 * <pre>
 * flag state:
 *      0 - Check FIFO full interrupt
 *      1 - Check FIFO data readdy interrupt
 *      2 - Check ambient light cancellation overflow interrupt
 *      3 - Check proximity threshold triggered interrupt
 *      4 - Check power ready flag
 *      5 - Check internal temperature ready flag
 * </pre>
 *
 * @param[in] flag
 *    Flag that specifie interrupt, power ready or internal temperature
 *
 * @return
 *    1 - Interrupt has not occured; 0 - Interrupt has occured
 ******************************************************************************/
uint32_t mikroe_max30101_check_int_pow_and_temp(uint8_t flag);

/***************************************************************************//**
 * @brief
 *    Read all FIFO samples function. Function is used to read all FIFO data
 *    samples and returns average value.
 *
 * @param[in] buff
 *    output buffer
 *
 * @return
 *    16-bit data representing the number of samples to read
 ******************************************************************************/
uint16_t mikroe_max30101_read_fifo_all_samples(uint8_t *buff);

/***************************************************************************//**
 * @brief
 *    Get Red value function. Function is used to read the oldest RED value.
 *
 * @param[in] buff
 *    output buffer
 *
 * @return
 *    result 32-bit value representing the oldest RED value
 ******************************************************************************/
uint32_t mikroe_max30101_get_red_val(void);

/***************************************************************************//**
 * @brief
 *    Set registers values function
 *
 * @note
 *    Consult datasheet in order to discern what to write into register or use
 *    predefined values.
 * <pre>
 * Registers structure fields
 *      avg_samp      -   8-bit data representing the number of samples
 *      mode          -   8-bit data representing led mode
 *      adc_range     -   8-bit data representing ADC range
 *      samp_rate     -   8-bit data representing sample rate
 *      pulse_width   -   8-bit data representing the puls width
 *      led_pow_lvl   -   8-bit data representing the power level
 *</pre>
 *
 * @param[in] registers
 *    Click registers object.
 *
 * @return
 *    None
 ******************************************************************************/
void mikroe_max30101_set_registers(mikroe_max30101_set_registers_t *registers);

#ifdef __cplusplus
}
#endif

/** @} */
#endif /* MIKROE_MAX30101_H_ */
