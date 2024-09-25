/***************************************************************************//**
 * @file   mikroe_nt3h2111.c
 * @brief  Implementation for high level NT3H2111 functionalities.
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
#include "sl_sleeptimer.h"
#include "mikroe_nt3h2111.h"
#include "mikroe_nt3h2111_config.h"
#include "drv_i2c_master.h"
#include "drv_digital_in.h"

#if !defined (min)
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

typedef struct
{
  i2c_master_t i2c;
  digital_in_t fd_pin;
}mikroe_nt3h2111_t;

// Global variables
static mikroe_nt3h2111_t s_nt3h2111;
static bool nt3h2111_is_initialized = false;

/**************************************************************************//**
 * @brief
 *  Initialize NT3H2111 peripherals.
 *
 * @param[in] init
 *  Initialization settings.
 *****************************************************************************/

/**************************************************************************//**
 * @brief
 *  Read data through I2C.
 *
 * @param[in] size
 *  Number of bytes to be read
 *
 * @param[out] pdata
 *  Buffer to store read data
 *
 * @returns
 *  I2C transfer result code.
 *****************************************************************************/
static sl_status_t nt3h2111_i2c_read_bytes(uint32_t size, uint8_t *pdata)
{
  if (NULL == pdata) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (I2C_MASTER_SUCCESS != i2c_master_read(&s_nt3h2111.i2c,
                                            pdata,
                                            size)) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

/**************************************************************************//**
 * @brief
 *  Write data through I2C.
 *
 * @param[in] size
 *  Number of bytes to be written
 *
 * @param[out] pdata
 *  Buffer of data to be written
 *
 * @returns
 *  I2C transfer result code.
 *****************************************************************************/
sl_status_t nt3h2111_i2c_write_bytes(uint32_t size,
                                     uint8_t *pdata)
{
  if (NULL == pdata) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (I2C_MASTER_SUCCESS != i2c_master_write(&s_nt3h2111.i2c,
                                             pdata,
                                             size)) {
    return SL_STATUS_TRANSMIT;
  }

  return SL_STATUS_OK;
}

sl_status_t nt3h2111_init(mikroe_i2c_handle_t i2cspm)
{
  if (i2cspm == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  // If already initialized, return status
  if (nt3h2111_is_initialized == true) {
    return SL_STATUS_ALREADY_INITIALIZED;
  }

  // Update i2cspm instance and i2c addr
  s_nt3h2111.i2c.handle = i2cspm;

  i2c_master_config_t i2c_cfg;
  i2c_master_configure_default(&i2c_cfg);

  i2c_cfg.addr = MIKROE_NT3H211_ADDR;
  i2c_cfg.timeout_pass_count = 0;

#if (MIKROE_NT3H211_I2C_UC == 1)
  i2c_cfg.speed = MIKROE_NT3H211_I2C_SPEED_MODE;
#endif

  if (i2c_master_open(&s_nt3h2111.i2c, &i2c_cfg) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

#if defined(MIKROE_NT3H211_FD_PORT) && defined(MIKROE_NT3H211_FD_PIN)
  pin_name_t pin = hal_gpio_pin_name(MIKROE_NT3H211_FD_PORT,
                                     MIKROE_NT3H211_FD_PIN);

  digital_in_init(&s_nt3h2111.fd_pin, pin);
#endif

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  De-initialize the NT3H2111.
 ******************************************************************************/
sl_status_t nt3h2111_deinit(void)
{
  if (s_nt3h2111.i2c.handle == false) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  i2c_master_close(&s_nt3h2111.i2c);

  // Mark driver as not initialized
  nt3h2111_is_initialized = false;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *  Read a configuration register in NT3H2111.
 ******************************************************************************/
sl_status_t nt3h2111_get_config(nt3h2111_config_reg_addr_t rega, uint8_t *data)
{
  sl_status_t result;
  uint8_t buff[2];

  if (NULL == data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  /* Assign mema. */
  buff[0] = NT3H2111_CONFIG_REG_ADDR;

  /* Assign rega. */
  buff[1] = rega;

  /* Write addresses. */
  result = nt3h2111_i2c_write_bytes(2, buff);
  if (result != SL_STATUS_OK) {
    return result;
  }

  /* Read regdat from NT3H2111.  */
  return nt3h2111_i2c_read_bytes(1, data);
}

/***************************************************************************//**
 *  Write to a configuration register in NT3H2111.
 ******************************************************************************/
sl_status_t nt3h2111_set_config(nt3h2111_config_reg_addr_t rega,
                                uint8_t mask,
                                uint8_t regd)
{
  uint8_t buff[4];

  /* Assign mema. */
  buff[0] = NT3H2111_CONFIG_REG_ADDR;

  /* Assign rega. */
  buff[1] = rega;

  /* Assign mask. */
  buff[2] = mask;

  /* Assign regdat. */
  buff[3] = regd;

  /* Write to NT3H2111. */
  return nt3h2111_i2c_write_bytes(4, buff);
}

/***************************************************************************//**
 *  Read a session register in NT3H2111.
 ******************************************************************************/
sl_status_t nt3h2111_get_session(nt3h2111_session_reg_addr_t rega,
                                 uint8_t *data)
{
  sl_status_t result;
  uint8_t buff[2];

  if (NULL == data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  /* Assign mema. */
  buff[0] = NT3H2111_SESSION_REG_ADDR;

  /* Assign rega. */
  buff[1] = rega;

  /* Write addresses. */
  result = nt3h2111_i2c_write_bytes(2, buff);
  if (result != SL_STATUS_OK) {
    return result;
  }

  /* Read regdat from NT3H2111.  */
  return nt3h2111_i2c_read_bytes(1, data);
}

/***************************************************************************//**
 *  Write to a session register in NT3H2111.
 ******************************************************************************/
sl_status_t nt3h2111_set_session(nt3h2111_session_reg_addr_t rega,
                                 uint8_t mask,
                                 uint8_t regd)
{
  uint8_t buff[4];

  /* Assign mema. */
  buff[0] = NT3H2111_SESSION_REG_ADDR;

  /* Assign rega. */
  buff[1] = rega;

  /* Assign mask. */
  buff[2] = mask;

  /* Assign regdat. */
  buff[3] = regd;

  /* Write to NT3H2111. */
  return nt3h2111_i2c_write_bytes(4, buff);
}

/***************************************************************************//**
 *  Read a memory block (16 bytes) from NT3H2111.
 ******************************************************************************/
sl_status_t nt3h2111_read_block(uint8_t mema, uint8_t *data)
{
  sl_status_t result;

  if (NULL == data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  /* Write memory address of the block of memory that is intended to be read */
  result = nt3h2111_i2c_write_bytes(1,
                                    &mema);
  if (result != SL_STATUS_OK) {
    return result;
  }

  /* Read a block. */
  return nt3h2111_i2c_read_bytes(NT3H2111_BLOCK_SIZE, data);
}

/***************************************************************************//**
 *   Write a memory block (16 bytes) to NT3H2111.
 ******************************************************************************/
sl_status_t nt3h2111_write_block(uint8_t mema, const uint8_t *data)
{
  sl_status_t result = SL_STATUS_OK;
  uint32_t timeout = NT3H2111_WRITE_DELAY_MS / 5 + 1;
  uint8_t ns_reg;
  uint8_t buff[NT3H2111_BLOCK_SIZE + 1];

  if (NULL == data) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  /* Assign mema. */
  buff[0] = mema;

  /* Assign data. */
  memcpy(&buff[1], data, NT3H2111_BLOCK_SIZE);

  /* Write to NT3H2111. */
  result = nt3h2111_i2c_write_bytes(sizeof(buff), buff);
  if (result != SL_STATUS_OK) {
    return result;
  }

  /* Do not wait for completion when writing SRAM */
  if ((mema >= NT3H2111_MEM_BLOCK_START_SRAM)
      && (mema < NT3H2111_MEM_BLOCK_START_SRAM + NT3H2111_MEM_SRAM_BLOCKS)) {
    return SL_STATUS_OK;
  }

  /* Wait for completion */
  do {
    sl_sleeptimer_delay_millisecond(5);
    result = nt3h2111_get_session(SESSION_NS_REG, &ns_reg);
    if (result != SL_STATUS_OK) {
      break;
    }

    timeout--;
  } while (timeout && (ns_reg & NT3H2111_NS_REG_MASK_EEPROM_WR_BUSY));

  if (0 == timeout) {
    return SL_STATUS_TIMEOUT;
  }

  return result;
}

/***************************************************************************//**
 * Read byte(s) from the selected device
 ******************************************************************************/
sl_status_t nt3h2111_read_bytes(uint8_t addr, uint8_t *bytes, uint16_t len)
{
  sl_status_t result = SL_STATUS_OK;
  uint8_t rx_buff[16];
  uint16_t bytes_read = 0;

  if (NULL == bytes) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  while (bytes_read < len) {
    uint8_t current_block = (addr + bytes_read) / NT3H2111_BLOCK_SIZE;
    uint8_t begin = (addr + bytes_read) % NT3H2111_BLOCK_SIZE;
    uint8_t current_len =
      min((len - bytes_read), (NT3H2111_BLOCK_SIZE - begin));

    if (current_len < NT3H2111_BLOCK_SIZE) {
      size_t i = 0;

      /* Read block into rx_buff only */
      result = nt3h2111_read_block(current_block, rx_buff);
      if (result != SL_STATUS_OK) {
        break;
      }

      /* Modify rx_buff */
      for (i = 0; i < current_len; i++) {
        bytes[bytes_read + i] = rx_buff[begin + i];
      }
    } else {
      /* Full block read */
      result = nt3h2111_read_block(current_block, bytes + bytes_read);
      if (result != SL_STATUS_OK) {
        break;
      }
    }

    bytes_read += current_len;
  }
  return result;
}

/***************************************************************************//**
 * Write byte(s) to the selected device
 ******************************************************************************/
sl_status_t nt3h2111_write_bytes(uint8_t addr,
                                 const uint8_t *bytes,
                                 uint16_t len)
{
  sl_status_t result = SL_STATUS_OK;
  uint8_t rx_buff[16];
  uint16_t bytes_written = 0;

  if (NULL == bytes) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  while (bytes_written < len) {
    uint8_t current_block = (addr + bytes_written) / NT3H2111_BLOCK_SIZE;
    uint8_t begin = (addr + bytes_written) % NT3H2111_BLOCK_SIZE;
    uint8_t current_len = min(len - bytes_written, NT3H2111_BLOCK_SIZE - begin);

    if (current_len < NT3H2111_BLOCK_SIZE) {
      size_t i = 0;

      /* Read block into rx_buff only */
      result = nt3h2111_read_block(current_block, rx_buff);
      if (result != SL_STATUS_OK) {
        break;
      }

      /* check if it is the first Block(0x00) and not the I2C Addr
       * be careful with writing of first byte in management block
       * the byte contains part of the serial number on read but
       * on write the I2C address of the device can be modified
       */
      if ((0x00 == current_block) && (NT3H2111__MEM_ADRR_I2C_ADDR < begin)) {
        rx_buff[0] = MIKROE_NT3H211_ADDR;
      }

      /* modify rx_buffer */
      for (i = 0; i < current_len; i++) {
        rx_buff[begin + i] = bytes[bytes_written + i];
      }

      /* Write back modified buffer */
      result = nt3h2111_write_block(current_block, rx_buff);
      if (result != SL_STATUS_OK) {
        break;
      }
    } else {
      /* Full block write */
      result = nt3h2111_write_block(current_block,
                                    (bytes + bytes_written));
      if (result != SL_STATUS_OK) {
        break;
      }
    }

    bytes_written += current_len;
  }

  return result;
}

/***************************************************************************//**
 *  Encode NC_REG content to one byte of data.
 ******************************************************************************/
uint8_t nt3h2111_encode_nc_reg(nt3h2111_nc_reg_t nc_reg)
{
  uint8_t result = 0;

  if (nc_reg.nfcs_i2c_rst_on_off) {
    result += ((ON << NT3H2111_NC_REG_NFCS_I2C_RST_ON_OFF_SHIFT)
               & NT3H2111_NC_REG_NFCS_I2C_RST_ON_OFF_M);
  }

  if (nc_reg.pthru_on_off) {
    result += ((ON << NT3H2111_NC_REG_PTHRU_ON_OFF_SHIFT)
               & NT3H2111_NC_REG_PTHRU_ON_OFF_M);
  }

  result += ((nc_reg.fd_off << NT3H2111_NC_REG_FD_OFF_SHIFT)
             & NT3H2111_NC_REG_FD_OFF_M);

  result += ((nc_reg.fd_on << NT3H2111_NC_REG_FD_ON_SHIFT)
             & NT3H2111_NC_REG_FD_ON_M);

  if (nc_reg.sram_mirror_on_off) {
    result += ((ON << NT3H2111_NC_REG_SRAM_MIRROR_ON_OFF_SHIFT)
               & NT3H2111_NC_REG_SRAM_MIRROR_ON_OFF_M);
  }

  if (nc_reg.transfer_dir) {
    result += ((TRANSFER_DIR_NFC_TO_I2C << NT3H2111_NC_REG_TRANSFER_DIR_SHIFT)
               & NT3H2111_NC_REG_TRANSFER_DIR_M);
  }

  return result;
}

/***************************************************************************//**
 *  Decode NC_REG content from one byte of raw data.
 ******************************************************************************/
nt3h2111_nc_reg_t nt3h2111_decode_nc_reg(uint8_t reg_value)
{
  nt3h2111_nc_reg_t nc_reg;

  if (reg_value & BIT7_MASK) {
    nc_reg.nfcs_i2c_rst_on_off = ON;
  } else {
    nc_reg.nfcs_i2c_rst_on_off = OFF;
  }

  if (reg_value & BIT6_MASK) {
    nc_reg.pthru_on_off = ON;
  } else {
    nc_reg.pthru_on_off = OFF;
  }

  nc_reg.fd_off
    = (reg_value & NT3H2111_NC_REG_FD_OFF_M >> NT3H2111_NC_REG_FD_OFF_SHIFT);

  nc_reg.fd_on
    = (reg_value & NT3H2111_NC_REG_FD_ON_M >> NT3H2111_NC_REG_FD_ON_SHIFT);

  if (reg_value & BIT1_MASK) {
    nc_reg.sram_mirror_on_off = ON;
  } else {
    nc_reg.sram_mirror_on_off = OFF;
  }

  if (reg_value & BIT0_MASK) {
    nc_reg.transfer_dir = TRANSFER_DIR_NFC_TO_I2C;
  } else {
    nc_reg.transfer_dir = TRANSFER_DIR_I2C_TO_NFC;
  }

  return nc_reg;
}

/***************************************************************************//**
 *  Encode I2C_CLOCK_STR content to one byte of data.
 ******************************************************************************/
uint8_t nt3h2111_i2c_encode_clock_str(
  nt3h2111_i2c_clock_str_t i2c_clock_str_reg)
{
  uint8_t result = 0;

  if (i2c_clock_str_reg.i2c_clock_str) {
    result += ((ENABLED << NT3H2111_I2C_CLOCK_STR_SHIFT)
               & NT3H2111_I2C_CLOCK_STR_M);
  }

  return result;
}

/***************************************************************************//**
 *  Decode one byte of raw I2C_CLOCK_STR data.
 ******************************************************************************/
nt3h2111_i2c_clock_str_t nt3h2111_i2c_decode_clock_str(uint8_t reg_value)
{
  nt3h2111_i2c_clock_str_t i2c_clock_str_reg;

  if (reg_value & (BIT1_MASK)) {
    i2c_clock_str_reg.nrg_auth_reached = true;
  }

  if (reg_value & BIT0_MASK) {
    i2c_clock_str_reg.i2c_clock_str = ENABLED;
  } else {
    i2c_clock_str_reg.i2c_clock_str = DISABLED;
  }

  return i2c_clock_str_reg;
}

/***************************************************************************//**
 *  Decode one byte of raw REG_LOCK data.
 ******************************************************************************/
nt3h2111_reg_lock_t nt3h2111_decode_reg_lock(uint8_t reg_value)
{
  nt3h2111_reg_lock_t reg_lock;

  if (reg_value & BIT1_MASK) {
    reg_lock.reg_lock_i2c = NT3H2111_REG_LOCKED;
  } else {
    reg_lock.reg_lock_i2c = NT3H2111_REG_UNLOCKED;
  }
  if (reg_value & BIT0_MASK) {
    reg_lock.reg_lock_nfc = NT3H2111_REG_LOCKED;
  } else {
    reg_lock.reg_lock_nfc = NT3H2111_REG_UNLOCKED;
  }

  return reg_lock;
}
