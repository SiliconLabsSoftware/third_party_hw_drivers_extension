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
#include "em_cmu.h"
#include "em_usart.h"
#include "em_eusart.h"
#include "sl_component_catalog.h"
#if defined (SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif // SL_CATALOG_POWER_MANAGER_PRESENT
#include "uwb2_dwm3000_config.h"

#include "sl_hal_gpio.h"
#include "sl_device_clock.h"
#include "sl_device_peripheral.h"
#include "sl_clock_manager.h"
#include "sl_gpio.h"
#include "sl_udelay.h"

#define _HAL_CAT2EXP(a, b)           a ## b
#define HAL_CAT2(a, b)               _HAL_CAT2EXP(a, b)
#define _HAL_CAT3EXP(a, b, c)        a ## b ## c
#define HAL_CAT3(a, b, c)            _HAL_CAT3EXP(a, b, c)

// UWB/DW3000
#define HAL_UWB_IRQ_PORT              DWM3000_INT_PORT
#define HAL_UWB_IRQ_PIN               DWM3000_INT_PIN
#define HAL_UWB_IRQ_IT_NBR            DWM3000_INT_PIN

#if defined(DWM3000_RESET_PORT) && defined(DWM3000_RESET_PIN)
#define HAL_UWB_RST_PORT              DWM3000_RESET_PORT
#define HAL_UWB_RST_PIN               DWM3000_RESET_PIN
#define HAL_UWB_RST_IT_NBR            DWM3000_RESET_PIN
#endif

#if defined(DWM3000_WAKE_PORT) && defined(DWM3000_WAKE_PIN)
#define HAL_UWB_WAKEUP_PORT           DWM3000_WAKE_PORT
#define HAL_UWB_WAKEUP_PIN            DWM3000_WAKE_PIN
#define HAL_UWB_WAKEUP_IT_NBR         DWM3000_WAKE_PIN
#endif

#if defined(DWM3000_ON_PORT) && defined(DWM3000_ON_PIN)
#define HAL_UWB_EXTON_PORT            DWM3000_ON_PORT
#define HAL_UWB_EXTON_PIN             DWM3000_ON_PIN
#define HAL_UWB_EXTON_IT_NBR          DWM3000_ON_PIN
#endif

#define HAL_SPI_PERIPHERAL            DWM3000_PERIPHERAL

#define HAL_SPI_CS_PORT               DWM3000_CS_PORT
#define HAL_SPI_CS_PIN                DWM3000_CS_PIN

#define HAL_SPI_CLK_PORT              DWM3000_CLK_PORT
#define HAL_SPI_CLK_PIN               DWM3000_CLK_PIN

#define HAL_SPI_MISO_PORT             DWM3000_RX_PORT
#define HAL_SPI_MISO_PIN              DWM3000_RX_PIN

#define HAL_SPI_MOSI_PORT             DWM3000_TX_PORT
#define HAL_SPI_MOSI_PIN              DWM3000_TX_PIN

#define HAL_SPI_FREQ_MIN              DWM3000_FREQ_MIN
#define HAL_SPI_FREQ_MAX              DWM3000_FREQ_MAX

#define SPI_GPIO_SLOW_SLEWRATE        5
#define SPI_GPIO_FAST_SLEWRATE        7
#define SPI_DUMMY_TX_VALUE            0xFF

static void process_deca_irq(uint8_t int_no, void *ctx);

#if defined(HAL_UWB_WAKEUP_PORT) && defined(HAL_UWB_WAKEUP_PIN)
#define port_wakeup_start()  sl_gpio_set_pin_mode(&hal_uwb_wake,          \
                                                  SL_GPIO_MODE_PUSH_PULL, \
                                                  1);
#define port_wakeup_end()    sl_gpio_set_pin_mode(&hal_uwb_wake,          \
                                                  SL_GPIO_MODE_PUSH_PULL, \
                                                  0);
#else
#define port_wakeup_start()  sl_gpio_set_pin_mode(&hal_spi_cs,            \
                                                  SL_GPIO_MODE_PUSH_PULL, \
                                                  0);
#define port_wakeup_end()    sl_gpio_set_pin_mode(&hal_spi_cs,            \
                                                  SL_GPIO_MODE_PUSH_PULL, \
                                                  1);
#endif

enum SpiPeripheralType {
  spiPeripheralTypeUsart = 0,         ///< USART peripheral
  spiPeripheralTypeEusart = 1         ///< EUSART peripheral
};

enum SpiPeripheralType spi_peripheral_type;

static const sl_gpio_t hal_spi_cs =
{
  .port = HAL_SPI_CS_PORT,
  .pin = HAL_SPI_CS_PIN
};

static const sl_gpio_t hal_spi_clk =
{
  .port = HAL_SPI_CLK_PORT,
  .pin = HAL_SPI_CLK_PIN
};

static const sl_gpio_t hal_spi_mosi =
{
  .port = HAL_SPI_MOSI_PORT,
  .pin = HAL_SPI_MOSI_PIN
};

static const sl_gpio_t hal_spi_miso =
{
  .port = HAL_SPI_MISO_PORT,
  .pin = HAL_SPI_MISO_PIN
};

static const sl_gpio_t hal_uwb_irq =
{
  .port = HAL_UWB_IRQ_PORT,
  .pin = HAL_UWB_IRQ_PIN
};

#if defined (HAL_UWB_RST_PORT) && defined (HAL_UWB_RST_PIN)
static const sl_gpio_t hal_uwb_reset =
{
  .port = DWM3000_RESET_PORT,
  .pin = DWM3000_RESET_PIN
};
#endif

#if defined (HAL_UWB_WAKEUP_PORT) && defined (HAL_UWB_WAKEUP_PIN)
static const sl_gpio_t hal_uwb_wake =
{
  .port = HAL_UWB_WAKEUP_PORT,
  .pin = HAL_UWB_WAKEUP_PIN
};
#endif

#if defined (DWM3000_ON_PORT) && defined (DWM3000_ON_PIN)
static const sl_gpio_t hal_uwb_exton =
{
  .port = DWM3000_ON_PORT,
  .pin = DWM3000_ON_PIN
};
#endif

static volatile bool spi_lock = false;

static sl_status_t init_usart(USART_TypeDef *usart)
{
  USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;
  sl_bus_clock_t bus_clock;

  usartInit.msbf = true;
  usartInit.clockMode = usartClockMode0;
  usartInit.master = true;
  usartInit.baudrate = HAL_SPI_FREQ_MIN;
  usartInit.autoCsEnable = false;
  usartInit.databits = usartDatabits8;

  sl_clock_manager_enable_bus_clock(SL_BUS_CLOCK_GPIO);
  if (0) {
  }
#if defined(USART0)
  else if (usart == USART0) {
    bus_clock = sl_device_peripheral_get_bus_clock(SL_PERIPHERAL_USART0);
  }
#endif
#if defined(USART1)
  else if (usart == USART1) {
    bus_clock = sl_device_peripheral_get_bus_clock(SL_PERIPHERAL_USART1);
  }
#endif
#if defined(USART2)
  else if (usart == USART2) {
    bus_clock = sl_device_peripheral_get_bus_clock(SL_PERIPHERAL_USART2);
  }
#endif
  else {
    return SL_STATUS_INVALID_PARAMETER;
  }
  sl_clock_manager_enable_bus_clock(bus_clock);

  USART_InitSync(usart, &usartInit);

#if defined(USART_ROUTEPEN_TXPEN)
  HAL_SPI_PERIPHERAL->ROUTELOC0 =
    (HAL_SPI_PERIPHERAL->ROUTELOC0
     & ~(_USART_ROUTELOC0_TXLOC_MASK
         | _USART_ROUTELOC0_RXLOC_MASK
         | _USART_ROUTELOC0_CLKLOC_MASK))
    | (HAL_SPI_TX_LOC  << _USART_ROUTELOC0_TXLOC_SHIFT)
    | (HAL_SPI_RX_LOC  << _USART_ROUTELOC0_RXLOC_SHIFT)
    | (HAL_SPI_CLK_LOC << _USART_ROUTELOC0_CLKLOC_SHIFT);

  HAL_SPI_PERIPHERAL->ROUTEPEN = USART_ROUTEPEN_TXPEN
                                 | USART_ROUTEPEN_RXPEN
                                 | USART_ROUTEPEN_CLKPEN
                                 | USART_ROUTEPEN_CSPEN;
#else
  GPIO->USARTROUTE[DWM3000_PERIPHERAL_NO].ROUTEEN =
    GPIO_USART_ROUTEEN_TXPEN
    | GPIO_USART_ROUTEEN_RXPEN
    | GPIO_USART_ROUTEEN_CLKPEN;
  GPIO->USARTROUTE[DWM3000_PERIPHERAL_NO].TXROUTE =
    ((uint32_t)DWM3000_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
    | ((uint32_t)DWM3000_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[DWM3000_PERIPHERAL_NO].RXROUTE =
    ((uint32_t)DWM3000_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
    | ((uint32_t)DWM3000_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[DWM3000_PERIPHERAL_NO].CLKROUTE =
    ((uint32_t)DWM3000_CLK_PORT << _GPIO_USART_CLKROUTE_PORT_SHIFT)
    | ((uint32_t)DWM3000_CLK_PIN << _GPIO_USART_CLKROUTE_PIN_SHIFT);
#endif
  return SL_STATUS_OK;
}

#if defined(EUSART_PRESENT)
static sl_status_t init_eusart(EUSART_TypeDef *eusart)
{
  sl_bus_clock_t bus_clock;

  EUSART_SpiAdvancedInit_TypeDef eusartAdvancedSpiInit =
    EUSART_SPI_ADVANCED_INIT_DEFAULT;
  EUSART_SpiInit_TypeDef eusartSpiInit = EUSART_SPI_MASTER_INIT_DEFAULT_HF;
  eusartSpiInit.advancedSettings = &eusartAdvancedSpiInit;

  if (0) {
#if defined(EUSART0)
  } else if (eusart == EUSART0) {
    bus_clock = sl_device_peripheral_get_bus_clock(SL_PERIPHERAL_EUSART0);
#endif
#if defined(EUSART1)
  } else if (eusart == EUSART1) {
    bus_clock = sl_device_peripheral_get_bus_clock(SL_PERIPHERAL_EUSART1);
#endif
#if defined(EUSART2)
  } else if (eusart == EUSART2) {
    bus_clock = sl_device_peripheral_get_bus_clock(SL_PERIPHERAL_EUSART2);
#endif
#if defined(EUSART3)
  } else if (eusart == EUSART3) {
    bus_clock = sl_device_peripheral_get_bus_clock(SL_PERIPHERAL_EUSART3);
#endif
#if defined(EUSART4)
  } else if (eusart == EUSART4) {
    bus_clock = sl_device_peripheral_get_bus_clock(SL_PERIPHERAL_EUSART4);
#endif
  } else {
    return SL_STATUS_INVALID_PARAMETER;
  }

  sl_clock_manager_enable_bus_clock(SL_BUS_CLOCK_GPIO);
  sl_clock_manager_enable_bus_clock(bus_clock);

  eusartSpiInit.clockMode = eusartClockMode0;
  eusartSpiInit.bitRate = HAL_SPI_FREQ_MIN;
  eusartSpiInit.master = true;
  eusartSpiInit.databits = eusartDataBits8;

  eusartAdvancedSpiInit.msbFirst = true;
  eusartAdvancedSpiInit.forceLoad = false;
  eusartAdvancedSpiInit.autoCsEnable = false;

  GPIO->EUSARTROUTE[DWM3000_PERIPHERAL_NO].TXROUTE =
    ((uint32_t)HAL_SPI_MOSI_PORT
      << _GPIO_EUSART_TXROUTE_PORT_SHIFT)
    | ((uint32_t)HAL_SPI_MOSI_PIN
       << _GPIO_EUSART_TXROUTE_PIN_SHIFT);
  GPIO->EUSARTROUTE[DWM3000_PERIPHERAL_NO].RXROUTE =
    ((uint32_t)HAL_SPI_MISO_PORT
      << _GPIO_EUSART_RXROUTE_PORT_SHIFT)
    | ((uint32_t)HAL_SPI_MISO_PIN
       << _GPIO_EUSART_RXROUTE_PIN_SHIFT);
  GPIO->EUSARTROUTE[DWM3000_PERIPHERAL_NO].SCLKROUTE =
    ((uint32_t)HAL_SPI_CLK_PORT
      << _GPIO_EUSART_SCLKROUTE_PORT_SHIFT)
    | ((uint32_t)HAL_SPI_CLK_PIN
       << _GPIO_EUSART_SCLKROUTE_PIN_SHIFT);

  GPIO->EUSARTROUTE[DWM3000_PERIPHERAL_NO].ROUTEEN =
    GPIO_EUSART_ROUTEEN_TXPEN
    | GPIO_EUSART_ROUTEEN_RXPEN
    | GPIO_EUSART_ROUTEEN_SCLKPEN;

  EUSART_SpiInit(eusart, &eusartSpiInit);

  return SL_STATUS_OK;
}

#endif // #if defined(EUSART_PRESENT)

/*!
 *
 * -----------------------------------------------------------------------------
 * Function: openspi()
 *
 * Low level abstract function to open and initialise access to the SPI device.
 * returns 0 for success, or -1 for error
 */
int openspi(void)
{
#if defined(EUSART_PRESENT)
  if (EUSART_NUM(HAL_SPI_PERIPHERAL) != -1) {
    init_eusart(HAL_SPI_PERIPHERAL);
    spi_peripheral_type = spiPeripheralTypeEusart;
  } else {
    init_usart(HAL_SPI_PERIPHERAL);
    spi_peripheral_type = spiPeripheralTypeUsart;
  }
#else
  init_usart(HAL_SPI_PERIPHERAL);
  spi_peripheral_type = spiPeripheralTypeUsart;
#endif

  sl_gpio_set_pin_mode(&hal_spi_clk, SL_GPIO_MODE_PUSH_PULL, 0);
  sl_gpio_set_pin_mode(&hal_spi_mosi, SL_GPIO_MODE_PUSH_PULL, 0);
  sl_gpio_set_pin_mode(&hal_spi_miso, SL_GPIO_MODE_INPUT_PULL, 0);
  sl_gpio_set_pin_mode(&hal_spi_cs, SL_GPIO_MODE_PUSH_PULL, 1);

#if defined (DWM3000_ON_PORT) && defined (DWM3000_ON_PIN)
  sl_gpio_set_pin_mode(&hal_uwb_exton, SL_GPIO_MODE_PUSH_PULL, 1);
#endif
  return 0;
}

/*!
 *
 * -----------------------------------------------------------------------------
 * Function: closespi()
 *
 * Low level abstract function to close the the SPI device.
 * returns 0 for success, or -1 for error
 */
int closespi(void)
{
  if (0) {
  }
#if defined(USART_PRESENT)
  else if (spiPeripheralTypeUsart == spi_peripheral_type) {
    USART_Reset((USART_TypeDef *)HAL_SPI_PERIPHERAL);
  }
#endif
#if defined(EUSART_PRESENT)
  else if (spiPeripheralTypeEusart == spi_peripheral_type) {
#if defined(_SILICON_LABS_32B_SERIES_2)
    EUSART_Reset((EUSART_TypeDef *)HAL_SPI_PERIPHERAL);
#else
    sl_hal_eusart_reset(HAL_SPI_PERIPHERAL);
#endif
  }
#endif
  else {
    return -1;
  }
  return 0;
}

static void port_spi_set_bitrate(uint32_t goal_bitrate)
{
  uint8_t required_slewrate =
    (goal_bitrate
     >= 10000000) ? SPI_GPIO_FAST_SLEWRATE : SPI_GPIO_SLOW_SLEWRATE;
  sl_gpio_set_slew_rate(hal_spi_clk.port,
                        required_slewrate);
  sl_gpio_set_slew_rate(hal_spi_mosi.port,
                        required_slewrate);

  if (spiPeripheralTypeUsart == spi_peripheral_type) {
    USART_BaudrateSyncSet((USART_TypeDef *)HAL_SPI_PERIPHERAL, 0, goal_bitrate);
  } else if (spiPeripheralTypeEusart == spi_peripheral_type) {
    EUSART_BaudrateSet((EUSART_TypeDef *)HAL_SPI_PERIPHERAL, 0, goal_bitrate);
  }
}

/* @fn  set_dw_spi_slow_rate
 * @brief sets slow SPI clock speed for the DW chip
 *        left for compatibility.
 * */
void set_dw_spi_slow_rate(void)
{
  port_spi_set_bitrate(HAL_SPI_FREQ_MIN);
}

/* @fn      set_dw_spi_fast_rate
 * @brief   sets High SPI clock speed for the DW chip
 * */
void set_dw_spi_fast_rate(void)
{
  port_spi_set_bitrate(HAL_SPI_FREQ_MAX);
}

static inline void spi_transfer_open(void)
{
  while (spi_lock) {}
  spi_lock = true;

#if defined (SL_CATALOG_POWER_MANAGER_PRESENT)
  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
#endif
  sl_gpio_clear_pin(&hal_spi_cs);
}

static inline void spi_transfer_close(void)
{
  sl_gpio_set_pin(&hal_spi_cs);
#if defined (SL_CATALOG_POWER_MANAGER_PRESENT)
  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
#endif
  spi_lock = false;
}

static inline uint16_t spi_tx_rx(uint16_t data)
{
  if (0) {
  }
#if defined(USART_PRESENT)
  else if (spiPeripheralTypeUsart == spi_peripheral_type) {
    return USART_SpiTransfer((USART_TypeDef *)HAL_SPI_PERIPHERAL, data);
  }
#endif
#if defined(EUSART_PRESENT)
  else if (spiPeripheralTypeEusart == spi_peripheral_type) {
    return EUSART_Spi_TxRx((EUSART_TypeDef *)HAL_SPI_PERIPHERAL, data);
  }
#endif
  return 0;
}

static inline sl_status_t spi_blocking_transmit(const uint8_t *buffer,
                                                int count)
{
  for (int i = 0; i < count; i++) {
    spi_tx_rx(buffer[i]);
  }
  return SL_STATUS_OK;
}

static inline sl_status_t spi_blocking_receive(uint8_t *buffer, int count)
{
  for (int i = 0; i < count; i++) {
    buffer[i] = spi_tx_rx(SPI_DUMMY_TX_VALUE);
  }
  return SL_STATUS_OK;
}

int readfromspi(uint16_t headerLength,
                uint8_t *headerBuffer,
                uint16_t readLength,
                uint8_t *readBuffer)
{
  sl_status_t sc = 0;

  spi_transfer_open();
  if (headerBuffer && headerLength) {
    sc |= spi_blocking_transmit(headerBuffer, headerLength);
  }
  if (!sc && readBuffer && readLength) {
    sc |= spi_blocking_receive(readBuffer, readLength);
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
  sl_status_t sc = 0;

  spi_transfer_open();
  if (headerBuffer && headerLength) {
    sc |= spi_blocking_transmit(headerBuffer, headerLength);
  }
  if (!sc && bodyBuffer && bodyLength) {
    sc |= spi_blocking_transmit(bodyBuffer, bodyLength);
  }
  if (crc8) {
    sc |= spi_blocking_transmit(crc8, sizeof(*crc8));
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
  sl_gpio_disable_interrupts(1 << HAL_UWB_IRQ_IT_NBR);
}

void enable_dw3000_irq(void)
{
  sl_gpio_enable_interrupts(1 << HAL_UWB_IRQ_IT_NBR);
}

/* @fn      reset_DW1000
 * @brief   DW_RESET pin on DW1000 has 2 functions
 *          In general it is output, but it also can be used to reset the
 *          digital part of DW1000 by driving this pin low.
 *          Note, the DW_RESET pin should not be driven high externally.
 * */
void reset_DW3000(void)
{
  sl_gpio_set_pin_mode(&hal_uwb_reset, SL_GPIO_MODE_PUSH_PULL, 0);
  sl_udelay_wait(2000);
  sl_gpio_set_pin_mode(&hal_uwb_reset, SL_GPIO_MODE_PUSH_PULL, 1);
  sl_udelay_wait(2000);
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
  sl_udelay_wait(500);
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

error_e port_init_dw_chip(void)
{
  dwt_setlocaldataptr(0);

  int sc = openspi();
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
  uint32_t s = sl_hal_gpio_get_enabled_interrupts();
  if (s & (1 << HAL_UWB_IRQ_IT_NBR)) {
    sl_gpio_disable_interrupts(1 << HAL_UWB_IRQ_IT_NBR);
  }
  return s;
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
  if (s & (1 << HAL_UWB_IRQ_IT_NBR)) {
    sl_gpio_enable_interrupts(1 << HAL_UWB_IRQ_IT_NBR);
  }
}

/* @fn      process_deca_irq
 * @brief   main call-back for processing of DW3000 IRQ
 *          it re-enters the IRQ routing and processes all events.
 *          After processing of all events, DW3000 will clear the IRQ line.
 * */
static void process_deca_irq(uint8_t int_no, void *ctx)
{
  (void)ctx;
  (void)int_no;
  bool pin_value;

  while (SL_STATUS_OK == sl_gpio_get_pin_input(&hal_uwb_irq, &pin_value)) {
    if (!pin_value) {
      break;
    }
    dwt_isr();
  }
}

void dw_irq_init(void)
{
  sl_gpio_set_pin_mode(&hal_uwb_irq, SL_GPIO_MODE_INPUT_PULL, 0);

  int32_t int_no = DWM3000_INT_PIN;
  sl_gpio_configure_external_interrupt(
    &hal_uwb_irq,
    &int_no,
    SL_GPIO_INTERRUPT_RISING_EDGE,
    process_deca_irq,
    NULL);
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
