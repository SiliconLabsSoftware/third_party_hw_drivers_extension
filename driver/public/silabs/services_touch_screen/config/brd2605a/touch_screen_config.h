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
extern "C"
{
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Touch settings

// <o TOUCHSCREEN_HOR_RES> Touch Screen Horizontal Resolution
// <i> Default: 240
#define TOUCHSCREEN_HOR_RES              320

// <o TOUCHSCREEN_VER_RES> Touch Screen Vertical Resolution
// <i> Default: 320
#define TOUCHSCREEN_VER_RES              480

// <o TOUCHSCREEN_XPLATE_RES> X-plate resistance
// <i> Default: 200
// <d> 200
#define TOUCHSCREEN_XPLATE_RES           200

// <q TOUCHSCREEN_X_INV> Invert X-axis
// <i> Default: 0
// <d> 0
#define TOUCHSCREEN_X_INV                1

// <q TOUCHSCREEN_Y_INV> Invert Y-axis
// <i> Default: 0
// <d> 0
#define TOUCHSCREEN_Y_INV                1

// <q TOUCHSCREEN_XY_SWAP> XY Swap
// <i> Default: 0
// <d> 0
#define TOUCHSCREEN_XY_SWAP              0

// <o TOUCHSCREEN_X_MIN> Calib X-min
// <i> Default: 460
#define TOUCHSCREEN_X_MIN                678

// <o TOUCHSCREEN_X_MAX> Calib X-max
// <i> Default: 3620
#define TOUCHSCREEN_X_MAX                3566

// <o TOUCHSCREEN_Y_MIN> Calib Y-min
// <i> Default: 340
#define TOUCHSCREEN_Y_MIN                393

// <o TOUCHSCREEN_Y_MAX> Calib Y-max
// <i> Default: 3580
#define TOUCHSCREEN_Y_MAX                3521

// </h>

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>
// <adc_ch1 signal=P,N> SL_ADC_XM
// $[ADC_CH1_SL_ADC_XM]
#ifndef SL_ADC_XM_PERIPHERAL                    
#define SL_ADC_XM_PERIPHERAL                 ADC_CH1
#endif
#ifndef SL_ADC_XM_PERIPHERAL_NO                 
#define SL_ADC_XM_PERIPHERAL_NO              1
#endif

// ADC_CH1 P on ULP_GPIO_8/GPIO_72
#ifndef SL_ADC_XM_P_PORT                        
#define SL_ADC_XM_P_PORT                     ULP
#endif
#ifndef SL_ADC_XM_P_PIN                         
#define SL_ADC_XM_P_PIN                      8
#endif
#ifndef SL_ADC_XM_P_LOC                         
#define SL_ADC_XM_P_LOC                      4
#endif

// ADC_CH1 N on ULP_GPIO_5/GPIO_69
#ifndef SL_ADC_XM_N_PORT                        
#define SL_ADC_XM_N_PORT                     ULP
#endif
#ifndef SL_ADC_XM_N_PIN                         
#define SL_ADC_XM_N_PIN                      5
#endif
#ifndef SL_ADC_XM_N_LOC                         
#define SL_ADC_XM_N_LOC                      345
#endif
// [ADC_CH1_SL_ADC_XM]$

// <adc_ch2 signal=P,N> SL_ADC_YP
// $[ADC_CH2_SL_ADC_YP]
#ifndef SL_ADC_YP_PERIPHERAL                    
#define SL_ADC_YP_PERIPHERAL                 ADC_CH2
#endif
#ifndef SL_ADC_YP_PERIPHERAL_NO                 
#define SL_ADC_YP_PERIPHERAL_NO              2
#endif

// ADC_CH2 P on ULP_GPIO_1/GPIO_65
#ifndef SL_ADC_YP_P_PORT                        
#define SL_ADC_YP_P_PORT                     ULP
#endif
#ifndef SL_ADC_YP_P_PIN                         
#define SL_ADC_YP_P_PIN                      1
#endif
#ifndef SL_ADC_YP_P_LOC                         
#define SL_ADC_YP_P_LOC                      29
#endif

// ADC_CH2 N on GPIO_30
#ifndef SL_ADC_YP_N_PORT                        
#define SL_ADC_YP_N_PORT                     HP
#endif
#ifndef SL_ADC_YP_N_PIN                         
#define SL_ADC_YP_N_PIN                      30
#endif
#ifndef SL_ADC_YP_N_LOC                         
#define SL_ADC_YP_N_LOC                      360
#endif
// [ADC_CH2_SL_ADC_YP]$

// <gpio> TOUCHSCREEN_XP
// $[GPIO_TOUCHSCREEN_XP]
#ifndef TOUCHSCREEN_XP_PORT                     
#define TOUCHSCREEN_XP_PORT                  HP
#endif
#ifndef TOUCHSCREEN_XP_PIN                      
#define TOUCHSCREEN_XP_PIN                   7
#endif
// [GPIO_TOUCHSCREEN_XP]$

// <gpio> TOUCHSCREEN_YM
// $[GPIO_TOUCHSCREEN_YM]
#ifndef TOUCHSCREEN_YM_PORT                     
#define TOUCHSCREEN_YM_PORT                  HP
#endif
#ifndef TOUCHSCREEN_YM_PIN                      
#define TOUCHSCREEN_YM_PIN                   6
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

// Positive Input Channel 1 Selection
#ifdef SL_ADC_XM_P_PIN
#define TOUCHSCREEN_XM_POS_INPUT_CHNL_SEL \
  ((SL_ADC_XM_P_PIN == 0)    ? 0          \
   : (SL_ADC_XM_P_PIN == 2)  ? 1          \
   : (SL_ADC_XM_P_PIN == 4)  ? 2          \
   : (SL_ADC_XM_P_PIN == 6)  ? 3          \
   : (SL_ADC_XM_P_PIN == 8)  ? 4          \
   : (SL_ADC_XM_P_PIN == 10) ? 5          \
   : (SL_ADC_XM_P_PIN == 25) ? 6          \
   : (SL_ADC_XM_P_PIN == 27) ? 7          \
   : (SL_ADC_XM_P_PIN == 29) ? 8          \
   : (SL_ADC_XM_P_PIN == 1)  ? 10         \
   : (SL_ADC_XM_P_PIN == 3)  ? 11         \
   : (SL_ADC_XM_P_PIN == 5)  ? 12         \
   : (SL_ADC_XM_P_PIN == 11) ? 13         \
   : (SL_ADC_XM_P_PIN == 9)  ? 14         \
   : (SL_ADC_XM_P_PIN == 7)  ? 15         \
   : (SL_ADC_XM_P_PIN == 26) ? 16         \
   : (SL_ADC_XM_P_PIN == 28) ? 17         \
   : (SL_ADC_XM_P_PIN == 30) ? 18         \
                              : -1)
#else
#define TOUCHSCREEN_XM_POS_INPUT_CHNL_SEL 10
#endif
// Negative Input Channel Selection
#ifdef SL_ADC_XM_N_PIN
#define TOUCHSCREEN_XM_NEG_INPUT_CHNL_SEL \
  ((SL_ADC_XM_N_PIN == 1)    ? 0          \
   : (SL_ADC_XM_N_PIN == 3)  ? 1          \
   : (SL_ADC_XM_N_PIN == 5)  ? 2          \
   : (SL_ADC_XM_N_PIN == 11) ? 3          \
   : (SL_ADC_XM_N_PIN == 9)  ? 4          \
   : (SL_ADC_XM_N_PIN == 7)  ? 5          \
   : (SL_ADC_XM_N_PIN == 26) ? 6          \
   : (SL_ADC_XM_N_PIN == 28) ? 7          \
   : (SL_ADC_XM_N_PIN == 30) ? 8          \
                              : -1)
#else
#define TOUCHSCREEN_XM_NEG_INPUT_CHNL_SEL 7
#endif

// Positive Input Channel 2 Selection
#ifdef SL_ADC_XM_P_PIN
#define TOUCHSCREEN_YP_POS_INPUT_CHNL_SEL \
  ((SL_ADC_YP_P_PIN == 0)    ? 0          \
   : (SL_ADC_YP_P_PIN == 2)  ? 1          \
   : (SL_ADC_YP_P_PIN == 4)  ? 2          \
   : (SL_ADC_YP_P_PIN == 6)  ? 3          \
   : (SL_ADC_YP_P_PIN == 8)  ? 4          \
   : (SL_ADC_YP_P_PIN == 10) ? 5          \
   : (SL_ADC_YP_P_PIN == 25) ? 6          \
   : (SL_ADC_YP_P_PIN == 27) ? 7          \
   : (SL_ADC_YP_P_PIN == 29) ? 8          \
   : (SL_ADC_YP_P_PIN == 1)  ? 10         \
   : (SL_ADC_YP_P_PIN == 3)  ? 11         \
   : (SL_ADC_YP_P_PIN == 5)  ? 12         \
   : (SL_ADC_YP_P_PIN == 11) ? 13         \
   : (SL_ADC_YP_P_PIN == 9)  ? 14         \
   : (SL_ADC_YP_P_PIN == 7)  ? 15         \
   : (SL_ADC_YP_P_PIN == 26) ? 16         \
   : (SL_ADC_YP_P_PIN == 28) ? 17         \
   : (SL_ADC_YP_P_PIN == 30) ? 18         \
                              : -1)
#else
#define TOUCHSCREEN_YP_POS_INPUT_CHNL_SEL 10
#endif
// Negative Input Channel Selection
#ifdef SL_ADC_YP_N_PIN
#define TOUCHSCREEN_YP_NEG_INPUT_CHNL_SEL \
  ((SL_ADC_YP_N_PIN == 1)    ? 0           \
   : (SL_ADC_YP_N_PIN == 3)  ? 1           \
   : (SL_ADC_YP_N_PIN == 5)  ? 2           \
   : (SL_ADC_YP_N_PIN == 11) ? 3           \
   : (SL_ADC_YP_N_PIN == 9)  ? 4           \
   : (SL_ADC_YP_N_PIN == 7)  ? 5           \
   : (SL_ADC_YP_N_PIN == 26) ? 6           \
   : (SL_ADC_YP_N_PIN == 28) ? 7           \
   : (SL_ADC_YP_N_PIN == 30) ? 8           \
                              : -1)
#else
#define TOUCHSCREEN_YP_NEG_INPUT_CHNL_SEL 7
#endif

#define TOUCHSCREEN_XM_PORT                      SL_ADC_XM_P_PORT
#define TOUCHSCREEN_XM_PIN                       SL_ADC_XM_P_PIN

#define TOUCHSCREEN_YP_PORT                      SL_ADC_YP_P_PORT
#define TOUCHSCREEN_YP_PIN                       SL_ADC_YP_P_PIN

#ifdef __cplusplus
extern "C"
}
#endif

#endif // TOUCH_SCREEN_CONFIG_H_
