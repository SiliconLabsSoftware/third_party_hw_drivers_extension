# AD8318 - RF Meter Click (Mikroe) #

## Summary ##

This example project shows an example for Mikroe RF Meter Click board driver integration with the Silicon Labs Platform.

RF Meter Click is a compact add-on board that measures radio frequency power. It covers a frequency span from 1MHz up to 8GHz over a 60dB range, approximately. RF Meter Click is based on the AD8318, a logarithmic detector/controller from Analog Devices. It is a demodulating logarithmic amplifier capable of accurately converting an RF input signal to a corresponding decibel-scaled output voltage. The voltage output of the AD8318 goes to the MCP3201, a successive approximation 12-bit analog-to-digital converter with an onboard sample and holds circuitry from Microchip. The RF Meter uses a 3-wire SPI serial interface of the MCP3201 to communicate to the host MCU over the mikroBUS™ socket.

This example can be used for testing equipment that includes RF transmitters or relays. It enables monitoring and measuring frequency levels to ensure proper functioning and leak detection.

## Required Hardware ##

- [EFR32xG24-EK2703A EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)
- Or [SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-pk6031a-wifi-6-bluetooth-le-soc-pro-kit) (BRD4002 + BRD4338A)
- [Mikroe RF Meter Click board based on AD8318](https://www.mikroe.com/rf-meter-click-click)

## Hardware Connection ##

- **If EFR32xG24 Explorer Kit is used:**

  The RF Meter Click board supports MikroBus; therefore, it can easily connect to the MikroBus socket of the EFR32xG24 Explorer Kit. Be sure that the 45-degree corner of the board matches the 45-degree white line of the Explorer Kit. The hardware connection is shown in the image below:

  ![hardware_connection](image/hardware_connection.png)

- **If the SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit is used**:

  The hardware connection is shown in the table below:

  | Description  | BRD4338A GPIO | BRD4002 Breakout Pad | The RF Meter Click board   |
  | -------------| ------------- | -------------------- | ---------------------------|
  | RTE_GSPI_MASTER_CLK_PIN  | GPIO_25       | P25                  | SCK                 |
  | RTE_GSPI_MASTER_MISO_PIN | GPIO_26       | P27                  | SDO                 |
  | RTE_GSPI_MASTER_CS0_PIN  | GPIO_28       | P31                  | CS                  |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by "ad8318".

2. Click **Create** button on the **Third Party Hardware Drivers - AD8318 - RF Meter Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

    ![create_project](image/create_example.png)

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_rf_meter_ad8318/app.c` into the project root folder (overwriting existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        **If EFR32xG24 Explorer Kit is used:**

        - [Services] → [Timers] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: EUSART] → default instance name: **vcom**
        - [Application] → [Utility] → [Log]
        - [Third Party Hardware Drivers] → [Sensors] → [AD8318 - RF Meter Click (Mikroe)]

        **If the SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit is used**:

        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
        - [Third Party Hardware Drivers] → [Sensors] → [AD8318 - RF Meter Click (Mikroe)]

4. Install printf float

    - Open Properties of the Project.
    - Select C/C++ Build → Settings → Tool Settings → GNU ARM C Linker → General. Check Printf float.

5. Build and flash this example to the board.

**Note :**

- Make sure that the **Third Party Hardware Drivers** extension has already be installed. If not, follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install the " AD8318 - RF Meter Click (Mikroe)" component.

## How It Works ##

### Driver Layer Diagram ###

![software_layer](image/software_layer.png)

### Testing ###

This example uses for measuring radio frequency power, and convert it to decibels per milliwatt value (dBm). This value is used to define signal strength in wires and cables at radio frequencies.

You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like TeraTerm to receive the data from the USB. Note that your board uses the default baud rate of 115200. A screenshot of the console output is shown in the figure below.

![console_log](image/console_log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
