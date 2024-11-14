///
/// @file EXT4_LargeFont.ino
/// @brief Example of features for EXT4 board
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Nov 2024
/// @version 810
///
/// @copyright (c) Rei Vilo, 2010-2024
/// @copyright Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
/// @copyright For exclusive use with Pervasive Displays screens
///
/// @see ReadMe.txt for references
/// @n
///

// <<#ifndef ARDUINO_ARCH_SILABS
// #error Silicon Labs architecture required
// #endif
//
// #ifndef ARDUINO_MATTER
// #error Matter library required
// #endif

// Screen
#include "PDLS_EXT4_Basic_Matter.h"

#if (PDLS_EXT4_BASIC_MATTER_RELEASE < 810)
#error Required PDLS_EXT4_BASIC_MATTER_RELEASE 810
#endif // PDLS_EXT4_BASIC_MATTER_RELEASE

// SDK
// #include <Arduino.h>
#include "hV_HAL_Peripherals.h"

// Include application, user and local libraries
// #include <SPI.h>

// Configuration
#include "hV_Configuration.h"

// Set parameters
#define DISPLAY_MULTIPLIER 1

// Define structures and classes

// Define variables and constants

// PDLS
Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_290_KS_0F, boardArduinoNanoMatter);
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_290_KS_0F, boardSiLabsBG24Explorer);

// Prototypes

// Utilities
///
/// @brief Wait with countdown
/// @param second duration, s
///
void wait(uint8_t second)
{
    for (uint8_t i = second; i > 0; i--)
    {
        mySerial.print(formatString(" > %i  \r", i));
        delay(1000);
    }
    mySerial.print("         \r");
}

// Functions

#if (DISPLAY_MULTIPLIER == 1)

void displayMultiplier()
{
    myScreen.setOrientation(3);
    uint16_t x = 0;
    uint16_t y = 0;

    String text = "abAB12";
    uint8_t length = text.length();

    myScreen.selectFont(Font_Terminal16x24);
    myScreen.gText(x, y, text);
    x += myScreen.characterSizeX() * length;
    myScreen.gTextLarge(x, y, text);
    y += myScreen.characterSizeY() * 2;
    x = 0;

    myScreen.selectFont(Font_Terminal12x16);
    myScreen.gText(x, y, text);
    x += myScreen.characterSizeX() * length;
    myScreen.gTextLarge(x, y, text);
    y += myScreen.characterSizeY() * 2;
    x = 0;

    myScreen.selectFont(Font_Terminal8x12);
    myScreen.gText(x, y, text);
    x += myScreen.characterSizeX() * length;
    myScreen.gTextLarge(x, y, text);
    y += myScreen.characterSizeY() * 2;
    x = 0;

    myScreen.selectFont(Font_Terminal6x8);
    myScreen.gText(x, y, text);
    x += myScreen.characterSizeX() * length;
    myScreen.gTextLarge(x, y, text);
    y += myScreen.characterSizeY() * 2;
    x = 0;

    myScreen.flush();
    delay(8);
}

#endif //DISPLAY_MULTIPLIER

// Add setup code
///
/// @brief Setup
///
void setup()
{
    // mySerial = Serial by default, otherwise edit hV_HAL_Peripherals.h
    mySerial.begin(115200);
    delay(500);
    mySerial.println();
    mySerial.println("=== " __FILE__);
    mySerial.println("=== " __DATE__ " " __TIME__);
    mySerial.println();

    // Start
    myScreen.begin();
    myScreen.setPowerProfile(POWER_MODE_AUTO, POWER_SCOPE_GPIO_ONLY);
    myScreen.regenerate(); // Clear buffer and screen

#if (DISPLAY_MULTIPLIER == 1)

    mySerial.println("DISPLAY_MULTIPLIER... ");
    myScreen.clear();
    displayMultiplier();
    wait(8);

#endif  // DISPLAY_MULTIPLIER

    mySerial.println("Regenerate... ");
    myScreen.regenerate();

    mySerial.println("=== ");
    mySerial.println();
}

// Add loop code
///
/// @brief Loop, empty
///
void loop()
{
    delay(1000);
}
