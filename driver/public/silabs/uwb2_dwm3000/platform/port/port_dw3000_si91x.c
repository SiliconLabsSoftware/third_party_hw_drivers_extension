/*! ----------------------------------------------------------------------------
 * @file    port_dw3000.c
 * @brief   HW specific definitions and functions for DW3000 Interface
 *
 * @attention
 *
 * Copyright 2016 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 * @author
 */

#include "port_dw3000.h"
#include "sl_component_catalog.h"
#if defined (SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif // SL_CATALOG_POWER_MANAGER_PRESENT
#include "uwb2_dwm3000_config.h"

#include "drv_digital_in.h"
#include "drv_digital_out.h"

#if (defined(SLI_SI917))
#include "sl_driver_gpio.h"

#define PIN_INTR_NO               PIN_INTR_0
#define AVL_INTR_NO               0 // available interrupt number
#else // SLI_SI917
#include "sl_hal_gpio.h"
#include "sl_gpio.h"
#endif // SLI_SI917

typedef struct port_dw3000
{
  spi_master_t spi;

#if defined (DWM3000_RESET_PORT)
  digital_out_t gpio_reset;
#endif // DWM3000_RESET_PORT

#if defined (DWM3000_WAKE_PORT)
  digital_out_t gpio_wake;
#endif // DWM3000_WAKE_PORT

#if defined (DWM3000_ON_PORT)
  digital_in_t gpio_on;
#endif // DWM3000_ON_PORT

#if defined (DWM3000_CS_PORT)
  digital_out_t gpio_cs;
#endif // DWM3000_CS_PORT

#if defined (DWM3000_INT_PORT)
  digital_in_t gpio_int;
#endif // DWM3000_CS_PORT
} uwb2_dwm300_t;

static uwb2_dwm300_t uwb2_dwm3000;

#if (defined(SLI_SI917))
static void process_deca_irq(uint32_t int_no);

#else // SLI_SI917
static void process_deca_irq(uint8_t int_no, void *ctx);

#endif // SLI_SI917

#if defined(HAL_UWB_WAKEUP_PORT) && defined(HAL_UWB_WAKEUP_PIN)
#define port_wakeup_start()  digital_out_high(&uwb2_dwm3000.gpio_wake);
#define port_wakeup_end()    digital_out_low(&uwb2_dwm3000.gpio_wake);
#else // HAL_UWB_WAKEUP_PORT & HAL_UWB_WAKEUP_PIN
#if defined(DWM3000_CS_PORT) && defined(DWM3000_CS_PIN)
#define port_wakeup_start()  digital_out_low(&uwb2_dwm3000.gpio_cs);
#define port_wakeup_end()    digital_out_high(&uwb2_dwm3000.gpio_cs);
#else // DWM3000_CS_PORT & DWM3000_CS_PIN
#define port_wakeup_start()
#define port_wakeup_end()
#endif // DWM3000_CS_PORT & DWM3000_CS_PIN
#endif // HAL_UWB_WAKEUP_PORT & HAL_UWB_WAKEUP_PIN

int uwb2_dwm3000_spi_init(mikroe_spi_handle_t spidrv)
{
  if (NULL == spidrv) {
    return _ERR;
  }

  spi_master_config_t spi_cfg;
  spi_master_configure_default(&spi_cfg);
  spi_cfg.mode = SPI_MASTER_MODE_0;
  spi_cfg.speed = DWM3000_FREQ_MIN;

  uwb2_dwm3000.spi.handle = spidrv;

  if (spi_master_open(&uwb2_dwm3000.spi, &spi_cfg) == SPI_MASTER_ERROR) {
    return _ERR;
  }

#if defined (DWM3000_RESET_PORT)
  pin_name_t gpio_reset_pin = hal_gpio_pin_name(DWM3000_RESET_PORT,
                                                DWM3000_RESET_PIN);
  digital_out_init(&uwb2_dwm3000.gpio_reset, gpio_reset_pin);
#endif // DWM3000_RESET_PORT

#if defined (DWM3000_WAKE_PORT)
  pin_name_t gpio_wake_pin = hal_gpio_pin_name(DWM3000_WAKE_PORT,
                                               DWM3000_WAKE_PIN);
  digital_out_init(&uwb2_dwm3000.gpio_wake, gpio_wake_pin);
#endif // DWM3000_WAKE_PORT

#if defined (DWM3000_ON_PORT)
  pin_name_t gpio_on_pin = hal_gpio_pin_name(DWM3000_ON_PORT,
                                             DWM3000_ON_PIN);
  digital_in_init(&uwb2_dwm3000.gpio_on, gpio_on_pin);
#endif // DWM3000_ON_PORT

#if defined (DWM3000_CS_PORT)
  pin_name_t gpio_cs_pin = hal_gpio_pin_name(DWM3000_CS_PORT,
                                             DWM3000_CS_PIN);
  digital_out_init(&uwb2_dwm3000.gpio_cs, gpio_cs_pin);
  digital_out_high(&uwb2_dwm3000.gpio_cs);
#endif // DWM3000_CS_PORT

#if defined  DWM3000_INT_PORT
  int32_t int_no;
  pin_name_t gpio_int_pin = hal_gpio_pin_name(DWM3000_INT_PORT,
                                              DWM3000_INT_PIN);
  digital_in_init(&uwb2_dwm3000.gpio_int, gpio_int_pin);

#if (defined(SLI_SI917))
  int_no = PIN_INTR_NO;
  sl_gpio_driver_configure_interrupt((sl_gpio_t *)&uwb2_dwm3000.gpio_int.pin,
                                     int_no,
                                     SL_GPIO_INTERRUPT_RISING_EDGE,
                                     process_deca_irq,
                                     AVL_INTR_NO);

  sl_gpio_driver_disable_interrupts((int_no << 16)
                                    | SL_GPIO_INTERRUPT_RISE_EDGE);

#else // SLI_SI917
  int_no = DWM3000_INT_PIN;
  sl_gpio_configure_external_interrupt(
    (const sl_gpio_t *)&uwb2_dwm3000.gpio_int.pin,
    &int_no,
    SL_GPIO_INTERRUPT_RISING_EDGE,
    process_deca_irq,
    NULL);
  sl_gpio_disable_interrupts(1 << int_no);

#endif // SLI_SI917
#endif // DWM3000_INT_PORT

  return _NO_ERR;
}

int uwb2_dwm3000_spi_close(void)
{
  spi_master_close(&uwb2_dwm3000.spi);
  return _NO_ERR;
}

static void port_spi_set_bitrate(uint32_t goal_bitrate)
{
  spi_master_set_speed(&uwb2_dwm3000.spi, goal_bitrate);
}

void set_dw_spi_slow_rate(void)
{
  port_spi_set_bitrate(DWM3000_FREQ_MIN);
}

void set_dw_spi_fast_rate(void)
{
  port_spi_set_bitrate(DWM3000_FREQ_MAX);
}

static inline void spi_transfer_open(void)
{
#if (!SPI_USE_SPIDRV_API) && defined (SL_CATALOG_POWER_MANAGER_PRESENT)
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
#endif // SPI_USE_SPIDRV_API & SL_CATALOG_POWER_MANAGER_PRESENT

#if defined (DWM3000_CS_PORT)
  digital_out_low(&uwb2_dwm3000.gpio_cs);
#endif // DWM3000_CS_PORT
}

static inline void spi_transfer_close(void)
{
#if defined (DWM3000_CS_PORT)
  digital_out_high(&uwb2_dwm3000.gpio_cs);
#endif // DWM3000_CS_PORT

#if (!SPI_USE_SPIDRV_API) && defined (SL_CATALOG_POWER_MANAGER_PRESENT)
  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
#endif // SPI_USE_SPIDRV_API & SL_CATALOG_POWER_MANAGER_PRESENT
}

int readfromspi(uint16_t headerLength,
                uint8_t *headerBuffer,
                uint16_t readLength,
                uint8_t *readBuffer)
{
  dwt_error_e sc = DWT_SUCCESS;

  spi_transfer_open();

  if ((headerBuffer) && (headerLength) && (readBuffer) && (readLength)) {
    if (spi_master_write_then_read(&uwb2_dwm3000.spi, headerBuffer,
                                   headerLength,
                                   readBuffer,
                                   readLength) != SPI_MASTER_SUCCESS) {
      sc = DWT_ERROR;
    }
  }
  spi_transfer_close();

  return sc;
}

static int spi_write(uint16_t headerLength,
                     const uint8_t *headerBuffer,
                     uint16_t bodyLength,
                     const uint8_t *bodyBuffer,
                     uint8_t *crc8)
{
  dwt_error_e sc = DWT_SUCCESS;
  uint32_t total_length = 0;

  spi_transfer_open();

  if (headerBuffer && headerLength) {
    total_length += headerLength;
  }

  if (bodyBuffer && bodyLength) {
    total_length += bodyLength;
  }

  if (crc8) {
    total_length += 1;
  }

  uint8_t tx_buff[total_length];
  uint16_t tx_idx = 0;

  if (headerBuffer && headerLength) {
    for (uint16_t i = 0; i < headerLength; i++)
    {
      tx_buff[tx_idx] = headerBuffer[i];
      tx_idx++;
    }
  }

  if (bodyBuffer && bodyLength) {
    for (uint16_t i = 0; i < bodyLength; i++)
    {
      tx_buff[tx_idx] = bodyBuffer[i];
      tx_idx++;
    }
  }

  if (crc8) {
    tx_buff[tx_idx] = *crc8;
  }

  if (spi_master_write(&uwb2_dwm3000.spi, tx_buff,
                       total_length) != SPI_MASTER_SUCCESS) {
    sc = DWT_ERROR;
  }
  spi_transfer_close();

  return sc;
}

int writetospi(uint16_t headerLength,
               const uint8_t *headerBuffer,
               uint16_t bodyLength,
               const uint8_t *bodyBuffer)
{
  return spi_write(headerLength, headerBuffer, bodyLength, bodyBuffer, NULL);
}

int writetospiwithcrc(uint16_t headerLength,
                      const uint8_t *headerBuffer,
                      uint16_t bodyLength,
                      const uint8_t *bodyBuffer,
                      uint8_t crc8)
{
  return spi_write(headerLength, headerBuffer, bodyLength, bodyBuffer, &crc8);
}

void disable_dw3000_irq(void)
{
#if (defined(SLI_SI917))
  sl_gpio_driver_disable_interrupts(((1 << PIN_INTR_NO) << 16)
                                    | SL_GPIO_INTERRUPT_RISE_EDGE);
#else // SLI_SI917
  sl_gpio_disable_interrupts(1 << DWM3000_INT_PIN);
#endif // SLI_SI917
}

void enable_dw3000_irq(void)
{
#if (defined(SLI_SI917))
  sl_gpio_driver_enable_interrupts(((1 << PIN_INTR_NO) << 16)
                                   | SL_GPIO_INTERRUPT_RISE_EDGE);
#else // SLI_SI917
  sl_gpio_enable_interrupts(1 << DWM3000_INT_PIN);
#endif // SLI_SI917
}

/* @fn      reset_DW1000
 * @brief   DW_RESET pin on DW1000 has 2 functions
 *          In general it is output, but it also can be used to reset the
 *          digital part of DW1000 by driving this pin low.
 *          Note, the DW_RESET pin should not be driven high externally.
 * */
void reset_DW3000(void)
{
#if defined (DWM3000_RESET_PORT)
  digital_out_low(&uwb2_dwm3000.gpio_reset);
#endif // DWM3000_RESET_PORT

  deca_sleep(2);

#if defined (DWM3000_RESET_PORT)
  digital_out_high(&uwb2_dwm3000.gpio_reset);
#endif // DWM3000_RESET_PORT

  deca_sleep(2);
}

__attribute__((weak)) void wakeup_device_with_io(void)
{
  port_wakeup_dw3000_fast();
}

/* @fn      port_wakeup_dw3000_fast
 * @brief   waking up of DW3000 using DW_CS pin
 *
 *          the fast wakeup takes ~1ms:
 *          500us to hold the CS  - TODO: this time can be reduced
 *          500us to the crystal to startup
 *          + ~various time 100us...10ms
 * */
error_e port_wakeup_dw3000_fast(void)
{
  port_wakeup_start();
  deca_usleep(500);
  port_wakeup_end();

  return _NO_ERR;
}

/* @fn      port_wakeup_dw3000
 * @brief   waking up of DW3000 using DW_CS pin
 *
 *          the fast wakeup takes ~1ms:
 *          500us to hold the CS  - TODO: this time can be reduced
 *          500us to the crystal to startup
 *          + ~various time 100us...10ms
 * */
error_e port_wakeup_dw3000(void)
{
  // device wake-up
  port_wakeup_dw3000_fast();

  // it takes ~500us for crystal startup total for the DW3000 to switch to
  //   RC_IDLE 120MHz
  sl_udelay_wait(500);

  // we are here on 120MHz RC

  /* Need to make sure DW IC is in IDLE_RC before proceeding */
  uint32_t cnt = 0;
  for (; (!dwt_checkidlerc()) && (cnt < 10); cnt++)
  {
    // FIXME: DW3000 B0 silicon bugfix
    if (cnt == 5) {
      port_wakeup_dw3000_fast();
    }
    sl_udelay_wait(20);
  }
  return (cnt >= 10) ? _ERR_INIT : _NO_ERR;
}

/**
 *  @brief     Bare-metal level
 *          initialise master/slave DW1000 (check if can talk to device and wake
 *   up and reset)
 */
static int port_init_device(void)
{
  set_dw_spi_slow_rate();

  if (dwt_check_dev_id() != DWT_SUCCESS) {
    return DWT_ERROR;
  }

  // clear the sleep bit in case it is set - so that after the hard reset below
  //   the DW does not go into sleep
  dwt_softreset(0);

  return 0;
}

error_e port_disable_wake_init_dw(void)
{
  disable_dw3000_irq();               /**< disable NVIC IRQ until we configure
                                       *   the device */

  reset_DW3000();

  // this is called here to wake up the device (i.e. if it was in sleep mode
  //   before the restart)
  port_wakeup_dw3000();

  if (port_init_device() != 0x00) {
    return _ERR_INIT;
  }
  return _NO_ERR;
}

error_e port_init_dw_chip(mikroe_spi_handle_t spidrv)
{
  dwt_setlocaldataptr(0);

  int sc = uwb2_dwm3000_spi_init(spidrv);
  if (0 != sc) {
    return _ERR_SPI;
  }
  return _NO_ERR;
}

void port_stop_all_UWB(void)
{
  port_disable_dw_irq_and_reset(1);
  dwt_setcallbacks(NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

/******************************************************************************
 *
 *                              IRQ section
 *
 ******************************************************************************/
/*! ----------------------------------------------------------------------------
 * Function: decamutexon()
 *
 * Description: This function should disable interrupts.
 *
 *
 * input parameters: void
 *
 * output parameters: uint16
 * returns the state of the DW1000 interrupt
 */
decaIrqStatus_t decamutexon(void)
{
  uint32_t interrupts_enabled;
#if (defined(SLI_SI917))
  interrupts_enabled = sl_gpio_driver_get_enabled_interrupts();
  if (interrupts_enabled & ((1 << PIN_INTR_NO) << 16)) {
    sl_gpio_driver_disable_interrupts(((1 << PIN_INTR_NO) << 16)
                                      | SL_GPIO_INTERRUPT_RISE_EDGE);
  }
#else // SLI_SI917
  interrupts_enabled = sl_hal_gpio_get_enabled_interrupts();
  if (interrupts_enabled & (1 << DWM3000_INT_PIN)) {
    sl_gpio_disable_interrupts(1 << DWM3000_INT_PIN);
  }
#endif // SLI_SI917

  return interrupts_enabled;
}

/*! ----------------------------------------------------------------------------
 * Function: decamutexoff()
 *
 * Description: This function should re-enable interrupts, or at least restore
 *              their state as returned(&saved) by decamutexon
 * This is called at the end of a critical section
 *
 * input parameters:
 * @param s - the state of the DW1000 interrupt as returned by decamutexon
 *
 * output parameters
 *
 * returns the state of the DW1000 interrupt
 */
void decamutexoff(decaIrqStatus_t s)
{
#if (defined(SLI_SI917))
  if (s & ((1 << PIN_INTR_NO) << 16)) {
    sl_gpio_driver_enable_interrupts(((1 << PIN_INTR_NO) << 16)
                                     | SL_GPIO_INTERRUPT_RISE_EDGE);
  }
#else // SLI_SI917
  if (s & (1 << DWM3000_INT_PIN)) {
    sl_gpio_enable_interrupts(1 << DWM3000_INT_PIN);
  }
#endif // SLI_SI917
}

/* @fn      process_deca_irq
 * @brief   main call-back for processing of DW3000 IRQ
 *          it re-enters the IRQ routing and processes all events.
 *          After processing of all events, DW3000 will clear the IRQ line.
 * */
#if (defined(SLI_SI917))
static void process_deca_irq(uint32_t int_no)
{
#else // SLI_SI917
static void process_deca_irq(uint8_t int_no, void *ctx)
{
  (void)ctx;
#endif // SLI_SI917
  (void)int_no;

  while (digital_in_read(&uwb2_dwm3000.gpio_int) != 0)
  {
    dwt_isr();
  }   // while DW3000 IRQ line active
}

/*
 * @brief disable DW_IRQ, reset DW3000
 *        and set
 *        app.DwCanSleep = DW_CANNOT_SLEEP;
 *        app.DwEnterSleep = DW_NOT_SLEEPING;
 * */
error_e port_disable_dw_irq_and_reset(int reset)
{
  disable_dw3000_irq();   /**< disable NVIC IRQ until we configure the device */

  // this is called to reset the DW device
  if (reset) {
    reset_DW3000();
  }

  return _NO_ERR;
}

/* Wrapper function to be used by decadriver. Declared in deca_device_api.h */
void deca_sleep(unsigned int time_ms)
{
  sl_udelay_wait(time_ms * 1000);
}

/* Wrapper function to be used by decadriver. Declared in deca_device_api.h */
void deca_usleep(unsigned long time_us)
{
  sl_udelay_wait(time_us);
}
