/***************************************************************************//**
 * @file sparkfun_vcnl4040.c
 * @brief SPARKFUN VCNL4040 Proximity sensor
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdbool.h>
#include "sl_status.h"
#include "sparkfun_vcnl4040_config.h"
#include "sparkfun_vcnl4040.h"

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

typedef struct
{
  i2c_master_t i2c;
  uint8_t slave_address;
} sparkfun_proximity_t;

static sparkfun_vcnl4040_core_version_t core_version = {
  .major = 0,
  .minor = 1,
  .build = 0,
  .revision = 100,
}; // Structure to hold the software version of the core driver

static SPARKFUN_VCNL4040_Sensor_Config_TypeDef vcnl4040_cfg = {
  .PSDuty = 0x0,
  .IRLEDCurrent = 0x0,
  .PSPersistence = 0x0,
  .PSIntegrationTime = 0x0,
  .PSResolution = 0x0,
  .PSInterruptType = 0x0,
  .PSEnabled = false,
  .PSSmartPersEnabled = false,
  .PSActiveForceEnabled = false,
  .PSLogicEnabled = false,
  .PSCancelThresh = 0x0,
  .PSHighThreshold = 0x0,
  .PSLowThreshold = 0x0,

  .ALSPersistence = 0x0,
  .ALSIntegrationTime = 0x0,
  .ALSHighThreshold = 0x0,
  .ALSLowThreshold = 0x0,
  .ALSEnabled = false,
  .ALSIntEnabled = false,

  .WhiteEnabled = false
}; // Structure to hold VCNL4040 driver config

static sparkfun_proximity_t sparkfun_proximity;
static sparkfun_vcnl4040_norm_interrupt_callback_t
  vcnl4040_interrupt_callback = NULL;
// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 *  Initialize the VCNL4040 sensor.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_init(mikroe_i2c_handle_t i2cspm_instance)
{
  i2c_master_config_t i2c_cfg;
  sl_status_t sc = SL_STATUS_OK;
  uint16_t id;

  if (i2cspm_instance == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  sparkfun_proximity.i2c.handle = i2cspm_instance;

  i2c_master_configure_default(&i2c_cfg);
  i2c_cfg.addr = SPARKFUN_VCNL4040_I2C_BUS_ADDRESS;

  sparkfun_proximity.slave_address = i2c_cfg.addr;

#if (SPARKFUN_VCNL4040_I2C_UC == 1)
  i2c_cfg.speed = SPARKFUN_VCNL4040_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&sparkfun_proximity.i2c, &i2c_cfg) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  i2c_master_set_slave_address(&sparkfun_proximity.i2c,
                               sparkfun_proximity.slave_address);
  i2c_master_set_speed(&sparkfun_proximity.i2c, i2c_cfg.speed);
  i2c_master_set_timeout(&sparkfun_proximity.i2c, 0);

  if (sparkfun_vcnl4040_get_id(&id) != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  if (id != SPARKFUN_VCNL4040_DEVICE_ID) {
    return SL_STATUS_FAIL;
  }

  sc |= sparkfun_vcnl4040_set_ir_led_sink_current(0x7);
  sc |= sparkfun_vcnl4040_set_ir_duty_cycle(0x0);
  sc |= sparkfun_vcnl4040_set_proximity_integration_time(0x7);
  sc |= sparkfun_vcnl4040_set_proximity_resolution(0x1);
  sc |= sparkfun_vcnl4040_enable_smart_persistence(true);
  sc |= sparkfun_vcnl4040_power_on_proximity(true);
  sc |= sparkfun_vcnl4040_set_ambient_integration_time(0x0);
  sc |= sparkfun_vcnl4040_power_on_ambient(true);
  sc |= sparkfun_vcnl4040_enable_white_channel(true);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  } else {
    return SL_STATUS_OK;
  }
}

/**************************************************************************//**
 *  De-initalize the VCNL4040 sensor.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_deinit(void)
{
  sparkfun_proximity.i2c.handle = NULL;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Reads the device ID.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_id(uint16_t *id)
{
  if (id == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  return sparkfun_vcnl4040_i2c_read_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_ID,
    id);
}

/**************************************************************************//**
 *  Set the duty cycle of the IR LED.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_set_ir_duty_cycle(uint8_t duty_value)
{
  uint8_t send_data = 0;

  switch (duty_value) {
    case 0x0:
      send_data = SPARKFUN_VCNL4040_PS_DUTY_40;
      break;
    case 0x1:
      send_data = SPARKFUN_VCNL4040_PS_DUTY_80;
      break;
    case 0x2:
      send_data = SPARKFUN_VCNL4040_PS_DUTY_160;
      break;
    case 0x3:
      send_data = SPARKFUN_VCNL4040_PS_DUTY_320;
      break;
    default:
      return SL_STATUS_INVALID_PARAMETER;
  }
  vcnl4040_cfg.PSDuty = duty_value;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_CONF1,
    LOWER,
    (uint8_t)SPARKFUN_VCNL4040_PS_DUTY_MASK,
    send_data);
}

/**************************************************************************//**
 *  Get the duty cycle of the IR LED.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_ir_duty_cycle(uint8_t *duty_value)
{
  if (duty_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *duty_value = vcnl4040_cfg.PSDuty;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Sets the IR LED sink current.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_set_ir_led_sink_current(uint8_t current_value)
{
  uint8_t send_data = 0;

  switch (current_value) {
    case 0x0:
      send_data = SPARKFUN_VCNL4040_LED_50MA;
      break;
    case 0x1:
      send_data = SPARKFUN_VCNL4040_LED_75MA;
      break;
    case 0x2:
      send_data = SPARKFUN_VCNL4040_LED_100MA;
      break;
    case 0x3:
      send_data = SPARKFUN_VCNL4040_LED_120MA;
      break;
    case 0x4:
      send_data = SPARKFUN_VCNL4040_LED_140MA;
      break;
    case 0x5:
      send_data = SPARKFUN_VCNL4040_LED_160MA;
      break;
    case 0x6:
      send_data = SPARKFUN_VCNL4040_LED_180MA;
      break;
    case 0x7:
      send_data = SPARKFUN_VCNL4040_LED_200MA;
      break;
    default:
      return SL_STATUS_INVALID_PARAMETER;
  }
  vcnl4040_cfg.IRLEDCurrent = current_value;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_MS,
    UPPER,
    SPARKFUN_VCNL4040_LED_I_MASK,
    send_data);
}

/***************************************************************************//**
 *  Gets the IR LED sink current.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_get_ir_led_sink_current(uint8_t *current_value)
{
  if (current_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *current_value = vcnl4040_cfg.IRLEDCurrent;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Sets proximity interrupt persistence.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_set_proximity_interrupt_persistence(
  uint8_t pers_value)
{
  uint8_t send_data = 0;

  switch (pers_value) {
    case 0x0:
      send_data = SPARKFUN_VCNL4040_PS_PERS_1;
      break;
    case 0x1:
      send_data = SPARKFUN_VCNL4040_PS_PERS_2;
      break;
    case 0x2:
      send_data = SPARKFUN_VCNL4040_PS_PERS_3;
      break;
    case 0x3:
      send_data = SPARKFUN_VCNL4040_PS_PERS_4;
      break;
    default:
      return SL_STATUS_INVALID_PARAMETER;
  }
  vcnl4040_cfg.PSPersistence = pers_value;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_CONF1,
    LOWER,
    SPARKFUN_VCNL4040_PS_PERS_MASK,
    send_data);
}

/**************************************************************************//**
 *  Gets proximity interrupt persistence.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_proximity_interrupt_persistence(
  uint8_t *pers_value)
{
  if (pers_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *pers_value = vcnl4040_cfg.PSPersistence;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Sets the integration time for the proximity sensor.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_set_proximity_integration_time(uint8_t time_value)
{
  uint8_t send_data = 0;

  switch (time_value) {
    case 0x0:
      send_data = SPARKFUN_VCNL4040_PS_IT_1T;
      break;
    case 0x1:
      send_data = SPARKFUN_VCNL4040_PS_IT_15T;
      break;
    case 0x2:
      send_data = SPARKFUN_VCNL4040_PS_IT_2T;
      break;
    case 0x3:
      send_data = SPARKFUN_VCNL4040_PS_IT_25T;
      break;
    case 0x4:
      send_data = SPARKFUN_VCNL4040_PS_IT_3T;
      break;
    case 0x5:
      send_data = SPARKFUN_VCNL4040_PS_IT_35T;
      break;
    case 0x6:
      send_data = SPARKFUN_VCNL4040_PS_IT_4T;
      break;
    case 0x7:
      send_data = SPARKFUN_VCNL4040_PS_IT_8T;
      break;
    default:
      return SL_STATUS_INVALID_PARAMETER;
  }
  vcnl4040_cfg.PSIntegrationTime = time_value;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_ALS_CONF,
    LOWER,
    SPARKFUN_VCNL4040_PS_IT_MASK,
    send_data);
}

/**************************************************************************//**
 *  Gets the integration time of the proximity sensor.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_proximity_integration_time(
  uint8_t *time_value)
{
  if (time_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *time_value = vcnl4040_cfg.PSIntegrationTime;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Powers on proximity detection.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_power_on_proximity(bool enable)
{
  uint8_t send_data = 0;

  if (enable == true) {
    send_data = SPARKFUN_VCNL4040_PS_SD_POWER_ON;
  } else {
    send_data = SPARKFUN_VCNL4040_PS_SD_POWER_OFF;
  }
  vcnl4040_cfg.PSEnabled = enable;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_CONF1,
    LOWER,
    SPARKFUN_VCNL4040_PS_SD_MASK,
    send_data);
}

/**************************************************************************//**
 *  Gets proximity detection powered status.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_proximity_powered(bool *enabled)
{
  if (enabled == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *enabled = vcnl4040_cfg.PSEnabled;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Sets the proximity resolution.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_set_proximity_resolution(uint8_t resolution_value)
{
  uint8_t send_data = 0;

  if (resolution_value == 0x0) {
    send_data = SPARKFUN_VCNL4040_PS_HD_12_BIT;
  } else if (resolution_value == 0x1) {
    send_data = SPARKFUN_VCNL4040_PS_HD_16_BIT;
  }
  vcnl4040_cfg.PSResolution = resolution_value;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_CONF2,
    UPPER,
    SPARKFUN_VCNL4040_PS_HD_MASK,
    send_data);
}

/**************************************************************************//**
 *  Gets the proximity resolution.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_proximity_resolution(
  uint8_t *resolution_value)
{
  if (resolution_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *resolution_value = vcnl4040_cfg.PSResolution;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Sets the proximity interrupt type.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_set_proximity_int_type(uint8_t interrupt_value)
{
  uint8_t send_data = 0;

  switch (interrupt_value) {
    case 0x0:
      send_data = SPARKFUN_VCNL4040_PS_INT_DISABLE;
      break;
    case 0x1:
      send_data = SPARKFUN_VCNL4040_PS_INT_CLOSE;
      break;
    case 0x2:
      send_data = SPARKFUN_VCNL4040_PS_INT_AWAY;
      break;
    case 0x3:
      send_data = SPARKFUN_VCNL4040_PS_INT_BOTH;
      break;
    default:
      return SL_STATUS_INVALID_PARAMETER;
  }
  vcnl4040_cfg.PSInterruptType = interrupt_value;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_CONF2,
    UPPER,
    SPARKFUN_VCNL4040_PS_INT_MASK,
    send_data);
}

/**************************************************************************//**
 *  Gets the proximity interrupt type.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_proximity_int_type(uint8_t *interrupt_value)
{
  if (interrupt_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *interrupt_value = vcnl4040_cfg.PSInterruptType;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Enables smart persistence.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_enable_smart_persistence(bool enable)
{
  uint8_t send_data = 0;

  if (enable) {
    send_data = SPARKFUN_VCNL4040_PS_SMART_PERS_ENABLE;
  } else {
    send_data = SPARKFUN_VCNL4040_PS_SMART_PERS_DISABLE;
  }
  vcnl4040_cfg.PSSmartPersEnabled = enable;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_CONF3,
    LOWER,
    SPARKFUN_VCNL4040_PS_SMART_PERS_MASK,
    send_data);
}

/***************************************************************************//**
 *  Gets the smart persistence enabled status.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_get_smart_persistence_enabled(bool *enabled)
{
  if (enabled == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *enabled = vcnl4040_cfg.PSSmartPersEnabled;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Enables active force mode.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_enable_active_force_mode(bool enable)
{
  uint8_t send_data = 0;
  if (enable) {
    send_data = SPARKFUN_VCNL4040_PS_AF_ENABLE;
  } else {
    send_data = SPARKFUN_VCNL4040_PS_AF_DISABLE;
  }
  vcnl4040_cfg.PSActiveForceEnabled = enable;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_CONF3,
    LOWER,
    SPARKFUN_VCNL4040_PS_AF_MASK,
    send_data);
}

/***************************************************************************//**
 *  Gets active force mode enabled status.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_get_active_force_mode_enabled(bool *enabled)
{
  if (enabled == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *enabled = vcnl4040_cfg.PSActiveForceEnabled;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Requests a single PS measurement (active force mode).
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_trigger_proximity_measurement(void)
{
  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_CONF3,
    LOWER,
    SPARKFUN_VCNL4040_PS_TRIG_MASK,
    SPARKFUN_VCNL4040_PS_TRIG_TRIGGER);
}

/***************************************************************************//**
 *  Enables the proximity detection logic output mode.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_enable_proximity_logic_mode(bool enable)
{
  uint8_t send_data = 0;
  if (enable) {
    send_data = SPARKFUN_VCNL4040_PS_MS_ENABLE;
  } else {
    send_data = SPARKFUN_VCNL4040_PS_MS_DISABLE;
  }
  vcnl4040_cfg.PSLogicEnabled = enable;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_MS,
    UPPER,
    SPARKFUN_VCNL4040_PS_MS_MASK,
    send_data);
}

/***************************************************************************//**
 *  Gets the proximity detection logic output mode status.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_get_proximity_logic_mode_enabled(bool *enabled)
{
  if (enabled == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *enabled = vcnl4040_cfg.PSLogicEnabled;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Sets the proximity sensing cancelation value.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_set_proximity_cancelation(uint16_t cancel_value)
{
  vcnl4040_cfg.PSCancelThresh = cancel_value;

  return sparkfun_vcnl4040_i2c_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_CANC,
    cancel_value);
}

/***************************************************************************//**
 *  Gets the proximity sensing cancelation value.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_get_proximity_cancelation(uint16_t *cancel_value)
{
  if (cancel_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *cancel_value = vcnl4040_cfg.PSCancelThresh;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Sets the proximity sensing low threshold.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_set_proximity_low_threshold(
  uint16_t threshold_value)
{
  vcnl4040_cfg.PSLowThreshold = threshold_value;

  return sparkfun_vcnl4040_i2c_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_THDL,
    threshold_value);
}

/***************************************************************************//**
 *  Gets the proximity sensing low threshold.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_get_proximity_low_threshold(
  uint16_t *threshold_value)
{
  if (threshold_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *threshold_value = vcnl4040_cfg.PSLowThreshold;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Sets the proximity sensing high threshold.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_set_proximity_high_threshold(
  uint16_t threshold_value)
{
  vcnl4040_cfg.PSHighThreshold = threshold_value;

  return sparkfun_vcnl4040_i2c_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_THDH,
    threshold_value);
}

/***************************************************************************//**
 *  Gets the proximity sensing high threshold.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_get_proximity_high_threshold(
  uint16_t *threshold_value)
{
  if (threshold_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *threshold_value = vcnl4040_cfg.PSHighThreshold;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Reads the proximity sensor data value..
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_proximity(uint16_t *proximity_data)
{
  if (proximity_data == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  return sparkfun_vcnl4040_i2c_read_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_DATA,
    proximity_data);
}

/***************************************************************************//**
 *  Reads the interrupt flag and checks PS_IF_CLOSE interrupt status.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_is_close(bool *is_close)
{
  uint8_t data;
  sl_status_t ret;

  if (is_close == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  ret = sparkfun_vcnl4040_i2c_read_command_upper(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_INT_FLAG,
    &data);
  *is_close = (bool)(data & SPARKFUN_VCNL4040_INT_FLAG_CLOSE);

  return ret;
}

/***************************************************************************//**
 *  Reads the interrupt flag and checks PS_IF_AWAY interrupt status.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_is_away(bool *is_away)
{
  uint8_t data;
  sl_status_t ret;

  if (is_away == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  ret = sparkfun_vcnl4040_i2c_read_command_upper(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_INT_FLAG,
    &data);
  *is_away = (bool)(data & SPARKFUN_VCNL4040_INT_FLAG_AWAY);

  return ret;
}

/**************************************************************************//**
 *  Sets the ambient interrupt persistence value.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_set_ambient_interrupt_persistence(
  uint8_t pers_value)
{
  uint8_t send_data = 0;

  switch (pers_value) {
    case 0x0:
      send_data = SPARKFUN_VCNL4040_ALS_PERS_1;
      break;
    case 0x1:
      send_data = SPARKFUN_VCNL4040_ALS_PERS_2;
      break;
    case 0x2:
      send_data = SPARKFUN_VCNL4040_ALS_PERS_4;
      break;
    case 0x3:
      send_data = SPARKFUN_VCNL4040_ALS_PERS_8;
      break;
    default:
      return SL_STATUS_INVALID_PARAMETER;
  }
  vcnl4040_cfg.ALSPersistence = pers_value;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_ALS_CONF,
    LOWER,
    SPARKFUN_VCNL4040_ALS_PERS_MASK,
    send_data);
}

/**************************************************************************//**
 *  Gets the ambient interrupt persistence value.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_ambient_interrupt_persistance(
  uint8_t *pers_value)
{
  if (pers_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *pers_value = vcnl4040_cfg.ALSPersistence;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Enables ambient light detection interrupts.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_enable_ambient_interrupts(bool enable)
{
  uint8_t send_data;

  if (enable) {
    send_data = SPARKFUN_VCNL4040_ALS_INT_ENABLE;
  } else {
    send_data = SPARKFUN_VCNL4040_ALS_INT_DISABLE;
  }
  vcnl4040_cfg.ALSIntEnabled = enable;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_ALS_CONF,
    LOWER,
    SPARKFUN_VCNL4040_ALS_INT_EN_MASK,
    send_data);
}

/**************************************************************************//**
 *  Gets the ambient light detection interrupt enabled status.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_ambient_interrupt_enabled(bool *enable)
{
  if (enable == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *enable = vcnl4040_cfg.ALSIntEnabled;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Powers on ambient light detection.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_power_on_ambient(bool enable)
{
  uint8_t send_data;

  if (enable) {
    send_data = SPARKFUN_VCNL4040_ALS_SD_POWER_ON;
  } else {
    send_data = SPARKFUN_VCNL4040_ALS_SD_POWER_OFF;
  }
  vcnl4040_cfg.ALSEnabled = enable;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_ALS_CONF,
    LOWER,
    SPARKFUN_VCNL4040_ALS_SD_MASK,
    send_data);
}

/**************************************************************************//**
 *  Gets the ambient light detection power status.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_ambient_powered(bool *enable)
{
  if (enable == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *enable = vcnl4040_cfg.ALSEnabled;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Sets the integration time for the ambient light sensor.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_set_ambient_integration_time(uint8_t time_value)
{
  uint8_t send_data = 0;

  switch (time_value) {
    case 0x0:
      send_data = SPARKFUN_VCNL4040_ALS_IT_80MS;
      break;
    case 0x1:
      send_data = SPARKFUN_VCNL4040_ALS_IT_160MS;
      break;
    case 0x2:
      send_data = SPARKFUN_VCNL4040_ALS_IT_320MS;
      break;
    case 0x3:
      send_data = SPARKFUN_VCNL4040_ALS_IT_640MS;
      break;
    default:
      return SL_STATUS_INVALID_PARAMETER;
  }
  vcnl4040_cfg.ALSIntegrationTime = time_value;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_ALS_CONF,
    LOWER,
    (uint8_t)SPARKFUN_VCNL4040_ALS_IT_MASK,
    send_data);
}

/**************************************************************************//**
 *  Gets the integration time for the ambient light sensor.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_ambient_integration_time(uint8_t *time_value)
{
  if (time_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *time_value = vcnl4040_cfg.ALSIntegrationTime;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Sets the ambient light sensing low threshold.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_set_ambient_low_threshold(
  uint16_t threshold_value)
{
  vcnl4040_cfg.ALSLowThreshold = threshold_value;

  return sparkfun_vcnl4040_i2c_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_ALS_THDL,
    threshold_value);
}

/**************************************************************************//**
 *  Gets the ambient light sensing low threshold.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_ambient_low_threshold(
  uint16_t *threshold_value)
{
  if (threshold_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *threshold_value = vcnl4040_cfg.ALSLowThreshold;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Sets the ambient light sensing high threshold.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_set_ambient_high_threshold(
  uint16_t threshold_value)
{
  vcnl4040_cfg.ALSHighThreshold = threshold_value;

  return sparkfun_vcnl4040_i2c_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_ALS_THDH,
    threshold_value);
}

/**************************************************************************//**
 *  Gets the ambient light sensing high threshold.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_ambient_high_threshold(
  uint16_t *threshold_value)
{
  if (threshold_value == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *threshold_value = vcnl4040_cfg.ALSHighThreshold;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Reads the ambient light sensor data value.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_ambient(uint16_t *ambient_data)
{
  if (ambient_data == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  return sparkfun_vcnl4040_i2c_read_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_ALS_DATA,
    ambient_data);
}

/**************************************************************************//**
 *  Enables the white measurement channel.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_enable_white_channel(bool enable)
{
  uint8_t send_data;

  if (enable) {
    send_data = SPARKFUN_VCNL4040_WHITE_ENABLE;
  } else {
    send_data = SPARKFUN_VCNL4040_WHITE_DISABLE;
  }
  vcnl4040_cfg.WhiteEnabled = enable;

  return sparkfun_vcnl4040_i2c_masked_write_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_PS_MS,
    UPPER,
    (uint8_t)SPARKFUN_VCNL4040_WHITE_EN_MASK,
    send_data);
}

/**************************************************************************//**
 *  Gets the white measurement channel enabled status.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_white_channel_enabled(bool *enable)
{
  if (enable == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  *enable = vcnl4040_cfg.ALSEnabled;

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Reads the white light data value.
 *****************************************************************************/
sl_status_t sparkfun_vcnl4040_get_white(uint16_t *white_data)
{
  if (white_data == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  return sparkfun_vcnl4040_i2c_read_command(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_WHITE_DATA,
    white_data);
}

/***************************************************************************//**
 *  Reads the interrupt flag and checks PS_IF_H interrupt status.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_is_light(bool *is_light)
{
  uint8_t data;
  sl_status_t ret;

  if (is_light == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  ret = sparkfun_vcnl4040_i2c_read_command_upper(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_INT_FLAG,
    &data);
  *is_light = data & SPARKFUN_VCNL4040_INT_FLAG_ALS_HIGH;

  return ret;
}

/***************************************************************************//**
 *  Reads the interrupt flag and checks PS_IF_L interrupt status.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_is_dark(bool *is_dark)
{
  uint8_t data;
  sl_status_t ret;

  if (is_dark == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  ret = sparkfun_vcnl4040_i2c_read_command_upper(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_INT_FLAG,
    &data);
  *is_dark = data & SPARKFUN_VCNL4040_INT_FLAG_ALS_LOW;

  return ret;
}

/***************************************************************************//**
 *  Resigters the callback function for interrupt events.
 ******************************************************************************/
void sparkfun_vcnl4040_interrupt_callback_register(
  sparkfun_vcnl4040_norm_interrupt_callback_t callback)
{
  vcnl4040_interrupt_callback = callback;
}

/***************************************************************************//**
 *  This function indentifies which interrupt condition was met.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_is_interrupt(bool *is_light,
                                           bool *is_dark,
                                           bool *is_close,
                                           bool *is_away)
{
  sparkfun_vcnl4040_irq_source_t irq_source = INT_AWAY;
  *is_light = false;
  *is_dark = false;
  *is_close = false;
  *is_away = false;

  sl_status_t ret;
  uint8_t data;

  ret = sparkfun_vcnl4040_i2c_read_command_upper(
    &sparkfun_proximity.i2c,
    SPARKFUN_VCNL4040_INT_FLAG,
    &data);
  if (data & SPARKFUN_VCNL4040_INT_AWAY_MASK) {
    *is_away = true;
    irq_source = INT_AWAY;
  }
  if (data & SPARKFUN_VCNL4040_INT_CLOSE_MASK) {
    *is_close = true;
    irq_source = INT_CLOSE;
  }
  if (data & SPARKFUN_VCNL4040_INT_LIGHT_MASK) {
    *is_light = true;
    irq_source = INT_LIGHT;
  }
  if (data & SPARKFUN_VCNL4040_INT_DARK_MASK) {
    *is_dark = true;
    irq_source = INT_DARK;
  }

  if (vcnl4040_interrupt_callback != NULL) {
    vcnl4040_interrupt_callback(irq_source);
  }
  return ret;
}

/***************************************************************************//**
 *  Gets the version code of the sensor.
 ******************************************************************************/
sl_status_t sparkfun_vcnl4040_get_core_version(
  sparkfun_vcnl4040_core_version_t *version)
{
  if (version == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  version->minor = core_version.minor;
  version->major = core_version.major;
  version->build = core_version.build;
  version->revision = core_version.revision;

  return SL_STATUS_OK;
}
