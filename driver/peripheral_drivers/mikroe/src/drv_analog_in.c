/***************************************************************************//**
 * @file drv_analog_in.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - Analog IN
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

#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "em_cmu.h"
#include "drv_analog_in.h"

#if defined(_SILICON_LABS_32B_SERIES_1)
#include "em_adc.h"
#elif defined(_SILICON_LABS_32B_SERIES_2)
#include "em_iadc.h"
#endif

#if defined(_SILICON_LABS_32B_SERIES_1)
#define CLK_ADC_FREQ             16000000
#elif defined(_SILICON_LABS_32B_SERIES_2)
// Set CLK_ADC to 10 MHz
#define CLK_SRC_ADC_FREQ         20000000 // CLK_SRC_ADC
#define CLK_ADC_FREQ             10000000 // CLK_ADC - 10 MHz max in normal mode
#else
#error "Unsupported platform"
#endif

#ifndef MIKROE_CONFIG_ADC_AVDD
#define MIKROE_CONFIG_ADC_AVDD   3300
#endif

#if defined(_SILICON_LABS_32B_SERIES_2)
#define calc_adc_pos(port, pin) \
  ((((port) * 16) + pin) + (unsigned int)iadcNegInputPortAPin0)
#define adc_mux_even(ref, adc, bus, index) \
  GPIO->bus ## BUSALLOC |=                 \
    GPIO_ ## bus ## BUSALLOC_ ## bus ## EVEN ## index ## _ ## adc;
#endif

#if defined(_SILICON_LABS_32B_SERIES_1)
static ADC_PosSel_TypeDef get_adc_aportx(analog_in_t *obj);

#elif defined(_SILICON_LABS_32B_SERIES_2)
static IADC_PosInput_t get_adc_aportx(analog_in_t *obj);
static err_t allocate_analog_bus_even0(analog_in_t *obj);

#endif
static err_t hal_adc_init(analog_in_t *obj);
static void hal_adc_deinit(analog_in_t *obj);
uint16_t hal_adc_read(analog_in_t *obj);

static analog_in_t *_owner = NULL;

static err_t _acquire(analog_in_t *obj, bool obj_open_state)
{
  err_t status = ACQUIRE_SUCCESS;

  if ((obj_open_state == true) && (_owner == obj)) {
    return ACQUIRE_FAIL;
  }
  if (_owner != obj) {
    _owner = obj;
  }
  return status;
}

void analog_in_configure_default(analog_in_config_t *config)
{
  config->input_pin = 0xFFFFFFFF;
  config->resolution = ANALOG_IN_RESOLUTION_DEFAULT;
  config->vref_input = ANALOG_IN_VREF_EXTERNAL;
  config->vref_value = -1.0;
}

err_t analog_in_open(analog_in_t *obj, analog_in_config_t *config)
{
  analog_in_config_t *p_config = &obj->config;
  memcpy(p_config, config, sizeof(analog_in_config_t));

  if (_acquire(obj, true) == ACQUIRE_FAIL) {
    return ADC_ERROR;
  }

  return hal_adc_init(obj);
}

err_t analog_in_set_resolution(analog_in_t *obj,
                               analog_in_resolution_t resolution)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    obj->config.resolution = resolution;
    return ADC_SUCCESS;
  } else {
    return ADC_ERROR;
  }
}

err_t analog_in_set_vref_input(analog_in_t *obj, analog_in_vref_t vref)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    obj->config.vref_input = vref;
    hal_adc_deinit(obj);
    return hal_adc_init(obj);
  } else {
    return ADC_ERROR;
  }
}

err_t analog_in_set_vref_value(analog_in_t *obj, float vref_value)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    obj->config.vref_value = vref_value;
    hal_adc_deinit(obj);
    return hal_adc_init(obj);
  } else {
    return ADC_ERROR;
  }
}

err_t analog_in_read(analog_in_t *obj, uint16_t *readDatabuf)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    *readDatabuf = hal_adc_read(obj);
    switch (obj->config.resolution) {
      case ANALOG_IN_RESOLUTION_6_BIT:
        *readDatabuf >>= 6;
        break;
      case ANALOG_IN_RESOLUTION_8_BIT:
        *readDatabuf >>= 4;
        break;
      case ANALOG_IN_RESOLUTION_10_BIT:
        *readDatabuf >>= 2;
        break;
      case ANALOG_IN_RESOLUTION_12_BIT:
        break;
      default:
        return ADC_ERROR;
    }
    return ADC_SUCCESS;
  } else {
    return ADC_ERROR;
  }
}

err_t analog_in_read_voltage(analog_in_t *obj, float *readDatabuf)
{
  if (_acquire(obj, false) != ACQUIRE_FAIL) {
    *readDatabuf = (float)hal_adc_read(obj) * obj->config.vref_value / 4096;
    return ADC_SUCCESS;
  } else {
    return ADC_ERROR;
  }
}

void analog_in_close(analog_in_t *obj)
{
  hal_adc_deinit(obj);
  obj->handle = NULL;
  _owner = NULL;
}

#if defined(_SILICON_LABS_32B_SERIES_1)
static const ADC_PosSel_TypeDef aportx_possel_table_A[] = {
  adcPosSelAPORT3XCH8,
  adcPosSelAPORT4XCH9,
  adcPosSelAPORT3XCH10,
  adcPosSelAPORT4XCH11,
  adcPosSelAPORT3XCH12,
  adcPosSelAPORT4XCH13,
  adcPosSelAPORT3XCH14,
  adcPosSelAPORT4XCH15
};
static const ADC_PosSel_TypeDef aportx_possel_table_B[] = {
  adcPosSelAPORT3XCH22,
  adcPosSelAPORT4XCH23,
  adcPosSelAPORT3XCH24,
  adcPosSelAPORT4XCH25,
  adcPosSelAPORT3XCH26,
  adcPosSelAPORT4XCH27,
  adcPosSelAPORT3XCH28,
  adcPosSelAPORT4XCH29,
  adcPosSelAPORT3XCH30,
  adcPosSelAPORT4XCH31
};
static const ADC_PosSel_TypeDef aportx_possel_table_C[] = {
  adcPosSelAPORT1XCH0,
  adcPosSelAPORT2XCH1,
  adcPosSelAPORT1XCH2,
  adcPosSelAPORT2XCH3,
  adcPosSelAPORT1XCH4,
  adcPosSelAPORT2XCH5,
  adcPosSelAPORT1XCH6,
  adcPosSelAPORT2XCH7,
  adcPosSelAPORT1XCH8,
  adcPosSelAPORT2XCH9,
  adcPosSelAPORT1XCH10,
  adcPosSelAPORT2XCH11,
};
static const ADC_PosSel_TypeDef aportx_possel_table_D[] = {
  adcPosSelAPORT3XCH0,
  adcPosSelAPORT4XCH1,
  adcPosSelAPORT3XCH2,
  adcPosSelAPORT4XCH3,
  adcPosSelAPORT3XCH4,
  adcPosSelAPORT4XCH5,
  adcPosSelAPORT3XCH6,
  adcPosSelAPORT4XCH7,
};
static const ADC_PosSel_TypeDef aportx_possel_table_F[] = {
  adcPosSelAPORT1XCH16,
  adcPosSelAPORT2XCH17,
  adcPosSelAPORT1XCH18,
  adcPosSelAPORT2XCH19,
  adcPosSelAPORT1XCH20,
  adcPosSelAPORT2XCH21,
  adcPosSelAPORT1XCH22,
  adcPosSelAPORT2XCH23,
  adcPosSelAPORT1XCH24,
  adcPosSelAPORT2XCH25,
  adcPosSelAPORT1XCH26,
  adcPosSelAPORT2XCH27,
  adcPosSelAPORT1XCH28,
  adcPosSelAPORT2XCH29,
  adcPosSelAPORT1XCH30,
  adcPosSelAPORT2XCH31
};
static const ADC_PosSel_TypeDef aportx_possel_table_I[] = {
  adcPosSelAPORT0XCH0,
  adcPosSelAPORT0XCH1,
  adcPosSelAPORT0XCH2,
  adcPosSelAPORT0XCH3
};
static ADC_PosSel_TypeDef get_adc_aportx(analog_in_t *obj)
{
  GPIO_Port_TypeDef port_index;
  unsigned int pin_index;

  port_index = (GPIO_Port_TypeDef)hal_gpio_port_index(obj->config.input_pin);
  pin_index = hal_gpio_pin_index(obj->config.input_pin);

  switch (port_index) {
    case SL_GPIO_PORT_A:
      if (pin_index <= 7) {
        return aportx_possel_table_A[pin_index];
      } else {
        return adcPosSelVSS;
      }

    case SL_GPIO_PORT_B:
      if ((pin_index >= 6) && (pin_index <= 15)) {
        return aportx_possel_table_B[pin_index - 6];
      } else {
        return adcPosSelVSS;
      }

    case SL_GPIO_PORT_C:
      if (pin_index <= 11) {
        return aportx_possel_table_C[pin_index];
      } else {
        return adcPosSelVSS;
      }

    case SL_GPIO_PORT_D:
      if ((pin_index >= 8) && (pin_index <= 15)) {
        return aportx_possel_table_D[pin_index - 8];
      } else {
        return adcPosSelVSS;
      }

    case SL_GPIO_PORT_F:
      if (pin_index <= 15) {
        return aportx_possel_table_F[pin_index];
      } else {
        return adcPosSelVSS;
      }

    case SL_GPIO_PORT_I:
      if (pin_index <= 3) {
        return aportx_possel_table_I[pin_index];
      } else {
        return adcPosSelVSS;
      }

    default:
      return adcPosSelVSS;
  }
  return adcPosSelVSS;
}

#elif defined(_SILICON_LABS_32B_SERIES_2)
static IADC_PosInput_t get_adc_aportx(analog_in_t *obj)
{
  GPIO_Port_TypeDef port_index;
  unsigned int pin_index;

  port_index = (GPIO_Port_TypeDef)hal_gpio_port_index(obj->config.input_pin);
  pin_index = hal_gpio_pin_index(obj->config.input_pin);

  if (pin_index > 15) {
    return iadcPosInputGnd;
  }

  switch (port_index) {
    case SL_GPIO_PORT_A:
      return (IADC_PosInput_t)calc_adc_pos(0, pin_index);

    case SL_GPIO_PORT_B:
      return (IADC_PosInput_t)calc_adc_pos(1, pin_index);

    case SL_GPIO_PORT_C:
      return (IADC_PosInput_t)calc_adc_pos(2, pin_index);

    case SL_GPIO_PORT_D:
      return (IADC_PosInput_t)calc_adc_pos(3, pin_index);

    default:
      return iadcPosInputGnd;
  }
}

#endif

#if defined(_SILICON_LABS_32B_SERIES_1)
static err_t hal_adc_init(analog_in_t *obj)
{
  // Enable ADC0 clock
  CMU_ClockEnable(cmuClock_ADC0, true);

  // Declare init structs
  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
  ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

  // Modify init structs and initialize
  init.prescale = ADC_PrescaleCalc(CLK_ADC_FREQ, 0); // Init to max ADC clock
                                                     //   for Series 1
  init.timebase = ADC_TimebaseCalc(0);

  initSingle.diff = false;              // single ended
  initSingle.resolution = adcRes12Bit;  // 12-bit resolution
  initSingle.acqTime = adcAcqTime4;     // set acquisition time to meet minimum
                                        //   requirements

  // Select ADC input.
  initSingle.posSel = get_adc_aportx(obj);
  if (initSingle.posSel == adcPosSelVSS) {
    return ADC_ERROR;
  }

  uint32_t reference_voltage = (uint32_t)floor(obj->config.vref_value * 100)
                               * 10;
  if (obj->config.vref_input == ANALOG_IN_VREF_EXTERNAL) {
    switch (reference_voltage) {
      case MIKROE_CONFIG_ADC_AVDD:
        // Buffered VDD.
        initSingle.reference = adcRefVDD;
        break;

      default:
        // Single-ended external reference from pin 6.
        initSingle.reference = adcRefExtSingle;
        break;
    }
  } else if (obj->config.vref_input == ANALOG_IN_VREF_INTERNAL) {
    switch (reference_voltage) {
      case 2500:
        // internal 2.5V reference
        initSingle.reference = adcRef2V5;
        break;

      case 1250:
        // Internal 1.25 V reference.
        initSingle.reference = adcRef1V25;
        break;

      case MIKROE_CONFIG_ADC_AVDD:
        // Buffered VDD.
        initSingle.reference = adcRefVDD;
        break;

      default:
        return ADC_ERROR;
    }
  } else {
    return ADC_ERROR;
  }

  ADC_Init((ADC_TypeDef *)obj->handle, &init);
  ADC_InitSingle((ADC_TypeDef *)obj->handle, &initSingle);

  return ADC_SUCCESS;
}

static void hal_adc_deinit(analog_in_t *obj)
{
  ADC_Reset((ADC_TypeDef *)obj->handle);
}

uint16_t hal_adc_read(analog_in_t *obj)
{
  ADC_TypeDef *ptr = (ADC_TypeDef *)obj->handle;
  // Start ADC conversion
  ADC_Start(ptr, adcStartSingle);

  // Wait for conversion to be complete
  while (!(ptr->STATUS & _ADC_STATUS_SINGLEDV_MASK)) {}

  // Get ADC result
  return (uint16_t)ADC_DataSingleGet((ADC_TypeDef *)obj->handle);
}

#elif defined(_SILICON_LABS_32B_SERIES_2)
static err_t allocate_analog_bus_even0(analog_in_t *obj)
{
  switch ((GPIO_Port_TypeDef)hal_gpio_port_index(obj->config.input_pin)) {
    case SL_GPIO_PORT_A:
#if (IADC_COUNT == 1)
      if (obj->handle == IADC0) {
        adc_mux_even(obj->handle, ADC0, A, 0);
        return ADC_SUCCESS;
#elif (IADC_COUNT == 2)
  } else if (obj->handle == IADC1) {
    adc_mux_even(obj->handle, ADC1, A, 0);
    return ADC_SUCCESS;
#endif
      } else {
        return ADC_ERROR;
      }

    case SL_GPIO_PORT_B:
#if (IADC_COUNT == 1)
      if (obj->handle == IADC0) {
        adc_mux_even(obj->handle, ADC0, B, 0);
        return ADC_SUCCESS;
#elif (IADC_COUNT == 2)
  } else if (obj->handle == IADC1) {
    adc_mux_even(obj->handle, ADC1, B, 0);
    return ADC_SUCCESS;
#endif
      } else {
        return ADC_ERROR;
      }

    case SL_GPIO_PORT_C:
    case SL_GPIO_PORT_D:
#if (IADC_COUNT == 1)
      if (obj->handle == IADC0) {
        adc_mux_even(obj->handle, ADC0, CD, 0);
        return ADC_SUCCESS;
#elif (IADC_COUNT == 2)
  } else if (obj->handle == IADC1) {
    adc_mux_even(obj->handle, ADC1, CD, 0);
    return ADC_SUCCESS;
#endif
      } else {
        return ADC_ERROR;
      }

    default:
      return ADC_ERROR;
  }
}

static err_t hal_adc_init(analog_in_t *obj)
{
  // Declare initialization structures
  IADC_Init_t init = IADC_INIT_DEFAULT;
  IADC_AllConfigs_t initAllConfigs = IADC_ALLCONFIGS_DEFAULT;
  IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;

  // Single input structure
  IADC_SingleInput_t singleInput = IADC_SINGLEINPUT_DEFAULT;

  // Enable IADC register clock.
#if (IADC_COUNT == 1)
  if (obj->handle == IADC0) {
    CMU_ClockEnable(cmuClock_IADC0, true);
#elif (IADC_COUNT == 2)
} else if (obj->handle == IADC0) {
  CMU_ClockEnable(cmuClock_IADC0, true);
#endif
  } else {
    return ADC_ERROR;
  }
  // Enable GPIO register clock.
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Use the FSRC0 as the IADC clock so it can run in EM2
  CMU_ClockSelectSet(cmuClock_IADCCLK, cmuSelect_FSRCO);

  // Set the prescaler needed for the intended IADC clock frequency
  init.srcClkPrescale =
    IADC_calcSrcClkPrescale((IADC_TypeDef *)obj->handle, CLK_SRC_ADC_FREQ, 0);

  // Shutdown between conversions to reduce current
  init.warmup = iadcWarmupNormal;

  /**
   * Configuration 0 is used by both scan and single conversions by
   * default.  Use internal bandgap as the reference and specify the
   * reference voltage in mV.
   */
  uint32_t reference_voltage = (uint32_t)floor(obj->config.vref_value * 100)
                               * 10;
  if (obj->config.vref_input == ANALOG_IN_VREF_EXTERNAL) {
    switch (reference_voltage) {
      case MIKROE_CONFIG_ADC_AVDD:
        initAllConfigs.configs[0].reference = iadcCfgReferenceVddx;
        initAllConfigs.configs[0].vRef = MIKROE_CONFIG_ADC_AVDD;
        break;

      case ((MIKROE_CONFIG_ADC_AVDD * 8) / 100):
        initAllConfigs.configs[0].reference = iadcCfgReferenceVddX0P8Buf;
        initAllConfigs.configs[0].vRef = ((MIKROE_CONFIG_ADC_AVDD * 8) / 100);
        break;

      default:
        if (reference_voltage <= 1250) {
          // External reference (unbuffered) VREFP to VREFN. Up to 1.25V.
          initAllConfigs.configs[0].reference = iadcCfgReferenceExt1V25;
          initAllConfigs.configs[0].vRef = reference_voltage;
#if defined(_IADC_CFG_REFSEL_VREF2P5)
        } else if (reference_voltage <= 2500) {
          initAllConfigs.configs[0].reference = iadcCfgReferenceExt2V5;
          initAllConfigs.configs[0].vRef = reference_voltage;
#endif
#if defined(_IADC_CFG_REFSEL_VREFBUF)
        } else if (reference_voltage < (MIKROE_CONFIG_ADC_AVDD - 300)) {
          // pad_vrefp (buffered) to pad_vrefn (pad_refp < vddx - 0.3)
          initAllConfigs.configs[0].reference = iadcCfgReferenceBuf;
          initAllConfigs.configs[0].vRef = reference_voltage;
#endif
        } else {
          return ADC_ERROR;
        }
        break;
    }
  } else if (obj->config.vref_input == ANALOG_IN_VREF_INTERNAL) {
    switch (reference_voltage) {
      case MIKROE_CONFIG_ADC_AVDD:
        initAllConfigs.configs[0].reference = iadcCfgReferenceVddx;
        initAllConfigs.configs[0].vRef = MIKROE_CONFIG_ADC_AVDD;
        break;
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_1)
      case 1232:
        if ((chipRev.major == 2UL)
            && (chipRev.minor == 0UL)) {
          initAllConfigs.configs[0].reference = iadcCfgReferenceInt1V2;
          initAllConfigs.configs[0].vRef = 1232;
        } else {
          return ADC_ERROR;
        }
        break;

      case 1180:
        if ((chipRev.major == 2UL)
            && (chipRev.minor != 0UL)) {
          initAllConfigs.configs[0].reference = iadcCfgReferenceInt1V2;
          initAllConfigs.configs[0].vRef = 1180;
        } else {
          return ADC_ERROR;
        }
        break;

      case 1210:
        if (chipRev.major != 2UL) {
          initAllConfigs.configs[0].reference = iadcCfgReferenceInt1V2;
          initAllConfigs.configs[0].vRef = 1210;
        } else {
          return ADC_ERROR;
        }
        break;
#else
      case 1210:
        initAllConfigs.configs[0].reference = iadcCfgReferenceInt1V2;
        initAllConfigs.configs[0].vRef = 1210;
        break;
#endif

      default:
        return ADC_ERROR;
    }
  } else {
    return ADC_ERROR;
  }

  /**
   * Resolution is not configurable directly but is based on the
   * selected oversampling ratio (osrHighSpeed), which defaults to
   * 2x and generates 12-bit results.
   */
  if ((obj->config.resolution < ANALOG_IN_RESOLUTION_6_BIT)
      || (obj->config.resolution > ANALOG_IN_RESOLUTION_12_BIT)) {
    // Resolution is not supported
    return ADC_ERROR;
  }
  initAllConfigs.configs[0].osrHighSpeed = iadcCfgOsrHighSpeed2x;
  initAllConfigs.configs[0].analogGain = iadcCfgAnalogGain1x;

  /**
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
    IADC_calcAdcClkPrescale((IADC_TypeDef *)obj->handle,
                            CLK_ADC_FREQ,
                            0,
                            iadcCfgModeNormal,
                            init.srcClkPrescale);

  /**
   * Specify the input channel.  When negInput = iadcNegInputGnd, the
   * conversion is single-ended.
   */
  singleInput.posInput = get_adc_aportx(obj);
  if (singleInput.posInput == iadcPosInputGnd) {
    return ADC_ERROR;
  }
  singleInput.negInput = iadcNegInputGnd;

  // Allocate the analog bus for ADC inputs
  if (ADC_ERROR == allocate_analog_bus_even0(obj)) {
    return ADC_ERROR;
  }

  // Initialize IADC
  IADC_init((IADC_TypeDef *)obj->handle, &init, &initAllConfigs);

  // Initialize a single-channel conversion
  IADC_initSingle((IADC_TypeDef *)obj->handle, &initSingle, &singleInput);

  return ADC_SUCCESS;
}

static void hal_adc_deinit(analog_in_t *obj)
{
  IADC_reset((IADC_TypeDef *)obj->handle);
}

uint16_t hal_adc_read(analog_in_t *obj)
{
  IADC_command((IADC_TypeDef *)obj->handle, iadcCmdStartSingle);
  // while combined status bits 8 & 6 don't equal 1 and 0 respectively)
  while ((IADC_getStatus((IADC_TypeDef *)obj->handle)
          & (_IADC_STATUS_CONVERTING_MASK | _IADC_STATUS_SINGLEFIFODV_MASK))
         != IADC_STATUS_SINGLEFIFODV) {}
  return IADC_pullSingleFifoResult((IADC_TypeDef *)obj->handle).data;
}

#endif

// ------------------------------------------------------------------------- END
