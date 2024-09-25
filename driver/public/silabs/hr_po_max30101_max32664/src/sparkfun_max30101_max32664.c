/***************************************************************************//**
 * @file sparkfun_hr_po_max30101_max32664.c
 * @brief This is a Library written for the MAXIM 32664 Biometric Sensor Hub
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
#include <stdlib.h>
#include <string.h>
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "sparkfun_max30101_max32664.h"
#include "sparkfun_max30101_max32664_config.h"

#define BIO_HUB_ADDRESS           0x55

#define ENABLE_CMD_DELAY          45 // Milliseconds
#define CMD_DELAY                 6  // Milliseconds
#define MAXFAST_ARRAY_SIZE        6  // Number of bytes....
#define MAXFAST_EXTENDED_DATA     5
#define MAX30101_LED_ARRAY        12 // 4 values of 24 bit (3 byte) LED values

#define SET_FORMAT                0x00
#define READ_FORMAT               0x01 // Index Byte under Family Byte:
                                       // READ_OUTPUT_MODE (0x11)
#define WRITE_SET_THRESHOLD       0x01 // Index Byte for WRITE_INPUT(0x14)
#define WRITE_EXTERNAL_TO_FIFO    0x00

#define WRITE_FIFO_INPUT_BYTE     0x04
#define APP_MODE                  0x00
#define BOOTLOADER_MODE           0x08
#define NO_WRITE                  0x00

// Variables ------------
static i2c_master_t bio_hub;
static bio_hub_algo_mode_t user_selected_mode;
static uint8_t sample_rate = 100;

// Private functions------------
static sl_status_t read_version(uint8_t family,
                                uint8_t index,
                                bio_hub_version_t *vers);
static sl_status_t enable_write(uint8_t family_byte,
                                uint8_t index_byte,
                                uint8_t enable_byte);
static sl_status_t write_byte(uint8_t family_byte,
                              uint8_t index_byte,
                              uint8_t write_byte);
static sl_status_t write_byte2(uint8_t family_byte,
                               uint8_t index_byte,
                               uint8_t write_byte,
                               uint8_t write_val);
static sl_status_t write_long_bytes(uint8_t family_byte,
                                    uint8_t index_byte,
                                    uint8_t write_byte,
                                    const int32_t *write_val,
                                    const size_t size);
static sl_status_t write_bytes(uint8_t family_byte,
                               uint8_t index_byte,
                               uint8_t write_byte,
                               const uint8_t *write_val,
                               size_t size);
static sl_status_t read_byte(uint8_t family_byte,
                             uint8_t index_byte,
                             uint8_t *byte_read);
static sl_status_t read_byte2(uint8_t family_byte,
                              uint8_t index_byte,
                              uint8_t write_byte,
                              uint8_t *byte_read);
static sl_status_t read_multiple_bytes_int(uint8_t family_byte,
                                           uint8_t index_byte,
                                           uint8_t write_byte,
                                           const size_t num_of_reads,
                                           int32_t *user_array);
static sl_status_t read_multiple_bytes(uint8_t family_byte,
                                       uint8_t index_byte,
                                       uint8_t write_byte,
                                       const size_t num_of_reads,
                                       uint8_t *user_array);
static sl_status_t read_fill_array(uint8_t family_byte,
                                   uint8_t index_byte,
                                   uint8_t num_of_reads,
                                   uint8_t *array);

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_DEVICE_MODE (0x02) Index Byte: 0x00, Write Byte: 0x00
 *    The following function initializes the sensor. To place the MAX32664 into
 *    application mode, the MFIO pin must be pulled HIGH while the board is held
 *    in reset for 10ms. After 50 additional ms have elapsed the board should be
 *    in application mode and will return two bytes,
 *    the first 0x00 is a successful communication byte,
 *    followed by 0x00 which is the byte indicating which mode the IC is in.
 *
 * @param[out] status
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_init(mikroe_i2c_handle_t i2cspm_instance, uint8_t address)
{
  uint8_t response_byte;
  sl_status_t sc;

  if (i2cspm_instance == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  bio_hub.handle = i2cspm_instance;

  i2c_master_config_t i2c_cfg;
  i2c_master_configure_default(&i2c_cfg);

  if (address) {
    i2c_cfg.addr = address;
  } else {
    i2c_cfg.addr = BIO_HUB_ADDRESS;
  }

  i2c_cfg.timeout_pass_count = 0;

#if (SPARKFUN_MAX30101_I2C_UC == 1)
  i2c_cfg.speed = SPARKFUN_MAX30101_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&bio_hub, &i2c_cfg) == I2C_MASTER_ERROR) {
    return SL_STATUS_FAIL;
  }

  // The following function initializes the sensor. To place the MAX32664 into
  // application mode, the MFIO pin must be pulled HIGH while the board is held
  // in reset for 10ms. After 50 addtional ms have elapsed the board should be
  // in application mode and will return two bytes, the first 0x00 is a
  // successful communcation byte, followed by 0x00 which is the byte indicating
  // which mode the IC is in.
#if defined(CONFIG_BIO_HUB_RESET_PORT) && defined(CONFIG_BIO_HUB_RESET_PIN)
  GPIO_PinModeSet(CONFIG_BIO_HUB_RESET_PORT,
                  CONFIG_BIO_HUB_RESET_PIN,
                  gpioModePushPull,
                  0);
#endif
#if defined(CONFIG_BIO_HUB_MFIO_PORT) && defined(CONFIG_BIO_HUB_MFIO_PIN)
  GPIO_PinModeSet(CONFIG_BIO_HUB_MFIO_PORT,
                  CONFIG_BIO_HUB_MFIO_PIN,
                  gpioModePushPull,
                  1);
#endif

#if defined(CONFIG_BIO_HUB_MFIO_PORT) && defined(CONFIG_BIO_HUB_MFIO_PIN)
  GPIO_PinOutSet(CONFIG_BIO_HUB_MFIO_PORT, CONFIG_BIO_HUB_MFIO_PIN);
#endif
#if defined(CONFIG_BIO_HUB_RESET_PORT) && defined(CONFIG_BIO_HUB_RESET_PIN)
  GPIO_PinOutClear(CONFIG_BIO_HUB_RESET_PORT, CONFIG_BIO_HUB_RESET_PIN);
  sl_sleeptimer_delay_millisecond(10);
  GPIO_PinOutSet(CONFIG_BIO_HUB_RESET_PORT, CONFIG_BIO_HUB_RESET_PIN);
  sl_sleeptimer_delay_millisecond(1000);
#endif
#if defined(CONFIG_BIO_HUB_MFIO_PORT) && defined(CONFIG_BIO_HUB_MFIO_PIN)
  GPIO_PinModeSet(CONFIG_BIO_HUB_MFIO_PORT,
                  CONFIG_BIO_HUB_MFIO_PIN,
                  gpioModeInputPull,
                  1); // To be used as an interrupt later
#endif

  sc = read_byte(READ_DEVICE_MODE, 0x00, &response_byte);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  return response_byte == 0 ? SL_STATUS_OK:SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_DEVICE_MODE (0x02) Index Byte: 0x00, Write Byte: 0x00
 *    The following function puts the MAX32664 into bootloader mode.
 *    To place the MAX32664 into bootloader mode,
 *    the MFIO pin must be pulled LOW while the board is held in reset for 10ms.
 *    After 50 addtional ms have elapsed the board should be in bootloader mode
 *    and will return two bytes, the first 0x00 is a successful communcation
 *    byte, followed by 0x08 which is the byte indicating
 *    that the board is in bootloader mode.
 *
 * @param[in] i2cspm_instance
 *    I2C instance
 * @param[in] address
 *    Device address, default is 0x55
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
#if defined(CONFIG_BIO_HUB_RESET_PORT) && defined(CONFIG_BIO_HUB_RESET_PIN) \
  && defined(CONFIG_BIO_HUB_MFIO_PORT) && defined(CONFIG_BIO_HUB_MFIO_PIN)
sl_status_t bio_hub_init_bootloader_mode(sl_i2cspm_t *i2cspm_instance,
                                         uint8_t address)
{
  uint8_t response_byte;
  sl_status_t sc;

  if (i2cspm_instance == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  bio_hub_i2cspm_instance = i2cspm_instance;
  i2c_address = address;

  GPIO_PinModeSet(CONFIG_BIO_HUB_RESET_PORT,
                  CONFIG_BIO_HUB_RESET_PIN,
                  gpioModePushPull,
                  0);
  GPIO_PinModeSet(CONFIG_BIO_HUB_MFIO_PORT,
                  CONFIG_BIO_HUB_MFIO_PIN,
                  gpioModePushPull,
                  1);

  GPIO_PinOutClear(CONFIG_BIO_HUB_MFIO_PORT, CONFIG_BIO_HUB_MFIO_PIN);
  GPIO_PinOutClear(CONFIG_BIO_HUB_RESET_PORT, CONFIG_BIO_HUB_RESET_PIN);
  sl_sleeptimer_delay_millisecond(10);
  GPIO_PinOutSet(CONFIG_BIO_HUB_RESET_PORT, CONFIG_BIO_HUB_RESET_PIN);
  sl_sleeptimer_delay_millisecond(50);
  // Bootloader mode is enabled when this ends.

  // Let's check to see if the device made it into bootloader mode.
  sc = read_byte(READ_DEVICE_MODE, 0x00, &response_byte);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  return SL_STATUS_OK;
}

#endif

/***************************************************************************//**
 * @brief
 *    Family Byte: HUB_STATUS (0x00),
 *    Index Byte: 0x00,
 *    No Write Byte.
 *    The following function checks the status of the FIFO.
 *
 * @param[out] status
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_sensor_hub_status(uint8_t *status)
{
  if (status == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte(0x00, 0x00, status); // Just family and index byte.
}

/***************************************************************************//**
 * @brief
 *    This function sets very basic settings to get sensor and biometric data.
 *    The biometric data includes data about heartrate, the confidence
 *    level, SpO2 levels, and whether the sensor has detected a finger or not.
 *
 * @param[in] mode
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_config_bpm(bio_hub_algo_mode_t mode)
{
  sl_status_t sc;

  if (mode > BIO_HUB_ALGO_MODE_TWO) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Just the data
  sc = bio_hub_set_output_mode(BIO_HUB_OUTPUT_MODE_ALGO_DATA);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // One sample before interrupt is fired.
  sc = bio_hub_set_fifo_threshold(0x01);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // One sample before interrupt is fired.
  sc = bio_hub_agc_algo_control(true);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  sc = bio_hub_max30101_control(true);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  sc = bio_hub_maxim_fast_algo_control(mode);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  user_selected_mode = mode;
  sc = bio_hub_read_algo_samples(&sample_rate);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  sl_sleeptimer_delay_millisecond(1000);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    This function sets very basic settings to get LED count values from
 *    the MAX30101. Sensor data includes 24 bit LED values for
 *    the three LED channels: Red, IR, and Green.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_config_sensor(void)
{
  sl_status_t sc; // Our status chauffeur

  // Just the sensor data (LED)
  sc = bio_hub_set_output_mode(BIO_HUB_OUTPUT_MODE_SENSOR_DATA);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // One sample before interrupt is fired to the MAX32664
  sc = bio_hub_set_fifo_threshold(0x01);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // Enable Sensor.
  sc = bio_hub_max30101_control(true);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // Enable algorithm
  sc = bio_hub_maxim_fast_algo_control(BIO_HUB_ALGO_MODE_ONE);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  sl_sleeptimer_delay_millisecond(1000);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    This function sets very basic settings to get sensor and biometric data.
 *    Sensor data includes 24 bit LED values for the two LED channels:
 *    Red and IR.
 *    The biometric data includes data about heartrate, the confidence
 *    level, SpO2 levels, and whether the sensor has detected a finger or not.
 *    Of note, the number of samples is set to one.
 *
 * @param[in] mode
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_config_sensor_bpm(uint8_t mode)
{
  sl_status_t sc;

  if (mode > BIO_HUB_ALGO_MODE_TWO) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Data and sensor data
  sc = bio_hub_set_output_mode(BIO_HUB_OUTPUT_MODE_SENSOR_AND_ALGORITHM);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // One sample before interrupt is fired to the MAX32664
  sc = bio_hub_set_fifo_threshold(0x01);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // Enable Sensor.
  sc = bio_hub_max30101_control(true);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // Enable algorithm
  sc = bio_hub_maxim_fast_algo_control(mode);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  user_selected_mode = mode;
  sc = bio_hub_read_algo_samples(&sample_rate);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  sl_sleeptimer_delay_millisecond(1000);
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    This function takes the 8 bytes from the FIFO buffer related to the wrist
 *    heart rate algortihm: heart rate (uint16_t), confidence (uint8_t) ,
 *    SpO2 (uint16_t), and the finger detected status (uint8_t).
 *    Note that the the algorithm is stated as "wrist" though
 *    the sensor only works with the finger.
 *    The data is loaded into the whrmFifo and returned.
 *
 * @param[out] lib_bpm
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_bpm(bio_hub_data_t *lib_bpm)
{
  uint8_t status_chauf; // The status chauffeur captures return values.
  sl_status_t sc;
  uint8_t samp_avail;

  if (lib_bpm == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = bio_hub_read_sensor_hub_status(&status_chauf);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // Communication Error
  if (status_chauf == 1) {
    sc = SL_STATUS_FAIL;
    goto error;
  }

  sc = bio_hub_num_samples_out_fifo(&samp_avail);
  if (SL_STATUS_OK != sc) {
    goto error;
  }

  if (user_selected_mode == BIO_HUB_ALGO_MODE_ONE) {
    uint8_t bpm_arr[MAXFAST_ARRAY_SIZE + 1];

    sc = read_fill_array(READ_DATA_OUTPUT, READ_DATA, sizeof(bpm_arr), bpm_arr);
    if (SL_STATUS_OK != sc) {
      goto error;
    }

    // SUCCESS: 0x00
    if (bpm_arr[0]) {
      sc = SL_STATUS_FAIL;
      goto error;
    }

    // Heart Rate formatting
    lib_bpm->heart_rate = (uint16_t)bpm_arr[1] << 8;
    lib_bpm->heart_rate |= (bpm_arr[2]);
    lib_bpm->heart_rate /= 10;

    // Confidence formatting
    lib_bpm->confidence = bpm_arr[3];

    // Blood oxygen level formatting
    lib_bpm->oxygen = (uint16_t)bpm_arr[4] << 8;
    lib_bpm->oxygen |= bpm_arr[5];
    lib_bpm->oxygen /= 10;

    // "Machine State" - has a finger been detected?
    lib_bpm->status = bpm_arr[6];

    return SL_STATUS_OK;
  } else if (user_selected_mode == BIO_HUB_ALGO_MODE_TWO) {
    uint8_t bpm_arr_two[MAXFAST_ARRAY_SIZE + MAXFAST_EXTENDED_DATA + 1];

    sc = read_fill_array(READ_DATA_OUTPUT, READ_DATA,
                         sizeof(bpm_arr_two),
                         bpm_arr_two);
    if (SL_STATUS_OK != sc) {
      goto error;
    }

    // SUCCESS: 0x00
    if (bpm_arr_two[0]) {
      sc = SL_STATUS_FAIL;
      goto error;
    }

    // Heart Rate formatting
    lib_bpm->heart_rate = (uint16_t)bpm_arr_two[1] << 8;
    lib_bpm->heart_rate |= (bpm_arr_two[2]);
    lib_bpm->heart_rate /= 10;

    // Confidence formatting
    lib_bpm->confidence = bpm_arr_two[3];

    // Blood oxygen level formatting
    lib_bpm->oxygen = (uint16_t)bpm_arr_two[4] << 8;
    lib_bpm->oxygen |= bpm_arr_two[5];
    lib_bpm->oxygen /= 10.0;

    // "Machine State" - has a finger been detected?
    lib_bpm->status = bpm_arr_two[6];

    // Sp02 r Value formatting
    uint16_t tempVal = (uint16_t)bpm_arr_two[7] << 8;
    tempVal |= bpm_arr_two[8];
    lib_bpm->r_value = tempVal;
    lib_bpm->r_value /= 10.0;

    // Extended Machine State formatting
    lib_bpm->ext_status = bpm_arr_two[9];

    // There are two additional bytes of data that were requested but that
    // have not been implemented in firmware 10.1 so will not be saved to
    // user's data.
    return SL_STATUS_OK;
  } else {
    lib_bpm->heart_rate = 0;
    lib_bpm->confidence = 0;
    lib_bpm->oxygen = 0;
    return SL_STATUS_FAIL;
  }

  error:
  lib_bpm->heart_rate = 0;
  lib_bpm->confidence = 0;
  lib_bpm->oxygen = 0;
  return sc;
}

/***************************************************************************//**
 * @brief
 *    This function takes 9 bytes of LED values from the MAX30101
 *    associated with the RED, IR, and GREEN LEDs.
 *    In addition it gets the 8 bytes from the FIFO buffer related to
 *    the wrist heart rate algortihm: heart rate (uint16_t),
 *    confidence (uint8_t), SpO2 (uint16_t),
 *    and the finger detected status (uint8_t). Note that the the algorithm
 *    is stated as "wrist" though the sensor only works with the finger.
 *    The data is loaded into the whrmFifo and returned.
 *
 * @param[out] lib_led_fifo
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_sensor(bio_hub_data_t *lib_led_fifo)
{
  sl_status_t sc;
  uint8_t sen_arr[MAX30101_LED_ARRAY + 1];

  if (lib_led_fifo == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = read_fill_array(READ_DATA_OUTPUT, READ_DATA, sizeof(sen_arr), sen_arr);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // SUCCESS: 0x00
  if (sen_arr[0]) {
    return SL_STATUS_FAIL;
  }

  // Value of LED one....
  lib_led_fifo->ir_led = (uint32_t)sen_arr[1] << 16;
  lib_led_fifo->ir_led |= (uint32_t)sen_arr[2] << 8;
  lib_led_fifo->ir_led |= sen_arr[3];

  // Value of LED two...
  lib_led_fifo->red_led = (uint32_t)sen_arr[4] << 16;
  lib_led_fifo->red_led |= (uint32_t)sen_arr[5] << 8;
  lib_led_fifo->red_led |= sen_arr[6];

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    This function takes the information of both the LED value and
 *    the biometric data from the MAX32664's FIFO.
 *    In essence it combines the two functions
 *    above into a single function call.
 *
 * @param[out] lib_led_bpm
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_sensor_bpm(bio_hub_data_t *lib_led_bpm)
{
  sl_status_t sc;
  uint8_t bpm_sen_arr[MAXFAST_ARRAY_SIZE + MAX30101_LED_ARRAY + 1];

  if (lib_led_bpm == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  if (user_selected_mode == BIO_HUB_ALGO_MODE_ONE) {
    sc = read_fill_array(READ_DATA_OUTPUT,
                         READ_DATA,
                         MAXFAST_ARRAY_SIZE + MAX30101_LED_ARRAY,
                         bpm_sen_arr);
    if (SL_STATUS_OK != sc) {
      return sc;
    }

    // SUCCESS: 0x00
    if (bpm_sen_arr[0]) {
      return SL_STATUS_FAIL;
    }

    // Value of LED one....
    lib_led_bpm->ir_led = (uint32_t)bpm_sen_arr[1] << 16;
    lib_led_bpm->ir_led |= (uint32_t)bpm_sen_arr[2] << 8;
    lib_led_bpm->ir_led |= bpm_sen_arr[3];

    // Value of LED two...
    lib_led_bpm->red_led = (uint32_t)bpm_sen_arr[4] << 16;
    lib_led_bpm->red_led |= (uint32_t)bpm_sen_arr[5] << 8;
    lib_led_bpm->red_led |= bpm_sen_arr[6];

    // -- What happened here? -- There are two uint32_t values that are given by
    // the sensor for LEDs that do not exists on the MAX30101. So we have to
    // request those empty values because they occupy the buffer:
    // bpm_sen_arr[6-11].

    // Heart rate formatting
    lib_led_bpm->heart_rate = (uint16_t)bpm_sen_arr[13] << 8;
    lib_led_bpm->heart_rate |= (bpm_sen_arr[14]);
    lib_led_bpm->heart_rate /= 10;

    // Confidence formatting
    lib_led_bpm->confidence = bpm_sen_arr[15];

    // Blood oxygen level formatting
    lib_led_bpm->oxygen = (uint16_t)bpm_sen_arr[16] << 8;
    lib_led_bpm->oxygen |= bpm_sen_arr[17];
    lib_led_bpm->oxygen /= 10;

    // "Machine State" - has a finger been detected?
    lib_led_bpm->status = bpm_sen_arr[18];

    return SL_STATUS_OK;
  } else if (user_selected_mode == BIO_HUB_ALGO_MODE_TWO) {
    uint8_t bpm_sen_arr_two[MAXFAST_ARRAY_SIZE
                            + MAXFAST_EXTENDED_DATA
                            + MAX30101_LED_ARRAY + 1];

    sc = read_fill_array(READ_DATA_OUTPUT, READ_DATA,
                         sizeof(bpm_sen_arr_two),
                         bpm_sen_arr_two);
    if (SL_STATUS_OK != sc) {
      return sc;
    }

    // SUCCESS: 0x00
    if (bpm_sen_arr_two[0]) {
      return SL_STATUS_FAIL;
    }

    // Value of LED one....
    lib_led_bpm->ir_led = (uint32_t)bpm_sen_arr_two[1] << 16;
    lib_led_bpm->ir_led |= (uint32_t)bpm_sen_arr_two[2] << 8;
    lib_led_bpm->ir_led |= bpm_sen_arr_two[3];

    // Value of LED two...
    lib_led_bpm->red_led = (uint32_t)bpm_sen_arr_two[4] << 16;
    lib_led_bpm->red_led |= (uint32_t)bpm_sen_arr_two[5] << 8;
    lib_led_bpm->red_led |= bpm_sen_arr_two[6];

    // -- What happened here? -- There are two uint32_t values that are given by
    // the sensor for LEDs that do not exists on the MAX30101. So we have to
    // request those empty values because they occupy the buffer:
    // bpm_sen_arr_two[6-11].

    // Heart rate formatting
    lib_led_bpm->heart_rate = (uint16_t)bpm_sen_arr_two[13] << 8;
    lib_led_bpm->heart_rate |= bpm_sen_arr_two[14];
    lib_led_bpm->heart_rate /= 10;

    // Confidence formatting
    lib_led_bpm->confidence = bpm_sen_arr_two[15];

    // Blood oxygen level formatting
    lib_led_bpm->oxygen = (uint16_t)bpm_sen_arr_two[16] << 8;
    lib_led_bpm->oxygen |= bpm_sen_arr_two[17];
    lib_led_bpm->oxygen /= 10;

    // "Machine State" - has a finger been detected?
    lib_led_bpm->status = bpm_sen_arr_two[18];

    // Sp02 r Value formatting
    uint16_t tempVal = (uint16_t)bpm_sen_arr_two[19] << 8;
    tempVal |= bpm_sen_arr_two[20];
    lib_led_bpm->r_value = tempVal;
    lib_led_bpm->r_value /= 10.0;

    // Extended Machine State formatting
    lib_led_bpm->ext_status = bpm_sen_arr_two[21];

    // There are two additional bytes of data that were requested but that
    // have not been implemented in firmware 10.1 so will not be saved to
    // user's data.
    //
    return SL_STATUS_OK;
  } else {
    lib_led_bpm->ir_led = 0;
    lib_led_bpm->red_led = 0;
    lib_led_bpm->heart_rate = 0;
    lib_led_bpm->confidence = 0;
    lib_led_bpm->oxygen = 0;
    lib_led_bpm->status = 0;
    lib_led_bpm->r_value = 0;
    lib_led_bpm->ext_status = 0;

    return SL_STATUS_FAIL;
  }
}

/***************************************************************************//**
 * @brief
 *    This function modifies the pulse width of the MAX30101 LEDs.
 *    All of the LEDs are modified to the same width.
 *    This will affect the number of samples that can be collected
 *    and will also affect the ADC resolution.
 *    Width(us) - Resolution -  Sample Rate
 *    Default: 69us - 15 resolution - 50 samples per second.
 *      69us     -    15      -   <= 3200 (fastest - least resolution)
 *      118us    -    16      -   <= 1600
 *      215us    -    17      -   <= 1600
 *      411us    -    18      -   <= 1000 (slowest - highest resolution)
 *
 * @param[in] width
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_pulse_width(max30101_pulse_width_t width)
{
  sl_status_t sc;
  uint8_t bits;
  uint8_t reg_val;

  // Make sure the correct pulse width is selected.
  if (width == MAX30101_PULSE_WIDTH_69) {
    bits = 0;
  } else if (width == MAX30101_PULSE_WIDTH_118) {
    bits = 1;
  } else if (width == MAX30101_PULSE_WIDTH_215) {
    bits = 2;
  } else if (width == MAX30101_PULSE_WIDTH_411) {
    bits = 3;
  } else {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Get current register value so that nothing is overwritten.
  sc = bio_hub_read_register_max30101(MAX30101_CONFIGURATION_REGISTER,
                                      &reg_val);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  reg_val &= MAX30101_PULSE_MASK; // Mask bits to change.
  reg_val |= bits; // Add bits
  // Write Register
  bio_hub_write_register_max30101(MAX30101_CONFIGURATION_REGISTER, reg_val);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    This function reads the MAX30101_CONFIGURATION_REGISTER (0x0A), bits [1:0]
 *    from the MAX30101 Sensor.
 *    It returns one of the four settings in microseconds.
 *
 * @param[out] width
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_pulse_width(uint16_t *width)
{
  sl_status_t sc;
  uint8_t reg_val;

  if (width == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = bio_hub_read_register_max30101(MAX30101_CONFIGURATION_REGISTER,
                                      &reg_val);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  reg_val &= MAX30101_READ_PULSE_MASK;

  if (reg_val == 0) {
    *width = 69;
  } else if (reg_val == 1) {
    *width = 118;
  } else if (reg_val == 2) {
    *width = 215;
  } else if (reg_val == 3) {
    *width = 411;
  } else {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    This function changes the sample rate of the MAX30101 sensor. The sample
 *    rate is affected by the set pulse width of the MAX30101 LEDs.
 *    Default: 69us - 15 resolution - 50 samples per second.
 *    Width(us) - Resolution -  Sample Rate
 *      69us     -    15      -   <= 3200 (fastest - least resolution)
 *      118us    -    16      -   <= 1600
 *      215us    -    17      -   <= 1600
 *      411us    -    18      -   <= 1000 (slowest - highest resolution)
 *    Samples Options:
 *    50, 100, 200, 400, 800, 1000, 1600, 3200
 *
 * @param[in] samp_rate
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_sample_rate(max30101_sample_rate_t samp_rate)
{
  sl_status_t sc;
  uint8_t bits;
  uint8_t reg_val;

  // Make sure the correct sample rate was picked
  if (samp_rate == MAX30101_SAMPLE_RATE_50) {
    bits = 0;
  } else if (samp_rate == MAX30101_SAMPLE_RATE_100) {
    bits = 1;
  } else if (samp_rate == MAX30101_SAMPLE_RATE_200) {
    bits = 2;
  } else if (samp_rate == MAX30101_SAMPLE_RATE_400) {
    bits = 3;
  } else if (samp_rate == MAX30101_SAMPLE_RATE_800) {
    bits = 4;
  } else if (samp_rate == MAX30101_SAMPLE_RATE_1000) {
    bits = 5;
  } else if (samp_rate == MAX30101_SAMPLE_RATE_1600) {
    bits = 6;
  } else if (samp_rate == MAX30101_SAMPLE_RATE_3200) {
    bits = 7;
  } else {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Get current register value so that nothing is overwritten.
  sc = bio_hub_read_register_max30101(MAX30101_CONFIGURATION_REGISTER,
                                      &reg_val);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  reg_val &= MAX30101_SAMP_MASK; // Mask bits to change.
  reg_val |= (bits << 2); // Add bits but shift them first to correct position.
  // Write Register
  bio_hub_write_register_max30101(MAX30101_CONFIGURATION_REGISTER, reg_val);

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    This function reads the MAX30101_CONFIGURATION_REGISTER (0x0A),
 *    bits [4:2] from the MAX30101 Sensor.
 *    It returns one of the 8 possible sample rates.
 *
 * @param[out] samp_rate
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_sample_rate(uint16_t *samp_rate)
{
  sl_status_t sc;
  uint8_t reg_val;

  if (samp_rate == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = bio_hub_read_register_max30101(MAX30101_CONFIGURATION_REGISTER,
                                      &reg_val);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  reg_val &= MAX30101_READ_SAMP_MASK;
  reg_val = (reg_val >> 2);

  if (reg_val == 0) {
    *samp_rate = 50;
  } else if (reg_val == 1) {
    *samp_rate = 100;
  } else if (reg_val == 2) {
    *samp_rate = 200;
  } else if (reg_val == 3) {
    *samp_rate = 400;
  } else if (reg_val == 4) {
    *samp_rate = 800;
  } else if (reg_val == 5) {
    *samp_rate = 1000;
  } else if (reg_val == 6) {
    *samp_rate = 1600;
  } else if (reg_val == 7) {
    *samp_rate = 3200;
  } else {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    MAX30101 Register: MAX30101_CONFIGURATION_REGISTER (0x0A), bits [6:5]
 *    This functions sets the dynamic range of the MAX30101's ADC. The function
 *    accepts the higher range as a parameter.
 *    Default Range: 7.81pA - 2048nA
 *    Possible Ranges:
 *    7.81pA  - 2048nA
 *    15.63pA - 4096nA
 *    32.25pA - 8192nA
 *    62.5pA  - 16384nA
 *
 * @param[in] adc_val
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_adc_range(uint16_t adc_val)
{
  sl_status_t sc;
  uint8_t reg_val;
  uint8_t bits;

  if (adc_val <= 2048) {
    bits = 0;
  } else if (adc_val <= 4096) {
    bits = 1;
  } else if (adc_val <= 8192) {
    bits = 2;
  } else if (adc_val <= 16384) {
    bits = 3;
  } else {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sc = bio_hub_read_register_max30101(MAX30101_CONFIGURATION_REGISTER,
                                      &reg_val);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  reg_val &= MAX30101_ADC_MASK;
  reg_val |= bits << 5;

  sc = bio_hub_write_register_max30101(MAX30101_CONFIGURATION_REGISTER,
                                       reg_val);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    MAX30101 Register: MAX30101_CONFIGURATION_REGISTER (0x0A), bits [6:5]
 *    This function returns the set ADC range of the MAX30101 sensor.
 *
 * @param[out] adc_val
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_adc_range(uint16_t *adc_val)
{
  sl_status_t sc;
  uint8_t reg_val;

  if (adc_val == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = bio_hub_read_register_max30101(MAX30101_CONFIGURATION_REGISTER,
                                      &reg_val);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  reg_val &= MAX30101_READ_ADC_MASK;
  reg_val = (reg_val >> 5); // Shift our bits to the front of the line.

  if (reg_val == 0) {
    *adc_val = 2048;
  } else if (reg_val == 1) {
    *adc_val = 4096;
  } else if (reg_val == 2) {
    *adc_val = 8192;
  } else if (reg_val == 3) {
    *adc_val = 16384;
  } else {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    Family Byte: SET_DEVICE_MODE (0x01),
 *    Index Byte: 0x01,
 *    Write Byte: 0x00
 *    The following function is an alternate way to set the mode of the of
 *    MAX32664. It can take three parameters: Enter and Exit Bootloader Mode, as
 *    well as reset.
 *
 * @param[in] selection
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_operating_mode(
  bio_hub_device_mode_write_byte_t selection)
{
  sl_status_t sc;
  uint8_t response_byte;

  // Must be one of the three....
  if ((selection != BIO_HUB_EXIT_BOOTLOADER)
      && (selection != BIO_HUB_RESET)
      && (selection != BIO_HUB_ENTER_BOOTLOADER)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sc = write_byte(SET_DEVICE_MODE, 0x00, selection);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  // Here we'll check if the board made it into Bootloader mode...
  // This is in fact the status byte,
  // need second returned byte - bootloader mode
  sc = read_byte(READ_DEVICE_MODE, 0x00, &response_byte);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  return (response_byte == SUCCESS) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 *    Family Byte: IDENTITY (0x01),
 *    Index Byte: READ_MCU_TYPE,
 *    Write Byte: NONE
 *    The following function returns a byte that signifies the microcontoller
 *    that is in communcation with your host microcontroller.
 *    Returns 0x00 for the MAX32625 and 0x01 for the MAX32660/MAX32664.
 *
 * @param[in] return_byte
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_get_mcu_type(uint8_t *return_byte)
{
  if (return_byte == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte2(IDENTITY, READ_MCU_TYPE, NO_WRITE, return_byte);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: BOOTLOADER_INFO (0x80),
 *    Index Byte: BOOTLOADER_VERS (0x00)
 *    This function checks the version number of the bootloader on the chip and
 *    returns a four bytes: Major version Byte, Minor version Byte, Space Byte,
 *    and the Revision Byte.
 *
 * @param[in] sen_switch
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_get_bootloader_inf(int32_t *boot_vers)
{
  sl_status_t sc;
  int32_t rev_num[4];

  if (boot_vers == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  *boot_vers = 0;

  sc = read_multiple_bytes_int(BOOTLOADER_INFO,
                               BOOTLOADER_VERS,
                               0x00,
                               4,
                               rev_num);

  if (SL_STATUS_OK != sc) {
    return sc;
  }
  *boot_vers |= (int32_t)rev_num[1] << 16;
  *boot_vers |= (int32_t)rev_num[2] << 8;
  *boot_vers |= rev_num[3];
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    Family Byte: ENABLE_SENSOR (0x44),
 *    Index Byte: ENABLE_MAX30101 (0x03),
 *    Write Byte: sen_switch (parameter - 0x00 or 0x01).
 *    This function enables the MAX30101.
 *
 * @param[in] sen_switch
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_max30101_control(bool sen_switch)
{
  // Check that communication was successful, not that the sensor is enabled.
  return enable_write(ENABLE_SENSOR, ENABLE_MAX30101, sen_switch ? 1 : 0);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_SENSOR_MODE (0x45),
 *    Index Byte: READ_ENABLE_MAX30101 (0x03)
 *    This function checks if the MAX30101 is enabled or not.
 *
 * @param[in] state
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_max30101_state(uint8_t *state)
{
  if (state == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte(READ_SENSOR_MODE, READ_ENABLE_MAX30101, state);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: ENABLE_SENSOR (0x44),
 *    Index Byte: ENABLE_ACCELEROMETER (0x04),
 *    Write Byte: accel_switch (parameter - 0x00 or 0x01).
 *    This function enables the ACCELEROMETER.
 *
 * @param[in] accel_switch
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_accel_control(bool accel_switch)
{
  // Check that communication was successful, not that the sensor is enabled.
  return enable_write(ENABLE_SENSOR,
                      ENABLE_ACCELEROMETER,
                      accel_switch ? 1 : 0);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: OUTPUT_MODE (0x10),
 *    Index Byte: SET_FORMAT (0x00),
 *    Write Byte : output_type (Parameter values in OUTPUT_MODE_WRITE_BYTE)
 *
 * @param[in] output_type
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_output_mode(
  bio_hub_output_mode_write_byte_t output_type)
{
  // Bytes between 0x00 and 0x07
  if (output_type > BIO_HUB_OUTPUT_MODE_SENSOR_ALGO_COUNTER) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Check that communication was successful, not that the IC is outputting
  // correct format.
  return write_byte(OUTPUT_MODE, SET_FORMAT, output_type);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: OUTPUT_MODE,
 *    Index Byte: WRITE_SET_THRESHOLD,
 *    Write byte: intThres (parameter - value betwen 0 and 0xFF).
 *    This function changes the threshold for the FIFO interrupt bit/pin.
 *    The interrupt pin is the MFIO pin which is set to INPUT after
 *    IC initialization (begin).
 *
 * @param[in] int_thresh
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_fifo_threshold(uint8_t int_thresh)
{
  // Checks that there was succesful communcation, not that the threshold was
  // set correctly.
  return write_byte(OUTPUT_MODE, WRITE_SET_THRESHOLD, int_thresh);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_DATA_OUTPUT (0x12),
 *    Index Byte: NUM_SAMPLES (0x00),
 *    Write Byte: NONE
 *    This function returns the number of samples available in the FIFO.
 *
 * @param[in] samp_avail
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_num_samples_out_fifo(uint8_t *samp_avail)
{
  if (samp_avail == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte(READ_DATA_OUTPUT, NUM_SAMPLES, samp_avail);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_DATA_OUTPUT (0x12),
 *    Index Byte: READ_DATA (0x00),
 *    Write Byte: NONE
 *    This function returns the data in the FIFO.
 *
 * @param[in] buf
 * @param[in] size
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_get_data_out_fifo(uint8_t *buf, uint8_t *size)
{
  uint8_t samples;
  sl_status_t sc;

  if ((buf == NULL) || (size == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = bio_hub_num_samples_out_fifo(&samples);
  if (SL_STATUS_OK != sc) {
    return sc;
  }
  if (*size < samples) {
    return SL_STATUS_WOULD_OVERFLOW;
  }
  *size = samples;
  return read_fill_array(READ_DATA_OUTPUT, READ_DATA, samples, buf);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_DATA_OUTPUT (0x12),
 *    Index Byte: READ_DATA (0x00),
 *    Write Byte: NONE
 *    This function adds support for the acceleromter that is NOT included on
 *    SparkFun's product,
 *    The Family Registery of 0x13 and 0x14 is skipped for now.
 *
 * @param[in] reg_addr
 * @param[in] reg_cont
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_num_samples_external_sensor(uint8_t *samp_avail)
{
  if (samp_avail == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte2(READ_DATA_INPUT,
                    SAMPLE_SIZE,
                    WRITE_ACCELEROMETER,
                    samp_avail);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: WRITE_REGISTER (0x40),
 *    Index Byte: WRITE_MAX30101 (0x03),
 *    Write Bytes: Register Address and Register Value
 *    This function writes the given register value
 *    at the given register address for the MAX30101 sensor
 *
 * @param[in] reg_addr
 * @param[in] reg_cont
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_write_register_max30101(uint8_t reg_addr, uint8_t reg_val)
{
  return write_byte2(WRITE_REGISTER, WRITE_MAX30101, reg_addr, reg_val);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: WRITE_REGISTER (0x40),
 *    Index Byte: WRITE_ACCELEROMETER (0x04),
 *    Write Bytes: Register Address and Register Value
 *    This function writes the given register value
 *    at the given register address for the Accelerometer
 *
 * @param[in] reg_addr
 * @param[in] reg_cont
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_write_register_accel(uint8_t reg_addr, uint8_t reg_val)
{
  return write_byte2(WRITE_REGISTER, WRITE_ACCELEROMETER, reg_addr, reg_val);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_REGISTER (0x41),
 *    Index Byte: READ_MAX30101 (0x03),
 *    Write Byte: Register Address
 *    This function reads the given register address for the MAX30101 Sensor and
 *    returns the values at that register.
 *
 * @param[in] reg_addr
 * @param[in] reg_cont
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_register_max30101(uint8_t reg_addr, uint8_t *reg_cont)
{
  if (reg_cont == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte2(READ_REGISTER, READ_MAX30101, reg_addr, reg_cont);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_REGISTER (0x41),
 *    Index Byte: READ_MAX30101 (0x03),
 *    Write Byte: Register Address
 *    This function reads the given register address for the MAX30101 Sensor and
 *    returns the values at that register.
 *
 * @param[in] reg_array
 * @param[in] reg_cont
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_register_accel(uint8_t reg_addr, uint8_t *reg_cont)
{
  if (reg_cont == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte2(READ_REGISTER, READ_ACCELEROMETER, reg_addr, reg_cont);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_ATTRIBUTES_AFE (0x42),
 *    Index Byte: RETRIEVE_AFE_MAX30101/ (0x03)
 *    This function retrieves the attributes of the AFE (Analog Front End) of
 *    the MAX30101 sensor. It returns the number of bytes in a word for
 *    the sensor and the number of registers available.
 *
 * @param[in] reg_array
 * @param[in] num_of_bytes
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_get_afe_attributes_max30101(bio_hub_sensor_attr_t *max_attr)
{
  uint8_t temp_array[2] = { 0x00 };
  sl_status_t sc;

  if (max_attr == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = read_fill_array(READ_ATTRIBUTES_AFE,
                       RETRIEVE_AFE_MAX30101,
                       2,
                       temp_array);
  max_attr->byte_word = temp_array[0];
  max_attr->avail_registers = temp_array[1];
  return sc;
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_ATTRIBUTES_AFE (0x42),
 *    Index Byte: RETRIEVE_AFE_ACCELEROMETER (0x04)
 *    This function retrieves the attributes of the AFE (Analog Front End) of
 *    the Accelerometer. It returns the number of bytes in a word for the sensor
 *    and the number of registers available.
 *
 * @param[in] reg_array
 * @param[in] num_of_bytes
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_get_afe_attributes_accelerometer(
  bio_hub_sensor_attr_t *max_attr)
{
  uint8_t temp_array[2] = { 0x00 };
  sl_status_t sc;

  if (max_attr == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sc = read_fill_array(READ_ATTRIBUTES_AFE,
                       RETRIEVE_AFE_ACCELEROMETER,
                       2,
                       temp_array);
  max_attr->byte_word = temp_array[0];
  max_attr->avail_registers = temp_array[1];
  return sc;
}

/***************************************************************************//**
 * @brief
 *    Family Byte: DUMP_REGISTERS (0x43),
 *    Index Byte: DUMP_REGISTER_MAX30101 (0x03)
 *    This function returns all registers and register values sequentially of
 *    the MAX30101 sensor: register zero and register value zero to register n
 *    and register value n. There are 36 registers in this case.
 *
 * @param[in] reg_array
 * @param[in] num_of_bytes
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_dump_register_max30101(uint8_t *reg_array,
                                           uint8_t num_of_bytes)
{
  if (reg_array == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_fill_array(DUMP_REGISTERS,
                         DUMP_REGISTER_MAX30101,
                         num_of_bytes,
                         reg_array);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: DUMP_REGISTERS (0x43),
 *    Index Byte: DUMP_REGISTER_ACCELEROMETER (0x04)
 *    This function returns all registers and register values sequentially of
 *    the Accelerometer: register zero and register value zero to register n
 *    and register value n.
 *
 * @param[in] num_reg
 * @param[in] reg_array
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_dump_register_accelerometer(uint8_t num_reg,
                                                uint8_t *reg_array)
{
  if (reg_array == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  // Fake read amount
  return read_fill_array(DUMP_REGISTERS,
                         DUMP_REGISTER_ACCELEROMETER,
                         num_reg,
                         reg_array);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: SET_TARG_PERC (0x00),
 *    Write Byte: AGC_GAIN_ID (0x00)
 *    This function sets the target percentage of the full-scale ADC range that
 *    the automatic gain control algorithm uses. It takes a paramater of zero to
 *    100 percent.
 *
 * @param[in] perc
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_algo_range(uint8_t perc)
{
  if (perc > 100) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Successful communication or no?
  return write_byte2(CHANGE_ALGORITHM_CONFIG, SET_TARG_PERC, AGC_GAIN_ID, perc);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: SET_STEP_SIZE (0x00),
 *    Write Byte: AGC_STEP_SIZE_ID (0x01)
 *    This function changes the step size
 *    toward the target for the AGC algorithm.
 *    It takes a paramater of zero to 100 percent.
 *
 * @param[in] step
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_algo_step_size(uint8_t step)
{
  if (step > 100) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Successful communication or no?
  return write_byte2(CHANGE_ALGORITHM_CONFIG,
                     SET_STEP_SIZE,
                     AGC_STEP_SIZE_ID,
                     step);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: SET_SENSITIVITY (0x00),
 *    Write Byte: AGC_SENSITIVITY_ID (0x02)
 *    This function changes the sensitivity of the AGC algorithm.
 *
 * @param[in] sense
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_algo_sensitivity(uint8_t sense)
{
  if (sense > 100) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Successful communication or no?
  return write_byte2(CHANGE_ALGORITHM_CONFIG,
                     SET_SENSITIVITY,
                     AGC_SENSITIVITY_ID,
                     sense);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: SET_AVG_SAMPLES (0x00),
 *    Write Byte: AGC_NUM_SAMP_ID (0x03)
 *    This function changes the number of samples that are averaged.
 *    It takes a paramater of zero to 255.
 *
 * @param[in] avg
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_algo_samples(uint8_t avg)
{
  // Successful communication or no?
  return write_byte2(CHANGE_ALGORITHM_CONFIG,
                     SET_AVG_SAMPLES,
                     AGC_NUM_SAMP_ID,
                     avg);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: SET_PULSE_OX_COEF (0x02),
 *    Write Byte: MAXIMFAST_COEF_ID (0x0B)
 *    This function takes three values that are used as the Sp02 coefficients.
 *
 * @param[in] coef1, coef2, coef3
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_maxim_fast_coef(int32_t coef1,
                                        int32_t coef2,
                                        int32_t coef3)
{
  int32_t coef_arr[3] = { coef1, coef2, coef3 };

  return write_long_bytes(CHANGE_ALGORITHM_CONFIG,
                          SET_PULSE_OX_COEF,
                          MAXIMFAST_COEF_ID,
                          coef_arr,
                          3);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_ALGORITHM_CONFIG (0x51),
 *    Index Byte: READ_AGC_PERCENTAGE (0x00),
 *    Write Byte: READ_AGC_PERC_ID (0x00)
 *    This function reads and returns the currently set target percentage
 *    of the full-scale ADC range
 *    that the Automatic Gain Control algorithm is using.
 *
 * @param[out] range
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_algo_range(uint8_t *range)
{
  if (range == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte2(READ_ALGORITHM_CONFIG,
                    READ_AGC_PERCENTAGE,
                    READ_AGC_PERC_ID,
                    range);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_ALGORITHM_CONFIG (0x51),
 *    Index Byte: READ_AGC_STEP_SIZE (0x00),
 *    Write Byte: READ_AGC_STEP_SIZE_ID (0x01)
 *    This function returns the step size toward
 *    the target for the AGC algorithm.
 *    It returns a value between zero and 100 percent.
 *
 * @param[out] step_size
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_algo_step_size(uint8_t *step_size)
{
  if (step_size == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte2(READ_ALGORITHM_CONFIG,
                    READ_AGC_STEP_SIZE,
                    READ_AGC_STEP_SIZE_ID,
                    step_size);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_ALGORITHM_CONFIG (0x51),
 *    Index Byte: READ_AGC_SENSITIVITY_ID (0x00),
 *    Write Byte: READ_AGC_SENSITIVITY_ID (0x02)
 *    This function returns the sensitivity (percentage)
 *    of the automatic gain control.
 *
 * @param[out] sensitivity
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_algo_sensitivity(uint8_t *sensitivity)
{
  if (sensitivity == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte2(READ_ALGORITHM_CONFIG,
                    READ_AGC_SENSITIVITY,
                    READ_AGC_SENSITIVITY_ID,
                    sensitivity);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_ALGORITHM_CONFIG (0x51),
 *    Index Byte: READ_AGC_NUM_SAMPLES (0x00),
 *    Write Byte: READ_AGC_NUM_SAMPlES_ID (0x03)
 *    This function changes the number of samples that are averaged.
 *    It takes a parameter of zero to 255.
 *
 * @param[out] samples
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_algo_samples(uint8_t *samples)
{
  if (samples == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  return read_byte2(READ_ALGORITHM_CONFIG,
                    READ_AGC_NUM_SAMPLES,
                    READ_AGC_NUM_SAMPLES_ID,
                    samples);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: READ_ALGORITHM_CONFIG (0x51),
 *    Index Byte: READ_MAX_FAST_COEF (0x02)
 *    Write Byte: READ_MAX_FAST_COEF_ID (0x0B)
 *    This function reads the maximum age for the wrist heart rate monitor
 *    (WHRM) algorithm. It returns three uint32_t integers that are
 *     multiplied by 100,000.
 *
 * @param[in] coef_arr
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_maxim_fast_coef(int32_t *coef_arr)
{
  sl_status_t sc;

  if (coef_arr == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sc = read_multiple_bytes_int(READ_ALGORITHM_CONFIG,
                               READ_MAX_FAST_COEF,
                               READ_MAX_FAST_COEF_ID,
                               3,
                               coef_arr);
  coef_arr[0] = coef_arr[0] * 100000;
  coef_arr[1] = coef_arr[1] * 100000;
  coef_arr[2] = coef_arr[2] * 100000;
  return sc;
}

/***************************************************************************//**
 * @brief
 *    Family Byte: ENABLE_ALGORITHM (0x52), Index Byte: ENABLE_AGC_ALGO (0x00)
 *    This function enables (one) or disables (zero) the automatic gain control
 *    algorithm.
 *
 * @param[in] enable
 *    false: Disable
 *    true:  Enable
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_agc_algo_control(bool enable)
{
  return enable_write(ENABLE_ALGORITHM, ENABLE_AGC_ALGO, enable ? 1 : 0);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: ENABLE_ALGORITHM (0x52), Index Byte: ENABLE_WHRM_ALGO (0x02)
 *    This function enables (one)
 *    or disables (zero) the wrist heart rate monitor algorithm.
 *
 * @param[in] mode
 *    Control mode
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_maxim_fast_algo_control(bio_hub_algo_mode_t mode)
{
  if (mode > BIO_HUB_ALGO_MODE_TWO) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return enable_write(ENABLE_ALGORITHM, ENABLE_WHRM_ALGO, mode);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: BOOTLOADER_FLASH (0x80), Index Byte: SET_NUM_PAGES (0x02),
 *    Write Bytes: 0x00 - Number of pages at byte 0x44 from .msbl file.
 *
 * @param[in] total_pages
 *    Number of pages
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_num_pages(uint8_t total_pages)
{
  return write_byte2(BOOTLOADER_FLASH, SET_NUM_PAGES, 0x00, total_pages);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: BOOTLOADER_FLASH (0x80), Index Byte: ERASE_FLASH (0x03)
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_erase_flash(void)
{
  uint8_t buf[2];
  uint8_t status;

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  buf[0] = BOOTLOADER_FLASH;
  buf[1] = ERASE_FLASH;

  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             buf,
                                             2)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                            &status,
                                            1)) {
    return SL_STATUS_TRANSMIT;
  }

  return (status == 0) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

/***************************************************************************//**
 * @brief
 *    Family Byte: BOOTLOADER_INFO (0x81), Index Byte: BOOTLOADER_VERS (0x00).
 *
 * @param[out] bio_hub_vers
 *    Bootloader Version
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_bootloader_vers(bio_hub_version_t *boot_vers)
{
  if (boot_vers == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return read_version(BOOTLOADER_INFO, BOOTLOADER_VERS, boot_vers);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: IDENTITY (0xFF), Index Byte: READ_SENSOR_HUB_VERS (0x03).
 *
 * @param[out] bio_hub_vers
 *    HUB Version
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_sensor_hub_version(bio_hub_version_t *bio_hub_vers)
{
  if (bio_hub_vers == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return read_version(IDENTITY, READ_SENSOR_HUB_VERS, bio_hub_vers);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: IDENTITY (0xFF), Index Byte: READ_ALGO_VERS (0x07).
 *
 * @param[out] lib_algo_vers
 *    algorithm version
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_algorithm_version(bio_hub_version_t *lib_algo_vers)
{
  if (lib_algo_vers == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return read_version(IDENTITY, READ_SENSOR_HUB_VERS, lib_algo_vers);
}

// ------------------Function Below for MAX32664 Version D (Blood Pressure) ----

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: BPT_MEDICATION (0x00).
 *
 * @param[in] medication
 *    MEDICATION value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_write_patient_bp_medication(bool medication)
{
  return write_byte2(CHANGE_ALGORITHM_CONFIG,
                     BPT_CONFIG,
                     BPT_MEDICATION,
                     medication);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: BPT_MEDICATION (0x00).
 *
 * @param[out] medication
 *    MEDICATION value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_patient_bp_medication(uint8_t *medication)
{
  if (medication == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return read_byte2(CHANGE_ALGORITHM_CONFIG,
                    BPT_CONFIG,
                    BPT_MEDICATION,
                    medication);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: SYSTOLIC_VALUE (0x01).
 *
 * @param[in] sys_val1, sys_val2, sys_val3
 *    Systolic value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_write_systolic_vals(uint8_t sys_val1,
                                        uint8_t sys_val2,
                                        uint8_t sys_val3)
{
  uint8_t sys_vals[3] = { sys_val1, sys_val2, sys_val3 };

  return write_bytes(CHANGE_ALGORITHM_CONFIG,
                     BPT_CONFIG,
                     SYSTOLIC_VALUE,
                     sys_vals,
                     3);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: SYSTOLIC_VALUE (0x01).
 *
 * @param[out] user_array
 *    Systolic value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_systolic_vals(uint8_t *user_array)
{
  if (user_array == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return read_multiple_bytes(CHANGE_ALGORITHM_CONFIG,
                             BPT_CONFIG,
                             SYSTOLIC_VALUE,
                             3,
                             user_array);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: DIASTOLIC_VALUE (0x02).
 *
 * @param[in] dias_val1, dias_val2, dias_val3
 *    DIASTOLIC value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_write_diastolic_vals(uint8_t dias_val1,
                                         uint8_t dias_val2,
                                         uint8_t dias_val3)
{
  uint8_t dias_vals[3] = { dias_val1, dias_val2, dias_val3 };

  return write_bytes(CHANGE_ALGORITHM_CONFIG,
                     BPT_CONFIG,
                     DIASTOLIC_VALUE,
                     dias_vals,
                     3);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: DIASTOLIC_VALUE (0x02).
 *
 * @param[out] user_array
 *    DIASTOLIC value
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_diastolic_vals(uint8_t *user_array)
{
  if (user_array == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return read_multiple_bytes(CHANGE_ALGORITHM_CONFIG,
                             BPT_CONFIG,
                             DIASTOLIC_VALUE,
                             3,
                             user_array);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: BPT_CALIB_DATA (0x03).
 *
 * @param[in] bpt_calib_data
 *    Algorithm calibrate data
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_write_bpt_algo_data(const uint8_t *bpt_calib_data)
{
  if (bpt_calib_data == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return write_bytes(CHANGE_ALGORITHM_CONFIG,
                     BPT_CONFIG,
                     BPT_CALIB_DATA,
                     bpt_calib_data,
                     824);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: BPT_CALIB_DATA (0x03).
 *
 * @param[out] user_array
 *    Buffer to read algorithm data
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_bpt_algo_data(uint8_t *user_array)
{
  if (user_array == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return read_multiple_bytes(CHANGE_ALGORITHM_CONFIG,
                             BPT_CONFIG,
                             BPT_CALIB_DATA,
                             824,
                             user_array);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: PATIENT_RESTING (0x05).
 *
 * @param[in] resting
 *    Patient resting
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_write_patient_resting(bool resting)
{
  return write_byte2(CHANGE_ALGORITHM_CONFIG,
                     BPT_CONFIG,
                     PATIENT_RESTING,
                     resting);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: PATIENT_RESTING (0x05).
 *
 * @param[out] resting
 *    Patient resting status
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_patient_resting(uint8_t *resting)
{
  if (resting == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return read_byte2(CHANGE_ALGORITHM_CONFIG,
                    BPT_CONFIG,
                    PATIENT_RESTING,
                    resting);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: AGC_SP02_COEFS (0x0B).
 *
 * @param[in] int_A, int_B, int_C
 *    Algorithm coefficient to be write
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_write_spo2_algo_coef(int32_t int_A,
                                         int32_t int_B,
                                         int32_t int_C)
{
  int32_t coefVals[3] = { int_A, int_B, int_C };

  return write_long_bytes(CHANGE_ALGORITHM_CONFIG,
                          BPT_CONFIG,
                          AGC_SP02_COEFS,
                          coefVals,
                          3);
}

/***************************************************************************//**
 * @brief
 *    Family Byte: CHANGE_ALGORITHM_CONFIG (0x50),
 *    Index Byte: BPT_CONFIG (0x04),
 *    Write Byte: AGC_SP02_COEFS (0x0B).
 *
 * @param[out] user_array
 *    Buffer to read algorithm coefficient data
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_spo2_algo_coef(int32_t *user_array)
{
  if (user_array == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Have the user provide their own array here and pass the pointer to it
  return read_multiple_bytes_int(CHANGE_ALGORITHM_CONFIG,
                                 BPT_CONFIG,
                                 AGC_SP02_COEFS,
                                 3,
                                 user_array);
}

// ------------------- Private Functions -----------------------

static sl_status_t read_version(uint8_t family,
                                uint8_t index,
                                bio_hub_version_t *vers)
{
  uint8_t buf[4];

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  buf[0] = family;
  buf[1] = index;

  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             buf,
                                             2)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                            buf,
                                            4)) {
    return SL_STATUS_TRANSMIT;
  }

  // Pass through if SUCCESS (0x00).
  if (buf[0]) {
    vers->major = 0;
    vers->minor = 0;
    vers->revision = 0;
    return SL_STATUS_FAIL;
  }

  vers->major = buf[1];
  vers->minor = buf[2];
  vers->revision = buf[3];

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    This function uses the given family, index, and write byte to enable
 *    the given sensor.
 ******************************************************************************/
static sl_status_t enable_write(uint8_t family_byte,
                                uint8_t index_byte,
                                uint8_t enable_byte)
{
  uint8_t write_buf[3];
  uint8_t statusByte;

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  write_buf[0] = family_byte;
  write_buf[1] = index_byte;
  write_buf[2] = enable_byte;

  // Write buffer
  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             write_buf,
                                             3)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(ENABLE_CMD_DELAY);

  // Read buffer
  if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                            &statusByte,
                                            1)) {
    return SL_STATUS_TRANSMIT;
  }

  return (statusByte == 0) ? SL_STATUS_OK : SL_STATUS_TRANSMIT;
}

/***************************************************************************//**
 * @brief
 *   This function uses the given family, index, and write byte to communicate
 *   with the MAX32664 which in turn communicates with downward sensors. There
 *   are two steps demonstrated in this function. First a write to the MCU
 *   indicating what you want to do, a delay,
 *   and then a read to confirm positive transmission.
 ******************************************************************************/
static sl_status_t write_byte(uint8_t family_byte,
                              uint8_t index_byte,
                              uint8_t write_byte)
{
  uint8_t write_buf[3];
  uint8_t statusByte;

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  write_buf[0] = family_byte;
  write_buf[1] = index_byte;
  write_buf[2] = write_byte;

  // Write buffer
  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             write_buf,
                                             3)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  // Read buffer
  if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                            &statusByte,
                                            1)) {
    return SL_STATUS_TRANSMIT;
  }

  return (statusByte == 0) ? SL_STATUS_OK : SL_STATUS_TRANSMIT;
}

/***************************************************************************//**
 * @brief
 *   This function sends information to the MAX32664 to specifically write
 *   values to the registers of downward sensors and so also requires a
 *   register address and register value as parameters.
 *   Again there is the write of the specific bytes followed by
 *   a read to confirm positive transmission.
 ******************************************************************************/
static sl_status_t write_byte2(uint8_t family_byte,
                               uint8_t index_byte,
                               uint8_t write_byte,
                               uint8_t write_val)
{
  uint8_t write_buf[4];
  uint8_t statusByte;

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  write_buf[0] = family_byte;
  write_buf[1] = index_byte;
  write_buf[2] = write_byte;
  write_buf[3] = write_val;

  // Write buffer
  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             write_buf,
                                             4)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                            &statusByte,
                                            1)) {
    return SL_STATUS_TRANSMIT;
  }

  return (statusByte == 0) ? SL_STATUS_OK : SL_STATUS_TRANSMIT;
}

/***************************************************************************//**
 * @brief
 *   This function sends information to the MAX32664 to specifically write
 *   values to the registers of downward sensors and so also requires a
 *   register address and register value as parameters. Again there is the write
 *   of the specific bytes followed by a read to confirm positive transmission.
 ******************************************************************************/
static sl_status_t write_long_bytes(uint8_t family_byte,
                                    uint8_t index_byte,
                                    uint8_t write_byte,
                                    const int32_t *write_val,
                                    const size_t size)
{
  uint8_t write_buf[(size * 4) + 3];
  uint8_t statusByte;

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  write_buf[0] = family_byte;
  write_buf[1] = index_byte;
  write_buf[2] = write_byte;
  memcpy(&write_buf[3], write_val, size * 4);

  // Write buffer
  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             write_buf,
                                             (size * 4) + 3)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                            &statusByte,
                                            1)) {
    return SL_STATUS_TRANSMIT;
  }

  return (statusByte == 0) ? SL_STATUS_OK : SL_STATUS_TRANSMIT;
}

/***************************************************************************//**
 * @brief
 *   This function sends information to the MAX32664 to specifically write
 *   values to the registers of downward sensors and so also requires a
 *   register address and register value as parameters. Again there is the write
 *   of the specific bytes followed by a read to confirm positive transmission.
 ******************************************************************************/
static sl_status_t write_bytes(uint8_t family_byte,
                               uint8_t index_byte,
                               uint8_t write_byte,
                               const uint8_t *write_val,
                               size_t size)
{
  uint8_t write_buf[size + 3];
  uint8_t statusByte;

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  write_buf[0] = family_byte;
  write_buf[1] = index_byte;
  write_buf[2] = write_byte;
  memcpy(&write_buf[3], write_val, size);

  // Write buffer
  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             write_buf,
                                             size + 3)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                            &statusByte,
                                            1)) {
    return SL_STATUS_TRANSMIT;
  }

  return (statusByte == 0) ? SL_STATUS_OK : SL_STATUS_TRANSMIT;
}

/***************************************************************************//**
 * @brief
 *   This function handles all read commands or stated another way,
 *   all information requests.
 *   It starts a request by writing the family byte an index byte,
 *   and then delays 60 microseconds,
 *   during which the MAX32664 retrieves the requested information.
 *   An I-squared-C request is then issued, and the information is read.
 ******************************************************************************/
static sl_status_t read_byte(uint8_t family_byte,
                             uint8_t index_byte,
                             uint8_t *byte_read)
{
  uint8_t write_buf[2];
  uint8_t read_buf[2];

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  write_buf[0] = family_byte;
  write_buf[1] = index_byte;

  // Write buffer
  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             write_buf,
                                             2)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                            read_buf,
                                            2)) {
    return SL_STATUS_TRANSMIT;
  }

  if (read_buf[0]) {
    return SL_STATUS_IO;  // Return the error, see: read_status_byte_value_t
  }

  *byte_read = read_buf[1];
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   This function is exactly as the one above except it accepts also receives a
 *   Write Byte as a parameter. It starts a request by writing the family byte,
 *   index byte, and write byte to the MAX32664 and then delays 60 microseconds,
 *   during which the MAX32664 retrieves the requested information.
 *   A I-squared-C request is then issued, and the information is read.
 ******************************************************************************/
static sl_status_t read_byte2(uint8_t family_byte,
                              uint8_t index_byte,
                              uint8_t write_byte,
                              uint8_t *byte_read)
{
  uint8_t write_buf[3];
  uint8_t read_buf[2];

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  write_buf[0] = family_byte;
  write_buf[1] = index_byte;
  write_buf[2] = write_byte;

  // Write buffer
  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             write_buf,
                                             3)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  // Read buffer
  if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                            read_buf,
                                            2)) {
    return SL_STATUS_TRANSMIT;
  }

  if (read_buf[0]) {
    return SL_STATUS_IO; // Return the error, see: read_status_byte_value_t
  }

  *byte_read = read_buf[1];
  return SL_STATUS_OK;
}

static sl_status_t read_fill_array(uint8_t family_byte,
                                   uint8_t index_byte,
                                   uint8_t num_of_reads,
                                   uint8_t *array)
{
  uint8_t write_buf[2];

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  write_buf[0] = family_byte;
  write_buf[1] = index_byte;

  // Write buffer
  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             write_buf,
                                             2)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                            array,
                                            num_of_reads)) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *    This function handles all read commands or stated another way,
 *    all information requests.
 *    It starts a request by writing the family byte, an index byte, and
 *    a write byte and then then delays 60 microseconds,
 *    during which the MAX32664 retrieves the requested information.
 *    An I-squared-C request is then issued, and the information is read.
 *    This function is very similar to the one above
 *    except it returns three uint32_t bytes instead of one.
 ******************************************************************************/
static sl_status_t read_multiple_bytes_int(uint8_t family_byte,
                                           uint8_t index_byte,
                                           uint8_t write_byte,
                                           const size_t num_of_reads,
                                           int32_t *user_array)
{
  uint8_t write_buf[3];

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  write_buf[0] = family_byte;
  write_buf[1] = index_byte;
  write_buf[2] = write_byte;

  // Write buffer
  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             write_buf,
                                             3)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  if ((num_of_reads * 4) < 255) {
    uint8_t read_buf[(num_of_reads * 4) + 1];

    if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                              read_buf,
                                              (num_of_reads * 4) + 1)) {
      return SL_STATUS_TRANSMIT;
    }

    if (read_buf[0]) {
      return SL_STATUS_IO; // Return the error, see: read_status_byte_value_t
    }
    memcpy(user_array, &read_buf[1], num_of_reads * 4);
  } else {
    return SL_STATUS_WOULD_OVERFLOW;
  }

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   This function handles all read commands or stated another way,
 *   all information requests.
 *   It starts a request by writing the family byte, an index byte, and
 *   a write byte and then then delays 60 microseconds,
 *   during which the MAX32664 retrieves the requested information.
 *   An I-squared-C request is then issued,
 *   and the information is read. This function is very similar to the one above
 *   except it returns three uint32_t bytes instead of one.
 ******************************************************************************/
static sl_status_t read_multiple_bytes(uint8_t family_byte,
                                       uint8_t index_byte,
                                       uint8_t write_byte,
                                       const size_t num_of_reads,
                                       uint8_t *user_array)
{
  uint8_t write_buf[3];

  if (bio_hub.handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  write_buf[0] = family_byte;
  write_buf[1] = index_byte;
  write_buf[2] = write_byte;

  // Write buffer
  if (I2C_MASTER_SUCCESS != i2c_master_write(&bio_hub,
                                             write_buf,
                                             3)) {
    return SL_STATUS_TRANSMIT;
  }

  sl_sleeptimer_delay_millisecond(CMD_DELAY);

  if (num_of_reads < 255) {
    uint8_t read_buf[num_of_reads + 1];

    if (I2C_MASTER_SUCCESS != i2c_master_read(&bio_hub,
                                              read_buf,
                                              num_of_reads + 1)) {
      return SL_STATUS_TRANSMIT;
    }

    if (read_buf[0]) {
      return SL_STATUS_IO; // Return the error, see: read_status_byte_value_t
    }

    memcpy(user_array, &read_buf[1], num_of_reads);
  } else {
    return SL_STATUS_WOULD_OVERFLOW;
  }

  return SL_STATUS_OK;
}
