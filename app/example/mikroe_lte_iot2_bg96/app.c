/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "app_iostream_cli.h"
#include "at_parser_events.h"
#include "mikroe_bg96.h"

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
#else
#define app_printf(...)                app_log(__VA_ARGS__)
#endif

#if (defined(SLI_SI917))
#define USART_INSTANCE_USED            UART_1
static usart_peripheral_t uart_instance = USART_INSTANCE_USED;
#endif

static mikroe_uart_handle_t app_uart_instance = NULL;

void app_init(void)
{
#if (defined(SLI_SI917))
  app_uart_instance = &uart_instance;
#else
  app_uart_instance = sl_iostream_uart_mikroe_handle;

  sl_iostream_set_default(sl_iostream_vcom_handle);
  app_log_iostream_set(sl_iostream_vcom_handle);
#endif

  app_printf("Hello World LTE IoT 2 Click !!!\r\n");
  bg96_init(app_uart_instance);
  app_printf("BG96 init done !!!\r\n");
  app_printf("Type your command to execute the function !!!\r\n");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  app_iostream_cli_process_action();
  at_parser_process();
  at_platform_process();
  at_event_process();
}
