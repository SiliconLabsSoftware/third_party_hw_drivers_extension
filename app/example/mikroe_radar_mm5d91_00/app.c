/***************************************************************************//**
 * @file app.c
 * @brief Barcode Decoder Example Application
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
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
#include "mikroe_mm5d91_00.h"

#if (defined(SLI_SI917))
#include "sl_si91x_usart.h"
#include "rsi_debug.h"
#else
#include "sl_iostream_init_usart_instances.h"
#include "sl_iostream_init_eusart_instances.h"
#include "app_log.h"
#endif

#if (defined(SLI_SI917))
#define app_printf(...)                DEBUGOUT(__VA_ARGS__)
#define USART_INSTANCE_USED            UART_1

static usart_peripheral_t uart_instance = USART_INSTANCE_USED;
#else
#define app_printf(...)                app_log(__VA_ARGS__)
#endif

mikroe_uart_handle_t app_uart_instance = NULL;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#if (defined(SLI_SI917))
  app_uart_instance = &uart_instance;
#else
  app_uart_instance = sl_iostream_uart_mikroe_handle;

  sl_iostream_set_default(sl_iostream_vcom_handle);
  app_log_iostream_set(sl_iostream_vcom_handle);
#endif

  if (SL_STATUS_OK == mikroe_radar_init(app_uart_instance)) {
    app_printf("Init: Success\r\n");
  } else {
    app_printf("Init: Failed\r\n");
  }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint8_t evt_id;
  uint8_t evt_payload_size;
  uint8_t evt_payload[16];

  if (SL_STATUS_OK == mikroe_radar_get_event(&evt_id,
                                             evt_payload,
                                             &evt_payload_size)) {
    if (RADAR_CMD_ID_DETECT_IN_EVT == evt_id) {
      app_printf(" EVENT: IN\r\n");
      radar_float_bytes_t distance;
      memcpy(distance.b_data, &evt_payload[8], 4);
      radar_float_ieee_to_mchip(&distance.f_data);
      app_printf(" Target distance: %.3f m\r\n", distance.f_data);
      memcpy(distance.b_data, &evt_payload[12], 4);
      radar_float_ieee_to_mchip(&distance.f_data);
      app_printf(" Accuracy (+/-): %.3f m\r\n", distance.f_data);
    } else {
      app_printf(" EVENT: OUT\r\n");
    }
    uint32_t evt_time = (( uint32_t ) evt_payload[3] << 24)
                        | (( uint32_t ) evt_payload[2] << 16)
                        | (( uint16_t ) evt_payload[1] << 8)
                        | evt_payload[0];
    app_printf(" Elapsed time: %.2f s\r\n", evt_time / 1000.0);
    float temperature;
    if (SL_STATUS_OK == mikroe_radar_get_temperature(&temperature)) {
      app_printf(" Temperature: %.2f C\r\n\n", temperature);
    }
  }
}
