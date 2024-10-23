///
/// @file EXT4_Matter_RGB.ino
/// @brief Example of Matter device for EXT4 RGB LED
///
/// @details Based on Matter color lightbulb example
///
/// The example shows the basic usage of the Arduino Matter API.
///
/// The example lets users control a WS2812 LED strip/ring acting as a Matter lightbulb.
/// It's possible to switch the LEDs on/off, adjust the brightness and the color as well.
/// The device has to be commissioned to a Matter hub first.
/// A WS2812 (NeoPixel) strip/ring/device needs to be connected to the board to the SPI SDO.
///
/// Compatible boards:
/// - Arduino Nano Matter
/// - SparkFun Thing Plus MGM240P
/// - xG24 Explorer Kit
/// - xG24 Dev Kit
///
/// @author Tamas Jozsi (Silicon Labs)
///
/// @author Rei Vilo
/// @date 21 Aug 2024
/// @version 110
///
/// @copyright (c) Rei Vilo, 2010-2024
/// * 2024-06-06 Rei Vilo (Pervasive Displays)
/// * Added support for EXT4
/// @copyright Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
/// @copyright For exclusive use with Pervasive Displays screens
///

// Checks: Silicon Labs Matter only
#ifndef ARDUINO_ARCH_SILABS
#error Silicon Labs architecture required
#endif

#ifndef ARDUINO_MATTER
#error Matter library required
#endif

#include <Matter.h>
#include <MatterLightbulb.h>

MatterColorLightbulb myMatterRGB;

// WS2813C
#include "ezWS2812gpio.h"

// PDLS
#include "PDLS_EXT4_Basic_Matter.h"

#if (PDLS_EXT4_BASIC_MATTER_RELEASE < 805)
#error Required PDLS_EXT4_BASIC_MATTER_RELEASE 805
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

// --- Fast
pins_t myBoard = boardArduinoNanoMatter;
// pins_t myBoard = boardSiLabsBG24Explorer;
Screen_EPD_EXT4_Fast myScreen(eScreen_EPD_290_KS_0F, myBoard);

#define MATTER_EXAMPLE_NAME "Matter RGB"
#define MATTER_EXAMPLE_RELEASE 109

// WS2813
ezWS2812gpio myRGB(1, myBoard.ledData);

static bool wsState = false;
const uint8_t wsLimit = 64; // Limit for each RGB channel

static uint8_t countFlush = 1; // Counter for global update
const uint8_t FAST_BEFORE_GLOBAL = 8; // Number of fast updates before global update
bool flagDisplay = true; // Call screen refresh

struct hsb_s
{
    uint8_t hue = 0;
    uint8_t saturation = 0;
    uint8_t brightness = 0;
};

hsb_s currentHSB;
hsb_s oldHSB;

// Prototypes

///
/// @brief Display the About page
/// @see https://www.pervasivedisplays.com
///
void displayAbout();

///
/// @brief Display the QR-code
///
/// @param code text
/// * Matter address starting with `MT:`
/// * IP address starting with `https://`
/// @see https://github.com/ricmoo/qrcode/
///
void displayQR(const char * code);

///
/// @brief Manage and display the RGB LED
///
/// @param flag default = true = update LED, false otherwise
///
void displayValue(bool flag = true);

///
/// @brief Display a menu managed by a single button
///
/// @param button GPIO of the button
/// @param title text for title
/// @param option1 text for option 0, required
/// @param option2 text for option 1, optional
/// @param option3 text for option 2, optional
/// @param option4 text for option 3, optional
/// @return number of the selected option
/// @note Procedure
/// * Press and hold the button to choose the option
/// * A progress bar shows the selected option
/// * Release the button to select the option
/// @warning button should be initialised before calling menuOneButton()
///
uint8_t menuOneButton(uint8_t button, const char * title, const char * option1, const char * option2 = 0, const char * option3 = 0, const char * option4 = 0);

///
/// @brief Display QR-code
/// @param code text
///
void displayQR(const char * code);

///
/// @brief Display the commission process
///
void displayCommissioning();

///
/// @brief Display the decommission process
/// @see https://docs.arduino.cc/tutorials/nano-matter/user-manual/#device-decommissioning
///
void displayDecommissioning();

///
/// @brief Display the identification
///
void displayIdenfication();

// Functions

// --- QR-code
#include "qrcode.h"

void displayQR(const char * code)
{
    // Create the QR code
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    qrcode_initText(&qrcode, qrcodeData, 3, 0, code);

    uint16_t x = myScreen.screenSizeX();
    uint16_t y = myScreen.screenSizeY();
    uint8_t k = qrcode.size;
    uint16_t dxy = min(x, y);
    uint16_t dz = dxy / k;
    uint16_t dxy0 = (dxy - k * dz) / 2;
    uint16_t dx0 = x - dxy + dxy0;
    uint16_t dy0 = 0 + dxy0;

    myScreen.setPenSolid(true);
    myScreen.dRectangle(x - dxy, 0, dxy, dxy, myColours.white);

    for (uint8_t jy = 0; jy < k; jy++)
    {
        for (uint8_t ix = 0; ix < k; ix++)
        {
            uint16_t colour = qrcode_getModule(&qrcode, ix, jy) ? myColours.black : myColours.white;
            myScreen.dRectangle(dx0 + dz * ix, dy0 + dz * jy, dz, dz, colour);
        }
    }
    myScreen.setPenSolid(false);
}
// --- End of QR-code

// --- Screen
void displayValue(bool flag)
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    myMatterRGB.get_rgb(&r, &g, &b);
    if ((r < 4) and (g < 4) and (b < 4)) // Fix rounding error
    {
        r = 0;
        g = 0;
        b = 0;
    }

    if (flag)
    {
        if (wsState)
        {
            uint8_t wsRed = r * wsLimit / 255;
            uint8_t wsGreen = g * wsLimit / 255;
            uint8_t wsBlue = b * wsLimit / 255;
            myRGB.set_pixel(wsRed, wsGreen, wsBlue);
            mySerial.print(formatString("Setting bulb color to > r: %u  g: %u  b: %u", r, g, b));
            mySerial.println();
        }
        else
        {
            myRGB.set_pixel(0, 0, 0);
        }
    }

    myScreen.setPenSolid();
    myScreen.setFontSolid();

    uint16_t x = myScreen.screenSizeX();
    uint16_t y = myScreen.screenSizeY();
    uint16_t dx, dy, x0, y0;

    x0 = 0;
    dx = x / 6;
    y0 = 0;
    dy = y / 7;

    myScreen.selectFont(Font_Terminal12x16);

    myScreen.gText(x0, y0, MATTER_EXAMPLE_NAME);
    y += dy * 2;

    myScreen.gTextLarge(x - myScreen.characterSizeX() * 6, y0, (myMatterRGB.get_onoff() ? " ON" : "OFF"));

    myScreen.selectFont(Font_Terminal8x12);
    myScreen.gText(x0 + dx * 0, y0 + dy * 1, "Red");
    myScreen.gText(x0 + dx * 2, y0 + dy * 1, "Green");
    myScreen.gText(x0 + dx * 4, y0 + dy * 1, "Blue");

    myScreen.gText(x0 + dx * 2 - myScreen.characterSizeX() * 2, y0 + dy * 2 - myScreen.characterSizeY(), "%");
    myScreen.gText(x0 + dx * 4 - myScreen.characterSizeX() * 2, y0 + dy * 2 - myScreen.characterSizeY(), "%");
    myScreen.gText(x0 + dx * 6 - myScreen.characterSizeX() * 2, y0 + dy * 2 - myScreen.characterSizeY(), "%");

    myScreen.selectFont(Font_Terminal12x16);
    myScreen.gTextLarge(x0 + dx * 0, y0 + dy * 2, formatString("%4i", r * 100 / 256));
    myScreen.gTextLarge(x0 + dx * 2, y0 + dy * 2, formatString("%4i", g * 100 / 256));
    myScreen.gTextLarge(x0 + dx * 4, y0 + dy * 2, formatString("%4i", b * 100 / 256));

    myScreen.selectFont(Font_Terminal8x12);
    myScreen.gText(x0 + dx * 0, y0 + dy * 4, "Hue");
    myScreen.gText(x0 + dx * 2, y0 + dy * 4, "Saturation");
    myScreen.gText(x0 + dx * 4, y0 + dy * 4, "Brightness");

    myScreen.gText(x0 + dx * 2 - myScreen.characterSizeX() * 2, y0 + dy * 5 - myScreen.characterSizeY(), utf2iso("°"));
    myScreen.gText(x0 + dx * 4 - myScreen.characterSizeX() * 2, y0 + dy * 5 - myScreen.characterSizeY(), "%");
    myScreen.gText(x0 + dx * 6 - myScreen.characterSizeX() * 2, y0 + dy * 5 - myScreen.characterSizeY(), "%");

    myScreen.selectFont(Font_Terminal12x16);
    myScreen.gTextLarge(x0 + dx * 0, y0 + dy * 5, formatString("%4i", myMatterRGB.get_hue() * 360 / 256));
    myScreen.gTextLarge(x0 + dx * 2, y0 + dy * 5, formatString("%4i", myMatterRGB.get_saturation_percent()));
    myScreen.gTextLarge(x0 + dx * 4, y0 + dy * 5, formatString("%4i", myMatterRGB.get_brightness_percent()));
}

void displayAbout()
{
    myScreen.clear();

    myScreen.selectFont(Font_Terminal12x16);
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t dy = myScreen.characterSizeY();

    mySerial.println(MATTER_EXAMPLE_NAME);
    myScreen.gText(x, y, MATTER_EXAMPLE_NAME);
    y += dy * 2;
    myScreen.selectFont(Font_Terminal8x12);

    myScreen.gText(x, y, formatString("%8s v%i.%i.%i", "Release", MATTER_EXAMPLE_RELEASE / 100, (MATTER_EXAMPLE_RELEASE / 10) % 10, MATTER_EXAMPLE_RELEASE % 10));
    y += dy;
    myScreen.gText(x, y, formatString("%8s pin %i", "Switch", myBoard.button));
    y += dy;

    myScreen.gText(x, y, formatString("%8s %s", "Screen", myScreen.WhoAmI().c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %i x %i", "Size", myScreen.screenSizeX(), myScreen.screenSizeY()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %s", "Number", myScreen.screenNumber().c_str()));
    y += dy;
    myScreen.gText(x, y, formatString("%8s %s v%i.%i.%i", "PDLS", SCREEN_EPD_EXT3_VARIANT, SCREEN_EPD_EXT3_RELEASE / 100, (SCREEN_EPD_EXT3_RELEASE / 10) % 10, SCREEN_EPD_EXT3_RELEASE % 10));
    y += dy;
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
#endif // WITH_COLOURS_BWRY
    }

    y += dy;
    myScreen.gText(x, y, "Wait 10 seconds...");

    displayQR("https://www.pervasivedisplays.com");

    myScreen.flush();
    delay(10000);
    myScreen.clear();
}
// --- End of Screen

void displayIdenfication()
{
    pinMode(LED_BUILTIN, OUTPUT);
    static bool oldIdentification = false;
    bool flagIdentification = myMatterRGB.get_identify_in_progress();
    // Handle the identification enable/disable transitions
    if ((flagIdentification == true) and (oldIdentification == false))
    {
        // Identification starts
        mySerial.println("Identification started");
    }
    else if ((flagIdentification == false) and (oldIdentification == true))
    {
        // Identification ends
        // Turn off the LED once the identification ends
        digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
        mySerial.println("Identification ended");
    }
    oldIdentification = flagIdentification;

    // Blink the LED while the identification is in progress
    if (flagIdentification)
    {
        if (millis() % 1000 < 500)
        {
            digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
        }
        else
        {
            digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE);
        }
    }
}

// --- Menu
uint8_t menuOneButton(uint8_t button, const char * title, const char * option1, const char * option2, const char * option3, const char * option4)
{
    uint8_t result = 0;

    if (digitalRead(button) == LOW)
    {
        uint8_t delay_s = 2; // 2 seconds
        uint8_t options_max = 4;

        uint16_t x = myScreen.screenSizeX();
        uint16_t y = myScreen.screenSizeY();

        myScreen.selectFont(Font_Terminal12x16);
        uint16_t options_dy = myScreen.characterSizeY();
        myScreen.selectFont(Font_Terminal8x12);
        uint16_t title_dy = myScreen.characterSizeY();

        // From top to bottom
        // area_y0, +2, title_dy, +4, options_dy
        uint16_t area_y0 = y - (options_dy + 4 + title_dy + 2);
        myScreen.selectFont(Font_Terminal8x12);
        myScreen.setPenSolid(true);
        myScreen.rectangle(0, area_y0, x, y, myColours.white);
        myScreen.dRectangle(0, area_y0, x, 1, myColours.black);
        myScreen.setPenSolid(false);
        myScreen.gText(0, area_y0 + 2, title);

        uint16_t options_dx = x / options_max;
        myScreen.selectFont(Font_Terminal12x16);
        uint16_t options_y0 = y - options_dy;
        uint16_t bar_y0 = options_y0 - 4;

        uint8_t options_count = 0;
        myScreen.gText(options_count * options_dx, options_y0, option1);
        options_count += 1;

        if (option2 != 0)
        {
            myScreen.gText(options_count * options_dx, options_y0, option2);
            options_count += 1;

            if (option3 != 0)
            {
                myScreen.gText(options_count * options_dx, options_y0, option3);
                options_count += 1;

                if (option4 != 0)
                {
                    myScreen.gText(options_count * options_dx, options_y0, option4);
                    options_count += 1;
                }
            }
        }

        uint8_t options_index = 0;
        uint8_t delay_count = 0;

        uint32_t chronoSwitch = 0;

        while (digitalRead(myBoard.button) == LOW)
        {
            if (millis() - chronoSwitch > 1000)
            {
                if (delay_count == delay_s)
                {
                    options_index += 1;
                    options_index %= options_count;
                    delay_count = 0;
                }

                delay_count += 1;
                chronoSwitch = millis();

                myScreen.setPenSolid(true);
                myScreen.dRectangle(0, bar_y0, x, 4, myColours.white);
                myScreen.dRectangle(options_index * options_dx, bar_y0, options_dx * delay_count / delay_s, 2, myColours.black);
                myScreen.setPenSolid(false);
                myScreen.flush();
            }
        }
        result = options_index + 1;

        myScreen.setPenSolid(true);
        myScreen.rectangle(0, area_y0, x, y, myColours.white);
        // myScreen.flush();
    }

    return result;
}
// --- End of Menu

// --- Matter
void displayCommissioning()
{
    myScreen.selectFont(Font_Terminal12x16);
    uint16_t y = 0;
    uint16_t dy = myScreen.characterSizeY();

    myScreen.gText(0, y, MATTER_EXAMPLE_NAME);
    y += dy * 2;
    myScreen.flush();
    myScreen.selectFont(Font_Terminal8x12);

    mySerial.println("Matter device not commissioned");
    myScreen.gText(0, y, "Device not commissioned");
    y += dy;

    mySerial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    myScreen.gText(0, y, "Commission with");
    y += dy;
    mySerial.print(formatString("Manual pairing code: %s", Matter.getManualPairingCode().c_str()));
    mySerial.println();
    myScreen.gText(0, y, ". Manual pairing code");
    y += dy;
    myScreen.gText(0, y, Matter.getManualPairingCode());
    y += dy;
    mySerial.print(formatString("QR code URL: %s", Matter.getOnboardingQRCodeUrl().c_str()));
    mySerial.println();

    // // Get code URL
    // // Remove http address
    // char * buffer = strstr(Matter.getOnboardingQRCodeUrl().c_str(), "MT%3A") + 5 - 3;
    //
    // // Replace MT%3A by MT:
    // buffer[0] = 'M';
    // buffer[1] = 'T';
    // buffer[2] = ':';

    myScreen.gText(0, y, ". Scan QR-code");
    y += dy;
    // displayQR(buffer);
    displayQR(Matter.getOnboardingQRCodePayload().c_str());
    myScreen.flush();
}

void displayDecommissioning()
{
    myScreen.clear();
    myScreen.selectFont(Font_Terminal12x16);
    uint16_t y = 0;
    uint16_t dy = myScreen.characterSizeY();

    myScreen.gText(0, y, MATTER_EXAMPLE_NAME);
    y += dy * 2;
    myScreen.flush();
    myScreen.selectFont(Font_Terminal8x12);

    mySerial.println("Decommissioning");
    myScreen.gText(0, y, "Decommissioning");
    y += dy;
    myScreen.flush();

    if (!Matter.isDeviceCommissioned())
    {
        mySerial.println(". Already done");
        myScreen.gText(0, y, ". Already done");
        y += dy;
    }
    else
    {
        mySerial.println(". Starting");
        myScreen.gText(0, y, ". Starting");
        y += dy;
        // nvm3_eraseAll(nvm3_defaultHandle); // Decomission command
        Matter.decommission(); // 2.1.0
        mySerial.println(". Done");
        myScreen.gText(0, y, ". Done");
        y += dy;
    }
    mySerial.println("Restart the device");
    myScreen.gText(0, y, "Restart the device");
    myScreen.flush();
}
// --- End of Matter

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
    myScreen.setOrientation(3);
    myScreen.regenerate(); // Clear buffer and screen

    // --- WS2812
    myRGB.begin();
    myRGB.set_pixel(0, 0, 0);
    // --- End of WS2812

    // --- Matter
    pinMode(myBoard.button, INPUT_PULLUP);
    delay(100);
    if (digitalRead(myBoard.button) == LOW)
    {
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        Matter.begin();
        displayDecommissioning();

        while (true)
        {
            delay(500);
            digitalWrite(LED_BUILTIN, 1 - digitalRead(LED_BUILTIN));
        }
    }

    Matter.begin();
    myMatterRGB.begin();
    myMatterRGB.boost_saturation(51); // Boost saturation by 20 percent

    if (!Matter.isDeviceCommissioned())
    {
        displayCommissioning();
    }

    while (!Matter.isDeviceCommissioned())
    {
        delay(200);
    }

    myScreen.clear();
    myScreen.selectFont(Font_Terminal12x16);
    uint16_t y = 0;
    uint16_t dy = myScreen.characterSizeY();

    mySerial.println(MATTER_EXAMPLE_NAME);
    myScreen.gText(0, y, MATTER_EXAMPLE_NAME);
    y += dy * 2;
    myScreen.flush();
    myScreen.selectFont(Font_Terminal8x12);

    mySerial.println("Thread network");
    myScreen.gText(0, y, "Thread network");
    y += dy;

    mySerial.println(". Waiting");
    myScreen.gText(0, y, ". Waiting");
    y += dy;
    myScreen.flush();

    while (!Matter.isDeviceThreadConnected())
    {
        delay(200);
    }
    mySerial.println(". Connected");
    myScreen.gText(0, y, ". Connected");
    y += dy;
    myScreen.flush();

    mySerial.println("Matter device discovery");
    myScreen.gText(0, y, "Matter discovery");
    y += dy;

    mySerial.println(". Waiting");
    myScreen.gText(0, y, ". Waiting");
    y += dy;
    myScreen.flush();

    while (!myMatterRGB.is_online())
    {
        delay(200);
    }
    mySerial.println(". Device online");
    myScreen.gText(0, y, ". Device online");
    y += dy;
    myScreen.flush();

    delay(1000);
    // --- End of Matter

    myScreen.regenerate();
    displayValue(false);
}

void loop()
{
    displayIdenfication();

    // --- Menu
    uint8_t action = menuOneButton(myBoard.button, "RGB menu", "Toggle", "Exit", "Cancel", "About");

    if (action > 0)
    {
        flagDisplay = true;
    }

    switch (action)
    {
        case 1:

            mySerial.println("> Toggle");
            myMatterRGB.toggle();
            break;

        case 2:

            mySerial.println("> Exit");
            myRGB.set_pixel(0, 0, 0);
            myScreen.regenerate();
            while (1)
            {
                delay(1000);
            }
            break;

        case 3:

            mySerial.println("> Cancel");
            break;

        case 4:

            displayAbout();
            break;

        default:

            break;
    }
    // --- End of Menu

    static bool oldState = false;
    wsState = myMatterRGB.get_onoff();

    // If the current state is ON and the previous was OFF - turn on the LED
    if ((wsState == HIGH) and (oldState == LOW))
    {
        oldState = wsState;
        mySerial.println("Bulb ON");
        // Set the LEDs to the last received state
        flagDisplay = true;
    }

    // If the current state is OFF and the previous was ON - turn off the LED
    if ((wsState == LOW) and (oldState == HIGH))
    {
        oldState = wsState;
        mySerial.println("Bulb OFF");
        flagDisplay = true;
    }

    currentHSB.hue = myMatterRGB.get_hue();
    currentHSB.saturation = myMatterRGB.get_saturation_percent();
    currentHSB.brightness = myMatterRGB.get_brightness_percent();

    // If either the hue, saturation or the brightness changes - update the LEDs to reflect the latest change
    if ((oldHSB.hue != currentHSB.hue) or (oldHSB.saturation != currentHSB.saturation) or (oldHSB.brightness != currentHSB.brightness))
    {
        flagDisplay = true;
        oldHSB = currentHSB;
    }

    // Screen
    if (flagDisplay == true)
    {
        countFlush += 1;
        countFlush %= FAST_BEFORE_GLOBAL;
        if (countFlush == 0)
        {
            myScreen.regenerate();
            mySerial.println("countFlush = 0 ! regenerate");
        }
        displayValue();
        myScreen.flush();

        flagDisplay = false;
    }

    delay(100);
}

