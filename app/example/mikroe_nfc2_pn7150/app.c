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
#include "mikroe_pn7150.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#include "rsi_debug.h"

#define app_printf(...)          DEBUGOUT(__VA_ARGS__)
#define I2C_INSTANCE_USED        SL_I2C2

static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#else
#include "sl_i2cspm_instances.h"
#include "app_log.h"

#define app_printf(...)          app_log(__VA_ARGS__)
#endif

#define NFC2_IRQ_STATE_LOW    0
#define NFC2_IRQ_STATE_HIGH   1

static mikroe_i2c_handle_t app_i2c_instance = NULL;

static mikroe_pn7150_control_packet_t ctrl_pck_data;

/**
 * @brief NFC 2 display packet function.
 * @details This function displays data values.
 */
static void display_packet(mikroe_pn7150_control_packet_t *ctrl_pck);

/**
 * @brief NFC 2 display nfc data function.
 * @details This function displays nfc data values.
 */
static void display_nfc_data(mikroe_pn7150_control_packet_t *ctrl_pck);

/**
 * @brief NFC 2 read nfc data function.
 * @details This function reads nfc data and displays data.
 */
static void read_nfc_data(mikroe_pn7150_control_packet_t *ctrl_pck);

/**
 * @brief NFC 2  test antena function.
 * @details This function tests antenna and displays data.
 */
static void test_antenna(mikroe_pn7150_control_packet_t *ctrl_pck);

void app_init(void)
{
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_mikroe;
#endif

  app_printf(" Application Init ");

  if (SL_STATUS_OK != mikroe_pn7150_init(app_i2c_instance)) {
    app_printf("> PN7150 - NFC 2 Click board driver init failed.\n");
  }
  sl_sleeptimer_delay_millisecond(100);

  app_printf("        HW Reset       \r\n");
  mikroe_pn7150_hw_reset();
  sl_sleeptimer_delay_millisecond(100);

  app_printf("-----------------------\r\n");
  app_printf(" Reset and Init. Core  \r\n");
  mikroe_pn7150_cmd_core_reset();
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(&ctrl_pck_data);
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_cmd_core_init();
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(&ctrl_pck_data);
  sl_sleeptimer_delay_millisecond(100);
  display_packet(&ctrl_pck_data);

  while (mikroe_pn7150_check_irq() == NFC2_IRQ_STATE_HIGH) {}

  app_printf("-----------------------\r\n");
  app_printf(" Disabling Standby Mode \r\n");
  mikroe_pn7150_cmd_disable_standby_mode();
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(&ctrl_pck_data);
  sl_sleeptimer_delay_millisecond(100);
  display_packet(&ctrl_pck_data);

  test_antenna(&ctrl_pck_data);

  app_printf("-----------------------\r\n");
  app_printf("Starting Test Procedure\r\n");
  mikroe_pn7150_cmd_test_procedure();
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(&ctrl_pck_data);
  sl_sleeptimer_delay_millisecond(100);
  display_packet(&ctrl_pck_data);

  mikroe_pn7150_hw_reset();
  sl_sleeptimer_delay_millisecond(100);

  app_printf("-----------------------\r\n");
  app_printf("       NFC Config.     \r\n");
  mikroe_pn7150_default_cfg(&ctrl_pck_data);

  app_printf("-----------------------\r\n");
  app_printf("     Discovery Start   \r\n");
  mikroe_pn7150_cmd_start_discovery();
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(&ctrl_pck_data);
  sl_sleeptimer_delay_millisecond(100);
  display_packet(&ctrl_pck_data);

  app_printf("-----------------------\r\n");
  app_printf("-------- START --------\r\n");
  app_printf("-----------------------\r\n");
  sl_sleeptimer_delay_millisecond(500);

  app_printf(" Application Task ");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  while (mikroe_pn7150_check_irq() == NFC2_IRQ_STATE_HIGH) {
    read_nfc_data(&ctrl_pck_data);
  }

  while (mikroe_pn7150_check_irq() == NFC2_IRQ_STATE_LOW) {}

  app_printf("-----------------------\r\n");
  sl_sleeptimer_delay_millisecond(1000);
}

static void display_packet(mikroe_pn7150_control_packet_t *ctrl_pck)
{
  app_printf("- - - - - - - - - - - -\r\n");
  app_printf(" Message Type   = %d\r\n", ( uint16_t ) ctrl_pck->message_type);
  app_printf(" Pck Bound Flag = %d\r\n", ( uint16_t ) ctrl_pck->pck_bound_flag);
  app_printf(" Group Ident    = %d\r\n", ( uint16_t ) ctrl_pck->group_ident);
  app_printf(" Opcode Ident   = %d\r\n", ( uint16_t ) ctrl_pck->opcode_ident);
  app_printf(" Payload Length = %d\r\n", ( uint16_t ) ctrl_pck->payload_length);
  app_printf("- - - - - - - - - - - -\r\n");

  for (uint8_t n_cnt = 0; n_cnt < ctrl_pck_data.payload_length; n_cnt++) {
    app_printf(" Payload[ %.2d ]  = 0x%.2X\r\n", ( uint16_t ) n_cnt,
               ( uint16_t ) ( uint16_t ) ctrl_pck_data.payload[n_cnt]);
  }

  app_printf("- - - - - - - - - - - -\r\n");
  memset(ctrl_pck_data.payload, 0x00, 255);
}

static void display_nfc_data(mikroe_pn7150_control_packet_t *ctrl_pck)
{
  app_printf("- - - - - - - - - - - -\r\n");
  app_printf(" Read Block:\r\n");

  for (uint8_t n_cnt = 1; n_cnt < ctrl_pck->payload_length - 2; n_cnt++) {
    app_printf("\t 0x%.2X \r\n", ( uint16_t ) ctrl_pck->payload[n_cnt]);
  }
  app_printf("\t 0x%.2X \r\n",
             ( uint16_t ) ctrl_pck->payload[ctrl_pck->payload_length - 2]);

  app_printf("- - - - - - - - - - - -\r\n");
  memset(ctrl_pck->payload, 0x00, 255);
}

static void read_nfc_data(mikroe_pn7150_control_packet_t *ctrl_pck)
{
  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  display_nfc_data(ctrl_pck);
  app_printf("    Disconnect Card    \r\n");
  mikroe_pn7150_cmd_card_disconnected();
  sl_sleeptimer_delay_millisecond(10);
  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  sl_sleeptimer_delay_millisecond(10);

  while (mikroe_pn7150_check_irq() == NFC2_IRQ_STATE_LOW) {}

  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  sl_sleeptimer_delay_millisecond(100);
}

static void test_antenna(mikroe_pn7150_control_packet_t *ctrl_pck)
{
  app_printf("-----------------------\r\n");
  app_printf("    Testing Antenna    ");
  mikroe_pn7150_cmd_antenna_test(0x01);
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_cmd_antenna_test(0x07);
  sl_sleeptimer_delay_millisecond(100);
  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  sl_sleeptimer_delay_millisecond(100);

  mikroe_pn7150_cmd_antenna_test(0x0F);
  sl_sleeptimer_delay_millisecond(100);
  mikroe_pn7150_read_ctrl_packet_data(ctrl_pck);
  sl_sleeptimer_delay_millisecond(100);
  display_packet(ctrl_pck);
}
