/***************************************************************************//**
 * @file lv_conf.h
 * @brief lvgl config file
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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
 *
 * EVALUATION QUALITY
 * This code has been minimally tested to ensure that it builds with the
 * specified dependency versions and is suitable as a demonstration for
 * evaluation purposes only.
 * This code will be maintained at the sole discretion of Silicon Labs.
 *
 ******************************************************************************/
#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

// <<< Use Configuration Wizard in Context Menu >>>
// <h> LVGL settings

// <q LV_ENABLE_SOFTWARE_ROTATION> Enable software rotation (slower)
// <i> Default: 0
#define LV_ENABLE_SOFTWARE_ROTATION                    0

// <o LV_DISPLAY_ROTATION> Turn the display
// <LV_DISP_ROT_NONE=> none rotation
// <LV_DISP_ROT_90=> 90 degree rotation
// <LV_DISP_ROT_180=> 180 degree rotation
// <LV_DISP_ROT_270=> 270 degree rotation
#define LV_DISPLAY_ROTATION        LV_DISP_ROT_NONE

// <o LV_BUFFER_MODE> Buffering modes
// <0=> Mode 0: One buffer
// <1=> Mode 1: Two buffers
// <2=> Mode 2: Full refresh
#define LV_BUFFER_MODE        0


#define LV_INPUT_TYPE_NONE      0
#define LV_INPUT_TYPE_POINTER   1
#define LV_INPUT_TYPE_KEYPAD    2
#define LV_INPUT_TYPE_BUTTON    3
#define LV_INPUT_TYPE_ENCODER   4

// <o LV_INPUT_TYPE> Type of input device
// <LV_INPUT_TYPE_NONE=> Uninitialized state
// <LV_INPUT_TYPE_POINTER=> Touch pad, mouse, external button
// <LV_INPUT_TYPE_KEYPAD=> Keypad or keyboard
// <LV_INPUT_TYPE_BUTTON=> External (hardware button) which is assigned to a specific point of the screen
// <LV_INPUT_TYPE_ENCODER=> Encoder with only Left, Right turn and a Button
#define LV_INPUT_TYPE   LV_INPUT_TYPE_NONE

// </h>

// <h> Color Settings

/*====================
   COLOR SETTINGS
 *====================*/

// <o LV_COLOR_DEPTH> Color depth.
// <1=> 1 byte per pixel
// <8=> RGB332
// <16=> RGB565
// <32=> ARGB8888
// <d> 16
#define LV_COLOR_DEPTH 16

// <q LV_COLOR_16_SWAP> Swap the 2 bytes of RGB565 color. <0-1>
// <i> Useful if the display has an 8-bit interface (e.g. SPI)
// <d> 0
#define LV_COLOR_16_SWAP 0

// <q LV_COLOR_SCREEN_TRANSP> Enable features to draw on transparent background. <0-1>
// <i> It's required if opa, and transform_* style properties are used.
// <i> Can be also used if the UI is above another layer, e.g. an OSD menu or video player.
#define LV_COLOR_SCREEN_TRANSP 0

// <o LV_COLOR_MIX_ROUND_OFS> Adjust color mix functions rounding.
// <i> GPUs might calculate color mix (blending) differently.
// <0=> 0: round down
// <64=> 64: round up from x.75
// <128=> 128: round up from half
// <192=> 192: round up from x.25
// <254=> 254: round up
#define LV_COLOR_MIX_ROUND_OFS 0

// <o LV_COLOR_CHROMA_KEY_ARGB32> Images pixels with this color will not be drawn if they are chroma keyed). <f.h>
// <i> Default: 0x00ff00
#define LV_COLOR_CHROMA_KEY_ARGB32 0x00ff00
#define LV_COLOR_CHROMA_KEY lv_color_hex(LV_COLOR_CHROMA_KEY_ARGB32)

// </h>

/*=========================
   MEMORY SETTINGS
 *=========================*/

// <h> Memory Settings

// Custom memory allocation.
// 1: use custom malloc/free
// 0: use the built-in `lv_mem_alloc()` and `lv_mem_free()
#define LV_MEM_CUSTOM 0
#if LV_MEM_CUSTOM == 0
// <o LV_MEM_SIZE> Size of the memory available for `lv_mem_alloc()` in bytes (>= 2kB)
// <d> 49152
#define LV_MEM_SIZE 49152U

// <o LV_MEM_ADR> Set an address for the memory pool instead of allocating it as a normal array. Can be in external SRAM too.
// <d> 0
#define LV_MEM_ADR 0     /*0: unused*/

/*Instead of an address give a memory allocator that will be called to get a memory pool for LVGL. E.g. my_malloc*/
#if LV_MEM_ADR == 0
	#undef LV_MEM_POOL_INCLUDE
	#undef LV_MEM_POOL_ALLOC
#endif

#else       /*LV_MEM_CUSTOM*/
#define LV_MEM_CUSTOM_INCLUDE <stdlib.h>   /*Header for the dynamic memory function*/
#define LV_MEM_CUSTOM_ALLOC   malloc
#define LV_MEM_CUSTOM_FREE    free
#define LV_MEM_CUSTOM_REALLOC realloc
#endif     /*LV_MEM_CUSTOM*/

// <o LV_MEM_BUF_MAX_NUM> Number of the intermediate memory buffer used during rendering and other internal processing mechanisms.
// <i> You will see an error log message if there wasn't enough buffers.
#define LV_MEM_BUF_MAX_NUM 16

// <q LV_MEMCPY_MEMSET_STD> Use the standard `memcpy` and `memset` instead of LVGL's own functions. (Might or might not be faster).
// <d> 0
#define LV_MEMCPY_MEMSET_STD 0

// </h>

/*====================
   HAL SETTINGS
 *====================*/

// <h> HAL Settings

// <o LV_DISP_DEF_REFR_PERIOD> Default display refresh period.
// <i> LVG will redraw changed areas with this period time
#define LV_DISP_DEF_REFR_PERIOD 30      /*[ms]*/

// <o LV_INDEV_DEF_READ_PERIOD> Input device read period in milliseconds.
#define LV_INDEV_DEF_READ_PERIOD 30     /*[ms]*/

// Use a custom tick source that tells the elapsed time in milliseconds.
// It removes the need to manually update the tick with `lv_tick_inc()`)
#define LV_TICK_CUSTOM 0
#if LV_TICK_CUSTOM
// <s LV_TICK_CUSTOM_INCLUDE> Header for the system time function
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"

// <o LV_TICK_CUSTOM_SYS_TIME_EXPR> Expression evaluating to current system time in ms.
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

#endif   /*LV_TICK_CUSTOM*/

// <o LV_DPI_DEF> Default Dot Per Inch (px/inch).
// <i> Used to initialize default sizes such as widgets sized, style paddings.
// <i> (Not so important, you can adjust it to modify default sizes and spaces)
#define LV_DPI_DEF 130

// </h>

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/

// <h> Drawing Configuration

/*-------------
 * Drawing
 *-----------*/

// <e LV_DRAW_COMPLEX> Enable complex draw engine.
// <i> Required to draw shadow, gradient, rounded corners, circles, arc, skew lines, image transformations or any masks
// <d> 1
#define LV_DRAW_COMPLEX 1
#if LV_DRAW_COMPLEX != 0

// <o LV_SHADOW_CACHE_SIZE> Allow buffering some shadow calculation.
// <i> LV_SHADOW_CACHE_SIZE is the max. shadow size to buffer, where shadow size is `shadow_width + radius`
// <i> Caching has LV_SHADOW_CACHE_SIZE^2 RAM cost
// <d> 0
#define LV_SHADOW_CACHE_SIZE 0

// <o LV_CIRCLE_CACHE_SIZE> Set number of maximally cached circle data.
// <i> The circumference of 1/4 circle are saved for anti-aliasing
// <i> radius * 4 bytes are used per circle (the most often used radiuses are saved)
// <i> 0: to disable caching */
#define LV_CIRCLE_CACHE_SIZE 4
#endif /*LV_DRAW_COMPLEX*/
// </e>

// <o LV_LAYER_SIMPLE_BUF_SIZE> LV_LAYER_SIMPLE_BUF_SIZE
// <i> "Simple layers" are used when a widget has `style_opa < 255` to buffer the widget into a layer
// <i> and blend it as an image with the given opacity.
// <i> Note that `bg_opa`, `text_opa` etc don't require buffering into layer)
// <i> The widget can be buffered in smaller chunks to avoid using large buffers.
// <i> - LV_LAYER_SIMPLE_BUF_SIZE: [bytes] the optimal target buffer size. LVGL will try to allocate it
// <i> - LV_LAYER_SIMPLE_FALLBACK_BUF_SIZE: [bytes]  used if `LV_LAYER_SIMPLE_BUF_SIZE` couldn't be allocated.
// <i> Both buffer sizes are in bytes.
// <i> "Transformed layers" (where transform_angle/zoom properties are used) use larger buffers
// <i> and can't be drawn in chunks. So these settings affects only widgets with opacity.
#define LV_LAYER_SIMPLE_BUF_SIZE          24576U
// <o LV_LAYER_SIMPLE_FALLBACK_BUF_SIZE> LV_LAYER_SIMPLE_FALLBACK_BUF_SIZE
// <i> It is used if `LV_LAYER_SIMPLE_BUF_SIZE` couldn't be allocated.
#define LV_LAYER_SIMPLE_FALLBACK_BUF_SIZE 3072U

// <o LV_IMG_CACHE_DEF_SIZE> Default image cache size
// <i> Image caching keeps the images opened.
// <i> If only the built-in image formats are used there is no real advantage of caching. (I.e. if no new image decoder is added)
// <i> With complex image decoders (e.g. PNG or JPG) caching can save the continuous open/decode of images.
// <i> However the opened images might consume additional RAM.
// <i> 0: to disable caching
// <d> 0
#define LV_IMG_CACHE_DEF_SIZE 0

// <o LV_GRADIENT_MAX_STOPS> Number of stops allowed per gradient. Increase this to allow more stops.
// <i> This adds (sizeof(lv_color_t) + 1) bytes per additional stop
// <d> 2
#define LV_GRADIENT_MAX_STOPS 2

// <o LV_GRAD_CACHE_DEF_SIZE> Default gradient buffer size.
// <i> When LVGL calculates the gradient "maps" it can save them into a cache to avoid calculating them again.
// <i> LV_GRAD_CACHE_DEF_SIZE sets the size of this cache in bytes.
// <i> If the cache is too small the map will be allocated only while it's required for the drawing.
// <i> 0 mean no caching.
// <d> 0
#define LV_GRAD_CACHE_DEF_SIZE 0


// <e LV_DITHER_GRADIENT> Allow dithering the gradients (to achieve visual smooth color gradients on limited color depth display)
// <i> LV_DITHER_GRADIENT implies allocating one or two more lines of the object's rendering surface
// <i> The increase in memory consumption is (32 bits * object width) plus 24 bits * object width if using error diffusion */
// <d> 0
#define LV_DITHER_GRADIENT 0
#if LV_DITHER_GRADIENT
// <q LV_DITHER_ERROR_DIFFUSION> Add support for error diffusion dithering.
// <i> Error diffusion dithering gets a much better visual result, but implies more CPU consumption and memory when drawing.
// <i> The increase in memory consumption is (24 bits * object's width)
#define LV_DITHER_ERROR_DIFFUSION 0
#endif
// </e>

// <o LV_GRAD_CACHE_DEF_SIZE> Maximum buffer size to allocate for rotation.
// <i> Only used if software rotation is enabled in the display driver.
#define LV_DISP_ROT_MAX_BUF 10240

// </h>

/*-------------
 * GPU
 *-----------*/

/*Use Arm's 2D acceleration library Arm-2D */
#define LV_USE_GPU_ARM2D 0

/*Use STM32's DMA2D (aka Chrom Art) GPU*/
#define LV_USE_GPU_STM32_DMA2D 0
#if LV_USE_GPU_STM32_DMA2D
    /*Must be defined to include path of CMSIS header of target processor
    e.g. "stm32f7xx.h" or "stm32f4xx.h"*/
    #define LV_GPU_DMA2D_CMSIS_INCLUDE
#endif

/*Enable RA6M3 G2D GPU*/
#define LV_USE_GPU_RA6M3_G2D 0
#if LV_USE_GPU_RA6M3_G2D
    /*include path of target processor
    e.g. "hal_data.h"*/
    #define LV_GPU_RA6M3_G2D_INCLUDE "hal_data.h"
#endif

/*Use SWM341's DMA2D GPU*/
#define LV_USE_GPU_SWM341_DMA2D 0
#if LV_USE_GPU_SWM341_DMA2D
    #define LV_GPU_SWM341_DMA2D_INCLUDE "SWM341.h"
#endif

/*Use NXP's PXP GPU iMX RTxxx platforms*/
#define LV_USE_GPU_NXP_PXP 0
#if LV_USE_GPU_NXP_PXP
    /*1: Add default bare metal and FreeRTOS interrupt handling routines for PXP (lv_gpu_nxp_pxp_osa.c)
    *   and call lv_gpu_nxp_pxp_init() automatically during lv_init(). Note that symbol SDK_OS_FREE_RTOS
    *   has to be defined in order to use FreeRTOS OSA, otherwise bare-metal implementation is selected.
    *0: lv_gpu_nxp_pxp_init() has to be called manually before lv_init()
    */
    #define LV_USE_GPU_NXP_PXP_AUTO_INIT 0
#endif

/*Use NXP's VG-Lite GPU iMX RTxxx platforms*/
#define LV_USE_GPU_NXP_VG_LITE 0

/*Use SDL renderer API*/
#define LV_USE_GPU_SDL 0
#if LV_USE_GPU_SDL
    #define LV_GPU_SDL_INCLUDE_PATH <SDL2/SDL.h>
    /*Texture cache size, 8MB by default*/
    #define LV_GPU_SDL_LRU_SIZE (1024 * 1024 * 8)
    /*Custom blend mode for mask drawing, disable if you need to link with older SDL2 lib*/
    #define LV_GPU_SDL_CUSTOM_BLEND_MODE (SDL_VERSION_ATLEAST(2, 0, 6))
#endif

/*-------------
 * Logging
 *-----------*/

// <h> Logging Configuration

// <e LV_USE_LOG> Enable the log module
#define LV_USE_LOG 0
#if LV_USE_LOG

// <o LV_USE_LOG> Log level
// <i> How important log should be added:
// <i> LV_LOG_LEVEL_TRACE       A lot of logs to give detailed information
// <i> LV_LOG_LEVEL_INFO        Log important events
// <i> LV_LOG_LEVEL_WARN        Log if something unwanted happened but didn't cause a problem
// <i> LV_LOG_LEVEL_ERROR       Only critical issue, when the system may fail
// <i> LV_LOG_LEVEL_USER        Only logs added by the user
// <i> LV_LOG_LEVEL_NONE        Do not log anything
// <Trace=> LV_LOG_LEVEL_TRACE
// <Info=> LV_LOG_LEVEL_INFO
// <Warn=> LV_LOG_LEVEL_WARN
// <Error=> LV_LOG_LEVEL_ERROR
// <User=> LV_LOG_LEVEL_USER
// <None=> LV_LOG_LEVEL_NONE
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

// <q LV_LOG_PRINTF> 1: Print the log with 'printf'; 0: User need to register a callback with `lv_log_register_print_cb()`
#define LV_LOG_PRINTF 0

/*Enable/disable LV_LOG_TRACE in modules that produces a huge number of logs*/
#define LV_LOG_TRACE_MEM        1
#define LV_LOG_TRACE_TIMER      1
#define LV_LOG_TRACE_INDEV      1
#define LV_LOG_TRACE_DISP_REFR  1
#define LV_LOG_TRACE_EVENT      1
#define LV_LOG_TRACE_OBJ_CREATE 1
#define LV_LOG_TRACE_LAYOUT     1
#define LV_LOG_TRACE_ANIM       1

#endif  /*LV_USE_LOG*/
// </e>
// </h>

/*-------------
 * Asserts
 *-----------*/

// <h> Asserts Configuration

/*Enable asserts if an operation is failed or an invalid data is found.
 *If LV_USE_LOG is enabled an error message will be printed on failure*/
// <q LV_USE_ASSERT_NULL> Assert NULL
// <i> Check if the parameter is NULL. (Very fast, recommended)
#define LV_USE_ASSERT_NULL          1

// <q LV_USE_ASSERT_MALLOC> Assert malloc fail
// <i> Checks is the memory is successfully allocated or no. (Very fast, recommended)
#define LV_USE_ASSERT_MALLOC        1   /**/

// <q LV_USE_ASSERT_STYLE> Assert style
// <i> Check if the styles are properly initialized. (Very fast, recommended)
#define LV_USE_ASSERT_STYLE         0

// <q LV_USE_ASSERT_MEM_INTEGRITY> Assert memory integrity
// <i> Check the integrity of `lv_mem` after critical operations. (Slow)
#define LV_USE_ASSERT_MEM_INTEGRITY 0

// <q LV_USE_ASSERT_OBJ> Assert object existence
// <i> Check the object's type and existence (e.g. not deleted). (Slow)
#define LV_USE_ASSERT_OBJ           0

/*Add a custom handler when assert happens e.g. to restart the MCU*/
#define LV_ASSERT_HANDLER_INCLUDE <stdint.h>
#define LV_ASSERT_HANDLER while(1);   /*Halt by default*/

// </h>

/*-------------
 * Others
 *-----------*/

/*1: Show CPU usage and FPS count*/
#define LV_USE_PERF_MONITOR 0
#if LV_USE_PERF_MONITOR
    #define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT
#endif

/*1: Show the used memory and the memory fragmentation
 * Requires LV_MEM_CUSTOM = 0*/
#define LV_USE_MEM_MONITOR 0
#if LV_USE_MEM_MONITOR
    #define LV_USE_MEM_MONITOR_POS LV_ALIGN_BOTTOM_LEFT
#endif

/*1: Draw random colored rectangles over the redrawn areas*/
#define LV_USE_REFR_DEBUG 0

/*Change the built in (v)snprintf functions*/
#define LV_SPRINTF_CUSTOM 0
#if LV_SPRINTF_CUSTOM
    #define LV_SPRINTF_INCLUDE <stdio.h>
    #define lv_snprintf  snprintf
    #define lv_vsnprintf vsnprintf
#else   /*LV_SPRINTF_CUSTOM*/
    #define LV_SPRINTF_USE_FLOAT 0
#endif  /*LV_SPRINTF_CUSTOM*/

#define LV_USE_USER_DATA 1

/*Garbage Collector settings
 *Used if lvgl is bound to higher level language and the memory is managed by that language*/
#define LV_ENABLE_GC 0
#if LV_ENABLE_GC != 0
    #define LV_GC_INCLUDE "gc.h"                           /*Include Garbage Collector related things*/
#endif /*LV_ENABLE_GC*/

/*=====================
 *  COMPILER SETTINGS
 *====================*/

/*For big endian systems set to 1*/
#define LV_BIG_ENDIAN_SYSTEM 0

/*Define a custom attribute to `lv_tick_inc` function*/
#define LV_ATTRIBUTE_TICK_INC

/*Define a custom attribute to `lv_timer_handler` function*/
#define LV_ATTRIBUTE_TIMER_HANDLER

/*Define a custom attribute to `lv_disp_flush_ready` function*/
#define LV_ATTRIBUTE_FLUSH_READY

/*Required alignment size for buffers*/
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 1

/*Will be added where memories needs to be aligned (with -Os data might not be aligned to boundary by default).
 * E.g. __attribute__((aligned(4)))*/
#define LV_ATTRIBUTE_MEM_ALIGN

/*Attribute to mark large constant arrays for example font's bitmaps*/
#define LV_ATTRIBUTE_LARGE_CONST

/*Compiler prefix for a big array declaration in RAM*/
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY

/*Place performance critical functions into a faster memory (e.g RAM)*/
#define LV_ATTRIBUTE_FAST_MEM

/*Prefix variables that are used in GPU accelerated operations, often these need to be placed in RAM sections that are DMA accessible*/
#define LV_ATTRIBUTE_DMA

/*Export integer constant to binding. This macro is used with constants in the form of LV_<CONST> that
 *should also appear on LVGL binding API such as Micropython.*/
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning /*The default value just prevents GCC warning*/

/*Extend the default -32k..32k coordinate range to -4M..4M by using int32_t for coordinates instead of int16_t*/
#define LV_USE_LARGE_COORD 0

/*==================
 *   FONT USAGE
 *===================*/

// <h> Font Usage

/*Montserrat fonts with ASCII range and some symbols using bpp = 4
 *https://fonts.google.com/specimen/Montserrat*/
// <q LV_FONT_MONTSERRAT_8> Montserrat 8
// <d> 0
#define LV_FONT_MONTSERRAT_8  0

// <q LV_FONT_MONTSERRAT_10> Montserrat 10
// <d> 0
#define LV_FONT_MONTSERRAT_10 0

// <q LV_FONT_MONTSERRAT_12> Montserrat 12
// <d> 0
#define LV_FONT_MONTSERRAT_12 0

// <q LV_FONT_MONTSERRAT_14> Montserrat 14
// <d> 1
#define LV_FONT_MONTSERRAT_14 1

// <q LV_FONT_MONTSERRAT_16> Montserrat 16
// <d> 0
#define LV_FONT_MONTSERRAT_16 0

// <q LV_FONT_MONTSERRAT_18> Montserrat 18
// <d> 0
#define LV_FONT_MONTSERRAT_18 0

// <q LV_FONT_MONTSERRAT_20> Montserrat 20
// <d> 0
#define LV_FONT_MONTSERRAT_20 0

// <q LV_FONT_MONTSERRAT_22> Montserrat 22
// <d> 0
#define LV_FONT_MONTSERRAT_22 0

// <q LV_FONT_MONTSERRAT_24> Montserrat 24
// <d> 0
#define LV_FONT_MONTSERRAT_24 0

// <q LV_FONT_MONTSERRAT_26> Montserrat 26
// <d> 0
#define LV_FONT_MONTSERRAT_26 0

// <q LV_FONT_MONTSERRAT_28> Montserrat 28
// <d> 0
#define LV_FONT_MONTSERRAT_28 0

// <q LV_FONT_MONTSERRAT_30> Montserrat 30
// <d> 0
#define LV_FONT_MONTSERRAT_30 0

// <q LV_FONT_MONTSERRAT_32> Montserrat 32
// <d> 0
#define LV_FONT_MONTSERRAT_32 0

// <q LV_FONT_MONTSERRAT_34> Montserrat 34
// <d> 0
#define LV_FONT_MONTSERRAT_34 0

// <q LV_FONT_MONTSERRAT_36> Montserrat 36
// <d> 0
#define LV_FONT_MONTSERRAT_36 0

// <q LV_FONT_MONTSERRAT_38> Montserrat 38
// <d> 0
#define LV_FONT_MONTSERRAT_38 0

// <q LV_FONT_MONTSERRAT_40> Montserrat 40
// <d> 0
#define LV_FONT_MONTSERRAT_40 0

// <q LV_FONT_MONTSERRAT_42> Montserrat 42
// <d> 0
#define LV_FONT_MONTSERRAT_42 0

// <q LV_FONT_MONTSERRAT_44> Montserrat 44
// <d> 0
#define LV_FONT_MONTSERRAT_44 0

// <q LV_FONT_MONTSERRAT_46> Montserrat 46
// <d> 0
#define LV_FONT_MONTSERRAT_46 0

// <q LV_FONT_MONTSERRAT_48> Montserrat 48
// <d> 0
#define LV_FONT_MONTSERRAT_48 0

/*Demonstrate special features*/
// <q LV_FONT_MONTSERRAT_12_SUBPX> LV_FONT_MONTSERRAT_12_SUBPX
// <d> 0
#define LV_FONT_MONTSERRAT_12_SUBPX      0

// <q LV_FONT_MONTSERRAT_28_COMPRESSED> LV_FONT_MONTSERRAT_28_COMPRESSED
// <d> 0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0  /*bpp = 3*/

// <q LV_FONT_DEJAVU_16_PERSIAN_HEBREW> LV_FONT_DEJAVU_16_PERSIAN_HEBREW
// <d> 0
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0  /*Hebrew, Arabic, Persian letters and all their forms*/

// <q LV_FONT_SIMSUN_16_CJK> LV_FONT_SIMSUN_16_CJK
// <d> 0
#define LV_FONT_SIMSUN_16_CJK            0  /*1000 most common CJK radicals*/

/*Pixel perfect monospace fonts*/
// <q LV_FONT_UNSCII_8> LV_FONT_UNSCII_8
// <d> 0
#define LV_FONT_UNSCII_8  0

// <q LV_FONT_UNSCII_16> LV_FONT_UNSCII_16
// <d> 0
#define LV_FONT_UNSCII_16 0

/*Optionally declare custom fonts here.
 *You can use these fonts as default font too and they will be available globally.
 *E.g. #define LV_FONT_CUSTOM_DECLARE   LV_FONT_DECLARE(my_font_1) LV_FONT_DECLARE(my_font_2)*/
#define LV_FONT_CUSTOM_DECLARE

// <o LV_FONT_DEFAULT_OBJECT> Set a default font
// <lv_font_montserrat_8=> Montserrat 8
// <lv_font_montserrat_10=> Montserrat 10
// <lv_font_montserrat_12=> Montserrat 12
// <lv_font_montserrat_14=> Montserrat 14
// <d> lv_font_montserrat_14
#define LV_FONT_DEFAULT_OBJECT lv_font_montserrat_14
#define LV_FONT_DEFAULT &(LV_FONT_DEFAULT_OBJECT)

// <q LV_FONT_FMT_TXT_LARGE> Enable handling large font and/or fonts with a lot of characters.
// <d> 0
// <i> The limit depends on the font size, font face and bpp.
// <i> Compiler error will be triggered if a font needs it.
#define LV_FONT_FMT_TXT_LARGE 0

// <q LV_USE_FONT_COMPRESSED> Enables support for compressed fonts.
// <d> 0
#define LV_USE_FONT_COMPRESSED 0

// <e LV_USE_FONT_SUBPX> Enable subpixel rendering.
// <d> 0
#define LV_USE_FONT_SUBPX 0
#if LV_USE_FONT_SUBPX
// <q LV_FONT_SUBPX_BGR> Set the pixel order of the display, 0: RGB; 1:BGR order.
// <i> Physical order of RGB channels. Doesn't matter with "normal" fonts.
// <d> 0
#define LV_FONT_SUBPX_BGR 0
#endif
// </e>

// <q LV_USE_FONT_PLACEHOLDER> Enable drawing placeholders when glyph dsc is not found.
// <d> 1
#define LV_USE_FONT_PLACEHOLDER 1

// </h>

/*=================
 *  TEXT SETTINGS
 *=================*/
// <h> Text Settings

// <o LV_TXT_ENC> Select a character encoding for strings.
// <LV_TXT_ENC_UTF8=> Utf8
// <LV_TXT_ENC_ASCII=> Ascii
// <d> LV_TXT_ENC_UTF8
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/*Can break (wrap) texts on these chars*/
#define LV_TXT_BREAK_CHARS " ,.;:-_"

// <q LV_TXT_LINE_BREAK_LONG_LEN> If a word is at least this long, will break wherever "prettiest.
// <d> 0
#define LV_TXT_LINE_BREAK_LONG_LEN 0

// <o LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN> Minimum number of characters in a long word to put on a line before a break.
// <i> Depends on LV_TXT_LINE_BREAK_LONG_LEN.
// <d> 3
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3

// <o LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN> Minimum number of characters in a long word to put on a line after a break.
// <i> Depends on LV_TXT_LINE_BREAK_LONG_LEN.
// <d> 3
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/*The control character to use for signalling text recoloring.*/
#define LV_TXT_COLOR_CMD "#"

// <e LV_USE_BIDI> Support bidirectional texts.
// <i> Allows mixing Left-to-Right and Right-to-Left texts.
// <i> The direction will be processed according to the Unicode Bidirectional Algorithm:
// <i> https://www.w3.org/International/articles/inline-bidi-markup/uba-basics
// <d> 0
#define LV_USE_BIDI 0
#if LV_USE_BIDI
// <o LV_BIDI_BASE_DIR_DEF> Set the default direction
// <i> Supported values:
// <i> `LV_BASE_DIR_LTR` Left-to-Right
// <i> `LV_BASE_DIR_RTL` Right-to-Left
// <i> `LV_BASE_DIR_AUTO` detect texts base direction
// <LV_BASE_DIR_LTR=> Left-to-Right
// <LV_BASE_DIR_RTL=> Right-to-Left
// <LV_BASE_DIR_AUTO=> Detect texts base direction
// <d> LV_BASE_DIR_AUTO
#define LV_BIDI_BASE_DIR_DEF LV_BASE_DIR_AUTO
#endif
// </e>

// <e LV_USE_ARABIC_PERSIAN_CHARS> Enable Arabic/Persian processing
// <i> In these languages characters should be replaced with an other form based on their position in the text
// <d> 0
#define LV_USE_ARABIC_PERSIAN_CHARS 0
// </e>

// </h>

/*==================
 *  WIDGET USAGE
 *================*/

// <h> Widget Usage

/*Documentation of the widgets: https://docs.lvgl.io/latest/en/html/widgets/index.html*/
// <q LV_USE_ARC> Arc
// <d> 1
#define LV_USE_ARC        1

// <q LV_USE_BAR> Bar
// <d> 1
#define LV_USE_BAR        1

// <q LV_USE_BTN> Button
// <d> 1
#define LV_USE_BTN        1

// <q LV_USE_BTNMATRIX> Button matrix
// <d> 1
#define LV_USE_BTNMATRIX  1

// <q LV_USE_CANVAS> Canvas
// <d> 1
#define LV_USE_CANVAS     1

// <q LV_USE_CHECKBOX> Checkbox
// <d> 1
#define LV_USE_CHECKBOX   1

// <q LV_USE_DROPDOWN> Dropdown
// <i> Requires: lv_label
// <d> 1
#define LV_USE_DROPDOWN   1

// <q LV_USE_IMG> Image
// <i> Requires: lv_label
// <d> 1
#define LV_USE_IMG        1

// <e LV_USE_LABEL> Label
// <d> 1
#define LV_USE_LABEL      1
#if LV_USE_LABEL
// <q LV_LABEL_TEXT_SELECTION> Enable selecting text of the label
// <d> 1
#define LV_LABEL_TEXT_SELECTION 1

// <q LV_LABEL_LONG_TXT_HINT> Store some extra info in labels to speed up drawing of very long texts
// <d> 1
#define LV_LABEL_LONG_TXT_HINT 1
#endif
// </e>

// <q LV_USE_LINE> Line
// <d> 1
#define LV_USE_LINE       1

// <e LV_USE_ROLLER> Roller
// <d> 1
#define LV_USE_ROLLER     1   /*Requires: lv_label*/
#if LV_USE_ROLLER
// <o LV_ROLLER_INF_PAGES> Number of extra "pages" when the roller is infinite
// <d> 7
#define LV_ROLLER_INF_PAGES 7
#endif
// </e>

// <q LV_USE_SLIDER> Slider
// <d> 1
#define LV_USE_SLIDER     1   /*Requires: lv_bar*/

// <q LV_USE_SWITCH> Switch
// <d> 1
#define LV_USE_SWITCH     1

// <e LV_USE_TEXTAREA> Text area
// <i> Requires: lv_label
// <d> 1
#define LV_USE_TEXTAREA   1
#if LV_USE_TEXTAREA != 0
// <o LV_TEXTAREA_DEF_PWD_SHOW_TIME> LV_TEXTAREA_DEF_PWD_SHOW_TIME
// <d> 1500
#define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500    /*ms*/
#endif
// </e>

// <q LV_USE_TABLE> Table
// <d> 1
#define LV_USE_TABLE      1

/*==================
 * EXTRA COMPONENTS
 *==================*/

/*-----------
 * Widgets
 *----------*/
// <q LV_USE_ANIMIMG> Animing
// <d> 1
#define LV_USE_ANIMIMG    1

// <e LV_USE_CALENDAR> Calendar
// <d> 1
#define LV_USE_CALENDAR   1
#if LV_USE_CALENDAR
// <q LV_CALENDAR_WEEK_STARTS_MONDAY> Week start is Monday
// <d> 0
#define LV_CALENDAR_WEEK_STARTS_MONDAY 0
#if LV_CALENDAR_WEEK_STARTS_MONDAY
    #define LV_CALENDAR_DEFAULT_DAY_NAMES {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"}
#else
    #define LV_CALENDAR_DEFAULT_DAY_NAMES {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"}
#endif

#define LV_CALENDAR_DEFAULT_MONTH_NAMES {"January", "February", "March",  "April", "May",  "June", "July", "August", "September", "October", "November", "December"}

// <q LV_USE_CALENDAR_HEADER_ARROW> Calendar header arrow
// <d> 1
#define LV_USE_CALENDAR_HEADER_ARROW 1

// <q LV_USE_CALENDAR_HEADER_DROPDOWN> Calendar header dropdown
// <d> 1
#define LV_USE_CALENDAR_HEADER_DROPDOWN 1
#endif  /*LV_USE_CALENDAR*/
// </e>

// <q LV_USE_CHART> Chart
// <d> 1
#define LV_USE_CHART      1

// <q LV_USE_COLORWHEEL> Color wheel
// <d> 1
#define LV_USE_COLORWHEEL 1

// <q LV_USE_IMGBTN> Image button
// <d> 1
#define LV_USE_IMGBTN     1

// <q LV_USE_KEYBOARD> Keyboard
// <d> 1
#define LV_USE_KEYBOARD   1

// <q LV_USE_LED> Led
// <d> 1
#define LV_USE_LED        1

// <q LV_USE_LIST> List
// <d> 1
#define LV_USE_LIST       1

// <q LV_USE_MENU> Menu
// <d> 1
#define LV_USE_MENU       1

// <q LV_USE_METER> Meter
// <d> 1
#define LV_USE_METER      1

// <q LV_USE_MSGBOX> Msgbox
// <d> 1
#define LV_USE_MSGBOX     1

// <e LV_USE_SPAN> Span
// <d> 1
#define LV_USE_SPAN       1
#if LV_USE_SPAN
// <o LV_SPAN_SNIPPET_STACK_SIZE> A line text can contain maximum num of span descriptor
// <d> 64
#define LV_SPAN_SNIPPET_STACK_SIZE 64
#endif
// </e>

// <q LV_USE_SPINBOX> Spinbox
// <d> 1
#define LV_USE_SPINBOX    1

// <q LV_USE_SPINNER> Spinner
// <d> 1
#define LV_USE_SPINNER    1

// <q LV_USE_TABVIEW> Tabview
// <d> 1
#define LV_USE_TABVIEW    1

// <q LV_USE_TILEVIEW> Titleview
// <d> 1
#define LV_USE_TILEVIEW   1

// <q LV_USE_WIN> Win
// <d> 1
#define LV_USE_WIN        1

// </h>

/*-----------
 * Themes
 *----------*/

// <h> Themes

/*A simple, impressive and very complete theme*/
// <e LV_USE_THEME_DEFAULT> Use default theme
// <d> 1
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT

// <q LV_THEME_DEFAULT_DARK> Dark mode
// <i> 0: Light mode; 1: Dark mode
// <d> 0
#define LV_THEME_DEFAULT_DARK 0

// <q LV_THEME_DEFAULT_GROW> Grow on press
// <d> 1
#define LV_THEME_DEFAULT_GROW 1

// <o LV_THEME_DEFAULT_TRANSITION_TIME> Default transition time in [ms]
// <d> 80
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif /*LV_USE_THEME_DEFAULT*/
// </e>

// <q LV_USE_THEME_BASIC> Simple theme
// <i> A very simple theme that is a good starting point for a custom theme
// <d> 1
#define LV_USE_THEME_BASIC 1

// <q LV_USE_THEME_MONO> Mono theme
// <i> A theme designed for monochrome displays
// <d> 1
#define LV_USE_THEME_MONO 1

// </h>

/*-----------
 * Layouts
 *----------*/

// <h> Layouts

// <q LV_USE_FLEX> Flex Layout
// <i> A layout similar to Flexbox in CSS.
// <d> 1
#define LV_USE_FLEX 1

// <q LV_USE_GRID> Grid Layout
// <i> A layout similar to Grid in CSS.
// <d> 1
#define LV_USE_GRID 1

// </h>

/*---------------------
 * 3rd party libraries
 *--------------------*/

// <h> 3rd party libraries

/*File system interfaces for common APIs */

// <e LV_USE_FS_STDIO> FS stdio library
// <i> API for fopen, fread, etc
// <d> 0
#define LV_USE_FS_STDIO 0
#if LV_USE_FS_STDIO
#define LV_FS_STDIO_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_STDIO_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
// <o LV_FS_STDIO_CACHE_SIZE> Cache size
// <i> >0 to cache this number of bytes in lv_fs_read().
// <d> 0
#define LV_FS_STDIO_CACHE_SIZE 0
#endif
// </e>

// <e LV_USE_FS_POSIX> FS Posix library
// <i> API for open, read, etc
// <d> 0
#define LV_USE_FS_POSIX 0
#if LV_USE_FS_POSIX
#define LV_FS_POSIX_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_POSIX_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
// <o LV_FS_POSIX_CACHE_SIZE> Cache size
// <i> >0 to cache this number of bytes in lv_fs_read().
// <d> 0
#define LV_FS_POSIX_CACHE_SIZE 0
#endif
// </e>

/*API for CreateFile, ReadFile, etc*/
#define LV_USE_FS_WIN32 0
#if LV_USE_FS_WIN32
    #define LV_FS_WIN32_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
    #define LV_FS_WIN32_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
    #define LV_FS_WIN32_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

// <e LV_USE_FS_FATFS> FATFS library
// <i> API for FATFS (needs to be added separately). Uses f_open, f_read, etc
// <d> 0
#define LV_USE_FS_FATFS 0
#if LV_USE_FS_FATFS
#define LV_FS_FATFS_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
// <o LV_FS_FATFS_CACHE_SIZE> Cache size
// <i> >0 to cache this number of bytes in lv_fs_read().
// <d> 0
#define LV_FS_FATFS_CACHE_SIZE 0
#endif
// </e>

// <q LV_USE_PNG> PNG decoder library
// <d> 0
#define LV_USE_PNG 0

// <q LV_USE_BMP> BMP decoder library
// <d> 0
#define LV_USE_BMP 0

// <q LV_USE_SJPG> JPG + split JPG decoder library.
// <i> Split JPG is a custom format optimized for embedded systems.
// <d> 0
#define LV_USE_SJPG 0

// <q LV_USE_GIF> GIF decoder library
// <d> 0
#define LV_USE_GIF 0

// <q LV_USE_QRCODE> QR code library
// <d> 0
#define LV_USE_QRCODE 0

// <e LV_USE_FREETYPE> FreeType library
// <d> 0
#define LV_USE_FREETYPE 0
#if LV_USE_FREETYPE
// <o LV_FREETYPE_CACHE_SIZE> Memory used by FreeType to cache characters [bytes]
// <i> (-1: no caching)
// <d> 16384
#define LV_FREETYPE_CACHE_SIZE 16384U
#if LV_FREETYPE_CACHE_SIZE >= 0
// <q LV_FREETYPE_SBIT_CACHE> Cache use sbit cache
// <i> 1: bitmap cache use the sbit cache, 0:bitmap cache use the image cache.
// <i> sbit cache:it is much more memory efficient for small bitmaps(font size < 256)
// <i> if font size >= 256, must be configured as image cache
// <d> 0
#define LV_FREETYPE_SBIT_CACHE 0

// <o LV_FREETYPE_CACHE_FT_FACES> Maximum number of opened FT_Face objects managed by this cache instance.
// <i> (0:use system defaults)
// <d> 0
#define LV_FREETYPE_CACHE_FT_FACES 0

// <o LV_FREETYPE_CACHE_FT_SIZES> Maximum number of opened FT_Size objects managed by this cache instance.
// <i> (0:use system defaults)
// <d> 0
#define LV_FREETYPE_CACHE_FT_SIZES 0
#endif
#endif
// </e>

// <q LV_USE_RLOTTIE> Rlottie library
// <d> 0
#define LV_USE_RLOTTIE 0

// <e LV_USE_FFMPEG> FFmpeg library for image decoding and playing videos
// <i> Supports all major image formats so do not enable other image decoder with it
// <d> 0
#define LV_USE_FFMPEG 0
#if LV_USE_FFMPEG
// <q LV_FFMPEG_DUMP_FORMAT> Dump input information to stderr
// <d> 0
#define LV_FFMPEG_DUMP_FORMAT 0
#endif
// </e>

// </h>

/*-----------
 * Others
 *----------*/

// <h> Others

// <q LV_USE_SNAPSHOT> Enable API to take snapshot for object
// <d> 0
#define LV_USE_SNAPSHOT 0

// <q LV_USE_MONKEY> Enable Monkey test
// <d> 0
#define LV_USE_MONKEY 0

// <q LV_USE_GRIDNAV> Enable grid navigation
// <d> 0
#define LV_USE_GRIDNAV 0

// <q LV_USE_FRAGMENT> Enable lv_obj fragment
// <d> 0
#define LV_USE_FRAGMENT 0

// <q LV_USE_IMGFONT> Support using images as font in label or span widgets
// <d> 0
#define LV_USE_IMGFONT 0

// <q LV_USE_MSG> Enable a published subscriber based messaging system
// <d> 0
#define LV_USE_MSG 0

// <e LV_USE_IME_PINYIN> Enable Pinyin input method
// <i> Requires: lv_keyboard
// <d> 0
#define LV_USE_IME_PINYIN 0
#if LV_USE_IME_PINYIN
// <q LV_IME_PINYIN_USE_DEFAULT_DICT> Use default thesaurus
// <i> If you do not use the default thesaurus, be sure to use `lv_ime_pinyin` after setting the thesauruss
// <d> 0
#define LV_IME_PINYIN_USE_DEFAULT_DICT 1

// <o LV_IME_PINYIN_CAND_TEXT_NUM> Set the maximum number of candidate panels that can be displayed
// <i> This needs to be adjusted according to the size of the screen
// <d> 6
#define LV_IME_PINYIN_CAND_TEXT_NUM 6

// <e LV_IME_PINYIN_USE_K9_MODE> Use 9 key input(k9)
// <d> 1
#define LV_IME_PINYIN_USE_K9_MODE      1
#if LV_IME_PINYIN_USE_K9_MODE == 1
// <o LV_IME_PINYIN_K9_CAND_TEXT_NUM> LV_IME_PINYIN_K9_CAND_TEXT_NUM
// <d> 3
#define LV_IME_PINYIN_K9_CAND_TEXT_NUM 3
#endif // LV_IME_PINYIN_USE_K9_MODE
// </e>
#endif
// </e>

// </h>

/*==================
* EXAMPLES
*==================*/

/*Enable the examples to be built with the library*/
#define LV_BUILD_EXAMPLES 1

/*===================
 * DEMO USAGE
 ====================*/

/*Show some widget. It might be required to increase `LV_MEM_SIZE` */
#define LV_USE_DEMO_WIDGETS 0
#if LV_USE_DEMO_WIDGETS
#define LV_DEMO_WIDGETS_SLIDESHOW 0
#endif

/*Demonstrate the usage of encoder and keyboard*/
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 0

/*Benchmark your system*/
#define LV_USE_DEMO_BENCHMARK 0
#if LV_USE_DEMO_BENCHMARK
/*Use RGB565A8 images with 16 bit color depth instead of ARGB8565*/
#define LV_DEMO_BENCHMARK_RGB565A8 0
#endif

/*Stress test for LVGL*/
#define LV_USE_DEMO_STRESS 0

/*Music player demo*/
#define LV_USE_DEMO_MUSIC 0
#if LV_USE_DEMO_MUSIC
    #define LV_DEMO_MUSIC_SQUARE    0
    #define LV_DEMO_MUSIC_LANDSCAPE 0
    #define LV_DEMO_MUSIC_ROUND     0
    #define LV_DEMO_MUSIC_LARGE     0
    #define LV_DEMO_MUSIC_AUTO_PLAY 0
#endif

// <<< end of configuration section >>>

/*--END OF LV_CONF_H--*/

#endif /*LV_CONF_H*/
