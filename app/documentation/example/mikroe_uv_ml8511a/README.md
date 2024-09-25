# ML8511A - UV Click (Mikroe) #

## Summary ##

This project aims to show the hardware driver that is used to interface with the UV Click driver from Mikroe using the Silicon Labs platform.

UV Click is a compact add-on board that alerts users of excessive ultraviolet radiation levels. This board features the ML8511A, an ultraviolet light sensor suitable for acquiring UV intensity indoors or outdoors from Rohm Semiconductor. The ML8511A is equipped with an internal amplifier converting photocurrent to voltage depending on the UV intensity working with a wavelength between 280-390nm. It outputs an analog voltage linearly related to the measured UV intensity (mW/cm2), which can be later processed in analog or digital form. Also, the power consumption can be reduced using the available power management mode. This UV Click board is suitable for various applications, such as determining exposure to ultraviolet radiation in a laboratory or environmental settings, weather stations, industrial manufacturing, and many more.

## Required Hardware ##

- [EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)

- Or [SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-pk6031a-wifi-6-bluetooth-le-soc-pro-kit?tab=overview) (BRD4002 + BRD4338A)

- [UV CLICK](https://www.mikroe.com/uv-click)

## Hardware Connection ##

- If the EFR32xG24 Explorer Kit is used:

  The UV Click board supports MikroBus, so it can connect easily to EFR32xG24 Explorer Kit. Be sure that the board's 45-degree corner matches the Explorer Kit's 45-degree white line.

  ![hardware_connection](image/hardware_connection.png)

- If the SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit is used:

  | Description   | BRD4338A GPIO  | BRD4002 Breakout Pad | UV Click Board |
  | ------------- | -------------- | -------------------- | ------------------- |
  | Analog Signal | ULP_GPIO_1     | P16                  | AN                  |
  | Enable        | GPIO_46        | P24                  | EN                  |
  | RTE_GSPI_MASTER_CLK_PIN  | GPIO_25       | P25                  | SCK                 |
  | RTE_GSPI_MASTER_MISO_PIN | GPIO_26       | P27                  | SDO                 |
  | RTE_GSPI_MASTER_CS0_PIN  | GPIO_28       | P31                  | CS                  |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add your device to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter **UV**.

2. Click **Create** button on the **Third Party Hardware Drivers - ML8511A - UV Click (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_uv_ml8511a/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

      **If the EFR32xG24 Explorer Kit is used:**

        - [Services] → [Timers] → [Sleep Timer]
        - [Services] → [IO Stream] → [IO Stream: EUSART]** → default instance name: vcom
        - [Application] → [Utility] → [Log]
        - [Platform] → [Driver] → [SPI] → [SPIDRV] → default instance name: **mikroe**
        - [Third Party Hardware Drivers] → [Sensors] → [ML8511A - UV Click (Mikroe)]

      **If the SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit is used:**

        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
        - [Third Party Hardware Drivers] → [Sensors] → [ML8511A - UV Click (Mikroe)]
        - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [ADC] default instance name: channel_1

4. Install printf float

    - Open Properties of the project.
    - Select C/C++ Build > Settings > Tool Settings >GNU ARM C Linker > General. Check Printf float.
        ![float](image/float.png)

5. Build and flash this example to the board.

**Note:**

- Make sure that the **Third Party Hardware Drivers** extension is installed. If not, follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- Third-party Drivers Extension must be enabled for the project to install "ML8511A - UV Click (Mikroe)" component.

## How It Works ##

### Testing ###

After setting up all the required components, flash the code to the Explorer Kit, the application will start measuring the ultraviolet radiation value and evaluate the harm of the surrounding environment every 1 second. You can visualize the changes of these values on the Console Window.

![result](image/result.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
