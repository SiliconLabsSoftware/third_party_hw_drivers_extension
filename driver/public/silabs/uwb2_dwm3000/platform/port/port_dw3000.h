/**
 * @file      port.h
 *
 * @brief     port headers file to EFR32BG22
 *
 * @author    Decawave
 *
 * @attention Copyright 2017-2019 (c) Decawave Ltd, Dublin, Ireland.
 *            All rights reserved.
 *
 */

#ifndef PORT__H_
#define PORT__H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "error.h"
#include "deca_device_api.h"
#include "drv_spi_master.h"

/* spi functions prototypes */
int uwb2_dwm3000_spi_init(mikroe_spi_handle_t spidrv);
int uwb2_dwm3000_spi_close(void);
int writetospi(uint16_t headerLength,
               const uint8_t *headerBuffer,
               uint16_t bodyLength,
               const uint8_t *bodyBuffer);
int writetospiwithcrc(uint16_t headerLength,
                      const uint8_t *headerBuffer,
                      uint16_t bodyLength,
                      const uint8_t *bodyBuffer,
                      uint8_t crc8);
int readfromspi(uint16_t headerLength,
                uint8_t *headerBuffer,
                uint16_t readlength,
                uint8_t *readBuffer);
void set_dw_spi_fast_rate(void);
void set_dw_spi_slow_rate(void);

/* port functions prototypes */
error_e port_init_dw_chip(mikroe_spi_handle_t spidrv);
error_e port_wakeup_dw3000_fast(void);
error_e port_wakeup_dw3000(void);
void wakeup_device_with_io(void);

void enable_dw3000_irq(void);
void disable_dw3000_irq(void);
void reset_DW3000(void);

void port_stop_all_UWB(void);

error_e port_disable_dw_irq_and_reset(int reset);

#ifdef __cplusplus
}
#endif

#endif /* PORT__H__ */
