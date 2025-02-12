# TSD-10 - Turbidity Click (Mikroe) #

## Summary ##

This project shows the implementation of a Turbidity driver using Turbidity Click and TSD10 sensor with the Silicon Labs Platform.

Turbidity Click Bundle: This bundle gives you everything you need to take high-accuracy turbidity measurements. It allows users to combine Turbidity Sensor with Turbidity Click via an additional 3-wire Male to Male Cable - 15 cm to measure the relative clarity of any liquid. Turbidity Click allows users to upgrade their projects with a sensor that senses the cloudiness or haziness of a fluid caused by large numbers of individual particles invisible to the naked eye.

The sensor can be used in various domains. For example, with the washing machines application, depending on the sensor value, the washing controller can determine the amount of soil in the water and make decisions on how long to wash in all cycles. By measuring the turbidity of the wash water, the washing machine can conserve energy on lightly soiled loads by only washing as long as necessary. This will result in energy savings for the consumer.

## Required Hardware ##

- 1x [XG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit) EFR32xG24 Explorer Kit

- Or 1x [Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917 (e.g. [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit) or [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board))

- 1x [Turbidity Click board](https://www.mikroe.com/turbidity-click) carries MCP3221 Low-Power 12-Bit A/D Converter

- 1x [Turbidity Sensor - TSD10](https://www.mikroe.com/tsd-10-turbidity-sensor)

- 1x [3-wire Male to Male Cable - 15 cm](https://www.mikroe.com/3-wire-male-to-male-cable-15-cm)

## Hardware Connection ##

- If the EFR32xG24 Explorer Kit is used:

  The Turbidity Click board supports MikroBus, so it can connect easily to EFR32xG24 Explorer Kit via MikroBus header. Assure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line.
  The Turbidity Click features one 1x3 2.5mm connector suitable for connecting a Turbidity sensor via an additional 3-wire Male to Male Cable - 15 cm.

  ![hardware_connection](image/hardware_connection.png "BRD2703A xG24 Explorer Kit Board and Turbidity Click Board")

  The hardware connection will look like the figure below:

  ![real_hardware_connection](image/hardware_connection1.png)

- If the Wi-Fi Development Kit is used:

  | Description  | BRD4338A + BRD4002A | BRD2605A | Turbidity Click  |
  | -------------| ------------- | ------------------ | ---------------- |
  | I2C_SDA      | ULP_GPIO_6 [EXP_16]  | ULP_GPIO_6  | SDA              |
  | I2C_SCL      | ULP_GPIO_7 [EXP_15]  | ULP_GPIO_7  | SCL              |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with the filter 'turbidity'.

2. Click **Create** button on the **Third Party Hardware Drivers - TSD-10 - Turbidity Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_turbidity_tsd10/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

      **If the EFR32xG24 Explorer Kit is used:**
        - [Services] → [Timers] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: vcom
        - [Application] → [Utility] → [Log]
        - [Third-Party Hardware Drivers] → [Sensor] → [TSD-10 - Turbidity Click (Mikroe)]

      **If the Wi-Fi Development Kit is used:**
        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [I2C] → [i2c2]
        - [Third Party Hardware Drivers] → [Sensors] → [TSD-10 - Turbidity Click (Mikroe)]

4. Enable **Printf float**

   - Open Properties of the project.
   - Select C/C++ Build → Settings → Tool Settings → GNU ARM C Linker → General → Check **Printf float**.

      ![float](image/float.png)

5. Build and flash this example to the board.

**Note:**

- Make sure that the **Third Party Hardware Drivers** extension is installed. If not, follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- **Third Party Hardware Drivers** extension must be enabled for the project to install the "TSD-10 - Turbidity Click (Mikroe)" component.

## How It Works ##

### Driver Layer Diagram ###

![layer](image/driver_layer.png)

### Testing ###

The application monitors and displays Nephelometric Turbidity Units (NTU). Results are being sent to the USART Terminal.

- The Turbidity sensor measures the amount of transmitted light to determine the turbidity of water. Program will read the NTU value. NTU value in a range of 0 to 4000 corresponds with the turbidity level of water (for example, 0 NTU means water is clean, and NTU is 3000 which means water is cloudy).

- You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like Tera Term to receive the data from the USB. A screenshot of the console output is shown in the figure below:

  ![console_log](image/log.png "log")

- Looking at the figure above, we can know the NTU value and determine the amount of soil in the water, depending on that, the controller can take some actions.

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_Drivers_extension](https://github.com/SiliconLabs/third_party_hw_Drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_Drivers_extension](https://github.com/SiliconLabs/third_party_hw_Drivers_extension) repo.
