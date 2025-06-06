# ST7789 - Color TFT Display (Adafruit) #

## Summary ##

Small TFT displays are a great way to add graphics to your projects. They are like tiny LCD displays that you can control using a simple SPI serial interface. With a resolution of 240x135 pixels, 16-bit full color and an IPS screen, colors display great when off-axis up to 80 degrees in any direction. This TFT display can be used in smart watches and small electronic devices.

This project aims to implement a hardware driver interacting with the ST7789 TFT displays via APIs of SiSDK.

![intro](image/intro.png)

For testing, you will need a ST7789 display breakout, like [Adafruit 1.14" 240x135 Color TFT Display + MicroSD Card Breakout - ST7789](https://www.adafruit.com/product/4383). Make sure that the display you are using has the ST7789 driver chip.

![st7789_display](image/st7789_display.png)

## Required Hardware ##

- 1x [XG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit) EFR32xG24 Explorer Kit
- Or 1x [Wi-Fi Development Kit](https://www.silabs.com/development-tools/wireless/wi-fi) based on SiWG917 (e.g. [SIWX917-DK2605A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-dk2605a-wifi-6-bluetooth-le-soc-dev-kit) or [SIWX917-RB4338A](https://www.silabs.com/development-tools/wireless/wi-fi/siwx917-rb4338a-wifi-6-bluetooth-le-soc-radio-board))
- 1x ST7789 Color TFT display as listed below:
  - [Adafruit 1.14" 240x135 Color TFT Display + MicroSD Card Breakout - ST7789](https://www.adafruit.com/product/4383)

## Hardware Connection ##

To connect the Adafruit 1.14" 240x135 Color TFT Display + MicroSD Card Breakout - ST7789 with your board, you can see the pins mapping table below.

| EFR32xG24 Explorer Kit | BRD4338A + BRD4002A | BRD2605A    | Connection | Pin function |
| --- | --- | --- | --- | --- |
| PC8 | GPIO_47 (P26) | GPIO_10 | D/C | GPIO |
| PC0 | GPIO_28 (P31) | GPIO_28 | CS  | SPI CS |
| PC1 | GPIO_25 (P25) | GPIO_25 | CLK | SPI SCK |
| PC2 | GPIO_26 (P27) | GPIO_26 | MISO | SPI MISO |
| PC3 | GPIO_27 (P29) | GPIO_27 | MOSI | SPI MOSI |

## Setup ##

You can either create a project based on an example project or start with an empty example project.

> [!IMPORTANT]
> - Make sure that the [Third Party Hardware Drivers](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) extension is installed as part of the SiSDK. If not, follow [this documentation](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension/blob/master/README.md#how-to-add-to-simplicity-studio-ide).
> - **Third Party Hardware Drivers** extension must be enabled for the project to install the required components from this extension.

> [!TIP]
> To show all components in the **Third Party Hardware Drivers** extension, the **Evaluation** quality must be enabled in the Software Component view.

### Create a project based on an example project ###

1. From the Launcher Home, add your board to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **"st7789"**.

2. Click **Create** button on the example:

    - **Third Party Hardware Drivers - ST7789 - TFT LCD Display (Adafruit) - SPI** if using without DMA.

    - **Third Party Hardware Drivers - ST7789 - TFT LCD Display (Adafruit) - SPI with DMA** if using with DMA.

    Example project creation dialog pops up -> click Create and Finish and Project should be generated.
    ![Create_example](image/create_example.png)

3. Build and flash this example to the board.

### Start with an empty example project ###

1. Create an "Empty C Project" for the "EFR32xG24 Explorer Kit Board" or "SiWx917-RB4338A Radio Board" using Simplicity Studio v5. Use the default project settings.

2. Copy source files:
   - With Gecko EFR32 SOCs:
     - Copy the file `app/example/adafruit_tft_lcd_st7789/gecko/app.c` into the project root folder (overwriting existing file).
   - With SiWx917 SoCs:
     - Copy the file `app/example/adafruit_tft_lcd_st7789/si91x/app.c` into the project root folder (overwriting existing file).
   - Copy all the *.c files in the  `app/example/adafruit_tft_lcd_st7789` directory into the project root folder (overwriting existing file).

3. Install the software components:

   - Open the .slcp file in the project.

   - Select the SOFTWARE COMPONENTS tab.

   - Install the following components:
      - With Gecko EFR32 SOCs:
         - [Services] → [Timers] → [Sleep Timer]
         - [Platform] → [Driver] → [LED] → [Simple LED] → [led0, led1]
         - [Platform] → [Driver] → [Button] → [Simple Button] → [btn0, btn1]
      - With SiWx917 SoCs:
         - [WiSeConnect 3 SDK] → [Device] → [MCU] → [Service] → [Power Manager] → [Sleep Timer for Si91x]
         - [WiSeConnect 3 SDK] → [Device] → [MCU] → [Hardware] → [LED] → [led0, led1]
         - [WiSeConnect 3 SDK] → [Device] → [MCU] → [Hardware] → [Button] → [btn0, btn1]
      - [Application] → [Utility] → [Assert]
      - If using without DMA: [Third Party Hardware Drivers] → [Display & LED] → [ST7789 - TFT LCD Display (Adafruit) - SPI]
      - If using with DMA: [Third Party Hardware Drivers] → [Display & LED] → [ST7789 - TFT LCD Display (Adafruit) - SPI with DMA]
      - [Third Party Hardware Drivers] → [Services] → [GLIB - OLED Graphics Library]

4. Enable DMA support for SPI module (for SiWx917 SoCs)

   To improve SPI transfer speed, enable DMA support by changing configuration of GSPI component at: **[WiSeConnect 3 SDK] → [Device] → [Si91X] → [MCU] → [Peripheral] → [GSPI]** as the picture bellow

   | | |
   | - | - |
   | ![gspi](image/gspi.png) | ![gspi_dma](image/gspi_dma.png) |

5. Build and flash the project to your device.

## How It Works ##

You can see, as soon as the chip is reset, the program is started. First, the display will show full screen basic colors (white, red, green, blue, cyan, purple, yellow, orange, black). Next, the screen will display the words "HELLO WORLD", "Adafruit", "1.14" TFT". Next is a demo of basic functions such as inverting colors, enable / disable display. Next, the screen The screen will display 5 different images with dimensions of 135x240 pixels. And finally, demo the functions of drawing shapes such as (circle, triangle, rectangle).The demo process will be repeated from the beginning.
There is a timer used in the program - the value is changed through specific demos, by which the users see the detailed content. If you want to do so, you can change them via the macros at the top of the "app.c" file.

![demo](image/demo.gif)

![log](image/log.png)

## Report Bugs & Get Support ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of [third_party_hw_drivers_extension](https://github.com/SiliconLabsSoftware/third_party_hw_drivers_extension) repo.
