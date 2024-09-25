/***************************************************************************//**
* @file max17048.c
* @brief Driver for the MAX17048/9 Fuel Gauge
********************************************************************************
* # License
* <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
********************************************************************************
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
*******************************************************************************/
#include "maxim_fuel_gauge_max17048.h"

typedef struct
{
  bool is_initialized;
  i2c_master_t i2c;

  // Tracking for the state of SLEEP and ALSC in addition to the ATHD field
  uint8_t  config_lower_tracking;
  uint8_t  rcomp_tracking;
  uint8_t  valrt_max_tracking;
  uint8_t  valrt_min_tracking;
  uint8_t  vreset_tracking;
  uint8_t  hibthr_tracking;
  uint8_t  actthr_tracking;
  uint32_t rcomp_update_interval;

  digital_in_t alrt_pin;

#if MAX17048_ENABLE_HW_QSTRT
#if defined(MAX17048_ENABLE_QSTRT_PORT) && defined(MAX17048_ENABLE_QSTRT_PIN)
  digital_out_t qstrt_pin;
#endif
  sl_sleeptimer_timer_handle_t quick_start_timer_handle;
#endif
  sl_sleeptimer_timer_handle_t temp_timer_handle;
  interrupt_callback_t         interrupt_callback[5];
  void *callback_data[5];
} fuel_gauge_t;

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*******************************************************************************
 *************************** GLOBAL VARIABLES   *******************************
 ******************************************************************************/

#if (defined(SLI_SI917))
extern adc_config_t sl_bjt_config;
extern adc_ch_config_t sl_bjt_channel_config;
#endif

bool update_rcom = false;
static fuel_gauge_t fuel_gauge = {
  .is_initialized = false,
  .config_lower_tracking = 0x1C,
  .rcomp_tracking = 0x97,
  .valrt_max_tracking = 0xFF,
  .valrt_min_tracking = 0x00,
  .vreset_tracking = 0x96,
  .hibthr_tracking = 0x80,
  .actthr_tracking = 0x30,
  .rcomp_update_interval = MAX17048_RCOMP_UPDATE_INTERVAL_MS,
  .interrupt_callback = { NULL, NULL, NULL, NULL, NULL },
};

// Function prototypes (private API)
static sl_status_t max17048_read_register(uint8_t reg_addr,
                                          uint8_t *data);
static sl_status_t max17048_write_register(uint8_t reg_addr,
                                           const uint8_t *data);
static void max17048_temp_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                                         void *data);

#if MAX17048_ENABLE_HW_QSTRT
static void max17048_quick_start_callback(sl_sleeptimer_timer_handle_t *handle,
                                          void *data);

#endif
static void max17048_alrt_pin_callback(uint8_t pin);
static sl_status_t max17048_set_rcomp(uint8_t rcomp);
static sl_status_t max17048_get_alert_condition(uint8_t *alert_condition);
static sl_status_t max17048_clear_alert_condition(uint8_t alert_condition,
                                                  uint8_t source);
static sl_status_t max17048_clear_alert_status_bit(void);
static sl_status_t max17048_clear_reset_indicator_bit(void);

/** @endcond */

/***************************************************************************//**
 * @brief
 *    Read a 16-bit word of data from the MAX17048.
 *
 * @param[in] reg_addr
 *    The first register to begin reading from
 *
 * @param[out] data
 *    The data to read
 *
 * @note
 *    All registers must be written and read as 16-bit words;
 *    8-bit writes cause no effect.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_TRANSMIT I2C transmit failure
 ******************************************************************************/
static sl_status_t max17048_read_register(uint8_t reg_addr,
                                          uint8_t *data)
{
  /*
   * Invoke sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1)
   * to prevent the EFM32/EFR32 from entering energy mode EM2
   * or lower during I2C bus activity.
   */
#if (MAX17048_ENABLE_POWER_MANAGER == 1)
#if (defined(SLI_SI917))
  sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS1);
#else
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
#endif
#endif
  if (I2C_MASTER_SUCCESS != i2c_master_write_then_read(&fuel_gauge.i2c,
                                                       &reg_addr,
                                                       1,
                                                       data,
                                                       2)) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;

  /*
   * Call sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1)
   * to remove the requirement to remain in EM1 or higher
   * after I2C bus activity is complete.
   */
#if (MAX17048_ENABLE_POWER_MANAGER == 1)
#if (defined(SLI_SI917))
  sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS1);
#else
  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
#endif
#endif
}

/***************************************************************************//**
 * @brief
 *    Write a 16-bit word of data to the MAX17048.
 *
 * @param[in] reg_addr
 *    The first register to begin writing to
 *
 * @param[in] length
 *    The number of bytes to write
 *
 * @param[in] data
 *    The data to write
 *
 * @note
 *    All registers must be written and read as 16-bit words;
 *    8-bit writes cause no effect.
 *
 * @retval SL_STATUS_OK Success
 * @retval SL_STATUS_TRANSMIT I2C transmit failure
 ******************************************************************************/
static sl_status_t max17048_write_register(uint8_t reg_addr,
                                           const uint8_t *data)
{
  uint8_t i2c_write_data[3];

  i2c_write_data[0] = reg_addr;
  i2c_write_data[1] = data[0];
  i2c_write_data[2] = data[1];

  /*
   * Invoke sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1)
   * to prevent the EFM32/EFR32 from entering energy mode EM2
   * or lower during I2C bus activity.
   */
#if (MAX17048_ENABLE_POWER_MANAGER == 1)
#if (defined(SLI_SI917))
  sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS1);
#else
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
#endif
#endif
  if (I2C_MASTER_SUCCESS != i2c_master_write(&fuel_gauge.i2c,
                                             i2c_write_data,
                                             sizeof(i2c_write_data))) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;

  /*
   * Call sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1)
   * to remove the requirement to remain in EM1 or higher
   * after I2C bus activity is complete.
   */
#if (MAX17048_ENABLE_POWER_MANAGER == 1)
#if (defined(SLI_SI917))
  sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS1);
#else
  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
#endif
#endif
}

/***************************************************************************//**
 * @brief
 *    This function sets the MAX17048 RCOMP temperature compensation factor.
 *
 * @param[in] rcomp
 *    Temperature compensation factor
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
static sl_status_t max17048_set_rcomp(uint8_t rcomp)
{
  sl_status_t status;
  uint8_t buffer[2];

  // Update the private global variable to track
  fuel_gauge.rcomp_tracking = rcomp;
  buffer[0] = fuel_gauge.rcomp_tracking;
  buffer[1] = fuel_gauge.config_lower_tracking;
  status = max17048_write_register(MAX17048_CONFIG, buffer);

  return status;
}

/***************************************************************************//**
 *  Sleeptimer callback function to get the temperature,
 *  calculate the new RCOMP value, and write it to the MAX17048.
 ******************************************************************************/
static void max17048_temp_timer_callback(sl_sleeptimer_timer_handle_t *handle,
                                         void *data)
{
  (void)handle;
  (void)data;

  update_rcom = true;
}

/***************************************************************************//**
 *  Sleeptimer callback function to de-assert QSTRT pin after 1 ms.
 ******************************************************************************/
#if MAX17048_ENABLE_HW_QSTRT
static void max17048_quick_start_callback(sl_sleeptimer_timer_handle_t *handle,
                                          void *data)
{
  (void)handle;
  (void)data;
#if defined(MAX17048_ENABLE_QSTRT_PORT) && defined(MAX17048_ENABLE_QSTRT_PIN)
  digital_out_low(&fuel_gauge.qstrt_pin);
#endif
}

#endif

/***************************************************************************//**
 * This is a callback function that is invoked each time a GPIO interrupt
 * in one of the pushbutton inputs occurs. Pin number is passed as parameter.
 *
 * @param[in] pin  Pin number where interrupt occurs
 *
 * @note This function is called from ISR context and therefore it is
 *       not possible to call any API functions directly.
 ******************************************************************************/
static void max17048_alrt_pin_callback(uint8_t pin)
{
  (void)pin;
  interrupt_callback_t callback;
  uint8_t alert_condition = 0;

  // Check alert condition
  max17048_get_alert_condition(&alert_condition);

  if ((alert_condition & MAX17048_STATUS_VR) != 0) {
    /*
     * Process RESET alert callback because battery
     * has changed or there has been POR
     */
    callback = fuel_gauge.interrupt_callback[IRQ_RESET];
    callback(IRQ_RESET, fuel_gauge.callback_data[IRQ_RESET]);
    max17048_clear_alert_condition(alert_condition, MAX17048_STATUS_VR);
  } else if ((alert_condition & MAX17048_STATUS_HD) != 0) {
    // Cell nearing empty; may need to place system in ultra-low-power state
    callback = fuel_gauge.interrupt_callback[IRQ_EMPTY];
    callback(IRQ_EMPTY, fuel_gauge.callback_data[IRQ_EMPTY]);
    max17048_clear_alert_condition(alert_condition, MAX17048_STATUS_HD);
  } else if ((alert_condition & MAX17048_STATUS_VL) != 0) {
    /*
     * Voltage low alert; may need to set parameters for reduced
     * energy use before reaching empty threshold
     */
    callback = fuel_gauge.interrupt_callback[IRQ_VCELL_LOW];
    callback(IRQ_VCELL_LOW, fuel_gauge.callback_data[IRQ_VCELL_LOW]);
    max17048_clear_alert_condition(alert_condition, MAX17048_STATUS_VL);
  } else if ((alert_condition & MAX17048_STATUS_VH) != 0) {
    /*
     * Voltage high alert; may indicate battery is full charged
     * and need to place charging IC in maintenance/trickle charge state
     */
    callback = fuel_gauge.interrupt_callback[IRQ_VCELL_HIGH];
    callback(IRQ_VCELL_HIGH, fuel_gauge.callback_data[IRQ_VCELL_HIGH]);
    max17048_clear_alert_condition(alert_condition, MAX17048_STATUS_VH);
  } else if ((alert_condition & MAX17048_STATUS_SC) != 0) {
    // SOC changed by 1%; lowest priority interrupt
    callback = fuel_gauge.interrupt_callback[IRQ_SOC];
    callback(IRQ_SOC, fuel_gauge.callback_data[IRQ_SOC]);
    max17048_clear_alert_condition(alert_condition, MAX17048_STATUS_SC);
  }

  // Clears the ALRT status bit to release the ALRT pin.
  max17048_clear_alert_status_bit();
}

/***************************************************************************//**
 * @brief This function identifies which alert condition was met.
 *
 * @param[out] alert_condition
 *   These bits are set when they are cause an alert.
 *   Values in the STATUS register:
 *   - <b>Bit 1:</b> (voltage high) is set when VCELL has been above
 *     ALRT.VALRTMAX.
 *   - <b>Bit 2:</b> (voltage low) is set when VCELL has been below
 *     ALRT.VALRTMIN.
 *   - <b>Bit 3:</b> (voltage reset) is set after the device has been reset if
 *     EnVr is set.
 *   - <b>Bit 4:</b> (SOC low) is set when SOC crosses the value in CONFIG.ATHD.
 *   - <b>Bit 5:</b> (1% SOC change) is set when SOC changes by at least 1% if
 *     CONFIG.ALSC is set.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
static sl_status_t max17048_get_alert_condition(uint8_t *alert_condition)
{
  sl_status_t status;
  uint8_t buffer[2];

  status = max17048_read_register(MAX17048_STATUS, buffer);
  if (status == SL_STATUS_OK) {
    *alert_condition = buffer[0];
  }

  return status;
}

/***************************************************************************//**
 * @brief
 *    This function clears the alert status bit.
 *
 * @details
 *    When an alert is triggered, the IC drives the ALRT pin logic-low
 *    and sets CONFIG.ALRT = 1. The ALRT pin remains logic-low until the system
 *    software writes CONFIG.ALRT = 0 to clear the alert.
 *    The alert function is enabled by default, so any alert can
 *    occur immediately upon power-up. Entering sleep mode clears no alerts.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
static sl_status_t max17048_clear_alert_status_bit(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  buffer[0] = fuel_gauge.rcomp_tracking;

  /*
   * Clear the ALRT bit by rewriting the lower byte of the CONFIG register.
   * The config_lower_tracking holds the state of SLEEP and ALSC
   * in addition to the ATHD field, so this will clear ALRT without modifying
   * the other bits in the lower byte of CONFIG.
   */
  buffer[1] = fuel_gauge.config_lower_tracking;
  status = max17048_write_register(MAX17048_CONFIG, buffer);

  return status;
}

/***************************************************************************//**
 * @brief
 *    This function clears the reset indicator bit.
 *
 * @details
 *     RI (reset indicator) is set when the device powers up.
 *     Any time this bit is set, the IC is not configured, so the
 *     model should be loaded and the bit should be cleared
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
static sl_status_t max17048_clear_reset_indicator_bit(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  status = max17048_read_register(MAX17048_STATUS, buffer);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Clear RI bit in the upper byte of the STATUS register if it is set
  if ((buffer[0] & MAX17048_STATUS_RI) != 0) {
    buffer[0] &= ~MAX17048_STATUS_RI;
    status = max17048_write_register(MAX17048_STATUS, buffer);
  }

  return status;
}

/**************************************************************************/ /**
 * @brief This function clears the correct flag for the specified alert.
 *
 * @param[in] alert_condition
 *    These bits are set when they are cause an alert.
 *
 * @param[in] source
 *    MAX17048 interrupt source.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
static sl_status_t max17048_clear_alert_condition(uint8_t alert_condition,
                                                  uint8_t source)
{
  sl_status_t status;
  uint8_t buffer[2] = { 0, 0 };

  /*
   * Clearing the flag for the specified alert also clears the RI bit.
   * RI bit, which is written to 0 to enable the loaded model, so there is
   * no issue with writing it to 0 again when the device is running normally.
   */
  buffer[0] = alert_condition & ~source;
  status = max17048_write_register(MAX17048_STATUS, buffer);

  return status;
}

/***************************************************************************//**
 *  Initialize the MAX17048.
 ******************************************************************************/
sl_status_t max17048_init(mikroe_i2c_handle_t i2cspm)
{
  sl_status_t status;
  i2c_master_config_t fuel_gauge_cfg;

  if (i2cspm == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  // If already initialized, return status
  if (fuel_gauge.is_initialized == true) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  i2c_master_configure_default(&fuel_gauge_cfg);
  fuel_gauge_cfg.addr = MAX17048_I2C_ADDRESS;
  fuel_gauge.i2c.handle = i2cspm;

#if (MAX17048_I2C_UC == 1)
  fuel_gauge_cfg.speed = MAX17048_I2C_SPEED_MODE;
#endif

#if defined(MAX17048_ALRT_PORT) && defined(MAX17048_ALRT_PIN)
  digital_in_pullup_init(&fuel_gauge.alrt_pin,
                         hal_gpio_pin_name(MAX17048_ALRT_PORT,
                                           MAX17048_ALRT_PIN));
#endif

  if (i2c_master_open(&fuel_gauge.i2c, &fuel_gauge_cfg) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  i2c_master_set_speed(&fuel_gauge.i2c, fuel_gauge_cfg.speed);
  i2c_master_set_timeout(&fuel_gauge.i2c, 0);

#if defined(MAX17048_ALRT_PORT) && defined(MAX17048_ALRT_PIN)
#if (defined(SLI_SI917))
  sl_gpio_t gpio_port_pin = { MAX17048_ALRT_PIN / 16,
                              MAX17048_ALRT_PIN % 16 };
  sl_gpio_driver_configure_interrupt(&gpio_port_pin,
                                     GPIO_M4_INTR,
                                     SL_GPIO_INTERRUPT_FALLING_EDGE,
                                     (void *)max17048_alrt_pin_callback,
                                     AVL_INTR_NO);
#else
  GPIOINT_Init();
  GPIO_PinModeSet(MAX17048_ALRT_PORT,
                  MAX17048_ALRT_PIN,
                  gpioModeInputPullFilter,
                  1);
  GPIO_ExtIntConfig(MAX17048_ALRT_PORT,
                    MAX17048_ALRT_PIN,
                    MAX17048_ALRT_PIN,
                    false,
                    true,
                    true);

  // Register the callback function that is invoked when interrupt occurs
  GPIOINT_CallbackRegister(MAX17048_ALRT_PIN,
                           max17048_alrt_pin_callback);
#endif
#endif

#if MAX17048_ENABLE_HW_QSTRT
#if defined(MAX17048_ENABLE_QSTRT_PORT) && defined(MAX17048_ENABLE_QSTRT_PIN)
  digital_out_init(&fuel_gauge.qstrt_pin,
                   hal_gpio_pin_name(MAX17048_ENABLE_QSTRT_PORT,
                                     MAX17048_ENABLE_QSTRT_PIN));
#endif
#endif

  /* The driver calculates and updates the RCOMP factor at a rate of
   * 1000 ms <= MAX17048_RCOMP_UPDATE_INTERVAL_MS <= 60000 ms
   * and defaults to 1 minute (60000 ms = 1 minute).
   */
  if ((fuel_gauge.rcomp_update_interval < 1000)
      || (fuel_gauge.rcomp_update_interval > 60000)) {
    return SL_STATUS_INVALID_RANGE;
  }

#if (defined(SLI_SI917))
  status = sl_si91x_bjt_temperature_sensor_init(sl_bjt_channel_config,
                                                sl_bjt_config);
  if (status != SL_STATUS_OK) {
    return status;
  }
#else
  TEMPDRV_Init();
#endif

  // Read and clear RI bit if it is set
  status = max17048_clear_reset_indicator_bit();
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Change drive state to initialized
  fuel_gauge.is_initialized = true;

// User-specified stabilization delay
#if MAX17048_STABILIZATION_DELAY > 0
  sl_sleeptimer_delay_millisecond(MAX17048_STABILIZATION_DELAY);
#endif

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  De-initialize the MAX17048.
 ******************************************************************************/
sl_status_t max17048_deinit(void)
{
  sl_status_t status;

  if (fuel_gauge.is_initialized == false) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  // De-initialization tasks
#if defined(MAX17048_ALRT_PORT) && defined(MAX17048_ALRT_PIN)
#if (defined(SLI_SI917))
  sl_gpio_t alert_port_pin = { MAX17048_ALRT_PIN / 16,
                               MAX17048_ALRT_PIN % 16 };
  sl_gpio_driver_set_pin_mode(&alert_port_pin,
                              SL_GPIO_MODE_DISABLED,
                              0);
#else
  GPIO_PinModeSet(MAX17048_ALRT_PORT,
                  MAX17048_ALRT_PIN,
                  gpioModeDisabled,
                  1);
#endif
#endif

#if MAX17048_ENABLE_HW_QSTRT
#if defined(MAX17048_ENABLE_QSTRT_PORT) && defined(MAX17048_ENABLE_QSTRT_PIN)
#if (defined(SLI_SI917))
  sl_gpio_t qstrt_port_pin = { MAX17048_ENABLE_QSTRT_PORT / 16,
                               MAX17048_ENABLE_QSTRT_PIN % 16 };
  sl_gpio_driver_set_pin_mode(&qstrt_port_pin,
                              SL_GPIO_MODE_DISABLED,
                              0);
#else
  GPIO_PinModeSet(MAX17048_ENABLE_QSTRT_PORT,
                  MAX17048_ENABLE_QSTRT_PIN,
                  gpioModeDisabled,
                  0);
#endif
#endif
#endif

  status = max17048_disable_soc_interrupt();
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = max17048_disable_empty_interrupt();
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = max17048_disable_vhigh_interrupt();
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = max17048_disable_vlow_interrupt();
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = max17048_disable_reset_interrupt();
  if (status != SL_STATUS_OK) {
    return status;
  }

  status = max17048_enter_sleep();
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Update i2cspm instance
  fuel_gauge.i2c.handle = NULL;

  // Mark driver as not initialized
  fuel_gauge.is_initialized = false;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  This function returns the cell voltage in millivolts.
 ******************************************************************************/
sl_status_t max17048_update_rcom(void)
{
#if (defined(SLI_SI917))
  double temp;
#else
  uint8_t temp;
#endif
  uint8_t rcomp;

  /*
   * Invoke sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1)
   * to prevent the EFM32/EFR32 from entering energy mode EM2
   * or lower during reading the temperature.
   */
#if (MAX17048_ENABLE_POWER_MANAGER == 1)
#if (defined(SLI_SI917))
  sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS1);
#else
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
#endif
#endif

#if (defined(SLI_SI917))
  sl_si91x_bjt_temperature_sensor_read_data(&temp);
#else
  temp = TEMPDRV_GetTemp();
#endif

  if (temp > 20) {
    rcomp = RCOMP0 + (temp - 20) * TEMP_CO_UP;
  } else {
    rcomp = RCOMP0 + (temp - 20) * TEMP_CO_DOWN;
  }

  sl_status_t status = max17048_set_rcomp(rcomp);

  /*
   * Call sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1)
   * to remove the requirement to remain in EM1 or higher
   * after writing rcomp to the MAX17048 is completed
   */
#if (MAX17048_ENABLE_POWER_MANAGER == 1)
#if (defined(SLI_SI917))
  sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS1);
#else
  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
#endif
#endif

  if (status != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  This function returns the cell voltage in millivolts.
 ******************************************************************************/
sl_status_t max17048_get_vcell(uint32_t *vcell)
{
  sl_status_t status;
  uint8_t buffer[2];
  uint32_t vcell_reg_val;

  if (vcell == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  status = max17048_read_register(MAX17048_VCELL, buffer);
  vcell_reg_val = (buffer[0] << 8) | buffer[1];
  *vcell =
    (uint32_t)((uint64_t)(vcell_reg_val) * MAX17048_VCELL_RESOLUTION / 1000000);

  return status;
}

/***************************************************************************//**
 *  Read the SOC register and return the state-of-charge
 *  as an integer (0 - 100%).
 ******************************************************************************/
sl_status_t max17048_get_soc(uint32_t *soc)
{
  sl_status_t status;
  uint8_t buffer[2];
  uint16_t soc_reg_val;

  if (soc == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  status = max17048_read_register(MAX17048_SOC, buffer);
  soc_reg_val = (uint16_t)((buffer[0] << 8) | buffer[1]);
  *soc = (uint32_t)(soc_reg_val / MAX17048_SOC_RESOLUTION);

  return status;
}

/***************************************************************************//**
 *  This function gets an approximate value for the average SOC rate of change.
 ******************************************************************************/
sl_status_t max17048_get_crate(float *crate)
{
  sl_status_t status;
  uint8_t buffer[2];
  uint16_t crate_reg_val;

  if (crate == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  status = max17048_read_register(MAX17048_CRATE, buffer);
  crate_reg_val = (buffer[0] << 8) | buffer[1];
  *crate = (float)(crate_reg_val * MAX17048_CRATE_RESOLUTION);

  return status;
}

/***************************************************************************//**
 *  Set the RCOMP update interval.
 ******************************************************************************/
sl_status_t max17048_set_update_interval(uint32_t interval)
{
  sl_status_t status;

  if ((interval < 1000) || (interval > 60000)) {
    return SL_STATUS_INVALID_RANGE;
  }

  /*
   * Get the temperature, update the RCOMP value before restarting Sleeptimer
   * with the new update interval.
   */
  max17048_temp_timer_callback(NULL, NULL);

  fuel_gauge.rcomp_update_interval = interval;
  status = sl_sleeptimer_restart_periodic_timer_ms(
    &fuel_gauge.temp_timer_handle,
    fuel_gauge.rcomp_update_interval,
    max17048_temp_timer_callback,
    (void *)NULL,
    0,
    0);

  return status;
}

/***************************************************************************//**
 *  Get the RCOMP update interval.
 ******************************************************************************/
uint32_t max17048_get_update_interval(void)
{
  return fuel_gauge.rcomp_update_interval;
}

/***************************************************************************//**
 *  Mask MAX17048 interrupts
 ******************************************************************************/
void max17048_mask_interrupts(void)
{
#if (defined(SLI_SI917))
  sl_gpio_driver_clear_interrupts(GPIO_M4_INTR);
  sl_gpio_driver_disable_interrupts(
    (GPIO_M4_INTR << 16) | SL_GPIO_INTERRUPT_FALL_EDGE);
#else
  GPIO_IntDisable(1 << MAX17048_ALRT_PIN);
#endif
}

/***************************************************************************//**
 *  Unmask MAX17048 interrupts
 ******************************************************************************/
void max17048_unmask_interrupts(void)
{
  /*
   * Clear ALRT pin interrupt flag in case an edge was detected while
   * the interrupt was disabled.
   */
#if (defined(SLI_SI917))
  sl_gpio_driver_enable_interrupts(
    (GPIO_M4_INTR << 16) | SL_GPIO_INTERRUPT_FALL_EDGE);
#else
  GPIO_IntClear(1 << MAX17048_ALRT_PIN);
  GPIO_IntEnable(1 << MAX17048_ALRT_PIN);
#endif
}

/***************************************************************************//**
 *  Enable alerting when SOC changes.
 ******************************************************************************/
sl_status_t max17048_enable_soc_interrupt(interrupt_callback_t irq_cb,
                                          void *cb_data)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((irq_cb == NULL) || (cb_data == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  if ((fuel_gauge.interrupt_callback[IRQ_SOC] != NULL)
      || (fuel_gauge.callback_data[IRQ_SOC] != NULL)) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  fuel_gauge.interrupt_callback[IRQ_SOC] = irq_cb;
  fuel_gauge.callback_data[IRQ_SOC] = cb_data;
  // Get the upper byte of the CONFIG register
  buffer[0] = fuel_gauge.rcomp_tracking;

  /*
   * Set ALSC bit in the lower byte of the CONFIG register
   * to enable alerting when SOC changes
   */
  fuel_gauge.config_lower_tracking |= MAX17048_CONFIG_ALSC;
  // Get the lower byte of the CONFIG register
  buffer[1] = fuel_gauge.config_lower_tracking;

  status = max17048_write_register(MAX17048_CONFIG, buffer);

  return status;
}

/***************************************************************************//**
 *  Disables the MAX17048 state-of-charge (SOC) interrupt and unregisters
 *  the user-provided callback function.
 ******************************************************************************/
sl_status_t max17048_disable_soc_interrupt(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((fuel_gauge.interrupt_callback[IRQ_SOC] == NULL)
      || (fuel_gauge.callback_data[IRQ_SOC] == NULL)) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  fuel_gauge.interrupt_callback[IRQ_SOC] = NULL;
  fuel_gauge.callback_data[IRQ_SOC] = NULL;

  // Get the upper byte of the CONFIG register
  buffer[0] = fuel_gauge.rcomp_tracking;

  /*
   * Clear ALSC bit in the lower byte of the CONFIG register
   * to disable alerting when SOC changes
   */
  fuel_gauge.config_lower_tracking &= ~MAX17048_CONFIG_ALSC;
  // Get the lower byte of the CONFIG register
  buffer[1] = fuel_gauge.config_lower_tracking;
  status = max17048_write_register(MAX17048_CONFIG, buffer);

  return status;
}

/***************************************************************************//**
 * Enables the empty alert interrupt and set its threshold.
 ******************************************************************************/
sl_status_t max17048_enable_empty_interrupt(uint8_t athd,
                                            interrupt_callback_t irq_cb,
                                            void *cb_data)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((athd < 1) || (athd > 32)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if ((irq_cb == NULL) || (cb_data == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  if ((fuel_gauge.interrupt_callback[IRQ_EMPTY] != NULL)
      || (fuel_gauge.callback_data[IRQ_EMPTY] != NULL)) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  fuel_gauge.interrupt_callback[IRQ_EMPTY] = irq_cb;
  fuel_gauge.callback_data[IRQ_EMPTY] = cb_data;
  // Get the upper byte of the CONFIG register
  buffer[0] = fuel_gauge.rcomp_tracking;
  // Update the private global variable to track
  fuel_gauge.config_lower_tracking &= 0xE0;
  fuel_gauge.config_lower_tracking |= 32 - athd;
  // Update the lower byte of the CONFIG register
  buffer[1] = fuel_gauge.config_lower_tracking;

  status = max17048_write_register(MAX17048_CONFIG, buffer);

  return status;
}

/***************************************************************************//**
 * Disables the MAX17048 empty alert interrupt and unregisters the
 * the user-provided callback function.
 ******************************************************************************/
sl_status_t max17048_disable_empty_interrupt(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((fuel_gauge.interrupt_callback[IRQ_EMPTY] == NULL)
      || (fuel_gauge.callback_data[IRQ_EMPTY] == NULL)) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  fuel_gauge.interrupt_callback[IRQ_EMPTY] = NULL;
  fuel_gauge.callback_data[IRQ_EMPTY] = NULL;

  // Get the upper byte of the CONFIG register
  buffer[0] = fuel_gauge.rcomp_tracking;

  /*
   * Update the private global variable to track.
   * Setting a threshold of 1% (ATHD = 31) should prevent the interrupt
   * from being requested and the driver will ignore it.
   */
  fuel_gauge.config_lower_tracking &= 0xE0;
  fuel_gauge.config_lower_tracking |= 0x1F;
  // Update the lower byte of the CONFIG register
  buffer[1] = fuel_gauge.config_lower_tracking;

  status = max17048_write_register(MAX17048_CONFIG, buffer);

  return status;
}

/***************************************************************************//**
 *  Set the empty alert threshold.
 ******************************************************************************/
sl_status_t max17048_set_empty_threshold(uint8_t athd)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((athd == 0) || (athd > 32)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Get the upper byte of the CONFIG register
  buffer[0] = fuel_gauge.rcomp_tracking;
  // Update the private global variable to track
  fuel_gauge.config_lower_tracking &= 0xE0;
  fuel_gauge.config_lower_tracking |= 32 - athd;
  // Update the lower byte of the CONFIG register
  buffer[1] = fuel_gauge.config_lower_tracking;

  status = max17048_write_register(MAX17048_CONFIG, buffer);

  return status;
}

/***************************************************************************//**
 *  Get the empty threshold.
 ******************************************************************************/
uint8_t max17048_get_empty_threshold(void)
{
  return (32 - (fuel_gauge.config_lower_tracking & 0x1F));
}

/***************************************************************************//**
 *  Enable the voltage high alert interrupt, sets its threshold, and
 *  register a user-provided callback function to respond to it.
 ******************************************************************************/
sl_status_t max17048_enable_vhigh_interrupt(uint32_t valrt_max_mv,
                                            interrupt_callback_t irq_cb,
                                            void *cb_data)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((irq_cb == NULL) || (cb_data == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  if ((fuel_gauge.interrupt_callback[IRQ_VCELL_HIGH] != NULL)
      || (fuel_gauge.callback_data[IRQ_VCELL_HIGH] != NULL)) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  fuel_gauge.interrupt_callback[IRQ_VCELL_HIGH] = irq_cb;
  fuel_gauge.callback_data[IRQ_VCELL_HIGH] = cb_data;

  if (valrt_max_mv > MAX17048_VALRT_MAX_MV) {
    valrt_max_mv = MAX17048_VALRT_MAX_MV;
  }

  buffer[0] = fuel_gauge.valrt_min_tracking; // Get the VALRT.MIN register
  // Update the private global variable to track
  fuel_gauge.valrt_max_tracking =
    (uint8_t)(valrt_max_mv / MAX17048_VALRT_RESOLUTION);
  buffer[1] = fuel_gauge.valrt_max_tracking; // Update the VALRT.MAX register
  status = max17048_write_register(MAX17048_VALRT, buffer);

  return status;
}

/***************************************************************************//**
 *  Disables the MAX17048 voltage high alert interrupt and
 *  unregisters the user-provided callback function.
 ******************************************************************************/
sl_status_t max17048_disable_vhigh_interrupt(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((fuel_gauge.interrupt_callback[IRQ_VCELL_HIGH] == NULL)
      || (fuel_gauge.callback_data[IRQ_VCELL_HIGH] == NULL)) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  fuel_gauge.interrupt_callback[IRQ_VCELL_HIGH] = NULL;
  fuel_gauge.callback_data[IRQ_VCELL_HIGH] = NULL;

  // Get the VALRT.MIN register
  buffer[0] = fuel_gauge.valrt_min_tracking;
  // Update the private global variable to track
  fuel_gauge.valrt_max_tracking = 0xFF;
  // Update the VALRT.MAX register
  buffer[1] = fuel_gauge.valrt_max_tracking;
  status = max17048_write_register(MAX17048_VALRT, buffer);

  return status;
}

/***************************************************************************//**
 *  Set the voltage high alert interrupt threshold in millivolts.
 ******************************************************************************/
sl_status_t max17048_set_vhigh_threshold(uint32_t valrt_max_mv)
{
  sl_status_t status;
  uint8_t buffer[2];

  if (valrt_max_mv > MAX17048_VALRT_MAX_MV) {
    valrt_max_mv = MAX17048_VALRT_MAX_MV;
  }

  // Get the VALRT.MIN register
  buffer[0] = fuel_gauge.valrt_min_tracking;
  // Update the private global variable to track
  fuel_gauge.valrt_max_tracking =
    (uint8_t)(valrt_max_mv / MAX17048_VALRT_RESOLUTION);
  // Update the VALRT.MAX register
  buffer[1] = fuel_gauge.valrt_max_tracking;
  status = max17048_write_register(MAX17048_VALRT, buffer);

  return status;
}

/***************************************************************************//**
 *  Get the voltage high alert interrupt threshold in millivolts.
 ******************************************************************************/
uint32_t max17048_get_vhigh_threshold(void)
{
  return (uint32_t)(fuel_gauge.valrt_max_tracking * MAX17048_VALRT_RESOLUTION);
}

/***************************************************************************//**
 *  Enable the voltage low alert interrupt, sets its threshold, and
 *  register a user-provided callback function to respond to it.
 ******************************************************************************/
sl_status_t max17048_enable_vlow_interrupt(uint32_t valrt_min_mv,
                                           interrupt_callback_t irq_cb,
                                           void *cb_data)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((irq_cb == NULL) || (cb_data == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  if ((fuel_gauge.interrupt_callback[IRQ_VCELL_LOW] != NULL)
      || (fuel_gauge.callback_data[IRQ_VCELL_LOW] != NULL)) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  fuel_gauge.interrupt_callback[IRQ_VCELL_LOW] = irq_cb;
  fuel_gauge.callback_data[IRQ_VCELL_LOW] = cb_data;

  if (valrt_min_mv > MAX17048_VALRT_MIN_MV) {
    valrt_min_mv = MAX17048_VALRT_MIN_MV;
  }

  // Get the VALRT.MAX register
  buffer[1] = fuel_gauge.valrt_max_tracking;
  // Update the private global variable to track
  fuel_gauge.valrt_min_tracking =
    (uint8_t)(valrt_min_mv / MAX17048_VALRT_RESOLUTION);
  // Update the VALRT.MIN register
  buffer[0] = fuel_gauge.valrt_min_tracking;
  status = max17048_write_register(MAX17048_VALRT, buffer);

  return status;
}

/***************************************************************************//**
 *  Disables the MAX17048 voltage low alert interrupt and
 *  unregisters the user-provided callback function.
 ******************************************************************************/
sl_status_t max17048_disable_vlow_interrupt(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((fuel_gauge.interrupt_callback[IRQ_VCELL_LOW] == NULL)
      || (fuel_gauge.callback_data[IRQ_VCELL_LOW] == NULL)) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  fuel_gauge.interrupt_callback[IRQ_VCELL_LOW] = NULL;
  fuel_gauge.callback_data[IRQ_VCELL_LOW] = NULL;

  // Get the VALRT.MAX register
  buffer[1] = fuel_gauge.valrt_max_tracking;
  // Update the private global variable to track
  fuel_gauge.valrt_min_tracking = 0x00;
  // Update the VALRT.MIN register
  buffer[0] = fuel_gauge.valrt_min_tracking;
  status = max17048_write_register(MAX17048_VALRT, buffer);

  return status;
}

/***************************************************************************//**
 *  Set the voltage low alert interrupt threshold in millivolts.
 ******************************************************************************/
sl_status_t max17048_set_vlow_threshold(uint32_t valrt_min_mv)
{
  sl_status_t status;
  uint8_t buffer[2];

  if (valrt_min_mv > MAX17048_VALRT_MIN_MV) {
    valrt_min_mv = MAX17048_VALRT_MIN_MV;
  }

  // Get the VALRT.MAX register
  buffer[1] = fuel_gauge.valrt_max_tracking;
  // Update the private global variable to track
  fuel_gauge.valrt_min_tracking =
    (uint8_t)(valrt_min_mv / MAX17048_VALRT_RESOLUTION);
  // Update the VALRT.MIN register
  buffer[0] = fuel_gauge.valrt_min_tracking;
  status = max17048_write_register(MAX17048_VALRT, buffer);

  return status;
}

/***************************************************************************//**
 *  Get the voltage low alert interrupt threshold in millivolts.
 ******************************************************************************/
uint32_t max17048_get_vlow_threshold(void)
{
  return (uint32_t)(fuel_gauge.valrt_min_tracking * MAX17048_VALRT_RESOLUTION);
}

/***************************************************************************//**
 *  Enable the voltage reset alert interrupt and set its threshold.
 ******************************************************************************/
sl_status_t max17048_enable_reset_interrupt(uint32_t vreset_mv,
                                            interrupt_callback_t irq_cb,
                                            void *cb_data)
{
  sl_status_t status;
  uint8_t buffer[2];
  uint8_t vreset_val;

  if ((irq_cb == NULL) || (cb_data == NULL)) {
    return SL_STATUS_NULL_POINTER;
  }

  if ((fuel_gauge.interrupt_callback[IRQ_RESET] != NULL)
      || (fuel_gauge.callback_data[IRQ_RESET] != NULL)) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  if (vreset_mv > MAX17048_VRESET_MV) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  fuel_gauge.interrupt_callback[IRQ_RESET] = irq_cb;
  fuel_gauge.callback_data[IRQ_RESET] = cb_data;

  /*
   * Set EnVR bit in the upper of the STATUS register
   * to enable voltage reset alert.
   * By doing this, RI bit is clear.
   * RI bit, which is written to 0 to enable the loaded model, so there is
   * no issue with writing it to 0 again when the device is running normally.
   */
  buffer[0] = MAX17048_STATUS_ENVR;
  // Lower byte of STATUS is not implemented; writing to it has no effect
  buffer[1] = 0x00;
  status = max17048_write_register(MAX17048_STATUS, buffer);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Update the private global variable to track
  fuel_gauge.vreset_tracking &= 0x01;
  vreset_val = (uint8_t)(vreset_mv / MAX17048_VRESET_RESOLUTION);
  fuel_gauge.vreset_tracking |= (vreset_val << MAX17048_VRESET_SHIFT);
  // Update the VRESET register
  buffer[0] = fuel_gauge.vreset_tracking;
  // Writing to the ID register has no effect
  buffer[1] = 0x00;
  status = max17048_write_register(MAX17048_VRESET_ID, buffer);

  return status;
}

/***************************************************************************//**
 *  Disables the MAX17048 reset alert interrupt and unregisters
 *  the user-provided callback function.
 ******************************************************************************/
sl_status_t max17048_disable_reset_interrupt(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((fuel_gauge.interrupt_callback[IRQ_RESET] == NULL)
      || (fuel_gauge.callback_data[IRQ_RESET] == NULL)) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  fuel_gauge.interrupt_callback[IRQ_RESET] = NULL;
  fuel_gauge.callback_data[IRQ_RESET] = NULL;

  /*
   * Clear EnVR bit in the upper byte of the STATUS register
   * to disable voltage reset alert
   */
  buffer[0] = 0x00;
  // Lower byte of STATUS is not implemented; writing to it has no effect
  buffer[1] = 0x00;
  status = max17048_write_register(MAX17048_STATUS, buffer);

  return status;
}

/***************************************************************************//**
 *  Set the reset alert interrupt threshold in millivolts.
 ******************************************************************************/
sl_status_t max17048_set_reset_threshold(uint32_t vreset_mv)
{
  sl_status_t status;
  uint8_t buffer[2];
  uint8_t vreset_val;

  if (vreset_mv > MAX17048_VRESET_MV) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Update the private global variable to track
  fuel_gauge.vreset_tracking &= MAX17048_VRESET_DIS;
  vreset_val = (uint8_t)(vreset_mv / MAX17048_VRESET_RESOLUTION);
  fuel_gauge.vreset_tracking |= (vreset_val << MAX17048_VRESET_SHIFT);
  // Update the VRESET register
  buffer[0] = fuel_gauge.vreset_tracking;
  // Writing to the ID register has no effect
  buffer[1] = 0x00;
  status = max17048_write_register(MAX17048_VRESET_ID, buffer);

  return status;
}

/***************************************************************************//**
 *  Get the reset alert interrupt threshold in millivolts.
 ******************************************************************************/
uint32_t max17048_get_reset_threshold(void)
{
  return (uint32_t)((fuel_gauge.vreset_tracking >>
                     MAX17048_VRESET_SHIFT) * MAX17048_VRESET_RESOLUTION);
}

/***************************************************************************//**
 *  Read hibernate mode status.
 ******************************************************************************/
sl_status_t max17048_get_hibernate_state(max17048_hibstate_t *hibstat)
{
  sl_status_t status;
  uint8_t buffer[2];

  status = max17048_read_register(MAX17048_MODE, buffer);
  if (status == SL_STATUS_OK) {
    // Get HibStat bit in the upper byte of the MODE register
    *hibstat = (max17048_hibstate_t)((buffer[0] & MAX17048_MODE_HIBSTAT) != 0);
  }

  return status;
}

/***************************************************************************//**
 *  Set thresholds for entering and exiting hibernate mode.
 ******************************************************************************/
sl_status_t max17048_enable_auto_hibernate(float hib_thr, uint32_t act_thr)
{
  sl_status_t status;
  uint8_t buffer[2];

  if ((hib_thr == 0.0) || (act_thr == 0)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (hib_thr > MAX17048_HIBTHR_PERCENT) {
    hib_thr = MAX17048_HIBTHR_PERCENT;
  }

  if (act_thr > MAX17048_ACTTHR_MV) {
    act_thr = MAX17048_ACTTHR_MV;
  }

  // Update the private global variables to track
  fuel_gauge.hibthr_tracking = (uint8_t)(hib_thr / MAX17048_HIBTHR_RESOLUTION);
  fuel_gauge.actthr_tracking =
    (uint8_t)(act_thr * 1000 / MAX17048_ACTTHR_RESOLUTION);

  buffer[0] = fuel_gauge.hibthr_tracking;
  buffer[1] = fuel_gauge.actthr_tracking;
  status = max17048_write_register(MAX17048_HIBRT, buffer);

  return status;
}

/***************************************************************************//**
 *  Disables automatic hibernation by setting the activity and
 *  CRATE thresholds to 0x0.
 ******************************************************************************/
sl_status_t max17048_disable_auto_hibernate(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  // Update the private global variables to track
  fuel_gauge.hibthr_tracking = 0x00;
  fuel_gauge.actthr_tracking = 0x00;

  buffer[0] = fuel_gauge.hibthr_tracking;
  buffer[1] = fuel_gauge.actthr_tracking;
  status = max17048_write_register(MAX17048_HIBRT, buffer);

  return status;
}

/***************************************************************************//**
 *  Set the hibernate threshold level in percent.
 ******************************************************************************/
sl_status_t max17048_set_hibernate_threshold(float hib_thr)
{
  sl_status_t status;
  uint8_t buffer[2];

  if (hib_thr > MAX17048_HIBTHR_PERCENT) {
    hib_thr = MAX17048_HIBTHR_PERCENT;
  }
  // Update the private global variable to track
  fuel_gauge.hibthr_tracking = (uint8_t)(hib_thr / MAX17048_HIBTHR_RESOLUTION);
  buffer[0] = fuel_gauge.hibthr_tracking;
  buffer[1] = fuel_gauge.actthr_tracking;
  status = max17048_write_register(MAX17048_HIBRT, buffer);

  return status;
}

/***************************************************************************//**
 *  Get the hibernate threshold level in percent.
 ******************************************************************************/
float max17048_get_hibernate_threshold(void)
{
  return (float)(fuel_gauge.hibthr_tracking * MAX17048_HIBTHR_RESOLUTION);
}

/***************************************************************************//**
 *  Set the hibernate threshold level in minivolts.
 ******************************************************************************/
sl_status_t max17048_set_activity_threshold(uint32_t act_thr)
{
  sl_status_t status;
  uint8_t buffer[2];

  if (act_thr > MAX17048_ACTTHR_MV) {
    act_thr = MAX17048_ACTTHR_MV;
  }

  // Update the private global variable to track
  fuel_gauge.actthr_tracking =
    (uint8_t)(act_thr * 1000 / MAX17048_ACTTHR_RESOLUTION);
  buffer[0] = fuel_gauge.hibthr_tracking;
  buffer[1] = fuel_gauge.actthr_tracking;
  status = max17048_write_register(MAX17048_HIBRT, buffer);

  return status;
}

/***************************************************************************//**
 *  Get the activity threshold level in minivolts.
 ******************************************************************************/
uint32_t max17048_get_activity_threshold(void)
{
  return (uint32_t)(fuel_gauge.actthr_tracking * MAX17048_ACTTHR_RESOLUTION
                    / 1000);
}

/***************************************************************************//**
 *  Enables/disables the analog comparator in hibernate mode.
 ******************************************************************************/
sl_status_t max17048_enable_reset_comparator(bool enable)
{
  sl_status_t status;
  uint8_t buffer[2];

  if (enable) {
    // VRESET_DIS = 1 to disable the comparator in hibernate mode
    fuel_gauge.vreset_tracking |= MAX17048_VRESET_DIS;
  } else {
    // VRESET_DIS = 0 to keep the comparator enabled in hibernate mode
    fuel_gauge.vreset_tracking &= ~MAX17048_VRESET_DIS;
  }

  // Update the VRESET register
  buffer[0] = fuel_gauge.vreset_tracking;
  // Writing to the ID register has no effect
  buffer[1] = 0x00;
  status = max17048_write_register(MAX17048_VRESET_ID, buffer);

  return status;
}

/***************************************************************************//**
 *  Force the MAX17048 to enter sleep mode.
 ******************************************************************************/
sl_status_t max17048_enter_sleep(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  // Set EnSleep bit in the upper byte of the MODE register to enable sleep mode
  buffer[0] = MAX17048_MODE_ENSLEEP;
  // Lower byte of MODE is not implemented; writing to it has no effect
  buffer[1] = 0x00;
  status = max17048_write_register(MAX17048_MODE, buffer);
  if (status != SL_STATUS_OK) {
    return status;
  }
  // Get the upper byte of the CONFIG register
  buffer[0] = fuel_gauge.rcomp_tracking;
  fuel_gauge.config_lower_tracking |= MAX17048_CONFIG_SLEEP;
  // Get the lower byte of the CONFIG register
  buffer[1] = fuel_gauge.config_lower_tracking;
  status = max17048_write_register(MAX17048_CONFIG, buffer);

  return status;
}

/***************************************************************************//**
 *  Forces the MAX17048 to exit sleep mode.
 ******************************************************************************/
sl_status_t max17048_exit_sleep(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  // Get the upper byte of the CONFIG register
  buffer[0] = fuel_gauge.rcomp_tracking;
  // Clear SLEEP in the lower byte of the CONFIG register to exit sleep mode
  fuel_gauge.config_lower_tracking &= ~MAX17048_CONFIG_SLEEP;
  // Get the lower byte of the CONFIG register
  buffer[1] = fuel_gauge.config_lower_tracking;
  status = max17048_write_register(MAX17048_CONFIG, buffer);

  return status;
}

/***************************************************************************//**
 *  Force the MAX17048 to initiate a power-on reset (POR).
 ******************************************************************************/
sl_status_t max17048_force_reset(void)
{
  sl_status_t status;
  uint8_t buffer[2];

  // Force all tracking variables to their default values
  fuel_gauge.config_lower_tracking = 0x1C;
  fuel_gauge.rcomp_tracking = 0x97;
  fuel_gauge.valrt_max_tracking = 0xFF;
  fuel_gauge.valrt_min_tracking = 0x00;
  fuel_gauge.vreset_tracking = 0x96;
  fuel_gauge.hibthr_tracking = 0x80;
  fuel_gauge.actthr_tracking = 0x30;

  buffer[0] = MAX17048_RESET_UPPER_BYTE;
  buffer[1] = MAX17048_RESET_LOWER_BYTE;
  status = max17048_write_register(MAX17048_CMD, buffer);

  return status;
}

/***************************************************************************//**
 *  Force the MAX17048 to initiate a battery quick start.
 ******************************************************************************/
sl_status_t max17048_force_quick_start(void)
{
  sl_status_t status;

#if !MAX17048_ENABLE_HW_QSTRT
  uint8_t buffer[2];

  // Lower byte of MODE is not implemented; writing to it has no effect.
  buffer[1] = 0;

  /*
   * Note that when forcing a quickstart, the device cannot be in sleep
   * mode because the system is assumed to have just powered and the OCV
   * is still settling.  It is thus permissible to write EnSleep = 0.
   */
  buffer[0] = MAX17048_MODE_QUICK_START;
  status = max17048_write_register(MAX17048_MODE, buffer);
#else

#if defined(MAX17048_ENABLE_QSTRT_PORT) && defined(MAX17048_ENABLE_QSTRT_PIN)
  // Assert GPIO pin connected to QSTRT and delay 1 ms.
  digital_out_high(&fuel_gauge.qstrt_pin);
#endif

  status = sl_sleeptimer_start_timer_ms(&fuel_gauge.quick_start_timer_handle,
                                        1,
                                        max17048_quick_start_callback,
                                        (void *)NULL,
                                        0,
                                        0);
#endif /* MAX17048_ENABLE_HW_QSTRT */

  return status;
}

/***************************************************************************//**
 *  Load a custom model into the MAX17048 and enable it.
 ******************************************************************************/
sl_status_t max17048_load_model(const uint8_t *model)
{
  sl_status_t status;
  uint8_t buffer[2];
  uint8_t i2c_write_data[65];

  if (model == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  // Unlock the model
  buffer[0] = MAX17048_UNLOCK_UPPER_BYTE;
  buffer[1] = MAX17048_UNLOCK_LOWER_BYTE;

  status = max17048_write_register(MAX17048_LOCK_TABLE, buffer);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Write the model
  // Copy model and table start address into transfer buffer
  i2c_write_data[0] = MAX17048_TABLE;
  for (uint8_t i = 0; i < 64; i++) {
    i2c_write_data[i + 1] = model[i];
  }

  /*
   * Invoke sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1)
   * to prevent the EFM32/EFR32 from entering energy mode EM2
   * or lower during I2C bus activity.
   */
#if (MAX17048_ENABLE_POWER_MANAGER == 1)
#if (defined(SLI_SI917))
  sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS1);
#else
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
#endif
#endif

  if (I2C_MASTER_SUCCESS != i2c_master_write(&fuel_gauge.i2c,
                                             i2c_write_data,
                                             sizeof(i2c_write_data))) {
    return SL_STATUS_TRANSMIT;
  }
  return SL_STATUS_OK;

  /*
   * Call sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1)
   * to remove the requirement to remain in EM1 or higher
   * after I2C bus activity is complete.
   */
#if (MAX17048_ENABLE_POWER_MANAGER == 1)
#if (defined(SLI_SI917))
  sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS1);
#else
  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
#endif
#endif

  // Lock and load the new model
  buffer[0] = 0x0;
  buffer[1] = 0x0;

  status = max17048_write_register(MAX17048_LOCK_TABLE, buffer);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Read and clear RI bit if it is set
  max17048_clear_reset_indicator_bit();

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Read the MAX17048 device ID.
 ******************************************************************************/
sl_status_t max17048_get_id(uint8_t *id)
{
  sl_status_t status;
  uint8_t buffer[2];

  if (id == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  status = max17048_read_register(MAX17048_VRESET_ID, buffer);
  *id = buffer[1];

  return status;
}

/***************************************************************************//**
 *  Read the MAX17048 production version.
 ******************************************************************************/
sl_status_t max17048_get_production_version(uint16_t *ver)
{
  sl_status_t status;
  uint8_t buffer[2];

  if (ver == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  status = max17048_read_register(MAX17048_VERSION, buffer);
  // Return VERSION bytes in the correct order.
  *ver = (buffer[0] << 8) | buffer[1];

  return status;
}
