# STN1110 - OBDII Click (Mikroe) #

## Summary ##

This example project shows an example for Mikroe OBDII Click board driver integration with Silicon Labs Platform.

The OBDII Click can be used for the communication with the Electronic Control Unit (ECU) of a vehicle, via several different OBD II diagnostic protocols such as CAN, K LINE, L LINE and J1850. The STN1110 IC is used to process requests sent by the MCU via the UART interface and return back the responses from the ECU network nodes.

## Required Hardware ##

- [**EFR32xG24-EK2703A** EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)

- Or [SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-pk6031a-wifi-6-bluetooth-le-soc-pro-kit) (BRD4002 + BRD4338A)

- [**OBDII Click** board](https://www.mikroe.com/obdii-click)

- [OBDII cable](https://www.mikroe.com/obd-ii-to-db9-cable)

## Hardware Connection ##

- If the EFR32xG24 Explorer Kit is used:

  The OBDII Click board supports MikroBus, so it can connect easily to the Explorer Kit via MikroBus header. Assure that the 45-degree corner of Click board matches the 45-degree white line of the Explorer Kit.

  The hardware connection is shown in the image below:

    ![board](image/hardware_connection.png)

- If the SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit is used:

  | Description       | BRD4338A GPIO            | BRD4002 Breakout Pad | OBDII Click |
  | ----------------- | ------------------------ | -------------------- | ----------- |
  | UART1_RX_PIN      | GPIO_6                   | P19                  | TX          |
  | UART1_TX_PIN      | GPIO_7                   | P20                  | RX          |
  | RESET             | GPIO_46                  | P24                  | RST         |
  | INTERRUPT         | GPIO_47                  | P27                  | INT         |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **obdii**.

2. Click **Create** button on the **Third Party Hardware Drivers - STN1110 - OBDII Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

    ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_obdii_stn1110/app.c` into the project root folder (overwriting existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

      **If the EFR32xG24 Explorer Kit is used:**

        - [Services] → [Timers] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: EUSART] → default instance name: **vcom**
        - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: **mikroe**
        - [Third Party Hardware Drivers] → [Interface] → [STN1110 - OBDII Click (Mikroe)] → use default configuration
        - [Application] → [Utility] → [Assert]
        - [Application] → [Utility] → [Log]

      **If the SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit is used:**

        - [Application] → [Utility] → [Assert]
        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
        - [Third Party Hardware Drivers] → [Interface] → [STN1110 - OBDII Click (Mikroe)] → use default configuration
        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [UART] → disable "UART1 DMA"

4. Build and flash this example to the board.

**Note :**

- Make sure that the **Third Party Hardware Drivers** extension is installed. If not, follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install "STN1110 - OBDII Click (Mikroe)" component.

## How It Works ##

### Driver Layer Diagram ###

![software layer](image/sw_layers.png)

### Testing ###

This example demonstrates the use of OBDII click board by reading the engine RPM and vehicle speed and displaying results on the USB UART once per second.

You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like TeraTerm to receive the data from the USB. A screenshot of the console output is shown in the figure below.

![logging_screen](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
