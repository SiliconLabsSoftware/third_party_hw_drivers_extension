/***************************************************************************//**
 * @file app.c
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
 * # Experimental Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "w5x00_utils.h"
#include "ethernet.h"
#include "ethernet_client.h"
#include "ethernet_server.h"
#include "dns.h"
#include "sntp.h"
#include "http_server.h"
#include "w5x00.h"
#include "w5x00_utils.h"
#include "sl_sleeptimer.h"
#include "app_assert.h"

#if (defined(SLI_SI917))
#include "rsi_debug.h"
#include "sl_si91x_gspi.h"
static sl_gspi_instance_t gspi_instance = SL_GSPI_MASTER;
#define app_printf(...) DEBUGOUT(__VA_ARGS__)
#else /* None Si91x device */
#include "app_log.h"
#include "sl_spidrv_instances.h"
#define app_printf(...) app_log(__VA_ARGS__)
#endif

#define USE_DHCP

#define app_log_print_ip(ip)                                \
  app_printf("%d.%d.%d.%d", w5x00_ip4_addr_get_byte(ip, 0), \
             w5x00_ip4_addr_get_byte(ip, 1),                \
             w5x00_ip4_addr_get_byte(ip, 2),                \
             w5x00_ip4_addr_get_byte(ip, 3))

uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
w5x00_ip4_addr_t ip = { WIZNET_IP4_DATA(127, 0, 0, 1) };
w5x00_ip4_addr_t dns_server1 = { WIZNET_IP4_DATA(1, 1, 1, 1) };
w5x00_ip4_addr_t dns_server2 = { WIZNET_IP4_DATA(8, 8, 8, 8) };

w5x00_ethernet_t eth;
w5x00_ethernet_client_t client;

w5x00_ip4_addr_t local_ip = { WIZNET_IP4_DATA(10, 42, 0, 242) };
w5x00_ip4_addr_t gateway_ip = { WIZNET_IP4_DATA(10, 42, 0, 1) };
w5x00_ip4_addr_t subnet_mask = { WIZNET_IP4_DATA(255, 255, 255, 0) };
static mikroe_spi_handle_t app_spi_instance = NULL;

static const char *get_content_body(const char *uri)
{
  const char *start, *end;

  start = end = uri;
  while (*uri) {
    // line ending with <cr><lf>
    if (((uri[0] == '\r') && (uri[1] == '\n'))
        || ((uri[0] == '\n') && (uri[1] == '\r'))) {
      if (end == start) { // checking for empty line
        if (uri[2]) {
          return &uri[2]; // body is the next line
        } else {
          break; // next line is not found
        }
      }
      end = start = &uri[2];
      uri += 2;
    } else if ((uri[0] == '\n')
               && (uri[1] != '\r')) { // line ending with <lf>
      if (end == start) {   // checking for empty line
        if (uri[1]) {
          return &uri[1];   // body is the next line
        } else {
          break;   // next line is not found
        }
      }
      end = start = &uri[1];
      uri += 1;
    } else {
      end = uri++;
    }
  }
  return NULL;
}

void http_client_get_device_public_ip(const char *host,
                                      uint16_t port,
                                      const char *path)
{
  static char message[1024];

  w5x00_ethernet_client_init(&client, &eth, 10000);
  sl_sleeptimer_delay_millisecond(1000);
  app_printf("Connecting to: %s:%d\r\n", host, port);
  if (SL_STATUS_OK == w5x00_ethernet_client_connect_host(&client,
                                                         host,
                                                         port)) {
    app_printf("\r\nConnected!\r\n\r\n");

    snprintf(message,
             sizeof(message),
             "GET http://%s%s HTTP/1.1\r\nHost: %s\r\nAccept: text/html\r\n\r\n",
             host,
             path,
             host);
    app_printf("HTTP Request:\r\n\r\n%s\r\n\r\n", message);
    while (w5x00_ethernet_client_available_for_write(&client)
           < (int)strlen(message)) {}
    w5x00_ethernet_client_write(&client,
                                (uint8_t *)message,
                                strlen(message));
    // sl_sleeptimer_delay_millisecond(500);
    while (w5x00_ethernet_client_connected(&client)) {
      int length = w5x00_ethernet_client_read(&client,
                                              (uint8_t *)message,
                                              sizeof(message) - 1);
      if ((length > 0) && (length < (int)sizeof(message))) {
        message[length] = '\0';
        w5x00_ip4_addr_t ip;
        const char *body = NULL;

        app_printf("HTTP Response:\r\n\r\n%s\r\n\r\n", message);

        body = get_content_body(message);
        // app_printf("%.*s", length, message);
        if (NULL != body) {
          app_printf("HTTP Response body (public ip address): %s\r\n", body);
        }
        if (body
            && w5x00_ip4addr_aton(body, &ip)) {
          app_printf("Public ip (parsed from the body): %d.%d.%d.%d\r\n\r\n",
                     w5x00_ip4_addr_get_byte(&ip, 0),
                     w5x00_ip4_addr_get_byte(&ip, 1),
                     w5x00_ip4_addr_get_byte(&ip, 2),
                     w5x00_ip4_addr_get_byte(&ip, 3));
        } else {
          app_printf("Parse the ip from HTTP response body failed\r\n");
        }
      }
    }
    app_printf("Connection remote closed!\r\n");
  } else {
    app_printf("Connect eror!\r\n");
  }
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t status;

#if (defined(SLI_SI917))
  app_spi_instance = &gspi_instance;
#else
  app_spi_instance = sl_spidrv_mikroe_handle;
#endif

  w5x00_init(app_spi_instance);

#ifdef USE_DHCP
  status = w5x00_ethernet_dhcp_init(&eth, mac, 30000, 10000);
  app_printf("DHCP configuration: %s\r\n",
             SL_STATUS_OK == status ? "success":"failed");
#else
  status = w5x00_ethernet_static_init(&eth,
                                      mac,
                                      &local_ip,
                                      &gateway_ip,
                                      &subnet_mask,
                                      &dns_server1,
                                      3000);
  app_printf("Static address configuration: %s\r\n",
             SL_STATUS_OK == status ? "success":"failed");
#endif

  if (SL_STATUS_OK != status) {
    enum EthernetLinkStatus link_status;

    link_status = w5x00_ethernet_link_status(&eth);
    if (EthernetLinkON == link_status) {
      app_printf("Ethernet link status is on\r\n");
    } else if (EthernetLinkOFF == link_status) {
      app_printf("Ethernet link status is off\r\n");
      return;
    }
  }

  w5x00_ethernet_set_dns_server(&eth, dns_server1);
  // w5x00_ethernet_set_dns_server(&eth, dns_server2);

  memset(&local_ip, 0, sizeof(local_ip));
  w5x00_ethernet_get_local_ip(&eth, &local_ip);
  app_printf("local ip:    ");
  app_log_print_ip(&local_ip);
  app_printf("\r\n");

  memset(&gateway_ip, 0, sizeof(gateway_ip));
  w5x00_ethernet_get_gateway_ip(&eth, &gateway_ip);
  app_printf("gateway:     ");
  app_log_print_ip(&gateway_ip);
  app_printf("\r\n");

  memset(&subnet_mask, 0, sizeof(subnet_mask));
  w5x00_ethernet_get_subnet_mask(&eth, &subnet_mask);
  app_printf("subnet mask: ");
  app_log_print_ip(&subnet_mask);
  app_printf("\r\n");

  app_printf("dns:         ");
  app_log_print_ip(&eth.dns_server_address);
  app_printf("\r\n");
  http_client_get_device_public_ip("checkip.amazonaws.com", 80, "/");
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
