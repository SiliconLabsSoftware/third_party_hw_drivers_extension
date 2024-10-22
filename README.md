> [!CAUTION]
> You are viewing an older version of the extension. This major version is no longer maintained and may only receive critical bug fixes. Please check out the [main](https://github.com/SiliconLabs/third_party_hw_drivers_extension_staging) branch for the latest version.


# Third Party Hardware Drivers #

[![Version Badge](https://img.shields.io/badge/-v2.1.0-green)](https://github.com/SiliconLabs/third_party_hw_drivers_extension/releases)
[![GSDK Badge](https://img.shields.io/badge/GSDK-v4.4.4-green)](https://github.com/SiliconLabs/gecko_sdk/releases)
![License badge](https://img.shields.io/badge/License-Zlib-green)

This repo contains example drivers developed by Silicon Labs engineers and are provided as [an SDK extension](https://docs.silabs.com/simplicity-studio-5-users-guide/latest/ss-5-users-guide-getting-started/install-sdk-extensions). These drivers interface with different types of external hardware and are tested against a specific device listed in the driver name. These drivers are intended to be fully compatible with Silicon Labs' wireless stacks. All drivers provide a minimal Simplicity Studio project for testing and to show an example usage of the driver. The driver source is provided for modification and extension as needed. All drivers in this repository are considered to be EVALUATION QUALITY which implies that the code provided in the repos is subjected to limited testing and is provided provided as-is. It is not suitable for production environments.

A full list of all examples and drivers in this repository is defined below:

## Examples ##

| No | Name | Example | Documentation |
|:--:|:-------------|:-------------------:|:---------------:|
| 1 | A-172-MRQ - Fingerprint 2 Click (Mikroe) | [Example](./app/example/mikroe_fingerprint2_a172mrq) | [Documentation](./app/documentation/example/mikroe_fingerprint2_a172mrq) |
| 2 | A3967 - Stepper Click (Mikroe) | [Example](./app/example/mikroe_stepper_a3967) | [Documentation](./app/documentation/example/mikroe_stepper_a3967) |
| 3 | A4988 - Stepper 2 Click (Mikroe) | [Example](./app/example/mikroe_stepper2_a4988) | [Documentation](./app/documentation/example/mikroe_stepper2_a4988) |
| 4 | AD8318 - RF Meter Click (Mikroe) | [Example](./app/example/mikroe_rf_meter_ad8318) | [Documentation](./app/documentation/example/mikroe_rf_meter_ad8318) |
| 5 | AK9753 - Human Presence Sensor (Sparkfun) | [Example](./app/example/sparkfun_human_presence_ak9753) | [Documentation](./app/documentation/example/sparkfun_human_presence_ak9753) |
| 6 | AMG88XX - Grid-EYE Infrared Array Breakout (Sparkfun) | [Example](./app/example/sparkfun_ir_array_amg88xx) | [Documentation](./app/documentation/example/sparkfun_ir_array_amg88xx) |
| 7 | AS3935 - Thunder Click (Mikroe) | [Example](./app/example/mikroe_thunder_as3935) | [Documentation](./app/documentation/example/mikroe_thunder_as3935) |
| 8 | AS7265X - Triad Spectroscopy Sensor (SparkFun) | [Example](./app/example/sparkfun_spectroscopy_as7265x) | [Documentation](./app/documentation/example/sparkfun_spectroscopy_as7265x) |
| 9 | BG96 - LTE IoT 2 (Mikroe) | [Example](./app/example/mikroe_lte_iot2_bg96) | [Documentation](./app/documentation/example/mikroe_lte_iot2_bg96) |
| 10 | BMA400 - Accel 5 Click (Mikroe) - I2C | [Example](./app/example/mikroe_accel5_bma400) | [Documentation](./app/documentation/example/mikroe_accel5_bma400) |
| 11 | BMA400 - Accel 5 Click (Mikroe) - SPI | [Example](./app/example/mikroe_accel5_bma400) | [Documentation](./app/documentation/example/mikroe_accel5_bma400) |
| 12 | BME280 & CCS811 - Environmental Sensor Combo Breakout (Sparkfun) | [Example](./app/example/sparkfun_environmental_bme280_ccs811) | [Documentation](./app/documentation/example/sparkfun_environmental_bme280_ccs811) |
| 13 | BME688 - Environment 3 Click (Mikroe) - I2C | [Example](./app/example/mikroe_environment3_bme688) | [Documentation](./app/documentation/example/mikroe_environment3_bme688) |
| 14 | BME688 - Environment 3 Click (Mikroe) - SPI | [Example](./app/example/mikroe_environment3_bme688) | [Documentation](./app/documentation/example/mikroe_environment3_bme688) |
| 15 | BTHome v2 | [Example](./app/example/bthome_v2) | [Documentation](./app/documentation/example/bthome_v2) |
| 16 | BTHome v2 - Server | [Example](./app/example/bthome_v2_server) | [Documentation](./app/documentation/example/bthome_v2_server) |
| 17 | CAP1166 - Cap Touch 2 Click (Mikroe) | [Example](./app/example/mikroe_captouch2_cap1166) | [Documentation](./app/documentation/example/mikroe_captouch2_cap1166) |
| 18 | CMT-8540S-SMT - Buzz 2 Click (Mikroe) | [Example](./app/example/mikroe_buzz2_cmt_8540s_smt) | [Documentation](./app/documentation/example/mikroe_buzz2_cmt_8540s_smt) |
| 19 | DPS310 - Pressure 3 Click (Mikroe) - I2C | [Example](./app/example/mikroe_pressure3_dps310) | [Documentation](./app/documentation/example/mikroe_pressure3_dps310) |
| 20 | DPS310 - Pressure 3 Click (Mikroe) - SPI | [Example](./app/example/mikroe_pressure3_dps310) | [Documentation](./app/documentation/example/mikroe_pressure3_dps310) |
| 21 | DS3231M - RTC 10 (Mikroe) | [Example](./app/example/mikroe_rtc10_ds3231m) | [Documentation](./app/documentation/example/mikroe_rtc10_ds3231m) |
| 22 | DWM1000 - UWB Click (Mikroe) | [Example](./app/example/mikroe_uwb_dwm1000) | [Documentation](./app/documentation/example/mikroe_uwb_dwm1000) |
| 23 | DWM3000 - UWB 2 Click (Mikroe) | [Example](./app/example/mikroe_uwb2_dwm3000/SimplicityStudio) | [Documentation](./app/documentation/example/mikroe_uwb2_dwm3000) |
| 24 | E-Paper display 1.54" 200x200 dots (Mikroe) | [Example](./app/example/mikroe_e_paper_154_inch) | [Documentation](./app/documentation/example/mikroe_e_paper_154_inch) |
| 25 | EM3080-W - Barcode 2 Click (Mikroe) | [Example](./app/example/mikroe_barcode2_em3080w) | [Documentation](./app/documentation/example/mikroe_barcode2_em3080w) |
| 26 | EMG Click (Mikroe) | [Example](./app/example/mikroe_emg) | [Documentation](./app/documentation/example/mikroe_emg) |
| 27 | FSR400 - Force 3 Click (Mikroe) | [Example](./app/example/mikroe_force3_fsr400) | [Documentation](./app/documentation/example/mikroe_force3_fsr400) |
| 28 | HXD8357D - TFT LCD with Touchscreen (Adafruit) - SPI | [Example](./app/example/adafruit_tft_lcd_hxd8357d) | [Documentation](./app/documentation/example/adafruit_tft_lcd_hxd8357d) |
| 29 | HXD8357D - TFT LCD with Touchscreen (Adafruit) - SPI with DMA | [Example](./app/example/adafruit_tft_lcd_hxd8357d) | [Documentation](./app/documentation/example/adafruit_tft_lcd_hxd8357d) |
| 30 | HXD8357D - TFT LCD with Touchscreen (Adafruit) with LVGL | [Example](./app/example/adafruit_tft_lcd_hxd8357d_lvgl) | [Documentation](./app/documentation/example/adafruit_tft_lcd_hxd8357d_lvgl) |
| 31 | HXD8357D - TFT LCD with Touchscreen (Adafruit) with LVGL + DMA | [Example](./app/example/adafruit_tft_lcd_hxd8357d_lvgl) | [Documentation](./app/documentation/example/adafruit_tft_lcd_hxd8357d_lvgl) |
| 32 | ID-12LA - RFID Qwiic Reader (Sparkfun) | [Example](./app/example/sparkfun_rfid_id12la) | [Documentation](./app/documentation/example/sparkfun_rfid_id12la) |
| 33 | ILI9341 - TFT LCD with Touchscreen (Adafruit) - SPI | [Example](./app/example/adafruit_tft_lcd_ili9341) | [Documentation](./app/documentation/example/adafruit_tft_lcd_ili9341) |
| 34 | ILI9341 - TFT LCD with Touchscreen (Adafruit) - SPI with DMA | [Example](./app/example/adafruit_tft_lcd_ili9341) | [Documentation](./app/documentation/example/adafruit_tft_lcd_ili9341) |
| 35 | ILI9341 - TFT LCD with Touchscreen (Adafruit) with LVGL | [Example](./app/example/adafruit_tft_lcd_ili9341_lvgl) | [Documentation](./app/documentation/example/adafruit_tft_lcd_ili9341_lvgl) |
| 36 | ILI9341 - TFT LCD with Touchscreen (Adafruit) with LVGL + DMA | [Example](./app/example/adafruit_tft_lcd_ili9341_lvgl) | [Documentation](./app/documentation/example/adafruit_tft_lcd_ili9341_lvgl) |
| 37 | IR Generator (Silabs) | [Example](./app/example/silabs_ir_generate) | [Documentation](./app/documentation/example/silabs_ir_generate) |
| 38 | IRA-S210ST01 - PIR Sensor (Silabs) | [Example](./app/example/silabs_pir_ira_s210st01) | [Documentation](./app/documentation/example/silabs_pir_ira_s210st01) |
| 39 | IS31FL3741 - 13x9 PWM RGB LED Matrix (Adafruit) | [Example](./app/example/adafruit_rgb_led_is31fl3741) | [Documentation](./app/documentation/example/adafruit_rgb_led_is31fl3741) |
| 40 | L9958 - DC Motor 24 Click (Mikroe) | [Example](./app/example/mikroe_dcmotor24_l9958) | [Documentation](./app/documentation/example/mikroe_dcmotor24_l9958) |
| 41 | LB11685AV - Brushless 16 Click (Mikroe) | [Example](./app/example/mikroe_brushless16_lb11685av) | [Documentation](./app/documentation/example/mikroe_brushless16_lb11685av) |
| 42 | LCA717 - Relay 2 Click (Mikroe) | [Example](./app/example/mikroe_relay2_lca717) | [Documentation](./app/documentation/example/mikroe_relay2_lca717) |
| 43 | LEA-6S - GPS Click (Mikroe) | [Example](./app/example/mikroe_gps_lea6s) | [Documentation](./app/documentation/example/mikroe_gps_lea6s) |
| 44 | LIN Bus Master | [Example](./app/example/silabs_lin_bus/master) | [Documentation](./app/documentation/example/silabs_lin_bus) |
| 45 | LIN Bus Slave Device 1 | [Example](./app/example/silabs_lin_bus/slave) | [Documentation](./app/documentation/example/silabs_lin_bus) |
| 46 | LIN Bus Slave Device 2 | [Example](./app/example/silabs_lin_bus/slave) | [Documentation](./app/documentation/example/silabs_lin_bus) |
| 47 | MAX17048 - MAX17048EVKIT Evaluation Kits (Maxim) | [Example](./app/example/maxim_fuel_gauge_max17048) | [Documentation](./app/documentation/example/maxim_fuel_gauge_max17048) |
| 48 | MAX30101 & MAX32664 - Pulse Oximeter and Heart Rate Sensor (Sparkfun) | [Example](./app/example/sparkfun_hr_po_max30101_max32664) | [Documentation](./app/documentation/example/sparkfun_hr_po_max30101_max32664) |
| 49 | MAX30101 - Heart Rate 4 Click (Mikroe) | [Example](./app/example/mikroe_heartrate4_max30101) | [Documentation](./app/documentation/example/mikroe_heartrate4_max30101) |
| 50 | MAX6969 - UT-M 7-SEG R Click (Mikroe) | [Example](./app/example/mikroe_utm7segr_max6969) | [Documentation](./app/documentation/example/mikroe_utm7segr_max6969) |
| 51 | MAXM86161 - Heartrate 2 Click (Mikroe) | [Example](./app/example/mikroe_heartrate2_maxm86161) | [Documentation](./app/documentation/example/mikroe_heartrate2_maxm86161) |
| 52 | MIC 2 Click (Mikroe) | [Example](./app/example/mikroe_mic2) | [Documentation](./app/documentation/example/mikroe_mic2) |
| 53 | microSD Click (Mikroe) | [Example](./app/example/mikroe_microsd) | [Documentation](./app/documentation/example/mikroe_microsd) |
| 54 | ML8511A - UV Click (Mikroe) | [Example](./app/example/mikroe_uv_ml8511a) | [Documentation](./app/documentation/example/mikroe_uv_ml8511a) |
| 55 | MLX90632 - IrThermo 3 Click (Mikroe) | [Example](./app/example/mikroe_irthermo3_mlx90632) | [Documentation](./app/documentation/example/mikroe_irthermo3_mlx90632) |
| 56 | MLX90640 - IR Array Breakout (Sparkfun) | [Example](./app/example/sparkfun_ir_array_mlx90640) | [Documentation](./app/documentation/example/sparkfun_ir_array_mlx90640) |
| 57 | MM5D91-00 - Radar Click (Mikroe) | [Example](./app/example/mikroe_radar_mm5d91_00) | [Documentation](./app/documentation/example/mikroe_radar_mm5d91_00) |
| 58 | MMA8452Q - Triple Axis Accelerometer Breakout (Sparkfun) | [Example](./app/example/sparkfun_accelerometer_mma8452q) | [Documentation](./app/documentation/example/sparkfun_accelerometer_mma8452q) |
| 59 | MQ131 - Ozone 2 Click (Mikroe) | [Example](./app/example/mikroe_ozone2_mq131) | [Documentation](./app/documentation/example/mikroe_ozone2_mq131) |
| 60 | MQ3 - Alcohol Click (Mikroe) | [Example](./app/example/mikroe_alcohol_mq3) | [Documentation](./app/documentation/example/mikroe_alcohol_mq3) |
| 61 | MQ7 - CO Click (Mikroe) | [Example](./app/example/mikroe_co_mq7) | [Documentation](./app/documentation/example/mikroe_co_mq7) |
| 62 | NeoTrellis 4x4 Keypad (Adafruit) | [Example](./app/example/adafruit_neotrellis_keypad_atsamd09) | [Documentation](./app/documentation/example/adafruit_neotrellis_keypad_atsamd09) |
| 63 | NT3H2111 - NFC Tag 2 Click (Mikroe) - I2C | [Example](./app/example/mikroe_nfctag2_nt3h2111) | [Documentation](./app/documentation/example/mikroe_nfctag2_nt3h2111) |
| 64 | PCA9685 - Servo Click (Mikroe) | [Example](./app/example/mikroe_servo_pca9685) | [Documentation](./app/documentation/example/mikroe_servo_pca9685) |
| 65 | PL-N823-01 - PIR Click (Mikroe) | [Example](./app/example/mikroe_pir_pl_n823_01) | [Documentation](./app/documentation/example/mikroe_pir_pl_n823_01) |
| 66 | PN7150 - NFC 2 Click (Mikroe) - I2C | [Example](./app/example/mikroe_nfc2_pn7150) | [Documentation](./app/documentation/example/mikroe_nfc2_pn7150) |
| 67 | PN7150 - Read a T2T Tag with NCI | [Example](./app/example/mikroe_nfc2_pn7150_nci_t2t_read) | [Documentation](./app/documentation/example/mikroe_nfc2_pn7150_nci_t2t_read) |
| 68 | PN7150 - Write to a T2T Tag with NCI | [Example](./app/example/mikroe_nfc2_pn7150_nci_t2t_write) | [Documentation](./app/documentation/example/mikroe_nfc2_pn7150_nci_t2t_write) |
| 69 | Pocket Geiger Radiation Sensor - Type 5 (Sparkfun) | [Example](./app/example/sparkfun_dosimeter_type5) | [Documentation](./app/documentation/example/sparkfun_dosimeter_type5) |
| 70 | Qwiic Joystick (Sparkfun) | [Example](./app/example/sparkfun_qwiic_joystick) | [Documentation](./app/documentation/example/sparkfun_qwiic_joystick) |
| 71 | Qwiic Keypad (Sparkfun) | [Example](./app/example/sparkfun_qwiic_keypad) | [Documentation](./app/documentation/example/sparkfun_qwiic_keypad) |
| 72 | Qwiic Soil Moisture Sensor (Sparkfun) - I2C | [Example](./app/example/sparkfun_soil_moisture) | [Documentation](./app/documentation/example/sparkfun_soil_moisture) |
| 73 | RNG Click (Mikroe) | [Example](./app/example/mikroe_rng) | [Documentation](./app/documentation/example/mikroe_rng) |
| 74 | SCD41 & SPS30 - HVAC Click Bundle (Mikroe) | [Example](./app/example/mikroe_hvac_bundle_scd41_sps30) | [Documentation](./app/documentation/example/mikroe_hvac_bundle_scd41_sps30) |
| 75 | SCD41 - HVAC Click (Mikroe) | [Example](./app/example/mikroe_hvac_scd41) | [Documentation](./app/documentation/example/mikroe_hvac_scd41) |
| 76 | SGP40 - Air Quality Sensor (Sparkfun) | [Example](./app/example/sparkfun_air_quality_sgp40) | [Documentation](./app/documentation/example/sparkfun_air_quality_sgp40) |
| 77 | SHT40 & SGP40 - Environment 2 Click (Mikroe) | [Example](./app/example/mikroe_environment2_sht40_sgp40) | [Documentation](./app/documentation/example/mikroe_environment2_sht40_sgp40) |
| 78 | SHT40 - Temp&Hum 15 Click (Mikroe) | [Example](./app/example/mikroe_temphum15_sht40) | [Documentation](./app/documentation/example/mikroe_temphum15_sht40) |
| 79 | SHTC3 - Temp&Hum 9 Click (Mikroe) | [Example](./app/example/mikroe_temphum9_shtc3) | [Documentation](./app/documentation/example/mikroe_temphum9_shtc3) |
| 80 | Si8711CC - PWM Driver Click (Mikroe) | [Example](./app/example/mikroe_pwm_driver_si8711cc) | [Documentation](./app/documentation/example/mikroe_pwm_driver_si8711cc) |
| 81 | Smoke 2 Click (Mikroe) - I2C | [Example](./app/example/mikroe_smoke2_adpd188bi) | [Documentation](./app/documentation/example/mikroe_smoke2_adpd188bi) |
| 82 | Smoke 2 Click (Mikroe) - SPI | [Example](./app/example/mikroe_smoke2_adpd188bi) | [Documentation](./app/documentation/example/mikroe_smoke2_adpd188bi) |
| 83 | SPS30 - Particulate Matter Sensor (Sparkfun) - I2C | [Example](./app/example/sparkfun_particulate_matter_sensor_sps30) | [Documentation](./app/documentation/example/sparkfun_particulate_matter_sensor_sps30) |
| 84 | SPS30 - Particulate Matter Sensor (Sparkfun) - UART | [Example](./app/example/sparkfun_particulate_matter_sensor_sps30) | [Documentation](./app/documentation/example/sparkfun_particulate_matter_sensor_sps30) |
| 85 | SSD1306 - Micro OLED Breakout Qwiic (Sparkfun) | [Example](./app/example/sparkfun_micro_oled_ssd1306) | [Documentation](./app/documentation/example/sparkfun_micro_oled_ssd1306) |
| 86 | SSD1306 - Mikroe OLEDW Click (Mikroe) | [Example](./app/example/mikroe_oledw_ssd1306) | [Documentation](./app/documentation/example/mikroe_oledw_ssd1306) |
| 87 | SSD1306 - Mikroe OLEDW Click (Mikroe) with GLIB | [Example](./app/example/mikroe_oledw_ssd1306_glib) | [Documentation](./app/documentation/example/mikroe_oledw_ssd1306_glib) |
| 88 | SSD1351 - OLED C Click (Mikroe) | [Example](./app/example/mikroe_oledc_ssd1351) | [Documentation](./app/documentation/example/mikroe_oledc_ssd1351) |
| 89 | ST7789 - TFT LCD Display (Adafruit) - SPI | [Example](./app/example/adafruit_tft_lcd_st7789) | [Documentation](./app/documentation/example/adafruit_tft_lcd_st7789) |
| 90 | ST7789 - TFT LCD Display (Adafruit) - SPI with DMA | [Example](./app/example/adafruit_tft_lcd_st7789) | [Documentation](./app/documentation/example/adafruit_tft_lcd_st7789) |
| 91 | ST7789 - TFT LCD Display (Adafruit) - with LVGL + DMA | [Example](./app/example/adafruit_tft_lcd_st7789_lvgl_dma) | [Documentation](./app/documentation/example/adafruit_tft_lcd_st7789_lvgl_dma) |
| 92 | STN1110 - OBDII Click (Mikroe) | [Example](./app/example/mikroe_obdii_stn1110) | [Documentation](./app/documentation/example/mikroe_obdii_stn1110) |
| 93 | Stretch Click (Mikroe) | [Example](./app/example/mikroe_stretch) | [Documentation](./app/documentation/example/mikroe_stretch) |
| 94 | TB6549FG - DC Motor 3 Click (Mikroe) | [Example](./app/example/mikroe_dcmotor3_tb6549fg) | [Documentation](./app/documentation/example/mikroe_dcmotor3_tb6549fg) |
| 95 | TB9053FTG - DC Motor 26 Click (Mikroe) | [Example](./app/example/mikroe_dcmotor26_tb9053ftg) | [Documentation](./app/documentation/example/mikroe_dcmotor26_tb9053ftg) |
| 96 | Triac Driver (Silabs) | [Example](./app/example/silabs_triac) | [Documentation](./app/documentation/example/silabs_triac) |
| 97 | TSC2007 - Resistive Touch Screen Controller (Adafruit) | [Example](./app/example/adafruit_touchscreen_tsc2007) | [Documentation](./app/documentation/example/adafruit_touchscreen_tsc2007) |
| 98 | TSD-10 - Turbidity Click (Mikroe) | [Example](./app/example/mikroe_turbidity_tsd10) | [Documentation](./app/documentation/example/mikroe_turbidity_tsd10) |
| 99 | VCNL4040 - Proximity Sensor (Sparkfun) | [Example](./app/example/sparkfun_proximity_vcnl4040) | [Documentation](./app/documentation/example/sparkfun_proximity_vcnl4040) |
| 100 | VL53L1X - Distance Sensor (Sparkfun) | [Example](./app/example/sparkfun_distance_vl53l1x) | [Documentation](./app/documentation/example/sparkfun_distance_vl53l1x) |
| 101 | W5500 - ETH Wiz Click (Mikroe) | [Example](./app/example/mikroe_eth_wiz_w5500) | [Documentation](./app/documentation/example/mikroe_eth_wiz_w5500) |
| 102 | Water Detect Click (Mikroe) | [Example](./app/example/mikroe_water_detect) | [Documentation](./app/documentation/example/mikroe_water_detect) |
| 103 | Weather Station (Sparkfun) | [Example](./app/example/sparkfun_weatherstation) | [Documentation](./app/documentation/example/sparkfun_weatherstation) |


## Drivers ##

<table>
    <tbody>
        <tr>
                <td colspan="3" align="left"><b>Audio & Voice</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;CMT-8540S-SMT - Buzz 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/buzz2_cmt_8540s_smt">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MIC 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/mic2">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Clock & Timing</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;DS3231M - RTC 10 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/rtc10_ds3231m">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Display & LED</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;E-Paper display 1.54" 200x200 dots (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/e_paper_154_inch">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;HXD8357D - TFT LCD Display (Adafruit) - SPI</td>
                    <td>
                        <a href="./driver/public/silabs/tft_lcd_hxd8357d">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;HXD8357D - TFT LCD Display (Adafruit) - SPI with DMA</td>
                    <td>
                        <a href="./driver/public/silabs/tft_lcd_hxd8357d">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;ILI9341 - TFT LCD Display (Adafruit) - SPI</td>
                    <td>
                        <a href="./driver/public/silabs/tft_lcd_ili9341">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;ILI9341 - TFT LCD Display (Adafruit) - SPI with DMA</td>
                    <td>
                        <a href="./driver/public/silabs/tft_lcd_ili9341">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;IS31FL3741 - 13x9 PWM RGB LED Matrix (Adafruit) - I2C</td>
                    <td>
                        <a href="./driver/public/silabs/rgb_led_is31fl3741">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MAX6969 - UT-M 7-SEG R Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/utm7segr_max6969">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SSD1306 - Micro OLED Breakout (Sparkfun) - I2C</td>
                    <td>
                        <a href="./driver/public/silabs/micro_oled_ssd1306">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SSD1306 - OLED W Click (Mikroe) - SPI</td>
                    <td>
                        <a href="./driver/public/mikroe/oledw_ssd1306">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SSD1351 - OLED C Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/oledc_ssd1351">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;ST7789 - TFT LCD Display (Adafruit) - SPI</td>
                    <td>
                        <a href="./driver/public/silabs/tft_lcd_st7789">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;ST7789 - TFT LCD Display (Adafruit) - SPI with DMA</td>
                    <td>
                        <a href="./driver/public/silabs/tft_lcd_st7789">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Human Machine Interface</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;A-172-MRQ - Fingerprint 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/fingerprint2_a172mrq">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;CAP1166 - Capacitive Touch 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/captouch2_cap1166">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;NeoTrellis 4x4 Keypad (Adafruit)</td>
                    <td>
                        <a href="./driver/public/silabs/adafruit_neotrellis_keypad_atsamd09">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Qwiic Joystick (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/qwiic_joystick">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Qwiic Keypad (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/qwiic_keypad">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Touch Screen (Analog)</td>
                    <td>
                        <a href="./driver/public/silabs/services_touch_screen">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;TSC2007 - Resistive Touch Screen Controller (Adafruit)</td>
                    <td>
                        <a href="./driver/public/silabs/touchscreen_tsc2007">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Interface</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;STN1110 - OBDII Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/obdii_stn1110">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;W5500 - ETH WIZ Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/eth_wiz_w5500">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Miscellaneous</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;IR Generator (Silabs)</td>
                    <td>
                        <a href="./driver/public/silabs/ir_generate">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;LCA717 - Relay 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/relay2_lca717">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;RNG Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/rng">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Triac Driver (Silabs)</td>
                    <td>
                        <a href="./driver/public/silabs/triac">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Motor Control</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;A3967 - Stepper Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/silabs/stepper_a3967">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;A4988 - Stepper 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/stepper2_a4988">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;L9958 - DC Motor 24 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/dcmotor24_l9958">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;LB11685AV - Brushless 16 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/brushless16_lb11685av">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;PCA9685 - Servo Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/servo_pca9685">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Si8711CC - PWM Driver Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/pwm_driver_si8711cc">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;TB6549FG - DC Motor 3 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/dcmotor3_tb6549fg">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;TB9053FTG - DC Motor 26 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/dcmotor26_tb9053ftg">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Power Management</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MAX17048 - MAX17048EVKIT Evaluation Kits (Maxim)</td>
                    <td>
                        <a href="./driver/public/silabs/fuel_gauge_max17048">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Sensors</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;AD8318 - RF Meter Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/rf_meter_ad8318">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;ADPD188BI - Smoke 2 Click (Mikroe) - I2C</td>
                    <td>
                        <a href="./driver/public/mikroe/smoke2_adpd188bi">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;ADPD188BI - Smoke 2 Click (Mikroe) - SPI</td>
                    <td>
                        <a href="./driver/public/mikroe/smoke2_adpd188bi">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;AK9753 - Human Presence Sensor (Sparkfun) - I2C</td>
                    <td>
                        <a href="./driver/public/silabs/human_presence_ak9753">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;AMG88XX - Grid-EYE Infrared Array Breakout (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/ir_array_amg88xx">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;AS3935 - Thunder Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/thunder_as3935">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;AS7265x - Triad Spectroscopy Sensor (Sparkfun) - I2C</td>
                    <td>
                        <a href="./driver/public/silabs/spectroscopy_as7265x">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;BMA400 - Accel 5 Click (Mikroe) - I2C</td>
                    <td>
                        <a href="./driver/public/silabs/accel5_bma400">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;BMA400 - Accel 5 Click (Mikroe) - SPI</td>
                    <td>
                        <a href="./driver/public/silabs/accel5_bma400">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;BME280 - Atmospheric Sensor (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/environmental_bme280_ccs811">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;BME688 - Environment 3 Click (Mikroe) - I2C</td>
                    <td>
                        <a href="./driver/public/silabs/environment3_bme688">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;BME688 - Environment 3 Click (Mikroe) - SPI</td>
                    <td>
                        <a href="./driver/public/silabs/environment3_bme688">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;CCS811 - Air Quality Sensor (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/environmental_bme280_ccs811">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;DPS310 - Pressure 3 Click (Mikroe) - I2C</td>
                    <td>
                        <a href="./driver/public/mikroe/pressure3_dps310">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;DPS310 - Pressure 3 Click (Mikroe) - SPI</td>
                    <td>
                        <a href="./driver/public/mikroe/pressure3_dps310">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;EM3080-W - Barcode 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/barcode2_em3080w">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;EMG Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/emg">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;FSR400 - Force 3 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/force3_fsr400">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;IRA-S210ST01 - PIR Sensor (Silabs)</td>
                    <td>
                        <a href="./driver/public/silabs/pir_ira_s210st01">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MAX30101 & MAX32664  - Pulse Oximeter and Heart Rate Sensor (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/hr_po_max30101_max32664">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MAX30101 - Heart Rate 4 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/heartrate4_max30101">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MAXM86161 - Heart Rate 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/heartrate2_maxm86161">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MCP606 - Water Detect Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/water_detect">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;ML8511A - UV Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/uv_ml8511a">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MLX90632 - IrThermo 3 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/irthermo3_mlx90632">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MLX90640 - IR Array Breakout (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/ir_array_mlx90640">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MM5D91-00 - Radar Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/radar_mm5d91_00">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MMA8452Q - Triple Axis Accelerometer Breakout (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/accelerometer_mma8452q">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MQ131 - Ozone 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/ozone2_mq131">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MQ3 - Alcohol Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/alcohol_mq3">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;MQ7 - CO Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/co_mq7">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;PL-N823-01 - PIR Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/pir_pl_n823_01">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Pocket Geiger Radiation - Type 5 (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/dosimeter_type5">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Qwiic Soil Moisture Sensor (Sparkfun) - I2C</td>
                    <td>
                        <a href="./driver/public/silabs/soil_moisture">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SCD41 & SPS30 - HVAC Click Bundle (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/hvac_scd41">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SCD41 - HVAC Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/hvac_scd41">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SGP40 - Air Quality Sensor (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/air_quality_sgp40">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SHT40 & SGP40 - Environment 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/environment2_sht40_sgp40">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SHT40 - Temp&Hum 15 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/temphum15_sht40">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SHTC3 - Temp&Hum 9 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/temphum9_shtc3">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SPS30 - Particulate Matter Sensor (Sparkfun) - I2C</td>
                    <td>
                        <a href="./driver/public/silabs/particulate_matter_sensor_sps30">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;SPS30 - Particulate Matter Sensor (Sparkfun) - UART</td>
                    <td>
                        <a href="./driver/public/silabs/particulate_matter_sensor_sps30">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Stretch Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/stretch">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;TSD-10 - Turbidity Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/turbidity_tsd10">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;VCNL4040 - Proximity Sensor (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/proximity_vcnl4040">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;VL53L1X - Distance Sensor Breakout (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/distance_vl53l1x">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Weather Meter Kit - Rainfall (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/weather_station_rainfall">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Weather Meter Kit - Wind Direction (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/weather_station_wind_direction">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;Weather Meter Kit - Wind Speed (Sparkfun)</td>
                    <td>
                        <a href="./driver/public/silabs/weather_station_wind_speed">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Services</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;BTHome v2</td>
                    <td>
                        <a href="./driver/public/silabs/bthome_v2">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;BTHome v2 - Server</td>
                    <td>
                        <a href="./driver/public/silabs/bthome_v2_server">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;EnOcean Switch Proxy Server</td>
                    <td>
                        <a href="./driver/public/silabs/services_enocean_switch_proxy_server">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;FatFS - Generic FAT Filesystem</td>
                    <td>
                        <a href="./driver/public/silabs/services_fatfs">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;GLIB - OLED Graphics Library</td>
                    <td>
                        <a href="./driver/public/silabs/services_tphd_glib">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;LIN bus slave</td>
                    <td>
                        <a href="./driver/public/silabs/services_lin_bus_slave">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;LVGL - Graphics Library</td>
                    <td>
                        <a href="./driver/public/silabs/services_lvgl">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>&nbsp;&nbsp;&nbsp;&nbsp;mikroSDK 2.0 SDK - Peripheral Drivers</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ADC</td>
                    <td>
                        <a href="./driver/peripheral_drivers/mikroe">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Digital I/O</td>
                    <td>
                        <a href="./driver/peripheral_drivers/mikroe">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;I2C</td>
                    <td>
                        <a href="./driver/peripheral_drivers/mikroe">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PWM</td>
                    <td>
                        <a href="./driver/peripheral_drivers/mikroe">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SPI</td>
                    <td>
                        <a href="./driver/peripheral_drivers/mikroe">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;UART</td>
                    <td>
                        <a href="./driver/peripheral_drivers/mikroe">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>&nbsp;&nbsp;&nbsp;&nbsp;NFC</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;NFC - Common</td>
                    <td>
                        <a href="./driver/public/silabs/services_nfc/common">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;NFC - NCI</td>
                    <td>
                        <a href="./driver/public/silabs/services_nfc/nci">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;NFC - NDEF</td>
                    <td>
                        <a href="./driver/public/silabs/services_nfc/ndef">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;NFC - Tag</td>
                    <td>
                        <a href="./driver/public/silabs/services_nfc/tag">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Storage</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;microSD - microSD Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/microsd">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                <td colspan="3" align="left"><b>Wireless Connectivity</b></td>
            </tr>
            <tr></tr>
            <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;BG96 - LTE IoT 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/lte_iot2_bg96">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;DWM1000 - UWB Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/uwb_dwm1000">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;DWM3000 - UWB 2 Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/silabs/uwb2_dwm3000">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;ID-12LA - RFID Reader (Sparkfun) - I2C</td>
                    <td>
                        <a href="./driver/public/silabs/rfid_id12la">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;LEA-6S - GPS Click (Mikroe)</td>
                    <td>
                        <a href="./driver/public/mikroe/gps_lea6s">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;NT3H2111 - NFC Tag 2 Click (Mikroe) - I2C</td>
                    <td>
                        <a href="./driver/public/silabs/nfctag2_nt3h2111">Driver</a>
                        </td>
                </tr>
        <tr></tr>
        <tr>
                    <td colspan="2" align="left">&nbsp;&nbsp;&nbsp;&nbsp;PN7150 - NFC 2 Click (Mikroe) - I2C</td>
                    <td>
                        <a href="./driver/public/mikroe/nfc2_pn7150">Driver</a>
                        </td>
                </tr>
        </tbody>
</table>

## Requirements ##

- Desired Silicon Labs Starter Kit
- Driver specific test hardware
- Simplicity Studio v5.9.3.1 or newer
- Gecko SDK Suite 4.4.4, available via Simplicity Studio

## How to add to Simplicity Studio IDE ##

[![TPHD Quick Look Video](doc/yt_thumbnail.jpg)](https://youtu.be/vjOGhZXcWGw "Quick Look - SparkFun Micro OLED Display - Silicon Labs")

> **Detailed documentation available on [https://docs.silabs.com](https://docs.silabs.com/application-examples/1.0.0/ae-getting-started/how-do-you-use-it#adding-sdk-extensions-for-hardware-drivers) or [here](doc/readme.md)**.

- Clone the repository somewhere on your PC

  `git clone https://github.com/SiliconLabs/third_party_hw_drivers_extension.git`

- Add the SDK extension to the GSDK

  - In Simplicity Studio go to **Preferences** → **Simplicity Studio** → **SDKs** and select the Gecko SDK Suite to which the SDK extension will be added. Click **Add Extension…**
  - Click **Browse** and navigate to the root folder of the package above and click **Select Folder**.
  - The SDK extension should be displayed in the Detected SDK Extension window with the **Third Party Hardware Drivers** name, version, and path. Click **OK** and then **Trust** and **Apply and Close**.
  - Restart Simplicity Studio

> Make sure that **Evaluation** quality software components are enabled in the Software Component view.

## SDK Outline ##

The Third Party Hardware Drivers SDK contains the following files and folders.

```txt
  +--third_party_hw_drivers.slce  > The UC SDK Extension description file
  +--third_party_hw_drivers.slsdk > File provides the ability to auto-scan protocols from the extension folder
  +--templates.xml                > This file contains meta data for showing the project example on Simplicity Studio
  +--app
  |  +--documentation             > Contains the simple documentations for each application example
  |  +--example                   > Simpler examples to demonstrate specific features
  |-
  +--driver
  |  +--component                 > Contains UC component description files (SLCC)
  |  +--inc                       > Contains Third Party Hardware Driver SDK Extension helpers file
  |  +--peripheral_drivers        > Contains the implementation of the peripheral driver layer's components.
  |-
  +--public                      > Contains the public interfaces for each integrated driver
  |  +--mikroe                   > Contains the public interfaces for integrated drivers from MikroE
  |     +--temphum9_shtc3
  |     +--heartrate2_maxm86161
  |     +--...
  |  +--sparkfun                 > Contains the public interfaces for integrated drivers from SparkFun
  |  +--silabs                   > Contains the public interfaces for integrated drivers from Silabs
  |  +--...
  |-
  +--third_party                 > Contains third-party libraries, and repositories like Mikroe Click library.
  |  +--mikrosdk_click_v2        > Contains the MikroE Click Board™ library sources
  |  +--...
  +--doc                         > Contains the doxygen documentation files
  |-
```

## Reporting Bugs/Issues and Posting Questions and Comments ##

To report bugs in the Application Examples projects, please create a new "Issue" in the "Issues" section of this repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of this repo.
