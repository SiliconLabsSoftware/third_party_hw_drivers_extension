/***************************************************************************//**
 * @file adafruit_ili9341_touch.h
 * @brief Adafruit ILI9341 TFT LCD with Touchscreen Header File
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
// -----------------------------------------------------------------------------
//                       Includes
// -----------------------------------------------------------------------------
#include "sl_si91x_adc.h"
#include "gpio_helper_si91x.h"
#include "touch_screen.h"
#include "touch_screen_config.h"

// -----------------------------------------------------------------------------
//                       Macros
// -----------------------------------------------------------------------------
#define DELAY_10US_COUNTER             46             // Delay count

#define PS4_SOC_FREQ                   180000000 /*<! PLL out clock 180MHz            */
#define SOC_PLL_REF_FREQUENCY          40000000 /*<! PLL input REFERENCE clock 40MHZ */
#define DVISION_FACTOR                 0 // Division factor
#define CHANNEL_SAMPLE_LENGTH          4 // Number of ADC sample collect for operation
#define ADC_PING_BUFFER                0x0000A000 // ADC buffer starting address
#define AUXADC_DATA_TWELFTH            BIT(11) // Reading twelfth bit of AUXADC_DATA reg

#define SL_ADC_CHANNEL_0               0

// -----------------------------------------------------------------------------
//                       Local Functions
// -----------------------------------------------------------------------------
static void delay_10us(uint32_t idelay);
static void set_input(enum TOUCH_SCREEN_CHANNEL channel);
static void set_output(enum TOUCH_SCREEN_CHANNEL channel, uint8_t value);
static sl_status_t adc_start_read(enum TOUCH_SCREEN_CHANNEL channel);
static uint16_t adc_read_u12(void);
static void adc_stop(void);

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

static const struct touch_screen_config touch_screen_config =
{
  .h_res = TOUCHSCREEN_HOR_RES,
  .v_res = TOUCHSCREEN_VER_RES,
  .xplate_res = TOUCHSCREEN_XPLATE_RES,
  .x_min = TOUCHSCREEN_X_MIN,
  .x_max = TOUCHSCREEN_X_MAX,
  .y_min = TOUCHSCREEN_Y_MIN,
  .y_max = TOUCHSCREEN_Y_MAX,
  .x_inv = TOUCHSCREEN_X_INV,
  .y_inv = TOUCHSCREEN_Y_INV,
  .xy_swap = TOUCHSCREEN_XY_SWAP
};

static const struct touch_screen_analog_interface touch_screen_analog_interface
  = {
  .delay_10us = delay_10us,
  .set_input = set_input,
  .set_output = set_output,
  .adc_start_read = adc_start_read,
  .adc_read_u12 = adc_read_u12,
  .adc_stop = adc_stop
  };

static sl_adc_config_t adc_config = {
  .num_of_channel_enable = 1,
  .operation_mode = SL_ADC_STATIC_MODE,
};

static sl_adc_channel_config_t adc_channel_config = {
  .channel = SL_ADC_CHANNEL_0,
  .input_type[SL_ADC_CHANNEL_0] = SL_ADC_SINGLE_ENDED,
  .pos_inp_sel[SL_ADC_CHANNEL_0] = TOUCHSCREEN_XM_POS_INPUT_CHNL_SEL,
  .neg_inp_sel[SL_ADC_CHANNEL_0] = TOUCHSCREEN_XM_NEG_INPUT_CHNL_SEL,
  .num_of_samples[SL_ADC_CHANNEL_0] = 1,
  .sampling_rate[SL_ADC_CHANNEL_0] = 100000,
};
static uint8_t channel = 0;

enum TOUCH_SCREEN_CHANNEL touch_screen_channel = TOUCH_SCREEN_CHANNEL_XM;

static volatile bool adc_complete = false;

// -----------------------------------------------------------------------------
//                       Local Functions
// -----------------------------------------------------------------------------

/*******************************************************************************
 * Callback event function
 * It is responsible for the event which are triggered by ADC interface
 * @param  event       : INTERNAL_DMA => Single channel data acquisition done.
 *                       ADC_STATIC_MODE_CALLBACK => Static mode adc data
 *                       acquisition done.
 ******************************************************************************/
static void callback_event(uint8_t channel_no, uint8_t event)
{
  (void) channel_no;

  switch (event) {
    case SL_INTERNAL_DMA:
      adc_complete = true;
      break;
    case SL_ADC_STATIC_MODE_EVENT:
      adc_complete = true;
      break;
  }
}

static sl_status_t adc_init(void)
{
  sl_status_t status;

  // Initialize ADC
  status = sl_si91x_adc_init(adc_channel_config,
                             adc_config,
                             (float)3.3f);
  if (SL_STATUS_OK != status) {
    return status;
  }

  // Register user callback function
  status = sl_si91x_adc_register_event_callback(callback_event);
  if (SL_STATUS_OK != status) {
    return status;
  }

  adc_complete = false;

  return SL_STATUS_OK;
}

static sl_status_t adc_start(uint8_t pos_inp_sel)
{
  sl_status_t status;

  channel = adc_channel_config.channel;
  adc_channel_config.pos_inp_sel[channel] = pos_inp_sel;

  status = sl_si91x_adc_configure_static_mode(adc_channel_config, channel);
  if (SL_STATUS_OK != status) {
    return status;
  }

  // Configure ADC channel.
  status = sl_si91x_adc_set_channel_configuration(adc_channel_config,
                                                  adc_config);
  if (SL_STATUS_OK != status) {
    return status;
  }

  // Start the ADC conversion
  status = sl_si91x_adc_start(adc_config);
  if (SL_STATUS_OK != status) {
    return status;
  }

  return SL_STATUS_OK;
}

static sl_status_t adc_read(uint16_t *data)
{
  sl_status_t status;
  uint16_t adc_value;

  // Wait until ADC data acquisition done
  while (!adc_complete) {}
  adc_complete = false;

  status = sl_si91x_adc_read_data_static(adc_channel_config,
                                         adc_config,
                                         &adc_value);

  if (adc_value & AUXADC_DATA_TWELFTH) {
    adc_value = (int16_t)(adc_value & (ADC_MASK_VALUE));
  } else {
    adc_value = adc_value | AUXADC_DATA_TWELFTH;
  }
  *data = adc_value;
  return status;
}

/*******************************************************************************
 * Delay function for 10us
 ******************************************************************************/
static void delay_10us(uint32_t idelay)
{
  for (uint32_t x = 0; x < DELAY_10US_COUNTER * idelay; x++) {
    __NOP();
  }
}

static void set_input(enum TOUCH_SCREEN_CHANNEL channel)
{
  switch (channel) {
    case TOUCH_SCREEN_CHANNEL_XM:
      si91x_gpio_pin_setup(
        TOUCHSCREEN_XM_PORT,
        TOUCHSCREEN_XM_PIN,
        SL_GPIO_MODE_0,
        GPIO_INPUT,
        0);
      break;
    case TOUCH_SCREEN_CHANNEL_XP:
      si91x_gpio_pin_setup(TOUCHSCREEN_XP_PORT,
                           TOUCHSCREEN_XP_PIN,
                           SL_GPIO_MODE_0,
                           GPIO_INPUT,
                           0);
      break;
    case TOUCH_SCREEN_CHANNEL_YM:
      si91x_gpio_pin_setup(TOUCHSCREEN_YM_PORT,
                           TOUCHSCREEN_YM_PIN,
                           SL_GPIO_MODE_0,
                           GPIO_INPUT,
                           0);
      break;
    case TOUCH_SCREEN_CHANNEL_YP:
      si91x_gpio_pin_setup(
        TOUCHSCREEN_YP_PORT,
        TOUCHSCREEN_YP_PIN,
        SL_GPIO_MODE_0,
        GPIO_INPUT,
        0);
      break;
  }
}

static void set_output(enum TOUCH_SCREEN_CHANNEL channel, uint8_t value)
{
  switch (channel) {
    case TOUCH_SCREEN_CHANNEL_XM:
      si91x_gpio_pin_setup(
        TOUCHSCREEN_XM_PORT,
        TOUCHSCREEN_XM_PIN,
        SL_GPIO_MODE_0,
        GPIO_OUTPUT,
        value);
      break;
    case TOUCH_SCREEN_CHANNEL_XP:
      si91x_gpio_pin_setup(TOUCHSCREEN_XP_PORT,
                           TOUCHSCREEN_XP_PIN,
                           SL_GPIO_MODE_0,
                           GPIO_OUTPUT,
                           value);
      break;
    case TOUCH_SCREEN_CHANNEL_YM:
      si91x_gpio_pin_setup(TOUCHSCREEN_YM_PORT,
                           TOUCHSCREEN_YM_PIN,
                           SL_GPIO_MODE_0,
                           GPIO_OUTPUT,
                           value);
      break;
    case TOUCH_SCREEN_CHANNEL_YP:
      si91x_gpio_pin_setup(
        TOUCHSCREEN_YP_PORT,
        TOUCHSCREEN_YP_PIN,
        SL_GPIO_MODE_0,
        GPIO_OUTPUT,
        value);
      break;
  }
}

static sl_status_t adc_start_read(enum TOUCH_SCREEN_CHANNEL channel)
{
  if (TOUCH_SCREEN_CHANNEL_XM == channel) {
    adc_start(TOUCHSCREEN_XM_POS_INPUT_CHNL_SEL);
  } else if (TOUCH_SCREEN_CHANNEL_YP == channel) {
    adc_start(TOUCHSCREEN_YP_POS_INPUT_CHNL_SEL);
  } else {
    return SL_STATUS_INVALID_PARAMETER;
  }
  return SL_STATUS_OK;
}

static uint16_t adc_read_u12(void)
{
  uint16_t adc_value = 0;

  if (SL_STATUS_OK == adc_read(&adc_value)) {
    return adc_value;
  }
  return 0;
}

static void adc_stop(void)
{
  sl_si91x_adc_stop(adc_config);
}

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 * Initialize Touch Screen feature.
 *****************************************************************************/
sl_status_t touch_screen_interface_init(struct touch_screen *ts)
{
  ts->aif = &touch_screen_analog_interface;
  ts->config = &touch_screen_config;
  adc_init();
  return SL_STATUS_OK;
}
