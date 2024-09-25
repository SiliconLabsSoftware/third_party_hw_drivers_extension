/***************************************************************************//**
 * @file gnss_max_m10s_driver.c
 * @brief GNSS receiver module API implementation
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
#include <stdlib.h>
#include "sl_sleeptimer.h"
#include "gnss_max_m10s_driver.h"
#include "gnss_max_m10s_ubx.h"
#include "gnss_max_m10s_nmea.h"
#ifdef SLI_SI917
#include "si91x_device.h"
#endif

typedef i2c_master_t max_m10s_i2c_t;

static max_m10s_i2c_t max_m10s_i2c;

/**************************************************************************//**
 * @brief Initialize the UBX packet structure members
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init_packet_data(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief Initialize the gnss_cfg_data structure members
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_init(sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief Check if the device is connected by sending the slave address
 *        through I2C.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_ping(sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief Returns the number of bytes available to read.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] data : pointer to the data to send.
 * @param[in] length : length of the data to send.
 * @return length of data written.
 *****************************************************************************/
static uint8_t gnss_max_m10s_write_bytes(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint8_t *data,
                                         uint8_t length);

/**************************************************************************//**
 * @brief Returns the number of bytes available to read.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] data : pointer to the buffer to store data.
 * @param[in] length : length of the data to be read.
 * @return length of data read.
 *****************************************************************************/
static uint8_t gnss_max_m10s_read_bytes(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint8_t *data,
                                        uint8_t length);

/**************************************************************************//**
 * @brief Returns the number of bytes available to read.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @param[in] assumeSuccess : success or failure of function.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_ublox_init(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Checks if the GNSS receiver module is connected.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_ublox_is_connected(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief to change packet_cfg_payload_size.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] payload_size : size of the UBX payload.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_set_packet_cfg_payload_size(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  size_t payload_size);

/**************************************************************************//**
 * @brief create the UBX file buffer.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_create_file_buffer(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief Given an UBX key, load the payload with data
 *        that can then be extracted to 8, 16, or 32 bits
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] key : key to the UBX packet.
 * @param[in] layer : layer for the UBX.
 * @param[in] max_wait : timeout value.
 * @return the following values are returned:
 * - \ref SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED if we got a config packet full
 *        of response data that has CLS/ID match to our query packet
 * - \ref SL_MAX_M10S_UBLOX_STATUS_CRC_FAIL if we got a corrupt config packet
 *        that has CLS/ID match to our query packet
 * - \ref SL_MAX_M10S_UBLOX_STATUS_TIMEOUT if we timed out
 * - \ref SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN if we got an a valid packetCfg
 *        but that the packetCfg has been or is currently being overwritten
 *****************************************************************************/
static sl_max_m10s_ublox_status_e gnss_max_m10s_getval(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint32_t key,
  uint8_t layer,
  uint16_t max_wait);

/**************************************************************************//**
 * @brief Given an UBX key, return its value.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] key : key to the UBX packet.
 * @param[in] val : to store the value of the UBX key.
 * @param[in] layer : layer for the UBX.
 * @param[in] max_wait : timeout value.
 * @return none.
 *****************************************************************************/
static bool gnss_max_m10s_getval8(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                  uint32_t key,
                                  uint8_t *val,
                                  uint8_t layer,
                                  uint16_t max_wait);

/**************************************************************************//**
 * @brief calculate and store the two byte checksum over
 *        the entirety of the message.
 * @param[in] msg : pointer to the UBX packet.
 *****************************************************************************/
static void gnss_max_m10s_calc_checksum(sl_max_m10s_ubx_packet_t *msg);

/**************************************************************************//**
 * @brief sends the UBX packet via I2C.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] outgoingUBX : pointer to the UBX packet.
 * @return the following values are returned:
 * - \ref SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED if we got a config packet full
 *        of response data that has CLS/ID match to our query packet
 * - \ref SL_MAX_M10S_UBLOX_STATUS_CRC_FAIL if we got a corrupt config packet
 *        that has CLS/ID match to our query packet
 * - \ref SL_MAX_M10S_UBLOX_STATUS_TIMEOUT if we timed out
 * - \ref SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN if we got an a valid packetCfg
 *        but that the packetCfg has been or is currently being overwritten
 *        (remember that Serial data can arrive very slowly)
 *****************************************************************************/
static sl_max_m10s_ublox_status_e gnss_max_m10s_send_i2c_command(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  sl_max_m10s_ubx_packet_t *outgoingUBX);

/**************************************************************************//**
 * @brief  checks for ACK when the module is responding with register content
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] incomingUBX : pointer to UBX packet.
 * @param[in] requested_class : class for the UBX packet.
 * @param[in] requested_id : ID for the UBX packet.
 * @param[in] max_time : timeout value.
 * @return the following values are returned:
 * - \ref SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED if we got a config packet full
 *        of response data that has CLS/ID match to our query packet
 * - \ref SL_MAX_M10S_UBLOX_STATUS_CRC_FAIL if we got a corrupt config packet
 *        that has CLS/ID match to our query packet
 * - \ref SL_MAX_M10S_UBLOX_STATUS_TIMEOUT if we timed out
 * - \ref SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN if we got an a valid packetCfg
 *        but that the packetCfg has been or is currently being overwritten
 *        (remember that Serial data can arrive very slowly)
 *****************************************************************************/
static sl_max_m10s_ublox_status_e gnss_max_m10s_wait_for_ack_response(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  sl_max_m10s_ubx_packet_t *outgoingUBX,
  uint8_t requested_class,
  uint8_t requested_id,
  uint16_t max_time);

/**************************************************************************//**
 * @brief  For non-CFG queries no ACK is sent so we use this function
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] incomingUBX : pointer to UBX packet.
 * @param[in] requested_class : class for the UBX packet.
 * @param[in] requested_id : ID for the UBX packet.
 * @param[in] max_time : timeout value.
 * @return the following values are returned:
 * - \ref SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED if we got a config packet full
 *        of response data that has CLS/ID match to our query packet
 * - \ref SL_MAX_M10S_UBLOX_STATUS_CRC_FAIL if we got a corrupt config packet
 *        that has CLS/ID match to our query packet
 * - \ref SL_MAX_M10S_UBLOX_STATUS_TIMEOUT if we timed out
 * - \ref SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN if we got an a valid packetCfg
 *        but that the packetCfg has been or is currently being overwritten
 *        (remember that Serial data can arrive very slowly)
 *****************************************************************************/
static sl_max_m10s_ublox_status_e gnss_max_m10s_wait_for_no_ack_response(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  sl_max_m10s_ubx_packet_t *outgoingUBX,
  uint8_t requested_class,
  uint8_t requested_id,
  uint16_t max_time);

/**************************************************************************//**
 * @brief Polls I2C for data, passing any new bytes to gnss_m10s_process()
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] incomingUBX : pointer to incoming UBX packet.
 * @param[in] requested_class : class for the UBX packet.
 * @param[in] requested_id : ID for the UBX packet.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_check_ublox_i2c(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  sl_max_m10s_ubx_packet_t *incomingUBX,
  uint8_t requested_class,
  uint8_t requested_id);

/**************************************************************************//**
 * @brief Processes UBX binary sentences one byte at a time, take a given byte
 *        and file it into the proper array
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] incoming : character to check
 * @param[in] incomingUBX : UBX packet to process.
 * @param[in] requested_class : class of the UBX packet.
 * @param[in] requested_id : ID for the UBX packet.
 *****************************************************************************/
static void gnss_max_m10s_process(sl_max_m10s_cfg_data_t *gnss_cfg_data_data,
                                  uint8_t incoming,
                                  sl_max_m10s_ubx_packet_t *incomingUBX,
                                  uint8_t requested_class,
                                  uint8_t requested_id);

/**************************************************************************//**
 * @brief Check how much space is available in the buffer
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return number of bytes available in the file buffer.
 *****************************************************************************/
static uint16_t gnss_max_m10s_file_buffer_space_available(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief Check how much space is used in the buffer
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @return number of bytes used in the file buffer.
 *****************************************************************************/
static uint16_t gnss_max_m10s_file_buffer_used(
  sl_max_m10s_cfg_data_t *gnss_cfg_data);

/**************************************************************************//**
 * @brief Write the_bytes to the file buffer
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] the_bytes : pointer to bytes to write.
 * @param[in] num_bytes : number of bytes.
 *****************************************************************************/
static void gnss_max_m10s_write_file_buffer(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t *the_bytes,
  uint16_t num_bytes);

/**************************************************************************//**
 * @brief Start defining a new (empty) UBX-CFG-VALSET ubx packet.
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] layer : RAM layer.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_new_cfg_valset(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t layer);

/**************************************************************************//**
 * @brief Add another key and value to an existing UBX-CFG-VALSET ubx packet
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] key : UBX packet key.
 * @param[in] value : UBX type communication.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_add_cfg_valset(
  sl_max_m10s_cfg_data_t *gnss_cfg_data_data,
  uint32_t key,
  uint8_t *value,
  uint8_t N);

/**************************************************************************//**
 * @brief Send the UBX-CFG-VALSET ubx packet
 * @param[in] gnss_cfg_data : pointer to the structure
 *                            containing GNSS configuration data.
 * @param[in] max_wait : Timeout value.
 * @return The following values are returned:
 * - \ref SL_STATUS_OK on success.
 * - \ref SL_STATUS_FAIL on failure.
 *****************************************************************************/
static sl_status_t gnss_max_m10s_send_cfg_valset(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait);

static sl_status_t gnss_max_m10s_init_packet_data(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  gnss_cfg_data->packet_ack.cls = 0;
  gnss_cfg_data->packet_ack.id = 0;
  gnss_cfg_data->packet_ack.len = 0;
  gnss_cfg_data->packet_ack.counter = 0;
  gnss_cfg_data->packet_ack.starting_spot = 0;
  gnss_cfg_data->packet_ack.payload = gnss_cfg_data->msg_data.payload_ack;
  gnss_cfg_data->packet_ack.checksum_a = 0;
  gnss_cfg_data->packet_ack.checksum_b = 0;
  gnss_cfg_data->packet_ack.valid =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_ack.class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;

  gnss_cfg_data->packet_buf.cls = 0;
  gnss_cfg_data->packet_buf.id = 0;
  gnss_cfg_data->packet_buf.len = 0;
  gnss_cfg_data->packet_buf.counter = 0;
  gnss_cfg_data->packet_buf.starting_spot = 0;
  gnss_cfg_data->packet_buf.payload = gnss_cfg_data->msg_data.payload_buf;
  gnss_cfg_data->packet_buf.checksum_a = 0;
  gnss_cfg_data->packet_buf.checksum_b = 0;
  gnss_cfg_data->packet_buf.valid =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_buf.class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;

  gnss_cfg_data->packet_auto.cls = 0;
  gnss_cfg_data->packet_auto.id = 0;
  gnss_cfg_data->packet_auto.len = 0;
  gnss_cfg_data->packet_auto.counter = 0;
  gnss_cfg_data->packet_auto.starting_spot = 0;
  gnss_cfg_data->packet_auto.payload = NULL;
  gnss_cfg_data->packet_auto.checksum_a = 0;
  gnss_cfg_data->packet_auto.checksum_b = 0;
  gnss_cfg_data->packet_auto.valid =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_auto.class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;

  gnss_cfg_data->packet_cfg.cls = 0;
  gnss_cfg_data->packet_cfg.id = 0;
  gnss_cfg_data->packet_cfg.len = 0;
  gnss_cfg_data->packet_cfg.counter = 0;
  gnss_cfg_data->packet_cfg.starting_spot = 0;
  gnss_cfg_data->packet_cfg.payload = NULL;
  gnss_cfg_data->packet_cfg.checksum_a = 0;
  gnss_cfg_data->packet_cfg.checksum_b = 0;
  gnss_cfg_data->packet_cfg.valid =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_cfg.class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_init(sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  sl_status_t status;
  i2c_master_config_t max_m10s_cfg;

  if (NULL == gnss_cfg_data->i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Configure default i2csmp instance
  max_m10s_i2c.handle = gnss_cfg_data->i2c_instance;

  i2c_master_configure_default(&max_m10s_cfg);

  max_m10s_cfg.addr = gnss_cfg_data->device_address;
  max_m10s_cfg.timeout_pass_count = 0;

  if (i2c_master_open(&max_m10s_i2c, &max_m10s_cfg) == I2C_MASTER_ERROR) {
    return SL_STATUS_INITIALIZATION;
  }

  gnss_max_m10s_init_packet_data(gnss_cfg_data);

  gnss_cfg_data->msg_data.packet_cfg_payloadsize = 0;
  gnss_cfg_data->msg_data.payload_cfg = NULL;
  gnss_cfg_data->msg_data.payload_auto = NULL;
  gnss_cfg_data->reset_current_sentence_on_bus_error = true;
  gnss_cfg_data->auto_send_at_space_remaining = 0;
  gnss_cfg_data->ubx_file_buffer = NULL;
  gnss_cfg_data->file_buffer_head = 0;
  gnss_cfg_data->file_buffer_tail = 0;
  gnss_cfg_data->file_buffer_max_avail = 0;
  gnss_cfg_data->ignore_this_payload = false;
  gnss_cfg_data->max_nmea_byte_count = SL_MAX_M10S_NMEA_BYTE_COUNT;
  gnss_cfg_data->packetUBXNAVPVT = NULL;
  gnss_cfg_data->packetUBXNAVCLOCK = NULL;
  gnss_cfg_data->packetUBXNAVDOP = NULL;
  gnss_cfg_data->packetUBXNAVEOE = NULL;
  gnss_cfg_data->packetUBXNAVPOSLLH = NULL;
  gnss_cfg_data->packetUBXNAVSAT = NULL;
  gnss_cfg_data->packetUBXNAVSTATUS = NULL;
  gnss_cfg_data->packetUBXNAVSIG = NULL;
  gnss_cfg_data->packetUBXNAVTIMELS = NULL;
  gnss_cfg_data->packetUBXNAVTIMEUTC = NULL;
  gnss_cfg_data->packetUBXNAVVELNED = NULL;
  gnss_cfg_data->packetUBXUNIQID = NULL;
  gnss_cfg_data->storageNMEAGNGGA = NULL;
  gnss_cfg_data->storageNMEAGNRMC = NULL;
  gnss_cfg_data->storageNMEAGPGGA = NULL;
  gnss_cfg_data->storageNMEAGPRMC = NULL;
  gnss_cfg_data->nmea_data = NULL;
  gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETBUF;
  gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE;

  if (gnss_cfg_data->protocol_type == SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_nmea_init(gnss_cfg_data);
  } else {
    status = SL_STATUS_OK;
  }
  return status;
}

sl_status_t gnss_max_m10s_deinit(sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->packetUBXNAVPVT != NULL) {
    free(gnss_cfg_data->packetUBXNAVPVT);
    gnss_cfg_data->packetUBXNAVPVT = NULL;
  }

  if (gnss_cfg_data->packetUBXNAVCLOCK != NULL) {
    free(gnss_cfg_data->packetUBXNAVCLOCK);
    gnss_cfg_data->packetUBXNAVCLOCK = NULL;
  }

  if (gnss_cfg_data->packetUBXNAVDOP != NULL) {
    free(gnss_cfg_data->packetUBXNAVDOP);
    gnss_cfg_data->packetUBXNAVDOP = NULL;
  }

  if (gnss_cfg_data->packetUBXNAVEOE != NULL) {
    free(gnss_cfg_data->packetUBXNAVEOE);
    gnss_cfg_data->packetUBXNAVEOE = NULL;
  }

  if (gnss_cfg_data->packetUBXNAVPOSLLH != NULL) {
    free(gnss_cfg_data->packetUBXNAVPOSLLH);
    gnss_cfg_data->packetUBXNAVPOSLLH = NULL;
  }

  if (gnss_cfg_data->packetUBXNAVSAT != NULL) {
    free(gnss_cfg_data->packetUBXNAVSAT);
    gnss_cfg_data->packetUBXNAVSAT = NULL;
  }

  if (gnss_cfg_data->packetUBXNAVSIG != NULL) {
    free(gnss_cfg_data->packetUBXNAVSIG);
    gnss_cfg_data->packetUBXNAVSIG = NULL;
  }

  if (gnss_cfg_data->packetUBXNAVSTATUS != NULL) {
    free(gnss_cfg_data->packetUBXNAVSTATUS);
    gnss_cfg_data->packetUBXNAVSTATUS = NULL;
  }

  if (gnss_cfg_data->packetUBXNAVVELNED != NULL) {
    free(gnss_cfg_data->packetUBXNAVVELNED);
    gnss_cfg_data->packetUBXNAVVELNED = NULL;
  }

  if (gnss_cfg_data->packetUBXNAVTIMELS != NULL) {
    free(gnss_cfg_data->packetUBXNAVTIMELS);
    gnss_cfg_data->packetUBXNAVTIMELS = NULL;
  }

  if (gnss_cfg_data->packetUBXNAVTIMEUTC != NULL) {
    free(gnss_cfg_data->packetUBXNAVTIMEUTC);
    gnss_cfg_data->packetUBXNAVTIMEUTC = NULL;
  }

  if (gnss_cfg_data->packetUBXUNIQID != NULL) {
    free(gnss_cfg_data->packetUBXUNIQID);
    gnss_cfg_data->packetUBXUNIQID = NULL;
  }

  if (gnss_cfg_data->nmea_data != NULL) {
    free(gnss_cfg_data->nmea_data);
    gnss_cfg_data->nmea_data = NULL;
  }

  return status;
}

sl_status_t gnss_max_m10s_begin(uint16_t max_wait,
                                sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  sl_status_t status;
  status = gnss_max_m10s_init(gnss_cfg_data);
  if (SL_STATUS_OK == status) {
    /// printf("gnss_max_m10s_init is successful\r\n");
    gnss_cfg_data->status = SL_MAX_M10S_INIT_OK;
  } else {
    gnss_cfg_data->status = SL_MAX_M10S_INIT_FAIL;
    /// printf("gnss_max_m10s_init failed!!\r\n");
    return status;
  }

  status = gnss_max_m10s_ublox_init(gnss_cfg_data, max_wait);
  if (SL_STATUS_OK == status) {
    /// printf("gnss_max_m10s_ublox_init is successful\r\n");
    gnss_cfg_data->status = SL_MAX_M10S_INIT_OK;
  } else {
    gnss_cfg_data->status = SL_MAX_M10S_INIT_FAIL;
    /// printf("gnss_max_m10s_ublox_init failed!!\r\n");
    return status;
  }

  gnss_cfg_data->status = SL_MAX_M10S_BEGIN_OK;
  return status;
}

static sl_status_t gnss_max_m10s_ping(sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  size_t write_buffer_size = 1;
  uint8_t i2c_write_data[write_buffer_size];

  i2c_write_data[0] = 0;

  if (NULL == gnss_cfg_data->i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (I2C_MASTER_SUCCESS != i2c_master_write(&max_m10s_i2c,
                                             i2c_write_data,
                                             write_buffer_size)) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

static uint8_t gnss_max_m10s_write_bytes(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint8_t *data, uint8_t length)
{
  if (NULL == gnss_cfg_data->i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (0 == length) {
    return 0;
  }

  if (I2C_MASTER_SUCCESS != i2c_master_write(&max_m10s_i2c,
                                             data,
                                             length)) {
    return 0;
  }

  return length;
}

static uint8_t gnss_max_m10s_read_bytes(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint8_t *data, uint8_t length)
{
  if (NULL == gnss_cfg_data->i2c_instance) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if (0 == length) {
    return 0;
  }

  if (I2C_MASTER_SUCCESS != i2c_master_read(&max_m10s_i2c,
                                            data,
                                            length)) {
    return 0;
  }

  return length;
}

static sl_status_t gnss_max_m10s_create_file_buffer(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  if (0 == FILE_BUFFER_SIZE) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->ubx_file_buffer != NULL) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->ubx_file_buffer =
    (uint8_t *)malloc(FILE_BUFFER_SIZE * sizeof(uint8_t));

  if (NULL == gnss_cfg_data->ubx_file_buffer) {
    return SL_STATUS_FAIL;
  }

  gnss_cfg_data->file_buffer_head = 0;
  gnss_cfg_data->file_buffer_tail = 0;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_ublox_init(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  sl_status_t status = SL_STATUS_OK;

  gnss_cfg_data->signs_of_life = false;

  if (0 == gnss_cfg_data->msg_data.packet_cfg_payloadsize) {
    gnss_max_m10s_set_packet_cfg_payload_size(gnss_cfg_data, MAX_PAYLOAD_SIZE);
  }

  status = gnss_max_m10s_create_file_buffer(gnss_cfg_data);

  if (status != SL_STATUS_OK) {
    goto return_failure;
  }

  status = gnss_max_m10s_ublox_is_connected(gnss_cfg_data, max_wait);

  if (status != SL_STATUS_OK) {
    goto return_failure;
  }

  return_failure:
  return status;
}

static sl_status_t gnss_max_m10s_set_packet_cfg_payload_size(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  size_t payload_size)
{
  sl_status_t success = SL_STATUS_OK;
  sl_max_m10s_msg_data_t *msg_data_local = &(gnss_cfg_data->msg_data);

  if ((payload_size == 0) && (msg_data_local->payload_cfg != NULL)) {
    free(msg_data_local->payload_cfg);
    msg_data_local->payload_cfg = NULL;
    gnss_cfg_data->packet_cfg.payload = msg_data_local->payload_cfg;
    msg_data_local->packet_cfg_payloadsize = payload_size;
  } else if (msg_data_local->payload_cfg == NULL) {
    msg_data_local->payload_cfg =
      (uint8_t *)malloc(payload_size * sizeof(uint8_t));
    gnss_cfg_data->packet_cfg.payload = msg_data_local->payload_cfg;

    if (msg_data_local->payload_cfg == NULL) {
      success = SL_STATUS_FAIL;
      msg_data_local->packet_cfg_payloadsize = 0;
    } else {
      msg_data_local->packet_cfg_payloadsize = payload_size;
    }
  } else {
    uint8_t *new_payload = (uint8_t *)malloc(payload_size * sizeof(uint8_t));

    if (new_payload == NULL) {
      success = SL_STATUS_FAIL;
    } else {
      memcpy(new_payload,
             msg_data_local->payload_cfg,
             payload_size
             <= msg_data_local->packet_cfg_payloadsize ? payload_size : msg_data_local->packet_cfg_payloadsize);

      free(msg_data_local->payload_cfg);
      msg_data_local->payload_cfg = new_payload;
      gnss_cfg_data->packet_cfg.payload = msg_data_local->payload_cfg;
      msg_data_local->packet_cfg_payloadsize = payload_size;
    }
  }

  return (success);
}

static sl_status_t gnss_max_m10s_ublox_is_connected(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  uint8_t en;
  bool result;

  if (gnss_max_m10s_ping(gnss_cfg_data) != SL_STATUS_OK) {
    return SL_STATUS_FAIL;
  }

  result = gnss_max_m10s_getval8(gnss_cfg_data,
                                 UBLOX_CFG_I2C_INPORT_UBX,
                                 &en,
                                 VAL_LAYER_RAM,
                                 max_wait);

  if (false == result) {
    return SL_STATUS_FAIL;
  }

  return SL_STATUS_OK;
}

static sl_max_m10s_ublox_status_e gnss_max_m10s_getval(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint32_t key,
  uint8_t layer,
  uint16_t max_wait)
{
  uint8_t get_layer;
  sl_max_m10s_ublox_status_e ret_val;

  gnss_cfg_data->packet_cfg.cls = UBX_CLASS_CFG;
  gnss_cfg_data->packet_cfg.id = UBX_CFG_VALGET;
  gnss_cfg_data->packet_cfg.len = 4 + 4 * 1;
  gnss_cfg_data->packet_cfg.starting_spot = 0;

  memset(gnss_cfg_data->msg_data.payload_cfg, 0, gnss_cfg_data->packet_cfg.len);

  get_layer = VAL_LAYER_DEFAULT;

  if (layer == VAL_LAYER_RAM) {
    get_layer = 0;
  } else if (layer == VAL_LAYER_BBR) {
    get_layer = 1;
  } else if (layer == VAL_LAYER_FLASH) {
    get_layer = 2;
  }

  gnss_cfg_data->msg_data.payload_cfg[0] = 0;
  gnss_cfg_data->msg_data.payload_cfg[1] = get_layer;
  key &= ~UBX_CFG_SIZE_MASK;
  gnss_cfg_data->msg_data.payload_cfg[4] = key >> 8 * 0;
  gnss_cfg_data->msg_data.payload_cfg[5] = key >> 8 * 1;
  gnss_cfg_data->msg_data.payload_cfg[6] = key >> 8 * 2;
  gnss_cfg_data->msg_data.payload_cfg[7] = key >> 8 * 3;
  gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

  /// printf("getVal key: %lu\r\n",key);
  ret_val = gnss_max_m10s_send_command(gnss_cfg_data,
                                       max_wait,
                                       false);

  return ret_val;
}

static bool gnss_max_m10s_getval8(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                  uint32_t key,
                                  uint8_t *val,
                                  uint8_t layer,
                                  uint16_t max_wait)
{
  bool result =
    (SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED
     == gnss_max_m10s_getval(gnss_cfg_data, key, layer, max_wait));

  if (result) {
    *val = gnss_max_m10s_extract_byte(&(gnss_cfg_data->packet_cfg), 8);
  }
  return result;
}

sl_max_m10s_ublox_status_e gnss_max_m10s_send_command(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool expect_ack_only)
{
  sl_max_m10s_ubx_packet_t *outgoingUBX;
  sl_max_m10s_ublox_status_e ret_val = SL_MAX_M10S_UBLOX_STATUS_SUCCESS;

  if (gnss_cfg_data->active_packet_buffer
      == SL_MAX_M10S_UBLOX_PACKET_PACKETCFG) {
    outgoingUBX = &(gnss_cfg_data->packet_cfg);
  } else if (gnss_cfg_data->active_packet_buffer
             == SL_MAX_M10S_UBLOX_PACKET_PACKETAUTO) {
    outgoingUBX = &(gnss_cfg_data->packet_auto);
  } else {
    outgoingUBX = &(gnss_cfg_data->packet_buf);
  }

  gnss_max_m10s_calc_checksum(outgoingUBX);

  ret_val = gnss_max_m10s_send_i2c_command(gnss_cfg_data, outgoingUBX);

  if (ret_val != SL_MAX_M10S_UBLOX_STATUS_SUCCESS) {
    return ret_val;
  }

  if (max_wait > 0) {
    if ((outgoingUBX->cls == UBX_CLASS_CFG) || (expect_ack_only == true)) {
      ret_val = gnss_max_m10s_wait_for_ack_response(gnss_cfg_data,
                                                    outgoingUBX,
                                                    outgoingUBX->cls,
                                                    outgoingUBX->id,
                                                    max_wait);
    } else {
      ret_val = gnss_max_m10s_wait_for_no_ack_response(gnss_cfg_data,
                                                       outgoingUBX,
                                                       outgoingUBX->cls,
                                                       outgoingUBX->id,
                                                       max_wait);
    }
  }

  return ret_val;
}

static void gnss_max_m10s_calc_checksum(sl_max_m10s_ubx_packet_t *msg)
{
  msg->checksum_a = 0;
  msg->checksum_b = 0;

  msg->checksum_a += msg->cls;
  msg->checksum_b += msg->checksum_a;

  msg->checksum_a += msg->id;
  msg->checksum_b += msg->checksum_a;

  msg->checksum_a += (msg->len & 0xFF);
  msg->checksum_b += msg->checksum_a;

  msg->checksum_a += (msg->len >> 8);
  msg->checksum_b += msg->checksum_a;

  for (uint16_t i = 0; i < msg->len; i++) {
    msg->checksum_a += msg->payload[i];
    msg->checksum_b += msg->checksum_a;
  }
}

void gnss_max_m10s_add_to_checksum(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                   uint8_t incoming)
{
  gnss_cfg_data->rolling_checksum_a += incoming;
  gnss_cfg_data->rolling_checksum_b += gnss_cfg_data->rolling_checksum_a;
}

static sl_max_m10s_ublox_status_e gnss_max_m10s_send_i2c_command(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  sl_max_m10s_ubx_packet_t *outgoingUBX)
{
  uint16_t bytes_left_to_send = outgoingUBX->len;
  uint16_t start_spot = 0;
  uint16_t i = 0;

  if (bytes_left_to_send + 8 <= I2C_TRANSACTION_SIZE) {
    uint8_t buf[I2C_TRANSACTION_SIZE];
    buf[0] = UBX_SYNCH_1;
    buf[1] = UBX_SYNCH_2;
    buf[2] = outgoingUBX->cls;
    buf[3] = outgoingUBX->id;
    buf[4] = outgoingUBX->len & 0xFF;
    buf[5] = outgoingUBX->len >> 8;

    for (; i < outgoingUBX->len; i++) {
      buf[i + 6] = outgoingUBX->payload[start_spot + i];
    }

    buf[i + 6] = outgoingUBX->checksum_a;
    buf[i + 7] = outgoingUBX->checksum_b;

    /// TODO : Debugging purpose.

    /**
     * printf("sendI2cCommand dump\n");
     * for(i=0; i < bytes_left_to_send + 8; i++) {
     * printf("0x%x ",buf[i]);
     * }
     */

    if (gnss_max_m10s_write_bytes(gnss_cfg_data, buf, bytes_left_to_send + 8)
        != bytes_left_to_send + 8) {
      return SL_MAX_M10S_UBLOX_STATUS_I2C_COMM_FAILURE;
    }
  } else {
    uint8_t buf[6];

    buf[0] = UBX_SYNCH_1;
    buf[1] = UBX_SYNCH_2;
    buf[2] = outgoingUBX->cls;
    buf[3] = outgoingUBX->id;
    buf[4] = outgoingUBX->len & 0xFF;        /// LSB
    buf[5] = outgoingUBX->len >> 8;          /// MSB

    if (gnss_max_m10s_write_bytes(gnss_cfg_data, buf, 6) != 6) {
      return SL_MAX_M10S_UBLOX_STATUS_I2C_COMM_FAILURE;
    }

    while (bytes_left_to_send > 1) {
      uint16_t len = bytes_left_to_send;
      uint16_t x;

      if (len > I2C_TRANSACTION_SIZE) {
        len = I2C_TRANSACTION_SIZE;
      }

      bytes_left_to_send -= len;
      x = len;

      if ((start_spot + x) >= (outgoingUBX->len)) {
        x = outgoingUBX->len - start_spot;
      }

      if (gnss_max_m10s_write_bytes(gnss_cfg_data,
                                    &outgoingUBX->payload[start_spot],
                                    x) != x) {
        return SL_MAX_M10S_UBLOX_STATUS_I2C_COMM_FAILURE;
      }

      start_spot += x;
    }

    if (bytes_left_to_send == 1) {
      buf[0] = outgoingUBX->payload[start_spot];
      buf[1] = outgoingUBX->checksum_a;
      buf[2] = outgoingUBX->checksum_b;

      if (gnss_max_m10s_write_bytes(gnss_cfg_data, buf, 3) != 3) {
        return SL_MAX_M10S_UBLOX_STATUS_I2C_COMM_FAILURE;
      }
    } else {
      buf[0] = outgoingUBX->checksum_a;
      buf[1] = outgoingUBX->checksum_b;

      if (gnss_max_m10s_write_bytes(gnss_cfg_data, buf, 2) != 2) {
        return SL_MAX_M10S_UBLOX_STATUS_I2C_COMM_FAILURE;
      }
    }
  }

  return SL_MAX_M10S_UBLOX_STATUS_SUCCESS;
}

static sl_max_m10s_ublox_status_e gnss_max_m10s_wait_for_ack_response(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  sl_max_m10s_ubx_packet_t *outgoingUBX,
  uint8_t requested_class,
  uint8_t requested_id,
  uint16_t max_time)
{
  unsigned long start_time;

  gnss_cfg_data->packet_ack.valid =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_buf.valid =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_auto.valid =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_ack.class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_buf.class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_auto.class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  outgoingUBX->class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  outgoingUBX->valid = SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() < (start_time + (unsigned long)max_time)) {
    if (gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                           outgoingUBX,
                                           requested_class,
                                           requested_id) == SL_STATUS_OK) {
      if ((outgoingUBX->class_and_id_match
           == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
          && (gnss_cfg_data->packet_ack.class_and_id_match
              == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
          && (outgoingUBX->valid == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
          && (outgoingUBX->cls == requested_class)
          && (outgoingUBX->id == requested_id)) {
        return SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED;
      } else if ((outgoingUBX->class_and_id_match
                  == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED)
                 && (gnss_cfg_data->packet_ack.class_and_id_match
                     == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)) {
        return SL_MAX_M10S_UBLOX_STATUS_DATA_SENT;
      } else if ((outgoingUBX->class_and_id_match
                  == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
                 && (gnss_cfg_data->packet_ack.class_and_id_match
                     == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
                 && ((outgoingUBX->cls != requested_class)
                     || (outgoingUBX->id != requested_id))) {
        return SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN;
      } else if ((gnss_cfg_data->packet_ack.class_and_id_match
                  == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
                 && (outgoingUBX->class_and_id_match
                     == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_VALID)
                 && (outgoingUBX->valid
                     == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_VALID)) {
        return SL_MAX_M10S_UBLOX_STATUS_CRC_FAIL;
      } else if (gnss_cfg_data->packet_ack.class_and_id_match
                 == SL_MAX_M10S_UBLOX_PACKET_NOTACKNOWLEDGED) {
        return SL_MAX_M10S_UBLOX_STATUS_COMMAND_NACK;
      } else if ((outgoingUBX->class_and_id_match
                  == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
                 && (gnss_cfg_data->packet_ack.class_and_id_match
                     == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_VALID)
                 && (outgoingUBX->valid
                     == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
                 && (outgoingUBX->cls == requested_class)
                 && (outgoingUBX->id == requested_id)) {
        return SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED;
      } else if ((outgoingUBX->class_and_id_match
                  == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_VALID)
                 && (gnss_cfg_data->packet_ack.class_and_id_match
                     == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_VALID)) {
        return SL_MAX_M10S_UBLOX_STATUS_FAIL;
      } else if ((outgoingUBX->class_and_id_match
                  == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
                 && (gnss_cfg_data->packet_ack.class_and_id_match
                     == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED)) {
      }
    }

    gnss_max_m10s_delay(1);
  }

  if ((outgoingUBX->class_and_id_match
       == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
      && (gnss_cfg_data->packet_ack.class_and_id_match
          == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED)
      && (outgoingUBX->valid == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
      && (outgoingUBX->cls == requested_class)
      && (outgoingUBX->id == requested_id)) {
    return SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED;
  }

  return SL_MAX_M10S_UBLOX_STATUS_TIMEOUT;
}

static sl_max_m10s_ublox_status_e gnss_max_m10s_wait_for_no_ack_response(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  sl_max_m10s_ubx_packet_t *outgoingUBX,
  uint8_t requested_class,
  uint8_t requested_id,
  uint16_t max_time)
{
  unsigned long start_time;

  gnss_cfg_data->packet_ack.valid =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_buf.valid =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_auto.valid =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_ack.class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_buf.class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  gnss_cfg_data->packet_auto.class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  outgoingUBX->class_and_id_match =
    SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
  outgoingUBX->valid = SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;

  start_time = gnss_max_m10s_milli_sec();

  while (gnss_max_m10s_milli_sec() - start_time < max_time) {
    if (gnss_max_m10s_check_ublox_internal(gnss_cfg_data,
                                           outgoingUBX,
                                           requested_class,
                                           requested_id) == SL_STATUS_OK) {
      if ((outgoingUBX->class_and_id_match
           == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
          && (outgoingUBX->valid == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
          && (outgoingUBX->cls == requested_class)
          && (outgoingUBX->id == requested_id)) {
        return SL_MAX_M10S_UBLOX_STATUS_DATA_RECEIVED;
      } else if ((outgoingUBX->class_and_id_match
                  == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)
                 && ((outgoingUBX->cls != requested_class)
                     || (outgoingUBX->id != requested_id))) {
        return SL_MAX_M10S_UBLOX_STATUS_DATA_OVERWRITTEN;
      } else if ((outgoingUBX->class_and_id_match
                  == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED)
                 && (outgoingUBX->valid
                     == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_VALID)) {
      } else if (outgoingUBX->class_and_id_match
                 == SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_VALID) {
        return SL_MAX_M10S_UBLOX_STATUS_CRC_FAIL;
      }
    }

    gnss_max_m10s_delay(1);
  }

  return SL_MAX_M10S_UBLOX_STATUS_TIMEOUT;
}

sl_status_t gnss_max_m10s_check_ublox_internal(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  sl_max_m10s_ubx_packet_t *incomingUBX,
  uint8_t requested_class,
  uint8_t requested_id)
{
  sl_status_t status = SL_STATUS_OK;

  status = gnss_max_m10s_check_ublox_i2c(gnss_cfg_data,
                                         incomingUBX,
                                         requested_class,
                                         requested_id);

  return status;
}

void gnss_max_m10s_delay(uint32_t period)
{
  for (uint32_t x = 0; x < MS_DELAY_COUNTER * period; x++) {
    __NOP();
  }
}

static sl_status_t gnss_max_m10s_check_ublox_i2c(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  sl_max_m10s_ubx_packet_t *incomingUBX,
  uint8_t requested_class,
  uint8_t requested_id)
{
  uint16_t bytes_to_read;
  uint8_t bytes_returned;
  uint8_t rx_byte = 0;
  bool bytes_valid = true;

  bytes_to_read = 1;

  while (bytes_valid) {
    bytes_returned = gnss_max_m10s_read_bytes(gnss_cfg_data, &rx_byte,
                                              (uint8_t)bytes_to_read);

    if (bytes_returned) {
      gnss_max_m10s_process(gnss_cfg_data, rx_byte, incomingUBX,
                            requested_class, requested_id);

      if (rx_byte == 0xFF) {
        bytes_valid = false;
      }
    } else {
      if (gnss_cfg_data->reset_current_sentence_on_bus_error) {
        gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE;
        return SL_STATUS_FAIL;
      }
    }
  }

  return SL_STATUS_OK;
}

bool gnss_max_m10s_auto_lookup(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                               uint16_t *max_size)
{
  uint8_t ID;

  if (gnss_cfg_data->active_packet_buffer
      == SL_MAX_M10S_UBLOX_PACKET_PACKETCFG) {
    ID = gnss_cfg_data->packet_cfg.id;
  } else if (gnss_cfg_data->active_packet_buffer
             == SL_MAX_M10S_UBLOX_PACKET_PACKETAUTO) {
    ID = gnss_cfg_data->packet_auto.id;
  } else {
    ID = gnss_cfg_data->packet_buf.id;
  }

  if (max_size != NULL) {
    *max_size = 0;
  }

  if (ID == UBX_NAV_PVT) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_PVT_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVPVT != NULL);
  } else if (ID == UBX_NAV_CLOCK) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_CLOCK_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVCLOCK != NULL);
  } else if (ID == UBX_NAV_DOP) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_DOP_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVDOP != NULL);
  } else if (ID == UBX_NAV_EOE) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_EOE_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVEOE != NULL);
  } else if (ID == UBX_NAV_POSLLH) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_POSLLH_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVPOSLLH != NULL);
  } else if (ID == UBX_NAV_SAT) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_SAT_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVSAT != NULL);
  } else if (ID == UBX_NAV_SIG) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_SIG_MAX_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVSIG != NULL);
  } else if (ID == UBX_NAV_STATUS) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_STATUS_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVSTATUS != NULL);
  } else if (ID == UBX_NAV_TIMELS) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_TIMELS_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVTIMELS != NULL);
  } else if (ID == UBX_NAV_TIMEUTC) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_TIMEUTC_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVTIMEUTC != NULL);
  } else if (ID == UBX_NAV_VELNED) {
    if (max_size != NULL) {
      *max_size = UBX_NAV_VELNED_LEN;
    }

    return (gnss_cfg_data->packetUBXNAVVELNED != NULL);
  }

  return false;
}

static void gnss_max_m10s_process(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                  uint8_t incoming,
                                  sl_max_m10s_ubx_packet_t *incomingUBX,
                                  uint8_t requested_class,
                                  uint8_t requested_id)
{
  static volatile uint8_t stored_class = 0;
  static volatile uint8_t stored_id = 0;
  uint16_t maxPayload = 0;

  if (requested_class || requested_id) {
    stored_class = requested_class;
    stored_id = requested_id;
  }

  if ((gnss_cfg_data->current_sentence == SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE)
      || (gnss_cfg_data->current_sentence
          == SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NMEA)) {
    if (incoming == UBX_SYNCH_1) {
      gnss_cfg_data->ubx_frame_counter = 0;
      gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_UBX;
      gnss_cfg_data->packet_buf.counter = 0;
      gnss_cfg_data->ignore_this_payload = false;
      gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETBUF;
    } else if (incoming == '$') {
      gnss_cfg_data->nmea_byte_counter = 0;
      gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NMEA;
    } else if (incoming == 0xD3) {
      gnss_cfg_data->rtcm_frame_counter = 0;
      gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_RTCM;
    } else {
      // printf("Unknown character");
    }
  }

  if (gnss_cfg_data->current_sentence == SL_MAX_M10S_UBLOX_SENTENCE_TYPE_UBX) {
    if ((gnss_cfg_data->ubx_frame_counter == 0) && (incoming != UBX_SYNCH_1)) {
      gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE;
    } else if ((gnss_cfg_data->ubx_frame_counter == 1)
               && (incoming != UBX_SYNCH_2)) {
      gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE;
    } else if (gnss_cfg_data->ubx_frame_counter == 2) {
      gnss_cfg_data->packet_buf.cls = incoming;
      gnss_cfg_data->rolling_checksum_a = 0;
      gnss_cfg_data->rolling_checksum_b = 0;
      gnss_cfg_data->packet_buf.counter = 0;
      gnss_cfg_data->packet_buf.valid =
        SL_MAX_M10S_UBLOX_PACKET_VALIDITY_NOT_DEFINED;
      gnss_cfg_data->packet_buf.starting_spot = incomingUBX->starting_spot;
    } else if (gnss_cfg_data->ubx_frame_counter == 3) {
      gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETBUF;
      gnss_cfg_data->packet_buf.id = incoming;

      if (gnss_cfg_data->packet_buf.cls != UBX_CLASS_ACK) {
        bool log_because_auto =
          gnss_max_m10s_auto_lookup(gnss_cfg_data,
                                    &maxPayload);

        if ((gnss_cfg_data->packet_buf.cls == stored_class)
            && (gnss_cfg_data->packet_buf.id == stored_id)) {
          gnss_cfg_data->active_packet_buffer =
            SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;
          incomingUBX->cls = gnss_cfg_data->packet_buf.cls;
          incomingUBX->id = gnss_cfg_data->packet_buf.id;
          incomingUBX->counter = gnss_cfg_data->packet_buf.counter;
        } else if (log_because_auto) {
          if (gnss_cfg_data->msg_data.payload_auto != NULL) {
            free(gnss_cfg_data->msg_data.payload_auto);

            gnss_cfg_data->msg_data.payload_auto = NULL;
            gnss_cfg_data->packet_auto.payload =
              gnss_cfg_data->msg_data.payload_auto;
          }

          if ((!log_because_auto)) {
            maxPayload = UBX_MAX_LENGTH;
          }

          gnss_cfg_data->packet_auto.payload = (uint8_t *)malloc(
            maxPayload * sizeof(uint8_t));

          if (gnss_cfg_data->packet_auto.payload == NULL) {
            gnss_cfg_data->active_packet_buffer =
              SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;
            incomingUBX->cls = gnss_cfg_data->packet_buf.cls;
            incomingUBX->id = gnss_cfg_data->packet_buf.id;
            incomingUBX->counter = gnss_cfg_data->packet_buf.counter;
          } else {
            gnss_cfg_data->active_packet_buffer =
              SL_MAX_M10S_UBLOX_PACKET_PACKETAUTO;
            gnss_cfg_data->packet_auto.cls = gnss_cfg_data->packet_buf.cls;
            gnss_cfg_data->packet_auto.id = gnss_cfg_data->packet_buf.id;
            gnss_cfg_data->packet_auto.counter =
              gnss_cfg_data->packet_buf.counter;
            gnss_cfg_data->packet_auto.starting_spot =
              gnss_cfg_data->packet_buf.starting_spot;
          }
        } else {
          gnss_cfg_data->ignore_this_payload = true;
        }
      }
    } else if (gnss_cfg_data->ubx_frame_counter == 4) {
      gnss_cfg_data->packet_buf.len = incoming;
    } else if (gnss_cfg_data->ubx_frame_counter == 5) {
      gnss_cfg_data->packet_buf.len |= incoming << 8;
    } else if (gnss_cfg_data->ubx_frame_counter == 6) {
      if (gnss_cfg_data->packet_buf.len == 0) {
        gnss_cfg_data->packet_buf.checksum_a = incoming;
      } else {
        gnss_cfg_data->packet_buf.payload[0] = incoming;
      }
    } else if (gnss_cfg_data->ubx_frame_counter == 7) {
      if (gnss_cfg_data->packet_buf.len == 0) {
        gnss_cfg_data->packet_buf.checksum_b = incoming;
      } else if (gnss_cfg_data->packet_buf.len == 1) {
        gnss_cfg_data->packet_buf.checksum_a = incoming;
      } else {     /// Length is >= 2 so this must be a payload byte
        gnss_cfg_data->packet_buf.payload[1] = incoming;
      }

      /// if a packet is already in process or ack, class or ID of the packet matches
      if ((gnss_cfg_data->active_packet_buffer
           == SL_MAX_M10S_UBLOX_PACKET_PACKETBUF)
          && (gnss_cfg_data->packet_buf.cls == UBX_CLASS_ACK)
          && (gnss_cfg_data->packet_buf.payload[0] == stored_class)
          && (gnss_cfg_data->packet_buf.payload[1] == stored_id)) {
        /// Check if .len is 2
        if (gnss_cfg_data->packet_buf.len == 2) {
          gnss_cfg_data->active_packet_buffer =
            SL_MAX_M10S_UBLOX_PACKET_PACKETACK;
          gnss_cfg_data->packet_ack.cls = gnss_cfg_data->packet_buf.cls;
          gnss_cfg_data->packet_ack.id = gnss_cfg_data->packet_buf.id;
          gnss_cfg_data->packet_ack.len = gnss_cfg_data->packet_buf.len;
          gnss_cfg_data->packet_ack.counter = gnss_cfg_data->packet_buf.counter;
          gnss_cfg_data->packet_ack.payload[0] =
            gnss_cfg_data->packet_buf.payload[0];
          gnss_cfg_data->packet_ack.payload[1] =
            gnss_cfg_data->packet_buf.payload[1];
        }
      }
    }

    gnss_max_m10s_process_ubx(gnss_cfg_data, incoming, stored_class, stored_id);

    gnss_cfg_data->ubx_frame_counter++;
  } else if (gnss_cfg_data->current_sentence
             == SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NMEA) { /// Process incoming NMEA mesages.
    if ((gnss_cfg_data->nmea_byte_counter == 0) && (incoming != '$')) {
      gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE; /// Reset.
    } else if ((gnss_cfg_data->nmea_byte_counter == 1) && (incoming != 'G')) {
      gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE; /// Reset.
    } else if ((gnss_cfg_data->nmea_byte_counter >= 0)
               && (gnss_cfg_data->nmea_byte_counter <= 5)) {
      gnss_cfg_data->nmea_address_field[gnss_cfg_data->nmea_byte_counter] =
        incoming; /// Store the start character and NMEA address field
    }

    if (gnss_cfg_data->nmea_byte_counter == 5) {
      /// set gnss_cfg_data->signs_of_life to true if the NMEA header is valid
      if (!gnss_cfg_data->signs_of_life) {
        gnss_cfg_data->signs_of_life = gnss_max_m10s_is_nmea_header_valid(
          gnss_cfg_data);
      }

      /// Check if automatic storage is available for this message
      if (gnss_max_m10s_is_nmea_auto(gnss_cfg_data)) {
        uint8_t *length_ptr =
          gnss_max_m10s_get_nmea_working_length_ptr(gnss_cfg_data); /// Get a pointer to the working copy length
        uint8_t *nmea_ptr = gnss_max_m10s_get_nmea_working_ptr(gnss_cfg_data); /// Get a pointer to the working copy NMEA data
        uint8_t nmea_max_length = gnss_max_m10s_get_nmea_max_length(
          gnss_cfg_data);
        *length_ptr = 6; /// Set the working copy length

        memset(nmea_ptr, 0, nmea_max_length); /// Clear the working copy

        /// Copy the start character and address field into the working copy
        memcpy(nmea_ptr, &gnss_cfg_data->nmea_address_field[0], 6);
      }

      /// Check if it should be passed to processNMEA
      if (gnss_max_m10s_process_current_nmea(gnss_cfg_data)) {
        for (uint8_t i = 0; i < 6; i++) {
          /// Process the start character and address field
          gnss_max_m10s_process_nmea(gnss_cfg_data->nmea_address_field[i],
                                     gnss_cfg_data->nmea_data);
        }
      }
    }

    if ((gnss_cfg_data->nmea_byte_counter > 5)
        || (gnss_cfg_data->nmea_byte_counter < 0)) {
      if (gnss_max_m10s_is_nmea_auto(gnss_cfg_data)) {
        uint8_t *length_ptr =
          gnss_max_m10s_get_nmea_working_length_ptr(gnss_cfg_data); /// Get a pointer to the working copy length
        uint8_t *nmea_ptr = gnss_max_m10s_get_nmea_working_ptr(gnss_cfg_data); /// Get a pointer to the working copy NMEA data
        uint8_t nmea_max_length = gnss_max_m10s_get_nmea_max_length(
          gnss_cfg_data);

        if (*length_ptr < nmea_max_length) {
          *(nmea_ptr + *length_ptr) = incoming;         /// Store the character
          *length_ptr = *length_ptr + 1;                /// Increment the length

          if (*length_ptr == nmea_max_length) {
            /// For Debugging:

            // printf("Process: NMEA  buffer is full\r\n");
          }
        }
      }

      if (gnss_max_m10s_process_current_nmea(gnss_cfg_data)) {
        gnss_max_m10s_process_nmea(incoming,
                                   gnss_cfg_data->nmea_data);
      }
    }

    if (incoming == '*') {
      gnss_cfg_data->nmea_byte_counter = -5;
    }

    gnss_cfg_data->nmea_byte_counter++;

    if (gnss_cfg_data->nmea_byte_counter
        == gnss_cfg_data->max_nmea_byte_count) {
      gnss_cfg_data->nmea_byte_counter = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE;
    }

    if (gnss_cfg_data->nmea_byte_counter == 0) {
      if (gnss_max_m10s_is_nmea_auto(gnss_cfg_data)) {
        uint8_t *working_length_ptr =
          gnss_max_m10s_get_nmea_working_length_ptr(gnss_cfg_data); /// Get a pointer to the working copy length
        uint8_t *working_nmea_ptr =
          gnss_max_m10s_get_nmea_working_ptr(gnss_cfg_data); /// Get a pointer to the working copy NMEA data
        uint8_t nmea_max_length = gnss_max_m10s_get_nmea_max_length(
          gnss_cfg_data);

        /// Check the checksum
        uint8_t nmea_checksum = 0;
        uint8_t chars_checked = 1; /// Start after the $
        uint8_t this_char = '\0';

        while ((chars_checked < (nmea_max_length - 1))
               && (chars_checked < ((*working_length_ptr) - 4))
               && (this_char != '*')) {
          this_char = *(working_nmea_ptr + chars_checked); /// Get a char from the working copy
          /// Ex-or the char into the checksum except '*'
          if (this_char != '*') {
            nmea_checksum ^= this_char;
          }
          chars_checked++;
        }

        if (this_char == '*') {
          uint8_t expected_checksum_1 = (nmea_checksum >> 4) + '0';

          if (expected_checksum_1 >= ':') {
            expected_checksum_1 += 'A' - ':';
          }
          uint8_t expected_checksum_2 = (nmea_checksum & 0x0F) + '0';

          if (expected_checksum_2 >= ':') {
            expected_checksum_2 += 'A' - ':';
          }

          if ((expected_checksum_1 == *(working_nmea_ptr + chars_checked))
              && (expected_checksum_2
                  == *(working_nmea_ptr + chars_checked + 1))) {
            uint8_t *complete_length_ptr =
              gnss_max_m10s_get_nmea_complete_length_ptr(gnss_cfg_data); /// Get a pointer to the complete copy length
            uint8_t *complete_nmea_ptr =
              gnss_max_m10s_get_complete_copy_nmea(gnss_cfg_data); /// Get a pointer to the complete copy NMEA data

            memset(complete_nmea_ptr, 0, nmea_max_length); /// Clear the previous complete copy
            memcpy(complete_nmea_ptr, working_nmea_ptr, *working_length_ptr); /// Copy the working copy into the complete copy

            *complete_length_ptr = *working_length_ptr; /// Update the length
            sl_max_m10s_nmea_automatic_flags_t *flags_ptr =
              gnss_max_m10s_get_nmea_flags_ptr(gnss_cfg_data); /// Get a pointer to the flags
            sl_max_m10s_nmea_automatic_flags_t flags_copy = *flags_ptr;

            flags_copy.flags.bits.complete_copy_valid = 1; /// Set the complete copy valid flag
            flags_copy.flags.bits.complete_copy_read = 0; /// Clear the complete copy read flag

            *flags_ptr = flags_copy; /// Update the flags
          }
        }
      }
      gnss_cfg_data->current_sentence = SL_MAX_M10S_UBLOX_SENTENCE_TYPE_NONE; // All done!
    }
  }
}

uint32_t gnss_max_m10s_milli_sec()
{
  uint32_t tickcount = sl_sleeptimer_get_tick_count();
  return sl_sleeptimer_tick_to_ms(tickcount);
}

static uint16_t gnss_max_m10s_file_buffer_space_available(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  return (FILE_BUFFER_SIZE - gnss_max_m10s_file_buffer_used(gnss_cfg_data));
}

static uint16_t gnss_max_m10s_file_buffer_used(
  sl_max_m10s_cfg_data_t *gnss_cfg_data)
{
  if (gnss_cfg_data->file_buffer_head >= gnss_cfg_data->file_buffer_tail) {
    return (gnss_cfg_data->file_buffer_head - gnss_cfg_data->file_buffer_tail);
  } else {
    return ((uint16_t)(((uint32_t)(gnss_cfg_data->file_buffer_head)
                        + (uint32_t)FILE_BUFFER_SIZE)
                       - (uint32_t)(gnss_cfg_data->file_buffer_tail)));
  }
}

bool gnss_max_m10s_store_packet(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                sl_max_m10s_ubx_packet_t *msg)
{
  uint16_t totalLength;
  uint8_t sync_chars[] = { UBX_SYNCH_1, UBX_SYNCH_2 }; /// Store the two sync chars
  uint8_t msg_length[2];

  if ((gnss_cfg_data->ubx_file_buffer == NULL) || (FILE_BUFFER_SIZE == 0)) {
    return false;
  }

  totalLength = msg->len + 8; /// Total length. Include sync chars, class, id, length and checksum bytes

  if (totalLength > gnss_max_m10s_file_buffer_space_available(gnss_cfg_data)) {
    return false;
  }

  gnss_max_m10s_write_file_buffer(gnss_cfg_data, sync_chars, 2);

  /// Store the Class & ID
  gnss_max_m10s_write_file_buffer(gnss_cfg_data, &msg->cls, 1);
  gnss_max_m10s_write_file_buffer(gnss_cfg_data, &msg->id, 1);

  /// Store the length
  msg_length[0] = msg->len & 0xFF;
  msg_length[1] = msg->len >> 8;

  gnss_max_m10s_write_file_buffer(gnss_cfg_data, msg_length, 2);

  /// Store the payload
  gnss_max_m10s_write_file_buffer(gnss_cfg_data, msg->payload, msg->len);

  /// Store the checksum
  gnss_max_m10s_write_file_buffer(gnss_cfg_data, &msg->checksum_a, 1);
  gnss_max_m10s_write_file_buffer(gnss_cfg_data, &msg->checksum_b, 1);

  return true;
}

static void gnss_max_m10s_write_file_buffer(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t *the_bytes,
  uint16_t num_bytes)
{
  uint16_t bytes_before_wrapped_around = FILE_BUFFER_SIZE
                                         - gnss_cfg_data->file_buffer_head;
  uint16_t bytes_left_to_copy;
  uint16_t bytes_in_buffer;

  if (bytes_before_wrapped_around > num_bytes) {
    bytes_before_wrapped_around = num_bytes;
  }

  /// Copy the data into the buffer
  memcpy(&(gnss_cfg_data->ubx_file_buffer[gnss_cfg_data->file_buffer_head]),
         the_bytes,
         bytes_before_wrapped_around);

  bytes_left_to_copy = num_bytes - bytes_before_wrapped_around;

  if (bytes_left_to_copy > 0) {
    memcpy(&(gnss_cfg_data->ubx_file_buffer[0]),
           &the_bytes[bytes_before_wrapped_around],
           bytes_left_to_copy);
    gnss_cfg_data->file_buffer_head = bytes_left_to_copy;
  } else {
    gnss_cfg_data->file_buffer_head += num_bytes;
  }

  bytes_in_buffer = gnss_max_m10s_file_buffer_used(gnss_cfg_data);

  if (bytes_in_buffer > gnss_cfg_data->file_buffer_max_avail) {
    gnss_cfg_data->file_buffer_max_avail = bytes_in_buffer;
  }
}

sl_status_t gnss_max_m10s_set_i2c_output(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint8_t comSettings,
                                         uint8_t layer,
                                         uint16_t max_wait)
{
  uint8_t value;
  sl_status_t result;

  result = gnss_max_m10s_new_cfg_valset(gnss_cfg_data, layer);

  value = (uint8_t) (comSettings & COM_TYPE_UBX) == 0 ? 0 : 1;
  result &= gnss_max_m10s_add_cfg_valset(gnss_cfg_data,
                                         UBLOX_CFG_I2C_OUTPORT_UBX,
                                         &value, 1);

  value = (uint8_t) (comSettings & COM_TYPE_NMEA) == 0 ? 0 : 1;
  result &= gnss_max_m10s_add_cfg_valset(gnss_cfg_data,
                                         UBLOX_CFG_I2C_OUTPORT_NMEA,
                                         &value, 1);

  result &= gnss_max_m10s_send_cfg_valset(gnss_cfg_data, max_wait);

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_new_cfg_valset(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint8_t layer)
{
  gnss_cfg_data->packet_cfg.cls = UBX_CLASS_CFG;
  gnss_cfg_data->packet_cfg.id = UBX_CFG_VALSET;
  gnss_cfg_data->packet_cfg.len = 4;
  gnss_cfg_data->packet_cfg.starting_spot = 0;
  gnss_cfg_data->num_cfg_keys = 0;

  memset(gnss_cfg_data->msg_data.payload_cfg,
         0,
         gnss_cfg_data->msg_data.packet_cfg_payloadsize);

  gnss_cfg_data->msg_data.payload_cfg[0] = 0;
  gnss_cfg_data->msg_data.payload_cfg[1] = layer;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_add_cfg_valset(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint32_t key,
  uint8_t *value,
  uint8_t N)
{
  gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

  if ((gnss_cfg_data->auto_send_at_space_remaining > 0)
      && (gnss_cfg_data->packet_cfg.len
          >= (gnss_cfg_data->msg_data.packet_cfg_payloadsize
              - gnss_cfg_data->auto_send_at_space_remaining))) {
    if (gnss_max_m10s_send_command(gnss_cfg_data,
                                   GNSS_POLL_MAX_TIMEOUT,
                                   false)
        != SL_MAX_M10S_UBLOX_STATUS_DATA_SENT) {
      return SL_STATUS_FAIL;
    }

    gnss_cfg_data->packet_cfg.len = 4;
    gnss_cfg_data->packet_cfg.starting_spot = 0;
    gnss_cfg_data->num_cfg_keys = 0;

    memset(&(gnss_cfg_data->msg_data.payload_cfg[4]),
           0,
           gnss_cfg_data->msg_data.packet_cfg_payloadsize - 4);
  }

  if (gnss_cfg_data->packet_cfg.len
      >= (gnss_cfg_data->msg_data.packet_cfg_payloadsize - (4 + N))) {
    return SL_STATUS_FAIL;
  }

  if (gnss_cfg_data->num_cfg_keys == CFG_VALSET_MAX_KEYS) {
    return SL_STATUS_FAIL;
  }

  key &= ~UBX_CFG_SIZE_MASK;

  for (uint8_t i = 0; i < 4; i++) {
    gnss_cfg_data->msg_data.payload_cfg[gnss_cfg_data->packet_cfg.len
                                        + i] = key >> (8 * i);
  }

  for (uint8_t i = 0; i < N; i++) {
    gnss_cfg_data->msg_data.payload_cfg[gnss_cfg_data->packet_cfg.len + i
                                        + 4] = *value++;
  }

  gnss_cfg_data->packet_cfg.len = gnss_cfg_data->packet_cfg.len + 4 + N;
  gnss_cfg_data->num_cfg_keys++;

  return SL_STATUS_OK;
}

static sl_status_t gnss_max_m10s_send_cfg_valset(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait)
{
  gnss_cfg_data->active_packet_buffer = SL_MAX_M10S_UBLOX_PACKET_PACKETCFG;

  if (0 == gnss_cfg_data->num_cfg_keys) {
    return SL_STATUS_OK;
  }

  bool success = gnss_max_m10s_send_command(gnss_cfg_data,
                                            max_wait,
                                            false)
                 == SL_MAX_M10S_UBLOX_STATUS_DATA_SENT;

  if (success) {
    gnss_cfg_data->num_cfg_keys = 0;
  }

  return SL_STATUS_OK;
}

sl_status_t gnss_max_m10s_get_nav_pvt(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                      uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navstatus(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_navstatus_ubx(gnss_cfg_data, max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navepoch(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_navepoch_ubx(gnss_cfg_data,
                                          max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navsat(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    if (SL_STATUS_OK
        == gnss_max_m10s_set_packet_cfg_payload_size(gnss_cfg_data,
                                                     UBX_NAV_SAT_LEN)) {
      return gnss_max_m10s_get_navsat_ubx(gnss_cfg_data, max_wait);
    } else {
      return SL_STATUS_FAIL;
    }
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navsig(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    if (SL_STATUS_OK == gnss_max_m10s_set_packet_cfg_payload_size(gnss_cfg_data,
                                                                  UBX_NAV_SAT_LEN))
    {
      return gnss_max_m10s_get_navsig_ubx(gnss_cfg_data, max_wait);
    } else {
      return SL_STATUS_FAIL;
    }
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navdop(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_navdop_ubx(gnss_cfg_data, max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navposllh(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_navposllh_ubx(gnss_cfg_data, max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navtimels(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_navtimels_ubx(gnss_cfg_data, max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navtimeutc(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                         uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_navtimeutc_ubx(gnss_cfg_data, max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navclock(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_navclock_ubx(gnss_cfg_data, max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navvelned(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait)
{
  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_navvelned_ubx(gnss_cfg_data, max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_fix_type(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait,
                                       uint8_t *fix_type)
{
  sl_status_t status = SL_STATUS_OK;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *fix_type = gnss_cfg_data->packetUBXNAVPVT->data.fixType;
  } else {
    status = gnss_max_m10s_get_quality_nmea(gnss_cfg_data, fix_type, max_wait);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_satellite_number(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  uint8_t *satellite)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *satellite = gnss_cfg_data->packetUBXNAVPVT->data.numSV;
  } else {
    status =
      gnss_max_m10s_get_satellite_nmea(gnss_cfg_data, satellite, max_wait);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_longitude(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait,
                                        int32_t *longitude)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *longitude = gnss_cfg_data->packetUBXNAVPVT->data.lon;
  } else {
    status =
      gnss_max_m10s_get_longitude_nmea(gnss_cfg_data, longitude, max_wait);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_latitude(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait,
                                       int32_t *latitude)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *latitude = gnss_cfg_data->packetUBXNAVPVT->data.lat;
  } else {
    status = gnss_max_m10s_get_latitude_nmea(gnss_cfg_data, latitude, max_wait);
  }

  return status;
}

sl_status_t gnss_max_m10s_get_altitude_msl(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *altitude_msl)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *altitude_msl = gnss_cfg_data->packetUBXNAVPVT->data.hMSL;
  } else {
    status = gnss_max_m10s_get_altitude_msl_nmea(gnss_cfg_data,
                                                 altitude_msl,
                                                 max_wait);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_altitude(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait,
                                       int32_t *altitude)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *altitude = gnss_cfg_data->packetUBXNAVPVT->data.height;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_carrier_solution(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int8_t *carrier_solution)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *carrier_solution =
      gnss_cfg_data->packetUBXNAVPVT->data.solution_flags.bits.carrSoln;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_differential_solution(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *differential_solution)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *differential_solution =
      (bool)gnss_cfg_data->packetUBXNAVPVT->data.solution_flags.bits.diffSoln;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_head_vehicle_valid(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *head_vehicle_valid)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *head_vehicle_valid =
      (bool)gnss_cfg_data->packetUBXNAVPVT->data.solution_flags.bits.
      headVehValid;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_gnss_fix_ok(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                          uint16_t max_wait,
                                          bool *gnss_fix_ok)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *gnss_fix_ok =
      (bool)gnss_cfg_data->packetUBXNAVPVT->data.solution_flags.bits.gnssFixOK;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_confirm_time(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *confirm_time)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *confirm_time =
      (bool)gnss_cfg_data->packetUBXNAVPVT->data.confirm_flags.confirm_data.
      confirmed_time;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_confirm_date(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *confirm_date)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *confirm_date =
      (bool)gnss_cfg_data->packetUBXNAVPVT->data.confirm_flags.confirm_data.
      confirmed_date;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_fully_resolved(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *fully_resolved)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *fully_resolved =
      (bool)gnss_cfg_data->packetUBXNAVPVT->data.valid.bits.fullyResolved;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_invalid_data(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  bool *invalid_data)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *invalid_data =
      (bool)gnss_cfg_data->packetUBXNAVPVT->data.correction_flags.bits.
      invalidLlh;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_time_of_week(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  uint32_t *time_of_week)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *time_of_week = gnss_cfg_data->packetUBXNAVPVT->data.iTOW;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_horizontal_accuracy(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *horizontal_accuracy)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *horizontal_accuracy = gnss_cfg_data->packetUBXNAVPVT->data.hAcc;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_vertical_accuracy(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *vertical_accuracy)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *vertical_accuracy = gnss_cfg_data->packetUBXNAVPVT->data.vAcc;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_ned_north(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                        uint16_t max_wait,
                                        int32_t *ned_north)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *ned_north = gnss_cfg_data->packetUBXNAVPVT->data.velN;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_ned_east(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait,
                                       int32_t *ned_east)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *ned_east = gnss_cfg_data->packetUBXNAVPVT->data.velE;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_ned_down(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                       uint16_t max_wait,
                                       int32_t *ned_down)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *ned_down = gnss_cfg_data->packetUBXNAVPVT->data.velD;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_magnetic_declination(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int16_t *magnetic_declination)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *magnetic_declination = gnss_cfg_data->packetUBXNAVPVT->data.magDec;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_magnetic_declination_accuracy(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  uint16_t *magnetic_accuracy)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *magnetic_accuracy = gnss_cfg_data->packetUBXNAVPVT->data.magAcc;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_heading_of_motion(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *head_of_motion)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *head_of_motion = gnss_cfg_data->packetUBXNAVPVT->data.headMot;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_ground_speed(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *ground_speed)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *ground_speed = gnss_cfg_data->packetUBXNAVPVT->data.gSpeed;
  } else {
    status =
      gnss_max_m10s_get_speed_nmea(gnss_cfg_data, ground_speed, max_wait);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_speed_accuracy(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *speed_accuracy)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *speed_accuracy = gnss_cfg_data->packetUBXNAVPVT->data.sAcc;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_heading_accuracy(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *heading_accuracy)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *heading_accuracy = gnss_cfg_data->packetUBXNAVPVT->data.headAcc;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_positional_dillution(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int16_t *position_dillution)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *position_dillution = gnss_cfg_data->packetUBXNAVPVT->data.pDOP;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_head_vehicle(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *head_vehicle)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *head_vehicle = gnss_cfg_data->packetUBXNAVPVT->data.headVeh;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_day(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                  uint16_t max_wait,
                                  uint8_t *day)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *day = gnss_cfg_data->packetUBXNAVPVT->data.day;
  } else {
    status = gnss_max_m10s_get_day_nmea(gnss_cfg_data, day, max_wait);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_month(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    uint16_t max_wait,
                                    uint8_t *month)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *month = gnss_cfg_data->packetUBXNAVPVT->data.month;
  } else {
    status = gnss_max_m10s_get_month_nmea(gnss_cfg_data, month, max_wait);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_year(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                   uint16_t max_wait,
                                   uint16_t *year)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *year = gnss_cfg_data->packetUBXNAVPVT->data.year;
  } else {
    status = gnss_max_m10s_get_year_nmea(gnss_cfg_data, year, max_wait);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_hour(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                   uint16_t max_wait,
                                   uint8_t *hour)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *hour = gnss_cfg_data->packetUBXNAVPVT->data.hour;
  } else {
    status = gnss_max_m10s_get_hour_nmea(gnss_cfg_data, hour, max_wait);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_minute(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait,
                                     uint8_t *minute)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *minute = gnss_cfg_data->packetUBXNAVPVT->data.min;
  } else {
    status = gnss_max_m10s_get_min_nmea(gnss_cfg_data, minute, max_wait);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_seconds(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                      uint16_t max_wait,
                                      uint8_t *seconds)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *seconds = gnss_cfg_data->packetUBXNAVPVT->data.sec;
  } else {
    status = gnss_max_m10s_get_second_nmea(gnss_cfg_data, seconds, max_wait);
  }
  return status;
}

sl_status_t gnss_max_m10s_get_milliseconds(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  uint16_t *milli_seconds)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *milli_seconds = gnss_cfg_data->packetUBXNAVPVT->data.iTOW % 1000;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_nanoseconds(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                          uint16_t max_wait,
                                          int32_t *nano)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *nano = gnss_cfg_data->packetUBXNAVPVT->data.nano;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_valid_time(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait,
                                     bool *valid_time)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *valid_time = gnss_cfg_data->packetUBXNAVPVT->data.valid.bits.valid_time;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_valid_date(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                     uint16_t max_wait,
                                     bool *valid_date)
{
  sl_status_t status;

  if (gnss_cfg_data->protocol_type != SL_MAX_M10S_PROTOCOL_NMEA) {
    status = gnss_max_m10s_get_navpvt_ubx(gnss_cfg_data, max_wait);

    *valid_date = gnss_cfg_data->packetUBXNAVPVT->data.valid.bits.valid_date;
  } else {
    status = SL_STATUS_NOT_AVAILABLE;
  }
  return status;
}

sl_status_t gnss_max_m10s_get_geoid(sl_max_m10s_cfg_data_t *gnss_cfg_data,
                                    uint16_t max_wait,
                                    int32_t *geoid)
{
  if (gnss_cfg_data->protocol_type == SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_geoid_nmea(gnss_cfg_data, geoid, max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_horizontal_dilution(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  uint8_t *hdop)
{
  if (gnss_cfg_data->protocol_type == SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_horizontal_dilution_nmea(gnss_cfg_data,
                                                      hdop,
                                                      max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_course_over_ground(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  int32_t *course)
{
  if (gnss_cfg_data->protocol_type == SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_ground_course_nmea(gnss_cfg_data, course,
                                                max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}

sl_status_t gnss_max_m10s_get_navigation_system(
  sl_max_m10s_cfg_data_t *gnss_cfg_data,
  uint16_t max_wait,
  char *nav_system)
{
  if (gnss_cfg_data->protocol_type == SL_MAX_M10S_PROTOCOL_NMEA) {
    return gnss_max_m10s_get_nav_system_nmea(gnss_cfg_data, nav_system,
                                             max_wait);
  } else {
    return SL_STATUS_NOT_AVAILABLE;
  }
}
