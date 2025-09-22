///
/// @file Matter_Nano_EXT4_WhoAmI.ino
/// @brief Example of features for EXT4 board
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Sep 2025
/// @version 921
///
/// @copyright (c) Rei Vilo, 2010-2025
/// @copyright All rights reserved
/// @copyright For exclusive use with Pervasive Displays screens
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// * Evaluation edition: for professionals or organisations, no commercial usage
/// @n All rights reserved
///
/// * Commercial edition: for professionals or organisations, commercial usage
/// @n All rights reserved
///
/// @see ReadMe.txt for references
/// @n
///

// Screen
#include "PDLS_EXT4_Basic_Matter.h"

#if (PDLS_EXT4_BASIC_MATTER_RELEASE < 812)
#error Required PDLS_EXT4_BASIC_MATTER_RELEASE 812
#endif // PDLS_EXT4_BASIC_MATTER_RELEASE

// SDK
// #include <Arduino.h>
#include "hV_HAL_Peripherals.h"

// Include application, user and local libraries
// #include <SPI.h>

// Configuration
#include "hV_Configuration.h"

// Set parameters
#define DISPLAY_WHOAMI 1

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

#if (DISPLAY_WHOAMI == 1)

///
/// @brief Who am i? test screen
///
/// @image html T2_WHOAMI.jpg
/// @image latex T2_WHOAMI.PDF width=10cm
///
void displayWhoAmI()
{
    myScreen.setOrientation(3);
    uint16_t x = 4;
    uint16_t y = 4;

    myScreen.selectFont(Font_Terminal12x16);
    myScreen.gText(x, y, "Who Am I");
    myScreen.gText(x + 1, y, "Who Am I");
    y += myScreen.characterSizeY();

    myScreen.selectFont(Font_Terminal8x12);
    uint16_t dy = myScreen.characterSizeY();

    myScreen.gText(x, y, formatString("%8s %s", "Screen", myScreen.WhoAmI().c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %ix%i", "Size", myScreen.screenSizeX(), myScreen.screenSizeY()));
    y += dy;
    String text = myScreen.screenNumber();
    myScreen.gText(x, y, formatString("%8s %s", "Number", text.c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %s v%i.%i.%i", "PDLS", SCREEN_EPD_EXT3_VARIANT, SCREEN_EPD_EXT3_RELEASE / 100, (SCREEN_EPD_EXT3_RELEASE / 10) % 10, SCREEN_EPD_EXT3_RELEASE % 10));
    y += dy;

    myScreen.gText(x, y, formatString("%8s", "Colours"));
    x += myScreen.characterSizeX() * 8;
    myScreen.setPenSolid(true);
    myScreen.dRectangle(x + dy * 0, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(false);
    myScreen.dRectangle(x + dy * 1, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(true);

    uint8_t number = myScreen.screenColours();

    if (number >= 3)
    {
        myScreen.dRectangle(x + dy * 2, y, dy - 1, dy - 1, myColours.red);

#if defined(WITH_COLOURS_BWRY)
        if (number == 4)
        {
            myScreen.dRectangle(x + dy * 3, y, dy - 1, dy - 1, myColours.yellow);
        }
#endif  // WITH_COLOURS_BWRY
    }

    myScreen.flush();
}

#endif  // DISPLAY_WHOAMI

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
    // myScreen.setPowerProfile(POWER_MODE_AUTO, POWER_SCOPE_GPIO_ONLY);
    myScreen.regenerate();  // Clear buffer and screen

#if (DISPLAY_WHOAMI == 1)

    mySerial.println("DISPLAY_WHOAMI");
    myScreen.clear();
    displayWhoAmI();
    wait(8);

#endif  // DISPLAY_WHOAMI

    mySerial.println("Regenerate");
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
