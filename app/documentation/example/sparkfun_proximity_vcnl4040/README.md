# VCNL4040 - Proximity Sensor (Sparkfun) #

## Summary ##

This project aims to implement a hardware driver interacting with the [VCNL4040 proximity sensor](https://www.vishay.com/ppg?84274) using Silicon Labs platform.

VCNL4040 integrates a proximity sensor (PS), ambient light sensor (ALS), and a high power IRED into one small package. It incorporates photodiodes, amplifiers, and analog to digital converting circuits into a single chip by CMOS process. The 16-bit high resolution ALS offers excellent sensing capabilities with sufficient selections to fulfill most applications whether dark or high transparency lens design. High and low interrupt thresholds can be programmed for both ALS and PS, allowing the component to use a minimal amount of the microcontroller's resources. With the ability to detect objects up to 20 cm, it is often used in automatic towel and soap dispensers or automatic faucets.

The patented Filtron technology achieves ambient light spectral sensitivity closest to real human eye response and offers the best background light cancellation capability (including sunlight) without utilizing the microcontrollers’ resources. This sensor is excellent if you need a digital light sensor for your next glitter cannon.

## Required Hardware ##

- 1x [BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit) BGM220 Bluetooth Module Explorer Kit
- Or 1x [Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917 (e.g. [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit) or [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board))
- 1x [SparkFun Proximity Sensor Breakout - 20cm, VCNL4040 (Qwiic)](https://www.sparkfun.com/products/15177)

## Hardware Connection ##

- If the BGM220P Explorer Kit is used:

  A VCNL4040 sensor board can be easily connected up with two I2C wires (SDA and SCL) along with 3v3 and GND. For the designated boards, SparkFun Qwiic compatible STEMMA QT connectors can be used.

    ![connection](image/connection.png)

- If the Wi-Fi Development Kit is used:

  | Description  | BRD4338A + BRD4002A | BRD2605A | SparkFun Proximity VCNL4040 |
  | -------------| ------------------- | ------------ | ------------------ |
  | I2C_SDA      | ULP_GPIO_6 [EXP_16] | Qwiic cable  | SDA                |
  | I2C_SCL      | ULP_GPIO_7 [EXP_15] | Qwiic cable  | SCL                |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter **"VCNL4040"**.

2. Click **Create** button on the **Third Party Hardware Drivers - VCNL4040 - Proximity Sensor (Sparkfun)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

    ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/sparkfun_proximity_vcnl4040/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

      **If the BGM220P Explorer Kit is used:**
        - [Services] → [Timers] → [Sleep Timer]
        - [Services] →  [IO Stream] → [IO Stream: USART] → instance name: **vcom**
        - [Application] →  [Utility] → [Log]
        - [Platform] → [Driver] → [I2C] → [I2CSPM] → default instance name: **qwiic**
        - [Third Party Hardware Drivers] -> [Sensors] -> [VCNL4040 - Proximity Sensor (Sparkfun)] → use default configuration

      **If the Wi-Fi Development Kit is used:**
        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [I2C] → [i2c2]
        - [Third Party Hardware Drivers] -> [Sensors] -> [VCNL4040 - Proximity Sensor (Sparkfun)] → use default configuration

4. Build and flash the project to your device

## How It Works ##

### API Overview ###

The driver is divided into two layers: platform and interface layer. The platform layer performs the interaction between the sensor and the microcontroller by I2C protocol (I2CSPM platform service). The interface layer provides users public APIs to communicate with or control the operation of the sensor.

[sparkfun_vcnl4040_platform.c](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/tree/master/driver/public/silabs/proximity_vcnl4040/src/sparkfun_vcnl4040_platform.c) : Implementation for communication between the microcontroller and sensor board through the Silabs I2CSPM platform service.

[sparkfun_vcnl4040.c](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/tree/master/driver/public/silabs/proximity_vcnl4040/src/sparkfun_vcnl4040.c) : Implements public APIs to interface with VCNL4040 - Sparkfun Proximity Sensor.

[sparkfun_vcnl4040_config.h](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/tree/master/driver/public/silabs/proximity_vcnl4040/config/sparkfun_vcnl4040_config.h) : Defines the configuration for interrupt operation mode.

### Testing ###

The below chart represents the workflow of a simple testing program. The left chart shows the initialization steps that are needed before reading data and the right chart shows the periodic measuring process.

![flowchart](image/flowchart.png)

Use Putty or another program to read the serial output.
The BGM220P uses by default a baudrate of 115200. You should expect a similar output to the one below.

![console](image/test.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
