# Pervasive Displays Library Suite - Basic edition for EXT4 and EPDK-Matter

[![Release](https://img.shields.io/github/v/release/rei-vilo/PDLS_EXT4_Basic_Matter)](https://github.com/rei-vilo/PDLS_EXT4_Basic_Matter/releases) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/77ecc9fa99834e299505f5f2a7c83e53)](https://app.codacy.com/gh/rei-vilo/PDLS_EXT4_Basic_Matter/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=rei-vilo/PDLS_EXT4_Basic_Matter&amp;utm_campaign=Badge_Grade) ![Arduino lint](https://github.com/rei-vilo/PDLS_EXT4_Basic_Matter/actions/workflows/main.yml/badge.svg) [![GitHub issues](https://img.shields.io/github/issues/rei-vilo/PDLS_EXT3_Basic_Global)](https://github.com/rei-vilo/PDLS_EXT3_Basic_Global/issues)

[![](https://img.shields.io/badge/-Knowledge_Base-orange)](https://docs.pervasivedisplays.com/knowledge/) 

The Pervasive Displays Library Suite is specifically designed for the [Pervasive Displays](https://www.pervasivedisplays.com) e-paper screens, extension boards and evaluation kits.

![](img/Pervasive_Displays_logo_gray.png)

> ### The library **PDLS_EXT4_Basic_Matter** turns legacy. 
>
> As a replacement, use the [PDLS_Basic](https://github.com/rei-vilo/PDLS_Basic) library, with dedicated examples for the [EXT4](https://github.com/rei-vilo/PDLS_Basic/tree/main/examples/EXT4) and [EPDK-Matter](https://github.com/rei-vilo/PDLS_Basic/tree/main/examples/EXT4_Matter).

The **PDLS\_EXT4\_Basic\_Matter** library supports the screens

* iTC [monochrome screens with wide temperature and embedded fast update](https://www.pervasivedisplays.com/products/?_sft_etc_itc=pu&_sft_temperature_range=m15c-to-p60c) (film `K`), up to 2.9";

<!-- <center>![](img/Logo-PDI-white.png) ![](img/Logo_Arduino.png) ![](img/Logo_SiLabs.png)</center> -->

the boards

* Pervasive Displays [EPD Extension Kit Gen 4 (EXT4)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-4-EXT4/), 
* Pervasive Displays EXT4-to-Nano adapter;

and the kit

* ![](https://img.shields.io/badge/-NEW-orange) [E-Paper Development Kit for Matter (EPDK-Matter)](https://www.pervasivedisplays.com), which includes a Pervasive Displays [EPD Extension Kit Gen 4 (EXT4)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-3-EXT3/) and a Pervasive Displays EXT4-to-Nano adapter boards, a Pervasive Displays [2.9″ 384x168 E-ink monochrome high-definition](https://www.pervasivedisplays.com/product/2-9-e-ink-displays/) with embedded fast update and wide temperature display, an [Arduino Nano Matter](https://store.arduino.cc/pages/nano-matter) board based on [Silicon Labs MGM240S](https://www.silabs.com/wireless/zigbee/efr32mg24-series-2-modules).

The library has also been tested successfully against the [Silicon Labs EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit) board.

## Features

The Pervasive Displays Library Suite provides a high-level interface to drive the e-paper screens.

* Fast update
* Graphics routines
* Text routines
* Four extended fonts with double-sized variants
* Access to sensors interrupt signals

## Documentation

* [Knowledge Base](https://docs.pervasivedisplays.com/knowledge/)

## Support

* Please refer to [PDLS_EXT3_Basic_Global](https://github.com/rei-vilo/PDLS_EXT3_Basic_Global/issues) for reporting issues and contributing pull requests.

## Hardware

### EXT4

![](img/Board_EXT4.png)

### Connection to Arduino Nano Matter

![](img/Board_EXT4_Nano_Matter.png)
<!-- 
### Connection to SiLabs xG24 Explorer Kit

![](img/Board_EXT4_xG24.png) -->

## Software

### EXT4 sensors examples

Accelerometer-based orientation | Double-sized fonts
--- | ---
![](img/EXT4_8.gif) | ![](img/EXT4_7.png)

### EXT4 Matter examples

Matter commissioning | Matter connection
--- | ---
![](img/EXT4_1.png) | ![](img/EXT4_2.png)

Weather sensor | RGB bulb
--- | ---
![](img/EXT4_3.png) | ![](img/EXT4_4.png)

One-button menu | About
--- | ---
![](img/EXT4_6.gif) | ![](img/EXT4_5.png)

### EXT4 screen sizes example

The EXT4 extension board and the `PDLS_EXT4_Basic_Matter` library support the monochrome screens with wide temperature and embedded fast update (film `K`) up to 2.90".

1.52" | 2.06"
--- | ---
![](img/EXT4_152_KS.png) | ![](img/EXT4_206_KS.png) 

2.13" | 2.66"
--- | ---
![](img/EXT4_213_KS.png) | ![](img/EXT4_266_KS.png) 

2.71" | 2.90"
--- | ---
![](img/EXT4_271_KS.png) | ![](img/EXT4_290_KS.png)

## Examples

### Common examples

* Common_Colours
* Common_Fonts
* Common_Forms
* Common_Orientation
* Common_Persistent
* Common_Text
* Common_WhoAmI

### Fast update examples

* Example_Fast_Line
* Example_Fast_Orientation
* Example_Fast_Speed

### EXT4 sensors examples

* EXT4_WhoAmI
* EXT4_ScreenSizes
* EXT4_LargeFont
* EXT4_Accelerometer
* EXT4_Weather
* EXT4_WS2813C

### EXT4 Matter examples 
 
* EXT4_Matter_RGB 
* EXT4_Matter_Weather 

## Installation

Software

* [Arduino IDE](https://www.arduino.cc/en/software) or [Arduino CLI](https://arduino.github.io/arduino-cli/)
* [Installing Additional Arduino Libraries](https://www.arduino.cc/en/guide/libraries)

    + Using the Library Manager
    + Importing a `.zip` Library

Hardware

* [Connecting the Pervasive Displays e-Paper EPD Extension Kit Gen 4](https://embeddedcomputing.weebly.com/connecting-the-pervasive-displays-ext4.html)

## Configuration

Software

* Arduino SDK
* SPI library
* Wire library
* [QRCode library](https://www.arduino.cc/reference/en/libraries/qrcode/) to generate the QR-codes
* [Silicon Labs Arduino Core](https://github.com/siliconlabs/arduino) release `2.2.0` or later

Hardware

* Pervasive Displays EPD Extension Kit Gen 4 (EXT4)
* Pervasive Displays E-Paper Development Kit for Matter (EPDK-Matter) with Arduino Nano Matter

## Licence

**Copyright** &copy; Pervasive Displays Inc., 2021-2025, under licence by Rei Vilo

*For exclusive use with Pervasive Displays screens*

**Portions** &copy; Pervasive Displays, Silicon Laboratories Inc., Arduino, 2010-2025

**Licence** [Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)](./LICENCE.md)

![](img/by-sa.svg)

**Copyright** &copy; Rei Vilo, 2010-2025

