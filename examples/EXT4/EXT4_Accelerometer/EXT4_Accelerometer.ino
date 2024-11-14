///
/// @file EXT4_Accelerometer.ino
/// @brief Example of features for EXT4 board and sensors
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

// PDLS
#include "PDLS_EXT4_Basic_Matter.h"

#if (PDLS_EXT4_BASIC_MATTER_RELEASE < 810)
#error Required PDLS_EXT4_BASIC_MATTER_RELEASE 810
#endif // PDLS_EXT4_BASIC_MATTER_RELEASE

// SDK
// #include <Arduino.h>
#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

#if (USE_EXT_BOARD != BOARD_EXT4)
#error Required USE_EXT_BOARD = BOARD_EXT4
#endif // USE_EXT_BOARD

// Include application, user and local libraries
// #include <SPI.h>

// Define structures and classes

// Define variables and constants

// PDLS
pins_t myBoard = boardArduinoNanoMatter;
// pins_t myBoard = boardSiLabsBG24Explorer;
Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_290_KS_0F, myBoard);

uint8_t orientation = 9;
uint8_t oldOrientation = 0;
int16_t aX, aY, aZ;

// Prototypes

// Accelerometer
#define ACC_I2C 0x19

uint8_t bufferWrite[8] = { 0 };
uint8_t bufferRead[8] = { 0 };

typedef union
{
    int16_t i16[3];
    uint8_t ui8[6];
} xyz_i16_ui8_t;
xyz_i16_ui8_t myXYZ;

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
    myScreen.regenerate();  // Clear buffer and screen

    // MEMS digital output motion sensor: ultra-low-power high-performance 3-axis "femto" accelerometer
    // https://www.st.com/en/mems-and-sensors/lis2dh12.html
    hV_HAL_Wire_begin();

    bufferWrite[0] = 0x0f; // WhoAmI, should be 0x33
    hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 1, bufferRead, 1);

    mySerial.print(formatString("WhoAmI = 0x%02x", bufferRead[0]));
    mySerial.println();

    bufferWrite[0] = 0x23;
    bufferWrite[1] = 0x80; // Block data update
    hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 2, bufferRead, 0);

    bufferWrite[0] = 0x20;
    // bufferWrite[1] = 0x2f;  // 10 Hz, LPM, XYZ
    bufferWrite[1] = 0x1f;  // 1 Hz, LPM, XYZ
    hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 2, bufferRead, 0);

    bufferWrite[0] = 0x1f;
    bufferWrite[1] = 0x00; // No temperature
    hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 2, bufferRead, 0);

    bufferRead[0] = 0x00;
    while ((bufferRead[0] & 0x0f) != 0x0f)
    {
        bufferWrite[0] = 0x27; // Status
        hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 1, bufferRead, 1);
        mySerial.print(".");
        delay(8);
    }
    mySerial.println();

    pinMode(myBoard.button, INPUT_PULLUP);
}

void loop()
{
    // put your main code here, to run repeatedly:

    bufferWrite[0] = 0x27;  // Status
    bufferRead[0] = 0x00;
    hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 1, bufferRead, 1);

    if ((bufferRead[0] & 0x0f) == 0x0f)
    {

        bufferWrite[0] = 0xa8;  // xyz, MSB
        bufferRead[0] = 0x00;

        hV_HAL_Wire_transfer(ACC_I2C, bufferWrite, 1, myXYZ.ui8, 6);
        aX = myXYZ.i16[0] / 16;
        aY = myXYZ.i16[1] / 16;
        aZ = myXYZ.i16[2] / 16;

        mySerial.println();
        mySerial.print(formatString("x %5i, y %5i, z %5i", myXYZ.i16[0] / 16, myXYZ.i16[1] / 16, myXYZ.i16[2] / 16));

        oldOrientation = orientation;
        if (oldOrientation == 9) // Default
        {
            orientation = 3;
        }

        // Tilt Sensing Using a Three-Axis Accelerometer
        // https://www.nxp.com/docs/en/application-note/AN3461.pdf
        if (abs(aZ) < 900)
        {
            if ((aX > 500) and (abs(aY) < 400))
            {
                orientation = 2;
            }
            else if ((aX < -500) and (abs(aY) < 400))
            {
                orientation = 0;
            }
            else if ((aY > 500) and (abs(aX) < 400))
            {
                orientation = 3;
            }
            else if ((aY < -500) and (abs(aX) < 400))
            {
                orientation = 1;
            }
        }
        mySerial.print(formatString(", o %i -> %i", oldOrientation, orientation));

        if (orientation != oldOrientation) // Update display
        {
            myScreen.clear();
            myScreen.setOrientation(orientation);
            myScreen.selectFont(Font_Terminal12x16);
            myScreen.gText(0, 0, "Orientation");
            uint16_t dt = myScreen.characterSizeY();
            myScreen.selectFont(Font_Terminal16x24);
            myScreen.gText(0, dt, formatString("%i", orientation));
            myScreen.gText(1, dt, formatString("%i", orientation));

            uint16_t x = myScreen.screenSizeX();
            uint16_t y = myScreen.screenSizeY();
            uint16_t dx = x / 2;
            uint16_t dy = y / 2;
            uint16_t dz = hV_HAL_min(dx, dy) / 3;

            myScreen.setPenSolid(true);
            myScreen.triangle(dx, dy - dz, dx + dz, dy, dx - dz, dy, myColours.black);
            myScreen.rectangle(dx + dz / 3, dy, dx - dz / 3, dy + dz, myColours.black);

            myScreen.setPenSolid(false);
            myScreen.circle(dx, dy, dz  + 8, myColours.black);

            myScreen.flush();
            mySerial.print(" *");
        }
    }
    else
    {
        mySerial.print(".");
        delay(8);
    }

    if (digitalRead(myBoard.button) == LOW)
    {
        myScreen.regenerate();
        while (true)
        {
            delay(1000);
        }
    }

    delay(1000);
}
