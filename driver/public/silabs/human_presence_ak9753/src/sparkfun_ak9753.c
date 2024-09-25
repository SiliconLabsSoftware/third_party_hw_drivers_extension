/***************************************************************************//**
 * @file sparkfun_as7265x.c
 * @brief AS7265X Source file
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

#include <string.h>
#include "sl_sleeptimer.h"
#include "sparkfun_ak9753.h"
#include "sparkfun_ak9753_config.h"
#include "drv_digital_out.h"

#ifdef __cplusplus
extern "C" {
#endif

// instance for I2C communication
i2c_master_t ak9753_i2c;
pin_name_t pdn_name;
digital_out_t pdn_out;

/***************************************************************************//**
 * Local Variables
 ******************************************************************************/
static sparkfun_ak9753_config_t sparkfun_ak9753_cfg = {
  .I2C_address = SPARKFUN_AK9753_ADDR,
  .i2cspm_instance = NULL,
  .cut_off_freq = SPARKFUN_AK975X_FREQ_8_8HZ,
  .mode = SPARKFUN_AK975X_MODE_0,
  .upper_threshold13 = 0x00,
  .lower_threshold13 = 0x00,
  .upper_threshold24 = 0x00,
  .lower_threshold24 = 0x00,
  .hysteresis_value24 = 0x00,
  .hysteresis_value13 = 0x00,
  .int_source_setting = 0x00,
  .int_present = false,
  .PDN_present = false
};      /* Structure to hold AK9753 driver config */

/***************************************************************************//**
* Read register value
*******************************************************************************/
static sl_status_t sparkfun_ak9753_platform_read_register(uint8_t addr,
                                                          uint8_t *pdata)
{
  if ((pdata == NULL) || (ak9753_i2c.handle == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  } else {
    if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(&ak9753_i2c,
                                                         &addr,
                                                         1,
                                                         pdata,
                                                         1)) {
      return SL_STATUS_TRANSMIT;
    }
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Write into register
*******************************************************************************/
sl_status_t sparkfun_ak9753_platform_write_register(uint8_t addr,
                                                    uint8_t data)
{
  if (ak9753_i2c.handle == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  uint8_t i2c_write_data[2] = { addr, data };
  if (I2C_MASTER_SUCCESS != i2c_master_write(&ak9753_i2c,
                                             i2c_write_data,
                                             2)) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Read multiple bytes from sensor
*******************************************************************************/
sl_status_t sparkfun_ak9753_platform_read_blocking_register(uint8_t addr,
                                                            uint8_t *pdata,
                                                            uint8_t len)
{
  if ((pdata == NULL) || (ak9753_i2c.handle == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  } else {
    if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(&ak9753_i2c,
                                                         &addr,
                                                         1,
                                                         pdata,
                                                         len)) {
      return SL_STATUS_TRANSMIT;
    }
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Return the version information of the MMA8452Q.
*******************************************************************************/
void sparkfun_ak9753_get_core_version(
  sparkfun_ak9753_core_version_t *core_version)
{
  if (core_version != NULL) {
    core_version->major = SPARKFUN_AK9753_MAJOR_VERSION;
    core_version->build = SPARKFUN_AK9753_BUILD_VERSION;
    core_version->minor = SPARKFUN_AK9753_MINOR_VERSION;
    core_version->revision = SPARKFUN_AK9753_REVISION_VERSION;
  }
}

/***************************************************************************//**
* Set mode of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_mode(sparkfun_ak9753_mode_t mode)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t current_settings = 0;

  if (mode > SPARKFUN_AK975X_MODE_3) {
    mode = SPARKFUN_AK975X_MODE_0;                         // Default to mode 0
  }
  if (mode == 0b011) {
    mode = SPARKFUN_AK975X_MODE_0;                  // 0x03 is prohibited
  }
  // Read, mask set, write
  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ECNTL1,
                                               &current_settings);
  current_settings &= 0b11111000; // Clear Mode bits
  current_settings |= mode;
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1,
                                                current_settings);

  sparkfun_ak9753_cfg.mode = mode;

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Power down the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_power_down(void)
{
  if (sparkfun_ak9753_cfg.PDN_present) {
    digital_out_low(&pdn_out);
    return SL_STATUS_OK;
  }
  return SL_STATUS_NOT_INITIALIZED;
}

/***************************************************************************//**
* Power up the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_power_up(void)
{
  if (sparkfun_ak9753_cfg.PDN_present) {
    digital_out_high(&pdn_out);
    return SL_STATUS_OK;
  }
  return SL_STATUS_NOT_INITIALIZED;
}

/***************************************************************************//**
* Get mode of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_mode(sparkfun_ak9753_mode_t *mode)
{
  if (mode == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  *mode = sparkfun_ak9753_cfg.mode;

  return SL_STATUS_OK;
}

/***************************************************************************//**
* Set cut off frequency of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_cutoff_freq(sparkfun_ak9753_cutoff_freq_t fc)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t current_settings = 0;

  if (fc > 0b101) {
    fc = 0;                        // Default to 0.3Hz
  }
  // Read, mask set, write
  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ECNTL1,
                                               &current_settings);
  current_settings &= 0b11000111; // Clear EFC bits
  current_settings |= (fc << 3);  // Mask in
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1,
                                                current_settings);

  sparkfun_ak9753_cfg.cut_off_freq = fc;

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Get cut off frequency of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_cutoff_freq(uint8_t *fc)
{
  if (fc == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  *fc = sparkfun_ak9753_cfg.cut_off_freq;

  return SL_STATUS_OK;
}

/***************************************************************************//**
* Get the available status of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_is_data_ready(bool *data_ready)
{
  if (data_ready == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  uint8_t value = 0;
  sl_status_t sc = SL_STATUS_OK;

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ST1, &value);
  *data_ready = (value & (1 << 0)); // Bit 0 is DRDY

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Initialize the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_init(
  sparkfun_ak9753_config_t *sparkfun_ak9753_config)
{
  if (sparkfun_ak9753_config == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Update i2cspm instance
  ak9753_i2c.handle =
    sparkfun_ak9753_config->i2cspm_instance;

  i2c_master_config_t i2c_cfg;
  i2c_master_configure_default(&i2c_cfg);

  i2c_cfg.addr = sparkfun_ak9753_config->I2C_address;

#if (SPARKFUN_AK9753_I2C_UC == 1)
  i2c_cfg.speed = SPARKFUN_AK9753_I2C_SPEED_MODE;
#endif

#if (defined SPARKFUN_AK9753_CONFIG_PDN_PORT) \
  && (defined SPARKFUN_AK9753_CONFIG_PDN_PIN)
  pdn_name = hal_gpio_pin_name(SPARKFUN_AK9753_CONFIG_PDN_PORT,
                               SPARKFUN_AK9753_CONFIG_PDN_PIN);
  digital_out_init(&pdn_out, pdn_name);
  digital_out_high(&pdn_out);
  sparkfun_ak9753_config->PDN_present = true;
#endif

  if (i2c_master_open(&ak9753_i2c, &i2c_cfg)
      == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  i2c_master_set_speed(&ak9753_i2c, i2c_cfg.speed);
  i2c_master_set_timeout(&ak9753_i2c, 0);

  uint8_t device_id = 0;
  sl_status_t sc = SL_STATUS_OK;

  sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_WIA2, &device_id);

  if (device_id != 0x13) {
    return SL_STATUS_FAIL;
  }

  // Set to continuous read
  sc |= sparkfun_ak9753_set_mode(sparkfun_ak9753_config->mode);

  sc |= sparkfun_ak9753_set_cutoff_freq(sparkfun_ak9753_config->cut_off_freq);

  sc |= sparkfun_ak9753_set_hysteresis_ir13(
    sparkfun_ak9753_config->hysteresis_value13);
  sc |= sparkfun_ak9753_set_hysteresis_ir24(
    sparkfun_ak9753_config->hysteresis_value24);
  sc |= sparkfun_ak9753_set_threshold_ir13(true,
                                           sparkfun_ak9753_config->upper_threshold13);
  sc |= sparkfun_ak9753_set_threshold_ir13(false,
                                           sparkfun_ak9753_config->lower_threshold13);
  sc |= sparkfun_ak9753_set_threshold_ir24(true,
                                           sparkfun_ak9753_config->upper_threshold24);
  sc |= sparkfun_ak9753_set_threshold_ir24(false,
                                           sparkfun_ak9753_config->lower_threshold24);
  sc |= sparkfun_ak9753_set_interrupts(
    sparkfun_ak9753_config->int_source_setting);

  sparkfun_ak9753_cfg = *sparkfun_ak9753_config;

  sc |= sparkfun_ak9753_get_dummy();

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Deinitialize the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_deinit(void)
{
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Refresh the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_dummy(void)
{
  uint8_t dummy = 0;
  return sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ST2, &dummy);
  // Do nothing but read the register
}

/***************************************************************************//**
* Get the value IR1 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_ir1_data(int16_t *measurement_data)
{
  if (measurement_data == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint8_t temp[2] = { 0x00, 0x00 };

  sc = sparkfun_ak9753_platform_read_blocking_register(SPARKFUN_AK975X_IR1,
                                                       temp,
                                                       2);
  *measurement_data = temp[0] | (temp[1] << 8);

  return sc;
}

/***************************************************************************//**
* Get the value IR2 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_ir2_data(int16_t *measurement_data)
{
  if (measurement_data == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint8_t temp[2] = { 0x00, 0x00 };

  sc = sparkfun_ak9753_platform_read_blocking_register(SPARKFUN_AK975X_IR2,
                                                       temp,
                                                       2);
  *measurement_data = temp[0] | (temp[1] << 8);

  return sc;
}

/***************************************************************************//**
* Get the value IR3 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_ir3_data(int16_t *measurement_data)
{
  if (measurement_data == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint8_t temp[2] = { 0x00, 0x00 };

  sc = sparkfun_ak9753_platform_read_blocking_register(SPARKFUN_AK975X_IR3,
                                                       temp,
                                                       2);
  *measurement_data = temp[0] | (temp[1] << 8);

  return sc;
}

/***************************************************************************//**
* Get the value IR4 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_ir4_data(int16_t *measurement_data)
{
  if (measurement_data == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint8_t temp[2] = { 0x00, 0x00 };

  sc = sparkfun_ak9753_platform_read_blocking_register(SPARKFUN_AK975X_IR4,
                                                       temp,
                                                       2);
  *measurement_data = temp[0] | (temp[1] << 8);

  return sc;
}

/***************************************************************************//**
* Get the value temperature in raw of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_raw_temp(uint16_t *raw_temperature_data)
{
  if (raw_temperature_data == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint8_t value[2] = { 0x00, 0x00 };

  sc = sparkfun_ak9753_platform_read_blocking_register(SPARKFUN_AK975X_TMP,
                                                       value,
                                                       2);

  // Temp is 10-bit. TMPL0:5 fixed at 0
  *raw_temperature_data = ((int16_t)(value[0] | (value[1] << 8)) >> 6);

  return sc;
}

/***************************************************************************//**
* Get the value temperature in Celcius of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_tempC(float *tempC)
{
  if (tempC == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint16_t raw_temperature_data;

  sc = sparkfun_ak9753_get_raw_temp(&raw_temperature_data);

  *tempC = (int16_t)raw_temperature_data * 0.125 + 26.75;

  return sc;
}

/***************************************************************************//**
* Get the value temperature in F of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_tempF(float *tempF)
{
  if (tempF == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint16_t temperature = 0;

  sc = sparkfun_ak9753_get_raw_temp(&temperature);
  *tempF = (int16_t)temperature * 0.225 + 80.15;

  return sc;
}

/***************************************************************************//**
* Set the threshold Ir2-Ir4 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_threshold_ir24(bool height,
                                               uint16_t threshold_value)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t k = threshold_value >> 5; // high bits [11;5]
  uint8_t b = threshold_value << 3; // low bits

  b &= 0b11111000; // low bits
  k &= 0b01111111; // high bits
  if (height) {
    sc |=
      sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ETH24H_LOW, b);
    sc |=
      sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ETH24H_HIGH, k);
    sparkfun_ak9753_cfg.upper_threshold24 = threshold_value;
  } else {
    sc |=
      sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ETH24L_LOW, b);
    sc |=
      sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ETH24L_HIGH, k);
    sparkfun_ak9753_cfg.lower_threshold24 = threshold_value;
  }
  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Get the threshold Ir2-Ir4 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_threshold_ir24(bool height,
                                               uint16_t *threshold_value)
{
  if (threshold_value == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  if (threshold_value == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (height) {
    *threshold_value = sparkfun_ak9753_cfg.upper_threshold24;
  } else {
    *threshold_value = sparkfun_ak9753_cfg.lower_threshold24;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
* Set the threshold Ir2-Ir4 of the sensor to EEprom.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_threshold_eeprom_ir24(bool height,
                                                      uint16_t threshold_value)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t h = threshold_value >> 6; // high bits [11;6]
  uint8_t current_mode = 0;

  threshold_value &= 0b00111111; // low bits
  h &= 0b00111111; // high bits
  threshold_value |= 0b11000000; // mask
  h |= 0b11000000; // mask

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ECNTL1,
                                               &current_mode);
  uint8_t temp = SPARKFUN_AK975X_EEPROM_MODE | (current_mode & 0b00111000);
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1, temp);

  if (height) {
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EKEY,
                                                  SPARKFUN_AK975X_EKEY_ON);
    // low byte is at adresse 0x55 OR 0X57 11+[6:0] for ETH24H OR ETH24L
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EETH24H_LOW,
                                                  threshold_value);
    sl_sleeptimer_delay_millisecond(15);

    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EKEY,
                                                  SPARKFUN_AK975X_EKEY_ON);

    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EETH24H_HIGH,
                                                  h);
    sl_sleeptimer_delay_millisecond(15);
  } else {
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EKEY,
                                                  SPARKFUN_AK975X_EKEY_ON);

    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EETH24L_LOW,
                                                  threshold_value);
    sl_sleeptimer_delay_millisecond(15);

    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EKEY,
                                                  SPARKFUN_AK975X_EKEY_ON);

    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EETH24L_HIGH,
                                                  h);
    sl_sleeptimer_delay_millisecond(15);
  }
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1,
                                                current_mode);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Get the threshold Ir2-Ir4 of the sensor to EEprom.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_eeprom_threshold_ir24(bool height,
                                                      uint16_t *threshold_value)
{
  if (threshold_value == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint8_t current_mode = 0;

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ECNTL1,
                                               &current_mode);

  uint8_t temp = SPARKFUN_AK975X_EEPROM_MODE | (current_mode & 0b00111000);
  uint8_t threshold_high = 0;
  uint8_t threshold_low = 0;

  if (height) {
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1, temp);
    sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_EETH24H_LOW,
                                                 &threshold_low);
    sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_EETH24H_HIGH,
                                                 &threshold_high);
  } else {
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1, temp);
    sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_EETH24L_LOW,
                                                 &threshold_low);
    sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_EETH24L_HIGH,
                                                 &threshold_high);
  }

  *threshold_value =
    ((uint16_t)(threshold_high & 0b00111111) <<
      6) | (threshold_low & 0b00111111);

  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1,
                                                current_mode);

  return sc;
}

/***************************************************************************//**
* Set the threshold Ir1-Ir3 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_threshold_ir13(bool height,
                                               uint16_t threshold_value)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t k = threshold_value >> 5; // high bits [11;5]
  uint8_t b = threshold_value << 3; // low bits

  b &= 0b11111000; // low bits
  k &= 0b01111111; // high bits
  if (height) {
    sc |=
      sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ETH13H_LOW, b);
    sc |=
      sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ETH13H_HIGH, k);
    sparkfun_ak9753_cfg.upper_threshold13 = threshold_value;
  } else {
    sc |=
      sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ETH13L_LOW, b);
    sc |=
      sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ETH13L_HIGH, k);
    sparkfun_ak9753_cfg.lower_threshold13 = threshold_value;
  }

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Get the threshold Ir1-Ir3 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_threshold_ir13(bool height,
                                               uint16_t *threshold_value)
{
  if (threshold_value == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (height) {
    *threshold_value = sparkfun_ak9753_cfg.upper_threshold13;
  } else {
    *threshold_value = sparkfun_ak9753_cfg.lower_threshold13;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
* Set the threshold Ir1-Ir3 of the sensor to EEprom.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_threshold_eeprom_ir13(bool height,
                                                      uint16_t threshold_value)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t h = threshold_value >> 6; // high bits [11;6]
  uint8_t current_mode = 0;

  threshold_value &= 0b00111111; // low bits
  h &= 0b00111111; // high bits
  threshold_value |= 0b11000000; // mask
  h |= 0b11000000; // mask

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ECNTL1,
                                               &current_mode);
  uint8_t temp = SPARKFUN_AK975X_EEPROM_MODE | (current_mode & 0b00111000);
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1, temp);

  if (height) {
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EKEY,
                                                  SPARKFUN_AK975X_EKEY_ON);

    // low byte is at adresse 0x52 OR 0X54 11+[6:0] for ETH13H OR ETH13L
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EETH13H_LOW,
                                                  threshold_value);
    sl_sleeptimer_delay_millisecond(15);

    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EKEY,
                                                  SPARKFUN_AK975X_EKEY_ON);

    // high byte is at adresse 0x51 OR 0X53 11+[6:0] for ETH13H OR ETH13L
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EETH13H_HIGH,
                                                  h);
    sl_sleeptimer_delay_millisecond(15);
  } else {
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EKEY,
                                                  SPARKFUN_AK975X_EKEY_ON);

    // low byte is at address 0x52 OR 0X54 11+[6:0] for ETH13H OR ETH13L
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EETH13L_LOW,
                                                  threshold_value);
    sl_sleeptimer_delay_millisecond(15);

    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EKEY,
                                                  SPARKFUN_AK975X_EKEY_ON);

    // high byte is at adresse 0x51 OR 0X53 11+[6:0] for ETH13H OR ETH13L
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EETH13L_HIGH,
                                                  h);
    sl_sleeptimer_delay_millisecond(15);
  }

  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1,
                                                current_mode);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Get the threshold Ir1-Ir3 of the sensor to EEprom.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_eeprom_threshold_ir13(bool height,
                                                      uint16_t *threshold_value)
{
  if (threshold_value == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint8_t current_mode = 0;

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ECNTL1,
                                               &current_mode);

  uint8_t temp = SPARKFUN_AK975X_EEPROM_MODE | (current_mode & 0b00111000);
  uint8_t threshold_high = 0;
  uint8_t threshold_low = 0;

  if (height) {
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1, temp);
    sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_EETH13H_LOW,
                                                 &threshold_low);
    sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_EETH13H_HIGH,
                                                 &threshold_high);
  } else {
    sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1, temp);
    sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_EETH13L_LOW,
                                                 &threshold_low);
    sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_EETH13L_HIGH,
                                                 &threshold_high);
  }

  *threshold_value =
    ((uint16_t)(threshold_high & 0b00111111) <<
      6) | (threshold_low & 0b00111111);

  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1,
                                                current_mode);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Set hysteresis Ir2-Ir4 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_hysteresis_ir24(uint8_t hysteresis_value)
{
  // we keep the 5 last bits mask
  hysteresis_value &= 0b00011111;
  hysteresis_value |= 0b11100000;

  sparkfun_ak9753_cfg.hysteresis_value24 = hysteresis_value;

  return sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EHYS24,
                                                 hysteresis_value);
}

/***************************************************************************//**
* Get hysteresis Ir2-Ir4 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_hysteresis_ir24(uint8_t *hysteresis_value)
{
  if (hysteresis_value == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *hysteresis_value = sparkfun_ak9753_cfg.hysteresis_value24;

  return SL_STATUS_OK;
}

/***************************************************************************//**
* Set hysteresis Ir2-Ir4 of the sensor to EEprom.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_hysteresis_eeprom_ir24(uint8_t hysteresis_value)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t current_mode = 0;
  uint8_t temp = 0;

  // we keep the 5 last bits mask
  hysteresis_value &= 0b00011111;
  hysteresis_value |= 0b11100000;

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ECNTL1,
                                               &current_mode);
  temp = SPARKFUN_AK975X_EEPROM_MODE | (current_mode & 0b00111000);
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1, temp);

  // open EEPROM mode
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EKEY,
                                                SPARKFUN_AK975X_EKEY_ON);

  // allow EEPROM writing
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EEHYS24,
                                                hysteresis_value);
  sl_sleeptimer_delay_millisecond(15);
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1,
                                                current_mode);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Get hysteresis Ir2-Ir4 of the sensor to EEprom.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_eeprom_hysteresis_ir24(
  uint8_t *hysteresis_value)
{
  if (hysteresis_value == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint8_t current_mode = 0;
  uint8_t raw_value = 0;

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ECNTL1,
                                               &current_mode);
  uint8_t temp = SPARKFUN_AK975X_EEPROM_MODE | (current_mode & 0b00111000);
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1, temp);

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_EEHYS24,
                                               &raw_value);
  *hysteresis_value = raw_value & 0b00011111;

  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1,
                                                current_mode);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Set hysteresis Ir1-Ir3 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_hysteresis_ir13(uint8_t hysteresis_value)
{
  // we keep the 5 last bits mask
  hysteresis_value &= 0b00011111;
  hysteresis_value |= 0b11100000;

  sparkfun_ak9753_cfg.hysteresis_value13 = hysteresis_value;

  return sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EHYS13,
                                                 hysteresis_value);
}

/***************************************************************************//**
* Get hysteresis Ir1-Ir3 of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_hysteresis_ir13(uint8_t *hysteresis_value)
{
  if (hysteresis_value == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *hysteresis_value = sparkfun_ak9753_cfg.hysteresis_value13;

  return SL_STATUS_OK;
}

/***************************************************************************//**
* Set hysteresis Ir1-Ir3 of the sensor to EEprom.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_hysteresis_eeprom_ir13(uint8_t hysteresis_value)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t current_mode = 0;

  // we keep the 5 last bits mask
  hysteresis_value &= 0b00011111;
  hysteresis_value |= 0b11100000;

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ECNTL1,
                                               &current_mode);
  uint8_t temp = SPARKFUN_AK975X_EEPROM_MODE | (current_mode & 0b00111000);
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1, temp);

  // open EEPROM mode
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EKEY,
                                                SPARKFUN_AK975X_EKEY_ON);

  //   allow EEPROM writing
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EEHYS13,
                                                hysteresis_value);
  sl_sleeptimer_delay_millisecond(15);

  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1,
                                                current_mode);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Get hysteresis Ir2-Ir4 of the sensor to EEprom.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_eeprom_hysteresis_ir13(
  uint8_t *hysteresis_value)
{
  if (hysteresis_value == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_status_t sc = SL_STATUS_OK;
  uint8_t current_mode = 0;
  uint8_t raw_value = 0;

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ECNTL1,
                                               &current_mode);
  uint8_t temp = SPARKFUN_AK975X_EEPROM_MODE | (current_mode & 0b00111000);
  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1, temp);

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_EEHYS13,
                                               &raw_value);
  *hysteresis_value = raw_value & 0b00011111;

  sc |= sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_ECNTL1,
                                                current_mode);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Set interrupts mode of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_set_interrupts(uint8_t int_source)
{
  // mask , assembly of SPARKFUN_AK975X_EINTEN bits
  uint8_t v = 0b11000000 | int_source;
  sparkfun_ak9753_cfg.int_source_setting = v;
  return sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_EINTEN, v);
}

/***************************************************************************//**
* Get interrupts mode of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_get_int_source(uint8_t *int_source)
{
  if (int_source == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  *int_source = sparkfun_ak9753_cfg.int_source_setting;

  return SL_STATUS_OK;
}

/***************************************************************************//**
* Get the status over run state of the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_is_data_overrun(bool *data_overrun)
{
  if (data_overrun == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  uint8_t value = 0;
  sl_status_t sc = SL_STATUS_OK;

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_ST1, &value);

  *data_overrun = (value & (1 << 1)); // Bit 1 is DOR

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
* Soft reset the sensor.
*******************************************************************************/
sl_status_t sparkfun_ak9753_sw_reset(void)
{
  return sparkfun_ak9753_platform_write_register(SPARKFUN_AK975X_CNTL2, 0xFF);
}

/***************************************************************************//**
* Check the driver interrupt status
*******************************************************************************/
sl_status_t sparkfun_ak9753_is_interrupt(bool *isIR13H,
                                         bool *isIR13L,
                                         bool *isIR24H,
                                         bool *isIR24L,
                                         bool *isDataReady,
                                         bool *isDataOverrun)
{
  if ((isIR13H == NULL)
      || (isIR13L == NULL)
      || (isIR24H == NULL)
      || (isIR24L == NULL)
      || (isDataReady == NULL)
      || (isDataOverrun == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  uint8_t value = 0;
  sl_status_t sc = SL_STATUS_OK;

  sc |= sparkfun_ak9753_platform_read_register(SPARKFUN_AK975X_INTST, &value);
  *isIR13H = (1 << 4) & value;
  *isIR13L = (1 << 3) & value;
  *isIR24H = (1 << 2) & value;
  *isIR24L = (1 << 1) & value;
  *isDataReady = (1 << 0) & value;

  sc |= sparkfun_ak9753_is_data_overrun(isDataOverrun);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

#ifdef __cplusplus
}
#endif
