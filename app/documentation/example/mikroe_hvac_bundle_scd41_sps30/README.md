# SCD41 & SPS30 - HVAC Click Bundle (Mikroe) #

## Summary ##

This project aims to show the hardware driver that is used to interface with the HVAC Bunble Click including SCD41 carbon dioxide sensor and SPS30 optical particle sensor using Silicon Labs platform.

This driver has the related drivers, which may be worth reading before. Find them here:

- [SCD41 - HVAC Click (Mikroe)](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/app/documentation/example/mikroe_hvac_scd41)
- [SPS30 - Particulate Matter Sensor (Sparkfun) - I2C](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/tree/master/app/documentation/example/sparkfun_particulate_matter_sensor_sps30)

## Required Hardware ##

- 1x [XG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit) EFR32xG24 Explorer Kit

- Or 1x [Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917 (e.g. [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit) or [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board))

- 1x [HVAC Click board](https://www.mikroe.com/hvac-click) based on SCD41 sensor

- 1x [SPS30 - Particulate Matter Sensor](https://www.sparkfun.com/products/15103)

## Hardware Connection ##

- **If the EFR32xG24 Explorer Kit is used**:

  The HVAC Click board supports MikroBus, so it can connect easily to the MikroBus header of the Explorer Kit. Be sure that the 45-degree corner of the Click Board matches the 45-degree white line of the Silicon Labs Explorer Kit. The Click board also has extra connector to connect with the Particulate Matter Sensor SPS30.

  The hardware connection is shown in the image below:

  ![board](image/hardware_connection.png)

- **If the Wi-Fi Development Kit is used**:

  The hardware connection is shown in the table below:

  | Description  | BRD4338A + BRD4002A | BRD2605A     | HVAC Click   |
  | -------------| -----------| -------------| ------------------------|
  | I2C_SDA      | ULP_GPIO_6 [EXP_16] | ULP_GPIO_6   | SDA            |
  | I2C_SCL      | ULP_GPIO_7 [EXP_15] | ULP_GPIO_7   | SCL            |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by "bundle".

2. Click **Create** button on the **Third Party Hardware Drivers - SCD41 & SPS30 - HVAC Click Bundle (Mikroe)** example. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for your board using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/mikroe_hvac_bundle_scd41_sps30/app.c` into the project root folder (overwriting the existing file).

3. Install the software components:

    - Open the .slcp file in the project.

    - Select the SOFTWARE COMPONENTS tab.

    - Install the following components:

        - **If the Explorer Kit is used:**
          - [Services] → [Timers] → [Sleep Timer]
          - [Services] → [IO Stream] → [IO Stream: USART] → default instance name: vcom
          - [Application] → [Utility] → [Log]
          - [Application] → [Utility] → [Assert]
          - [Third Party Hardware Drivers] → [Sensors] → [SCD41 & SPS30 - HVAC Click Bundle (Mikroe)]

        - **If the Wi-Fi Development Kit is used:**
          - [Application] → [Utility] → [Assert]
          - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Service] → [Sleep Timer for Si91x]
          - [WiSeConnect 3 SDK] → [Device] → [Si91x] → [MCU] → [Peripheral] → [I2C] → [i2c2]
          - [Third Party Hardware Drivers] → [Sensors] → [SCD41 & SPS30 - HVAC Click Bundle (Mikroe)]

4. Enable **Printf float**

   - Open Properties of the project.
   - Select C/C++ Build → Settings → Tool Settings → GNU ARM C Linker → General → Check **Printf float**.

      ![float](image/print_float.png)

5. Build and flash this example to the board.

## How It Works ##

After initializing, the program will measure the Carbon Dioxide concentration, temperature, relative humidity, and other particle concentrations such as (pm1.0, pm2.5, pm4.0, pm10.0 ...). Use Putty/Tera Term (or another program) to read the values of the serial output. You should expect a similar output to the one below.

![logging_screen](image/log.png)

There is a periodic timer in the code, which determines the sampling intervals; the default sensor sampling rate is 5000 ms. If you need more frequent sampling, it is possible to change the corresponding timer value of the `app.c` file.

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
