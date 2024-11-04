# Pervasive Displays Library Suite - Basic edition for EXT4 and EPDK-Matter

[![Release](https://img.shields.io/github/v/release/rei-vilo/PDLS_EXT4_Basic_Matter)](https://github.com/rei-vilo/PDLS_EXT4_Basic_Matter/releases) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/77ecc9fa99834e299505f5f2a7c83e53)](https://app.codacy.com/gh/rei-vilo/PDLS_EXT4_Basic_Matter/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=rei-vilo/PDLS_EXT4_Basic_Matter&amp;utm_campaign=Badge_Grade) ![Arduino lint](https://github.com/rei-vilo/PDLS_EXT4_Basic_Matter/actions/workflows/main.yml/badge.svg) [![GitHub issues](https://img.shields.io/github/issues/rei-vilo/PDLS_EXT3_Basic_Global)](https://github.com/rei-vilo/PDLS_EXT3_Basic_Global/issues)

[![](https://img.shields.io/badge/-Wiki-blue)](https://docs.pervasivedisplays.com/) [![](https://img.shields.io/badge/-User_Guide-blue)](https://pdls.pervasivedisplays.com/userguide/index.html) [![](https://img.shields.io/badge/-Reference_manual-blue)](https://rei-vilo.github.io/PDLS_EXT3_Basic_Documentation/html/index.html)

The Pervasive Displays Library Suite is specifically designed for the [Pervasive Displays](https://www.pervasivedisplays.com) e-paper screens, extension boards and evaluation kits.

<center>![](https://pdls.pervasivedisplays.com/userguide/img/Logo_PDI_text_320.png)</center>

The **PDLS\_EXT4\_Basic\_Matter** library supports the screens

* iTC [monochrome screens with wide temperature and embedded fast update](https://www.pervasivedisplays.com/products/?_sft_etc_itc=pu&_sft_temperature_range=m15c-to-p60c) (film `K`), up to 2.9". 

<!-- <center>![](img/Logo-PDI-white.png) ![](img/Logo_Arduino.png) ![](img/Logo_SiLabs.png)</center> -->

the board

* Pervasive Displays [EPD Extension Kit Gen 4 (EXT4)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-4-EXT4/) and EXT4-to-Nano adapter boards;

and the kit

* [E-paper Pervasive Displays Kit for Matter (EPDK-Matter)](https://www.pervasivedisplays.com), which includes a Pervasive Displays [EPD Extension Kit Gen 4 (EXT4)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-3-EXT3/) and a Pervasive Displays EXT4-to-Nano adapter boards, a Pervasive Displays [2.9″ 384x168 E-ink monochrome high-definition](https://www.pervasivedisplays.com/product/2-9-e-ink-displays/) with embedded fast update and wide temperature display, an [Arduino Nano Matter](https://store.arduino.cc/pages/nano-matter) board based on [Silicon Labs MGM240S](https://www.silabs.com/wireless/zigbee/efr32mg24-series-2-modules).

The library has also been tested successfully against the [Silicon Labs EFR32xG24 Explorer Kit](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit) board.

## Features

The Pervasive Displays Library Suite provides a high-level interface to drive the e-paper screens.

* Fast update
* Graphics routines
* Text routines
* Four extended fonts with double-sized variants
* Access to sensors interrupt signals

## Documentation

* [Wiki](https://docs.pervasivedisplays.com/) 
* [User guide](https://pdls.pervasivedisplays.com/userguide/index.html) with commented examples
* [Reference manual](https://rei-vilo.github.io/PDLS_EXT3_Basic_Documentation/index.html) 

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

* [Connecting the Pervasive Displays e-Paper EPD Extension Kit Gen 4](https://embeddedcomputing.weebly.com/connecting-the-e-paper-epd-extension-kit-gen-4.html)

## Configuration

Software

* Arduino SDK
* SPI library
* Wire library
* [QRCode library](https://www.arduino.cc/reference/en/libraries/qrcode/) to generate the QR-codes
* [Silicon Labs Arduino Core](https://github.com/siliconlabs/arduino)

Hardware

+ Pervasive Displays EPD Extension Kit Gen 4 (EXT4)

## Licence

**Copyright** &copy; Rei Vilo, 2010-2024

For exclusive use with Pervasive Displays screens

**Licence** [Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)](./LICENCE.md)

![](img/by-sa.svg)

**Portions** &copy; Pervasive Displays, Silicon Laboratories Inc., Arduino, 2010-2024

