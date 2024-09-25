#ifndef _ADAFRUIT_TSC2007_H
#define _ADAFRUIT_TSC2007_H

#include "sl_status.h"

/*!
 *    @brief  Different function commands
 */
typedef enum {
  TSC2007_MEASURE_TEMP0 = 0,
  TSC2007_MEASURE_AUX = 2,
  TSC2007_MEASURE_TEMP1 = 4,
  TSC2007_ACTIVATE_X = 8,
  TSC2007_ACTIVATE_Y = 9,
  TSC2007_ACTIVATE_YPLUS_X = 10,
  TSC2007_SETUP_COMMAND = 11,
  TSC2007_MEASURE_X = 12,
  TSC2007_MEASURE_Y = 13,
  TSC2007_MEASURE_Z1 = 14,
  TSC2007_MEASURE_Z2 = 15
} adafruit_tsc2007_function_t;

/*!
 *    @brief  Power and IRQ modes
 */
typedef enum {
  TSC2007_POWERDOWN_IRQON = 0,
  TSC2007_ADON_IRQOFF = 1,
  TSC2007_ADOFF_IRQON = 2,
} adafruit_tsc2007_power_t;

/*!
 *    @brief  ADC resolution
 */
typedef enum {
  TSC2007_ADC_12BIT = 0,
  TSC2007_ADC_8BIT = 1,
} adafruit_tsc2007_resolution_t;

struct adafruit_tsc2007_config {
  void *mikroe_drv_i2c_handle;
};

/***************************************************************************//**
 *    @brief
 *      TSC2007 Initialization.
 *   @param[in] x
 *      I2C instance.
 *    @return
 *     Returns SL_STATUS_OK on success, or else error code
 ******************************************************************************/
sl_status_t adafruit_tsc2007_init(const struct adafruit_tsc2007_config *config);

/***************************************************************************//**
 *    @brief
 *      Perform TSC2007 reading.
 *
 *    @return
 *     Returns SL_STATUS_OK on success, or else error code
 ******************************************************************************/
sl_status_t adafruit_tsc2007_read_touch(void);

/***************************************************************************//**
 *    @brief
 *      Get Current TSC2007 ADC value.
 *   @param[out] x
 *      x ADC output.
 *    @param[out] pt_y
 *      y ADC output.
 *    @param[out] z1
 *      z1 ADC output.
 *    @param[out] z2
 *      z2 ADC output.
 *
 ******************************************************************************/
void adafruit_tsc2007_get_touch(uint16_t *x,
                                uint16_t *y,
                                uint16_t *z1,
                                uint16_t *z2);

/***************************************************************************//**
 *    @brief
 *      Get Current touch point coordinate(xy) and pressure(rtouch).
 *
 *    @param[out] pt_x
 *      x-coordinate output.
 *    @param[out] pt_y
 *      y-coordinate output.
 *    @param[out] rtouch
 *      pressure output.
 *    @return
 *     Returns SL_STATUS_OK on success, or else error code
 ******************************************************************************/
sl_status_t adafruit_tsc2007_get_touch_point(int32_t *pt_x,
                                             int32_t *pt_y,
                                             int32_t *rtouch);

void adafruit_tsc2007_register_touch_detect_isr_callback(
  void (*callback)(void));

#endif // _ADAFRUIT_TSC2007_H
