/***************************************************************************//**
 * @file sparkfun_type5.h
 * @brief Dosimeter header file.
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

#ifndef SPARKFUN_TYPE5_H_
#define SPARKFUN_TYPE5_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief
 *   Sparkfun Type5 callback function;
 *
 * @details
 *   This callback function is executed whenever noise or radiation is detected.
 *
 ******************************************************************************/
typedef void (*sparkfun_type5_callback_t)(void);

// -----------------------------------------------------------------------------
//                       Public Function
// -----------------------------------------------------------------------------

/***************************************************************************//**
 * @brief
 *  Initialize the Sparkfun dosimeter type5 sensor.
 *
 * @details
 *  This function initializes the dosimeter type5 sensor by initializing GPIO
 *  and GPIO interrupt for NS_PIN and SIG_PIN.
 *
 ******************************************************************************/
void sparkfun_type5_init(void);

/***************************************************************************//**
 * @brief
 *  Register callback function for noise detection.
 *
 * @details
 *  This function registers the callback function for dosimeter type5 sensor.
 *  This function will be executed when there is noise detected by this sensor.
 *
 ******************************************************************************/
void sparkfun_type5_register_noise_callback(sparkfun_type5_callback_t callback);

/***************************************************************************//**
 * @brief
 *  Register callback function for radiation detection.
 *
 * @details
 *  This function registers the callback function for dosimeter type5 sensor.
 *  This function will be executed when there is radiation detected
 *  by this sensor.
 *
 ******************************************************************************/
void sparkfun_type5_register_radiation_callback(
  sparkfun_type5_callback_t callback);

/***************************************************************************//**
 * @brief
 *  Process data.
 *
 * @details
 *  This function processes the radiation count data and user can register a
 *  callback function to handle the radiation count data.
 *
 * @return
 *  The number of radiation count.
 ******************************************************************************/
void sparkfun_type5_process(void);

/***************************************************************************//**
 * @brief
 *  Get radiation count during integration time.
 *
 * @details
 *  This function gets the number of radiation counts during the integration
 *  time.
 *
 * @return
 *  The number of radiation count.
 ******************************************************************************/
uint32_t sparkfun_type5_get_radiation_count(void);

/***************************************************************************//**
 * @brief
 *  Get integration time.
 *
 * @details
 *  This function gets the integration time.
 *
 * @return
 *  The integration time.
 ******************************************************************************/
uint32_t sparkfun_type5_get_integration_time(void);

/***************************************************************************//**
 * @brief
 *  Get the number of radiation count by minute.
 *
 * @details
 *  This function gets the number of radiation count by minute.
 *
 * @return
 *  The number of radiation count by minute.
 ******************************************************************************/
double sparkfun_type5_get_cpm(void);

/***************************************************************************//**
 * @brief
 *  Get the radiation dose.
 *
 * @details
 *  This function gets the radiation dose, exprimed in Sievert (uSv/h).
 *
 * @return
 *  The the radiation dose.
 ******************************************************************************/
double sparkfun_type5_get_usvh(void);

/***************************************************************************//**
 * @brief
 *  Get the error of the measurement.
 *
 * @details
 *  This function gets the error of the measurement.The range of precision of
 *  the measurement is: [ uSvh-uSvhError, uSvh+uSvhError ].
 *
 * @return
 *  The error of the measurement.
 ******************************************************************************/
double sparkfun_type5_get_usvh_error(void);

#ifdef __cplusplus
}
#endif
#endif // SPARKFUN_TYPE5_H_
