///
/// @file EXT4_WS2813C.ino
/// @brief Example of features for EXT4 board
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Nov 2024
/// @version 110
///
/// @copyright (c) Rei Vilo, 2010-2024
/// @copyright Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
/// @copyright For exclusive use with Pervasive Displays screens
///
/// @see ReadMe.txt for references
/// @n Step 110 Added list of colours
///

// Screen
#include "Arduino.h"

// WS2813C
#include "ezWS2812gpio.h"

// Set parameters

// Define structures and classes

// Define variables and constants
#include "PDLS_EXT4_Basic_Matter.h"

const pins_t myBoard = boardArduinoNanoMatter;

// WS2813
ezWS2812gpio myRGB(1, myBoard.ledData);

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

        myRGB.set_pixel(workRed, workGreen, workBlue);
        delay(ms);
    }

    mySerial.print(formatString("Step %3i: %3i.%3i.%3i\n", steps, toRed, toGreen, toBlue));
    myRGB.set_pixel(toRed, toGreen, toBlue);

    delay(ms);
    Serial.println("---");
}

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

    myRGB.begin();
    mySerial.println("Black");
    myRGB.set_pixel(0, 0, 0);

    delay(500);
    mySerial.println("White");
    myRGB.set_pixel(21, 21, 21);
    delay(500);
    mySerial.println("Black");
    myRGB.set_pixel(0, 0, 0);
    delay(500);

    mySerial.println("Red");
    myRGB.set_pixel(64, 0, 0);
    delay(500);
    mySerial.println("Yellow");
    myRGB.set_pixel(32, 32, 0);
    delay(500);
    mySerial.println("Green");
    myRGB.set_pixel(0, 64, 0);
    delay(500);
    mySerial.println("Cyan");
    myRGB.set_pixel(0, 32, 32);
    delay(500);
    mySerial.println("Blue");
    myRGB.set_pixel(0, 0, 64);
    delay(500);
    mySerial.println("Violet");
    myRGB.set_pixel(32, 0, 32);
    delay(500);
    mySerial.println("Black");
    myRGB.set_pixel(0, 0, 0);

    // wsFromTo(0, 0, 0, 64, 0, 0, 16, 100);
    // wsFromTo(64, 0, 0, 0, 64, 0, 16, 100);
    // wsFromTo(0, 64, 0, 0, 0, 64, 16, 100);
    // wsFromTo(0, 0, 64, 64, 0, 0, 16, 100);
    // wsFromTo(64, 0, 0, 64, 64, 64, 16, 100);
    // wsFromTo(64, 64, 64, 0, 0, 0, 16, 100);

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

