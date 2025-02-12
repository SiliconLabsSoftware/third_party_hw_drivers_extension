/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "mikroe_a4988.h"

#if (defined(SLI_SI917))
#include "sl_si91x_button_instances.h"
#include "rsi_debug.h"

#define app_printf(...)              DEBUGOUT(__VA_ARGS__)
#else
#include "sl_simple_button_instances.h"
#include "app_log.h"

#define app_printf(...)              app_log(__VA_ARGS__)
#endif

// The 28BYJ-48 motor features a 1/64 reduction gear set.
#define MOTOR_GEAR_RATIO             (64)

static uint8_t *mikroe_a4988_state_mgs[] = {
  (uint8_t *)"DISABLED",
  (uint8_t *)"IDLE",
  (uint8_t *)"RUNNING",
  (uint8_t *)"SLEEP",
  (uint8_t *)"ERROR",
};
static bool btn_press = false;

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  sl_status_t stt = SL_STATUS_FAIL;

  app_printf("Stepper2 Click demo application !!!\r\n");

  stt = mikroe_a4988_init();
  app_printf("mikroe_a4988_init = 0x%lx\r\n", stt);

  stt = mikroe_a4988_set_direction(CLOCKWISE);
  app_printf("mikroe_a4988_set_dir clockwise = 0x%lx\r\n", stt);

  mikroe_a4988_reset();
  app_printf("mikroe_a4988_reset\r\n");

  mikroe_a4988_start();
  app_printf("mikroe_a4988_start\r\n");

  app_printf("mikroe_a4988_get_state = %s\r\n",
             mikroe_a4988_state_mgs[mikroe_a4988_get_state()]);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static uint8_t state;
  if (btn_press) {
    btn_press = false;
    state = mikroe_a4988_get_state();
    app_printf("mikroe_a4988_get_state = %s\r\n",
               mikroe_a4988_state_mgs[state]);
  }
}

/***************************************************************************//**
 * Emergency button: if the motor is enabled then a button press disables it.
 * The next button press will enable and start it again.
 ******************************************************************************/
#if (defined(SLI_SI917))
void sl_si91x_button_isr(uint8_t pin, int8_t state)
{
  if ((pin == button_btn0.pin) && (state == BUTTON_PRESSED)) {
    btn_press = true;
    if (mikroe_a4988_get_state() == DISABLED) {
      mikroe_a4988_enable(true);
      mikroe_a4988_start();
    } else {
      mikroe_a4988_stop();
      mikroe_a4988_enable(false);
    }
  }
}

#else
void sl_button_on_change(const sl_button_t *handle)
{
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
    btn_press = true;
    if (mikroe_a4988_get_state() == DISABLED) {
      mikroe_a4988_enable(true);
      mikroe_a4988_start();
    } else {
      mikroe_a4988_stop();
      mikroe_a4988_enable(false);
    }
  }
}

#endif
