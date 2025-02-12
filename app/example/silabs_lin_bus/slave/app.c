/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
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
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif // SL_CATALOG_POWER_MANAGER_PRESENT
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "em_device.h"

#include "sl_simple_led_instances.h"

#include "sl_lin.h"

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;

#if SL_LIN_SLAVE1
#define BASE_ADDRESS  10
uint8_t testdata[8] = { 0xde, 0xad, 0xbe, 0xef, 0xca, 0xfe, 0xba, 0xbe, };
#elif SL_LIN_SLAVE2
#define BASE_ADDRESS  20
uint8_t testdata[8] = { 0xca, 0xfe, 0xba, 0xbe, 0xde, 0xad, 0xbe, 0xef, };
#endif

uint8_t workbuf[8] = { 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, };

static void incoming_packet(uint8_t frame_id,
                            bool writable,
                            uint8_t *data,
                            int len,
                            bool success)
{
  (void)frame_id;
  (void)writable;
  (void)data;
  (void)len;

  if (success) {
    if (frame_id == BASE_ADDRESS + 3) {
      sl_lin_slave_update_readable_endpoint(BASE_ADDRESS + 4, data);
    }

    sl_led_toggle(&sl_led_led0);
  }
}

static void outgoing_packet(uint8_t frame_id,
                            bool writable,
                            uint8_t *data,
                            int len,
                            bool success)
{
  (void)frame_id;
  (void)writable;
  (void)data;
  (void)len;

  if (success) {
    sl_led_toggle(&sl_led_led1);
  }
}

void app_init(void)
{
  // DCDC_E302 – DCDC Interrupts Block EM2/3 Entry or Cause Unexpected Wake-up
  sl_interrupt_manager_disable_irq(DCDC_IRQn);

  // assume enough time has passed, clear DC-DC and EMU interrupt flags
  // for detecting changes
  DCDC->IF_CLR = _DCDC_IF_MASK;
  EMU->IF_CLR = _EMU_IF_MASK;
  sl_interrupt_manager_clear_irq_pending(DCDC_IRQn);

  sl_lin_slave_register_writable_endpoint(BASE_ADDRESS + 1,
                                          8,
                                          incoming_packet,
                                          true);
  sl_lin_slave_register_readable_endpoint(BASE_ADDRESS + 2,
                                          8,
                                          outgoing_packet,
                                          testdata,
                                          true);
  sl_lin_slave_register_writable_endpoint(BASE_ADDRESS + 3,
                                          8,
                                          incoming_packet,
                                          true);
  sl_lin_slave_register_readable_endpoint(BASE_ADDRESS + 4,
                                          8,
                                          outgoing_packet,
                                          workbuf,
                                          true);
  sl_lin_slave_register_readable_endpoint(BASE_ADDRESS + 5,
                                          8,
                                          outgoing_packet,
                                          testdata,
                                          true);

  sl_lin_slave_inject_checksum_error(BASE_ADDRESS + 5);

  sl_lin_slave_register_writable_endpoint(31, 8, incoming_packet, true);
  sl_lin_slave_register_readable_endpoint(32, 8, outgoing_packet, testdata,
                                          true);
  sl_lin_slave_register_writable_endpoint(35, 8, incoming_packet, true);

#if SL_LIN_SLAVE1
  sl_lin_slave_register_writable_endpoint(36, 8, incoming_packet, true);
#elif SL_LIN_SLAVE2
  sl_lin_slave_register_readable_endpoint(36, 8, outgoing_packet, testdata,
                                          true);
#endif

  sl_lin_slave_init();

#if defined(SL_CATALOG_KERNEL_PRESENT)
  // Start the kernel. Task(s) created in app_init() will start running.
  sl_system_kernel_start();
#else // SL_CATALOG_KERNEL_PRESENT
  while (1) {
    // Do not remove this call: Silicon Labs components process action routine
    // must be called from the super loop.
    sl_system_process_action();

#if defined(SLEEP_ACT_PORT) && defined(SLEEP_ACT_PIN)
    GPIO_PinOutToggle(SLEEP_ACT_PORT, SLEEP_ACT_PIN);
#endif

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
    // Let the CPU go to sleep if the system allows it.
    sl_power_manager_sleep();
#endif
  }
#endif // SL_CATALOG_KERNEL_PRESENT
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Set advertising interval to 100ms.
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        160, // min. adv. interval (milliseconds * 1.6)
        160, // max. adv. interval (milliseconds * 1.6)
        0,   // adv. duration
        0);  // max. num. adv. events
      app_assert_status(sc);
      // Start advertising and enable connections.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_advertiser_connectable_scannable);
      app_assert_status(sc);
      break;

    // -------------------------------
    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      break;

    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id:
      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Restart advertising after client has disconnected.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_advertiser_connectable_scannable);
      app_assert_status(sc);
      break;

    ///////////////////////////////////////////////////////////////////////////
    // Add additional event handlers here as your application requires!      //
    ///////////////////////////////////////////////////////////////////////////

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}
