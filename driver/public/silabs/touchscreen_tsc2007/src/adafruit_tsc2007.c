
#include <stdlib.h>
#include <string.h>

#include "drv_i2c_master.h"
#include "adafruit_tsc2007_config.h"
#include "adafruit_tsc2007.h"

static sl_status_t tsc2007_command(adafruit_tsc2007_function_t func,
                                   adafruit_tsc2007_power_t pwr,
                                   adafruit_tsc2007_resolution_t res,
                                   uint16_t *out);

static i2c_master_t mikroe_i2c = {
  .handle = NULL
};
static i2c_master_config_t mikroe_i2c_config;
static uint16_t t_x, t_y, t_z1, t_z2;

sl_status_t adafruit_tsc2007_init(const struct adafruit_tsc2007_config *config)
{
  mikroe_i2c.handle = config->mikroe_drv_i2c_handle;
  i2c_master_configure_default(&mikroe_i2c_config);
  mikroe_i2c_config.addr = ADAFRUIT_TSC2007_I2CADDR;
//  mikroe_i2c_config.addr = I2C_MASTER_SPEED_FAST;

  i2c_master_open(&mikroe_i2c, &mikroe_i2c_config);

  return tsc2007_command(TSC2007_MEASURE_TEMP0,
                         TSC2007_POWERDOWN_IRQON,
                         TSC2007_ADC_12BIT,
                         NULL);
}

sl_status_t adafruit_tsc2007_read_touch(void)
{
  sl_status_t sc;

  sc = tsc2007_command(TSC2007_MEASURE_X,
                       TSC2007_ADON_IRQOFF,
                       TSC2007_ADC_12BIT,
                       &t_x);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  sc = tsc2007_command(TSC2007_MEASURE_Y,
                       TSC2007_ADON_IRQOFF,
                       TSC2007_ADC_12BIT,
                       &t_y);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  sc = tsc2007_command(TSC2007_MEASURE_Z1,
                       TSC2007_ADON_IRQOFF,
                       TSC2007_ADC_12BIT,
                       &t_z1);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  sc = tsc2007_command(TSC2007_MEASURE_Z2,
                       TSC2007_ADON_IRQOFF,
                       TSC2007_ADC_12BIT,
                       &t_z2);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  sc = tsc2007_command(TSC2007_MEASURE_TEMP0,
                       TSC2007_POWERDOWN_IRQON,
                       TSC2007_ADC_12BIT,
                       NULL);
  if (SL_STATUS_OK != sc) {
    return sc;
  }

  if ((t_x == 4095) || (t_y == 4095)) {
    return SL_STATUS_INVALID_RANGE;
  }
  return SL_STATUS_OK;
}

void adafruit_tsc2007_get_touch(uint16_t *x,
                                uint16_t *y,
                                uint16_t *z1,
                                uint16_t *z2)
{
  if (x && y && z1 && z2) {
    *x = t_x;
    *y = t_y;
    *z1 = t_z1;
    *z2 = t_z2;
  }
}

sl_status_t adafruit_tsc2007_get_touch_point(int32_t *pt_x,
                                             int32_t *pt_y,
                                             int32_t *rtouch)
{
  uint16_t x, y;
  float rz;

  if ((pt_x == NULL) || (pt_y == NULL)) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  if ((t_x == 4095) || (t_y == 4095)) {
    return SL_STATUS_INVALID_RANGE;
  }

#if ADAFRUIT_TSC2007_XY_SWAP != 0
  x = t_y;
  y = t_x;
#else
  x = t_x;
  y = t_y;
#endif

  if (x > ADAFRUIT_TSC2007_X_MIN) {
    x -= ADAFRUIT_TSC2007_X_MIN;
  } else {
    x = 0;
  }

  if (y > ADAFRUIT_TSC2007_Y_MIN) {
    y -= ADAFRUIT_TSC2007_Y_MIN;
  } else {
    y = 0;
  }

  (*pt_x) = (uint32_t)((uint32_t)x * ADAFRUIT_TSC2007_HOR_RES)
            / (ADAFRUIT_TSC2007_X_MAX - ADAFRUIT_TSC2007_X_MIN);

  (*pt_y) = (uint32_t)((uint32_t)y * ADAFRUIT_TSC2007_VER_RES)
            / (ADAFRUIT_TSC2007_Y_MAX - ADAFRUIT_TSC2007_Y_MIN);

#if ADAFRUIT_TSC2007_X_INV != 0
  (*pt_x) = ADAFRUIT_TSC2007_HOR_RES - (*pt_x);
#endif

#if ADAFRUIT_TSC2007_Y_INV != 0
  (*pt_y) = ADAFRUIT_TSC2007_VER_RES - (*pt_y);
#endif

  if (rtouch) {
#if ADAFRUIT_TSC2007_XPLATE_RES > 0
    rz = t_z2;
    rz /= t_z1;
    rz -= 1;
    rz *= t_x;
    rz *= ADAFRUIT_TSC2007_XPLATE_RES;
    rz /= 4096;

    *rtouch = rz;
#else
    *rtouch = 0;
#endif
  }
  return SL_STATUS_OK;
}

static sl_status_t tsc2007_command(adafruit_tsc2007_function_t func,
                                   adafruit_tsc2007_power_t pwr,
                                   adafruit_tsc2007_resolution_t res,
                                   uint16_t *out)
{
  uint8_t cmd;
  uint8_t reply[2];

  if (!mikroe_i2c.handle) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  cmd = (uint8_t)func << 4;
  cmd |= (uint8_t)pwr << 2;
  cmd |= (uint8_t)res << 1;

  if (i2c_master_write_then_read(&mikroe_i2c,
                                 &cmd, 1,
                                 reply, 2) != I2C_MASTER_SUCCESS) {
    return SL_STATUS_TRANSMIT;
  }

  if (out) {
    *out = ((uint16_t)reply[0] << 4) | (reply[1] >> 4); // 12 bits
  }
  return SL_STATUS_OK;
}
