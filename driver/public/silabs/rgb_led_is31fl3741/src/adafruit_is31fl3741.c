/***************************************************************************//**
 * @file adafruit_is31fl3741.c
 * @brief Adafruit RGB LED source file.
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

// -----------------------------------------------------------------------------
//                               Includes
// -----------------------------------------------------------------------------

#include <math.h>
#include <string.h>
#include "adafruit_is31fl3741.h"

// -----------------------------------------------------------------------------
//                               Macros
// -----------------------------------------------------------------------------

/* Expand 16-bit RGB565 packed color to RGB888 in 3 uint8_t variables*/
#define IS31FL3741_EXPAND(_COLOR_, _R_, _G_, _B_)                  \
  uint8_t _R_ = ((_COLOR_ >> 8) & 0xF8) | (_COLOR_ >> 13);         \
  uint8_t _G_ = ((_COLOR_ >> 3) & 0xFC) | ((_COLOR_ >> 9) & 0x03); \
  uint8_t _B_ = ((_COLOR_ << 3) & 0xF8) | ((_COLOR_ >> 2) & 0x07); \

// -----------------------------------------------------------------------------
//                       Local Variables
// -----------------------------------------------------------------------------

static i2c_master_t rgb_led;

static uint8_t device_list[] = {
#if DISPLAY_1_I2C_ADDRESS != 0
  DISPLAY_1_I2C_ADDRESS,
#endif
#if DISPLAY_2_I2C_ADDRESS != 0
  DISPLAY_2_I2C_ADDRESS,
#endif
#if DISPLAY_3_I2C_ADDRESS != 0
  DISPLAY_3_I2C_ADDRESS,
#endif
#if DISPLAY_4_I2C_ADDRESS != 0
  DISPLAY_4_I2C_ADDRESS,
#endif
};

#if (IS31FL3741_DISPLAY_LAYOUT == 0)
static uint8_t ledbuf[13 * 9 * 3 + 1];

#elif ((IS31FL3741_DISPLAY_LAYOUT == 1) || (IS31FL3741_DISPLAY_LAYOUT == 4))
static uint8_t ledbuf1[13 * 9 * 3 + 1];
static uint8_t ledbuf2[13 * 9 * 3 + 1];

#elif (IS31FL3741_DISPLAY_LAYOUT == 2)
static uint8_t ledbuf1[13 * 9 * 3 + 1];
static uint8_t ledbuf2[13 * 9 * 3 + 1];
static uint8_t ledbuf3[13 * 9 * 3 + 1];

#elif (IS31FL3741_DISPLAY_LAYOUT == 3) || (IS31FL3741_DISPLAY_LAYOUT == 5)
static uint8_t ledbuf1[13 * 9 * 3 + 1];
static uint8_t ledbuf2[13 * 9 * 3 + 1];
static uint8_t ledbuf3[13 * 9 * 3 + 1];
static uint8_t ledbuf4[13 * 9 * 3 + 1];

#endif

static bool initialized = false;
static uint8_t red_offset;
static uint8_t green_offset;
static uint8_t blue_offset;

// -----------------------------------------------------------------------------
//                       Local Functions
// -----------------------------------------------------------------------------

/**************************************************************************//**
 *  Unlock command register before writing data to RGB LED IS31FL3741.
 *****************************************************************************/
static sl_status_t adafruit_is31fl3741_unlock(void)
{
  uint8_t send_data = 0xC5;

  return adafruit_is31fl3741_i2c_write_reg(
    ADAFRUIT_IS31FL3741_COMMAND_REGISTER_LOCK,
    &send_data,
    1);
}

/**************************************************************************//**
 *  Select page for further control of the RGB LED IS31FL3741.
 *****************************************************************************/
static sl_status_t adafruit_is31fl3741_select_page(uint8_t page)
{
  sl_status_t ret = SL_STATUS_OK;

  if (page > 4) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  ret |= adafruit_is31fl3741_unlock();
  ret |= adafruit_is31fl3741_i2c_write_reg(ADAFRUIT_IS31FL3741_COMMAND_REGISTER,
                                           &page,
                                           1);
  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Use to set either the PWM or the scaling for a single led.
 *****************************************************************************/
static sl_status_t adafruit_is31fl3741_set_led_value(uint8_t first_page,
                                                     uint16_t led_num,
                                                     uint8_t value)
{
  sl_status_t ret = SL_STATUS_OK;
  uint8_t cmd[2];

  if (led_num >= 351) {
    return SL_STATUS_INVALID_RANGE;
  }

  cmd[1] = value;
  if (led_num < 180) {
    cmd[0] = (uint8_t)led_num;
    ret |= adafruit_is31fl3741_select_page(first_page);
  } else {
    cmd[0] = (uint8_t)(led_num - 180);
    ret |= adafruit_is31fl3741_select_page(first_page + 1);
  }
  ret |= adafruit_is31fl3741_i2c_write(cmd, 2);

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *   Use to set either the PWM or the scaling for all the leds.
 *****************************************************************************/
static sl_status_t adafruit_is31fl3741_fill_two_pages(uint8_t first_page,
                                                      uint8_t value)
{
  sl_status_t ret = SL_STATUS_OK;
  uint8_t buf[32];
  memset(buf, value, sizeof buf);

  uint8_t page_bytes = 180;
  for (uint8_t page = 0; page < 2; page++) {
    ret |= adafruit_is31fl3741_select_page(first_page + page);
    uint8_t addr = 0;
    while (page_bytes) {
      uint8_t bytesThisPass = (uint8_t)fmin((int)page_bytes, 31);
      buf[0] = addr;
      ret |= adafruit_is31fl3741_i2c_write(buf, bytesThisPass + 1);
      page_bytes -= bytesThisPass;
      addr += bytesThisPass;
    }
    page_bytes = 171;
  }

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *   Send data from buffer to RGB LED IS31FL3741 module.
 *****************************************************************************/
static sl_status_t adafruit_is31fl3741_write_data(uint8_t *offset)
{
  sl_status_t ret = SL_STATUS_OK;

  if (offset == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  uint8_t *ptr = offset;
  uint16_t chunk = 351 - 1;

  uint8_t page_bytes = 180;
  for (uint8_t page = 0; page < 2; page++) {
    adafruit_is31fl3741_select_page(page);
    uint8_t addr = 0;
    while (page_bytes) {
      uint8_t bytesThisPass = (uint8_t)fmin(page_bytes, chunk);
      uint8_t save = *ptr;
      *ptr = addr;
      ret |= adafruit_is31fl3741_i2c_write(ptr, bytesThisPass + 1);
      *ptr = save;
      page_bytes -= bytesThisPass;
      ptr += bytesThisPass;
      addr += bytesThisPass;
    }
    page_bytes = 171;
  }

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

// -----------------------------------------------------------------------------
//                       Public Functions
// -----------------------------------------------------------------------------

/**************************************************************************//**
 *  Initialize the RGB LED IS31FL3741.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_init(mikroe_i2c_handle_t i2cspm)
{
  i2c_master_config_t rgb_led_config;
  sl_status_t ret = SL_STATUS_OK;

  if (i2cspm == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  if (initialized) {
    return SL_STATUS_INITIALIZATION;
  }

  // Configure default i2csmp instance
  rgb_led.handle = i2cspm;

  i2c_master_configure_default(&rgb_led_config);
  rgb_led_config.addr = IS31FL3741_DEFAULT_I2C_ADDR;

#if (ADAFRUIT_IS31FL3741_I2C_UC == 1)
  rgb_led_config.speed = ADAFRUIT_IS31FL3741_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&rgb_led, &rgb_led_config) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  i2c_master_set_speed(&rgb_led, rgb_led_config.speed);
  i2c_master_set_timeout(&rgb_led, 0);

  adafruit_is31fl3741_i2c_set_instance(&rgb_led);
  ret |= adafruit_is31fl3741_reset();
  ret |= adafruit_is31fl3741_set_global_current(0x05);
  ret |= adafruit_is31fl3741_set_global_led_scaling(0x10);

  initialized = true;

  IS3741_order order = IS3741_BGR;
  red_offset = (order >> 4) & 3;
  green_offset = (order >> 2) & 3;
  blue_offset = order & 3;

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Performs the software reset then update all the register to POR values.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_reset(void)
{
  sl_status_t ret = SL_STATUS_OK;
  uint8_t send_data;

  for (uint8_t i = 0; i < sizeof(device_list); i++) {
    adafruit_is31fl3741_i2c_select_device(device_list[i]);
    ret |= adafruit_is31fl3741_select_page(4);
    send_data = 0xAE;
    ret |= adafruit_is31fl3741_i2c_write_reg(ADAFRUIT_IS31FL3741_FUNCREG_RESET,
                                             &send_data,
                                             1);
  }

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Enable/disable output via the shutdown register bit.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_enable(bool en)
{
  sl_status_t ret = SL_STATUS_OK;
  uint8_t data;

  for (uint8_t i = 0; i < sizeof(device_list); i++) {
    adafruit_is31fl3741_i2c_select_device(device_list[i]);
    ret |= adafruit_is31fl3741_select_page(4);
    ret |= adafruit_is31fl3741_i2c_read_reg(ADAFRUIT_IS31FL3741_FUNCREG_CONFIG,
                                            &data,
                                            1);
    if (en) {
      data |= 0x01;
    } else {
      data &= 0xFE;
    }
    ret |= adafruit_is31fl3741_i2c_write_reg(ADAFRUIT_IS31FL3741_FUNCREG_CONFIG,
                                             &data,
                                             1);
  }

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Sets global current-mirror from 0 (off) to 255 (brightest).
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_set_global_current(uint8_t current)
{
  sl_status_t ret = SL_STATUS_OK;

  for (uint8_t i = 0; i < sizeof(device_list); i++) {
    adafruit_is31fl3741_i2c_select_device(device_list[i]);
    ret |= adafruit_is31fl3741_select_page(4);
    ret |= adafruit_is31fl3741_i2c_write_reg(
      ADAFRUIT_IS31FL3741_FUNCREG_GCURRENT,
      &current,
      1);
  }

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  } else {
    return SL_STATUS_OK;
  }
}

/**************************************************************************//**
 *  Gets the global current-mirror register setting.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_get_global_current(uint8_t *current)
{
  if (current == NULL) {
    return SL_STATUS_NULL_POINTER;
  }
  return adafruit_is31fl3741_i2c_read_reg(ADAFRUIT_IS31FL3741_FUNCREG_GCURRENT,
                                          current,
                                          1);
}

/**************************************************************************//**
 *  Sets the scaling level for a single LED.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_set_led_scaling(uint16_t led_num, uint8_t scale)
{
  sl_status_t ret = SL_STATUS_OK;

  for (uint8_t i = 0; i < sizeof(device_list); i++) {
    ret |= adafruit_is31fl3741_set_led_value(2, led_num, scale);
  }

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  } else {
    return SL_STATUS_OK;
  }
}

/**************************************************************************//**
 *  Sets the PWM level for a single LED.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_set_led_pwm(uint16_t led_num, uint8_t pwm)
{
  sl_status_t ret = SL_STATUS_OK;

  for (uint8_t i = 0; i < sizeof(device_list); i++) {
    ret |= adafruit_is31fl3741_set_led_value(0, led_num, pwm);
  }

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  } else {
    return SL_STATUS_OK;
  }
}

/**************************************************************************//**
 *  Sets the scaling level for all LEDs.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_set_global_led_scaling(uint8_t scaling)
{
  sl_status_t ret = SL_STATUS_OK;

  for (uint8_t i = 0; i < sizeof(device_list); i++) {
    adafruit_is31fl3741_i2c_select_device(device_list[i]);
    adafruit_is31fl3741_fill_two_pages(2, scaling);
  }

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  } else {
    return SL_STATUS_OK;
  }
}

/**************************************************************************//**
 *  Sets the PWM level for all LEDs.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_set_global_led_pwm(uint8_t pwm)
{
  sl_status_t ret = SL_STATUS_OK;

  for (uint8_t i = 0; i < sizeof(device_list); i++) {
    adafruit_is31fl3741_i2c_select_device(device_list[i]);
    ret |= adafruit_is31fl3741_fill_two_pages(0, pwm);
  }

  if (ret != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  } else {
    return SL_STATUS_OK;
  }
}

/**************************************************************************//**
 *  Draws a single pixel on the RGB LED IS31FL3741 with specific color.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  uint8_t *ptr;
  uint16_t offset = 0;
  static const uint8_t rowmap[] = { 8, 5, 4, 3, 2, 1, 0, 7, 6 };
  static const uint8_t remap[] = { 2, 0, 1 };

  // Expand RGB color565 to RGB color888
  IS31FL3741_EXPAND(color, r, g, b)

#if (IS31FL3741_DISPLAY_LAYOUT == 0) // Layout 1x1
  if ((x >= 0) && (y >= 0) && (x < 13) && (y < 9)) {
    y = rowmap[y];
    offset = (x + ((x < 10) ? (y * 10) : (80 + y * 3))) * 3;
  }
  ptr = &ledbuf[1 + offset];

#elif (IS31FL3741_DISPLAY_LAYOUT == 1) // Layout 1x2
  if (x >= 13) {
    x -= 13;
    ptr = ledbuf2;
  } else {
    ptr = ledbuf1;
  }

  if ((x >= 0) && (y >= 0) && (x < 13) && (y < 9)) {
    y = rowmap[y];
    offset = (x + ((x < 10) ? (y * 10) : (80 + y * 3))) * 3;
  }
  ptr += 1 + offset;

#elif (IS31FL3741_DISPLAY_LAYOUT == 2) // Layout 1x3
  if (x >= 26) {
    x -= 26;
    ptr = ledbuf3;
  } else if ((x >= 13) && (x < 26)) {
    x -= 13;
    ptr = ledbuf2;
  } else {
    ptr = ledbuf1;
  }

  if ((x >= 0) && (y >= 0) && (x < 13) && (y < 9)) {
    y = rowmap[y];
    offset = (x + ((x < 10) ? (y * 10) : (80 + y * 3))) * 3;
  }
  ptr += 1 + offset;

#elif (IS31FL3741_DISPLAY_LAYOUT == 3) // Layout 1x4
  if (x >= 39) {
    x -= 39;
    ptr = ledbuf4;
  } else if ((x >= 26) && (x < 39)) {
    x -= 26;
    ptr = ledbuf3;
  } else if ((x >= 13) && (x < 26)) {
    x -= 13;
    ptr = ledbuf2;
  } else {
    ptr = ledbuf1;
  }

  if ((x >= 0) && (y >= 0) && (x < 13) && (y < 9)) {
    y = rowmap[y];
    offset = (x + ((x < 10) ? (y * 10) : (80 + y * 3))) * 3;
  }
  ptr += 1 + offset;

#elif (IS31FL3741_DISPLAY_LAYOUT == 4) // Layout 2x1
  if (y >= 9) {
    y -= 9;
    ptr = ledbuf2;
  } else {
    ptr = ledbuf1;
  }

  if ((x >= 0) && (y >= 0) && (x < 13) && (y < 9)) {
    y = rowmap[y];
    offset = (x + ((x < 10) ? (y * 10) : (80 + y * 3))) * 3;
  }
  ptr += 1 + offset;

#elif (IS31FL3741_DISPLAY_LAYOUT == 5) // Layout 2x2
  if ((x >= 13) && (y >= 9)) {
    x -= 13;
    y -= 9;
    ptr = ledbuf4;
  } else if ((x < 13) && (y >= 9)) {
    y -= 9;
    ptr = ledbuf3;
  } else if ((x >= 13) && (y < 9)) {
    x -= 13;
    ptr = ledbuf2;
  } else {
    ptr = ledbuf1;
  }

  if ((x >= 0) && (y >= 0) && (x < 13) && (y < 9)) {
    y = rowmap[y];
    offset = (x + ((x < 10) ? (y * 10) : (80 + y * 3))) * 3;
  }
  ptr += 1 + offset;

#endif
  if ((x & 1) || (x == 12)) {
    ptr[remap[red_offset]] = r;
    ptr[remap[green_offset]] = g;
    ptr[remap[blue_offset]] = b;
  } else {
    ptr[red_offset] = r;
    ptr[green_offset] = g;
    ptr[blue_offset] = b;
  }

  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Fill all the pixel of the RGB LED IS31FL3741 with specific color.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_fill(uint16_t color)
{
  for (int16_t i = 0; i < WIDTH; i++) {
    for (int16_t j = 0; j < HEIGHT; j++) {
      adafruit_is31fl3741_draw_pixel(i, j, color);
    }
  }
  return SL_STATUS_OK;
}

/**************************************************************************//**
 *  Push data from the buffer to the RGB LED IS31FL3741 module.
 *****************************************************************************/
sl_status_t adafruit_is31fl3741_show(void)
{
  sl_status_t ret = SL_STATUS_OK;

// Layout 1x1
#if (IS31FL3741_DISPLAY_LAYOUT == 0)
  adafruit_is31fl3741_i2c_select_device(device_list[0]);
  ret |= adafruit_is31fl3741_write_data(ledbuf);

// Layout 1x2 and 2x1
#elif ((IS31FL3741_DISPLAY_LAYOUT == 1) || (IS31FL3741_DISPLAY_LAYOUT == 4))
  adafruit_is31fl3741_i2c_select_device(device_list[0]);
  ret |= adafruit_is31fl3741_write_data(ledbuf1);
  adafruit_is31fl3741_i2c_select_device(device_list[1]);
  ret |= adafruit_is31fl3741_write_data(ledbuf2);

// Layout 1x3
#elif (IS31FL3741_DISPLAY_LAYOUT == 2)
  adafruit_is31fl3741_i2c_select_device(device_list[0]);
  ret |= adafruit_is31fl3741_write_data(ledbuf1);
  adafruit_is31fl3741_i2c_select_device(device_list[1]);
  ret |= adafruit_is31fl3741_write_data(ledbuf2);
  adafruit_is31fl3741_i2c_select_device(device_list[2]);
  ret |= adafruit_is31fl3741_write_data(ledbuf3);

// Layout 1x4 and 2x2
#elif ((IS31FL3741_DISPLAY_LAYOUT == 3) || (IS31FL3741_DISPLAY_LAYOUT == 5))
  adafruit_is31fl3741_i2c_select_device(device_list[0]);
  ret |= adafruit_is31fl3741_write_data(ledbuf1);
  adafruit_is31fl3741_i2c_select_device(device_list[1]);
  ret |= adafruit_is31fl3741_write_data(ledbuf2);
  adafruit_is31fl3741_i2c_select_device(device_list[2]);
  ret |= adafruit_is31fl3741_write_data(ledbuf3);
  adafruit_is31fl3741_i2c_select_device(device_list[3]);
  ret |= adafruit_is31fl3741_write_data(ledbuf4);

#endif
  return ret;
}
