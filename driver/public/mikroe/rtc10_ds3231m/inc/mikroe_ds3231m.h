/***************************************************************************//**
 * @file mirkroe_ds3231m.h
 * @brief SCL DS3231M Prototypes
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

#ifndef MIKROE_DS3231M_H_
#define MIKROE_DS3231M_H_

#include "sl_status.h"
#include "drv_i2c_master.h"
#include "rtc10.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief
 *    Initialization function.
 *
 * @param[in] i2cspm_instance
 *    I2CSPM instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 *    SL_STATUS_FAIL Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_init(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    Set I2C instance function.
 *
 * @param[in] i2c_instance
 *    I2C instance
 *
 * @return
 *    SL_STATUS_OK Successful.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_i2c_instance(mikroe_i2c_handle_t i2c_instance);

/***************************************************************************//**
 * @brief
 *    Click Default Configuration function.
 *
 * @return
 *    SL_STATUS_OK Successful.
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_default_cfg(void);

/***************************************************************************//**
 * @brief
 *    Generic write function.
 *
 * @param[in] reg
 *    Register address.
 * @param[in] data_buf
 *    Data buf to be written.
 * @param[in] len
 *    Number of the bytes in data buf.
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_generic_write(uint8_t reg,
                                         uint8_t *data_buf,
                                         uint8_t len);

/***************************************************************************//**
 * @brief
 *    Generic read function.
 *
 * @param[in] reg
 *    Register address.
 * @param[out] data_buf
 *    Output data buf.
 * @param[in] len
 *    Number of the bytes to be read.
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_generic_read(uint8_t reg,
                                        uint8_t *data_buf,
                                        uint8_t len);

/***************************************************************************//**
 * @brief
 *    Hardware reset function.
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_hw_reset(void);

/***************************************************************************//**
 * @brief
 *    Check the state of interrupt function.
 *
 * @return
 *    Status of interrupt.
 ******************************************************************************/
uint8_t mikroe_ds3231m_check_interrupt(void);

/***************************************************************************//**
 * @brief
 *    Generic read function.
 *
 * @param[in] reg_address
 *    8-bit register address.
 *
 * @return
 *    Read data value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_read_byte(uint8_t reg_address);

/***************************************************************************//**
 * @brief
 *    Generic write function.
 *
 * @param[in] reg_address
 *    8-bit register address.
 * @param[in] write_data
 *    8-bit data to write.
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_write_byte(uint8_t reg_address, uint8_t write_data);

/***************************************************************************//**
 * @brief
 *    Get time seconds function.
 *
 * @return
 *    Time seconds value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_time_seconds(void);

/***************************************************************************//**
 * @brief
 *    Set time seconds function.
 *
 * @param[in] seconds
 *    time seconds [ 0 sec : 59 sec ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_time_seconds(uint8_t seconds);

/***************************************************************************//**
 * @brief
 *    Get time minutes function.
 *
 * @return
 *    Time minutes value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_time_minutes(void);

/***************************************************************************//**
 * @brief
 *    Set time minutes function.
 *
 * @param[in] minutes
 *    time minutes [ 0 min : 59 min ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_time_minutes(uint8_t minutes);

/***************************************************************************//**
 * @brief
 *    Get century flag function.
 *
 * @return
 *    Century flag value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_century_flag(void);

/***************************************************************************//**
 * @brief
 *    Set century flag function.
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_century_flag(void);

/***************************************************************************//**
 * @brief
 *    Get time hours function.
 *
 * @return
 *    Time hours value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_time_hours(void);

/***************************************************************************//**
 * @brief
 *    Set time hours function.
 *
 * @param[in] hours
 *    time hours [ 0 hrs : 23 hrs ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_time_hours(uint8_t hours);

/***************************************************************************//**
 * @brief
 *    Get day of the week function.
 *
 * @return
 *    Day of the week value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_day_of_the_week(void);

/***************************************************************************//**
 * @brief
 *    Set day of the week function.
 *
 * @param[in] w_day
 *    Day of the week data [ 1 : 7 ].
 *    - 1 : Monday;
 *    - 2 : Tuesday;
 *    - 3 : Wednesday;
 *    - 4 : Thursday;
 *    - 5 : Friday;
 *    - 6 : Saturday;
 *    - 7 : Sunday;
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_day_of_the_week(uint8_t w_day);

/***************************************************************************//**
 * @brief
 *    Get date day function.
 *
 * @return
 *    Date day value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_date_day(void);

/***************************************************************************//**
 * @brief
 *    Set date day function.
 *
 * @param[in] date_day
 *    Date day data [ 1 : 31 ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_date_day(uint8_t date_day);

/***************************************************************************//**
 * @brief
 *    Get date month function.
 *
 * @return
 *    Date month value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_date_month(void);

/***************************************************************************//**
 * @brief
 *    Set Date month day function.
 *
 * @param[in] date_month
 *    Date month data [ 1 : 12 ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_date_month(uint8_t date_month);

/***************************************************************************//**
 * @brief
 *    Get date year function.
 *
 * @return
 *    Date year value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_date_year(void);

/***************************************************************************//**
 * @brief
 *    Set Date year day function.
 *
 * @param[in] date_year
 *    Date year data [ xx00 : xx99 ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_date_year(uint16_t date_year);

/***************************************************************************//**
 * @brief
 *    Set time: hours, minutes and seconds function.
 *
 * @param[in] time_minutes
 *    Time minutes [ 0 min : 59 min ].
 * @param[in] time_hours
 *    Time hours [ 0 h : 23 h ]
 * @param[in] time_seconds
 *    Time seconds [ 0 sec : 59 sec ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_time(uint8_t time_hours,
                                    uint8_t time_minutes,
                                    uint8_t time_seconds);

/***************************************************************************//**
 * @brief
 *    Get time: hours, minutes and seconds function.
 *
 * @param[in] time_minutes
 *    Pointer of time minutes [ 0 min : 59 min ].
 * @param[in] time_hours
 *    Pointer of time hours [ 0 h : 23 h ]
 * @param[in] time_seconds
 *    Pointer of time seconds [ 0 sec : 59 sec ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_get_time (uint8_t *time_hours,
                                     uint8_t *time_minutes,
                                     uint8_t *time_seconds);

/***************************************************************************//**
 * @brief
 *    Set time: Date of the week, date day, date month and date year function.
 *
 * @param[in] day_of_the_week
 *    Date of the week data [ 1 : 7 ].
 * @param[in] date_day
 *    Date day data [ 1 : 31 ]
 * @param[in] date_month
 *    Date month data [ 1 : 12 ].
 * @param[in] date_year
 *    Date year data [ 0 : 99 ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_date(uint8_t day_of_the_week,
                                    uint8_t date_day,
                                    uint8_t date_month,
                                    uint16_t date_year);

/***************************************************************************//**
 * @brief
 *    Get time: Date of the week, date day, date month and date year function.
 *
 * @param[in] day_of_the_week
 *    Pointer of date of the week data [ 1 : 7 ].
 * @param[in] date_day
 *    Pointer of date day data [ 1 : 31 ]
 * @param[in] date_month
 *    Pointer of date month data [ 1 : 12 ].
 * @param[in] date_year
 *    Pointer of date year data [ 0 : 99 ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_get_date(uint8_t *day_of_the_week,
                                    uint8_t *date_day,
                                    uint8_t *date_month,
                                    uint8_t *date_year);

/***************************************************************************//**
 * @brief
 *    Get alarm time seconds function.
 *
 * @return
 *    Alarm time seconds value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_alarm_time_seconds(void);

/***************************************************************************//**
 * @brief
 *    Set alarm time seconds function.
 *
 * @param[in] seconds
 *    Alarm time seconds [ 0 sec : 59 sec ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_alarm_time_seconds(uint8_t seconds);

/***************************************************************************//**
 * @brief
 *    Get alarm time minutes function.
 *
 * @return
 *    Alarm time minutes value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_alarm_time_minutes(void);

/***************************************************************************//**
 * @brief
 *    Set alarm time minutes function.
 *
 * @param[in] minutes
 *    Alarm time minutes [ 0 min : 59 min ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_alarm_time_minutes(uint8_t minutes);

/***************************************************************************//**
 * @brief
 *    Get alarm time hours function.
 *
 * @return
 *    Alarm time hours value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_alarm_time_hours(void);

/***************************************************************************//**
 * @brief
 *    Set alarm time minutes function.
 *
 * @param[in] hours
 *    Alarm time minutes [ 0 h : 23 h ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_alarm_time_hours(uint8_t hours);

/***************************************************************************//**
 * @brief
 *    Get alarm date day function.
 *
 * @return
 *    Alarm date day value.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_alarm_date_day(void);

/***************************************************************************//**
 * @brief
 *    Set alarm date days function.
 *
 * @param[in] date_day
 *    Date days data [ 1 : 31 ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_alarm_date_day(uint8_t date_day);

/***************************************************************************//**
 * @brief
 *    Set Alarm 1 time: hours, minutes and seconds function.
 *
 * @param[in] time_days
 *    Time days data [ 1 : 31 ].
 * @param[in] time_hours
 *    Time hours [ 0 h : 23 h ].
 * @param[in] time_minutes
 *    Time minutes [ 0 min : 59 min ].
 * @param[in] time_seconds
 *    Time seconds [ 0 sec : 59 sec ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_first_alarm(uint8_t time_days,
                                           uint8_t time_hours,
                                           uint8_t time_minutes,
                                           uint8_t time_seconds);

/***************************************************************************//**
 * @brief
 *    Get Alarm 1 time: hours, minutes and seconds function.
 *
 * @param[in] time_days
 *    Pointer of time days data [ 1 : 31 ].
 * @param[in] time_hours
 *    Pointer of time hours [ 0 h : 23 h ].
 * @param[in] time_minutes
 *    Pointer of time minutes [ 0 min : 59 min ].
 * @param[in] time_seconds
 *    Pointer of time seconds [ 0 sec : 59 sec ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_get_first_alarm(uint8_t *time_days,
                                           uint8_t *time_hours,
                                           uint8_t *time_minutes,
                                           uint8_t *time_seconds);

/***************************************************************************//**
 * @brief
 *    Set Alarm 2 time: hours, minutes and seconds function.
 *
 * @param[in] time_days
 *    Time days [ 1 : 31 ].
 * @param[in] time_hours
 *    Time hours [ 0 h : 23 h ].
 * @param[in] time_minutes
 *    Time minutes [ 0 min : 59 min ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_second_alarm(uint8_t time_days,
                                            uint8_t time_hours,
                                            uint8_t time_minutes);

/***************************************************************************//**
 * @brief
 *    Get Alarm 2 time: hours, minutes and seconds function.
 *
 * @param[in] time_days
 *    Pointer of time days [ 1 : 31 ].
 * @param[in] time_hours
 *    Pointer of time hours [ 0 h : 23 h ].
 * @param[in] time_minutes
 *    Pointer of time minutes [ 0 min : 59 min ].
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_get_second_alarm(uint8_t *time_days,
                                            uint8_t *time_hours,
                                            uint8_t *time_minutes);

/***************************************************************************//**
 * @brief
 *    Enable counting function.
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_enable_counting(void);

/***************************************************************************//**
 * @brief
 *    Set configuration function.
 *
 * @param[in] config_data
 *    - bit [ 7 ]     : Enable oscillator.
 *                   When set to logic 0, the oscillator is started. When set
 *                   to logic 1, the oscillator is stopped when the device
 *                   switches to VBAT. This bit is clear (logic 0) when power
 *                   is first applied. When the device is powered by VCC, the
 *                   oscillator is always on regardless of the status of the
 *                   EOSC bit. When the oscillator is disabled, all register
 *                   data is static
 *    - bit [ 6 ]     : Battery-backed square-wave enable.
 *                   When set to logic 1 with INTCN = 0 and VCC < VPF, this
 *                   bit enables the 1Hz square wave. When BBSQW is logic 0,
 *                   INT/SQW goes high impedance when VCC falls below VPF.
 *                   This bit is disabled (logic 0) when power is first applied.
 *    - bit [ 5 ]     : Convert temperature.
 *                   Setting this bit to 1 forces the temperature sensor to
 *                   convert the temperature into digital code and execute the
 *                   temperature compensate algorithm to update the oscillators
 *                   accuracy. The device cannot be forced to execute the
 *                   temperature-compensate algorithm faster than once per
 *                   second. A user-initiated temperature conversion does not
 *                   affect the internal update cycle. The CONV bit remains at
 *                   a 1 from the time it is written until the temperature
 *                   conversion is completed, at which time both CONV and BSY
 *                   go to 0. The CONV bit should be used when monitoring the
 *                   status of a user-initiated conversion. See Figure 7 for
 *                   more details.
 *    - bit [ 4 : 3 ] : Not applicable.
 *                   These bits have no affect on the device and can be set to
 *                   either 0 or 1.
 *    - bit [ 2 ]     : Interrupt control.
 *                   This bit controls the INT/SQW output signal. When the
 *                   INTCN bit is set to logic 0, a 1Hz square wave is output
 *                   on INT/SQW. When the INTCN bit is set to logic 1, a match
 *                   between the timekeeping registers and either of the alarm
 *                   registers activates the INT/SQW output (if the alarm is
 *                   also enabled). The corresponding alarm flag is always set
 *                   regardless of the state of the INTCN bit. The INTCN bit
 *                   is set to a logic 1 when power is first applied.
 *    - bit [ 1 ]     : Alarm 2 interrupt enable.
 *                   When set to logic 1, this bit permits the alarm 2 flag
 *                   (A2F) bit in the status register to assert
 *                   INT/SQW (when INTCN = 1). When the A2IE bit is set to
 *                   logic 0 or INTCN is set to logic 0, the A2F bit does not
 *                   initiate an interrupt signal. The A2IE bit is disabled
 *                   (logic 0) when power is first applied.
 *    - bit [ 0 ]     : Alarm 1 interrupt enable.
 *                   When set to logic 1, this bit permits the alarm 1 flag
 *                   (A1F) bit in the status register to assert INT/SQW
 *                   (when INTCN = 1). When the A1IE bit is set to logic 0 or
 *                   INTCN is set to logic 0, the A1F bit does not initiate an
 *                   interrupt signal. The A1IE bit is disabled (logic 0) when
 *                   power is first applied.
 *
 * @return
 *    SL_STATUS_OK Successful
 *    SL_STATUS_FAIL Failed.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_config(uint8_t config_data);

/***************************************************************************//**
 * @brief Get configuration status function.
 *
 * @param[out] config_data
 *    - bit [ 7 ]     : EOscillator stop flag.
 *                   A logic 1 in this bit indicates that the oscillator either
 *                   is stopped or was stopped for some period and could be
 *                   used to judge the validity of the timekeeping data. This
 *                   bit is set to logic 1 any time that the oscillator stops.
 *                   This bit remains at logic 1 until written to logic 0. The
 *                   following are examples of conditions that can cause the
 *                   OSF bit to be set:
 *                       - 1 : The first time power is applied;
 *                       - 2 : The voltages present on both VCC and VBAT are
 *                             insufficient to support the oscillator;
 *                       - 3 : The EOSC bit is turned off in battery-backed
 *                             mode;
 *                       - 4 : External influences on the oscillator
 *                             (i.e., noise, leakage, etc.).
 *    - bit [ 6 : 4 ] : Unused ( 0 ).
 *                   These bits have no meaning and are fixed at 0 when read.
 *    - bit [ 3 ]     : Enabled 32.768kHz output.
 *                   This bit enables and disables the 32KHZ output. When set
 *                   to a logic 0, the 32KHZ output is high impedance. On
 *                   initial power-up, this bit is set to a logic 1 and the
 *                   32KHZ output is enabled and produces a 32.768kHz square
 *                   wave if the oscillator is enabled. These bits have no
 *                   affect on the device and can be set to either 0 or 1.
 *    - bit [ 2 ]     : Busy.
 *                   This bit indicates the device is busy executing
 *                   temperature conversion function. It goes to logic 1
 *                   when the conversion signal to the temperature sensor is
 *                   asserted, and then it is cleared when the device has
 *                   completed the temperature conversion.
 * - bit [ 1 ]     : Alarm 2 flag.
 *                   A logic 1 in the alarm 2 flag bit indicates that the time
 *                   matched the alarm 2 registers. If the A2IE bit is logic
 *                   1 and the INTCN bit is set to logic 1, INT/SQW is also
 *                   asserted. A2F is cleared when written to logic 0. This
 *                   bit can only be written to logic 0. Attempting to write
 *                   to logic 1 leaves the value unchanged.
 * - bit [ 0 ]     : Alarm 1 flag.
 *                   A logic 1 in the alarm 1 flag bit indicates that the time
 *                   matched the alarm 1 registers. If the A1IE bit is logic 1
 *                   and the INTCN bit is set to logic 1, INT/SQW is also
 *                   asserted. A1F is cleared when written to logic 0. This bit
 *                   can only be written to logic 0. Attempting to write to
 *                   logic 1 leaves the value unchanged. The A1IE bit is
 *                   disabled (logic 0) when power is first applied.
 *
 * @return
 *    Configuration satus.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_status(void);

/***************************************************************************//**
 * @brief
 *    Get temperature function.
 *
 * @return
 *    Temperature value.
 ******************************************************************************/
float mikroe_ds3231m_get_temperature(void);

#ifdef __cplusplus
extern "C"
}
#endif

#endif // MIKROE_DS3231M_H_
