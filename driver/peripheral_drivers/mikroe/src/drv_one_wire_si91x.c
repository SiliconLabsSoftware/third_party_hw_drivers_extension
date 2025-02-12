/***************************************************************************//**
 * @file  drv_one_wire_si91x.c
 * @brief mikroSDK 2.0 Click Peripheral Drivers - 1-Wire Master for Si91x
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
#include <stddef.h>
#include <stdbool.h>
#include "sl_si91x_peripheral_gpio.h"
#include "sl_udelay.h"
#include "drv_one_wire.h"
#include "drv_digital_in.h"
#include "drv_digital_out.h"

typedef struct {
  pin_name_t pin_name;
  sl_gpio_port_t port_index;
  unsigned int pin_index;
} one_wire_local_t;

/*!< @brief Helper macro getting minimum bits per transfer. */
#define HAL_ONE_WIRE_MINIMUM_BITS_PER_TRANSFER   (8)

#define DELAY_A                                  1
#define DELAY_B                                  34
#define DELAY_C                                  32
#define DELAY_D                                  3
#define DELAY_E                                  2
#define DELAY_F                                  29
#define DELAY_H                                  285
#define DELAY_I                                  38
#define DELAY_J                                  242

// ------------------------------------------------------------------ VARIABLES

/*!< @brief Static array for selecting One Wire data bits. */
static const uint8_t hal_one_wire_selected_bit[
  HAL_ONE_WIRE_MINIMUM_BITS_PER_TRANSFER] = {
  1, 2, 4, 8, 16, 32, 64, 128
};

/*!< @brief Helper macro for starting to enumerate device ID(s). */
static uint8_t hal_one_wire_search_rom_command = ONE_WIRE_CMD_ROM_SEARCH;

/*!< @brief Helper macro for reading a device ID. */
static uint8_t hal_one_wire_read_rom_command = ONE_WIRE_CMD_ROM_READ;

/*!< @brief Helper macro for ignoring a device ID. */
static uint8_t hal_one_wire_skip_rom_command = ONE_WIRE_CMD_ROM_SKIP;

/*!< @brief Helper macro for selecting a device with specific ID. */
static uint8_t hal_one_wire_match_rom_command = ONE_WIRE_CMD_ROM_MATCH;

/*!< @brief Helper flag if One Wire device is the last one left on the bus. */
static uint8_t last_device_flag = 0;

/*!< @brief Helper flag for last discrepancy. */
static uint8_t last_discrepancy = 0;

/*!< @brief Helper flag for last family discrepancy. */
static uint8_t last_family_discrepancy = 0;

static one_wire_t *owner = NULL;

/*!< @brief Helper instance consisting of hardware specifics. */
static one_wire_local_t one_wire_handle;

static void hal_one_wire_reconfigure(one_wire_t *obj);
static err_t hal_one_wire_reset(void);
static err_t hal_one_wire_search(one_wire_rom_address_t *one_wire_device_list);
static void hal_one_wire_write_bit(uint8_t write_data_buffer);
static void hal_one_wire_read_bit(uint8_t *read_data_buffer);
static void hal_one_wire_write_byte(uint8_t *write_data_buffer,
                                    size_t write_data_length);
static void hal_one_wire_read_byte(uint8_t *read_data_buffer,
                                   size_t read_data_length);
static void one_wire_timing_value_a(void);
static void one_wire_timing_value_b(void);
static void one_wire_timing_value_c(void);
static void one_wire_timing_value_d(void);
static void one_wire_timing_value_e(void);
static void one_wire_timing_value_f(void);
static void one_wire_timing_value_h(void);
static void one_wire_timing_value_i(void);
static void one_wire_timing_value_j(void);

err_t one_wire_open(one_wire_t *obj)
{
  if (obj == NULL) {
    return ONE_WIRE_ERROR;
  }

  if ((HAL_PIN_NC == obj->data_pin) || obj->state) {
    return ONE_WIRE_ERROR;
  }

  /* Enables appropriate PORT clock, configures pin to have digital output functionality,
   * makes sure that HIGH voltage state is applied on pin before any One Wire actions. */
  hal_one_wire_reconfigure(obj);

  owner = obj;

  return ONE_WIRE_SUCCESS;
}

void one_wire_configure_default(one_wire_t *obj)
{
  if (obj) {
    obj->data_pin = 0xFFFFFFFF;
    obj->state = false;
  }
}

err_t one_wire_reset(one_wire_t *obj)
{
  if (!obj) {
    return ONE_WIRE_ERROR;
  }

  if (!owner || (!obj->state)) {
    return ONE_WIRE_ERROR;
  }

  if ((owner != obj)) {
    hal_one_wire_reconfigure(obj);
  }

  if (hal_one_wire_reset()) {
    return ONE_WIRE_ERROR;
  }

  return ONE_WIRE_SUCCESS;
}

err_t one_wire_read_rom(one_wire_t *obj,
                        one_wire_rom_address_t *device_rom_address)
{
  if (!obj) {
    return ONE_WIRE_ERROR;
  }

  if (!owner || (!obj->state)) {
    return ONE_WIRE_ERROR;
  }

  if (!device_rom_address) {
    return ONE_WIRE_ERROR;
  }

  if ((owner != obj)) {
    hal_one_wire_reconfigure(obj);
  }

  // Initiate Reset sequence.
  if (hal_one_wire_reset()) {
    return ONE_WIRE_ERROR;
  }

  // Initiate "Read ROM" command.
  hal_one_wire_write_byte(&hal_one_wire_read_rom_command, 1);

  // Read ROM address.
  hal_one_wire_read_byte(device_rom_address->address, 8);

  return ONE_WIRE_SUCCESS;
}

err_t one_wire_skip_rom(one_wire_t *obj)
{
  if (!obj) {
    return ONE_WIRE_ERROR;
  }

  if (!owner || (!obj->state)) {
    return ONE_WIRE_ERROR;
  }

  if ((owner != obj)) {
    hal_one_wire_reconfigure(obj);
  }

  // Initiate Reset sequence.
  if (hal_one_wire_reset()) {
    return ONE_WIRE_ERROR;
  }

  // Initiate "Skip ROM" command.
  hal_one_wire_write_byte(&hal_one_wire_skip_rom_command, 1);

  return ONE_WIRE_SUCCESS;
}

err_t one_wire_match_rom(one_wire_t *obj,
                         one_wire_rom_address_t *device_rom_address)
{
  if (!obj) {
    return ONE_WIRE_ERROR;
  }

  if (!owner || (!obj->state)) {
    return ONE_WIRE_ERROR;
  }

  if (!device_rom_address) {
    return ONE_WIRE_ERROR;
  }

  if ((owner != obj)) {
    hal_one_wire_reconfigure(obj);
  }

  // Initiate Reset sequence.
  if (hal_one_wire_reset()) {
    return ONE_WIRE_ERROR;
  }

  // Initiate "Match" command.
  hal_one_wire_write_byte(&hal_one_wire_match_rom_command, 1);

  // Send ROM address.
  hal_one_wire_write_byte(device_rom_address->address, 8);

  return ONE_WIRE_SUCCESS;
}

err_t one_wire_search_first_device(one_wire_t *obj,
                                   one_wire_rom_address_t *one_wire_device_list)
{
  if (!obj) {
    return ONE_WIRE_ERROR;
  }

  if (!owner || (!obj->state)) {
    return ONE_WIRE_ERROR;
  }

  if (!one_wire_device_list) {
    return ONE_WIRE_ERROR;
  }

  if ((owner != obj)) {
    hal_one_wire_reconfigure(obj);
  }

  // Reset the search state.
  last_discrepancy = 0;
  last_device_flag = 0;
  last_family_discrepancy = 0;

  // Initiate search algorithm, in order to get first device on One Wire grid.
  hal_one_wire_search(one_wire_device_list);

  return ONE_WIRE_SUCCESS;
}

err_t one_wire_search_next_device(one_wire_t *obj,
                                  one_wire_rom_address_t *one_wire_device_list)
{
  if (!obj) {
    return ONE_WIRE_ERROR;
  }

  if (!owner || (!obj->state)) {
    return ONE_WIRE_ERROR;
  }

  if (!one_wire_device_list) {
    return ONE_WIRE_ERROR;
  }

  if ((owner != obj)) {
    hal_one_wire_reconfigure(obj);
  }

  // Initiate search algorithm, in order to get first device on One Wire grid.
  hal_one_wire_search(one_wire_device_list);

  return ONE_WIRE_SUCCESS;
}

err_t one_wire_write_byte(one_wire_t *obj,
                          uint8_t *write_data_buffer,
                          size_t write_data_length)
{
  if (!obj) {
    return ONE_WIRE_ERROR;
  }

  if (!owner || (!obj->state)) {
    return ONE_WIRE_ERROR;
  }

  if (write_data_length <= 0) {
    return ONE_WIRE_ERROR;
  }

  if (!write_data_buffer) {
    return ONE_WIRE_ERROR;
  }

  if ((owner != obj)) {
    hal_one_wire_reconfigure(obj);
  }

  hal_one_wire_write_byte(write_data_buffer, write_data_length);

  return ONE_WIRE_SUCCESS;
}

err_t one_wire_read_byte(one_wire_t *obj,
                         uint8_t *read_data_buffer,
                         size_t read_data_length)
{
  if (!obj) {
    return ONE_WIRE_ERROR;
  }

  if (!owner || (!obj->state)) {
    return ONE_WIRE_ERROR;
  }

  if (!read_data_buffer) {
    return ONE_WIRE_ERROR;
  }

  if (read_data_length <= 0) {
    return ONE_WIRE_ERROR;
  }

  if ((owner != obj)) {
    hal_one_wire_reconfigure(obj);
  }

  hal_one_wire_read_byte(read_data_buffer, read_data_length);

  return ONE_WIRE_SUCCESS;
}

static void hal_one_wire_reconfigure(one_wire_t *obj)
{
  digital_out_t one_wire_pin;

  // Memorize info about pin number (for future use).
  one_wire_handle.pin_name = obj->data_pin;
  one_wire_handle.port_index =
    (sl_gpio_port_t) hal_gpio_port_index(one_wire_handle.pin_name);
  one_wire_handle.pin_index = hal_gpio_pin_index(one_wire_handle.pin_name);

  // Set pin to be digital output.
  digital_out_init(&one_wire_pin, one_wire_handle.pin_name);

  // Make sure that we have HIGH voltage state before executing any actions.
  digital_out_high(&one_wire_pin);

  // Set object state to true.
  obj->state = true;
}

static err_t hal_one_wire_reset(void)
{
  /* Variable for checking whether there are device(s) on
   * One Wire data pin (0) or there aren't any devices at all (1). */
  uint8_t device_response = 1;

  // Make sure that pin has output capability.
  sl_si91x_gpio_set_pin_direction(one_wire_handle.port_index,
                                  one_wire_handle.pin_index,
                                  (sl_si91x_gpio_direction_t)GPIO_OUTPUT);

  // Set pin to LOW voltage level.
  sl_gpio_clear_pin_output(one_wire_handle.port_index,
                           one_wire_handle.pin_index);

  // Timing value for reset of One Wire bus - LOW voltage level.
  one_wire_timing_value_h();

  // Release pin ( pull-up resistor will do the rest (pull the data line up) ).
  sl_si91x_gpio_set_pin_direction(one_wire_handle.port_index,
                                  one_wire_handle.pin_index,
                                  (sl_si91x_gpio_direction_t)GPIO_INPUT);

  // Timing value for reset of One Wire bus - Master sample pulse.
  one_wire_timing_value_i();

  // Check whether there are devices on One Wire data pin.
  device_response = (sl_gpio_get_pin_input(one_wire_handle.port_index,
                                           one_wire_handle.pin_index))
                    ? 0x01 : 0x00;

  // Provide enough time for power injection into internal power logic of devices that are present.
  one_wire_timing_value_j();

  // Return final result of device response.
  return device_response;
}

static void hal_one_wire_write_bit(uint8_t write_data_buffer)
{
  // Make sure that pin has output capability.
  sl_si91x_gpio_set_pin_direction(one_wire_handle.port_index,
                                  one_wire_handle.pin_index,
                                  (sl_si91x_gpio_direction_t)GPIO_OUTPUT);

  // Set pin to LOW voltage level.
  sl_gpio_clear_pin_output(one_wire_handle.port_index,
                           one_wire_handle.pin_index);

  // Check whether a bit is binary one.
  if (write_data_buffer & 1) {
    // Timing value "a" for writing logical '1' - LOW voltage level.
    one_wire_timing_value_a();
    // Else, bit is binary zero.
  } else {
    // Timing value "c" for writing logical '0' - LOW voltage level.
    one_wire_timing_value_c();
  }

  // Release One Wire data line ( pull-up resistor will pull the data line up ).
  sl_si91x_gpio_set_pin_direction(one_wire_handle.port_index,
                                  one_wire_handle.pin_index,
                                  (sl_si91x_gpio_direction_t)GPIO_INPUT);

  // Recommended timing after writing 1's or 0's.
  if (write_data_buffer & 1) {
    // Timing value "b" for writing logical '1' - LOW voltage level.
    one_wire_timing_value_b();
  } else {
    // Timing value "d" for writing logical '0' - HIGH voltage level.
    one_wire_timing_value_d();
  }
}

static void hal_one_wire_read_bit(uint8_t *read_data_buffer)
{
  // Make sure that pin has output capability.
  sl_si91x_gpio_set_pin_direction(one_wire_handle.port_index,
                                  one_wire_handle.pin_index,
                                  (sl_si91x_gpio_direction_t)GPIO_OUTPUT);

  // Set pin to LOW voltage level.
  sl_gpio_clear_pin_output(one_wire_handle.port_index,
                           one_wire_handle.pin_index);

  // Timing value "a" for bit reading - LOW voltage level.
  one_wire_timing_value_a();

  // Release One Wire data line ( pull-up resistor will pull the data line up ).
  sl_si91x_gpio_set_pin_direction(one_wire_handle.port_index,
                                  one_wire_handle.pin_index,
                                  (sl_si91x_gpio_direction_t)GPIO_INPUT);

  // Timing value "e" for sampling read information.
  one_wire_timing_value_e();

  // Read bit.
  read_data_buffer[0] = (sl_gpio_get_pin_input(one_wire_handle.port_index,
                                               one_wire_handle.pin_index))
                        ? 0x01 : 0x00;

  // Timing value "f" for the rest of the read operation.
  one_wire_timing_value_f();
}

static err_t hal_one_wire_search(one_wire_rom_address_t *one_wire_device_list)
{
  // Final search result variable.
  uint8_t search_result = 0;

  // Initialize variables for search method.
  uint8_t rom_byte_number = 0;
  uint8_t rom_byte_mask = 1;
  uint8_t id_bit_number = 1;
  uint8_t last_zero = 0;

  // The first bit read in a bit search sequence.
  // ( logical AND of all of the id_bit_number bits of the devices that are still participating in the search ).
  uint8_t id_bit = 0;

  // The complement of id_bit.
  // ( logical AND of the complement of all id_bit_number bits of the devices that are still participating in the search ).
  uint8_t cmp_id_bit = 0;

  // Search direction ( bit-per-bit search ).
  uint8_t search_direction = 0;

  // If the last call was not the last one...
  if (!last_device_flag) {
    // If there were no any device while executing One Wire reset sequence...
    if (hal_one_wire_reset()) {
      // Reset all the important variables.
      last_device_flag = 0;
      last_discrepancy = 0;
      last_family_discrepancy = 0;

      // Stop searching because there are no any One Wire capable devices.
      return ONE_WIRE_ERROR;
    }

    // If device(s) has(have) been found, initiate "Search" command.
    hal_one_wire_write_byte(&hal_one_wire_search_rom_command, 1);

    // Iterate until all 64 bits (8 bytes) of unique ROM 'registration' numbers have not been found.
    do {
      // Read a bit.
      hal_one_wire_read_bit(&id_bit);

      // Then, read its complement.
      hal_one_wire_read_bit(&cmp_id_bit);

      // Check whether no devices participating in current search.
      if ((id_bit == 1) && (cmp_id_bit == 1)) {
        break;
        // Proceed, because we have found some device(s).
      } else {
        // We are going to check once again whether read bit and
        // its complement of all the devices on the One Wire grid are not the same.
        // If they are not the same, we are going to start our search with non-complement bit.
        if (id_bit != cmp_id_bit) {
          search_direction = id_bit;            // Bit write value for search.

          // Otherwise, there are both binary zeros and ones in the current
          // bit position of the participating ROM numbers. This is a discrepancy.
        } else {
          if (id_bit_number < last_discrepancy) {
            search_direction =
              ((one_wire_device_list->address[rom_byte_number] & rom_byte_mask)
               > 0);
          } else {
            search_direction = (id_bit_number == last_discrepancy);
          }

          // If 0 is picked, save its position.
          if (search_direction == 0) {
            last_zero = id_bit_number;

            // Check for last discrepancy in family.
            if (last_zero < 9) {
              last_family_discrepancy = last_zero;
            }
          }
        }

        // Set or clear bit in the ROM byte rom_byte_number with mask rom_byte_mask.
        if (search_direction == 1) {
          one_wire_device_list->address[rom_byte_number] |= rom_byte_mask;
        } else {
          one_wire_device_list->address[rom_byte_number] &= ~rom_byte_mask;
        }

        // Search number search direction write bit.
        hal_one_wire_write_bit(search_direction);

        // Increment the byte counter "id_bit_number", and shift the mask "rom_byte_mask".
        id_bit_number++;
        rom_byte_mask <<= 1;

        // If the mask is zero, then go to new "serial_num" byte, "rom_byte_number" and "reset_mask".
        if (rom_byte_mask == 0) {
          rom_byte_number++;
          rom_byte_mask = 1;
        }
      }
    } while (rom_byte_number < HAL_ONE_WIRE_MINIMUM_BITS_PER_TRANSFER);

    // If the search was successful then...
    if (!(id_bit_number < 65)) {
      last_discrepancy = last_zero;

      // Check for last device.
      if (last_discrepancy == 0) {
        last_device_flag = 1;
      }
      search_result = 1;
    }
  }
  // If no device found then reset counters so next "search" will be like a first.
  if (!search_result || !one_wire_device_list->address[0]) {
    last_discrepancy = 0;
    last_family_discrepancy = 0;
    last_device_flag = 0;
    search_result = 0;
  }

  // Return info whether we have found some device ID or not.
  return search_result;
}

static void hal_one_wire_write_byte(uint8_t *write_data_buffer,
                                    size_t write_data_length)
{
  size_t local_byte_checker = 0;
  uint8_t local_bit_checker = 0;

  // For every byte to be sent...
  while (local_byte_checker != write_data_length) {
    // ...restart bit checker...
    local_bit_checker = 0;

    // For every bit in byte to be sent...
    while (local_bit_checker != HAL_ONE_WIRE_MINIMUM_BITS_PER_TRANSFER) {
      // Make sure that pin has output capability.
      sl_si91x_gpio_set_pin_direction(one_wire_handle.port_index,
                                      one_wire_handle.pin_index,
                                      (sl_si91x_gpio_direction_t)GPIO_OUTPUT);

      // Set pin to LOW voltage level.
      sl_gpio_clear_pin_output(one_wire_handle.port_index,
                               one_wire_handle.pin_index);

      // Check whether a bit is binary one.
      if (write_data_buffer[local_byte_checker]
          & hal_one_wire_selected_bit[local_bit_checker]) {
        // Timing value "a" for writing logical '1' - HIGH voltage level.
        one_wire_timing_value_a();
        // Else, bit is binary zero.
      } else {
        // Timing value "c" for writing logical '0' - LOW voltage level.
        one_wire_timing_value_c();
      }

      // Release One Wire data line ( pull-up resistor will pull the data line up ).
      sl_si91x_gpio_set_pin_direction(one_wire_handle.port_index,
                                      one_wire_handle.pin_index,
                                      (sl_si91x_gpio_direction_t)GPIO_INPUT);

      // Recommended timing after writing 1's or 0's.
      if (write_data_buffer[local_byte_checker]
          & hal_one_wire_selected_bit[local_bit_checker]) {
        // Timing value "b" for writing logical '1' - LOW voltage level.
        one_wire_timing_value_b();
      } else {
        // Timing value "d" for writing logical '0' - HIGH voltage level.
        one_wire_timing_value_d();
      }
      // Increment so we could send another bit.
      ++local_bit_checker;
    }
    // Increment so we could send another byte.
    ++local_byte_checker;
  }
}

static void hal_one_wire_read_byte(uint8_t *read_data_buffer,
                                   size_t read_data_length)
{
  size_t local_byte_checker = 0;
  uint8_t local_bit_checker = 0;
  uint8_t local_buffer = 0;

  // For every byte to be read...
  while (local_byte_checker != read_data_length) {
    // ...restart bit checker and buffer...
    local_bit_checker = 0;
    local_buffer = 0;

    // For every bit in byte to be read...
    while (local_bit_checker != HAL_ONE_WIRE_MINIMUM_BITS_PER_TRANSFER) {
      // Make sure that pin has output capability.
      sl_si91x_gpio_set_pin_direction(one_wire_handle.port_index,
                                      one_wire_handle.pin_index,
                                      (sl_si91x_gpio_direction_t)GPIO_OUTPUT);

      // Set pin to LOW voltage level.
      sl_gpio_clear_pin_output(one_wire_handle.port_index,
                               one_wire_handle.pin_index);

      // Timing value "a" for bit reading - LOW voltage level.
      one_wire_timing_value_a();

      // Release One Wire data line ( pull-up resistor will pull the data line up ).
      sl_si91x_gpio_set_pin_direction(one_wire_handle.port_index,
                                      one_wire_handle.pin_index,
                                      (sl_si91x_gpio_direction_t)GPIO_INPUT);

      // Timing value "e" for sampling read information.
      one_wire_timing_value_e();

      // Read bit.
      local_buffer += ((sl_gpio_get_pin_input(one_wire_handle.port_index,
                                              one_wire_handle.pin_index))
                       ? 0x01 : 0x00) << local_bit_checker;

      // Timing value "f" for the rest of the read operation.
      one_wire_timing_value_f();

      // Increment so we could send another bit.
      ++local_bit_checker;
    }
    // Send back one logical level up a byte of data that has been just read.
    read_data_buffer[local_byte_checker++] = local_buffer;
  }
}

static void one_wire_timing_value_a(void)
{
  sl_udelay_wait(DELAY_A);
}

static void one_wire_timing_value_b(void)
{
  sl_udelay_wait(DELAY_B);
}

static void one_wire_timing_value_c(void)
{
  sl_udelay_wait(DELAY_C);
}

static void one_wire_timing_value_d(void)
{
  sl_udelay_wait(DELAY_D);
}

static void one_wire_timing_value_e(void)
{
  sl_udelay_wait(DELAY_E);
}

static void one_wire_timing_value_f(void)
{
  sl_udelay_wait(DELAY_F);
}

static void one_wire_timing_value_h(void)
{
  sl_udelay_wait(DELAY_H);
}

static void one_wire_timing_value_i(void)
{
  sl_udelay_wait(DELAY_I);
}

static void one_wire_timing_value_j(void)
{
  sl_udelay_wait(DELAY_J);
}

// ------------------------------------------------------------------------- END
