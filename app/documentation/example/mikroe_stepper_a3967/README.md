# A3967 - Stepper Click (Mikroe) #

## Summary ##

This project shows the driver implementation of the Stepper Click module, which implements the A3967 IC with the Silicon Labs Platform.

Stepper click is a complete solution for driving bipolar stepper motors with full/half and micro-steps. It features the A3967 IC from Allegro Microsystems with proprietary Satlington™ sink drivers on its outputs, which ensure high efficiency and reliable operation of the internal H-Bridges. This IC has the integrated translation section, used to simplify the control: using simple step control inputs from the host MCU, the stepper motor can be driven in both directions, with the predetermined step sizes. In addition, the output current is regulated allowing for noiseless operation of the stepper motor, with no resonance and ringing typically observed at unregulated stepper driver designs.

## Required Hardware ##

- 1x [XG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit) EFR32xG24 Explorer Kit

- Or 1x [Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917 (e.g. [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit) or [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board))

- 1x [Stepper Click](https://www.mikroe.com/stepper-click)

- 1x [Step Motor 5v](https://www.mikroe.com/step-motor-5v)

## Hardware Connection ##

- If the EFR32xG24 Explorer Kit is used:

  The Stepper Click board supports MikroBus. Therefore, it can connect easily to the MikroBus header of the Explorer Kit. Be sure that the 45-degree corner of the board matches the 45-degree white line of the Explorer Kit. In this example, we use a 28BYJ-48 5V stepper motor, so the +5V power supply and GND are connected to the Stepper Click Board™. Also, four control signals for the stepper motor are connected to pins 1A, 1B, 2A, and 2B respectively.

  ![board](image/hardware_connection.png)

- If the Wi-Fi Development Kit is used:

  | Description  | BRD4338A + BRD4002A | BRD2605A | Stepper Click  |
  | -------------| ------------- | -------------- | -------------- |
  | Step size bit 1 | GPIO_48 [P28] | GPIO_12 [P25]     | MS1            |
  | Step size bit 2 | GPIO_49 [P30] | GPIO_6 [P21]      | MS2            |
  | Direction       | GPIO_47 [P26] | GPIO_11 [P22]     | DIR            |
  | Step trigger    | GPIO_46 [P24] | GPIO_10 [P23]     | STP            |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by "stepper".

2. Click **Create** button on the **Third Party Hardware Drivers - A3967 - Stepper Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

    ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_stepper_a3967/app.c` into the project root folder (overwriting existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

      **If the EFR32xG24 Explorer Kit is used:**
        - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: vcom
        - [Application] → [Utility] → [Log]
        - [Third-Party Hardware Drivers] → [Motor Control] → [A3967 - Stepper Click (Mikroe)]

      **If the Wi-Fi Development Kit is used:**
        - [Third-Party Hardware Drivers] → [Motor Control] → [A3967 - Stepper Click (Mikroe)]

4. Build and flash this example to the board.

**Note:**

- Make sure that the **Third Party Hardware Drivers** extension is installed. If not, follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- **Third Party Hardware Drivers** extension must be enabled for the project to install **"A3967 - Stepper Click (Mikroe)"** component.

## How It Works ##

According to the data sheet, when the 28BYJ-48 motor is operated in full-step mode, each step corresponds to a rotation of 5.625°. This means there are 64 steps per revolution (360°/5.625° = 64).

In addition, the motor features a 1/64 reduction gear set. This means that there are in fact 4096 steps (64*64 steps per revolution).

After the main program is executed, the stepper motor will start to run like the GIF below:

![test](image/test.gif)

You also can check the output of PWM and it looks like the picture below:

![pwm_signal](image/pwm.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
