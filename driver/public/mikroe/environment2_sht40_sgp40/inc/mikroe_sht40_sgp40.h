/***************************************************************************//**
 * @file mikroe_sht4_sgp40.h
 * @brief SCL SHT40&SGP40 Prototypes
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

#ifndef MIKROE_SHT40_SGP40_H
#define MIKROE_SHT40_SGP40_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sl_status.h"
#include "environment2.h"
#include "drv_i2c_master.h"

/*!
 * @addtogroup environment2 Environment 2 Click Driver
 * @brief API for configuring and manipulating Environment 2 Click driver.
 * @{
 */

/**
 * @defgroup environment2_reg Environment 2 Registers List
 * @brief List of registers of Environment 2 Click driver.
 */

/**
 * @addtogroup environment2_cmd
 * @{
 */

/**
 * @brief Environment 2 SGP40 description commands.
 * @details Specified SGP40 commands for description of Environment 2 Click
 *   driver.
 */
#define MIKROE_ENVIRONMENT2_SGP40_CMD_MEASURE_RAW                       0x260F
#define MIKROE_ENVIRONMENT2_SGP40_CMD_MEASURE_TEST                      0x280E
#define MIKROE_ENVIRONMENT2_SGP40_CMD_HEATER_OFF                        0x3615
#define MIKROE_ENVIRONMENT2_SGP40_CMD_SOFT_RESET                        0x0006

/**
 * @brief Environment 2 SHT40 description commands.
 * @details Specified SHT40 commands for description of Environment 2 Click
 *   driver.
 */
#define MIKROE_ENVIRONMENT2_SHT40_CMD_MEASURE_T_RH_HIGH_PRECISION       0xFD
#define MIKROE_ENVIRONMENT2_SHT40_CMD_MEASURE_T_RH_MEDIUM_PRECISION     0xF6
#define MIKROE_ENVIRONMENT2_SHT40_CMD_MEASURE_T_RH_LOWEST_PRECISION     0xE0
#define MIKROE_ENVIRONMENT2_SHT40_CMD_READ_SERIAL                       0x89
#define MIKROE_ENVIRONMENT2_SHT40_CMD_SOFT_RESET                        0x94
#define MIKROE_ENVIRONMENT2_SHT40_CMD_ACTIVATE_HIGHEST_HEATER_1SEC      0x39
#define MIKROE_ENVIRONMENT2_SHT40_CMD_ACTIVATE_HIGHEST_HEATER_0_1SEC    0x32
#define MIKROE_ENVIRONMENT2_SHT40_CMD_ACTIVATE_MEDIUM_HEATER_1SEC       0x2F
#define MIKROE_ENVIRONMENT2_SHT40_CMD_ACTIVATE_MEDIUM_HEATER_0_1SEC     0x24
#define MIKROE_ENVIRONMENT2_SHT40_CMD_ACTIVATE_LOWEST_HEATER_1SEC       0x1E
#define MIKROE_ENVIRONMENT2_SHT40_CMD_ACTIVATE_LOWEST_HEATER_0_1SEC     0x15

/*! @} */ // environment2_cmd

/**
 * @defgroup environment2_set Environment 2 Registers Settings
 * @brief Settings for registers of Environment 2 Click driver.
 */

/**
 * @addtogroup environment2_set
 * @{
 */

/**
 * @brief Environment 2 device address setting.
 * @details Specified setting for device slave address selection of
 * Environment 2 Click driver.
 */
#define MIKROE_ENVIRONMENT2_SGP40_SET_DEV_ADDR                          0x59
#define MIKROE_ENVIRONMENT2_SHT40_SET_DEV_ADDR                          0x44

/**
 * @brief Environment 2 device selection.
 * @details Specified selection for device slave address of
 * Environment 2 Click driver.
 */
enum MIKROE_ENVIRONMENT2_DEVICE
{
  MIKROE_ENVIRONMENT2_SEL_SGP40,
  MIKROE_ENVIRONMENT2_SEL_SHT40
};

/**
 * @brief Environment 2 fixed point arithmetic parts.
 * @details Specified the fixed point arithmetic parts for VOC algorithm of
 * Environment 2 Click driver.
 */
#define MIKROE_F16(x) ((fix16_t)(((x) >= 0)              \
                                 ? ((x) * 65536.0 + 0.5) \
                                 : ((x) * 65536.0 - 0.5)))
#define MIKROE_VocAlgorithm_SAMPLING_INTERVAL                       (1.)
#define MIKROE_VocAlgorithm_INITIAL_BLACKOUT                        (45.)
#define MIKROE_VocAlgorithm_VOC_INDEX_GAIN                          (230.)
#define MIKROE_VocAlgorithm_SRAW_STD_INITIAL                        (50.)
#define MIKROE_VocAlgorithm_SRAW_STD_BONUS                          (220.)
#define MIKROE_VocAlgorithm_TAU_MEAN_VARIANCE_HOURS                 (12.)
#define MIKROE_VocAlgorithm_TAU_INITIAL_MEAN                        (20.)
#define MIKROE_VocAlgorithm_INIT_DURATION_MEAN                      ((3600. \
                                                                      * 0.75))
#define MIKROE_VocAlgorithm_INIT_TRANSITION_MEAN                    (0.01)
#define MIKROE_VocAlgorithm_TAU_INITIAL_VARIANCE                    (2500.)
#define MIKROE_VocAlgorithm_INIT_DURATION_VARIANCE                  ((3600. \
                                                                      * 1.45))
#define MIKROE_VocAlgorithm_INIT_TRANSITION_VARIANCE                (0.01)
#define MIKROE_VocAlgorithm_GATING_THRESHOLD                        (340.)
#define MIKROE_VocAlgorithm_GATING_THRESHOLD_INITIAL                (510.)
#define MIKROE_VocAlgorithm_GATING_THRESHOLD_TRANSITION             (0.09)
#define MIKROE_VocAlgorithm_GATING_MAX_DURATION_MINUTES             ((60. * 3.))
#define MIKROE_VocAlgorithm_GATING_MAX_RATIO                        (0.3)
#define MIKROE_VocAlgorithm_SIGMOID_L                               (500.)
#define MIKROE_VocAlgorithm_SIGMOID_K                               (-0.0065)
#define MIKROE_VocAlgorithm_SIGMOID_X0                              (213.)
#define MIKROE_VocAlgorithm_VOC_INDEX_OFFSET_DEFAULT                (100.)
#define MIKROE_VocAlgorithm_LP_TAU_FAST                             (20.0)
#define MIKROE_VocAlgorithm_LP_TAU_SLOW                             (500.0)
#define MIKROE_VocAlgorithm_LP_ALPHA                                (-0.2)
#define MIKROE_VocAlgorithm_PERSISTENCE_UPTIME_GAMMA                ((3. \
                                                                      * 3600.))
#define MIKROE_VocAlgorithm_MEAN_VARIANCE_ESTIMATOR__GAMMA_SCALING  (64.)
#define MIKROE_VocAlgorithm_MEAN_VARIANCE_ESTIMATOR__FIX16_MAX      (32767.)

/*! @} */ // environment2_set

/**
 * @addtogroup environment2_get
 * @{
 */

/**
 * @brief Environment 2 SGP40 description setting.
 * @details Specified SGP40 setting for description of Environment 2 Click
 *   driver.
 */
#define MIKROE_ENVIRONMENT2_SGP40_TEST_PASSED                           0xD400
#define MIKROE_ENVIRONMENT2_SGP40_TEST_FAILED                           0x4B00

/*! @} */ // environment2_get

/**
 * @defgroup environment2_map Environment 2 MikroBUS Map
 * @brief MikroBUS pin mapping of Environment 2 Click driver.
 */

/**
 * @addtogroup environment2_map
 * @{
 */

/**
 * @brief MikroBUS pin mapping.
 * @details Mapping pins of Environment 2 Click to the selected MikroBUS.
 */
#define MIKROE_ENVIRONMENT2_MAP_MIKROBUS(cfg, mikrobus) \
  cfg.scl = MIKROBUS(mikrobus, MIKROBUS_SCL);           \
  cfg.sda = MIKROBUS(mikrobus, MIKROBUS_SDA)

/*! @} */ // environment2_map

/*! @} */ // environment2

typedef int32_t fix16_t;

/**
 * @brief Environment 2 Click VOC algorithm object.
 * @details Struct to hold all the states of the VOC algorithm.
 */
typedef  environment2_voc_algorithm_params
  mikroe_environment2_voc_algorithm_params;

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
sl_status_t mikroe_environment2_init(mikroe_i2c_handle_t i2cspm_instance);

/***************************************************************************//**
 * @brief
 *    Environment 2 I2C writing function.
 *
 * @param[in] select_device
 *    Device selection.
 *
 * @param[in] cmd
 *    Command to be written.
 *
 * @param[in] tx_buf
 *    Data to be written.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_FAIL if write failed.
 ******************************************************************************/
sl_status_t mikroe_environment2_generic_write (
  enum MIKROE_ENVIRONMENT2_DEVICE select_device,
  uint16_t cmd);

/***************************************************************************//**
 * @brief
 *    Environment 2 I2C reading function.
 *
 * @param[in] select_device
 *    Device selection.
 *
 * @param[in] cmd
 *    Command to be written.
 *
 * @param[out] rx_buf
 *    Output read data.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if rx_buf is null.
 ******************************************************************************/
sl_status_t  mikroe_environment2_generic_read (
  enum MIKROE_ENVIRONMENT2_DEVICE select_device,
  uint16_t cmd,
  uint8_t *rx_buf);

/***************************************************************************//**
 * @brief
 *    Environment 2 get temperature and relative humidity function.
 *
 * @param[out] humidity
 *    Relative Humidity [ %RH ]
 *
 * @param[out] temperature
 *    Temperature [ degree Celsius ].
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if humidity or temperature is null.
 ******************************************************************************/
sl_status_t mikroe_environment2_get_temp_hum (float *humidity,
                                              float *temperature);

/***************************************************************************//**
 * @brief
 *    Environment 2 get air quality data function.
 *
 * @param[out] air_quality
 *    Air quality SRAW data.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_INVALID_PARAMETER if air_quality is null.
 ******************************************************************************/
sl_status_t mikroe_environment2_get_air_quality (uint16_t *air_quality);

/***************************************************************************//**
 * @brief
 *    Environment 2 SGP40 measurement test function.
 *
 * @return
 *    @li @c 0xD400 ( ENVIRONMENT2_SGP40_TEST_PASSED ) - All tests passed
 *   successfully,
 *    @li @c 0x4B00 ( ENVIRONMENT2_SGP40_TEST_FAILED ) - One or more tests have
 *   failed.
 ******************************************************************************/
sl_status_t mikroe_environment2_sgp40_measure_test (void);

/***************************************************************************//**
 * @brief
 *    Environment 2 heater off function.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_ERROR if error.
 ******************************************************************************/
sl_status_t mikroe_environment2_sgp40_heater_off (void);

/***************************************************************************//**
 * @brief
 *    Environment 2 soft reset function.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_ERROR if error.
 ******************************************************************************/
sl_status_t mikroe_environment2_sgp40_soft_reset (void);

/***************************************************************************//**
 * @brief
 *    Environment 2 VOC algorithm configuration function.
 *
 * @param[in] params
 *    Struct to hold all the states of the VOC algorithm.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_ERROR if error.
 ******************************************************************************/
sl_status_t mikroe_environment2_voc_algorithm_configuration (
  mikroe_environment2_voc_algorithm_params *params);

/***************************************************************************//**
 * @brief
 *    Environment 2 sensors configuration  function.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_ERROR if error.
 ******************************************************************************/
sl_status_t mikroe_environment2_config_sensors (void);

/***************************************************************************//**
 * @brief
 *    Environment 2 VOC algorithm process function.
 *
 * @param[in] ctx
 *    Click context object.
 *
 * @param[in] params
 *    Pointer to the environment2_voc_algorithm_params struct.
 *
 * @param[in] sraw
 *    Air quality SRAW data.
 *
 * @param[out] voc_index
 *    Calculated VOC index value from the raw sensor value.
 *
 * @param[in] params
 *    Struct to hold all the states of the VOC algorithm.
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_ERROR if error.
 ******************************************************************************/
sl_status_t mikroe_environment2_voc_algorithm_process (int32_t sraw,
                                                       int32_t *voc_index,
                                                       mikroe_environment2_voc_algorithm_params *params);

/***************************************************************************//**
 * @brief
 *    Environment 2 measure VOC index with relative humidity and temperature
 *   function.
 *
 * @param[in] voc_index
 *    Pointer to buffer for measured VOC index. Range 0..500.
 *
 * @param[in] relative_humidity
 *    Relative Humidity [ milli %RH ].
 *
 * @param[out] temperature
 *    Temperature [ milli degree Celsius ].
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_ERROR if error.
 ******************************************************************************/
sl_status_t mikroe_environment2_measure_voc_index_with_rh_t (int32_t *voc_index,
                                                             int32_t *relative_humidity,
                                                             int32_t *temperature);

/***************************************************************************//**
 * @brief
 *    Environment 2 get VOC index function.
 *
 * @param[out] voc_index
 *    Pointer to buffer for measured VOC index. Range 0..500.
 *
 * @return
 *    SL_STATUS_OK if there are no errors.
 *    SL_STATUS_ERROR if error.
 ******************************************************************************/
sl_status_t mikroe_environment2_get_voc_index (int32_t *voc_index);

#ifdef __cplusplus
}
#endif
#endif // ENVIRONMENT2_H

/*! @} */ // environment2

// ------------------------------------------------------------------------ END
