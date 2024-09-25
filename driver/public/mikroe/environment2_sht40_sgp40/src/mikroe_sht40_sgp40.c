/***************************************************************************//**
 * @file mikroe_sht4_sgp40.c
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

#include "mikroe_sht40_sgp40.h"
#include "mikroe_sht40_sgp40_config.h"

static environment2_t environment2;
// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

sl_status_t mikroe_environment2_init(mikroe_i2c_handle_t i2cspm_instance)
{
  environment2_cfg_t environment2_cfg;

  if (i2cspm_instance == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  environment2.i2c.handle = i2cspm_instance;

  // Call basic setup functions
  environment2_cfg_setup(&environment2_cfg);

#if (MIKROE_I2C_SHT40_SGP40_UC == 1)
  environment2_cfg.i2c_speed = MIKROE_I2C_SHT40_SGP40_SPEED_MODE;
#endif

  if (I2C_MASTER_SUCCESS
      != environment2_init(&environment2, &environment2_cfg)) {
    return SL_STATUS_INITIALIZATION;
  }

  return (ENVIRONMENT2_OK == environment2_config_sensors())
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_generic_write(
  enum MIKROE_ENVIRONMENT2_DEVICE select_device,
  uint16_t cmd)
{
  uint8_t device;

  switch (select_device) {
    case MIKROE_ENVIRONMENT2_SEL_SHT40:
      device = ENVIRONMENT2_SEL_SHT40;
      break;
    case MIKROE_ENVIRONMENT2_SEL_SGP40:
      device = ENVIRONMENT2_SEL_SGP40;
      break;
    default:
      return SL_STATUS_INVALID_PARAMETER;
  }

  return (ENVIRONMENT2_OK == environment2_generic_write(&environment2,
                                                        device,
                                                        cmd))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_generic_read(
  enum MIKROE_ENVIRONMENT2_DEVICE select_device,
  uint16_t cmd,
  uint8_t *tx_buf)
{
  uint8_t device;

  if (NULL == tx_buf) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  switch (select_device) {
    case MIKROE_ENVIRONMENT2_SEL_SHT40:
      device = ENVIRONMENT2_SEL_SHT40;
      break;
    case MIKROE_ENVIRONMENT2_SEL_SGP40:
      device = ENVIRONMENT2_SEL_SGP40;
      break;
    default:
      return SL_STATUS_INVALID_PARAMETER;
  }

  return (ENVIRONMENT2_OK == environment2_generic_read(&environment2,
                                                       device,
                                                       cmd,
                                                       tx_buf))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_get_temp_hum(float *humidity,
                                             float *temperature)
{
  if ((NULL == humidity) || (temperature == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return (ENVIRONMENT2_OK == environment2_get_temp_hum(&environment2,
                                                       humidity,
                                                       temperature))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_get_air_quality(uint16_t *air_quality)
{
  if (NULL == air_quality) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return (ENVIRONMENT2_OK == environment2_get_air_quality(&environment2,
                                                          air_quality))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_sgp40_measure_test(void)
{
  return (ENVIRONMENT2_SGP40_TEST_PASSED
          == environment2_sgp40_measure_test(&environment2))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_sgp40_heater_off(void)
{
  return (ENVIRONMENT2_OK
          == environment2_sgp40_heater_off(&environment2))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_sgp40_soft_reset(void)
{
  return (ENVIRONMENT2_OK
          == environment2_sgp40_soft_reset(&environment2))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_voc_algorithm_configuration(
  mikroe_environment2_voc_algorithm_params *params)
{
  if (params == NULL) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return (ENVIRONMENT2_OK
          == environment2_voc_algorithm_configuration(params))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_config_sensors(void)
{
  environment2_config_sensors();

  return ENVIRONMENT2_OK;
}

sl_status_t mikroe_environment2_voc_algorithm_process(int32_t sraw,
                                                      int32_t *voc_index,
                                                      mikroe_environment2_voc_algorithm_params *params)
{
  if ((voc_index == NULL) || (params == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return (ENVIRONMENT2_OK == environment2_voc_algorithm_process(params,
                                                                sraw,
                                                                voc_index))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_measure_voc_index_with_rh_t(int32_t *voc_index,
                                                            int32_t *relative_humidity,
                                                            int32_t *temperature)
{
  if ((NULL == voc_index) || (NULL == relative_humidity)
      || (NULL == temperature)) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return (ENVIRONMENT2_OK
          == environment2_measure_voc_index_with_rh_t(&environment2,
                                                      voc_index,
                                                      relative_humidity,
                                                      temperature))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_environment2_get_voc_index(int32_t *voc_index)
{
  if (NULL == voc_index) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return (ENVIRONMENT2_OK
          == environment2_get_voc_index(&environment2,
                                        voc_index))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}
