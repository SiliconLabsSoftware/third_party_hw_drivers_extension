/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include <string.h>
#include "sl_component_catalog.h"
#include "sl_sleeptimer.h"

#include "deca_probe_interface.h"
#include "port_dw3000.h"
#include "shared_functions.h"
#include "shared_defines.h"
#include "config_options.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_si91x_gspi.h"

#define app_printf(...) DEBUGOUT(__VA_ARGS__)

static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#else
#include "app_log.h"

#define app_printf(...) app_log(__VA_ARGS__)
#endif

// Comment out the line below in order to switch the application mode to
//   receiver
// #define DEMO_APP_TRANSMITTER

/* Default communication configuration. We use default non-STS DW mode. */
static dwt_config_t config = {
  5,                  /* Channel number. */
  DWT_PLEN_128,       /* Preamble length. Used in TX only. */
  DWT_PAC8,           /* Preamble acquisition chunk size. Used in RX only. */
  9,                  /* TX preamble code. Used in TX only. */
  9,                  /* RX preamble code. Used in RX only. */
  1,                  /* 0 to use standard 8 symbol SFD, 1 to use non-standard 8
                       *   symbol, 2 for non-standard 16 symbol SFD and 3 for 4z
                       *   8 symbol SDF type */
  DWT_BR_6M8,         /* Data rate. */
  DWT_PHRMODE_STD,    /* PHY header mode. */
  DWT_PHRRATE_STD,    /* PHY header rate. */
  (129 + 8 - 8),      /* SFD timeout (preamble length + 1 + SFD length - PAC
                       *   size). Used in RX only. */
  DWT_STS_MODE_OFF,   /* No STS mode enabled (STS Mode 0). */
  DWT_STS_LEN_64,     /* STS length, see allowed values in Enum
                       *   dwt_sts_lengths_e */
  DWT_PDOA_M0         /* PDOA mode off */
};

/* Index to access to sequence number of the blink frame in the tx_msg/rx_msg
 *   array. */
#define BLINK_FRAME_SN_IDX 1

#ifdef DEMO_APP_TRANSMITTER /* TX Side define*/

/* The frame sent in this example is an 802.15.4e standard blink. It is a
 *   12-byte frame composed of the following fields:
 *     - byte 0: frame type (0xC5 for a blink).
 *     - byte 1: sequence number, incremented for each new frame.
 *     - byte 2 -> end: message data
 */
static uint8_t tx_msg[] =
{ 0xC5, 0, 'S', 'I', 'L', 'I', 'C', 'O', 'N', 'L', 'A', 'B', 'S', '\0' };

/* The real length that is going to be transmitted */
#define FRAME_LENGTH                 (sizeof(tx_msg) + FCS_LEN)

/* Inter-frame delay period, in milliseconds. */
#define TRANSMITTING_INTERVAL_MSEC   2000

static sl_sleeptimer_timer_handle_t app_timer_handle;
static volatile bool tx_trigger_process = false;
void app_tx_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data);

#endif

#ifndef DEMO_APP_TRANSMITTER /* RX Side define*/

#define RX_MSG_DATA_IDX   2

/* Buffer to store received frame. See NOTE 1 below. */
static uint8_t rx_buffer[FRAME_LEN_MAX];
static uint8_t rx_msg[FRAME_LEN_MAX];
static uint8_t frame_id;

/* Hold copy of status register state here for reference so that it can be
 *   examined at a debug breakpoint. */
static uint32_t status_reg;

/* Hold copy of frame length of frame received (if good) so that it can be
 *   examined at a debug breakpoint. */
static uint16_t frame_len;

#endif

void app_init(void)
{
  app_printf("DWM3000 - UWB 2 Click Driver\r\n");

#ifdef DEMO_APP_TRANSMITTER
  app_printf("Application Mode: Transmitter\r\n");
#else
  app_printf("Application Mode: Receiver\r\n");
#endif

#if (defined(SLI_SI917))
  port_init_dw_chip(&gspi_instance);
#else
  port_init_dw_chip();
#endif

  reset_DW3000();

  if (dwt_probe((struct dwt_probe_s *)&dw3000_probe_interf)) {
    app_printf("DWM 3000 probe fail\r\n");
    while (1) {}
  }

  /* This initialization is added to avoid crashing the board when calling APIs
   *   that writes inside local data
   *    like setxtaltrim */
  if (dwt_initialise(DWT_DW_INIT) != DWT_SUCCESS) {
    app_printf("DWM 3000 init fail\r\n");
    while (1) {}
  }

  uint32_t dev_id = dwt_readdevid();

  /* Reads and validate device ID returns DWT_ERROR if it does not match
   *   expected else DWT_SUCCESS */
  if (dwt_check_dev_id() == DWT_SUCCESS) {
    app_printf("DWM3000 dev id = 0x%lx\r\n", dev_id);
  } else {
    app_printf("DWM3000 read dev id failed\r\n");
  }

  uint32_t part_id = dwt_getpartid();
  app_printf("DWM3000 part id = 0x%lx\r\n", part_id);

  uint32_t lot_id = dwt_getlotid();
  app_printf("DWM3000 lot id = 0x%lx\r\n", lot_id);

  uint8_t otprevision = dwt_otprevision();
  app_printf("DWM3000 otp revision = 0x%x\r\n", otprevision);

  /* Enabling LEDs here for debug so that for each TX the D1 LED will flash on
   *   DW3000 red eval-shield boards. */
  dwt_setleds(DWT_LEDS_ENABLE | DWT_LEDS_INIT_BLINK);

  /* Configure DW IC. See NOTE 5 below. */

  /* if the dwt_configure returns DWT_ERROR either the PLL or RX calibration has
   *   failed the host should reset the device */
  if (dwt_configure(&config)) {
    app_printf("DWM3000 config failed\r\n");
    while (1) {}
  }

#ifdef DEMO_APP_TRANSMITTER

  /* Configure the TX spectrum parameters (power PG delay and PG Count) */
  dwt_configuretxrf(&txconfig_options);

  app_printf("Start TX timer periodic = 0x%lx\r\n",
             sl_sleeptimer_start_periodic_timer_ms(&app_timer_handle,
                                                   TRANSMITTING_INTERVAL_MSEC,
                                                   app_tx_timer_callback,
                                                   NULL,
                                                   0,
                                                   0));
#endif
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
#ifdef DEMO_APP_TRANSMITTER /* TX Side */
  if (tx_trigger_process) {
    /* Write frame data to DW IC and prepare transmission. See NOTE 3 below.*/
    dwt_writetxdata(FRAME_LENGTH - FCS_LEN, tx_msg, 0); // Zero offset in TX buffer.

    /* In this example since the length of the transmitted frame does not
     *   change,
     * nor the other parameters of the dwt_writetxfctrl function, the
     * dwt_writetxfctrl call could be outside the main while(1) loop.
     */
    dwt_writetxfctrl(FRAME_LENGTH, 0, 0);         /* Zero offset in TX buffer,
                                                   *   no ranging. */

    /* Start transmission. */
    dwt_starttx(DWT_START_TX_IMMEDIATE);

    /* Poll DW IC until TX frame sent event set. See NOTE 4 below.
     * STATUS register is 4 bytes long but, as the event we are looking
     * at is in the first byte of the register, we can use this simplest
     * API function to access it.*/
    waitforsysstatus(NULL, NULL, DWT_INT_TXFRS_BIT_MASK, 0);

    /* Clear TX frame sent event. */
    dwt_writesysstatuslo(DWT_INT_TXFRS_BIT_MASK);

    app_printf("TX Frame Sent #%d\r\n", tx_msg[BLINK_FRAME_SN_IDX]);

    /* Increment the blink frame sequence number (modulo 256). */
    tx_msg[BLINK_FRAME_SN_IDX]++;

    tx_trigger_process = false;
  }

#else /* RX Side */

  /* Clear local RX buffer to avoid having leftovers from previous receptions
   *    This is not necessary but is included here to aid reading
   * the RX buffer.
   * This is a good place to put a breakpoint. Here (after first time through
   *   the loop) the local status register will be set for last event
   * and if a good receive has happened the data buffer will have the data in
   *   it, and frame_len will be set to the length of the RX frame. */
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(rx_msg, 0, sizeof(rx_msg));

  /* Activate reception immediately. See NOTE 2 below. */
  dwt_rxenable(DWT_START_RX_IMMEDIATE);

  /* Poll until a frame is properly received or an error/timeout occurs. See
   *   NOTE 3 below.
   * STATUS register is 5 bytes long but, as the event we are looking at is in
   *   the first byte of the register, we can use this simplest API
   * function to access it. */
  waitforsysstatus(&status_reg, NULL,
                   (DWT_INT_RXFCG_BIT_MASK | SYS_STATUS_ALL_RX_ERR), 0);

  if (status_reg & DWT_INT_RXFCG_BIT_MASK) {
    /* A frame has been received, copy it to our local buffer. */
    frame_len = dwt_getframelength();
    if (frame_len <= FRAME_LEN_MAX) {
      /* No need to read the FCS/CRC. */
      dwt_readrxdata(rx_buffer, (frame_len - FCS_LEN), 0);
      frame_id = *(rx_buffer + BLINK_FRAME_SN_IDX);
      memcpy(rx_msg, (rx_buffer + RX_MSG_DATA_IDX),
             (frame_len - FCS_LEN - RX_MSG_DATA_IDX));

      app_printf("Message received #%u: %s\r\n",
                 ( uint16_t ) frame_id, rx_msg);

      /* Clear good RX frame event in the DW IC status register. */
      dwt_writesysstatuslo(DWT_INT_RXFCG_BIT_MASK);
    }
  } else {
    /* Clear RX error events in the DW IC status register. */
    dwt_writesysstatuslo(SYS_STATUS_ALL_RX_ERR);
  }
#endif
}

#ifdef DEMO_APP_TRANSMITTER
void app_tx_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void) data;
  (void) handle;

  tx_trigger_process = true;
}

#endif
