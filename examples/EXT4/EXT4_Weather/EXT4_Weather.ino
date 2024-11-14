///
/// @file EXT4_Weather.ino
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
/// * 2024-06-06 Rei Vilo (Pervasive Displays)
/// * Added support for EXT4
/// @copyright Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
/// @copyright For exclusive use with Pervasive Displays screens
///
/// @see
/// * HDC2080 2% RH ultra-low-power digital relative humidity sensor, interrupt/DRDY
/// https://www.ti.com/product/HDC2080
/// * Weather Sensors Library
/// https://github.com/rei-vilo/SensorsWeather_Library
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
// --- Fast
pins_t myBoard = boardArduinoNanoMatter;
// pins_t myBoard = boardSiLabsBG24Explorer;
Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_290_KS_0F, myBoard);

// HDC2080
#include "Wire.h"

#define HDC_I2C 0x40

uint8_t bufferWrite[8] = { 0 };
uint8_t bufferRead[8] = { 0 };

struct measure_s
{
    float value;
    float oldValue = 999.9;
    float minimum = 999.9;
    float maximum = -999.9;
};

measure_s temperature;
measure_s humidity;

static uint32_t chrono32 = 0;
static uint8_t countFlush = 1; // Counter for global update
const uint8_t FAST_BEFORE_GLOBAL = 8; // Number of fast updates before golbal update
bool flagDisplay = true;

// Prototypes
bool displayValue(uint8_t slot, String name, measure_s * value, String unit);

// Functions
// --- Screen
bool displayValue(uint8_t slot, String name, measure_s * value, String unit)
{
    uint16_t x = myScreen.screenSizeX();
    uint16_t y = myScreen.screenSizeY();
    uint16_t dx, dy, x0, y0;

    x0 = x * slot / 2;
    dx = x / 8;
    y0 = 0;
    dy = y / 5;

    (*value).value = ((int32_t)(10 * (*value).value + 5)) / 10.0;
    bool result = ((*value).value != (*value).oldValue);
    (*value).oldValue = (*value).value;
    (*value).maximum = hV_HAL_max((*value).maximum, (*value).value);
    (*value).minimum = hV_HAL_min((*value).minimum, (*value).value);

    myScreen.setPenSolid(true);
    myScreen.setFontSolid(true);
    myScreen.dRectangle(x0, y0, dx * 4, dy * 4, myColours.white);

    myScreen.selectFont(Font_Terminal12x16);
    myScreen.gText(x0, y0, name);

    myScreen.selectFont(Font_Terminal16x24);
    myScreen.gTextLarge(x0, y0 + 1 * dy, formatString("%5.1f", (*value).value));

    myScreen.selectFont(Font_Terminal12x16);
    char unit_c[4] = {0};
    strcpy(unit_c, utf2iso(unit).c_str());
    myScreen.gText(x0 + 3 * dx - myScreen.characterSizeX() * 0, y0 + 1 * dy - myScreen.characterSizeY(), formatString("%s", unit_c));

    myScreen.selectFont(Font_Terminal8x12);
    myScreen.gText(x0, y0 + 3 * dy, "Minimum");
    myScreen.gText(x0 + 2 * dx, y0 + 3 * dy, "Maximum");

    myScreen.selectFont(Font_Terminal12x16);
    myScreen.gText(x0, y0 + 4 * dy, formatString("%5.1f", (*value).minimum));
    myScreen.gText(x0 + 2 * dx, y0 + 4 * dy, formatString("%5.1f", (*value).maximum));

    // for (uint8_t i = 0; i < 4; i += 1)
    // {
    //     myScreen.line(x0, y0 + i * dy, x0 + x, y0 + i * dy, myColours.grey);
    //     myScreen.line(x0 + i * dx, y0, x0 + i * dx, y0 + y, myColours.grey);
    // }

    myScreen.setPenSolid(false);
    return result;
}
// --- End of Screen

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
    myScreen.regenerate(); // Clear buffer and screen

    myScreen.setOrientation(3);

    // --- HDC2080
    hV_HAL_Wire_begin();

    bufferWrite[0] = 0x0e; // HDC20X0_CONFIGURATION
    bufferWrite[1] = 0x00; // HDC20X0_DEFAULT_SETTINGS
    hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 2, bufferRead, 0);

    bufferWrite[0] = 0x0f; // HDC20X0_MEASURE_CONFIGURATION
    bufferWrite[1] = 0xfe; // HDC20X0_MEASURE_SETTINGS
    hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 2, bufferRead, 0);
    // --- End of HDC2080
}

void loop()
{
    if (millis() > chrono32)
    {
        // HDC2080
        bufferWrite[0] = 0x0f; // HDC20X0_MEASURE_CONFIGURATION
        bufferWrite[1] = 0xff; // HDC20X0_DEFAULT_SETTINGS
        hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 2, bufferRead, 0);
        delay(50);

        // Temperature
        bufferWrite[0] = 0x00; // HDC20X0_TEMPERATURE_LOW
        hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 1, bufferRead, 2);
        temperature.value = bufferRead[0] + bufferRead[1] * 256.0;
        // Standard equation 1
        // temperature.value = temperature.value * 165.0 / 65536.0 - 40.0; // +273.15; // from °C to °K
        //
        // Highest accuracy with equation 2. Impact = (3.3 - 1.8) * 0.08 = 0.12°C
        temperature.value = (temperature.value * 165.0 / 65536.0) - (40.5 + 0.08 * (3.3 - 1.8)); // +273.15; // from °C to °K

        // Humidity
        bufferWrite[0] = 0x02; // HDC20X0_HUMIDITY_LOW
        hV_HAL_Wire_transfer(HDC_I2C, bufferWrite, 1, bufferRead, 2);
        humidity.value = bufferRead[0] + bufferRead[1] * 256.0;
        humidity.value = humidity.value / 65536.0 * 100.0;

        // Screen
        countFlush %= FAST_BEFORE_GLOBAL;
        if (countFlush == 0)
        {
            myScreen.regenerate();
            mySerial.println("countFlush = 0 ! regenerate");
            flagDisplay = true;
        }

        flagDisplay |= displayValue(0, "Temperature", &temperature, "°C");
        flagDisplay |= displayValue(1, "Humidity", &humidity, "%");
        if (flagDisplay == true)
        {
            countFlush += 1;
            myScreen.flush();

            flagDisplay = false;

            mySerial.print(formatString("countFlush = %i", countFlush));
            mySerial.println();
        }

        // Serial
        mySerial.print(formatString("Temperature = %5.1f < %5.1f < %5.1f oC, Humidity= %5.1f < %5.1f < %5.1f %%",
                                    temperature.minimum, temperature.value, temperature.maximum,
                                    humidity.minimum, humidity.value, humidity.maximum));
        mySerial.println();

        chrono32 = millis() + 10000; // 10 s
    }

    delay(100);
}

