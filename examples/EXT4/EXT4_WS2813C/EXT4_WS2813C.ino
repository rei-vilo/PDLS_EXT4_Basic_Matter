///
/// @file EXT4_WS2813C.ino
/// @brief Example of features for EXT4 board
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 12 Dec 2024
/// @version 811
///
/// @copyright (c) Rei Vilo, 2010-2024
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
/// @n Step 110 Added list of colours
///

// Screen
#include "Arduino.h"

// WS2813C
// #include "ezWS2812gpio.h"
#include "rawWS2813C.h"

// Set parameters

// Define structures and classes

// Define variables and constants
#include "PDLS_EXT4_Basic_Matter.h"

const pins_t myBoard = boardArduinoNanoMatter;

// WS2813
// ezWS2812gpio myRGB(1, myBoard.ledData);
rawWS2813C myRGB(1, myBoard.ledData);

// Prototypes

// Functions
void wsFromTo(uint8_t fromRed, uint8_t fromGreen, uint8_t fromBlue,
              uint8_t toRed, uint8_t toGreen, uint8_t toBlue,
              uint8_t steps, uint16_t ms)
{
    mySerial.print(formatString("From %3i.%3i.%3i to %3i.%3i.%3i in %4i steps and %6i ms\n", fromRed, fromGreen, fromBlue,
                                toRed, toGreen, toBlue,
                                steps, ms));
    uint16_t workRed, workGreen, workBlue;

    for (uint8_t i = 0; i < steps; i += 1)
    {
        workRed = fromRed + (toRed - fromRed) * i / steps;
        workGreen = fromGreen + (toGreen - fromGreen) * i / steps;
        workBlue = fromBlue + (toBlue - fromBlue) * i / steps;
        mySerial.print(formatString("Step %3i: %3i.%3i.%3i\n", i, workRed, workGreen, workBlue));

        // Release 2.2.0 replaces set_pixel() by set_all()
        myRGB.set_all(workRed, workGreen, workBlue);
        delay(ms);
    }

    mySerial.print(formatString("Step %3i: %3i.%3i.%3i\n", steps, toRed, toGreen, toBlue));
    myRGB.set_all(toRed, toGreen, toBlue);

    delay(ms);
    Serial.println("---");
}

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

    myRGB.begin();
    mySerial.println("Black");
    myRGB.set_all(0, 0, 0);

    delay(500);
    mySerial.println("White");
    myRGB.set_all(21, 21, 21);
    delay(500);
    mySerial.println("Black");
    myRGB.set_all(0, 0, 0);
    delay(500);

    mySerial.println("Red");
    myRGB.set_all(64, 0, 0);
    delay(500);
    mySerial.println("Yellow");
    myRGB.set_all(32, 32, 0);
    delay(500);
    mySerial.println("Green");
    myRGB.set_all(0, 64, 0);
    delay(500);
    mySerial.println("Cyan");
    myRGB.set_all(0, 32, 32);
    delay(500);
    mySerial.println("Blue");
    myRGB.set_all(0, 0, 64);
    delay(500);
    mySerial.println("Violet");
    myRGB.set_all(32, 0, 32);
    delay(500);
    mySerial.println("Black");
    myRGB.set_all(0, 0, 0);

    // wsFromTo(0, 0, 0, 64, 0, 0, 16, 100);
    // wsFromTo(64, 0, 0, 0, 64, 0, 16, 100);
    // wsFromTo(0, 64, 0, 0, 0, 64, 16, 100);
    // wsFromTo(0, 0, 64, 64, 0, 0, 16, 100);
    // wsFromTo(64, 0, 0, 64, 64, 64, 16, 100);
    // wsFromTo(64, 64, 64, 0, 0, 0, 16, 100);

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

