/*******************************************************************************
 * @file app.c
 * @brief Example application
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

#include "sl_cli_command.h"
#include "sl_cli_handles.h"
#include "ir_array_amg88xx.h"

#if (defined(SLI_SI917))
#include "sl_i2c_instances.h"
#else
#include "sl_i2cspm_instances.h"
#endif

#if (defined(SLI_SI917))
#define I2C_INSTANCE_USED            SL_I2C2
static sl_i2c_instance_t i2c_instance = I2C_INSTANCE_USED;
#endif

static mikroe_i2c_handle_t app_i2c_instance = NULL;

extern const sl_cli_command_entry_t app_cli_command_table[];

extern sl_cli_handle_t sl_cli_vcom_handle;
extern sl_cli_command_group_t sl_cli_vcom_command_group;

sl_cli_command_group_t sl_cli_app_command_group =
{
  { NULL },
  false,
  app_cli_command_table
};

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
#if (defined(SLI_SI917))
  app_i2c_instance = &i2c_instance;
#else
  app_i2c_instance = sl_i2cspm_qwiic;
#endif

  amg88xx_init(app_i2c_instance, AMG88XX_ADDRESS_OPEN, CELSIUS);
  // Remove unused autogen CLI command group
  sl_cli_command_remove_command_group(sl_cli_vcom_handle,
                                      &sl_cli_vcom_command_group);

  // Add application CLI commands
  sl_cli_command_add_command_group(sl_cli_vcom_handle,
                                   &sl_cli_app_command_group);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  //
}
