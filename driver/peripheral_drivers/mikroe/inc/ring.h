/***************************************************************************//**
 * @file ring.h
 * @brief mikroSDK 2.0 ring buffer
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

#ifndef _RING_H_
#define _RING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*!
 * @addtogroup platform Platform
 * @{
 */

/*!
 * @addtogroup ringbuf Ring buffer.
 * @brief Ring buffer library.
 *
 * Detailed description here.
 * @{
 */

typedef struct
{
  uint8_t *buffer;
  size_t capacity;
  volatile size_t size;
  volatile size_t head;
  volatile size_t tail;
} ring_buf8_t;

/**
 * @brief Initializes ring buffer with preallocated memory.
 *
 * @param ring The ring buffer instance.
 * @param buf Preallocated memory address.
 * @param capacity Preallocated memory size.
 *
 */
void ring_buf8_init(ring_buf8_t *ring, uint8_t *buf, size_t capacity);

/**
 * @brief Pushes data to the ring buffer.
 *
 * @param ring The ring buffer instance.
 * @param data_ Data to be pushed to the buffer.
 *
 * @return Returns true if data is pushed successfully, otherwise returns false.
 */
bool ring_buf8_push(ring_buf8_t *buf, uint8_t data_);

/**
 * @brief Pops data from the ring buffer. The caller needs to ensure that the
 * ring buffer is not empty.
 *
 * @param ring The ring buffer instance.
 *
 * @return Returns data.
 */
uint8_t ring_buf8_pop(ring_buf8_t *buf);

/**
 * @brief Returns true if the ring buffer is empty, otherwise returns false.
 */
bool ring_buf8_is_empty(ring_buf8_t *buf);

/**
 * @brief Returns true if the ring buffer is full, otherwise returns false.
 */
bool ring_buf8_is_full(ring_buf8_t *buf);

/**
 * @brief Returns number of bytes in the ring buffer.
 */
size_t ring_buf8_size(ring_buf8_t *buf);

/**
 * @brief Clears the ring buffer.
 */
void ring_buf8_clear(ring_buf8_t *buf);

/*! @} */ // ringbuf

/*! @} */ // platform

#ifdef __cplusplus
}
#endif

/*! @} */

#endif // _RING_H_
// ------------------------------------------------------------------------- END
