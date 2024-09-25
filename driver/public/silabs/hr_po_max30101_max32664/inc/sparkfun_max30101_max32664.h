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
#ifndef SPARKFUN_BIO_SENSOR_HUB_LIBRARY_H_
#define SPARKFUN_BIO_SENSOR_HUB_LIBRARY_H_

#include <stdbool.h>
#include "drv_i2c_master.h"

/***************************************************************************//**
 * @addtogroup MAX32664
 * @{
 *
 * @brief
 *  The implementation of a Biometric Sensor Hub.
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define MAX30101_CONFIGURATION_REGISTER 0x0A
#define MAX30101_PULSE_MASK             0xFC
#define MAX30101_READ_PULSE_MASK        0x03
#define MAX30101_SAMP_MASK              0xE3
#define MAX30101_READ_SAMP_MASK         0x1C
#define MAX30101_ADC_MASK               0x9F
#define MAX30101_READ_ADC_MASK          0x60

typedef enum BIO_HUB_ALGO_MODE {
  BIO_HUB_ALGO_DISABLE = 0,
  BIO_HUB_ALGO_MODE_ONE,
  BIO_HUB_ALGO_MODE_TWO
} bio_hub_algo_mode_t;

// Write Bytes under Family Byte: OUTPUT_MODE (0x10)
// and Index byte: SET_FORMAT (0x00)
typedef enum BIO_HUB_OUTPUT_MODE_WRITE_BYTE {
  BIO_HUB_OUTPUT_MODE_PAUSE                 = 0x00,
  BIO_HUB_OUTPUT_MODE_SENSOR_DATA,
  BIO_HUB_OUTPUT_MODE_ALGO_DATA,
  BIO_HUB_OUTPUT_MODE_SENSOR_AND_ALGORITHM,
  BIO_HUB_OUTPUT_MODE_PAUSE_TWO,
  BIO_HUB_OUTPUT_MODE_SENSOR_COUNTER_BYTE,
  BIO_HUB_OUTPUT_MODE_ALGO_COUNTER_BYTE,
  BIO_HUB_OUTPUT_MODE_SENSOR_ALGO_COUNTER
} bio_hub_output_mode_write_byte_t;

typedef enum MAX30101_SAMPLE_RATE {
  MAX30101_SAMPLE_RATE_50 = 50,
  MAX30101_SAMPLE_RATE_100 = 100,
  MAX30101_SAMPLE_RATE_200 = 200,
  MAX30101_SAMPLE_RATE_400 = 400,
  MAX30101_SAMPLE_RATE_800 = 800,
  MAX30101_SAMPLE_RATE_1000 = 1000,
  MAX30101_SAMPLE_RATE_1600 = 1600,
  MAX30101_SAMPLE_RATE_3200 = 3200
} max30101_sample_rate_t;

typedef enum MAX30101_PULSE_WIDTH {
  MAX30101_PULSE_WIDTH_69 = 69,
  MAX30101_PULSE_WIDTH_118 = 118,
  MAX30101_PULSE_WIDTH_215 = 215,
  MAX30101_PULSE_WIDTH_411 = 411
} max30101_pulse_width_t;

// Write Bytes under Family Byte: SET_DEVICE_MODE (0x01)
// and Index Byte: 0x00.
typedef enum BIO_HUB_DEVICE_MODE_WRITE_BYTE {
  BIO_HUB_EXIT_BOOTLOADER          = 0x00,
  BIO_HUB_RESET                    = 0x02,
  BIO_HUB_ENTER_BOOTLOADER         = 0x08
} bio_hub_device_mode_write_byte_t;

// Status Bytes are communicated back after every I-squared-C transmission and
// are indicators of success or failure of the previous transmission.
typedef enum READ_STATUS_BYTE_VALUE {
  SUCCESS                  = 0x00,
  ERR_UNAVAIL_CMD,
  ERR_UNAVAIL_FUNC,
  ERR_DATA_FORMAT,
  ERR_INPUT_VALUE,
  ERR_TRY_AGAIN,
  ERR_BTLDR_GENERAL        = 0x80,
  ERR_BTLDR_CHECKSUM,
  ERR_BTLDR_AUTH,
  ERR_BTLDR_INVALID_APP,
  ERR_UNKNOWN              = 0xFF
} read_status_byte_value_t;

// The family register bytes are the larger umbrella for all the Index and
// Write Bytes listed below. You can not reference a nestled byte without first
// referencing it's larger category: Family Register Byte.
typedef enum FAMILY_REGISTER_BYTES {
  HUB_STATUS               = 0x00,
  SET_DEVICE_MODE,
  READ_DEVICE_MODE,
  OUTPUT_MODE              = 0x10,
  READ_OUTPUT_MODE,
  READ_DATA_OUTPUT,
  READ_DATA_INPUT,
  WRITE_INPUT,
  WRITE_REGISTER           = 0x40,
  READ_REGISTER,
  READ_ATTRIBUTES_AFE,
  DUMP_REGISTERS,
  ENABLE_SENSOR,
  READ_SENSOR_MODE,
  CHANGE_ALGORITHM_CONFIG  = 0x50,
  READ_ALGORITHM_CONFIG,
  ENABLE_ALGORITHM,
  BOOTLOADER_FLASH         = 0x80,
  BOOTLOADER_INFO,
  IDENTITY                 = 0xFF
} family_register_bytes_t;

// Index Byte under the Family Byte: READ_DATA_OUTPUT (0x12)
typedef enum FIFO_OUTPUT_INDEX_BYTE {
  NUM_SAMPLES,
  READ_DATA
} fifo_output_index_byte_t;

// Index Byte under the Family Byte: READ_DATA_INPUT (0x13)
typedef enum FIFO_EXTERNAL_INDEX_BYTE {
  SAMPLE_SIZE,
  READ_INPUT_DATA,
  READ_SENSOR_DATA, // For external accelerometer
  READ_NUM_SAMPLES_INPUT, // For external accelerometer
  READ_NUM_SAMPLES_SENSOR
}fifo_external_index_byte_t;

// Index Byte under the Family Registry Byte: WRITE_REGISTER (0x40)
typedef enum WRITE_REGISTER_INDEX_BYTE {
  WRITE_MAX30101 = 0x03,
  WRITE_ACCELEROMETER
} write_register_index_byte_t;

// Index Byte under the Family Registry Byte: READ_REGISTER (0x41)
typedef enum READ_REGISTER_INDEX_BYTE {
  READ_MAX30101 = 0x03,
  READ_ACCELEROMETER
} read_register_index_byte_t;

// Index Byte under the Family Registry Byte: READ_ATTRIBUTES_AFE (0x42)
typedef enum GET_AFE_INDEX_BYTE {
  RETRIEVE_AFE_MAX30101 = 0x03,
  RETRIEVE_AFE_ACCELEROMETER
} get_afe_index_byte_t;

// Index Byte under the Family Byte: DUMP_REGISTERS (0x43)
typedef enum DUMP_REGISTER_INDEX_BYTE {
  DUMP_REGISTER_MAX30101 = 0x03,
  DUMP_REGISTER_ACCELEROMETER
} dump_register_index_byte_t;

// Index Byte under the Family Byte: ENABLE_SENSOR (0x44)
typedef enum SENSOR_ENABLE_INDEX_BYTE {
  ENABLE_MAX30101 = 0x03,
  ENABLE_ACCELEROMETER
} sensor_enable_index_byte_t;

// Index Byte for the Family Byte: READ_SENSOR_MODE (0x45)
typedef enum READ_SENSOR_ENABLE_INDEX_BYTE {
  READ_ENABLE_MAX30101 = 0x03,
  READ_ENABLE_ACCELEROMETER
} read_sensor_enable_index_byte_t;

// Index Byte under the Family Byte: CHANGE_ALGORITHM_CONFIG (0x50)
typedef enum ALGORITHM_CONFIG_INDEX_BYTE {
  SET_TARG_PERC            = 0x00,
  SET_STEP_SIZE            = 0x00,
  SET_SENSITIVITY          = 0x00,
  SET_AVG_SAMPLES          = 0x00,
  SET_PULSE_OX_COEF        = 0x02,
  BPT_CONFIG               = 0x04
} algorithm_config_index_byte_t;

// Write Bytes under the Family Byte: CHANGE_ALGORITHM_CONFIG (0x50) and the
// Index Byte: ALGORITHM_CONFIG_INDEX_BYTE - SET_TARG_PERC
typedef enum ALGO_AGC_WRITE_BYTE {
  AGC_GAIN_ID              = 0x00,
  AGC_STEP_SIZE_ID,
  AGC_SENSITIVITY_ID,
  AGC_NUM_SAMP_ID,
  MAXIMFAST_COEF_ID        = 0x0B
} algo_agc_write_byte_t;

typedef enum ALGO_BPT_WRITE_BYTE {
  BPT_MEDICATION           = 0x00,
  SYSTOLIC_VALUE,
  DIASTOLIC_VALUE,
  BPT_CALIB_DATA,           // Index + 824 bytes of calibration data
  PATIENT_RESTING          = 0x05,
  AGC_SP02_COEFS           = 0x0B
} algo_bpt_write_byte_t;

// Index Bytes under the Family Byte: READ_ALGORITHM_CONFIG (0x51)
typedef enum READ_ALGORITHM_INDEX_BYTE {
  READ_AGC_PERCENTAGE      = 0x00,
  READ_AGC_STEP_SIZE       = 0x00,
  READ_AGC_SENSITIVITY     = 0x00,
  READ_AGC_NUM_SAMPLES     = 0x00,
  READ_MAX_FAST_COEF       = 0x02
} read_algorithm_index_byte_t;

// Write Bytes under the Family Byte: READ_ALGORITHM_CONFIG (0x51)
// and Index Byte: READ_ALGORITHM_INDEX_BYTE - AGC
typedef enum READ_AGC_ALGO_WRITE_BYTE {
  READ_AGC_PERC_ID           = 0x00,
  READ_AGC_STEP_SIZE_ID,
  READ_AGC_SENSITIVITY_ID,
  READ_AGC_NUM_SAMPLES_ID,
  READ_MAX_FAST_COEF_ID      = 0x0B
} read_agc_algo_write_byte_t;

// Index Byte under the Family Byte: ENABLE_ALGORITHM (0x52).
typedef enum ALGORITHM_MODE_ENABLE_INDEX_BYTE {
  ENABLE_AGC_ALGO  = 0x00,
  ENABLE_WHRM_ALGO = 0x02
} algorithm_mode_enable_index_byte_t;

// Index Byte under the Family Byte: BOOTLOADER_FLASH (0x80).
typedef enum BOOTLOADER_FLASH_INDEX_BYTE {
  SET_INIT_VECTOR_BYTES    = 0x00,
  SET_AUTH_BYTES,
  SET_NUM_PAGES,
  ERASE_FLASH,
  SEND_PAGE_VALUE
} bootloader_flash_index_byte_t;

// Index Byte under the Family Byte: BOOTLOADER_INFO (0x81).
typedef enum BOOTLOADER_INFO_INDEX_BYTE {
  BOOTLOADER_VERS          = 0x00,
  PAGE_SIZE
} bootloader_info_index_byte_t;

// Index Byte under the Family Byte: IDENTITY (0xFF).
typedef enum IDENTITY_INDEX_BYTES {
  READ_MCU_TYPE            = 0x00,
  READ_SENSOR_HUB_VERS     = 0x03,
  READ_ALGO_VERS           = 0x07
} identity_index_bytes_t;

typedef struct {
  uint32_t ir_led;
  uint32_t red_led;
  uint16_t heart_rate; // LSB = 0.1bpm
  uint8_t  confidence; // 0-100% LSB = 1%
  uint16_t oxygen; // 0-100% LSB = 1%
  uint8_t  status; // 0: Success, 1: Not Ready,
                   // 2: Object Detectected, 3: Finger Detected
  float    r_value; // -- Algorithm Mode 2 vv
  int8_t   ext_status; // --
  uint8_t  reserve_one; // --
  uint8_t  resserve_two; // -- Algorithm Mode 2 ^^
} bio_hub_data_t;

typedef struct {
  // 3 bytes total
  uint8_t major;
  uint8_t minor;
  uint8_t revision;
} bio_hub_version_t;

typedef struct {
  uint8_t byte_word;
  uint8_t avail_registers;
} bio_hub_sensor_attr_t;

// All the defines below are: 1. Index Bytes nestled in the larger category of
// the family registry bytes listed above and 2. The Write Bytes nestled even
// farther under their Index Bytes.

// Functions ------------

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
sl_status_t bio_hub_init(mikroe_i2c_handle_t i2cspm_instance, uint8_t address);

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
sl_status_t bio_hub_init_bootloader_mode(mikroe_i2c_handle_t i2cspm_instance,
                                         uint8_t address);

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
sl_status_t bio_hub_read_sensor_hub_status(uint8_t *status);

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
  bio_hub_device_mode_write_byte_t selection);

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
sl_status_t bio_hub_config_bpm(bio_hub_algo_mode_t mode);

/***************************************************************************//**
 * @brief
 *    This function sets very basic settings to get LED count values from
 *    the MAX30101. Sensor data includes 24 bit LED values for
 *    the three LED channels: Red, IR, and Green.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_config_sensor(void);

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
sl_status_t bio_hub_config_sensor_bpm(uint8_t mode);

/***************************************************************************//**
 * @brief
 *    This function takes the 8 bytes from the FIFO buffer related to the wrist
 *    heart rate algortihm: heart rate (uint16_t), confidence (uint8_t),
 *    SpO2 (uint16_t), and the finger detected status (uint8_t).
 *    Note that the the algorithm is stated as "wrist" though the sensor only
 *    works with the finger. The data is loaded into the whrmFifo and returned.
 *
 * @param[out] lib_bpm
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_bpm(bio_hub_data_t *lib_bpm);

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
sl_status_t bio_hub_read_sensor(bio_hub_data_t *lib_led_fifo);

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
sl_status_t bio_hub_read_sensor_bpm(bio_hub_data_t *lib_led_bpm);

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
sl_status_t bio_hub_set_pulse_width(max30101_pulse_width_t width);

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
sl_status_t bio_hub_read_pulse_width(uint16_t *width);

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
sl_status_t bio_hub_set_sample_rate(max30101_sample_rate_t samp_rate);

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
sl_status_t bio_hub_read_sample_rate(uint16_t *samp_rate);

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
sl_status_t bio_hub_set_adc_range(uint16_t adc_val);

/***************************************************************************//**
 * @brief
 *    MAX30101 Register: MAX30101_CONFIGURATION_REGISTER (0x0A), bits [6:5]
 *    This function returns the set ADC range of the MAX30101 sensor.
 *
 * @param[out] adc_val
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_adc_range(uint16_t *adc_val);

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
sl_status_t bio_hub_get_mcu_type(uint8_t *return_byte);

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
sl_status_t bio_hub_get_bootloader_inf(int32_t *boot_vers);

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
sl_status_t bio_hub_max30101_control(bool sen_switch);

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
sl_status_t bio_hub_read_max30101_state(uint8_t *state);

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
sl_status_t bio_hub_accel_control(bool accel_switch);

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
  bio_hub_output_mode_write_byte_t output_type);

/***************************************************************************//**
 * @brief
 *    Family Byte: OUTPUT_MODE,
 *    Index Byte: WRITE_SET_THRESHOLD,
 *    Write byte: intThres (parameter - value betwen 0 and 0xFF).
 *    This function changes the threshold for the FIFO interrupt bit/pin. The
 *    interrupt pin is the MFIO pin which is set to INPUT after
 *    IC initialization (begin).
 *
 * @param[in] int_thresh
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_set_fifo_threshold(uint8_t int_thresh);

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
sl_status_t bio_hub_num_samples_out_fifo(uint8_t *samp_avail);

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
sl_status_t bio_hub_get_data_out_fifo(uint8_t *buf, uint8_t *size);

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
sl_status_t bio_hub_num_samples_external_sensor(uint8_t *samp_avail);

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
sl_status_t bio_hub_write_register_max30101(uint8_t regAddr, uint8_t regVal);

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
sl_status_t bio_hub_write_register_accel(uint8_t reg_addr, uint8_t reg_val);

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
sl_status_t bio_hub_read_register_max30101(uint8_t reg_addr, uint8_t *reg_cont);

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
sl_status_t bio_hub_read_register_accel(uint8_t reg_addr, uint8_t *reg_cont);

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
sl_status_t bio_hub_get_afe_attributes_max30101(
  bio_hub_sensor_attr_t *max_attr);

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
  bio_hub_sensor_attr_t *max_attr);

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
                                           uint8_t num_of_bytes);

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
                                                uint8_t *reg_array);

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
sl_status_t bio_hub_set_algo_range(uint8_t perc);

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
sl_status_t bio_hub_set_algo_step_size(uint8_t step);

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
sl_status_t bio_hub_set_algo_sensitivity(uint8_t sense);

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
sl_status_t bio_hub_set_algo_samples(uint8_t avg);

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
                                        int32_t coef3);

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
sl_status_t bio_hub_read_algo_range(uint8_t *range);

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
sl_status_t bio_hub_read_algo_step_size(uint8_t *step_size);

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
sl_status_t bio_hub_read_algo_sensitivity(uint8_t *sensitivity);

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
sl_status_t bio_hub_read_algo_samples(uint8_t *samples);

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
sl_status_t bio_hub_read_maxim_fast_coef(int32_t *coef_arr);

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
sl_status_t bio_hub_agc_algo_control(bool enable);

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
sl_status_t bio_hub_maxim_fast_algo_control(bio_hub_algo_mode_t mode);

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
sl_status_t bio_hub_set_num_pages(uint8_t total_pages);

/***************************************************************************//**
 * @brief
 *    Family Byte: BOOTLOADER_FLASH (0x80), Index Byte: ERASE_FLASH (0x03)
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_erase_flash(void);

/***************************************************************************//**
 * @brief
 *    Family Byte: BOOTLOADER_INFO (0x81), Index Byte: BOOTLOADER_VERS (0x00).
 *
 * @param[out] bio_hub_vers
 *    Bootloader Version
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_bootloader_vers(bio_hub_version_t *boot_vers);

/***************************************************************************//**
 * @brief
 *    Family Byte: IDENTITY (0xFF), Index Byte: READ_SENSOR_HUB_VERS (0x03).
 *
 * @param[out] bio_hub_vers
 *    HUB Version
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_sensor_hub_version(bio_hub_version_t *bio_hub_vers);

/***************************************************************************//**
 * @brief
 *    Family Byte: IDENTITY (0xFF), Index Byte: READ_ALGO_VERS (0x07).
 *
 * @param[out] lib_algo_vers
 *    algorithm version
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
sl_status_t bio_hub_read_algorithm_version(bio_hub_version_t *lib_algo_vers);

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
sl_status_t bio_hub_write_patient_bp_medication(bool medication);

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
sl_status_t bio_hub_read_patient_bp_medication(uint8_t *medication);

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
                                         uint8_t dias_val3);

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
sl_status_t bio_hub_read_diastolic_vals(uint8_t *user_array);

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
                                        uint8_t sys_val3);

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
sl_status_t bio_hub_read_systolic_vals(uint8_t *user_array);

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
sl_status_t bio_hub_write_bpt_algo_data(const uint8_t *bpt_calib_data);

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
sl_status_t bio_hub_read_bpt_algo_data(uint8_t *user_array);

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
sl_status_t bio_hub_write_patient_resting(bool resting);

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
sl_status_t bio_hub_read_patient_resting(uint8_t *resting);

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
                                         int32_t int_C);

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
sl_status_t bio_hub_read_spo2_algo_coef(int32_t *user_array);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // SPARKFUN_BIO_SENSOR_HUB_LIBRARY_H_
