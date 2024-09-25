/***************************************************************************//**
 * @file mikroe_mm5d91_00.c
 * @brief SCL MM5D91_00 Source File
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

#include "mikroe_mm5d91_00.h"
#include "mikroe_mm5d91_00_config.h"

static mikroe_radar_t mikroe_radar;
static radar_cfg_t cfg;
static bool initialized = false;

sl_status_t mikroe_radar_init(mikroe_uart_handle_t handle)
{
  if (handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  mikroe_radar.uart.handle = handle;

  radar_cfg_setup(&cfg);
#if defined(CONFIG_MM5D91_00_RST_PORT) && defined(CONFIG_MM5D91_00_RST_PIN)
  cfg.rst = hal_gpio_pin_name(CONFIG_MM5D91_00_RST_PORT,
                              CONFIG_MM5D91_00_RST_PIN);
#endif
#if defined(CONFIG_MM5D91_00_GPIO0_PORT) && defined(CONFIG_MM5D91_00_GPIO0_PIN)
  cfg.gp0 = hal_gpio_pin_name(CONFIG_MM5D91_00_GPIO0_PORT,
                              CONFIG_MM5D91_00_GPIO0_PIN);
#endif
#if defined(CONFIG_MM5D91_00_GPIO1_PORT) && defined(CONFIG_MM5D91_00_GPIO1_PIN)
  cfg.gp1 = hal_gpio_pin_name(CONFIG_MM5D91_00_GPIO1_PORT,
                              CONFIG_MM5D91_00_GPIO1_PIN);
#endif
#if defined(CONFIG_MM5D91_00_GPIO2_PORT) && defined(CONFIG_MM5D91_00_GPIO2_PIN)
  cfg.gp2 = hal_gpio_pin_name(CONFIG_MM5D91_00_GPIO2_PORT,
                              CONFIG_MM5D91_00_GPIO2_PIN);
#endif

  if (radar_init(&mikroe_radar, &cfg) != RADAR_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  if (radar_default_cfg(&mikroe_radar) != RADAR_OK) {
    return SL_STATUS_INITIALIZATION;
  }

  initialized = true;
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_set_uart_instance(mikroe_uart_handle_t handle)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  if (NULL == handle) {
    return SL_STATUS_NULL_POINTER;
  }

  mikroe_radar.uart.handle = handle;

  return SL_STATUS_OK;
}

int32_t mikroe_radar_generic_write(uint8_t *data_buf,
                                   uint16_t len)
{
  if ((data_buf == NULL) || (len == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (int32_t)radar_generic_write(&mikroe_radar, data_buf, len);
}

int32_t mikroe_radar_generic_read(uint8_t *data_buf,
                                  uint16_t max_len)
{
  if ((data_buf == NULL) || (max_len == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return (int32_t)radar_generic_read(&mikroe_radar, data_buf, max_len);
}

void mikroe_radar_enable_device(void)
{
  radar_enable_device(&mikroe_radar);
}

void mikroe_radar_disable_device(void)
{
  radar_disable_device(&mikroe_radar);
}

sl_status_t mikroe_radar_get_gpio2_pin(uint8_t *gpio_val)
{
  if (gpio_val == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  *gpio_val = radar_get_gpio2_pin(&mikroe_radar);
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_get_gpio1_pin(uint8_t *gpio_val)
{
  if (gpio_val == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  *gpio_val = radar_get_gpio1_pin(&mikroe_radar);
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_get_gpio0_pin(uint8_t *gpio_val)
{
  if (gpio_val == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  *gpio_val = radar_get_gpio0_pin(&mikroe_radar);
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_set_command(uint8_t cmd_id,
                                     uint8_t *payload,
                                     uint8_t payload_size)
{
  return (radar_set_command(&mikroe_radar, cmd_id, payload, payload_size)
          == RADAR_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_get_command(uint8_t cmd_id,
                                     uint8_t *payload,
                                     uint8_t *payload_size)
{
  return (radar_get_command(&mikroe_radar, cmd_id, payload, payload_size)
          == RADAR_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_get_event(uint8_t *evt_id,
                                   uint8_t *payload,
                                   uint8_t *payload_size)
{
  return (radar_get_event(&mikroe_radar, evt_id, payload, payload_size)
          == RADAR_OK) ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_get_temperature(float *temperature)
{
  return (RADAR_OK == radar_get_temperature(&mikroe_radar, temperature))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_set_detection_range(float min,
                                             float max)
{
  return (RADAR_OK == radar_set_detection_range(&mikroe_radar, min, max))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_reset_config(void)
{
  return (RADAR_OK == radar_reset_config(&mikroe_radar))
         ? SL_STATUS_OK : SL_STATUS_FAIL;
}

sl_status_t mikroe_radar_float_ieee_to_mchip(float *f_data)
{
  if (!f_data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  radar_float_ieee_to_mchip(f_data);
  return SL_STATUS_OK;
}

sl_status_t mikroe_radar_float_mchip_to_ieee(float *f_data)
{
  if (!f_data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  radar_float_mchip_to_ieee(f_data);
  return SL_STATUS_OK;
}
