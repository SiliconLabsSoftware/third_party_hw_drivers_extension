/***************************************************************************//**
 * @file hal_target.h
 * @brief mikroSDK 2.0 Click Peripheral Drivers
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

#ifndef _HAL_TARGET_H_
#define _HAL_TARGET_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SLI_SI917
#define UDELAY_MUL_FACTOR 0.55f
#else // SLI_SI917
#define UDELAY_MUL_FACTOR 1
#endif // SLI_SI917

#if !defined(LWIP_HDR_ERR_H)
typedef int8_t err_t;
#endif

typedef enum {
  ACQUIRE_SUCCESS = 0, ACQUIRE_INIT,

  ACQUIRE_FAIL = (-1)
} acquire_t;

extern void sl_udelay_wait(unsigned us);
extern void sl_sleeptimer_delay_millisecond(uint16_t time_ms);

// Delay functions in microseconds
#define Delay_6us()   sl_udelay_wait(6 * UDELAY_MUL_FACTOR)
#define Delay_10us()  sl_udelay_wait(10 * UDELAY_MUL_FACTOR)
#define Delay_22us()  sl_udelay_wait(22 * UDELAY_MUL_FACTOR)
#define Delay_50us()  sl_udelay_wait(50 * UDELAY_MUL_FACTOR)
#define Delay_80us()  sl_udelay_wait(80 * UDELAY_MUL_FACTOR)
#define Delay_100us() sl_udelay_wait(100 * UDELAY_MUL_FACTOR)
#define Delay_500us() sl_udelay_wait(500 * UDELAY_MUL_FACTOR)

// Delay functions in milliseconds
#define Delay_1ms()   sl_sleeptimer_delay_millisecond(1)
#define Delay_5ms()   sl_sleeptimer_delay_millisecond(5)
#define Delay_10ms()  sl_sleeptimer_delay_millisecond(10)
#define Delay_100ms() sl_sleeptimer_delay_millisecond(100)

// Delay functions in seconds
#define Delay_1sec()  sl_sleeptimer_delay_millisecond(1000)

#ifdef __cplusplus
}
#endif

#endif // _HAL_TARGET_H_
// ------------------------------------------------------------------------- END
