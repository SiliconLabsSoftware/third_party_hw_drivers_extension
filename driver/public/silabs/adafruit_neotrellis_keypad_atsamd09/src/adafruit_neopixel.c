/***************************************************************************//**
 * @file adafruit_neopixel.c
 * @brief adafruit_neopixel source file for Adafruit NeoTrellis 4x4 keypad.
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

#include <stdlib.h>
#include <string.h>
#include "sl_sleeptimer.h"
#include "adafruit_neopixel.h"
#include "adafruit_neotrellis_config.h"

static inline bool adafruit_neopixel_can_show(neopixel_t *pixel)
{
  return (sl_sleeptimer_get_tick_count() - pixel->endTime) >= 300L;
}

sl_status_t adafruit_neopixel_init(neopixel_t *pixel,
                                   uint16_t num_led,
                                   uint8_t pixel_pin,
                                   neoPixelType pixel_type,
                                   uint8_t i2c_addr,
                                   mikroe_i2c_handle_t i2c_inst)
{
  if ((NULL == pixel) || (NULL == i2c_inst)) {
    return SL_STATUS_NULL_POINTER;
  }

  sl_status_t sc = SL_STATUS_OK;
  i2c_master_config_t i2c_cfg;

  pixel->begun = false;
  pixel->brightness = 0;
  pixel->pixels = NULL;
  pixel->endTime = 0;
  pixel->adafruit_seesaw.i2c_instance.handle = i2c_inst;

  i2c_master_configure_default(&i2c_cfg);

  i2c_cfg.addr = i2c_addr;

#if (ADAFRUIT_NEO_TRELLIS_I2C_UC == 1)
  i2c_cfg.speed = ADAFRUIT_NEO_TRELLIS_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&pixel->adafruit_seesaw.i2c_instance,
                      &i2c_cfg) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  i2c_master_set_speed(&pixel->adafruit_seesaw.i2c_instance, i2c_cfg.speed);
  i2c_master_set_timeout(&pixel->adafruit_seesaw.i2c_instance, 0);

  sc |= adafruit_neopixel_update_type(pixel, pixel_type);
  sc |= adafruit_neopixel_update_length(pixel, num_led);
  sc |= adafruit_neopixel_set_pin(pixel, pixel_pin);
  return sc;
}

sl_status_t adafruit_neopixel_show(neopixel_t *pixel)
{
  sl_status_t sc;
  if (!pixel->pixels) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Data latch = 300+ microsecond pause in the output stream.  Rather than
  // put a delay at the end of the function, the ending time is noted and
  // the function will simply hold off (if needed) on issuing the
  // subsequent round of data until the latch time has elapsed.  This
  // allows the mainline code to start generating the next frame of data
  // rather than stalling for the latch.
  while (!adafruit_neopixel_can_show(pixel)) {}

  sc = adafruit_seesaw_i2c_register_write(&pixel->adafruit_seesaw,
                                          SEESAW_NEOPIXEL_BASE,
                                          SEESAW_NEOPIXEL_SHOW,
                                          NULL, 0);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  // Save EOD time for latch on next call
  pixel->endTime = sl_sleeptimer_get_tick_count();
  return SL_STATUS_OK;
}

sl_status_t adafruit_neopixel_set_pin(neopixel_t *pixel, uint8_t p)
{
  pixel->pin = p;
  return adafruit_seesaw_i2c_register_write(&pixel->adafruit_seesaw,
                                            SEESAW_NEOPIXEL_BASE,
                                            SEESAW_NEOPIXEL_PIN,
                                            &p, 1);
}

sl_status_t adafruit_neopixel_set_pixelColor_rgb(neopixel_t *pixel,
                                                 uint16_t n,
                                                 uint8_t r,
                                                 uint8_t g,
                                                 uint8_t b)
{
  if (n >= pixel->numLEDs) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (pixel->brightness) {   // See notes in setBrightness()
    r = (r * pixel->brightness) >> 8;
    g = (g * pixel->brightness) >> 8;
    b = (b * pixel->brightness) >> 8;
  }

  uint8_t *p;
  if (pixel->wOffset == pixel->rOffset) {   // Is an RGB-type strip
    p = &(pixel->pixels[n * 3]);       // 3 bytes per pixel
  } else {                    // Is a WRGB-type strip
    p = &(pixel->pixels[n * 4]);       // 4 bytes per pixel
    p[pixel->wOffset] = 0;           // But only R,G,B passed -- set W to 0
  }
  p[pixel->rOffset] = r;   // R,G,B always stored
  p[pixel->gOffset] = g;
  p[pixel->bOffset] = b;

  uint8_t len = (pixel->wOffset == pixel->rOffset ? 3 : 4);
  uint16_t offset = n * len;

  uint8_t writeBuf[6];
  writeBuf[0] = (offset >> 8);
  writeBuf[1] = offset;
  memcpy(&writeBuf[2], p, len);

  return adafruit_seesaw_i2c_register_write(&pixel->adafruit_seesaw,
                                            SEESAW_NEOPIXEL_BASE,
                                            SEESAW_NEOPIXEL_BUF,
                                            writeBuf, len + 2);
}

sl_status_t adafruit_neopixel_set_pixelColor_rgbw(neopixel_t *pixel,
                                                  uint16_t n,
                                                  uint8_t r,
                                                  uint8_t g,
                                                  uint8_t b,
                                                  uint8_t w)
{
  uint8_t *p;

  if (n >= pixel->numLEDs) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (pixel->brightness) {   // See notes in setBrightness()
    r = (r * pixel->brightness) >> 8;
    g = (g * pixel->brightness) >> 8;
    b = (b * pixel->brightness) >> 8;
    w = (w * pixel->brightness) >> 8;
  }

  if (pixel->wOffset == pixel->rOffset) {   // Is an RGB-type strip
    p = &(pixel->pixels[n * 3]);       // 3 bytes per pixel (ignore W)
  } else {                    // Is a WRGB-type strip
    p = &(pixel->pixels[n * 4]);       // 4 bytes per pixel
    p[pixel->wOffset] = w;           // Store W
  }
  p[pixel->rOffset] = r;   // Store R,G,B
  p[pixel->gOffset] = g;
  p[pixel->bOffset] = b;

  uint8_t len = (pixel->wOffset == pixel->rOffset ? 3 : 4);
  uint16_t offset = n * len;
  uint8_t writeBuf[6];

  writeBuf[0] = (offset >> 8);
  writeBuf[1] = offset;
  memcpy(&writeBuf[2], p, len);

  return adafruit_seesaw_i2c_register_write(&pixel->adafruit_seesaw,
                                            SEESAW_NEOPIXEL_BASE,
                                            SEESAW_NEOPIXEL_BUF,
                                            writeBuf, len + 2);
}

sl_status_t adafruit_neopixel_set_pixelColor(neopixel_t *pixel,
                                             uint16_t n,
                                             uint32_t c)
{
  if (n >= pixel->numLEDs) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  uint8_t *p, r = (uint8_t)(c >> 16), g = (uint8_t)(c >> 8), b = (uint8_t)c;
  if (pixel->brightness) {   // See notes in setBrightness()
    r = (r * pixel->brightness) >> 8;
    g = (g * pixel->brightness) >> 8;
    b = (b * pixel->brightness) >> 8;
  }
  if (pixel->wOffset == pixel->rOffset) {
    p = &(pixel->pixels[n * 3]);
  } else {
    p = &(pixel->pixels[n * 4]);
    uint8_t w = (uint8_t)(c >> 24);
    p[pixel->wOffset] = pixel->brightness ? ((w * pixel->brightness) >> 8) : w;
  }
  p[pixel->rOffset] = r;
  p[pixel->gOffset] = g;
  p[pixel->bOffset] = b;

  uint8_t len = (pixel->wOffset == pixel->rOffset ? 3 : 4);
  uint16_t offset = n * len;

  uint8_t writeBuf[6];
  writeBuf[0] = (offset >> 8);
  writeBuf[1] = offset;
  memcpy(&writeBuf[2], p, len);

  return adafruit_seesaw_i2c_register_write(&pixel->adafruit_seesaw,
                                            SEESAW_NEOPIXEL_BASE,
                                            SEESAW_NEOPIXEL_BUF,
                                            writeBuf, len + 2);
}

void adafruit_neopixel_set_brightness(neopixel_t *pixel, uint8_t b)
{
  pixel->brightness = b;
}

sl_status_t adafruit_neopixel_clear(neopixel_t *pixel)
{
  sl_status_t sc = SL_STATUS_OK;
  // Clear local pixel buffer
  memset(pixel->pixels, 0, pixel->numBytes);

  // Now clear the pixels on the seesaw
  uint8_t writeBuf[32];
  memset(writeBuf, 0, 32);
  for (uint8_t offset = 0; offset < pixel->numBytes; offset += 32 - 4) {
    writeBuf[0] = (offset >> 8);
    writeBuf[1] = offset;
    sc |= adafruit_seesaw_i2c_register_write(&pixel->adafruit_seesaw,
                                             SEESAW_NEOPIXEL_BASE,
                                             SEESAW_NEOPIXEL_BUF,
                                             writeBuf, 32);
  }
  return sc;
}

sl_status_t adafruit_neopixel_update_length(neopixel_t *pixel, uint16_t n)
{
  if (pixel->pixels) {
    free(pixel->pixels);   // Free existing data (if any)
  }
  // Allocate new data -- note: ALL PIXELS ARE CLEARED
  pixel->numBytes = n * ((pixel->wOffset == pixel->rOffset) ? 3 : 4);
  if ((pixel->pixels = (uint8_t *)malloc(pixel->numBytes))) {
    memset(pixel->pixels, 0, pixel->numBytes);
    pixel->numLEDs = n;
  } else {
    pixel->numLEDs = pixel->numBytes = 0;
  }

  uint8_t buf[] = { (uint8_t)(pixel->numBytes >> 8),
                    (uint8_t)(pixel->numBytes & 0xFF) };
  return adafruit_seesaw_i2c_register_write(&pixel->adafruit_seesaw,
                                            SEESAW_NEOPIXEL_BASE,
                                            SEESAW_NEOPIXEL_BUF_LENGTH,
                                            buf, 2);
}

sl_status_t adafruit_neopixel_update_type(neopixel_t *pixel, neoPixelType t)
{
  sl_status_t sc;

  bool oldThreeBytesPerPixel = (pixel->wOffset == pixel->rOffset); // false if
                                                                   //   RGBW

  pixel->wOffset = (t >> 6) & 0b11; // See notes in header file
  pixel->rOffset = (t >> 4) & 0b11; // regarding R/G/B/W offsets
  pixel->gOffset = (t >> 2) & 0b11;
  pixel->bOffset = t & 0b11;
  pixel->is800KHz = (t < 256); // 400 KHz flag is 1<<8

  sc = adafruit_seesaw_i2c_register_write(&pixel->adafruit_seesaw,
                                          SEESAW_NEOPIXEL_BASE,
                                          SEESAW_NEOPIXEL_SPEED,
                                          (uint8_t *)&(pixel->is800KHz), 1);

  if (sc != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  // If bytes-per-pixel has changed (and pixel data was previously
  // allocated), re-allocate to new size.  Will clear any data.
  if (pixel->pixels) {
    bool newThreeBytesPerPixel = (pixel->wOffset == pixel->rOffset);
    if (newThreeBytesPerPixel != oldThreeBytesPerPixel) {
      adafruit_neopixel_update_length(pixel, pixel->numLEDs);
    }
  }

  return SL_STATUS_OK;
}

uint32_t adafruit_neopixel_color_rgb(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint32_t adafruit_neopixel_color_rgbw(uint8_t r,
                                      uint8_t g,
                                      uint8_t b,
                                      uint8_t w)
{
  return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint32_t adafruit_neopixel_getPixelColor(neopixel_t *pixel, const uint16_t n)
{
  if (n >= pixel->numLEDs) {
    return 0;   // Out of bounds, return no color.
  }
  uint8_t *p;

  if (pixel->wOffset == pixel->rOffset) {   // Is RGB-type device
    p = &(pixel->pixels[n * 3]);
    if (pixel->brightness) {
      /*
       * Stored color was decimated by setBrightness().  Returned value
       * attempts to scale back to an approximation of the original 24-bit
       * value used when setting the pixel color, but there will always be
       * some error -- those bits are simply gone.  Issue is most
       * pronounced at low brightness levels.
       */
      return (((uint32_t)(p[pixel->rOffset] << 8) / pixel->brightness) << 16)
             | (((uint32_t)(p[pixel->gOffset] << 8) / pixel->brightness) << 8)
             | ((uint32_t)(p[pixel->bOffset] << 8) / pixel->brightness);
    } else {
      // No brightness adjustment has been made -- return 'raw' color
      return ((uint32_t)p[pixel->rOffset] << 16)
             | ((uint32_t)p[pixel->gOffset] << 8)
             | (uint32_t)p[pixel->bOffset];
    }
  } else {   // Is RGBW-type device
    p = &(pixel->pixels[n * 4]);
    if (pixel->brightness) {   // Return scaled color
      return (((uint32_t)(p[pixel->wOffset] << 8) / pixel->brightness) << 24)
             | (((uint32_t)(p[pixel->rOffset] << 8) / pixel->brightness) << 16)
             | (((uint32_t)(p[pixel->gOffset] << 8) / pixel->brightness) << 8)
             | ((uint32_t)(p[pixel->bOffset] << 8) / pixel->brightness);
    } else {   // Return raw color
      return ((uint32_t)p[pixel->wOffset] << 24)
             | ((uint32_t)p[pixel->rOffset] << 16)
             | ((uint32_t)p[pixel->gOffset] << 8)
             | (uint32_t)p[pixel->bOffset];
    }
  }
}
