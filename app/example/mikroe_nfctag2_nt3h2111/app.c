/***************************************************************************//**
 * @file
 * @brief Top level application functions
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
#include "mikroe_nt3h2111.h"
#include "sl_sleeptimer.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_i2c_instances.h"
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else /* None Si91x device */
#include "sl_i2cspm_instances.h"
#include "app_log.h"
#include "sl_iostream.h"
#define app_printf(...) app_log(__VA_ARGS__)
static sl_iostream_t *default_iostream = NULL;
#endif

// #define NTAG_I2C_BLOCK_SIZE 16

static uint16_t wdt_timer = 0;

static const uint8_t default_begining_of_memory[] = {
  0xAA, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0xE1, 0x10, 0x6D, 0x00
};

static const uint8_t ndef_rec[] = {
  0x03,          // NDEF Message
  15,            // Message size
  0xD1,          // Record header
  1,             // Type Length - 1 byte
  11,            // Payload Length - 11 bytes
  'U',           // Type / URI
  0x01,          // Payload
  's', 'i', 'l', 'a', 'b', 's', '.', 'c', 'o', 'm',
  0xFE           // NDEF Message End Mark
};

static const uint8_t null_block[] = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static const uint8_t default_page_56[] = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF
};

static const uint8_t default_page_57[] = {
  0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static const uint8_t default_page_58[] = {
  0x01, 0x00, 0xF8, 0x48,
  0x08, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static sl_status_t factory_reset_tag(void);
static sl_status_t test_read_session(void);
static sl_status_t test_write_session(void);
static uint8_t read_input (void);

static mikroe_i2c_handle_t app_i2c_instance = NULL;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
  default_iostream = sl_iostream_get_default();
#endif

  /* Initialize NT3H2111 I2C communication. */
  nt3h2111_init(app_i2c_instance);

  /* Print project name. */
  app_printf(
    "\r\n*********************************************************\r\n*\r\n");
  app_printf("* Third Party Hardware Drivers - NT3H2111 - NFC Tag 2 Click\r\n");
  app_printf(
    "*\r\n*********************************************************\r\n");
  app_printf("*\r\n* Give a input, your input should be between 0 and 4.\r\n");
  app_printf("* Press enter when you finish.\r\n");
  app_printf("*\r\n*  0. Factory reset Tag\r\n");
  app_printf("*\r\n*  1. Read the session registers\r\n");
  app_printf("*\r\n*  2. Write the session registers\r\n");
  app_printf("*\r\n*  3. Write Tag\r\n");
  app_printf("*\r\n*  4. Read Tag\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  sl_status_t stt;
  uint8_t input_cmd = 0;

  app_printf("\r\nGive a your number, press enter when you finish: ");
  app_printf("\r\n>");

  input_cmd = read_input();

  switch (input_cmd) {
    case 0:
      app_printf("Start to reset the memory to its default values\r\n");
      stt = factory_reset_tag();
      if (stt == SL_STATUS_OK) {
        app_printf("Reset successful!\r\n");
      } else {
        app_printf("Reseting Failed with Err = 0x%lx\r\n", stt);
      }
      break;
    case 1:
      app_printf("Start reading the content of Session Watchdog Timer\r\n");
      stt = test_read_session();
      if (stt == SL_STATUS_OK) {
        app_printf("Current Watchdog Timer = %d\r\n", wdt_timer);
      } else {
        app_printf("Read Watchdog Timer Failed with Err = 0x%lx\r\n", stt);
      }
      break;
    case 2:
      app_printf("Start writing new value to the Session Watchdog Timer\r\n");
      stt = test_write_session();
      if (stt == SL_STATUS_OK) {
        app_printf("Wrote successful Watchdog Timer to new value = %d\r\n",
                   wdt_timer);
      } else {
        app_printf("Set Watchdog Timer Failed with Err = 0x%lx\r\n", stt);
      }
      break;
    case 3:
      // Write an NDEF message into NT3H2111 EEPROM.
      app_printf("Start writing an NDEF message into NT3H2111 EEPROM\r\n");
      stt = nt3h2111_write_bytes(16, ndef_rec, sizeof(ndef_rec));
      if (stt == SL_STATUS_OK) {
        app_printf("Write Tag successful\r\n");
      } else {
        app_printf("Writing Tag failed with Err = 0x%lx\r\n", stt);
      }
      break;
    case 4:
    {
      uint8_t buff[32];
      app_printf("Start reading from NT3H2111 EEPROM\r\n");
      stt = nt3h2111_read_bytes(16, buff, 32);
      if (stt == SL_STATUS_OK) {
        app_printf("Read Tag successful!\r\n");
        for (int i = 0; i < 8; i++) {
          app_printf(" 0x%02x, 0x%02x, 0x%02x, 0x%02x\r\n",
                     buff[4 * i],
                     buff[4 * i + 1],
                     buff[4 * i + 2],
                     buff[4 * i + 3]);
        }
      } else {
        app_printf("Read failed with Err = 0x%lx\r\n", stt);
      }
    }
    break;
  }
  app_printf(
    "*\r\n*********************************************************\r\n");
}

/*
 * Example how to use :
 *  - nt3h2111_write_bytes
 *  - nt3h2111_write_block
 *
 * Resets the memory to its default values
 */
static sl_status_t factory_reset_tag(void)
{
  sl_status_t result;

  // Reset default eeprom memory values (smart poster)
  result = nt3h2111_write_block(0, default_begining_of_memory);
  if (result != SL_STATUS_OK) {
    return result;
  }

  // Reset pages from 1 to 56
  uint8_t page = 1;
  while (page < 56) {
    result = nt3h2111_write_block(page, null_block);
    if (result != SL_STATUS_OK) {
      return result;
    }
    page++;
  }
  // Reset pages 56,57,58
  result = nt3h2111_write_block(56, default_page_56);
  if (result != SL_STATUS_OK) {
    return result;
  }
  result = nt3h2111_write_block(57, default_page_57);
  if (result != SL_STATUS_OK) {
    return result;
  }
  result = nt3h2111_write_block(58, default_page_58);
  if (result != SL_STATUS_OK) {
    return result;
  }

  return SL_STATUS_OK;
}

/*
 * Example how to use :
 *  - nfctag2_session_get
 *  - nfctag2_session_set
 *
 * Dumps the content of Session Watchdog Timer
 * Changing of session settings
 */
static sl_status_t test_read_session(void)
{
  sl_status_t stt;
  uint8_t temp[2];

  stt = nt3h2111_get_session(SESSION_WDT_LS, &temp[0]);
  stt |= nt3h2111_get_session(SESSION_WDT_MS, &temp[1]);
  if (stt == SL_STATUS_OK) {
    wdt_timer = (uint16_t)temp[1] << 8 | temp[0];
  }
  return stt;
}

static sl_status_t test_write_session(void)
{
  sl_status_t stt;

  wdt_timer++;
  stt = nt3h2111_set_session(SESSION_WDT_MS, 0xff, wdt_timer >> 8);
  stt |= nt3h2111_set_session(SESSION_WDT_LS, 0xff, wdt_timer & 0xff);
  return stt;
}

static uint8_t read_input(void)
{
  char char_buff;
  size_t bytes_read;
  uint8_t input_cmd = 0;
  uint8_t size_len = 0;

  while (1) {
#if (defined(SLI_SI917))
    char_buff = DEBUGIN();
    bytes_read = 1;
#else
    sl_iostream_read(default_iostream, &char_buff, 1, &bytes_read);
#endif

    if (bytes_read > 0) {
      if ((char_buff >= '0') && (char_buff <= '4')) {
        if (size_len < 1) {
          input_cmd = char_buff - '0';
          size_len++;
          app_printf("%c", char_buff);
        } else {
          app_printf(
            "\r\ntoo many digits. Please input a number between 0 and 4, press ENTER when you finish:");
          size_len = 0;
        }
      } else if (char_buff == '\r') {
        if (size_len == 0) {
          app_printf(
            "\r\nPlease input a number between 0 and 9, press ENTER when you finish: ");
          continue;
        }
        app_printf("\r\n");
        break;
      } else {
        app_printf(
          "\r\nInvalid character. Please input a number between 0 and 4, press ENTER when you finish: ");
        size_len = 0;
      }
    }
  }

  return input_cmd;
}
