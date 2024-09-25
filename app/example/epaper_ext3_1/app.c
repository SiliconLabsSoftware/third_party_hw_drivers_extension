/***************************************************************************//**
 * @file
 * @brief Top level application functions
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

#include "app.h"
#include "epaper_display.h"
#include "sl_sleeptimer.h"

#include "glib.h"
#include "glib_font.h"
#include "silabs_logo.h"

static glib_context_t glib_context;
const char *about_silabs = "Silicon Labs is a leader in secure, "
                           "intelligent wireless technology for a more connected world.";

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  glib_init(&glib_context);
  glib_clear(&glib_context);
  // draw silabs logo
  glib_draw_xbitmap(&glib_context,
                    0, 20, silabs_logo,
                    SILABS_LOGO_WIDTH, SILABS_LOGO_HEIGHT, 1);

  glib_set_font(&glib_context, &glib_font_free_sans_9pt7b);
  glib_draw_string(&glib_context, about_silabs,
                   20, 20 + SILABS_LOGO_HEIGHT + 100);
  glib_update_display();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
