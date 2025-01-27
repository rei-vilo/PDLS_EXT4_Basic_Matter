///
/// @file EXT4_ScreenSizes.ino
/// @brief Example of features for EXT4 board and sensors
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Jan 2025
/// @version 812
///
/// @copyright (c) Rei Vilo, 2010-2025
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
///

// Set parameters
#define DISPLAY_SIZES 1
#define mySerial Serial

// PDLS
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

#if (USE_EXT_BOARD != BOARD_EXT4)
#error Required USE_EXT_BOARD = BOARD_EXT4
#endif // USE_EXT_BOARD

// Define structures and classes

// Define variables and constants
// Board
pins_t myBoard = boardArduinoNanoMatter;
// pins_t myBoard = boardSiLabsBG24Explorer;

// Screen
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_152_KS_0J, myBoard);
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_206_KS_0E, myBoard);
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_213_KS_0E, myBoard);
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_266_KS_0C, myBoard);
// Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_271_KS_0C, myBoard);
Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_290_KS_0F, myBoard);

// Fonts
uint8_t fontSmall, fontMedium, fontLarge, fontVery;

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
    String text = myScreen.screenNumber();
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

    // Screen
    myScreen.begin();
    // myScreen.setPowerProfile(POWER_MODE_AUTO, POWER_SCOPE_GPIO_ONLY);
    myScreen.regenerate(); // Clear buffer and screen

    // Fonts
#if (FONT_MODE == USE_FONT_TERMINAL)

    fontSmall = Font_Terminal6x8;
    fontMedium = Font_Terminal8x12;
    fontLarge = Font_Terminal12x16;
    fontVery = Font_Terminal16x24;

#else // FONT_MODE

    fontSmall = myScreen.addFont(Font_DejaVuSans12);
    fontSmall -= ((fontSmall > 0) ? 1 : 0);
    fontMedium = myScreen.addFont(Font_DejaVuSans16);
    fontMedium -= ((fontMedium > 0) ? 1 : 0);
    fontLarge = myScreen.addFont(Font_DejaVuSans24);
    fontLarge -= ((fontLarge > 0) ? 1 : 0);
    fontVery = myScreen.addFont(Font_DejaVuMono48);
    fontVery -= ((fontVery > 0) ? 1 : 0);

#endif // FONT_MODE

    // Example
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
