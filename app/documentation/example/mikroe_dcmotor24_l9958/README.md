# L9958 - DC Motor 24 Click (Mikroe) #

## Summary ##

This project aims to show the hardware driver that is used to interface with the DC motor driver using the L9958 with the Silicon Labs Platform.

DC Motor 24 Click is a compact add-on board with a brushed DC motor driver. This board features the L9958, an SPI-controlled H-bridge from STMicroelectronics. The L9958 is rated for an operating voltage range from 4V to 28V, with direct PWM motor control and current regulation threshold set by the SPI interface from 2.5A to 8.6A. It also has complete diagnostic and protection capabilities supporting robust and reliable operation. This Click board™ is suitable for controlling DC and stepper motors in safety-critical automotive applications and under extreme environmental conditions.

## Required Hardware ##

- 1x [BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit) BGM220 Bluetooth Module Explorer Kit

- Or 1x [Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917 (e.g. [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit) or [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board))

- 1x [DC Motor 24 Click](https://www.mikroe.com/dc-motor-24-click)

## Hardware Connection ##

- If the BGM220 Explorer Kit is used:

  The DC Motor 24 Click board supports MikroBus, so it can connect easily to the Explorer Kit via MikroBus header. Assure that the 45-degree corner of the Click board matches the 45-degree white line of the Explorer Kit.

  The hardware connection is shown in the image below:

  ![board](image/hardware_connection.png)

- If the Wi-Fi Development Kit is used:

  | Description  | BRD4338A + BRD4002A | BRD2605A     | DC Motor 24 Click |
  | ------------------------ | ------------- | -------------- | --------- |
  | DIR                      | GPIO_47 [P26] | GPIO_11        | DIR       |
  | EN                       | GPIO_46 [P24] | GPIO_10        | EN        |
  | PWM                      | GPIO_7 [P20]  | GPIO_7         | PWM       |
  | RTE_GSPI_MASTER_CLK_PIN  | GPIO_25 [P25] | GPIO_25        | SCK       |
  | RTE_GSPI_MASTER_MISO_PIN | GPIO_26 [P27] | GPIO_26        | SDO       |
  | RTE_GSPI_MASTER_MOSI_PIN | GPIO_27 [P29] | GPIO_27        | SDI       |
  | RTE_GSPI_MASTER_CS0_PIN  | GPIO_28 [P31] | GPIO_28        | CS        |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by 'l9958'.

2. Click **Create** button on the **Third Party Hardware Drivers - L9958 - DC Motor 24 Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "BGM220 Explorer Kit" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_dcmotor24_l9958/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

    **If the BGM220P Explorer Kit is used:**

      - [Services] → [IO Stream] → [IO Stream: EUSART] → default instance name: vcom
      - [Application] → [Utility] → [Log]
      - [Services] → [Timers] → [Sleep Timer]
      - [Third Party Hardware Drivers] → [Motor Control] → [L9958 - DC MOTOR 24 Click (Mikroe)]

    **If the Wi-Fi Development Kit is used:**

      - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
      - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [PWM] → [channel_0] → use default configuration
      - [Third Party Hardware Drivers] → [Motor Control] → [L9958 - DC MOTOR 24 Click (Mikroe)]

4. Build and flash this example to the board.

**Note:**

- Make sure that the **Third Party Hardware Drivers** extension is installed. If not, follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- **Third Party Hardware Drivers** extension must be enabled for the project to install the "L9958 - DC MOTOR 24 Click (Mikroe)" component.

## How It Works ##

This example demonstrates the use of the DC Motor 24 Click board. DC Motor 24 Click communicates with the BGM220 via SPI interface.
It shows moving in the left direction from slow to fast speed and from fast to slow speed then switching the direction.
The speed and the direction of the motor are shown in the USART Terminal where you can track these changes.

You can launch Console which is integrated into Simplicity Studio or use a third-party terminal tool like Putty to receive the data from the USB. A screenshot of the console output and an actual test image are shown in the figure below.

![logging_screen](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
