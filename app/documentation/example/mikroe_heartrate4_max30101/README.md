# MAX30101 - Heart Rate 4 Click (Mikroe) #

## Summary ##

This project aims to show the hardware driver that is used to interface with the heart rate sensor with the Silicon Labs Platform.

Heart rate 4 click carries the MAX30101 high-sensitivity pulse oximeter and heart-rate sensor from Analog Devices. The MAX30101 is a pulse oximeter and heart rate sensor that uses a combination of red and infrared LEDs and a photodetector to measure the oxygen saturation of blood and the heart rate of a person. The sensor is non-invasive, and it can be placed on the fingertip, earlobe, or other parts of the body to measure the heart rate and oxygen saturation. The sensor provides a high level of accuracy, and it can be used in a wide range of applications such as medical monitoring, sports and fitness tracking, and sleep analysis. The MAX30101 sensor communicates with other devices using I2C interface and it is also low power consumption making it suitable for portable devices and wearables.

## Required Hardware ##

- [EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview).

- Or [SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-pk6031a-wifi-6-bluetooth-le-soc-pro-kit) (BRD4002 + BRD4338A)

- [Heart Rate 4 Click](https://www.mikroe.com/heart-rate-4-click).

## Hardware Connection ##

- If the Explorer Kit is used:

  The Heart Rate 4 Click board supports MikroBus, so it can connect easily to the header of the Explorer Kit. During the assembly process, make sure that the 45-degree corner of the Click board matches the 45-degree white line of the Silicon Labs Explorer Kit.

  The hardware connection is shown in the image below:

  ![board](image/hardware_connection.png "Hardware connection")

- If the SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit is used:

  | Description | BRD4338A GPIO  | BRD4002 EXP Header | Heart Rate 4 Click |
  | ----------- | -------------  | ------------------ | ------------------ |
  | INT         | GPIO_46        | P24                | INT                |
  | SDA         | ULP_GPIO_6     | EXP_16             | SDA                |
  | SCL         | ULP_GPIO_7     | EXP_15             | SCL                |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by *max30101*

2. Click **Create** button on the **Third Party Hardware Drivers - MAX30101 - Heart Rate 4 Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_heartrate4_max30101/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

      **If the Explorer Kit is used:**

        - [Platform] → [Driver] → [GPIOINT]
        - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: vcom
        - [Services] → [Timers] → [Sleep Timer]
        - [Application] → [Utility] → [Log]
        - [Third Party Hardware Drivers] → [Sensors] → [MAX30101 - Heart Rate 4 Click (Mikroe)] -> use default configuration.
        ![config](image/default_config.png)

      **If the SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit is used:**
        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [I2C] → [i2c2]
        - [Third Party Hardware Drivers] → [Sensors] → [MAX30101 - Heart Rate 4 Click (Mikroe)] -> use default configuration.

4. Build and flash this example to the board.

**Note:**

- Make sure that the **Third Party Hardware Drivers** extension is installed. If not, follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- Third-party Drivers Extension extension must be enabled for the project to install "MAX30101 - Heart Rate 4 Click (Mikroe)" component.

## How It Works ##

This is an example that demonstrates the use of the Heart Rate 4 Click board. After resetting, the application reads data from the Red diode and displays the results on USB UART if the measured data is above defined threshold.

You can choose the mode of operation by selecting the corresponding macro MIKROE_HEARTRATE4_MODE_INTERRUPT or MIKROE_HEARTRATE4_MODE_POLLING.

You can launch Console that's integrated into Simplicity Studio or use a third-party terminal tool like TeraTerm to receive the data from the USB. A screenshot of the console output and an actual test image are shown in the figure below.

![logging_screen](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
