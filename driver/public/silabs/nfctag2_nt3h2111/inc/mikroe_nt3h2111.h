/***************************************************************************//**
 * @file   mikroe_nt3h2111.h
 * @brief  APIs and types for high level NT3H2111 functionalities.
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

#ifndef _MIKROE_NT3H2111_H_
#define _MIKROE_NT3H2111_H_

#include <stdint.h>
#include <stddef.h>
#include "sl_status.h"
#include "drv_i2c_master.h"
#include "drv_digital_in.h"

#define ENABLED                                          1
#define DISABLED                                         0

#define ON                                               1
#define OFF                                              0

#define NT3H2111__MEM_ADRR_I2C_ADDR                      0
#define NT3H2111_CONFIG_REG_ADDR                         0x3A
#define NT3H2111_SESSION_REG_ADDR                        0xFE
#define NT3H2111_MEM_BLOCK_START_SRAM                    0xF8
#define NT3H2111_MEM_SRAM_BLOCKS                         4

///  memory bit masks
#define NT3H2111_NC_REG_MASK_I2C_RST_ON_OFF              0x80
#define NT3H2111_NC_REG_MASK_PTHRU_ON_OFF                0x40
#define NT3H2111_NC_REG_MASK_FD_OFF                      0x30
#define NT3H2111_NC_REG_MASK_FD_ON                       0x0C
#define NT3H2111_NC_REG_MASK_SRAM_MIRROR_ON_OFF          0x02
#define NT3H2111_NC_REG_MASK_TRANSFER_DIR                0x01

#define NT3H2111_REG_LOCK_MASK_CONF_BYTES_ACCESS_I2C     0x02
#define NT3H2111_REG_LOCK_MASK_CONF_BYTES_ACCESS_RF      0x01

#define NT3H2111_NS_REG_MASK_NDEF_DATA_READ              0x80
#define NT3H2111_NS_REG_MASK_I2C_LOCKED                  0x40
#define NT3H2111_NS_REG_MASK_RF_LOCKED                   0x20
#define NT3H2111_NS_REG_MASK_SRAM_I2C_READY              0x10
#define NT3H2111_NS_REG_MASK_SRAM_RF_READY               0x08
#define NT3H2111_NS_REG_MASK_EEPROM_WR_ERR               0x04
#define NT3H2111_NS_REG_MASK_EEPROM_WR_BUSY              0x02
#define NT3H2111_NS_REG_MASK_RF_FIELD_PRESENT            0x01

#define NT3H2111_BLOCK_SIZE                              16
#define NT3H2111_WRITE_DELAY_MS                          10

#define BIT7_MASK                                        0x80
#define BIT6_MASK                                        0x40
#define BIT5_MASK                                        0x20
#define BIT4_MASK                                        0x10
#define BIT3_MASK                                        0x08
#define BIT2_MASK                                        0x04
#define BIT1_MASK                                        0x02
#define BIT0_MASK                                        0x01

#define NT3H2111_REG_LOCKED                              1
#define NT3H2111_REG_UNLOCKED                            0

#define NT3H2111_NC_REG_NFCS_I2C_RST_ON_OFF_M            BIT7_MASK
#define NT3H2111_NC_REG_NFCS_I2C_RST_ON_OFF_SHIFT        7
#define NT3H2111_NC_REG_PTHRU_ON_OFF_M                   BIT6_MASK
#define NT3H2111_NC_REG_PTHRU_ON_OFF_SHIFT               6
#define NT3H2111_NC_REG_FD_OFF_M                         (BIT5_MASK | BIT4_MASK)
#define NT3H2111_NC_REG_FD_OFF_SHIFT                     4
#define NT3H2111_NC_REG_FD_ON_M                          (BIT3_MASK | BIT2_MASK)
#define NT3H2111_NC_REG_FD_ON_SHIFT                      2
#define NT3H2111_NC_REG_SRAM_MIRROR_ON_OFF_M             BIT1_MASK
#define NT3H2111_NC_REG_SRAM_MIRROR_ON_OFF_SHIFT         1
#define NT3H2111_NC_REG_TRANSFER_DIR_M                   BIT0_MASK
#define NT3H2111_NC_REG_TRANSFER_DIR_SHIFT               0

#define NT3H2111_I2C_CLOCK_STR_M                         BIT0_MASK
#define NT3H2111_I2C_CLOCK_STR_SHIFT                     0

#define TRANSFER_DIR_I2C_TO_NFC                          0
#define TRANSFER_DIR_NFC_TO_I2C                          1

/***************************************************************************//**
 * @addtogroup NT3H2111 Driver
 * @brief NT3H2111 top level APIs and Types.
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

// NT3H2111 configuration register addresses within memory block.
typedef enum {
  CONFIG_NC_REG               = 0,
  CONFIG_LAST_NDEF_BLOCK      = 1,
  CONFIG_SRAM_MIRROR_BLOCK    = 2,
  CONFIG_WDT_LS               = 3,
  CONFIG_WDT_MS               = 4,
  CONFIG_I2C_CLOCK_STR        = 5,
  CONFIG_REG_LOCK             = 6
} nt3h2111_config_reg_addr_t;

// NT3H2111 session register addresses within memory block.
typedef enum {
  SESSION_NC_REG              = 0,
  SESSION_LAST_NDEF_BLOCK     = 1,
  SESSION_SRAM_MIRROR_BLOCK   = 2,
  SESSION_WDT_LS              = 3,
  SESSION_WDT_MS              = 4,
  SESSION_I2C_CLOCK_STR       = 5,
  SESSION_NS_REG              = 6
} nt3h2111_session_reg_addr_t;

/// NC_REG type.
/// Details please refer to NT3H2111_2211 datasheet Table 13 and 14.
typedef struct {
  bool    nfcs_i2c_rst_on_off;
  bool    pthru_on_off;
  uint8_t fd_off;
  uint8_t fd_on;
  bool    sram_mirror_on_off;
  bool    transfer_dir;
} nt3h2111_nc_reg_t;

/// I2C_CLOCK_STR type.
/// Details please refer to NT3H2111_2211 datasheet Table 13 and 14.
typedef struct {
  /* Status bit to show the number of negative PWD_AUTH attempts reached
   *  0b: PWD_AUTH still possible
   *  1b: PWD_AUTH locked
   * Note: this field is only available in session register.
   */
  bool    nrg_auth_reached;

  /* Enables (1b) or disable (0b) the I2C clock stretching. */
  bool    i2c_clock_str;
} nt3h2111_i2c_clock_str_t;

/// REG_LOCK type.
/// Details please refer to NT3H2111_2211 datasheet Table 13 and 14.
typedef struct {
  bool    reg_lock_i2c;
  bool    reg_lock_nfc;
} nt3h2111_reg_lock_t;

/// NS_REG type.
/// Detials please refer to NRAG I2C plus datasheet Table 14.
typedef struct {
  /* 1b: all data bytes read from the address specified in LAST_NDEF_BLOCK.
   * Bit is reset to 0b when read. */
  bool    ndef_data_read;

  /* 1b: Memory access is locked to the I2C interface. */
  bool    i2c_locked;

  /* 1b: Memory access is locked to the NFC interface. */
  bool    rf_locked;

  /* 1b: data is ready in SRAM buffer to be read by I2C. */
  bool    sram_i2c_ready;

  /* 1b: data is ready in SRAM buffer to be read by NFC. */
  bool    sram_rf_ready;

  /* 1b: HV voltage error during EEPROM write or erase cycle
   * Needs to be written back via I2C to 0b to be cleared. */
  bool    eeprom_wr_err;

  /* 1b: EEPROM write cycle in progress - access to EEPROM disabled.
   * 0b: EEPROM access possible. */
  bool    eeprom_wr_busy;

  /* 1b: NFC field is detected. */
  bool    rf_firld_present;
} nt3h2111_i2c_ns_reg_t;

/// Configuration register type.
/// Details please refer to NT3H2111_2211 datasheet Table 13.
typedef struct {
  /* NC_REG. */
  nt3h2111_nc_reg_t               nc_reg;

  /* LAST_NDEF_BLOCK: I2C block address of I2C block,
   * which contains last byte(s) of stored NDEF message. */
  uint8_t                         last_ndef_block;

  /* SRAM_MIRROR_BLOCK: 2C block address of SRAM
   * when mirrored into the User memory. */
  uint8_t                         sram_mirror_block;

  /* WDT_LS: Least Significant byte of watchdog time control register. */
  uint8_t                         wdt_ls;

  /* WDT_MS: Most Significant byte of watchdog time control register.
   * When writing WDT_MS byte, the content of WDT_MS and WDT_LS gets
   * active for the watchdog timer. */
  uint8_t                         wdt_ms;

  /* I2C_CLOCK_STR. */
  nt3h2111_i2c_clock_str_t        i2c_clock_str;

  /* REG_LOCK. */
  nt3h2111_reg_lock_t             reg_lock;
} nt3h2111_config_reg_t;

/// Session register type.
/// Details please refer to NT3H2111_2211 datasheet Table 14.
typedef struct {
  /* NC_REG. */
  nt3h2111_nc_reg_t               nc_reg;

  /* LAST_NDEF_BLOCK: I2C block address of I2C block,
   * which contains last byte(s) of stored NDEF message. */
  uint8_t                         last_ndef_block;

  /* SRAM_MIRROR_BLOCK: 2C block address of SRAM
   * when mirrored into the User memory. */
  uint8_t                         sram_mirror_block;

  /* WDT_LS: Least Significant byte of watchdog time control register. */
  uint8_t                         wdt_ls;

  /* WDT_MS: Most Significant byte of watchdog time control register.
   * When writing WDT_MS byte, the content of WDT_MS and WDT_LS gets
   * active for the watchdog timer. */
  uint8_t                         wdt_ms;

  /* I2C_CLOCK_STR. */
  nt3h2111_i2c_clock_str_t        i2c_clock_str;

  /* REG_LOCK. */
  nt3h2111_i2c_ns_reg_t           ns_reg;
} nt3h2111_session_reg_t;

/***************************************************************************//**
 * @brief
 *   Initialize the NT3H2111 driver with the values provided in the
 *   config file.
 *
 * @param[in] i2cspm
 *   The I2CSPM instance to use.
 *
 * @return
 *   SL_STATUS_OK on success.
 ******************************************************************************/
sl_status_t nt3h2111_init(mikroe_i2c_handle_t i2cspm);

/***************************************************************************//**
 * @brief
 *  De-initialize the NT3H2111.
 *
 * @return
 *   SL_STATUS_OK on success.
 ******************************************************************************/
sl_status_t nt3h2111_deinit(void);

/***************************************************************************//**
 * @brief
 *  Read a configuration register in NT3H2111.
 *
 * @param[in] rega
 *  Register address within memory block
 *
 * @param[out] data
 *  Buffer output
 *
 * @returns
 *  Read result (data and any error code)
 *
 * @note
 *  Details for I2C configuration registers. Please refer to NT3H2111_2211
 *  product data sheet section 8.3.12.
 *
 ******************************************************************************/
sl_status_t nt3h2111_get_config(nt3h2111_config_reg_addr_t rega, uint8_t *data);

/***************************************************************************//**
 * @brief
 *  Write to a configuration register in NT3H2111.
 *
 * @param[in] rega
 *  Register address within memory block
 *
 * @param[in] regd
 *  Register data to be written
 *
 * @returns
 *  Any error code
 *
 * @note
 *  Details for I2C configuration registers. Please refer to NT3H2111_2211
 *  product data sheet section 8.3.12.
 *
 ******************************************************************************/
sl_status_t nt3h2111_set_config(nt3h2111_config_reg_addr_t rega,
                                uint8_t mask,
                                uint8_t regd);

/**************************************************************************//**
 * @brief
 *  Read a session register in NT3H2111.
 *
 * @param[in] rega
 *  Register address within memory block
 *
 * @param[out] data
 *  Buffer output
 *
 * @returns
 *  Read result (data and any error code)
 *
 * @note
 *  Details for I2C session registers. Please refer to NT3H2111_2211
 *  product data sheet section 8.3.12.
 *
 *****************************************************************************/
sl_status_t nt3h2111_get_session(nt3h2111_session_reg_addr_t rega,
                                 uint8_t *data);

/**************************************************************************//**
 * @brief
 *  Write to a session register in NT3H2111.
 *
 * @param[in] rega
 *  Register address within memory block
 *
 * @param[in] regd
 *  Register data to be written
 *
 * @returns
 *  Any error code
 *
 * @note
 *  Details for I2C session registers. Please refer to NT3H2111_2211
 *  product data sheet section 8.3.12.
 *
 *****************************************************************************/
sl_status_t nt3h2111_set_session(nt3h2111_session_reg_addr_t rega,
                                 uint8_t mask,
                                 uint8_t regd);

/**************************************************************************//**
 * @brief
 *  Read a memory block (16 bytes) from NT3H2111.
 *
 * @param[in] mema
 *  The address of the block of memory (SRAM or EEPROM)
 *  that is intended to be read
 *
 * @param[out] data
 *  Data buffer to hold the result
 *
 * @returns
 *  I2C transfer status.
 *
 * @note
 *  Details for I2C READ operation, please refer to NT3H2111_2211 product
 *  data sheet section 9.7.
 *****************************************************************************/
sl_status_t nt3h2111_read_block(uint8_t mema, uint8_t *data);

/**************************************************************************//**
 * @brief
 *   Write a memory block (16 bytes) to NT3H2111.
 *
 * @param[in] mema
 *  The address of the block of memory (SRAM or EEPROM)
 *  that is intended to be written
 *
 * @param[in] data
 *  Data to be written
 *
 * @returns
 *  I2C transfer status.
 *
 * @note
 *  Details for I2C WRITE operation, please refer to NT3H2111_2211 product
 *  data sheet section 9.7.
 *****************************************************************************/
sl_status_t nt3h2111_write_block(uint8_t mema, const uint8_t *data);

/***************************************************************************//**
 * @brief Write byte(s) to the selected device
 *
 *  This functions writes the specified number of bytes to the given address of
 *  the selected device. Writing to an invalid address is undefined.
 *
 * @param[in] addr
 *  The address of the byte of memory (SRAM or EEPROM)
 *  that is intended to be written
 *
 * @param[in] bytes
 *  Array of bytes to be written
 *
 * @param[in] len
 *  Number of bytes to be written
 *
 ******************************************************************************/
sl_status_t nt3h2111_write_bytes(uint8_t addr,
                                 const uint8_t *bytes,
                                 uint16_t len);

/***************************************************************************//**
 * @brief Read byte(s) from the selected device
 *
 *  This functions reads the specified number of bytes to the given address of
 *  the selected device. Reading from an invalid address
 *  will result in a failed read.
 *
 * @param[in] addr
 *  The address of the byte of memory (SRAM or EEPROM)
 *  that is intended to be read
 *
 * @param[in] bytes
 *  Array of bytes to store read data
 *
 * @param[in] len
 *  Number of bytes to be read
 *
 ******************************************************************************/
sl_status_t nt3h2111_read_bytes(uint8_t addr, uint8_t *bytes, uint16_t len);

/**************************************************************************//**
 * @brief
 *  Encode NC_REG content to one byte of data.
 *
 * @param[in] nc_reg
 *  Content of NC_REG to be encoded
 *
 * @returns
 *  Encoded data
 *
 * @note
 *  Details for NC_REG. Please refer to NT3H2111_2211 product data sheet
 *  section 8.3.12.
 *
 * @warning
 *  This function is provided as prototype and has not been tested.
 *****************************************************************************/
uint8_t nt3h2111_encode_nc_reg(nt3h2111_nc_reg_t nc_reg);

/**************************************************************************//**
 * @brief
 *  Decode NC_REG content from one byte of raw data.
 *
 * @param[in] nc_reg
 *  One byte of raw NC_REG data to be decoded
 *
 * @returns
 *  Decoded NC_REG data
 *
 * @note
 *  Details for NC_REG. Please refer to NT3H2111_2211 product data sheet
 *  section 8.3.12.
 *
 * @warning
 *  This function is provided as prototype and has not been tested.
 *****************************************************************************/
nt3h2111_nc_reg_t nt3h2111_decode_nc_reg(uint8_t reg_value);

/**************************************************************************//**
 * @brief
 *  Encode I2C_CLOCK_STR content to one byte of data.
 *
 * @param[in] i2c_clock_str_reg
 *  Content of I2C_CLOCK_STR to be encoded
 *
 * @returns
 *  Encoded data
 *
 * @note
 *  Details for I2C_CLOCK_STR. Please refer to NT3H2111_2211 product data sheet
 *  section 8.3.12.
 *
 * @warning
 *  This function is provided as prototype and has not been tested.
 *****************************************************************************/
uint8_t nt3h2111_i2c_encode_clock_str(
  nt3h2111_i2c_clock_str_t i2c_clock_str_reg);

/***************************************************************************//**
 * @brief
 *  Decode one byte of raw I2C_CLOCK_STR data.
 *
 * @param[in] i2c_clock_str_reg
 *  Raw I2C_CLOCK_STR data to be decoded
 *
 * @returns
 *  Decoded I2C_CLOCK_STR data
 *
 * @note
 *  Details for I2C_CLOCK_STR. Please refer to NT3H2111_2211 product data sheet
 *  section 8.3.12.
 *
 * @warning
 *  This function is provided as prototype and has not been tested.
 ******************************************************************************/
nt3h2111_i2c_clock_str_t nt3h2111_i2c_decode_clock_str(uint8_t reg_value);

/***************************************************************************//**
 * @brief
 *  Decode one byte of raw REG_LOCK data.
 *
 * @param[in] i2c_clock_str_reg
 *  Raw REG_LOCK data to be decoded
 *
 * @returns
 *  Decoded REG_LOCK data
 *
 * @note
 *  Details for REG_LOCK. Please refer to NT3H2111_2211 product data sheet
 *  section 8.3.12.
 *
 ******************************************************************************/
nt3h2111_reg_lock_t nt3h2111_decode_reg_lock(uint8_t reg_value);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup NT3H2111 Driver) */
#endif // _MIKROE_NT3H2111_H_
