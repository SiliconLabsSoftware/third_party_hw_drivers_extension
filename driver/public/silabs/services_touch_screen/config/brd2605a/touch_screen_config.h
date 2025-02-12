/***************************************************************************//**
 * @file touch_screen_config.h
 * @brief Configuration file for touch screen driver.
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
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#ifndef TOUCH_SCREEN_CONFIG_H_
#define TOUCH_SCREEN_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h>Touch settings

// <o TOUCHSCREEN_HOR_RES> Touch Screen Horizontal Resolution
// <i> Default: 240
#define TOUCHSCREEN_HOR_RES              240

// <o TOUCHSCREEN_VER_RES> Touch Screen Vertical Resolution
// <i> Default: 320
#define TOUCHSCREEN_VER_RES              320

// <o TOUCHSCREEN_XPLATE_RES> X-plate resistance
// <i> Default: 200
// <d> 200
#define TOUCHSCREEN_XPLATE_RES              200

// <q TOUCHSCREEN_X_INV> Invert X-axis
// <i> Default: 0
// <d> 0
#define TOUCHSCREEN_X_INV                   1

// <q TOUCHSCREEN_Y_INV> Invert Y-axis
// <i> Default: 0
// <d> 0
#define TOUCHSCREEN_Y_INV                   1

// <q TOUCHSCREEN_XY_SWAP> XY Swap
// <i> Default: 0
// <d> 0
#define TOUCHSCREEN_XY_SWAP             0

// <o TOUCHSCREEN_X_MIN> Calib X-min
// <i> Default: 460
#define TOUCHSCREEN_X_MIN              428

// <o TOUCHSCREEN_X_MAX> Calib X-max
// <i> Default: 3620
#define TOUCHSCREEN_X_MAX              3628

// <o TOUCHSCREEN_Y_MIN> Calib Y-min
// <i> Default: 340
#define TOUCHSCREEN_Y_MIN              260

// <o TOUCHSCREEN_Y_MAX> Calib Y-max
// <i> Default: 3580
#define TOUCHSCREEN_Y_MAX              3776

// </h>

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>
// <adc signal=P0,N0> SL_ADC_XM
// $[ADC_SL_ADC_XM]
#ifndef SL_ADC_XM_PERIPHERAL
#define SL_ADC_XM_PERIPHERAL                     ADC
#endif

// ADC P0 on ULP_GPIO_8/GPIO_72
#ifndef SL_ADC_XM_P0_PORT
#define SL_ADC_XM_P0_PORT                        ULP
#endif
#ifndef SL_ADC_XM_P0_PIN
#define SL_ADC_XM_P0_PIN                         8
#endif
#ifndef SL_ADC_XM_P0_LOC
#define SL_ADC_XM_P0_LOC                         4
#endif

// ADC N0 on GPIO_28
#ifndef SL_ADC_XM_N0_PORT
#define SL_ADC_XM_N0_PORT                        ULP
#endif
#ifndef SL_ADC_XM_N0_PIN
#define SL_ADC_XM_N0_PIN                         28
#endif
#ifndef SL_ADC_XM_N0_LOC
#define SL_ADC_XM_N0_LOC                         7
#endif
// [ADC_SL_ADC_XM]$

// <adc signal=P1,N1> SL_ADC_YP
// $[ADC_SL_ADC_YP]
#ifndef SL_ADC_YP_PERIPHERAL
#define SL_ADC_YP_PERIPHERAL                     ADC
#endif

// ADC P1 on ULP_GPIO_1/GPIO_65
#ifndef SL_ADC_YP_P1_PORT
#define SL_ADC_YP_P1_PORT                        ULP
#endif
#ifndef SL_ADC_YP_P1_PIN
#define SL_ADC_YP_P1_PIN                         1
#endif
#ifndef SL_ADC_YP_P1_LOC
#define SL_ADC_YP_P1_LOC                         10
#endif

// ADC N1 on GPIO_30
#ifndef SL_ADC_YP_N1_PORT
#define SL_ADC_YP_N1_PORT                        ULP
#endif
#ifndef SL_ADC_YP_N1_PIN
#define SL_ADC_YP_N1_PIN                         30
#endif
#ifndef SL_ADC_YP_N1_LOC
#define SL_ADC_YP_N1_LOC                         8
#endif
// [ADC_SL_ADC_YP]$

// <gpio> TOUCHSCREEN_XP
// $[GPIO_TOUCHSCREEN_XP]
#ifndef TOUCHSCREEN_XP_PORT
#define TOUCHSCREEN_XP_PORT                      HP
#endif
#ifndef TOUCHSCREEN_XP_PIN
#define TOUCHSCREEN_XP_PIN                       7
#endif
// [GPIO_TOUCHSCREEN_XP]$

// <gpio> TOUCHSCREEN_YM
// $[GPIO_TOUCHSCREEN_YM]
#ifndef TOUCHSCREEN_YM_PORT
#define TOUCHSCREEN_YM_PORT                      HP
#endif
#ifndef TOUCHSCREEN_YM_PIN
#define TOUCHSCREEN_YM_PIN                       6
#endif
// [GPIO_TOUCHSCREEN_YM]$

// <<< sl:end pin_tool >>>

#ifndef HP
#define HP                                   0
#endif

#ifndef ULP
#define ULP                                  4
#endif

#ifndef UULP_VBAT
#define UULP_VBAT                            5
#endif

#define N0_START_LOCATION_PINTOOL 343
#define TOUCHSCREEN_XM_PORT                      SL_ADC_XM_P0_PORT
#define TOUCHSCREEN_XM_PIN                       SL_ADC_XM_P0_PIN
#define TOUCHSCREEN_XM_POS_INPUT_CHNL_SEL        SL_ADC_XM_P0_LOC
#define TOUCHSCREEN_XM_NEG_INPUT_CHNL_SEL        SL_ADC_XM_N0_LOC

#define TOUCHSCREEN_YP_PORT                      SL_ADC_YP_P1_PORT
#define TOUCHSCREEN_YP_PIN                       SL_ADC_YP_P1_PIN
#define TOUCHSCREEN_YP_POS_INPUT_CHNL_SEL        SL_ADC_YP_P1_LOC
#define TOUCHSCREEN_YP_NEG_INPUT_CHNL_SEL        SL_ADC_YP_N1_LOC

#ifdef __cplusplus
extern "C"
}
#endif

#endif /* TOUCH_SCREEN_CONFIG_H_ */
