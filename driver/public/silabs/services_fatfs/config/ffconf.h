/***************************************************************************//**
 * @file ffconf.h
 * @brief fatfs config file
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
#ifndef FFCONF_H_
#define FFCONF_H_
/*---------------------------------------------------------------------------/
/  Configurations of FatFs Module
/---------------------------------------------------------------------------*/

#define FFCONF_DEF  80286 /* Revision ID */

/*---------------------------------------------------------------------------/
/ Function Configurations
/---------------------------------------------------------------------------*/
// <<< Use Configuration Wizard in Context Menu >>>
// <h> Configurations of FatFs Module

// <q FF_FS_READONLY> Enable read-only file system. <0-1>
// <i> This option switches read-only configuration. (0:Read/Write or 1:Read-only)
// <i> Read-only configuration removes writing API functions, f_write(), f_sync(),
// <i> f_unlink(), f_mkdir(), f_chmod(), f_rename(), f_truncate(), f_getfree()
// <i> and optional writing functions as well.
// <i> Default: 0
#define FF_FS_READONLY  0

// <o FF_FS_MINIMIZE> Define minimization level to remove some basic API functions. <0-3>
#define FF_FS_MINIMIZE  0
// <i> This option defines minimization level to remove some basic API functions.
// <i>
// <i> 0: Basic functions are fully enabled.
// <i> 1: f_stat(), f_getfree(), f_unlink(), f_mkdir(), f_truncate() and f_rename()
// <i> are removed.
// <i> 2: f_opendir(), f_readdir() and f_closedir() are removed in addition to 1.
// <i> 3: f_lseek() function is removed in addition to 2.

// <o FF_USE_FIND> Filtered directory read functions option. <0-2>
#define FF_USE_FIND   0
// <i> This option switches filtered directory read functions, f_findfirst() and
// <i> f_findnext(). (0:Disable, 1:Enable 2:Enable with matching altname[] too)

// <q FF_USE_MKFS> Enable f_mkfs() function. <0-1>
#define FF_USE_MKFS   0
// <i> This option switches f_mkfs() function. (0:Disable or 1:Enable)

// <q FF_USE_FASTSEEK> Enable fast seek function. <0-1>
#define FF_USE_FASTSEEK 0
// <i> This option switches fast seek function. (0:Disable or 1:Enable)

// <q FF_USE_EXPAND> Enable f_expand() function. <0-1>
#define FF_USE_EXPAND 0
// <i> This option switches f_expand function. (0:Disable or 1:Enable)

// <q FF_USE_CHMOD> Enable f_chmod() and f_utime() function. <0-1>
#define FF_USE_CHMOD  0
// <i> This option switches attribute manipulation functions, f_chmod() and f_utime().
// <i> (0:Disable or 1:Enable) Also FF_FS_READONLY needs to be 0 to enable this option.

// <q FF_USE_LABEL> Enable f_getlabel() and f_setlabel() function. <0-1>
#define FF_USE_LABEL  0
// <i> This option switches volume label functions, f_getlabel() and f_setlabel().
// <i> (0:Disable or 1:Enable)

// <q FF_USE_FORWARD> Enable f_forward() function. <0-1>
#define FF_USE_FORWARD  0
// <i> This option switches f_forward() function. (0:Disable or 1:Enable)

// <o FF_USE_STRFUNC> String function option. <0-2>
#define FF_USE_STRFUNC  0
// <i> FF_USE_STRFUNC switches string functions, f_gets(), f_putc(), f_puts() and
// <i> f_printf().
// <i>
// <i> 0: Disable. FF_PRINT_LLI, FF_PRINT_FLOAT and FF_STRF_ENCODE have no effect.
// <i> 1: Enable without LF-CRLF conversion.
// <i> 2: Enable with LF-CRLF conversion.

// <q FF_PRINT_LLI> Enable f_printf() support long long argument. <0-1>
// <i> FF_PRINT_LLI = 1 makes f_printf() support long long argument and FF_PRINT_FLOAT = 1/2
#define FF_PRINT_LLI  1

// <q FF_PRINT_FLOAT> Enable f_printf() support floating point argument. <0-1>
// <i> makes f_printf() support floating point argument. These features want C99 or later.
#define FF_PRINT_FLOAT  1

// <o FF_STRF_ENCODE> Character encoding type. <0-3>
// <i>
// <i> When FF_LFN_UNICODE >= 1 with LFN enabled, string functions convert the character
// <i> encoding in it. FF_STRF_ENCODE selects assumption of character encoding ON THE FILE
// <i> to be read/written via those functions.
// <i>
// <i>   0: ANSI/OEM in current CP
// <i>   1: Unicode in UTF-16LE
// <i>   2: Unicode in UTF-16BE
// <i>   3: Unicode in UTF-8
// <0=> ANSI/OEM
// <1=> UTF-16LE
// <2=> UTF-16BE
// <3=> UTF-8
#define FF_STRF_ENCODE  3


/*---------------------------------------------------------------------------/
/ Locale and Namespace Configurations
/---------------------------------------------------------------------------*/

// <o FF_CODE_PAGE> OEM code page to be used on the target system.
// <437=> U.S.
// <720=> Arabic
// <737=> Greek
// <771=> KBL
// <775=> Baltic
// <850=> Latin 1
// <852=> Latin 2
// <855=> Cyrillic
// <857=> Turkish
// <860=> Portuguese
// <861=> Icelandic
// <862=> Hebrew
// <863=> Canadian French
// <864=> Arabic
// <865=> Nordic
// <866=> Russian
// <869=> Greek 2
// <932=> Japanese (DBCS)
// <936=> Simplified Chinese (DBCS)
// <949=> Korean (DBCS)
// <950=> Traditional Chinese (DBCS)
// <0=> Include all code pages
// <i> This option specifies the OEM code page to be used on the target system.
// <i> Incorrect code page setting can cause a file open failure.
#define FF_CODE_PAGE  850
/* This option specifies the OEM code page to be used on the target system.
/  Incorrect code page setting can cause a file open failure.
/
/   437 - U.S.
/   720 - Arabic
/   737 - Greek
/   771 - KBL
/   775 - Baltic
/   850 - Latin 1
/   852 - Latin 2
/   855 - Cyrillic
/   857 - Turkish
/   860 - Portuguese
/   861 - Icelandic
/   862 - Hebrew
/   863 - Canadian French
/   864 - Arabic
/   865 - Nordic
/   866 - Russian
/   869 - Greek 2
/   932 - Japanese (DBCS)
/   936 - Simplified Chinese (DBCS)
/   949 - Korean (DBCS)
/   950 - Traditional Chinese (DBCS)
/     0 - Include all code pages above and configured by f_setcp()
*/

// <e FF_LFN_ENABLE> Support for long file name
#define FF_LFN_ENABLE  1
// <o FF_USE_LFN_OPT> LFN (long file name) option <0-3>
#define FF_USE_LFN_OPT 1
// <i> The FF_USE_LFN switches the support for LFN (long file name).
// <i>
// <i>   0: Disable LFN. FF_MAX_LFN has no effect.
// <i>   1: Enable LFN with static  working buffer on the BSS. Always NOT thread-safe.
// <i>   2: Enable LFN with dynamic working buffer on the STACK.
// <i>   3: Enable LFN with dynamic working buffer on the HEAP.
// <i>  To enable the LFN, ffunicode.c needs to be added to the project. The LFN function
// <i>  requiers certain internal working buffer occupies (FF_MAX_LFN + 1) * 2 bytes and
// <i>  additional (FF_MAX_LFN + 44) / 15 * 32 bytes when exFAT is enabled.
#if FF_LFN_ENABLE
#define FF_USE_LFN    FF_USE_LFN_OPT
#endif

// <o FF_MAX_LFN> Define size of the working buffer in UTF-16 code unit. <1-255>
#define FF_MAX_LFN    255
// <i>
// <i>  The FF_MAX_LFN defines size of the working buffer in UTF-16 code unit and it can
// <i>  be in range of 12 to 255. It is recommended to be set it 255 to fully support LFN
// <i>  specification.
// <i>  When use stack for the working buffer, take care on stack overflow. When use heap
// <i>  memory for the working buffer, memory management functions, ff_memalloc() and
// <i>  ff_memfree() exemplified in ffsystem.c, need to be added to the project.

// <o FF_LFN_UNICODE> Character encoding on the API when LFN is enabled <0-3>
#define FF_LFN_UNICODE  0
// <i> This option switches the character encoding on the API when LFN is enabled.
// <i>
// <i>   0: ANSI/OEM in current CP (TCHAR = char)
// <i>   1: Unicode in UTF-16 (TCHAR = WCHAR)
// <i>   2: Unicode in UTF-8 (TCHAR = char)
// <i>   3: Unicode in UTF-32 (TCHAR = DWORD)
// <i>
// <i>  Also behavior of string I/O functions will be affected by this option.
// <i>  When LFN is not enabled, this option has no effect.

// <o FF_LFN_BUF> Defines size of file name members in the FILINFO structure. <1-255>
#define FF_LFN_BUF    255

// <o FF_LFN_BUF> Defines size of file name members in the FILINFO structure. <1-255>
#define FF_SFN_BUF    12
// <i>  This set of options defines size of file name members in the FILINFO structure
// <i>  which is used to read out directory items. These values should be suffcient for
// <i>  the file names to read. The maximum possible length of the read file name depends
// <i>  on character encoding. When LFN is not enabled, these options have no effect. */
// </e>

// <q FF_FS_RPATH> Enable relative path. <0-2>
#define FF_FS_RPATH   0
// <i> This option configures support for relative path.
// <i>
// <i>   0: Disable relative path and remove related functions.
// <i>   1: Enable relative path. f_chdir() and f_chdrive() are available.
// <i>   2: f_getcwd() function is available in addition to 1.


/*---------------------------------------------------------------------------/
/ Drive/Volume Configurations
/---------------------------------------------------------------------------*/
// <o FF_VOLUMES> Number of volumes (logical drives) to be used. <1-10>
#define FF_VOLUMES    1
// <i> Number of volumes (logical drives) to be used. (1-10)

// <o FF_STR_VOLUME_ID> Volume ID.
#define FF_STR_VOLUME_ID  0

#define FF_VOLUME_STRS    "RAM","NAND","CF","SD","SD2","USB","USB2","USB3"
/* FF_STR_VOLUME_ID switches support for volume ID in arbitrary strings.
/  When FF_STR_VOLUME_ID is set to 1 or 2, arbitrary strings can be used as drive
/  number in the path name. FF_VOLUME_STRS defines the volume ID strings for each
/  logical drives. Number of items must not be less than FF_VOLUMES. Valid
/  characters for the volume ID strings are A-Z, a-z and 0-9, however, they are
/  compared in case-insensitive. If FF_STR_VOLUME_ID >= 1 and FF_VOLUME_STRS is
/  not defined, a user defined volume string table is needed as:
/
/  const char* VolumeStr[FF_VOLUMES] = {"ram","flash","sd","usb",...
*/

// <q FF_MULTI_PARTITION> Enable multiple volumes on the physical drive. <0-1>
#define FF_MULTI_PARTITION  0
// <i> This option switches support for multiple volumes on the physical drive.
// <i>  By default (0), each logical drive number is bound to the same physical drive
// <i>  number and only an FAT volume found on the physical drive will be mounted.
// <i>  When this function is enabled (1), each logical drive number can be bound to
// <i>  arbitrary physical drive and partition listed in the VolToPart[]. Also f_fdisk()
// <i>  function will be available.

// <o FF_MIN_SS> Minimum sector size.
#define FF_MIN_SS   512

// <o FF_MAX_SS> Maximum sector size.
#define FF_MAX_SS   512
// <i> This set of options configures the range of sector size to be supported. (512,
// <i>  1024, 2048 or 4096) Always set both 512 for most systems, generic memory card and
// <i>  harddisk, but a larger value may be required for on-board flash memory and some
// <i>  type of optical media. When FF_MAX_SS is larger than FF_MIN_SS, FatFs is configured
// <i>  for variable sector size mode and disk_ioctl() function needs to implement
// <i>  GET_SECTOR_SIZE command.

// <q FF_LBA64> Enable support for 64-bit LBA. <0-1>
#define FF_LBA64    0
// <i> This option switches support for 64-bit LBA. (0:Disable or 1:Enable)
// <i>  To enable the 64-bit LBA, also exFAT needs to be enabled. (FF_FS_EXFAT == 1)

// <o FF_MIN_GPT> Minimum number of sectors to switch GPT as partitioning format. <f.h>
#define FF_MIN_GPT    0x10000000
// <i> Minimum number of sectors to switch GPT as partitioning format in f_mkfs and
// <i>  f_fdisk function. 0x100000000 max. This option has no effect when FF_LBA64 == 0.

// <q FF_USE_TRIM> Enable support for ATA-TRIM. <0-1>
#define FF_USE_TRIM   0
// <i> This option switches support for ATA-TRIM. (0:Disable or 1:Enable)
// <i>  To enable Trim function, also CTRL_TRIM command should be implemented to the
// <i>  disk_ioctl() function.



/*---------------------------------------------------------------------------/
/ System Configurations
/---------------------------------------------------------------------------*/
// <q FF_FS_TINY> Enable tiny buffer configuration. <0-1>
#define FF_FS_TINY    0
// <i> This option switches tiny buffer configuration. (0:Normal or 1:Tiny)
// <i>  At the tiny configuration, size of file object (FIL) is shrinked FF_MAX_SS bytes.
// <i>  Instead of private sector buffer eliminated from the file object, common sector
// <i>  buffer in the filesystem object (FATFS) is used for the file data transfer.

// <q FF_FS_EXFAT> Enable support for exFAT filesystem. <0-1>
#define FF_FS_EXFAT   1
// <i> This option switches support for exFAT filesystem. (0:Disable or 1:Enable)
// <i>  To enable exFAT, also LFN needs to be enabled. (FF_USE_LFN >= 1)
// <i>  Note that enabling exFAT discards ANSI C (C89) compatibility.

// <e FF_FS_NORTC> Enable if the system does not have an RTC or valid timestamp
// <i> The option FF_FS_NORTC switches timestamp feature. If the system does not have
// <i> an RTC or valid timestamp is not needed, set FF_FS_NORTC = 1 to disable the
// <i> timestamp feature. Every object modified by FatFs will have a fixed timestamp
// <i> defined by FF_NORTC_MON, FF_NORTC_MDAY and FF_NORTC_YEAR in local time.
// <i> To enable timestamp function (FF_FS_NORTC = 0), get_fattime() function need to be
// <i> added to the project to read current time form real-time clock. FF_NORTC_MON,
// <i> FF_NORTC_MDAY and FF_NORTC_YEAR have no effect.
// <i> These options have no effect in read-only configuration (FF_FS_READONLY = 1).
#define FF_FS_NORTC   0
// <o FF_NORTC_MON> RTC month
#define FF_NORTC_MON  1
// <o FF_NORTC_MDAY> RTC day of month
#define FF_NORTC_MDAY 1
// <o FF_NORTC_YEAR> RTC Year
#define FF_NORTC_YEAR 2022
// </e>

// <o FF_FS_NOFSINFO_BIT0> Enable force a full FAT scan at the first time after volume mount.
#define FF_FS_NOFSINFO_BIT0  0
// <o FF_FS_NOFSINFO_BIT0> Enable the use of last allocated cluster number.
#define FF_FS_NOFSINFO_BIT1  0
#define FF_FS_NOFSINFO       (FF_FS_NOFSINFO_BIT0 | FF_FS_NOFSINFO_BIT1)
// <i> If you need to know correct free space on the FAT32 volume, set bit 0 of this
// <i>  option, and f_getfree() function at the first time after volume mount will force
// <i>  a full FAT scan. Bit 1 controls the use of last allocated cluster number.
// <i>
// <i>  bit0=0: Use free cluster count in the FSINFO if available.
// <i>  bit0=1: Do not trust free cluster count in the FSINFO.
// <i>  bit1=0: Use last allocated cluster number in the FSINFO if available.
// <i>  bit1=1: Do not trust last allocated cluster number in the FSINFO.

// <q FF_FS_LOCK> Enable file lock function to control duplicated file open. <0-1>
#define FF_FS_LOCK    0
// <i> The option FF_FS_LOCK switches file lock function to control duplicated file open
// <i>  and illegal operation to open objects. This option must be 0 when FF_FS_READONLY
// <i>  is 1.
// <i>
// <i>  0:  Disable file lock function. To avoid volume corruption, application program
// <i>      should avoid illegal open, remove and rename to the open objects.
// <i>  >0: Enable file lock function. The value defines how many files/sub-directories
// <i>      can be opened simultaneously under file lock control. Note that the file
// <i>      lock control is independent of re-entrancy.

// <q FF_FS_REENTRANT> Enable re-entrancy (thread safe) of the FatFs. <0-1>
#define FF_FS_REENTRANT 0
// <i> The option FF_FS_REENTRANT switches the re-entrancy (thread safe) of the FatFs
// <i>  module itself. Note that regardless of this option, file access to different
// <i>  volume is always re-entrant and volume control functions, f_mount(), f_mkfs()
// <i>  and f_fdisk() function, are always not re-entrant. Only file/directory access
// <i>  to the same volume is under control of this featuer.
// <i>
// <i>   0: Disable re-entrancy. FF_FS_TIMEOUT have no effect.
// <i>   1: Enable re-entrancy. Also user provided synchronization handlers,
// <i>      ff_mutex_create(), ff_mutex_delete(), ff_mutex_take() and ff_mutex_give()
// <i>      function, must be added to the project. Samples are available in ffsystem.c.
// <o FF_FS_TIMEOUT> Timeout period in unit of O/S time tick.
#define FF_FS_TIMEOUT 1000
// <i>  The FF_FS_TIMEOUT defines timeout period in unit of O/S time tick.

// </h>
// <<< end of configuration section >>>

#endif /* FFCONF_H_ */
