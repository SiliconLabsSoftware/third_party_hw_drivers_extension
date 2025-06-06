# CAP1166 - Cap Touch 2 Click (Mikroe) #

## Summary ##

This project shows the driver implementation of an SPI capacitive touch sensor using the CAP1166 sensor with Silicon Labs Platform.

Cap Touch 2 click is a capacitive touch sensing Click board™ which features the CAP1166 chip, which is an advanced touch/proximity sensor IC. It packs a range of different touch/proximity functionalities allowing the development of robust and aesthetically pleasing touch interfaces. It can be used for desktop computer or notebook control, LCD/TFT monitors, various types of consumer electronics, home appliances, and other applications that can benefit of having a reliable and feature-rich touch activated user interface.

## Required Hardware ##

- 1x [BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit) BGM220 Bluetooth Module Explorer Kit

- Or 1x [Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917 (e.g. [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit) or [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board))

- 1x [Cap Touch 2 Click board](https://www.mikroe.com/cap-touch-2-click) based on CAP1166

## Hardware Connection ##

- **If the BGM220 Explorer Kit is used**:

  The Cap Touch 2 Click board supports MikroBus, so it can connect easily to the Explorer Kit via MikroBus header. Assure that the 45-degree corner of the Click board matches the 45-degree white line of the Explorer Kit.

  The hardware connection is shown in the image below:

  ![board](image/hardware_connection.png)

- **If the Wi-Fi Development Kit** is used:

  | Description  | BRD4338A + BRD4002A | BRD2605A    | Cap Touch 2 Click Board |
  | ------------ | -------------- | ---------------- | ----------------------- |
  | Alert/Interrupt          | GPIO_46 [P24]  | GPIO_10    | ALT               |
  | Device Reset             | GPIO_47 [P26]  | GPIO_11    | RST               |
  | RTE_GSPI_MASTER_CLK_PIN  | GPIO_25 [P25]  | GPIO_25    | SCK               |
  | RTE_GSPI_MASTER_MISO_PIN | GPIO_26 [P27]  | GPIO_26    | SDO               |
  | RTE_GSPI_MASTER_MOSI_PIN | GPIO_27 [P29]  | GPIO_27    | SDI               |
  | RTE_GSPI_MASTER_CS0_PIN  | GPIO_28 [P31]  | GPIO_28    | CS                |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **cap1166**.

2. Click **Create** button on the **Third Party Hardware Drivers - CAP1166 - Cap Touch 2 Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

    ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_captouch2_cap1166/app.c` into the project root folder (overwriting existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

      **If the BGM220 Explorer Kit is used**:

        - [Services] → [Timers] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: EUSART] → default instance name: vcom
        - [Application] → [Utility] → [Assert]
        - [Application] → [Utility] → [log]
        - [Platform] → [Driver] → [SPI] → [SPIDRV] → default instance name: **mikroe**
        - [Third Party Hardware Drivers] → [Human Machine Interface] → [CAP1166 - Capacitive Touch 2 Click (Mikroe)]

      **If the Wi-Fi Development Kit is used:**
        - [Application] → [Utility] → [Assert]
        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
        - [Third Party Hardware Drivers] → [Human Machine Interface] → [CAP1166 - Capacitive Touch 2 Click (Mikroe)]

4. Build and flash this example to the board.

## How It Works ##

The example checks touch detection (is interrupt occured) and shows a message on the logging screen on which input touch is detected or on which input touch is released. Also turns on LED on which linked input interrupt occured.

You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like TeraTerm to receive the data from the USB. A screenshot of the console output is shown in the figure below.

![logging_screen](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
