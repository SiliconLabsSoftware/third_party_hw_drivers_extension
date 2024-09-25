/***************************************************************************//**
 * @file ring.c
 * @brief mikroSDK 2.0 ring buffer library source file.
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
#include <string.h>
#include "ring.h"
#include "app_assert.h"

void ring_buf8_init(ring_buf8_t *ring, uint8_t *buf, size_t capacity)
{
  memset(buf, 0x00, capacity);

  ring->buffer = buf;
  ring->capacity = capacity;
}

bool ring_buf8_push(ring_buf8_t *ring, uint8_t data_)
{
  if (ring->size == ring->capacity) {
    return false;
  }

  ring->buffer[ring->head] = data_;
  ring->head = (ring->head + 1) % ring->capacity;
  ring->size++;

  return true;
}

uint8_t ring_buf8_pop(ring_buf8_t *ring)
{
  uint8_t result;
  app_assert(ring->size > 0);

  result = ring->buffer[ring->tail];
  ring->tail = (ring->tail + 1) % ring->capacity;
  ring->size--;

  return result;
}

bool ring_buf8_is_empty(ring_buf8_t *ring)
{
  return ring->size == 0;
}

bool ring_buf8_is_full(ring_buf8_t *ring)
{
  return ring->size == ring->capacity;
}

size_t ring_buf8_size(ring_buf8_t *ring)
{
  return ring->size;
}

void ring_buf8_clear(ring_buf8_t *ring)
{
  ring->size = 0;
  ring->head = 0;
  ring->tail = 0;
}

// ------------------------------------------------------------------------- END
