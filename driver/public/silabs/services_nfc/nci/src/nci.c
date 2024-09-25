/***************************************************************************//**
 * @file   nci.c
 * @brief  Implementation for an event based NCI.
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

#include <string.h>
#include <stdint.h>
#include "nci.h"
#include "nci_tml.h"

#define NCI_PACKET_HEADER_LEN             (3)
#define NCI_PACKET_PAYLOAD_MAX_LEN        (255)
#define NCI_PACKET_MAX_LEN \
  (NCI_PACKET_HEADER_LEN + NCI_PACKET_PAYLOAD_MAX_LEN)

static uint16_t nci_tml_mtu = NCI_PACKET_MAX_LEN;
static uint8_t packet_buff[NCI_PACKET_MAX_LEN];
static nci_evt_t nci_evt;
static volatile bool packet_ready = false;
static bool startup_event_passed = false;

extern nci_err_t nci_incoming_proprietary_packet_rsp_process(
  uint8_t *packet_buff);

extern nci_err_t nci_incoming_proprietary_packet_ntf_process(
  uint8_t *packet_buff);

/***************************************************************************//**
 * @brief
 *  Initialize NCI.
 ******************************************************************************/
void nci_init(void)
{
  nci_evt.header = nci_evt_startup;
}

/***************************************************************************//**
 * @brief
 *  Get current NCI event.
 *
 * @returns
 *  Pointer to current NCI event.
 ******************************************************************************/
nci_evt_t *nci_get_event(void)
{
  if (startup_event_passed) {
    nci_err_t nci_packet_process_err = nci_incoming_packet_process();

    if (nci_packet_process_err != nci_err_none) {
      nci_log_ln("NCI packet process error.");
    }
  } else {
    startup_event_passed = true;
  }

  return &nci_evt;
}

/***************************************************************************//**
 * @brief
 *  Calling this function would notify the NCI stack that a packet is incoming.
 *  Should be called in the NFCC interrupt ISR.
 ******************************************************************************/
void nci_notify_incoming_packet(void)
{
  packet_ready = true;
}

/***************************************************************************//**
 * @brief
 *  Check if there is any incoming packet waiting to be read.
 *
 * @returns
 *  true  - if there is an incoming packet to be read.
 *  false - if there is no incoming packet to be read.
 ******************************************************************************/
// bool nci_check_incoming_packet(void)
// {
//  return packet_incoming;
// }

/***************************************************************************//**
 * @brief
 *  Encode and send a NCI control packet.
 *
 * @param[in] packet
 *  NCI control packet to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_control_packet_send(nci_control_packet_t *packet)
{
  if (packet->payload_len > nci_tml_mtu) {
    nci_log_ln("NCI error: payload exceed MTU.");
    return nci_err_payload_exceed_mtu;
  }

  packet_buff[NCI_PACKET_MT_INDEX] &= ~NCI_PACKET_MT_M;
  packet_buff[NCI_PACKET_MT_INDEX] += (packet->mt << NCI_PACKET_MT_SHIFT);
  packet_buff[NCI_PACKET_PBF_INDEX] &= ~NCI_PACKET_PBF_M;
  packet_buff[NCI_PACKET_PBF_INDEX] += (packet->pbf << NCI_PACKET_PBF_SHIFT);
  packet_buff[NCI_PACKET_GID_INDEX] &= ~NCI_PACKET_GID_M;
  packet_buff[NCI_PACKET_GID_INDEX] += (packet->gid << NCI_PACKET_GID_SHIFT);
  packet_buff[NCI_PACKET_OID_INDEX] &= ~NCI_PACKET_OID_M;
  packet_buff[NCI_PACKET_OID_INDEX] += (packet->oid << NCI_PACKET_OID_SHIFT);
  packet_buff[NCI_PACKET_PAYLOAD_LEN_INDEX] = packet->payload_len;

  memcpy(&packet_buff[NCI_PACKET_PAYLOAD_START_INDEX],
         packet->payload,
         packet->payload_len);

  nci_tml_err_t nci_tml_err = nci_tml_transceive(packet_buff);

  if (nci_tml_err != nci_tml_err_none) {
    return nci_err_tml;
  }

  nci_tml_log("NCI TML transceive: ");
  nci_tml_packet_log(packet_buff, packet_buff[2] + 3);
  nci_tml_log_ln(" ");

  return nci_err_none;
}

/***************************************************************************//**
 * @brief
 *  Encode and send a NCI data packet.
 *
 * @param[in] packet
 *  NCI data packet to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_data_packet_send(nci_data_packet_t *packet)
{
  if (packet->payload_len > nci_tml_mtu) {
    nci_log_ln("NCI error: payload exceed MTU.");
    return nci_err_payload_exceed_mtu;
  }

  packet_buff[NCI_PACKET_MT_INDEX] &= ~NCI_PACKET_MT_M;
  packet_buff[NCI_PACKET_MT_INDEX]
    += (nci_packet_data << NCI_PACKET_MT_SHIFT);
  packet_buff[NCI_PACKET_PBF_INDEX] &= ~NCI_PACKET_PBF_M;
  packet_buff[NCI_PACKET_PBF_INDEX] += (packet->pbf << NCI_PACKET_PBF_SHIFT);
  packet_buff[NCI_PACKET_CONN_ID_INDEX] &= ~NCI_PACKET_CONN_ID_M;
  packet_buff[NCI_PACKET_CONN_ID_INDEX]
    += (packet->conn_id << NCI_PACKET_CONN_ID_SHIFT);

  /* Clear previous leftover OLD in the buffer. */
  packet_buff[NCI_PACKET_OID_INDEX] &= ~NCI_PACKET_OID_M;
  packet_buff[NCI_PACKET_PAYLOAD_LEN_INDEX] = packet->payload_len;

  memcpy(&packet_buff[NCI_PACKET_PAYLOAD_START_INDEX],
         packet->payload,
         packet->payload_len);

  if (nci_tml_transceive(packet_buff) != nci_tml_err_none) {
    return nci_err_tml;
  }

  nci_tml_log("NCI TML transceive: ");
  nci_tml_packet_log(packet_buff, packet_buff[2] + 3);
  nci_tml_log_ln(" ");

  return nci_err_none;
}

// -----------------------------------------------------------------------------
// NCI Core

// -------------------------------
// CORE_RESET

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_RESET_CMD.
 *
 * @param[in] cmd
 *  NCI CORE_RESET command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_reset(nci_core_reset_cmd_t *cmd)
{
  uint8_t payload[1];

  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_nci_core;
  packet.oid = nci_oid_core_reset;
  packet.payload_len = 1;
  packet.payload = payload;

  payload[0] = cmd->reset_type;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load CORE_RESET_RSP to nci event data.
 ******************************************************************************/
static void nci_core_reset_rsp_process(void)
{
  nci_evt.header = nci_evt_core_reset_rsp_rec;
  nci_evt.data.is_terminator_packet = true;
  nci_evt.data.len = 3;

  uint16_t index = NCI_PACKET_PAYLOAD_START_INDEX;

  nci_evt.data.payload.nci_data.core_reset_rsp.status
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.core_reset_rsp.nci_version
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.core_reset_rsp.config_status
    = packet_buff[index];
}

/***************************************************************************//**
 * @brief
 *  Process and load CORE_RESET_NTF to nci event data.
 ******************************************************************************/
static void nci_core_reset_ntf_process(void)
{
  nci_evt.header = nci_evt_core_reset_ntf_rec;
  nci_evt.data.is_terminator_packet = true;
  nci_evt.data.len = 2;

  uint16_t index = NCI_PACKET_PAYLOAD_START_INDEX;

  nci_evt.data.payload.nci_data.core_reset_ntf.reason_code
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.core_reset_ntf.config_status
    = packet_buff[index];
}

// -------------------------------
// CORE_INIT

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_INIT_CMD.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_init(void)
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_nci_core;
  packet.oid = nci_oid_core_init;
  packet.payload_len = 0;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load CORE_INIT_RSP to nci event data.
 ******************************************************************************/
static void nci_core_init_rsp_process(void)
{
  nci_evt.header = nci_evt_core_init_rsp_rec;
  nci_evt.data.is_terminator_packet = true;
  nci_evt.data.len = packet_buff[NCI_PACKET_PAYLOAD_LEN_INDEX];

  uint16_t index = NCI_PACKET_PAYLOAD_START_INDEX;

  nci_evt.data.payload.nci_data.core_init_rsp.status = packet_buff[index++];
  nci_evt.data.payload.nci_data.core_init_rsp.nfcc_feature
    = &packet_buff[index];
  index += 4;
  nci_evt.data.payload.nci_data.core_init_rsp.num_rf_interface
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.core_init_rsp.rf_interfaces
    = &packet_buff[index];
  index += nci_evt.data.payload.nci_data.core_init_rsp.num_rf_interface;
  nci_evt.data.payload.nci_data.core_init_rsp.max_logical_conn
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.core_init_rsp.max_routing_tab_size
    = &packet_buff[index];
  index += 2;
  nci_evt.data.payload.nci_data.core_init_rsp.max_ctrl_pkt_payload_size
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.core_init_rsp.max_size_large_param
    = &packet_buff[index];
  index += 2;
  nci_evt.data.payload.nci_data.core_init_rsp.manu_id
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.core_init_rsp.manu_spec_info
    = &packet_buff[index];
}

// -------------------------------
// CORE_SET_CONFIG

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_SET_CONFIG_CMD.
 *
 * @param[in] cmd
 *  NCI CORE_SET_CONFIG command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_set_config(nci_core_set_config_cmd_t *cmd)
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_nci_core;
  packet.oid = nci_oid_core_set_config;
  packet.payload_len = 1;

  for (int i = 0; i < cmd->num_of_params; i++) {
    /* Add length of ID and LEN fields. */
    packet.payload_len += 2;

    /* Add length of Val field. */
    packet.payload_len += cmd->param[i].len;
  }

  uint8_t payload[packet.payload_len];

  packet.payload = payload;

  uint8_t payload_index = 0;

  payload[payload_index++] = cmd->num_of_params;

  for (int i = 0; i < cmd->num_of_params; i++) {
    payload[payload_index++] = cmd->param[i].id;
    payload[payload_index++] = cmd->param[i].len;
    memcpy(&payload[payload_index], cmd->param[i].value, cmd->param[i].len);
  }

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load CORE_SET_CONFIG_RSP to nci event data.
 ******************************************************************************/
static void nci_core_set_config_rsp_process(void)
{
  nci_evt.header = nci_evt_core_set_config_rsp_rec;
}

// -------------------------------
// CORE_GET_CONFIG

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_GET_CONFIG_CMD.
 *
 * @param[in] cmd
 *  NCI CORE_GET_CONFIG command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_get_config(nci_core_get_config_cmd_t *cmd)
{
  (void) cmd;

  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load CORE_GET_CONFIG_RSP to nci event data.
 ******************************************************************************/
static void nci_core_get_config_rsp_process(void)
{
  nci_evt.header = nci_evt_core_get_config_rsp_rec;
}

// -------------------------------
// CORE_CONN_CREATE

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_CONN_CREATE_CMD.
 *
 * @param[in] cmd
 *  NCI CORE_CONN_CREATE command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_conn_create(nci_core_conn_create_cmd_t *cmd)
{
  (void) cmd;
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load CORE_CONN_CREATE_RSP to nci event data.
 ******************************************************************************/
static void nci_core_conn_create_rsp_process(void)
{
  nci_evt.header = nci_evt_core_conn_create_rsp_rec;
}

// -------------------------------
// CORE_CONN_CLOSE

/***************************************************************************//**
 * @brief
 *  Encode and send CORE_CONN_CLOSE_CMD.
 *
 * @param[in] cmd
 *  NCI CORE_CONN_CLOSE command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_core_conn_close(nci_core_conn_close_cmd_t *cmd)
{
  (void) cmd;
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load CORE_CONN_CLOSE_RSP to nci event data.
 ******************************************************************************/
static void nci_core_conn_close_rsp_process(void)
{
  nci_evt.header = nci_evt_core_conn_close_rsp_rec;
}

// -------------------------------
// CORE_CONN_CREDITS

/***************************************************************************//**
 * @brief
 *  Process and load CORE_CONN_CREDITS_NTF to nci event data.
 ******************************************************************************/
static void nci_core_conn_credits_ntf_process(void)
{
  nci_evt.header = nci_evt_core_conn_credits_ntf_rec;
}

// -------------------------------
// CORE_GENERIC_ERROR

/***************************************************************************//**
 * @brief
 *  Process and load CORE_GENERIC_ERROR_NTF to nci event data.
 ******************************************************************************/
static void nci_core_generic_error_ntf_process(void)
{
  nci_evt.header = nci_evt_core_generic_error_ntf_rec;
}

// -------------------------------
// CORE_INTERFACE_ERROR

/***************************************************************************//**
 * @brief
 *  Process and load CORE_INTERFACE_ERROR_NTF to nci event data.
 ******************************************************************************/
static void nci_core_interface_error_ntf_process(void)
{
  nci_evt.header = nci_evt_core_interface_error_ntf_rec;
}

// -----------------------------------------------------------------------------
// RF Management

// -------------------------------
// RF_DISCOVER_MAP

/***************************************************************************//**
 * @brief
 *  Encode and send RF_DISCOVER_MAP_CMD.
 *
 * @param[in] cmd
 *  NCI RF_DISCOVER_MAP command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_discover_map(nci_rf_discover_map_cmd_t *cmd)
{
  uint8_t payload[1 + cmd->num_of_mapping_config * 3];

  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_rf_management;
  packet.oid = nci_oid_rf_discover_map;
  packet.payload_len = 1 + cmd->num_of_mapping_config * 3;
  packet.payload = payload;

  payload[0] = cmd->num_of_mapping_config;
  for (int index = 0; index < cmd->num_of_mapping_config; index++) {
    payload[index * 3 + 1] = cmd->mapping_config[index].rf_protocol;
    payload[index * 3 + 2] = cmd->mapping_config[index].mode;
    payload[index * 3 + 3] = cmd->mapping_config[index].rf_interface;
  }

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_DISCOVER_MAP_RSP to nci event data.
 ******************************************************************************/
static void nci_rf_discover_map_rsp_process(void)
{
  nci_evt.header = nci_evt_rf_discover_map_rsp_rec;
  nci_evt.data.is_terminator_packet = true;
  nci_evt.data.len = packet_buff[NCI_PACKET_PAYLOAD_LEN_INDEX];

  uint16_t index = NCI_PACKET_PAYLOAD_START_INDEX;

  nci_evt.data.payload.nci_data.rf_discover_map_rsp.status = packet_buff[index];
}

// -------------------------------
// RF_SET_LISTEN_MODE_ROUTING

/***************************************************************************//**
 * @brief
 *  Encode and send RF_SET_LISTEN_MODE_ROUTING_CMD.
 *
 * @param[in] cmd
 *  NCI RF_SET_LISTEN_MODE_ROUTING command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_set_listen_mode_routing(
  nci_rf_set_listen_mode_routing_cmd_t *cmd)
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_rf_management;
  packet.oid = nci_oid_rf_set_listen_mode_routing;
  packet.payload_len = 2;

  for (int i = 0; i < cmd->num_of_routing_entries; i++) {
    packet.payload_len += 2 + cmd->routing_entries[0].length;
  }

  uint8_t payload[packet.payload_len];

  packet.payload = payload;

  payload[0] = cmd->more_messages;
  payload[1] = cmd->num_of_routing_entries;

  uint8_t payload_index = 2;

  for (int i = 0; i < cmd->num_of_routing_entries; i++) {
    payload[payload_index++] = cmd->routing_entries[i].type;
    payload[payload_index++] = cmd->routing_entries[i].length;
    memcpy(&payload[payload_index],
           &cmd->routing_entries[i].value,
           cmd->routing_entries[i].length);
    payload_index += cmd->routing_entries[i].length;
  }

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_SET_LISTEN_MODE_ROUTING_RSP to nci event data.
 ******************************************************************************/
static void nci_rf_set_listen_mode_routing_rsp_process(void)
{
  nci_evt.header = nci_evt_rf_set_listen_mode_routing_rsp_rec;
}

// -------------------------------
// RF_GET_LISTEN_MODE_ROUTING

/***************************************************************************//**
 * @brief
 *  Encode and send RF_GET_LISTEN_MODE_ROUTING_CMD.
 *
 * @param[in] cmd
 *  NCI RF_GET_LISTEN_MODE_ROUTING command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_get_listen_mode_routing(void)
{
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_GET_LISTEN_MODE_ROUTING_RSP to nci event data.
 ******************************************************************************/
static void nci_rf_get_listen_mode_routing_rsp_process(void)
{
  nci_evt.header = nci_evt_rf_get_listen_mode_routing_rsp_rec;
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_GET_LISTEN_MODE_ROUTING_NTF to nci event data.
 ******************************************************************************/
static void nci_rf_get_listen_mode_routing_ntf_process(void)
{
  nci_evt.header = nci_evt_rf_get_listen_mode_routing_ntf_rec;
}

// -------------------------------
// RF_DISCOVER

/***************************************************************************//**
 * @brief
 *  Encode and send RF_DISCOVER_CMD.
 *
 * @param[in] cmd
 *  NCI RF_DISCOVER command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_discover(nci_rf_discover_cmd_t *cmd)
{
  /* 2 x number of config plus "number of config" byte. */
  uint8_t payload[cmd->num_of_config * 2 + 1];

  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_rf_management;
  packet.oid = nci_oid_rf_discover;
  packet.payload_len = cmd->num_of_config * 2 + 1;
  packet.payload = payload;

  payload[0] = cmd->num_of_config;
  memcpy(&payload[1], cmd->configurations, cmd->num_of_config * 2);

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_DISCOVER_RSP to nci event data.
 ******************************************************************************/
static void nci_rf_discover_rsp_process(void)
{
  nci_evt.header = nci_evt_rf_discover_rsp_rec;
  nci_evt.data.is_terminator_packet = true;
  nci_evt.data.len = packet_buff[NCI_PACKET_PAYLOAD_LEN_INDEX];

  uint16_t index = NCI_PACKET_PAYLOAD_START_INDEX;

  nci_evt.data.payload.nci_data.rf_discover_rsp.status = packet_buff[index];
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_DISCOVER_NTF to nci event data.
 ******************************************************************************/
static void nci_rf_discover_ntf_process(void)
{
  nci_evt.header = nci_evt_rf_discover_ntf_rec;
  nci_evt.data.is_terminator_packet = true;
  nci_evt.data.len = packet_buff[NCI_PACKET_PAYLOAD_LEN_INDEX];

  uint16_t index = NCI_PACKET_PAYLOAD_START_INDEX;

  nci_evt.data.payload.nci_data.rf_discover_ntf.rf_disc_id
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_discover_ntf.rf_protocol
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_and_mode
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param_len
    = packet_buff[index++];

  switch (nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_and_mode) {
    case nci_nfc_a_passive_poll_mode:
      nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
      ->nfc_a_poll_params.sens_res = &packet_buff[index];
      index += 2;
      nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
      ->nfc_a_poll_params.nfcid1_len = packet_buff[index++];
      nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
      ->nfc_a_poll_params.nfcid1 = &packet_buff[index];
      index += nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
               ->nfc_a_poll_params.nfcid1_len;
      nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
      ->nfc_a_poll_params.sel_res_len = packet_buff[index++];
      if (nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
          ->nfc_a_poll_params.sel_res_len == 1) {
        nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
        ->nfc_a_poll_params.sel_res = packet_buff[index++];
      }
      break;
    case nci_nfc_b_passive_poll_mode:

      break;
    case nci_nfc_f_passive_poll_mode:

      break;
    default:
      break;
  }
  nci_evt.data.payload.nci_data.rf_discover_ntf.ntf_type = packet_buff[index];
}

// -------------------------------
// RF_DISCOVER_SELECT

/***************************************************************************//**
 * @brief
 *  Encode and send RF_DISCOVER_SELECT_CMD.
 *
 * @param[in] cmd
 *  NCI RF_DISCOVER_SELECT command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_discover_select(nci_rf_discover_select_cmd_t *cmd)
{
  (void) cmd;
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_DISCOVER_SELECT_RSP to nci event data.
 ******************************************************************************/
static void nci_rf_discover_select_rsp_process(void)
{
  nci_evt.header = nci_evt_rf_discover_select_rsp_rec;
}

// -------------------------------
// RF_INTF_ACTIVATED

/***************************************************************************//**
 * @brief
 *  Process and load RF_INTF_ACTIVATED_NTF to nci event data.
 ******************************************************************************/
static void nci_rf_intf_activated_ntf_process(void)
{
  nci_evt.header = nci_evt_rf_intf_activated_ntf_rec;
  nci_evt.data.is_terminator_packet = true;
  nci_evt.data.len = packet_buff[NCI_PACKET_PAYLOAD_LEN_INDEX];

  uint16_t index = NCI_PACKET_PAYLOAD_START_INDEX;

  nci_evt.data.payload.nci_data.rf_intf_activated_ntf.rf_disc_id
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_intf_activated_ntf.rf_interface
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_intf_activated_ntf.rf_protocol
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_intf_activated_ntf.act_rf_tech_and_mode
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_intf_activated_ntf
  .max_data_packet_payload_size = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_intf_activated_ntf.init_num_of_credits
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_intf_activated_ntf.len_rf_tech_params
    = packet_buff[index++];

  switch (nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_and_mode) {
    case nci_nfc_a_passive_poll_mode:
      nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
      ->nfc_a_poll_params.sens_res = &packet_buff[index];
      index += 2;
      nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
      ->nfc_a_poll_params.nfcid1_len = packet_buff[index++];
      nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
      ->nfc_a_poll_params.nfcid1 = &packet_buff[index];
      index += nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
               ->nfc_a_poll_params.nfcid1_len;
      nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
      ->nfc_a_poll_params.sel_res_len = packet_buff[index++];
      if (nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
          ->nfc_a_poll_params.sel_res_len == 1) {
        nci_evt.data.payload.nci_data.rf_discover_ntf.rf_tech_param
        ->nfc_a_poll_params.sel_res = packet_buff[index++];
      }
      break;
    case nci_nfc_b_passive_poll_mode:

      break;
    case nci_nfc_f_passive_poll_mode:

      break;
    default:
      break;
  }
  nci_evt.data.payload.nci_data.rf_intf_activated_ntf.rf_disc_id
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_intf_activated_ntf.rf_disc_id
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_intf_activated_ntf.rf_disc_id
    = packet_buff[index++];
  nci_evt.data.payload.nci_data.rf_intf_activated_ntf.rf_disc_id
    = packet_buff[index++];
}

// -------------------------------
// RF_DEACTIVATE

/***************************************************************************//**
 * @brief
 *  Encode and send RF_DEACTIVATE_CMD.
 *
 * @param[in] cmd
 *  NCI RF_DEACTIVATE command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_deactivate(nci_rf_deactivate_cmd_t *cmd)
{
  uint8_t payload[] = { cmd->deactivate_type };

  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;
  packet.gid = nci_gid_rf_management;
  packet.oid = nci_oid_rf_deactivate;
  packet.payload_len = 1;
  packet.payload = payload;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_DEACTIVATE_RSP to nci event data.
 ******************************************************************************/
static void nci_rf_deactivate_rsp_process(void)
{
  nci_evt.header = nci_evt_rf_deactivate_rsp_rec;
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_DEACTIVATE_NTF to nci event data.
 ******************************************************************************/
static void nci_rf_deactivate_ntf_process(void)
{
  nci_evt.header = nci_evt_rf_deactivate_ntf_rec;
}

// -------------------------------
// RF_DEACTIVATE

/***************************************************************************//**
 * @brief
 *  Process and load RF_FIELD_INFO_NTF to nci event data.
 ******************************************************************************/
static void nci_rf_field_info_ntf_process(void)
{
  nci_evt.header = nci_evt_rf_field_info_ntf_rec;
}

// -------------------------------
// RF_T3T_POLLING

/***************************************************************************//**
 * @brief
 *  Encode and send RF_T3T_POLLING_CMD.
 *
 * @param[in] cmd
 *  NCI RF_T3T_POLLING command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_t3t_polling(nci_rf_t3t_polling_cmd_t *cmd)
{
  (void) cmd;
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_T3T_POLLING_RSP to nci event data.
 ******************************************************************************/
static void nci_rf_t3t_polling_rsp_process(void)
{
  nci_evt.header = nci_evt_rf_t3t_polling_rsp_rec;
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_T3T_POLLING_NTF to nci event data.
 ******************************************************************************/
static void nci_rf_t3t_polling_ntf_process(void)
{
  nci_evt.header = nci_evt_rf_t3t_polling_ntf_rec;
}

// -------------------------------
// RF_NFCEE_ACTION

/***************************************************************************//**
 * @brief
 *  Process and load RF_NFCEE_ACTION_NTF to nci event data.
 ******************************************************************************/
static void nci_rf_nfcee_action_ntf_process(void)
{
  nci_evt.header = nci_evt_rf_nfcee_action_ntf_rec;
}

// -------------------------------
// RF_NFCEE_DISCOVERY_REQ

/***************************************************************************//**
 * @brief
 *  Process and load RF_NFCEE_DISCOVERY_REQ_NTF to nci event data.
 ******************************************************************************/
static void nci_rf_nfcee_discovery_req_ntf_process(void)
{
  nci_evt.header = nci_evt_rf_nfcee_discovery_req_ntf_rec;
}

// -------------------------------
// RF_PARAMETER_UPDATE

/***************************************************************************//**
 * @brief
 *  Encode and send RF_PARAMETER_UPDATE_CMD.
 *
 * @param[in] cmd
 *  NCI RF_PARAMETER_UPDATE command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_rf_parameter_update(nci_rf_parameter_update_cmd_t *cmd)
{
  (void) cmd;
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load RF_PARAMETER_UPDATE_RSP to nci event data.
 ******************************************************************************/
static void nci_rf_parameter_update_rsp_process(void)
{
  nci_evt.header = nci_evt_rf_parameter_update_rsp_rec;
}

// -----------------------------------------------------------------------------
// NFCEE Management

// -------------------------------
// NFCEE_DISCOVER

/***************************************************************************//**
 * @brief
 *  Encode and send NFCEE_DISCOVER_CMD.
 *
 * @param[in] cmd
 *  NCI NFCEE_DISCOVER command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_nfcee_discover(nci_nfcee_discover_cmd_t *cmd)
{
  (void) cmd;
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load NFCEE_DISCOVER_RSP to nci event data.
 ******************************************************************************/
static void nci_nfcee_discover_rsp_process(void)
{
  nci_evt.header = nci_evt_nfcee_discover_rsp_rec;
}

/***************************************************************************//**
 * @brief
 *  Process and load NFCEE_DISCOVER_NTF to nci event data.
 ******************************************************************************/
static void nci_nfcee_discover_ntf_process(void)
{
  nci_evt.header = nci_evt_nfcee_discover_ntf_rec;
}

// -------------------------------
// NFCEE_MODE_SET

/***************************************************************************//**
 * @brief
 *  Encode and send NFCEE_MODE_SET_CMD.
 *
 * @param[in] cmd
 *  NCI NFCEE_MODE_SET command to be sent.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_nfcee_mode_set(nci_nfcee_mode_set_cmd_t *cmd)
{
  (void) cmd;
  nci_control_packet_t packet;

  packet.mt = nci_packet_control_cmd;
  packet.pbf = nci_pbf_complete_msg;

  return nci_control_packet_send(&packet);
}

/***************************************************************************//**
 * @brief
 *  Process and load NFCEE_MODE_SET_RSP to nci event data.
 ******************************************************************************/
static void nci_nfcee_mode_set_rsp_process(void)
{
  nci_evt.header = nci_evt_nfcee_mode_set_rsp_rec;
}

// -----------------------------------------------------------------------------
// Incoming Packet Parser

/***************************************************************************//**
 * @brief
 *  Parse the incoming packet header and send to corresponding processing
 *  functions.
 *
 * @returns
 *  Any error code.
 ******************************************************************************/
nci_err_t nci_incoming_packet_process(void)
{
  if (!packet_ready) {
    /* Clear nci event header */
    nci_evt.header = nci_evt_none;
    return nci_err_none;
  }

  packet_ready = false;

  /* Get packet */
  nci_tml_err_t nci_tml_err = nci_tml_receive(packet_buff);
  if (nci_tml_err != nci_tml_err_none) {
    return nci_err_tml;
  }

  nci_tml_log("NCI TML receive:    ");
  nci_tml_packet_log(packet_buff, packet_buff[2] + 3);
  nci_tml_log_ln(" ");

  /* Clear nci event header */
  nci_evt.header = nci_evt_none;

  /* Check Message Type */
  switch ((packet_buff[NCI_PACKET_MT_INDEX] & NCI_PACKET_MT_M)
          >> NCI_PACKET_MT_SHIFT) {
    /* Data packet. */
    case nci_packet_data: {
      /* Set event header to data packet */
      nci_evt.header = nci_evt_data_packet_rec;

      /* Set PBF bit */
      nci_evt.data.is_terminator_packet
        = !(packet_buff[NCI_PACKET_PBF_INDEX]
            & NCI_PACKET_PBF_M) ? true : false;

      /* Set data length */
      nci_evt.data.len = packet_buff[NCI_PACKET_PAYLOAD_LEN_INDEX];

      /* Set connection id */
      nci_evt.data.payload.nci_data.rec_data_packet.conn_id
        = ((packet_buff[NCI_PACKET_CONN_ID_INDEX] & NCI_PACKET_CONN_ID_M)
           >> NCI_PACKET_CONN_ID_SHIFT);

      /* Assign payload pointer */
      nci_evt.data.payload.nci_data.rec_data_packet.payload
        = &packet_buff[NCI_PACKET_PAYLOAD_START_INDEX];
      break;
    }

    /* Control Packet - Response Message */
    case nci_packet_control_rsp: {
      /* Check GID */
      switch ((packet_buff[NCI_PACKET_GID_INDEX] & NCI_PACKET_GID_M)
              >> NCI_PACKET_GID_SHIFT) {
        /* NCI Core responses */
        case nci_gid_nci_core: {
          /* Check OID */
          switch ((packet_buff[NCI_PACKET_OID_INDEX] & NCI_PACKET_OID_M)
                  >> NCI_PACKET_OID_SHIFT) {
            case nci_oid_core_reset:
              /* Process CORE_RESET_RSP */
              nci_core_reset_rsp_process();
              break;
            case nci_oid_core_init:
              /* Process CORE_INIT_RSP */
              nci_core_init_rsp_process();
              break;
            case nci_oid_core_set_config:
              /* Process CORE_SET_CONFIG_RSP */
              nci_core_set_config_rsp_process();
              break;
            case nci_oid_core_get_config:
              /* Process CORE_GET_CONFIG_RSP */
              nci_core_get_config_rsp_process();
              break;
            case nci_oid_core_conn_create:
              /* Process CORE_CONN_CREATE_RSP */
              nci_core_conn_create_rsp_process();
              break;
            case nci_oid_core_conn_close:
              /* Process CORE_CONN_CLOSE_RSP */
              nci_core_conn_close_rsp_process();
              break;
            default:
              /* Error: OID not recognized */
              return nci_err_uknown_oid;
          }
          break;
        }

        /* NCI RF Management responses */
        case nci_gid_rf_management: {
          /* Check OID */
          switch ((packet_buff[NCI_PACKET_OID_INDEX] & NCI_PACKET_OID_M)
                  >> NCI_PACKET_OID_SHIFT) {
            case nci_oid_rf_discover_map:
              /* Process RF_DISCOVER_MAP_RSP */
              nci_rf_discover_map_rsp_process();
              break;
            case nci_oid_rf_set_listen_mode_routing:
              /* Process RF_SET_LISTEN_MODE_ROUTING_RSP */
              nci_rf_set_listen_mode_routing_rsp_process();
              break;
            case nci_oid_rf_get_listen_mode_routing:
              /* Process RF_GET_LISTEN_MODE_ROUTING_RSP */
              nci_rf_get_listen_mode_routing_rsp_process();
              break;
            case nci_oid_rf_discover:
              /* Process RF_DISCOVER_RSP */
              nci_rf_discover_rsp_process();
              break;
            case nci_oid_rf_discover_select:
              /* Process RF_DISCOVER_SELECT_RSP */
              nci_rf_discover_select_rsp_process();
              break;
            case nci_oid_rf_deactivate:
              /* Process RF_DEACTIVATE_RSP */
              nci_rf_deactivate_rsp_process();
              break;
            case nci_oid_rf_t3t_polling:
              /* Process RF_T3T_POLLING_RSP */
              nci_rf_t3t_polling_rsp_process();
              break;
            case nci_oid_rf_parameter_update:
              /* Process RF_PARAMETER_UPDATE_RSP */
              nci_rf_parameter_update_rsp_process();
              break;
            default:
              /* Error: OID not recognized */
              return nci_err_uknown_oid;
          }
          break;
        }

        /* NCI NFCEE management responses */
        case nci_gid_nfcee_management: {
          /* Check OID */
          switch ((packet_buff[NCI_PACKET_OID_INDEX] & NCI_PACKET_OID_M)
                  >> NCI_PACKET_OID_SHIFT) {
            case nci_oid_nfcee_discover:
              /* Process NFCEE_DISCOVER_RSP */
              nci_nfcee_discover_rsp_process();
              break;
            case nci_oid_nfcee_mode_set:
              /* Process NFCEE_MODE_SET_RSP */
              nci_nfcee_mode_set_rsp_process();
              break;
            default:
              /* Error: OID not recognized */
              return nci_err_uknown_oid;
          }
          break;
        }

        /* NCI proprietary responses */
        case nci_gid_proprietary: {
          /* Hand it to proprietary parser. */
          return nci_incoming_proprietary_packet_rsp_process(packet_buff);
        }
        default:
          /* Error: GID not recognized */
          return nci_err_uknown_gid;
      }
      break;
    }

    /* Control Packet - Notification Message */
    case nci_packet_control_ntf: {
      /* Check GID */
      switch ((packet_buff[NCI_PACKET_GID_INDEX] & NCI_PACKET_GID_M)
              >> NCI_PACKET_GID_SHIFT) {
        /* NCI Core notifications */
        case nci_gid_nci_core: {
          /* Check OID */
          switch ((packet_buff[NCI_PACKET_OID_INDEX] & NCI_PACKET_OID_M)
                  >> NCI_PACKET_OID_SHIFT) {
            case nci_oid_core_reset:
              /* Process CORE_RESET_NTF */
              nci_core_reset_ntf_process();
              break;
            case nci_oid_core_conn_credits:
              /* Process CORE_CONN_CREDITS_NTF */
              nci_core_conn_credits_ntf_process();
              break;
            case nci_oid_core_generic_error:
              /* Process CORE_GENERIC_ERROR_NTF */
              nci_core_generic_error_ntf_process();
              break;
            case nci_oid_core_interface_error:
              /* Process CORE_INTERFACE_ERROR_NTF */
              nci_core_interface_error_ntf_process();
              break;
            default:
              /* Error: OID not recognized */
              return nci_err_uknown_oid;
          }
          break;
        }

        /* NCI RF Management notifications */
        case nci_gid_rf_management: {
          /* Check OID */
          switch ((packet_buff[NCI_PACKET_OID_INDEX] & NCI_PACKET_OID_M)
                  >> NCI_PACKET_OID_SHIFT) {
            case nci_oid_rf_get_listen_mode_routing:
              /* Process RF_GET_LISTEN_MODE_ROUTING_NTF */
              nci_rf_get_listen_mode_routing_ntf_process();
              break;
            case nci_oid_rf_discover:
              /* Process RF_DISCOVER_NTF */
              nci_rf_discover_ntf_process();
              break;
            case nci_oid_rf_intf_activated:
              /* Process RF_INTF_ACTIVATED_NTF */
              nci_rf_intf_activated_ntf_process();
              break;
            case nci_oid_rf_deactivate:
              /* Process RF_DEACTIVATE_NTF */
              nci_rf_deactivate_ntf_process();
              break;
            case nci_oid_rf_field_info:
              /* Process RF_FIELD_INFO_NTF */
              nci_rf_field_info_ntf_process();
              break;
            case nci_oid_rf_t3t_polling:
              /* Process RF_T3T_POLLING_NTF */
              nci_rf_t3t_polling_ntf_process();
              break;
            case nci_oid_rf_nfcee_action:
              /* Process RF_NFCEE_ACTION_NTF */
              nci_rf_nfcee_action_ntf_process();
              break;
            case nci_oid_rf_nfcee_discovery_req:
              /* Process RF_NFCEE_DISCOVERY_REQ_NTF */
              nci_rf_nfcee_discovery_req_ntf_process();
              break;
            default:
              /* Error: OID not recognized */
              return nci_err_uknown_oid;
          }
          break;
        }

        /* NCI NFCEE Management notifications */
        case nci_gid_nfcee_management: {
          /* Check OID */
          switch ((packet_buff[NCI_PACKET_OID_INDEX] & NCI_PACKET_OID_M)
                  >> NCI_PACKET_OID_SHIFT) {
            case nci_oid_nfcee_discover:
              /* Process NFCEE_DISCOVER_NTF */
              nci_nfcee_discover_ntf_process();
              break;
            default:
              /* Error: OID not recognized */
              return nci_err_uknown_oid;
          }
          break;
        }

        /* NCI proprietary notifications */
        case nci_gid_proprietary: {
          /* Hand it to proprietary parser. */
          return nci_incoming_proprietary_packet_ntf_process(packet_buff);
        }
        default: {
          /* Error: GID not recognized */
          return nci_err_uknown_gid;
        }
      }
      break;
    }
    default: {
      /* Error: MT not recognized */
      return nci_err_uknown_mt;
    }
  }
  return nci_err_none;
}
