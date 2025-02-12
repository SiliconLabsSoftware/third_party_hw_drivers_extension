#ifndef SCL_STN1110_CONFIG_H_
#define SCL_STN1110_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// <<< sl:start pin_tool >>>

// <gpio optional=true> STN1110_RESET
// $[GPIO_STN1110_RESET]
#define STN1110_RESET_PORT       SL_GPIO_PORT_C
#define STN1110_RESET_PIN        8
// [GPIO_STN1110_RESET]$

// <gpio optional=true> STN1110_INT
// $[GPIO_STN1110_INT]
#define STN1110_INT_PORT         SL_GPIO_PORT_B
#define STN1110_INT_PIN          1
// [GPIO_STN1110_INT]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // SCL_STN1110_CONFIG_H_
