# External Storage - microSD Click (Mikroe) #

## Summary ##

This project aims to implement a hardware driver interacting with an SD Card using the microSD Click board. SD Cards support SDIO or SPI interfaces to communicate; this driver communicates via the SPI interface as SPI is widely available in embedded devices/micro-controllers.

This project integrates FatFS which is one of the most popular file system libraries. File system related abstraction is handled by FatFS, and lower-level functions like the media IO layer are provided by this project's source code. For more information about FatFS, see the following [source](http://elm-chan.org/fsw/ff/doc/appnote.html).

The microSD Click communicates with the target microcontroller over SPI interface that corresponds to the pinout on the mikroBUS™ socket as shown below.

![mikroBus](image/mikrobus.png)

## Required Hardware ##

- 1x [BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit) BGM220 Bluetooth Module Explorer Kit

- Or 1x [Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917 (e.g. [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit) or [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board))

- 1x [microSD Click board](https://www.mikroe.com/microsd-click)

- 1x microSD card, e.g. microSD card 32 GB

## Connections Required ##

- If the BGM220P Explorer Kit is used:

  The microSD Click board can be easily clicked into its place. During the assembly, be sure that the 45-degree corner of the board fits the 45-degree white line of the Explorer Kit.

  ![board](image/board.png)

- If the Wi-Fi Development Kit is used:

> [!IMPORTANT]
> The MISO pin is driven by a tri-stated output from the SDcard. Therefore, a pull-up resistor (e.g., 100k) should be placed on the MISO pin to hold that input into a known state when the SDcard is not selected.

  | Description  | BRD4338A + BRD4002A | BRD2605A     | microSD Click Board |
  | -------------| ------------- | ------------------ | ------------------- |
  | Card Detection           | GPIO_46 [P24] | GPIO_10        | CD                  |
  | RTE_GSPI_MASTER_CLK_PIN  | GPIO_25 [P25] | GPIO_25        | SCK                 |
  | RTE_GSPI_MASTER_MISO_PIN | GPIO_26 [P27] | GPIO_26        | SDO                 |
  | RTE_GSPI_MASTER_MOSI_PIN | GPIO_27 [P29] | GPIO_27        | SDI                 |
  | RTE_GSPI_MASTER_CS0_PIN  | GPIO_28 [P31] | GPIO_28        | CS                  |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **microsd**.

2. Click **Create** button on the **Third Party Hardware Drivers - microSD Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_microsd/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.
    - Select the SOFTWARE COMPONENTS tab.
    - Install the following components:

        **If the BGM220P Explorer Kit is used:**

        - **[Third Party Hardware Drivers] → [Storage] → [microSD - microSD Click (Mikroe)]**
        - **[Third Party Hardware Drivers] → [Storage] → [FatFS - Generic FAT Filesystem]**
        - **[Services] → [IO Stream] → [IO Stream: EUSART]** → with the default instance name: **vcom**
        - **[Application] → [Utility] → [Log]**
        - **[Application] → [Utility] → [Assert]**

        **If the Wi-Fi Development Kit is used:**
        - **[Application] → [Utility] → [Assert]**
        - **[Third Party Hardware Drivers] → [Storage] → [microSD - microSD Click (Mikroe)]**
        - **[Third Party Hardware Drivers] → [Storage] → [FatFS - Generic FAT Filesystem]** -> turn on *Enable if the system does not have an RTC or valid timestamp*
        ![fatfs_config](image/fatfs_config.png)

4. Build and flash the project to your device.

## How It Works ##

### API Overview ###

The driver is divided into a FatFs, a Media access interface, and an SPI platform. The FatFs layer is the Filesystem Layer independent of platforms and storage media, it is completely separated from the physical device, such as memory card, hard disk, or any type of storage device. The media access interface is not part of the FatFs module and it needs to be provided by the implementer. FatFs module controls the storage devices via a media access interface. The SPI platform layer provides integration to the host microcontroller hardware-dependent code.

![system_oveview](image/system_overview.png)

The dependency diagram is shown below.

![structure_overview](image/structure_overview.png)

**FatFS**: provides various filesystem functions for the applications.

- File Access
- Directory Access
- File and Directory Management
- Volume Management and System Configuration

For more information about the APIs of the FatFS module, see the following [source](http://elm-chan.org/fsw/ff/00index_e.html).

**Storage Device Controls Generic Media**: implements the generic media access interface by consuming device-specific storage device control implementations.
**Storage Device Controls SD Card**: implements the storage device control interfaces specific to SD Card.

**Storage Device Controls SD Card Platform**: implements basic interfaces required by the upper Storage Device Control layer. It implements SPI **byte, multibyte, exchange**, and other required functions by consuming platform SPI interfaces. The SPI instance is configurable via a public interface. SPI instance should be provided by passing as a parameter of the sd_card_spi_init() function.

**Silicon Labs Platform**: implements the peripheral driver core.

### Testing ###

This example demonstrates the basic features of the driver as shown below:

![Flow](image/workflow.png)

Use a terminal program, such as the Console that is integrated into Simplicity Studio or a third-party tool terminal like Tera Term to receive the logs from the virtual COM port. Make sure that A pull-up resistor (e.g., 100k) is placed on the MISO. You should expect a similar output to the one below.

![testing_format](image/testing.png)

Unmount the SD Card. Use a micro Card Reader to read files from the SD Card on your computer. You should expect a similar output to the one below.

![content_file](image/content_file.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
