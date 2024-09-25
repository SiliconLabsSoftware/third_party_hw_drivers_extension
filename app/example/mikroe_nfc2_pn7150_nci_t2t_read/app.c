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
#include "sl_sleeptimer.h"
#include "app_assert.h"

#include "t2t.h"
#include "nci.h"
#include "nfc_serial.h"

#include "mikroe_pn7150_config.h"
#include "mikroe_pn7150.h"

#if (defined(SLI_SI917))
#include "sl_driver_gpio.h"
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)           DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED         SL_I2C2
#define GPIO_M4_INTR              0 // M4 Pin interrupt number
#define AVL_INTR_NO               0 // available interrupt number

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "gpiointerrupt.h"
#include "app_log.h"

#define app_printf(...)          app_log(__VA_ARGS__)
#endif

static uint16_t read_index = 0;
static uint16_t t2t_area_size = 0;
static uint8_t data_buff[16];
static mikroe_i2c_handle_t app_i2c_instance = NULL;

static void app_int_callback(uint8_t intNo);

/*
 * T2T Read Process
 * -----------------------------------------------------------------------------
 * ----- Preparation Sequence ------
 * DH --> Core Reset CMD                                                --> NFCC
 * DH <-- Core Reset RSP                                                <-- NFCC
 * DH --> Core Init CMD                                                 --> NFCC
 * DH <-- Core Init RSP                                                 <-- NFCC
 * DH --> NXP Proprietary Act CMD                                       --> NFCC
 * DH <-- NXP Proprietary Act RSP                                       <-- NFCC
 * ----- RF Discover Phase ------
 * DH --> RF Discover Map CMD                                           --> NFCC
 * (RF Prot. = PROTOCOL_T2T, Mode = Poll, RF Intf. = Frame RF)
 * DH <-- RF Discover Map RSP                                           <-- NFCC
 * DH --> RF Discover CMD (NFC_A_PASSIVE_POLL_MODE)                     --> NFCC
 * DH <-- RF Discover RSP                                               <-- NFCC
 * DH <-- RF Intf Activated NTF (Prot = PROTOCOL_T2T, Intf = Frame RF)  <-- NFCC
 * ----- Data Exhange Phase (REPEAT) ------
 * DH --> NCI Data Message (READ Command, Block Address)                <-- NFCC
 * DH <-- Core Conn Credits NTF                                         <-- NFCC
 * DH <-- NCI Data Message (Read Response or NACK Response)             <-- NFCC
 * ----- RF Deactivate Phase ------
 * DH --> RF Deactivate CMD (Discovery)                                 <-- NFCC
 * DH <-- RF Deactivate RSP                                             <-- NFCC
 * DH --> RF Deactivate NTF                                             <-- NFCC
 * ----- Go Back to RF Discover Phase ------
 */

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  // Initialize PN71x0 I2C communication.
  if (SL_STATUS_OK != mikroe_pn7150_init(app_i2c_instance)) {
    app_printf("> PN7150 - NFC 2 Click board driver init failed.\n");
  }

#if (defined(SLI_SI917))
  sl_gpio_t gpio_port_pin = {
    MIKROE_PN7150_INT_PIN / 16,
    MIKROE_PN7150_INT_PIN % 16
  };
  sl_gpio_driver_configure_interrupt(&gpio_port_pin,
                                     GPIO_M4_INTR,
                                     SL_GPIO_INTERRUPT_RISING_EDGE,
                                     (void *)&app_int_callback,
                                     AVL_INTR_NO);
#else
  GPIO_ExtIntConfig(MIKROE_PN7150_INT_PORT,
                    MIKROE_PN7150_INT_PIN,
                    MIKROE_PN7150_INT_PIN,
                    true,
                    false,
                    true);
  GPIOINT_CallbackRegister(MIKROE_PN7150_INT_PIN, app_int_callback);
#endif

  app_printf("        HW Reset       \r\n");
  mikroe_pn7150_hw_reset();

  // Initialize NCI.
  nci_init();

  // Print project name.
  app_printf("\r\n******************************\r\n*\r\n");
  app_printf("* NCI T2T Read Demo\r\n");
  app_printf("*\r\n******************************\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  // Get NCI event.
  nci_evt_t *nci_evt = nci_get_event();

  if (nci_evt->header == nci_evt_none) {
    return;
  }

  // Log NCI event, if debug enabled.
  nci_evt_log(nci_evt->header);

  switch (nci_evt->header) {
    /* Start up event, enter init sequence. */
    case nci_evt_startup: {
      /* Step 1 in init sequence: core reset. */
      nci_core_reset_cmd_t core_reset_cmd;

      /* Keep configurations. */
      core_reset_cmd.reset_type = nci_core_reset_keep_config;

      /* Send command and check for error/ */
      nci_err_t nci_err = nci_core_reset(&core_reset_cmd);
      app_assert(nci_err == nci_err_none,
                 "NCI core reset error: %x \r\n",
                 nci_err);
      break;
    }

    /* Event generated by reception of core reset response. */
    case nci_evt_core_reset_rsp_rec: {
      /* Get core reset response data: NCI version of NFCC. */
      uint8_t nci_version
        = nci_evt->data.payload.nci_data.core_reset_rsp.nci_version;

      /* Get core reset response data: configuration status. */
      uint8_t config_status
        = nci_evt->data.payload.nci_data.core_reset_rsp.config_status;

      /* Log NCI version of NFCC. */
      app_printf("NFCC's NCI Version is %x.%x \r\n",
                 ((nci_version & 0xF0) >> 4),
                 (nci_version & 0x0F));

      /* Log configuration status. */
      if (config_status == nci_core_reset_keep_config) {
        nci_log_ln("NCI RF Configuration has been kept.");
      } else if (config_status == nci_core_reset_reset_conig) {
        nci_log_ln("NCI RF Configuration has been reset. ");
      }

      /* Step 2 in init sequence: core init. */
      /* Send command and check for error. */
      nci_err_t nci_err = nci_core_init();
      app_assert(nci_err == nci_err_none,
                 "NCI core init error: %x \r\n",
                 nci_err);
      break;
    }

    /* Event generated by reception of core init response. */
    case nci_evt_core_init_rsp_rec: {
      uint8_t *manu_spec_info
        = nci_evt->data.payload.nci_data.core_init_rsp.manu_spec_info;

      app_printf("NFCC's Firmware Version is %02x.%02x\r\n",
                 manu_spec_info[2],
                 manu_spec_info[3]);

      /* Activate NXP proprietary extensions,
       * send command and check for error.
       */
      nci_err_t nci_err = nci_proprietary_nxp_act();
      app_assert(nci_err == nci_err_none,
                 "NCI core init error: %x \r\n",
                 nci_err);
      break;
    }

    /* Event generated by reception of proprietary nxp act response. */
    case nci_evt_proprietary_nxp_act_rsp_rec: {
      nci_rf_mapping_config_t mapping_config_1 = {
        .rf_protocol = nci_protocol_t2t,
        .mode = nci_rf_mapping_mode_poll,
        .rf_interface = nci_rf_interface_frame
      };

      nci_rf_mapping_config_t mapping_config[1] = { mapping_config_1 };

      nci_rf_discover_map_cmd_t cmd = {
        .num_of_mapping_config = 1,
        .mapping_config = mapping_config
      };

      nci_err_t nci_err = nci_rf_discover_map(&cmd);
      app_assert(nci_err == nci_err_none,
                 "NCI RF discovery map error: %x \r\n",
                 nci_err);
      break;
    }
    case nci_evt_rf_discover_map_rsp_rec: {
      if (nci_evt->data.payload.nci_data.rf_discover_map_rsp.status
          != nci_status_ok) {
        nci_log_ln("RF Discover Map Response status not ok.");
        return;
      }

      uint8_t config[] = { nci_nfc_a_passive_poll_mode, 1 };

      nci_rf_discover_cmd_t cmd = {
        .num_of_config = 1,
        .configurations = config
      };

      nci_err_t nci_err = nci_rf_discover(&cmd);
      app_assert(nci_err == nci_err_none,
                 "NCI RF discovery error: %x \r\n",
                 nci_err);
      break;
    }

    case nci_evt_rf_discover_rsp_rec: {
      if (nci_evt->data.payload.nci_data.rf_discover_rsp.status
          != nci_status_ok) {
        nci_log_ln("RF Discover Response status not ok.");
        return;
      }

      /* Wait for NTF */
      break;
    }
    case nci_evt_rf_intf_activated_ntf_rec: {
      uint8_t t2t_cmd_buff[] = { T2T_CMD_READ, read_index };

      nci_data_packet_t pakcet = {
        .pbf = nci_pbf_complete_msg,
        .conn_id = 0,
        .payload_len = 2,
        .payload = t2t_cmd_buff
      };

      nci_err_t nci_err = nci_data_packet_send(&pakcet);
      app_assert(nci_err == nci_err_none,
                 "NCI RF discovery map error: %x \r\n",
                 nci_err);
      break;
    }
    case nci_evt_data_packet_rec: {
      memcpy(data_buff,
             nci_evt->data.payload.nci_data.rec_data_packet.payload,
             16);

      if (read_index == 0) {
        /* CC2, convert to size in blocks. */
        t2t_area_size = data_buff[14] * 2;

        /* Print header area. */
        app_printf("\r\nHeader: \r\nBlock 000 ");
        serial_put_hex_and_ascii(&data_buff[0], 4);
        app_printf("\r\nBlock 001 ");
        serial_put_hex_and_ascii(&data_buff[4], 4);
        app_printf("\r\nBlock 002 ");
        serial_put_hex_and_ascii(&data_buff[8], 4);
        app_printf("\r\nBlock 003 ");
        serial_put_hex_and_ascii(&data_buff[12], 4);
        app_printf("\r\n\r\nT2T Area (Size: %d blocks): \r\n", t2t_area_size);
      } else if (((4 + t2t_area_size) - read_index) == 2) {
        app_printf("Block %03u to %03u ", read_index, (read_index + 1));
        serial_put_hex(data_buff, 8);
        app_printf(" -- -- -- -- -- -- -- --    ");
        serial_put_ascii(data_buff, 8);
        app_printf("\r\n");
      } else {
        app_printf("Block %03u to %03u ", read_index, (read_index + 3));
        serial_put_hex_and_ascii(data_buff, 16);
        app_printf("\r\n");
      }

      read_index += 4;

      if (read_index < (t2t_area_size + 4)) {
        uint8_t t2t_cmd_buff[] = { T2T_CMD_READ, read_index };

        nci_data_packet_t pakcet = {
          .pbf = nci_pbf_complete_msg,
          .conn_id = 0,
          .payload_len = 2,
          .payload = t2t_cmd_buff
        };
        nci_err_t nci_err = nci_data_packet_send(&pakcet);
        app_assert(nci_err == nci_err_none,
                   "NCI send packet error: %x \r\n",
                   nci_err);
      } else {
        read_index = 0;
        nci_rf_deactivate_cmd_t cmd = {
          .deactivate_type = nci_rf_deact_type_discovery_mode
        };

        nci_err_t nci_err = nci_rf_deactivate(&cmd);
        app_assert(nci_err == nci_err_none,
                   "NCI rf deactive error: %x \r\n",
                   nci_err);
      }

      break;
    }
    case nci_evt_core_conn_credits_ntf_rec: {
      break;
    }
    case nci_evt_rf_deactivate_rsp_rec: {
      break;
    }
    case nci_evt_rf_deactivate_ntf_rec: {
      break;
    }
    default:
      break;
  }
}

static void app_int_callback(uint8_t intNo)
{
  (void) intNo;
  nci_notify_incoming_packet();
}
