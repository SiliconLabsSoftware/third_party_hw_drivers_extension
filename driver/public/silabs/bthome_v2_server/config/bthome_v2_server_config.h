/***************************************************************************//**
 * @file bthome_v2_server_config.h
 * @brief BThome v2 Server Configuration
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

#ifndef BTHOME_V2_SERVER_CONFIG_H_
#define BTHOME_V2_SERVER_CONFIG_H_

// <<< Use Configuration Wizard in Context Menu >>>

// <o MAX_ENCRYPT_DEVICE> The maximum number of encrypted devices can supported store key.
// <i> Default: 5
#define MAX_ENCRYPT_DEVICE    5

// <o MAX_DEVICE> The maximum number of device that can be managed.
// <i> Default: 10
#define MAX_DEVICE            10

// <<< end of configuration section >>>

#endif // BTHOME_V2_SERVER_CONFIG_H_
