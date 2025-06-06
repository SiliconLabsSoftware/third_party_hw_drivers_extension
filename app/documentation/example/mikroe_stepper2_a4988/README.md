# A4988 - Stepper 2 Click (Mikroe) #

## Summary ##

This project shows the driver implementation of the Stepper 2 Click module, which implements the A4988 IC with the Silicon Labs Platform.

Stepper 2 click is a complete solution for driving bipolar stepper motors with full/half and micro-steps. It features the A4988 IC from Allegro Microsystems with DMOS low RDSON drivers on its outputs, which ensure high efficiency and reliable operation of the internal H-Bridges. This IC has the integrated translator section, used to simplify the control: using simple step control inputs from the host MCU, the stepper motor can be driven in both directions, with the predetermined step sizes. In addition, the output current is regulated allowing for noiseless operation of the stepper motor with no resonance and ringing typically observed at unregulated stepper driver designs.

## Required Hardware ##

- 1x [BGM220-EK4314A](https://www.silabs.com/development-tools/wireless/bluetooth/bgm220-explorer-kit) BGM220 Bluetooth Module Explorer Kit

- Or 1x [Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917 (e.g. [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit) or [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board))

- 1x [Stepper 2 Click](https://www.mikroe.com/stepper-2-click)

## Hardware Connection ##

- If the EFR32xG24 Explorer Kit is used:

The Stepper 2 Click Board™ supports MikroBus, so it can connect easily to BGM220P Explorer Kit's MikroBus header. Be sure that the 45-degree corner of the Click Board™ matches the 45-degree white line of the Silicon Labs Explorer Kit. In this example, we use a 28BYJ-48 12V stepper motor, so the +12V power supply and GND are connected to the Stepper 2 Click Board™. Also, four control signals for the stepper motor are connected to pins 1A, 1B, 2A, and 2B respectively. The hardware connection is shown in the image below:

![board](image/hardware_connection.png)

- If the Wi-Fi Development Kit is used:

  | Description  | BRD4338A + BRD4002A | BRD2605A     | DC Motor 26 Click |
  | ------------ | ------------- | ------------------ | ----------------- |
  | Chip Enable    | GPIO_50 [P32] | GPIO_7      | EN        |
  | Reset          | GPIO_49 [P30] | GPIO_6      | RST       |
  | Sleep          | GPIO_46 [P24] | GPIO_10     | SLP(CS)   |
  | Direction      | GPIO_47 [P26] | GPIO_11     | INT       |
  | Step trigger   | GPIO_48 [P28] | GPIO_12     | PWM       |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with the filter "stepper2".

2. Click **Create** button on the **Third Party Hardware Drivers - A4988 - Stepper 2 Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.
![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_stepper2_a4988/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

      **If the Explorer Kit is used:**
        - [Services] → [Timer] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: vcom
        - [Application] → [Utility] → [Log]
        - [Platform] → [Driver] → [Button] → [Simple Button] → default instance name: btn0
        - [Third-Party Hardware Drivers] → [Motor Control] → [A4988 - Stepper 2 Click (Mikroe)]

      **If the Wi-Fi Development Kit is used:**
        - [Device] → [Si91x] → [MCU] → [Hardware] → [Button] → default instance name: btn0
        - [Third-Party Hardware Drivers] → [Motor Control] → [A4988 - Stepper 2 Click (Mikroe)]

4. Build and flash this example to the board.

## How It Works ##

Use Putty/Tera Term or other programs to read the serial output. The BGM220P uses by default a baudrate of 115200. You should expect a similar output to the one below. After the main program is executed, the stepper motor will start to rotate. When the user press button0, it will change the active state.

![logging_screen](image/log.png)
![pwm_signal](image/pwm.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
