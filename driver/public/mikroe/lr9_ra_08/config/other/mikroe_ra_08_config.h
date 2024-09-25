#ifndef MIKROE_RA_08_CONFIG_H_
#define MIKROE_RA_08_CONFIG_H_

#ifndef SLI_SI917
#include "em_gpio.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

// <<< sl:start pin_tool >>>

// <gpio optional=true> LR9_RESET
// $[GPIO_LR9_RESET]
#warning "RESET pin for RA 08 is not configured"
// #define LR9_RESET_PORT       gpioPortC
// #define LR9_RESET_PIN        8
// [GPIO_LR9_RESET]$

// <gpio optional=true> LR9_BT
// $[GPIO_LR9_BT]
#warning "BT pin for RA 08 is not configured"
// #define LR9_BT_PORT         gpioPortB
// #define LR9_BT_PIN          0
// [GPIO_LR9_BT]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // MIKROE_RA_08_CONFIG_H_
