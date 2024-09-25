/***************************************************************************//**
 * @file mirkroe_ds3231m.c
 * @brief SCL DS3231M Source code file.
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
#include "mikroe_ds3231m.h"
#include "mikroe_ds3231m_config.h"

static rtc10_cfg_t rtc_cfg;
static rtc10_t rtc_ctx;
static bool initialized = false;

/**************************************************************************//**
*  RTC clicks initialization.
******************************************************************************/
sl_status_t mikroe_ds3231m_init(mikroe_i2c_handle_t i2cspm_instance)
{
  if (i2cspm_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  if (initialized) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }
  rtc_ctx.i2c.handle = i2cspm_instance;

  rtc10_cfg_setup(&rtc_cfg);
  rtc_cfg.rst = hal_gpio_pin_name(MIKROE_DS3231M_RST_PORT,
                                  MIKROE_DS3231M_RST_PIN);
  rtc_cfg.an = hal_gpio_pin_name(MIKROE_DS3231M_AN_PORT,
                                 MIKROE_DS3231M_AN_PIN);
  rtc_cfg.int_pin = hal_gpio_pin_name(MIKROE_DS3231M_INT_PORT,
                                      MIKROE_DS3231M_INT_PIN);

#if (MIKROE_DS3231M_I2C_UC == 1)
  rtc_cfg.i2c_speed = MIKROE_DS3231M_I2C_SPEED_MODE;
#endif

  if (RTC10_OK != rtc10_init(&rtc_ctx, &rtc_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }
  initialized = true;

  return SL_STATUS_OK;
}

/**************************************************************************//**
*  Set I2C instance.
******************************************************************************/
sl_status_t mikroe_ds3231m_set_i2c_instance(mikroe_i2c_handle_t i2c_instance)
{
  if (NULL == i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  rtc_ctx.i2c.handle = i2c_instance;

  return SL_STATUS_OK;
}

/**************************************************************************//**
*  RTC clicks default configuration.
******************************************************************************/
sl_status_t mikroe_ds3231m_default_cfg(void)
{
  rtc10_default_cfg(&rtc_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  RTC clicks generic write.
******************************************************************************/
sl_status_t mikroe_ds3231m_generic_write(uint8_t reg,
                                         uint8_t *data_buf,
                                         uint8_t len)
{
  if (data_buf == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  rtc10_generic_write(&rtc_ctx, reg, data_buf, len);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  RTC clicks generic read.
******************************************************************************/
sl_status_t mikroe_ds3231m_generic_read(uint8_t reg,
                                        uint8_t *data_buf,
                                        uint8_t len)
{
  if (data_buf == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  rtc10_generic_read(&rtc_ctx, reg, data_buf, len);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  RTC clicks hardware reset.
******************************************************************************/
sl_status_t mikroe_ds3231m_hw_reset(void)
{
  rtc10_hw_reset(&rtc_ctx);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  RTC clicks check interrupt.
******************************************************************************/
uint8_t mikroe_ds3231m_check_interrupt(void)
{
  return rtc10_check_interrupt(&rtc_ctx);
}

/**************************************************************************//**
*  RTC clicks read byte.
******************************************************************************/
uint8_t mikroe_ds3231m_read_byte(uint8_t reg_address)
{
  return rtc10_read_byte(&rtc_ctx, reg_address);
}

/**************************************************************************//**
*  RTC clicks write byte.
******************************************************************************/
sl_status_t mikroe_ds3231m_write_byte(uint8_t reg_address, uint8_t write_data)
{
  rtc10_write_byte(&rtc_ctx, reg_address, write_data);
  return SL_STATUS_OK;
}

/**************************************************************************//**
*  RTC clicks get time seconds.
******************************************************************************/
uint8_t mikroe_ds3231m_get_time_seconds(void)
{
  return rtc10_get_time_seconds(&rtc_ctx);
}

/**************************************************************************//**
*  RTC clicks set time seconds.
******************************************************************************/
sl_status_t mikroe_ds3231m_set_time_seconds(uint8_t seconds)
{
  rtc10_set_time_seconds(&rtc_ctx, seconds);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get time minutes function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_time_minutes(void)
{
  return rtc10_get_time_minutes(&rtc_ctx);
}

/***************************************************************************//**
 *  Set time minutes function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_time_minutes(uint8_t minutes)
{
  rtc10_set_time_minutes(&rtc_ctx, minutes);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get century flag function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_century_flag(void)
{
  return rtc10_get_century_flag(&rtc_ctx);
}

/***************************************************************************//**
 *  Set century flag function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_century_flag(void)
{
  rtc10_set_century_flag(&rtc_ctx);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get time hours function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_time_hours(void)
{
  return rtc10_get_time_hours(&rtc_ctx);
}

/***************************************************************************//**
 *  Set time hours function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_time_hours(uint8_t hours)
{
  rtc10_set_time_hours(&rtc_ctx, hours);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get day of the week function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_day_of_the_week(void)
{
  return rtc10_get_day_of_the_week(&rtc_ctx);
}

/***************************************************************************//**
 *  Set day of the week function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_day_of_the_week(uint8_t w_day)
{
  rtc10_set_day_of_the_week(&rtc_ctx, w_day);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get date day function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_date_day(void)
{
  return rtc10_get_date_day(&rtc_ctx);
}

/***************************************************************************//**
 *  Set date day function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_date_day(uint8_t date_day)
{
  rtc10_set_date_day(&rtc_ctx, date_day);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get date month function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_date_month(void)
{
  return rtc10_get_date_month(&rtc_ctx);
}

/***************************************************************************//**
 *  Set Date month day function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_date_month(uint8_t date_month)
{
  rtc10_set_date_month(&rtc_ctx, date_month);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get date year function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_date_year(void)
{
  return rtc10_get_date_year(&rtc_ctx);
}

/***************************************************************************//**
 *  Set Date year day function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_date_year(uint16_t date_year)
{
  rtc10_set_date_year(&rtc_ctx, date_year);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Set time: hours, minutes and seconds function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_time(uint8_t time_hours,
                                    uint8_t time_minutes,
                                    uint8_t time_seconds)
{
  rtc10_set_time(&rtc_ctx, time_hours, time_minutes, time_seconds);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get time: hours, minutes and seconds function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_get_time(uint8_t *time_hours,
                                    uint8_t *time_minutes,
                                    uint8_t *time_seconds)
{
  if ((time_hours == NULL) || (time_minutes == NULL)
      || (time_seconds == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }
  rtc10_get_time(&rtc_ctx, time_hours, time_minutes, time_seconds);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Set time: Date of the week, date day, date month and date year function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_date(uint8_t day_of_the_week,
                                    uint8_t date_day,
                                    uint8_t date_month,
                                    uint16_t date_year)
{
  rtc10_set_date(&rtc_ctx, day_of_the_week, date_day, date_month, date_year);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get time: Date of the week, date day, date month and date year function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_get_date(uint8_t *day_of_the_week,
                                    uint8_t *date_day,
                                    uint8_t *date_month,
                                    uint8_t *date_year)
{
  if ((day_of_the_week == NULL) || (date_day == NULL) || (date_month == NULL)
      || (date_year == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }
  rtc10_get_date(&rtc_ctx,
                 day_of_the_week,
                 date_day,
                 date_month,
                 date_year);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get alarm time seconds function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_alarm_time_seconds(void)
{
  return rtc10_get_alarm_time_seconds(&rtc_ctx);
}

/***************************************************************************//**
 *  Set alarm time seconds function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_alarm_time_seconds(uint8_t seconds)
{
  rtc10_set_alarm_time_seconds(&rtc_ctx, seconds);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get alarm time minutes function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_alarm_time_minutes(void)
{
  return rtc10_get_alarm_time_minutes(&rtc_ctx);
}

/***************************************************************************//**
 *  Set alarm time minutes function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_alarm_time_minutes(uint8_t minutes)
{
  rtc10_set_alarm_time_minutes(&rtc_ctx, minutes);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get alarm time hours function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_alarm_time_hours(void)
{
  return rtc10_get_alarm_time_hours(&rtc_ctx);
}

/***************************************************************************//**
 *  Set alarm time minutes function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_alarm_time_hours(uint8_t hours)
{
  rtc10_set_alarm_time_hours(&rtc_ctx, hours);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get alarm date day function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_alarm_date_day(void)
{
  return rtc10_get_alarm_date_day(&rtc_ctx);
}

/***************************************************************************//**
 *  Set alarm date days function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_alarm_date_day(uint8_t date_day)
{
  rtc10_set_alarm_date_day(&rtc_ctx, date_day);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Set Alarm 1 time: hours, minutes and seconds function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_first_alarm(uint8_t time_days,
                                           uint8_t time_hours,
                                           uint8_t time_minutes,
                                           uint8_t time_seconds)
{
  rtc10_set_first_alarm(&rtc_ctx,
                        time_days,
                        time_hours,
                        time_minutes,
                        time_seconds);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get Alarm 1 time: hours, minutes and seconds function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_get_first_alarm(uint8_t *time_days,
                                           uint8_t *time_hours,
                                           uint8_t *time_minutes,
                                           uint8_t *time_seconds)
{
  if ((time_days == NULL) || (time_hours == NULL) || (time_minutes == NULL)
      || (time_seconds == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }
  rtc10_get_first_alarm(&rtc_ctx,
                        time_days,
                        time_hours,
                        time_minutes,
                        time_seconds);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Set Alarm 2 time: hours, minutes and seconds function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_second_alarm(uint8_t time_days,
                                            uint8_t time_hours,
                                            uint8_t time_minutes)
{
  rtc10_set_second_alarm(&rtc_ctx, time_days, time_hours, time_minutes);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get Alarm 2 time: hours, minutes and seconds function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_get_second_alarm(uint8_t *time_days,
                                            uint8_t *time_hours,
                                            uint8_t *time_minutes)
{
  if ((time_days == NULL) || (time_hours == NULL) || (time_minutes == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }
  rtc10_get_second_alarm(&rtc_ctx, time_days, time_hours, time_minutes);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Enable counting function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_enable_counting(void)
{
  rtc10_enable_counting(&rtc_ctx);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Set configuration function.
 ******************************************************************************/
sl_status_t mikroe_ds3231m_set_config(uint8_t config_data)
{
  rtc10_set_config(&rtc_ctx, config_data);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Get configuration status function.
 ******************************************************************************/
uint8_t mikroe_ds3231m_get_status(void)
{
  return rtc10_get_status(&rtc_ctx);
}

/***************************************************************************//**
 *  Get temperature function.
 ******************************************************************************/
float mikroe_ds3231m_get_temperature(void)
{
  return rtc10_get_temperature(&rtc_ctx);
}
