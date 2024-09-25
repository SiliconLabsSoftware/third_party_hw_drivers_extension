/***************************************************************************//**
 * @file mikroe_ph2.c
 * @brief Mikroe Ph2 Source File
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
#include "mikroe_ph2.h"
#include "mikroe_ph2_config.h"
#include "ph2.h"

static ph2_t mikroe_ph2;
static ph2_cfg_t mikroe_ph2_cfg;

/***************************************************************************//**
 * @brief
 * Initialization function.
 *
 * @param[in] i2c_instance
 *    mikroe_i2c_handle_t instance
 *
 * @return
 *    SL_STATUS_OK Successful initialization.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null
 *    SL_STATUS_INITIALIZATION Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ph2_init(mikroe_i2c_handle_t i2c_instance)
{
  if (NULL == i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  mikroe_ph2.i2c.handle = i2c_instance;

  ph2_cfg_setup(&mikroe_ph2_cfg);

#if (MIKROE_PH2_I2C_UC == 1)
  mikroe_ph2_cfg.i2c_speed = MIKROE_PH2_I2C_SPEED_MODE;
#endif

#if defined(MIKROE_PH2_ST1_PORT) && defined(MIKROE_PH2_ST1_PIN)
  mikroe_ph2_cfg.st1 = hal_gpio_pin_name(MIKROE_PH2_ST1_PORT,
                                         MIKROE_PH2_ST1_PIN);
#endif

#if defined(MIKROE_PH2_ST2_PORT) && defined(MIKROE_PH2_ST2_PIN)
  mikroe_ph2_cfg.st2 = hal_gpio_pin_name(MIKROE_PH2_ST2_PORT,
                                         MIKROE_PH2_ST2_PIN);
#endif

#if defined(MIKROE_DS18B20_DQ_PORT) && defined(MIKROE_DS18B20_DQ_PIN)
  mikroe_ph2_cfg.dq = hal_gpio_pin_name(MIKROE_DS18B20_DQ_PORT,
                                        MIKROE_DS18B20_DQ_PIN);
#endif

  if (PH2_OK != ph2_init(&mikroe_ph2, &mikroe_ph2_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 * This function sets the mikroe_i2c_handle_t instance used by platform functions.
 *
 * @param[in] i2c_instance
 *    mikroe_i2c_handle_t instance
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if i2cspm_instance is null.
 ******************************************************************************/
sl_status_t mikroe_ph2_set_i2c_instance(mikroe_i2c_handle_t i2c_instance)
{
  if (NULL == i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  mikroe_ph2.i2c.handle = i2c_instance;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 * This function read raw ADC value function.
 *
 * @param[in] raw_adc
 *     Output ADC result
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_read_raw_adc(uint16_t *raw_adc)
{
  if (NULL == raw_adc) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (ph2_read_raw_adc(&mikroe_ph2,
                           raw_adc) == PH2_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function reads raw ADC value and converts it to proportional voltage
 * level.
 *
 * @param[in] voltage
 *     Output voltage level (volts).
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_read_voltage(float *voltage)
{
  if (NULL == voltage) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (ph2_read_voltage(&mikroe_ph2,
                           voltage) == PH2_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function sets the voltage reference for pH 2 click driver.
 *
 * @param[in] vref
 *     Reference voltage (volts).
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_set_vref(float vref)
{
  return (ph2_set_vref(&mikroe_ph2,
                       vref) == PH2_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function is performing calibration of Ph 2 click driver
 * with known pH value substance.
 *
 * @param[in] pH_value
 *     pH value of the substance where calibration is performed.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_calibrate(float pH_value)
{
  return (ph2_calibrate(&mikroe_ph2,
                        pH_value) == PH2_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function is reading pH value wof substance where pH probe is placed.
 *
 * @param[in] pH_value
 *     pH value of the substance.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 * @note This function calculates the pH value with the assumption that the
 * temperature of the substance is 25 deg Celsius.
 ******************************************************************************/
sl_status_t mikroe_ph2_calculate_ph(float *pH_value)
{
  if (NULL == pH_value) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (ph2_calculate_ph(&mikroe_ph2,
                           pH_value) == PH2_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function is used to calibrate offset of Ph 2 click driver.
 * @param[in]:
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 * @note Disconnect the probe and short-circuit the BNC connector in order to
 * set the reference voltage. Adjust the potentiometer until the LEDs stop
 * blinking or until the voltage of the OUT pin is exactly at half the value of
 * the selected VCC voltage. If you are using LEDs for calibration, turn the
 * potentiometer clockwise when STAT1 LED is blinking and counter-clockwise
 * when STAT2 LED is blinking.
 ******************************************************************************/
sl_status_t mikroe_ph2_calibrate_offset(void)
{
  ph2_calibrate_offset(&mikroe_ph2);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 * This function is used turn on/off STAT1 LED of Ph 2 click driver.
 * @param[in] state
 *     TRUE: turn on
 *     FALSE: turn off
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_enable_st1(bool state)
{
  if (state) {
    ph2_enable_st1(&mikroe_ph2);
  } else {
    ph2_disable_st1(&mikroe_ph2);
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 * This function is used turn on/off STAT2 LED of Ph 2 click driver.
 * @param[in] state
 *     TRUE: turn on
 *     FALSE: turn off
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ph2_enable_st2(bool state)
{
  if (state) {
    ph2_enable_st2(&mikroe_ph2);
  } else {
    ph2_disable_st2(&mikroe_ph2);
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 * This function initializes all necessary pins and peripherals used
 * for DS18B20 sensor.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_INITIALIZATION: Initialization failed.
 ******************************************************************************/
sl_status_t mikroe_ds18b20_init(void)
{
  return (ph2_ds18b20_init(&mikroe_ph2,
                           &mikroe_ph2_cfg) == PH2_OK)
         ? SL_STATUS_OK : SL_STATUS_INITIALIZATION;
}

/***************************************************************************//**
 * @brief
 * This function checks the communication by reading the ROM address
 * as well as verifying family code and Maxim CRC 8.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_check_communication(void)
{
  return (ph2_ds18b20_check_communication(&mikroe_ph2) == PH2_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function selects the device one OneWirebus
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_select_device(void)
{
  return (ph2_ds18b20_select_device(&mikroe_ph2) == PH2_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function reads a desired number of scratchpad bytes using One Wire
 * communication protocol.
 * @param[out] scratchpad
 *             Output read scratchpad.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_read_scratchpad(uint8_t *scratchpad)
{
  if (NULL == scratchpad) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (ph2_ds18b20_read_scratchpad(&mikroe_ph2, scratchpad) == PH2_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function writes the temperature thresholds and configuration byte to
 * the scratchpad.
 * @param[in] temp_high : Temperature high limit.
 * @param[in] temp_low : Temperature low limit.
 * @param[in] config : Configuration byte.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_write_scratchpad(uint8_t temp_high,
                                            uint8_t temp_low,
                                            uint8_t config)
{
  return (ph2_ds18b20_write_scratchpad(&mikroe_ph2,
                                       temp_high,
                                       temp_low,
                                       config) == PH2_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function executes a default configuration of pH 2 DS18B20 sensor.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_default_cfg(void)
{
  return (ph2_ds18b20_default_cfg(&mikroe_ph2) == PH2_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function starts the measurement by sending an appropriate command for
 * that.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_start_measurement(void)
{
  return (ph2_ds18b20_start_measurement(&mikroe_ph2) == PH2_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 * This function reads the temperature value in Celsius.
 * @param[out] temperature : Temperature in Celsius.
 * @return
 *    SL_STATUS_OK: Success
 *    SL_STATUS_FAIL: Error
 ******************************************************************************/
sl_status_t mikroe_ds18b20_read_temperature(float *temperature)
{
  if (NULL == temperature) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (ph2_ds18b20_read_temperature(&mikroe_ph2, temperature) == PH2_OK)
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}
