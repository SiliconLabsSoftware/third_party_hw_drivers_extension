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
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_iadc.h"
#include "sl_udelay.h"
#include "touch_screen.h"
#include "touch_screen_config.h"

// -----------------------------------------------------------------------------
//                       Macros
// -----------------------------------------------------------------------------

// Set CLK_ADC to 10 MHz
#define CLK_SRC_ADC_FREQ         20000000 // CLK_SRC_ADC
#define CLK_ADC_FREQ             10000000 // CLK_ADC - 10 MHz max in normal mode

#define ADC_AVDD                 3300

#define calc_adc_pos(port, pin) \
  ((((port) * 16) + pin) + (unsigned int)iadcNegInputPortAPin0)
#define adc_mux_even(ref, adc, bus, index) \
  GPIO->bus ## BUSALLOC |=                 \
    GPIO_ ## bus ## BUSALLOC_ ## bus ## EVEN ## index ## _ ## adc;

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

static const struct touch_screen_analog_interface ts_analog_interface = {
  .delay_10us = delay_10us,
  .set_input = set_input,
  .set_output = set_output,
  .adc_start_read = adc_start_read,
  .adc_read_u12 = adc_read_u12,
  .adc_stop = adc_stop
};

// -----------------------------------------------------------------------------
//                       Local Functions
// -----------------------------------------------------------------------------

static IADC_PosInput_t get_adc_aportx(GPIO_Port_TypeDef port,
                                      unsigned int pin)
{
  if (pin > 15) {
    return iadcPosInputGnd;
  }

  switch (port) {
    case SL_GPIO_PORT_A:
      return (IADC_PosInput_t)calc_adc_pos(0, pin);

    case SL_GPIO_PORT_B:
      return (IADC_PosInput_t)calc_adc_pos(1, pin);

    case SL_GPIO_PORT_C:
      return (IADC_PosInput_t)calc_adc_pos(2, pin);

    case SL_GPIO_PORT_D:
      return (IADC_PosInput_t)calc_adc_pos(3, pin);

    default:
      return iadcPosInputGnd;
  }
}

static sl_status_t allocate_analog_bus_even0(GPIO_Port_TypeDef port)
{
  switch (port) {
    case SL_GPIO_PORT_A:
      adc_mux_even(obj->handle, ADC0, A, 0);
      return SL_STATUS_OK;

    case SL_GPIO_PORT_B:
      adc_mux_even(obj->handle, ADC0, B, 0);
      return SL_STATUS_OK;

    case SL_GPIO_PORT_C:
    case SL_GPIO_PORT_D:
      adc_mux_even(obj->handle, ADC0, CD, 0);
      return SL_STATUS_OK;

    default:
      return SL_STATUS_INVALID_PARAMETER;
  }
}

static sl_status_t adc_init(GPIO_Port_TypeDef port,
                            unsigned int pin)
{
  // Declare initialization structures
  IADC_Init_t init = IADC_INIT_DEFAULT;
  IADC_AllConfigs_t initAllConfigs = IADC_ALLCONFIGS_DEFAULT;
  IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;

  // Single input structure
  IADC_SingleInput_t singleInput = IADC_SINGLEINPUT_DEFAULT;

  // Enable IADC register clock.
  CMU_ClockEnable(cmuClock_IADC0, true);

  // Enable GPIO register clock.
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Use the FSRC0 as the IADC clock so it can run in EM2
  CMU_ClockSelectSet(cmuClock_IADCCLK, cmuSelect_FSRCO);

  // Set the prescaler needed for the intended IADC clock frequency
  init.srcClkPrescale =
    IADC_calcSrcClkPrescale(IADC0, CLK_SRC_ADC_FREQ, 0);

  // Shutdown between conversions to reduce current
  init.warmup = iadcWarmupNormal;

  /*
   * Configuration 0 is used by both scan and single conversions by
   * default.  Use internal bandgap as the reference and specify the
   * reference voltage in mV.
   */
  initAllConfigs.configs[0].reference = iadcCfgReferenceVddx;
  initAllConfigs.configs[0].vRef = ADC_AVDD;

  /*
   * Resolution is not configurable directly but is based on the
   * selected oversampling ratio (osrHighSpeed), which defaults to
   * 2x and generates 12-bit results.
   */
  initAllConfigs.configs[0].osrHighSpeed = iadcCfgOsrHighSpeed2x;
  initAllConfigs.configs[0].analogGain = iadcCfgAnalogGain1x;

  /*
   * CLK_SRC_ADC must be prescaled by some value greater than 1 to
   * derive the intended CLK_ADC frequency.
   *
   * Based on the default 2x oversampling rate (OSRHS)...
   *
   * conversion time = ((4 * OSRHS) + 2) / fCLK_ADC
   *
   * ...which results in a maximum sampling rate of 833 ksps with the
   * 2-clock input multiplexer switching time is included.
   */
  initAllConfigs.configs[0].adcClkPrescale =
    IADC_calcAdcClkPrescale(IADC0,
                            CLK_ADC_FREQ,
                            0,
                            iadcCfgModeNormal,
                            init.srcClkPrescale);

  /*
   * Specify the input channel.  When negInput = iadcNegInputGnd, the
   * conversion is single-ended.
   */
  singleInput.posInput = get_adc_aportx(port, pin);
  if (singleInput.posInput == iadcPosInputGnd) {
    return SL_STATUS_FAIL;
  }
  singleInput.negInput = iadcNegInputGnd;

  // Allocate the analog bus for ADC inputs
  if (SL_STATUS_OK != allocate_analog_bus_even0(port)) {
    return SL_STATUS_FAIL;
  }

  // Initialize IADC
  IADC_init(IADC0, &init, &initAllConfigs);

  // Initialize a single-channel conversion
  IADC_initSingle(IADC0, &initSingle, &singleInput);

  return SL_STATUS_OK;
}

static void adc_deinit(void)
{
  IADC_reset(IADC0);
}

static uint16_t adc_read(void)
{
  IADC_command(IADC0, iadcCmdStartSingle);
  // while combined status bits 8 & 6 don't equal 1 and 0 respectively)
  while ((IADC_getStatus(IADC0)
          & (_IADC_STATUS_CONVERTING_MASK | _IADC_STATUS_SINGLEFIFODV_MASK))
         != IADC_STATUS_SINGLEFIFODV) {}
  return IADC_pullSingleFifoResult(IADC0).data;
}

/*******************************************************************************
 * Delay function for 10us
 ******************************************************************************/
static void delay_10us(uint32_t idelay)
{
  sl_udelay_wait(idelay * 10);
}

static void set_input(enum TOUCH_SCREEN_CHANNEL channel)
{
  switch (channel) {
    case TOUCH_SCREEN_CHANNEL_XM:
      GPIO_PinModeSet(TOUCHSCREEN_XM_PORT,
                      TOUCHSCREEN_XM_PIN,
                      gpioModeInput,
                      0);
      break;
    case TOUCH_SCREEN_CHANNEL_XP:
      GPIO_PinModeSet(TOUCHSCREEN_XP_PORT,
                      TOUCHSCREEN_XP_PIN,
                      gpioModeInput,
                      0);
      break;
    case TOUCH_SCREEN_CHANNEL_YM:
      GPIO_PinModeSet(TOUCHSCREEN_YM_PORT,
                      TOUCHSCREEN_YM_PIN,
                      gpioModeInput,
                      0);
      break;
    case TOUCH_SCREEN_CHANNEL_YP:
      GPIO_PinModeSet(TOUCHSCREEN_YP_PORT,
                      TOUCHSCREEN_YP_PIN,
                      gpioModeInput,
                      0);
      break;
  }
}

static void set_output(enum TOUCH_SCREEN_CHANNEL channel, uint8_t value)
{
  switch (channel) {
    case TOUCH_SCREEN_CHANNEL_XM:
      GPIO_PinModeSet(TOUCHSCREEN_XM_PORT,
                      TOUCHSCREEN_XM_PIN,
                      gpioModePushPull,
                      value);
      break;
    case TOUCH_SCREEN_CHANNEL_XP:
      GPIO_PinModeSet(TOUCHSCREEN_XP_PORT,
                      TOUCHSCREEN_XP_PIN,
                      gpioModePushPull,
                      value);
      break;
    case TOUCH_SCREEN_CHANNEL_YM:
      GPIO_PinModeSet(TOUCHSCREEN_YM_PORT,
                      TOUCHSCREEN_YM_PIN,
                      gpioModePushPull,
                      value);
      break;
    case TOUCH_SCREEN_CHANNEL_YP:
      GPIO_PinModeSet(TOUCHSCREEN_YP_PORT,
                      TOUCHSCREEN_YP_PIN,
                      gpioModePushPull,
                      value);
      break;
  }
}

static sl_status_t adc_start_read(enum TOUCH_SCREEN_CHANNEL channel)
{
  if (TOUCH_SCREEN_CHANNEL_XM == channel) {
    adc_init(TOUCHSCREEN_XM_PORT,
             TOUCHSCREEN_XM_PIN);
  } else if (TOUCH_SCREEN_CHANNEL_YP == channel) {
    adc_init(TOUCHSCREEN_YP_PORT,
             TOUCHSCREEN_YP_PIN);
  } else {
    return SL_STATUS_INVALID_PARAMETER;
  }

  return SL_STATUS_OK;
}

static uint16_t adc_read_u12(void)
{
  return adc_read();
}

static void adc_stop(void)
{
  adc_deinit();
}

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/**************************************************************************//**
 * Initialize Touch Screen feature.
 *****************************************************************************/
sl_status_t touch_screen_interface_init(struct touch_screen *ts)
{
  ts->aif = &ts_analog_interface;
  ts->config = &touch_screen_config;
  return SL_STATUS_OK;
}
