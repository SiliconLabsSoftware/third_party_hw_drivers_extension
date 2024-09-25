# MAX6969 - UT-M 7-SEG R Click (Mikroe) #

## Summary ##

This project aims to show the hardware driver that is used to interface with the MAX6969 constant-current LED driver. UT-M 7-SEG R Click carries two SMD ultra-thin LED 7-SEG displays and the MAX6969 constant-current LED driver from Analog Devices. The click is designed to run on either 3.3V or 5V power supply. It communicates with the target microcontroller over SPI interface.

## Required Hardware ##

- [A BGM220 Explorer Kit board](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit)
- Or [SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-pk6031a-wifi-6-bluetooth-le-soc-pro-kit) (BRD4002 + BRD4338A)
- [UT-M 7-SEG R Click](https://www.mikroe.com/ut-m-7-seg-r-click)

## Hardware Connection ##

- **If the BGM220P Explorer Kit is used:**

    The UT-M 7-SEG R Click board supports MikroBus, so it can connect easily to EFR32xG24 Explorer Kit via MikroBus header. Assure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line. The hardware connection is shown in the image below:

    ![board](image/hardware_connection.png "Hardware connection")

- **If the SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit is used**:

    The hardware connection is shown in the table below:

    | Description  | BRD4338A GPIO | BRD4002 Breakout Pad | UT-M 7-SEG R Click board   |
    | -------------| ------------- | -------------------- | ---------------------------|
    | PWM_H        | GPIO_7        | P20                  | PWM                        |
    | RTE_GSPI_MASTER_CLK_PIN  | GPIO_25       | P25                  | SCK                 |
    | RTE_GSPI_MASTER_MISO_PIN | GPIO_26       | P27                  | SDO                 |
    | RTE_GSPI_MASTER_MOSI_PIN | GPIO_27       | P29                  | SDI                 |
    | RTE_GSPI_MASTER_CS0_PIN  | GPIO_28       | P31                  | LE                  |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by "max6969"

2. Click **Create** button on the **Third Party Hardware Drivers - MAX6969 - UT-M 7-SEG R Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

    ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_utm7segr_max6969/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

      **If the BGM220P Explorer Kit is used:**

       - [Services] → [IO Stream] → [IO Stream: EUSART] with the default instance name: **vcom**
       - [Services] → [Timers] → [Sleep Timer]
       - [Application] → [Utility] → [Log]
       - [Application] → [Utility] → [Assert]
       - [Third Party Hardware Drivers] → [Display & LED] → [MAX6969 - UT-M 7-SEG R Click (Mikroe)]

      **If the SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit is used:**

       - [Application] → [Utility] → [Assert]
       - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
       - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [PWM] → [channel_0] → use default configuration
       - [Third Party Hardware Drivers] → [Display & LED] → [MAX6969 - UT-M 7-SEG R Click (Mikroe)]

4. Build and flash this example to the board.

**Note:**

- Make sure that the **Third Party Hardware Drivers** extension is installed. If not, follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install "MAX6969 - UT-M 7-SEG R Click (Mikroe)" component.

## How It Works ##

This example demonstrates some of the available features of the 7-Segment LED module. After initialization, the application draws numbers from 0 to 99 on the screen.

![test](image/image_test.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
