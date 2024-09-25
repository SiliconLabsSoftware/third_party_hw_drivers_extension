# E-Paper EXT3-1 #

## Summary ##

Electronic paper (E-Paper) or intelligent paper, is a display device that reflects ambient light, mimicking the appearance of ordinary ink on paper. EPD Extension Kit Generation 3 (EXT3 Kit) is a new extension board that connects to E-Paper Display (EPD).
This project aims to implement a hardware driver interacting with the EXT3-1 Kit to control an E-Paper Display.

## Required Hardware ##

- [EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview)
- Or [SiWx917 Wi-Fi 6 and Bluetooth LE 8 MB Flash SoC Pro Kit](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-pk6031a-wifi-6-bluetooth-le-soc-pro-kit) (BRD4002 + BRD4338A)
- [EPD Extension Kit Generation 3 revision 1](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-3-ext3/#tab-3)
- [5.81″ E-ink displays](https://www.pervasivedisplays.com/product/5-81-e-ink-displays/)

## Hardware Connection ##

To connect the EPD Extension Kit to the EFR32xG24 Explorer Kit, you can see the pins mapping table below.

| EFR32xG24 Explorer Kit | SiWx917-RB4338A Radio Board | EXT3-1 Kit | Pin function |
| --- | --- | --- | --- |
| PC0 | GPIO_49 (P30)| CS | SPI CS |
| PC1 | GPIO_46 (P24) | CLK | SPI SCK |
| PC2 | GPIO_47 (P26) | MISO | SPI MISO |
| PC3 | GPIO_48 (P28) | MOSI | SPI MOSI |
| PA0 | GPIO_50 (P32) | D/C | GPIO |
| PD5 | GPIO_52 (P40) | RST | GPIO |
| PB1 | GPIO_51 (P34) | BUSY | GPIO |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **ext3-1**.

2. Click **Create** button on **Third Party Hardware Drivers - E-paper EXT3-1** examples. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

   ![create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit Board" using Simplicity Studio v5. Use the default project settings.

2. Copy the file `app/example/epaper_ext3_1/app.c` and `app/example/epaper_ext3_1/silabs_logo.h` into the project root folder (overwriting the existing file).

3. Install the software components:

   - Open the .slcp file in the project.

   - Select the SOFTWARE COMPONENTS tab.

   - Install the following components:
      - [Third Party Hardware Drivers] → [Display & LED] → [E-Paper EXT3-1 (Pervasive Displays)]
      - [Third Party Hardware Drivers] → [Services] → [GLIB - OLED Graphics Library]

4. Build and flash the project to your device.

**Note:**

- Make sure that the **Third Party Hardware Drivers** extension is installed. If not, follow [this documentation](https://github.com/SiliconLabs/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).

- SDK Extension must be enabled for the project to install **E-Paper EXT3-1 (Pervasive Displays)** component.

## How It Works ##

E-paper (also called ePaper) is an electronic display technology that mimics the appearance of paper. Using the same inks as the traditional printing industry, e-paper displays (EPDs) have tiny capsules filled with charged ink particles. When the proper charge is applied, an EPD creates highly detailed images with the contrast ratio and readability of printed material.

### Testing ###

The E-paper will look something like the picture below.

![result](image/result.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabs/third_party_hw_drivers_extension) repo.
