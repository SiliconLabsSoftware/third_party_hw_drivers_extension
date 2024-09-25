/***************************************************************************//**
 * @file gnss_max_m10s_driver.h
 * @brief MAX_M10S GNSS receiver API implementation
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef GNSS_MAX_M10S_H_
#define GNSS_MAX_M10S_H_

#ifdef __cplusplus
extern "C" {
#endif

/// Includes
#include "sl_status.h"
#include "gnss_max_m10s_ubx_struct.h"
#include "gnss_max_m10s_nmea_struct.h"
#include "drv_i2c_master.h"

/******************************************************************************/

/*******************************************************************************
 ***************************  Defines / Macros  ********************************
 ******************************************************************************/
#define GNSS_ADDRESS               0x42                   ///< Default GNSS receiver slave address
#define GNSS_POLL_MAX_TIMEOUT      1100                   ///< Default maximum timeout value
#define RX_LEN                     2                      ///< Read buffer length 2 bytes
#define TX_LEN                     2                      ///< Write buffer length 2 bytes
#define RD_BUF                     6                      ///< Read buffer length 6 bytes
#define WR_BUF                     1                      ///< Write buffer length 1 byte
#define MS_DELAY_COUNTER           4600                   ///< Value of delay counter for milli seconds delay
#define MAX_PAYLOAD_SIZE           276                    ///< Value of maximum payload size for UBX
#define UBX_NAV_PVT_LEN            92                     ///< Value for position/velocity/time type UBX packet length
#define UBX_NAV_STATUS_LEN         16                     ///< Value for navigation status type UBX packet length
#define UBX_NAV_DOP_LEN            18                     ///< Value for dilution precision type UBX packet length
#define UBX_NAV_CLOCK_LEN          20                     ///< Value for navigation clock solution type UBX packet length
#define UBX_NAV_EOE_LEN            4                      ///< Value for navigation end of epoch type UBX packet length
#define UBX_NAV_SAT_LEN            3068                   ///< Value for the maximum possible length of the UBX-NAV-SAT message
#define UBX_NAV_TIMELS_LEN         24                     ///< Value for length of the UBX-NAV-TIMELS message
#define UBX_NAV_TIMEUTC_LEN        20                     ///< Value for length of the UBX-NAV-TIMEUTC message
#define UBX_NAV_SIG_MAX_LEN        1480                   ///< Value for maximum length for UBX-NAV-SIG packet
#define UBX_NAV_POSLLH_LEN         28                     ///< Value for length for UBX-NAV-POSLLH packet
#define UBX_NAV_VELNED_LEN         36                     ///< Value for length for UBX-NAV-VELNED packet
#define UBX_CLASS_NAV              0x01                   ///< Value for NAV type UBX class
#define UBX_CLASS_SEC              0x27                   ///< Value for SEC type UBX class
#define UBX_CLASS_ACK              0x05                   ///< Value for ACK type UBX class
#define UBX_CLASS_NACK             0x00                   ///< Value for NACK type UBX class
#define UBX_MAX_LENGTH             3068                   ///< Value for maximum UBX data length
#define UBX_ACK_ACK                0x01                   ///< Value for message ID for ACK class
#define CFG_VALSET_MAX_KEYS        64                     ///< Value for maximum number of keys for cfg valset type packet
#define UBX_CFG_L                  0x01001000             ///< Value for bool type identification of UBX data.
#define UBLOX_CFG_I2C_INPORT_UBX   UBX_CFG_L | 0x10710001 ///< Value to indicate if UBX should be an input protocol on I2C
#define UBLOX_CFG_I2C_OUTPORT_UBX  UBX_CFG_L | 0x10720001 ///< Value to indicate if UBX should be an output protocol on I2C
#define UBLOX_CFG_I2C_OUTPORT_NMEA UBX_CFG_L | 0x10720002  ///< Value to indicate if NMEA should be an output protocol on I2C
#define VAL_LAYER_RAM              1                      ///< Value for RAM layer
#define UBX_SYNCH_1                0xB5                   ///< Value for UBX header synchronization 1
#define UBX_SYNCH_2                0x62                   ///< Value for UBX header synchronization 2
#define UBX_CLASS_CFG              0x06                   ///< Value for CFG type UBX class
#define UBX_CFG_VALGET             0x8B                   ///< Value for message ID for CFG type UBX data
#define UBX_CFG_VALSET             0x8A                   ///< Value for message ID for CFG VALSET type UBX data
#define VAL_LAYER_DEFAULT          0x7                    ///< Value for default layer
#define VAL_LAYER_RAM_BBR          5                      ///< Value for layer RAM and BBR
#define VAL_LAYER_BBR              2                      ///< Value for BBR layer
#define VAL_LAYER_FLASH            4                      ///< Value for Flash layer
#define COM_TYPE_UBX               1                      ///< Value for UBX type communication
#define COM_TYPE_NMEA              2                      ///< Value for UBX type communication
#define UBX_CFG_SIZE_MASK          0x0F00F000             ///< Value for mask of UBX size
#define UBX_SEC_UNIQID             0x03                   ///< Value for message ID of UBX_SEC_UINQID type UBX message
#define UBX_NAV_PVT                0x07                   ///< Value for message ID of UBX_NAV_PVT type UBX message
#define UBX_NAV_STATUS             0x03                   ///< Value for message ID of UBX_NAV_STATUS type UBX message
#define UBX_NAV_DOP                0x04                   ///< Value for message ID of UBX_NAV_DOP type UBX message
#define UBX_NAV_CLOCK              0x22                   ///< Value for message ID of UBX_NAV_CLOCK type UBX message
#define UBX_NAV_EOE                0x61                   ///< Value for message ID of UBX_NAV_EOE type UBX message
#define UBX_NAV_SAT                0x35                   ///< Value for message ID of UBX_NAV_SAT type UBX message
#define UBX_NAV_SIG                0x43                   ///< Value for message ID of UBX_NAV_SIG type UBX message
#define UBX_NAV_TIMELS             0x26                   ///< Value for message ID of UBX_NAV_TIMELS type UBX message
#define UBX_NAV_TIMEUTC            0x21                   ///< Value for message ID of UBX_NAV_TIMEUTC type UBX message
#define UBX_NAV_POSLLH             0x2                    ///< Value for message ID of UBX_NAV_POSLLH type UBX message
#define UBX_NAV_VELNED             0x12                   ///< Value for message ID of UBX_NAV_VELNED type UBX message
#define FILE_BUFFER_SIZE           MAX_PAYLOAD_SIZE       ///< Value for the UBX buffer size
#define I2C_TRANSACTION_SIZE       32                     ///< Value for I2C transaction size
#define I2C_POLLING_WAIT           100                    ///< Value to poll the I2C data
#define NMEA_MAX_BYTE_COUNT        100                    ///< Value for default maximum NMEA byte count

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/
/// @brief Enum for ublox status
typedef enum sl_max_m10s_ublox_status_e
{
  SL_MAX_M10S_UBLOX_STATUS_SUCCESS=0,            ///< To indicate success
  SL_MAX_M10S_UBLOX_STATUS_FAIL,                 ///< To indicate failure
  SL_MAX_M10S_UBLOX_STATUS_CRC_FAIL,             ///< To indicate failure in CRC of data received
  SL_MAX_M10S_UBLOX_STATUS_TIMEOUT,              ///< To indicate timeout for a process
  SL_MAX_M10S_UBLOX_STATUS_COMMAND_NACK,         ///< To indicate that the command was un-recognised, invalid or that the module is too busy to respond
  SL_MAX_M10S_UBLOX_STATUS_OUT_OF_RANGE,         ///< To indicate the received data is out of range
  SL_MAX_M10S_UBLOX_STATUS_INVALID_ARG,          ///< To indicate invalid argument
  SL_MAX_M10S_UBLOX_STATUS_INVALID_OPERATION,    ///< To indicate invalid operation
  SL_MAX_M10S_UBLOX_STATUS_MEM_ERR,              ///< To indicate memory error
  SL_MAX_M10S_UBLOX_STATUS_HW_ERR,               ///< To indicate hardware error
  SL_MAX_M10S_UBLOX_STATUS_DATA_SENT,            ///< To indicate that a data was set successfully
  SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED,        ///< To indicate that a data was received successfully
  SL_MAX_M10S_UBLOX_STATUS_I2C_COMM_FAILURE,     ///< To indicate I2C communication error
  SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN      ///< To indicate data has or is being overwritten by another packet
} sl_max_m10s_ublox_status_e;

/// @brief Enum for UBLOX packet validity
typedef enum
{
  SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_VALID,   ///< To indicate that the ublox packet validity is not achieved
  SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID,       ///< To indicate that the ublox packet validity is achieved
  SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED, ///< To indicate that the ublox packet validity is not defined
  SL_MAX_M10S_UBLOX_PACKET_NOTACKNOWLEDGED       ///< To indicate that we received a not acknowledged packet
} sl_max_m10s_ublox_packet_validity_e;

/// @brief Enum to identify which ubx packet buffer is in use.
typedef enum
{
  SL_MAX_M10S_UBLOX_PACKET_PACKETCFG,            ///< To indicate that the packet is config type UBX packet
  SL_MAX_M10S_UBLOX_PACKET_PACKETACK,            ///< To indicate that the packet is acknowledge type UBX packet
  SL_MAX_M10S_UBLOX_PACKET_PACKETBUF,            ///< To indicate that the packet is buffer for UBX packet
  SL_MAX_M10S_UBLOX_PACKET_PACKETAUTO            ///< To indicate that the packet will be handle automatically
} sl_max_m10s_ublox_packet_buffer_e;

/// @brief Enum for different UBLOX sentence type.
typedef enum
{
  SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE = 0,     ///< To indicate the UBLOX sentence type is unknown or not a UBlOX sentence
  SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NMEA,         ///< To indicate the UBLOX sentence type is NMEA
  SL_MAX_M10S_UBLOX_SENTENCE_TYPE_UBX,          ///< To indicate the UBLOX sentence type is UBX
  SL_MAX_M10S_UBLOX_SENTENCE_TYPE_RTCM          ///< To indicate the UBLOX sentence type is RTCM
} sl_max_m10s_ublox_sentence_types_e;

/// @brief Enum for status of the gnss processes like begin, ping etc.
typedef enum sl_max_m10s_gnss_status
{
  SL_MAX_M10S_BEGIN_OK,                          ///< status for begin is success
  SL_MAX_M10S_BEGIN_FAIL,                        ///< status for begin is failure
  SL_MAX_M10S_PING_OK,                           ///< status for ping is ok
  SL_MAX_M10S_PING_FAIL,                         ///< status for begin is failure
  SL_MAX_M10S_INIT_OK,                           ///< status ok for init
  SL_MAX_M10S_INIT_FAIL,                         ///< status for init is failure
  SL_MAX_M10S_UBLOX_INIT_OK,                     ///< status for ublox init is success
  SL_MAX_M10S_UBLOX_INIT_FAIL,                   ///< status for ublox init is failure
} sl_max_m10s_gnss_status_e;

/// @brief to define which protocol is used to get data from receiver.
typedef enum sl_max_m10s_protocol_type
{
  SL_MAX_M10S_PROTOCOL_UBX = 0,
  SL_MAX_M10S_PROTOCOL_NMEA,
}sl_max_m10s_protocol_type_e;

/// @brief to store the signed or unsigned 8 bit data from the payload when extracting data from the UBX packet.
typedef union
{
  uint32_t unsigned8;                           ///< To store unsigned 32 bit value
  int32_t signed8;                              ///< To store signed 32 bit value
} sl_max_m10s_unsigned_signed_8_t;

/// @brief to store the signed or unsigned 32 bit data from the payload when extracting data from the UBX packet.
typedef union
{
  uint32_t unsigned32;                           ///< To store unsigned 32 bit value
  int32_t signed32;                              ///< To store signed 32 bit value
} sl_max_m10s_unsigned_signed_32_t;

/// @brief to store the signed or unsigned 16 bit data from the payload when extracting data from the UBX packet.
typedef union
{
  uint16_t unsigned16;                           ///< To store unsigned 16 bit value
  int16_t signed16;                              ///< To store signed 16 bit value
} sl_max_m10s_unsigned_signed_16_t;

/// @brief UBX binary specific variables
typedef struct sl_max_m10s_ubx_packet
{
  uint8_t cls;                                             ///< class of the UBX packet.
  uint8_t id;                                              ///< ID for the UBX packet.
  uint16_t len;                                            ///< Length of the payload. Does not include cls, id, or checksum bytes
  uint16_t counter;                                        ///< Keeps track of number of overall bytes received. Some responses are larger than 255 bytes.
  uint16_t starting_spot;                                  ///< The counter value needed to go past before we begin recording into payload array
  uint8_t *payload;                                        ///< Used to allocate RAM for the payload if/when needed.
  uint8_t checksum_a;                                      ///< Checksum value given from the module
  uint8_t checksum_b;                                      ///< Checksum value given from the module
  sl_max_m10s_ublox_packet_validity_e valid;               ///< Goes from NOT_DEFINED to VALID or NOT_VALID when checksum is checked
  sl_max_m10s_ublox_packet_validity_e class_and_id_match;  ///< Goes from NOT_DEFINED to VALID or NOT_VALID when the Class and ID match the requestedClass and requestedID
}sl_max_m10s_ubx_packet_t;

/// @brief to store the different types of pay load from UBX packets.
typedef struct sl_max_m10s_msg_data
{
  uint8_t payload_ack[2];                                         ///< To store Acknowledge message payload
  uint8_t payload_buf[2];                                         ///< To store buffer payload
  size_t packet_cfg_payloadsize;                                  ///< Value for CFG type packet's payload size
  uint8_t *payload_cfg;                                           ///< To store the CFG type payload
  uint8_t *payload_auto;                                          ///< To store the automatic packet payload
}sl_max_m10s_msg_data_t;

/// @brief to store the GNSS configuration data
typedef struct sl_max_m10s_cfg_data
{
  uint8_t            device_address;                               ///< gnss receiver slave address
  uint8_t            num_cfg_keys;                                 ///< Number of CFG keys
  uint8_t            rolling_checksum_a;                           ///< Rolling checksum A for UBX frame
  uint8_t            rolling_checksum_b;                           ///< Rolling checksum B for UBX frame
  int8_t             nmea_byte_counter;                            ///< Counter for NMEA type data
  int8_t             max_nmea_byte_count;                          ///< Maximum number of NMEA byte
  bool               signs_of_life;                                ///< Flag to indicate if the process is ongoing
  bool               reset_current_sentence_on_bus_error;          ///< Flag to indicate if there is a bus error for current UBLOX sentence
  bool               ignore_this_payload;                          ///< Flag to indicate the current sentence payload should be ignored
  uint16_t           rtcm_frame_counter;                           ///< Counter for RTCM data
  uint16_t           file_buffer_head;                             ///< Value for starting of file buffer
  uint16_t           file_buffer_tail;                             ///< Value for ending of file buffer
  uint16_t           file_buffer_max_avail;                        ///< Value for maximum available size in file buffer
  uint16_t           ubx_frame_counter;                            ///< Counter for UBX frame data
  sl_max_m10s_gnss_status_e status;                                ///< Status of the driver function
  sl_max_m10s_protocol_type_e protocol_type;                       ///< To check the protocol type
  sl_max_m10s_ublox_sentence_types_e current_sentence;             ///< To show the current sentence type
  sl_max_m10s_ublox_packet_buffer_e active_packet_buffer;          ///< Active packet type
  mikroe_i2c_handle_t i2c_instance;                               ///< I2C instance used by the user
  uint8_t *ubx_file_buffer;                                        ///< To store the file buffer
  size_t             auto_send_at_space_remaining;                 ///< automatic send to the remaining space of the file buffer
  sl_max_m10s_ubx_nav_pvt_t *packetUBXNAVPVT;                      ///< UBX_NAV_PVT type UBX packet
  sl_max_m10s_ubx_nav_status_t *packetUBXNAVSTATUS;                ///< UBX_NAV_STATUS type UBX packet
  sl_max_m10s_ubx_nav_dop_t *packetUBXNAVDOP;                      ///< UBX_NAV_DOP type UBX packet
  sl_max_m10s_ubx_nav_clock_t *packetUBXNAVCLOCK;                  ///< UBX_NAV_CLOCK type UBX packet
  sl_max_m10s_ubx_nav_epoch_t *packetUBXNAVEOE;                    ///< UBX_NAV_EOE type UBX packet
  sl_max_m10s_ubx_nav_sat_t *packetUBXNAVSAT;                      ///< UBX_NAV_SAT type UBX packet
  sl_max_m10s_ubx_navsig_t *packetUBXNAVSIG;                       ///< UBX_NAV_SIG type UBX packet
  sl_max_m10s_ubx_nav_timels_t *packetUBXNAVTIMELS;                ///< UBX_NAV_TIMELS type UBX packet
  sl_max_m10s_ubx_nav_timeutc_t *packetUBXNAVTIMEUTC;              ///< UBX_NAV_TIMEUTC type UBX packet
  sl_max_m10s_ubx_nav_posllh_t *packetUBXNAVPOSLLH;                ///< UBX_NAV_POSLLH type UBX packet
  sl_max_m10s_ubx_nav_velned_t *packetUBXNAVVELNED;                ///< UBX_NAV_VELNED UBX packet
  sl_max_m10s_ubx_unique_id_t *packetUBXUNIQID;                    ///< UBX_SEC_UNIQID UBX packet
  sl_max_m10s_nmea_gga_t *storageNMEAGPGGA;                        ///< GPGGA type NMEA sentence
  sl_max_m10s_nmea_gga_t *storageNMEAGNGGA;                        ///< GNGGA type NMEA sentence
  sl_max_m10s_nmea_rmc_t *storageNMEAGPRMC;                        ///< GPRMC type NMEA sentence
  sl_max_m10s_nmea_rmc_t *storageNMEAGNRMC;                        ///< GNRMC type NMEA sentence
  sl_max_m10s_nmea_data_t *nmea_data;                              ///< Data from the both NMEA sentences
  sl_max_m10s_nmea_filtering_t processing_nmea;                    ///< Type of sentences to be processed
  sl_max_m10s_msg_data_t msg_data;                                 ///< structure for payload of UBX message
  sl_max_m10s_ubx_packet_t packet_ack;                             ///< structure for UBX acknowledge type message
  sl_max_m10s_ubx_packet_t packet_buf;                             ///< structure to store the UBX message as buffer
  sl_max_m10s_ubx_packet_t packet_auto;                            ///< structure to store automatic type UBX message
  sl_max_m10s_ubx_packet_t packet_cfg;                             ///< structure to store config type UBX message
  char                nmea_buffer[100];                            ///< Buffer to store NMEA sentence.
  uint8_t            nmea_address_field[6];                        ///< NMEA address field
} sl_max_m10s_cfg_data_t;

// -----------------------------------------------------------------------------
// Prototypes

/**************************************************************************//**
 * @brief De-Initialize the gnss_cfg_data structure members
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_deinit(sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief Initialization for the GNSS receiver module.
 * @param[in] max_wait : Timeout value.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note User must set GNSS configuration structure member gnss_cfg_data.protocol_type
 * before calling this function.
 *****************************************************************************/
sl_status_t gnss_max_m10s_begin(uint16_t max_wait,
                                sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief Called regularly to check for available bytes on the user specified port
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] incomingUBX : pointer to incoming UBX packet.
 * @param[in] requested_class : class for the UBX packet.
 * @param[in] requested_id : ID for the UBX packet.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
sl_status_t gnss_max_m10s_check_ublox_internal(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  sl_max_m10s_ubx_packet_t *incomingUBX,
  uint8_t requested_class,
  uint8_t requested_id);

/**************************************************************************//**
 * @brief sends the UBX packet via I2C.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[in] expect_ack_only : to expect only acknowledged data.
 * @return the following values are returned:
 * - \ref SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED if we got a config packet full of response data that has CLS/ID match to our query packet
 * - \ref SL_MAX_M10S_UBLOX_STATUS_CRC_FAIL if we got a corrupt config packet that has CLS/ID match to our query packet
 * - \ref SL_MAX_M10S_UBLOX_STATUS_TIMEOUT if we timed out
 * - \ref SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN if we got an a valid packetCfg but that the packetCfg has been
 *                                          or is currently being overwritten.
 *****************************************************************************/
sl_max_m10s_ublox_status_e gnss_max_m10s_send_command(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool expect_ack_only);

/**************************************************************************//**
 * @brief This is used when receiving messages from module.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] incoming : a byte to add to the rolling checksum.
 *****************************************************************************/
void gnss_max_m10s_add_to_checksum(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                   uint8_t incoming);

/**************************************************************************//**
 * @brief Check if we have storage allocated for an incoming "automatic" message
 *        and calculate how much RAM is needed to store the payload for a given automatic message
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_size : pointer to maximum size.
 * @return true/false.
 *****************************************************************************/
bool gnss_max_m10s_auto_lookup(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                               uint16_t *max_size);

/**************************************************************************//**
 * @brief Add a UBX packet to the file buffer
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] msg : pointer to UBX packet.
 * @return true if packet is stored properly else returns false.
 *****************************************************************************/
bool gnss_max_m10s_store_packet(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief to set output port for UBX data only.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] com_settings : communication type
 * @param[in] max_wait : Timeout value.
 * @param[in] layer : RAM layer.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note User must call this function after calling begin function
 *      to set the output port as:
 *      1. COM_TYPE_UBX: for UBX protocol.
 *      2. COM_TYPE_NMEA: for NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_set_i2c_output(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint8_t com_settings,
                                         uint8_t layer,
                                         uint16_t max_wait);

/**************************************************************************//**
 * @brief Function to get delay in millisecond
 * @param[in] period : delay in milliseconds.
 *****************************************************************************/
void gnss_max_m10s_delay(uint32_t period);

/**************************************************************************//**
 * @brief Get the current fix type
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] fix_type : pointer to store the fix_type data from UBX_NAV_PVT packet.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_fix_type(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait,
                                       uint8_t *fix_type);

/**************************************************************************//**
 * @brief Get the number of satellites.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] satellite : pointer to store the number of satellite data from UBX_NAV_PVT packet.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_satellite_number(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  uint8_t *satellite);

/**************************************************************************//**
 * @brief Get the current longitude in degrees
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] longitude : pointer to store the current longitude from UBX_NAV_PVT packet.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_longitude(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait,
                                        int32_t *longitude);

/**************************************************************************//**
 * @brief Get the current latitude in degrees
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] latitude : pointer to store the current latitude from UBX_NAV_PVT packet.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_latitude(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait,
                                       int32_t *latitude);

/**************************************************************************//**
 * @brief Get the current altitude in mm according to mean sea level
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] altitude_msl : pointer to store the altitude from mean sea level from UBX_NAV_PVT packet.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_altitude_msl(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *altitude_msl);

/**************************************************************************//**
 * @brief Get the current day
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] day : pointer to store the current day from UBX_NAV_PVT packet
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_day(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                  uint16_t max_wait,
                                  uint8_t *day);

/**************************************************************************//**
 * @brief Get the current month
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] month : pointer to store the current month from UBX_NAV_PVT packet
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_month(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    uint16_t max_wait,
                                    uint8_t *month);

/**************************************************************************//**
 * @brief Get the current year
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] year : pointer to store the current year from UBX_NAV_PVT packet
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_year(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                   uint16_t max_wait,
                                   uint16_t *year);

/**************************************************************************//**
 * @brief Get the hour of time in UTC format.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] hour : pointer to store the current hour in UTC from UBX_NAV_PVT packet
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_hour(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                   uint16_t max_wait,
                                   uint8_t *hour);

/**************************************************************************//**
 * @brief Get the minute of time in UTC format.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] minute : pointer to store the current minute in UTC from UBX_NAV_PVT packet
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_minute(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait,
                                     uint8_t *minute);

/**************************************************************************//**
 * @brief Get the seconds of time in UTC format.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] seconds : pointer to store the current seconds in UTC from UBX_NAV_PVT packet
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX and NMEA protocol.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_seconds(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                      uint16_t max_wait,
                                      uint8_t *seconds);

/**************************************************************************//**
 * @brief Get the milliseconds of time from time of the week data of UBX packet.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] milli_seconds : pointer to store the current millisecond in UTC from UBX_NAV_PVT packet
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_milliseconds(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  uint16_t *milli_seconds);

/**************************************************************************//**
 * @brief Get the nanoseconds of time in UTC format.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] nano : pointer to store the current nanoseconds from UBX_NAV_PVT packet
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_nanoseconds(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                          uint16_t max_wait,
                                          int32_t *nano);

/**************************************************************************//**
 * @brief validate the date from the UBX packet.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] valid_date : to check the date validity from UBX_NAV_PVT packet
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_valid_date(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait,
                                     bool *valid_date);

/**************************************************************************//**
 * @brief get the geoid separation from GGA type NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] geoid : to store the geoid separation in mm.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with NMEA protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_geoid(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    uint16_t max_wait,
                                    int32_t *geoid);

/**************************************************************************//**
 * @brief get the horizontal dilution of precision from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] hdop : to store the horizontal dilution of precision.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with NMEA protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_horizontal_dilution(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  uint8_t *hdop);

/**************************************************************************//**
 * @brief get the course over ground from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] course : to store the course over ground in degrees.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with NMEA protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_course_over_ground(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *course);

/**************************************************************************//**
 * @brief get the navigation system in use from NMEA sentence.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] nav_system : character representing navigation system in use.
 * @details `N` = GNSS, `P` = GPS, `L` = GLONASS, `A` = Galileo, `\0` = none.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with NMEA protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navigation_system(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  char *nav_system);

/**************************************************************************//**
 * @brief validate the time from the UBX packet.
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] valid_time : to check the time from UBX_NAV_PVT packet
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_valid_time(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait,
                                     bool *valid_time);

/**************************************************************************//**
 * @brief Get the current altitude in mm according to ellipsoid model
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] altitude : pointer to store height from ellipsoid.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_altitude(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait,
                                       int32_t *altitude);

/**************************************************************************//**
 * @brief Get the carrier phase range solution status
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] carrier_solution : pointer to store carrier phase range solution.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_carrier_solution(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int8_t *carrier_solution);

/**************************************************************************//**
 * @brief Get whether differential corrections were applied
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] diff_solution : pointer to store the differential correctiondata.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_differential_solution(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *diff_solution);

/**************************************************************************//**
 * @brief check the heading of vehicle
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] head_vehicle_valid : pointer to store the heading of vehicle check data.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_head_vehicle_valid(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *head_vehicle_valid);

/**************************************************************************//**
 * @brief Get heading of vehicle (2-D) in degree
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] head_vehicle : pointer to store the heading of vehicle data.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_head_vehicle(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *head_vehicle);

/**************************************************************************//**
 * @brief Get whether we have a valid fix
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] gnss_fix_ok : pointer to store the valid fix data.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_gnss_fix_ok(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                          uint16_t max_wait,
                                          bool *gnss_fix_ok);

/**************************************************************************//**
 * @brief get the Magnetic declination accuracy
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] magnetic_accuracy : pointer to store the magnetic declination accuracy data from the UBX packet.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_magnetic_declination_accuracy(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  uint16_t *magnetic_accuracy);

/**************************************************************************//**
 * @brief get the Magnetic declination
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] magnetic_declination : pointer to store the magnetic declination data from the UBX packet.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_magnetic_declination(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int16_t *magnetic_declination);

/**************************************************************************//**
 * @brief Check to see if the UTC time has been fully resolved
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] fully_resolved : pointer pointer to store the validity of time and date from the UBX packet.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_fully_resolved(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *fully_resolved);

/**************************************************************************//**
 * @brief get the data to check the invalid latitude, longitude, height from mean sea level and height from ellipsoid
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] invalid_data : pointer to store invalid data.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_invalid_data(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *invalid_data);

/**************************************************************************//**
 * @brief Get the time of week data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] time_of_week : pointer to store the time of week data.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_time_of_week(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  uint32_t *time_of_week);

/**************************************************************************//**
 * @brief Get the horizontal accuracy estimate in mm
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] horizontal_accuracy : pointer to store the horizontal accuracy estimate.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_horizontal_accuracy(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *horizontal_accuracy);

/**************************************************************************//**
 * @brief Get the vertical accuracy estimate in mm
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] vertical_accuracy : pointer to store the vertical accuracy estimate.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_vertical_accuracy(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *vertical_accuracy);

/**************************************************************************//**
 * @brief Get the NED north velocity in mm/s
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] ned_north : pointer to store the NED north velocity.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_ned_north(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait,
                                        int32_t *ned_north);

/**************************************************************************//**
 * @brief Get the NED east velocity in mm/s
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] ned_east : pointer to store the NED east velocity.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_ned_east(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait,
                                       int32_t *ned_east);

/**************************************************************************//**
 * @brief Get the NED down velocity in mm/s
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] ned_down : pointer to store the NED down velocity.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_ned_down(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait,
                                       int32_t *ned_down);

/**************************************************************************//**
 * @brief Get the heading of motion (as opposed to heading of car) in degrees * 10^-5.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] head_of_motion : pointer to store the heading of motion.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_heading_of_motion(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *head_of_motion);

/**************************************************************************//**
 * @brief Get the ground speed in mm/s.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] ground_speed : pointer to store the ground speed in mm/s
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note 1. Can be used with UBX and NMEA protocol.
 *       2. In NMEA protocol the unit for speed is in knots.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_ground_speed(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *ground_speed);

/**************************************************************************//**
 * @brief Get the heading accuracy estimate (both motion and vehicle) in degree.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] heading_accuracy : pointer to store the heading accuracy estimate (both motion and vehicle) in degree
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_heading_accuracy(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *heading_accuracy);

/**************************************************************************//**
 * @brief to check the confirm time is available or not.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] confirm_time : pointer to store the confirm time check data.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_confirm_time(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *confirm_time);

/**************************************************************************//**
 * @brief to check the confirm date is available or not.
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] confirm_date : pointer to store the confirm date check data.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_confirm_date(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *confirm_date);

/**************************************************************************//**
 * @brief get the speed accuracy estimate of the space vehicle
 * @param[in] gnss_cfg_data : pointer to the GNSS config data.
 * @param[in] max_wait : Timeout value.
 * @param[out] speed_accuracy : pointer pointer to store the speed accuracy estimate.
 * @return the following values are returned:
 * - \ref SL_STATUS_OK for success.
 * - \ref SL_STATUS_FAIL for failure.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_speed_accuracy(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *speed_accuracy);

/**************************************************************************//**
 * @brief Get the UBX-NAV-PVT type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1.To access the any data use: gnss_cfg_data.packetUBXNAVPVT.data.data_to_access
 *       2. Scaling factor of 10^-7 should be applied while reading the latitude and longitude data.
 *       3. Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_nav_pvt(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                      uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the UBX-NAV-STATUS type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1. To access the any data use: gnss_cfg_data.packetUBXNAVSTATUS.data.data_to_access
 *       2. Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navstatus(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the UBX-NAV-SAT type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1. To access the any data use: gnss_cfg_data.packetUBXNAVSAT.data.block/header.data_to_access
 *       2. Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navsat(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the UBX-NAV-SIG type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1. To access the any data use:
 *          gnss_cfg_data.packetUBXNAVSIG.data.block/header.data_to_access
 *       2. Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navsig(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the UBX-NAV-EOE type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1. To access the any data use:
 *          gnss_cfg_data.packetUBXNAVEOE.data.data_to_access
 *       2. Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navepoch(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the UBX-NAV-DOP type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1.Multiply with scaling factor of 0.01 while getting all the data from this function.
 *       2.To access the any data use:
 *       gnss_cfg_data.packetUBXNAVDOP.data.data_to_access
 *       3.Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navdop(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the UBX-NAV-POSLLH type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1. To access the any data use:
 *          gnss_cfg_data.packetUBXNAVPOSLLH.data.data_to_access
 *       2. Scaling factor of 10^-7 should be applied while measuring
 *          latitude and longitude in degree decimal format.
 *       3. Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navposllh(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the UBX-NAV-TIMELS type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1. To access the any data use:
 *          gnss_cfg_data.packetUBXNAVTIMELS.data.data_to_access
 *       2. Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navtimels(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the UBX-NAV-TIMEUTC type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1. To access the any data use:
 *          gnss_cfg_data.packetUBXNAVTIMEUTC.data.data_to_access
 *       2. Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navtimeutc(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the UBX-NAV-CLOCK type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1. To access the any data use:
 *          gnss_cfg_data.packetUBXNAVCLOCK.data.data_to_access
 *       2. Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navclock(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the UBX-NAV-VELNED type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note 1. To access the any data use:
 *          gnss_cfg_data.packetUBXNAVVELNED.data.data_to_access
 *       2. Scaling factor of 10^-5 should be applied while reading the data for
 *          heading accuracy and course accuracy.
 *       3. Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_navvelned(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait);

/**************************************************************************//**
 * @brief Get the position dilution type data from UBX packet
 * @param[in] gnss_cfg_data : pointer to the structure containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[out] position_dillution : pointer pointer to store the position dilution data.
 * @return following values
 * - \ref SL_STATUS_OK if data is available.
 * - \ref SL_STATUS_FAIL if data is not available.
 * @note Can be used with UBX protocol only.
 *****************************************************************************/
sl_status_t gnss_max_m10s_get_positional_dillution(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int16_t *position_dillution);

/**************************************************************************//**
 * @brief Check how much time has passed since the program started.
 * @return time taken in milliseconds.
 *****************************************************************************/
uint32_t gnss_max_m10s_milli_sec();

#ifdef __cplusplus
}
#endif

#endif // GNSS_MAX_M10S_H_

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
