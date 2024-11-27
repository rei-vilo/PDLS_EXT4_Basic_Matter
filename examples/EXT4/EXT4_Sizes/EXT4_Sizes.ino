///
/// @file EXT4_Sizes.ino
/// @brief Example of features for basic edition
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
#define DISPLAY_SIZES 1

// Define structures and classes

// Define variables and constants
// PDLS
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_152_KS_0J, boardArduinoNanoMatter);
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_206_KS_0E, boardArduinoNanoMatter);
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_213_KS_0E, boardArduinoNanoMatter);
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_266_KS_0C, boardArduinoNanoMatter);
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_271_KS_0C, boardArduinoNanoMatter);
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
#if (DISPLAY_SIZES == 1)

///
/// @brief Displays x.y size
///
void displayWhoAmI()
{
    myScreen.setOrientation(ORIENTATION_LANDSCAPE);
    myScreen.selectFont(fontMedium);

    uint16_t x = 4;
    uint16_t y = 4;
    uint16_t dy = myScreen.characterSizeY();

    myScreen.gText(x, y, formatString("x %i", myScreen.screenSizeX()));
    y += dy;
    myScreen.gText(x, y, formatString("y %i", myScreen.screenSizeY()));
    y += dy;

    myScreen.setPenSolid(true);
    myScreen.dRectangle(x + dy * 0, y, dy - 1, dy - 1, myColours.black);
    myScreen.setPenSolid(false);
    myScreen.dRectangle(x + dy * 1, y, dy - 1, dy - 1, myColours.black);

    myScreen.selectFont(fontSmall);
    char * text = myScreen.screenNumber();
    x = myScreen.screenSizeX() - myScreen.stringSizeX(text) - 4;
    y = myScreen.screenSizeY() - myScreen.characterSizeY() - 4;
    myScreen.gText(x, y, text);

    myScreen.flush();
}

#endif // DISPLAY_SIZES

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

#if (DISPLAY_SIZES == 1)

    mySerial.println("DISPLAY_SIZES");
    myScreen.clear();
    displayWhoAmI();
    wait(8);

#endif // DISPLAY_SIZES

    mySerial.println("Regenerate");
    myScreen.regenerate();

    mySerial.println("=== ");
    mySerial.println();
}

///
/// @brief Loop, empty
///
void loop()
{
    delay(1000);
}
