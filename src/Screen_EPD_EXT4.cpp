//
// Screen_EPD_EXT4.cpp
// Library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 28 Jun 2016
//
// Copyright (c) Rei Vilo, 2010-2025
// Licence Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
// For exclusive use with Pervasive Displays screens
// Portions (c) Pervasive Displays, 2010-2025
//
// Release 509: Added eScreen_EPD_EXT3_271_Fast
// Release 527: Added support for ESP32 PSRAM
// Release 541: Improved support for ESP32
// Release 550: Tested Xiao ESP32-C3 with SPI exception
// Release 601: Added support for screens with embedded fast update
// Release 602: Improved functions structure
// Release 604: Improved stability
// Release 607: Improved screens names consistency
// Release 608: Added screen report
// Release 609: Added temperature management
// Release 610: Removed partial update
// Release 700: Refactored screen and board functions
// Release 701: Improved functions names consistency
// Release 701: Added support for eScreen_EPD_EXT3_290_0F_Wide xE2290KS0Fx
// Release 702: Added support for eScreen_EPD_EXT3_206_0E_Wide xE2206KS0Ex
// Release 703: Added support for eScreen_EPD_EXT3_152_0J_Wide xE2152KS0Jx
// Release 800: Read OTP memory
// Release 801: Improved OTP implementation
// Release 802: Added support for 343-KS-0B xE2343PS0Bx
// Release 802: Added references to application notes
// Release 802: Refactored CoG functions
// Release 803: Added types for string and frame-buffer
// Release 804: Improved power management
// Release 810: Added support for EXT4
//

// Library header
#include "Screen_EPD_EXT4.h"

//
// === COG section
//
/// @cond
/// @see
/// * ApplicationNote_smallSize_fast-update_v02_20220907
/// * ApplicationNote_Small_Size_wide-Temperature_EPD_v03_20231031_B
/// * ApplicationNote_Small_Size_wide-Temperature_EPD_v01_20231225_A
/// * ApplicationNote_EPD343_Mono(E2343PS0Bx)_240320a
/// * ApplicationNote_for_5.8inch_fast-update_EPDE2581PS0B1_20230206b
/// * ApplicationNote_152_Size_wide-Temperature_EPD_v01_20231225_A
//

//
// --- Medium screens with K or P film
//

//
// --- End of Medium screens with K or P film
//

//
// --- Small screens with K or P film
//
void Screen_EPD_EXT4_Fast::COG_SmallKP_reset()
{
    // Application note § 2. Power on COG driver
    b_reset(5, 5, 10, 5, 5); // Small

    // Check after reset
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:

            if (digitalRead(b_pin.panelBusy) == HIGH)
            {
                mySerial.println();
                mySerial.println("hV * Incorrect type for 1.52-Wide");
                while (0x01);
            }
            break;

        default:

            break;
    }
}

void Screen_EPD_EXT4_Fast::COG_SmallKP_getDataOTP()
{
    // Read OTP
    uint8_t ui8 = 0;
    uint16_t _readBytes = 0;
    u_flagOTP = false;

    // Application note § 3. Read OTP memory
    // Register 0x50 flag
    // Additional settings for fast update, 154 206 213 266 271A 370 and 437 screens (s_flag50)
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_154_PS_0C:
        case eScreen_EPD_154_KS_0C:
        case eScreen_EPD_206_KS_0E:
        case eScreen_EPD_213_PS_0E:
        case eScreen_EPD_213_KS_0E:
        case eScreen_EPD_266_PS_0C:
        case eScreen_EPD_266_KS_0C:
        case eScreen_EPD_271_KS_0C: // 2.71(A)
        case eScreen_EPD_370_PS_0C:
        case eScreen_EPD_370_PS_0C_Touch:
        case eScreen_EPD_370_KS_0C:
        case eScreen_EPD_437_PS_0C:
        case eScreen_EPD_437_KS_0C:

            s_flag50 = true;
            break;

        default:

            s_flag50 = false;
            break;
    }

    // Screens with no OTP
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:
        case eScreen_EPD_290_KS_0F:

            u_flagOTP = true;
            mySerial.println("hV . OTP check passed - embedded PSR");
            return; // No PSR
            break;

        default:

            break;
    }

    // GPIO
    COG_SmallKP_reset(); // Although not mentioned, reset to ensure stable state

    // Read OTP
    _readBytes = 2;
    ui8 = 0;

    uint16_t offsetA5 = 0x0000;
    uint16_t offsetPSR = 0x0000;

    digitalWrite(b_pin.panelDC, LOW); // Command
    digitalWrite(b_pin.panelCS, LOW); // Select
    hV_HAL_SPI3_write(0xa2);
    digitalWrite(b_pin.panelCS, HIGH); // Unselect
    delay(10);

    digitalWrite(b_pin.panelDC, HIGH); // Data
    digitalWrite(b_pin.panelCS, LOW); // Select
    ui8 = hV_HAL_SPI3_read(); // Dummy
    digitalWrite(b_pin.panelCS, HIGH); // Unselect
    // mySerial.println, "hV . Dummy read 0x%02x", ui8);

    digitalWrite(b_pin.panelCS, LOW); // Select
    ui8 = hV_HAL_SPI3_read(); // First byte to be checked
    digitalWrite(b_pin.panelCS, HIGH); // Unselect
    // hV_HAL_log(LEVEL_INFO, "ui8= 0x%02x", ui8);

    // Check bank
    uint8_t bank = ((ui8 == 0xa5) ? 0 : 1);

    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_271_KS_09:
        case eScreen_EPD_271_KS_09_Touch:

            offsetPSR = 0x004b;
            offsetA5 = 0x0000;

            if (bank > 0)
            {
                COG_data[0] = 0xcf;
                COG_data[1] = 0x82;
                return;
            }
            break;

        case eScreen_EPD_271_PS_09:
        // case eScreen_EPD_271_KS_09_Touch:
        case eScreen_EPD_271_PS_09_Touch:
        case eScreen_EPD_287_PS_09:

            offsetPSR = 0x004b;
            offsetA5 = 0x0000;

            break;

        case eScreen_EPD_154_KS_0C:
        case eScreen_EPD_154_PS_0C:
        case eScreen_EPD_266_KS_0C:
        case eScreen_EPD_266_PS_0C:
        case eScreen_EPD_271_KS_0C: // 2.71(A)
        case eScreen_EPD_350_KS_0C:
        case eScreen_EPD_370_KS_0C:
        case eScreen_EPD_370_PS_0C:
        case eScreen_EPD_370_PS_0C_Touch:
        case eScreen_EPD_437_PS_0C:

            offsetPSR = (bank == 0) ? 0x0fb4 : 0x1fb4;
            offsetA5 = (bank == 0) ? 0x0000 : 0x1000;

            break;

        case eScreen_EPD_206_KS_0E:
        case eScreen_EPD_213_KS_0E:
        case eScreen_EPD_213_PS_0E:

            offsetPSR = (bank == 0) ? 0x0b1b : 0x171b;
            offsetA5 = (bank == 0) ? 0x0000 : 0x0c00;
            break;

        case eScreen_EPD_417_PS_0D:
        case eScreen_EPD_417_KS_0D:

            offsetPSR = (bank == 0) ? 0x0b1f : 0x171f;
            offsetA5 = (bank == 0) ? 0x0000 : 0x0c00;
            break;

        default:

            mySerial.println(formatString("hV * OTP check failed - Screen %i-%cS-0%c not supported", u_codeSize, u_codeFilm, u_codeDriver));
            mySerial.flush();
            while (true);
            break;
    }

    // Check second bank
    if (offsetA5 > 0x0000)
    {
        for (uint16_t index = 1; index < offsetA5; index += 1)
        {
            digitalWrite(b_pin.panelCS, LOW); // Select
            ui8 = hV_HAL_SPI3_read();
            digitalWrite(b_pin.panelCS, HIGH); // Unselect
        }

        digitalWrite(b_pin.panelCS, LOW); // Select
        ui8 = hV_HAL_SPI3_read(); // First byte to be checked
        digitalWrite(b_pin.panelCS, HIGH); // Unselect

        if (ui8 != 0xa5)
        {
            mySerial.println();
            mySerial.println(formatString("hV * OTP check failed - Bank %i, first 0x%02x, expected 0x%02x", bank, ui8, 0xa5));
            mySerial.flush();
            while (true);
        }
    }

    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_271_KS_09:
        case eScreen_EPD_271_PS_09:
        case eScreen_EPD_271_PS_09_Touch:
        // case eScreen_EPD_287_KS_09:
        case eScreen_EPD_287_PS_09:

            mySerial.println(formatString("hV . OTP check passed - Bank %i, first 0x%02x %s", bank, ui8, (bank == 0) ? "as expected" : "not checked"));
            break;

        default:

            mySerial.println(formatString("hV . OTP check passed - Bank %i, first 0x%02x as expected", bank, ui8));
            break;
    }

    // Ignore bytes 1..offsetPSR
    for (uint16_t index = offsetA5 + 1; index < offsetPSR; index += 1)
    {
        digitalWrite(b_pin.panelCS, LOW); // Select
        ui8 = hV_HAL_SPI3_read();
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
    }

    // Populate COG_data
    for (uint16_t index = 0; index < _readBytes; index += 1)
    {
        digitalWrite(b_pin.panelCS, LOW); // Select
        ui8 = hV_HAL_SPI3_read(); // Read OTP
        COG_data[index] = ui8;
        digitalWrite(b_pin.panelCS, HIGH); // Unselect
    }

    u_flagOTP = true;
}

void Screen_EPD_EXT4_Fast::COG_SmallKP_initial(uint8_t updateMode)
{
    // Application note § 4. Input initial command
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:

            // Soft reset
            b_sendCommand8(0x12);
            digitalWrite(b_pin.panelDC, LOW);
            b_waitBusy(LOW); // 150 and 152 specific

            // Work settings
            b_sendCommandData8(0x1a, u_temperature);

            if (updateMode == UPDATE_GLOBAL)
            {
                b_sendCommandData8(0x22, 0xd7);
            }
            else if (updateMode == UPDATE_FAST)
            {
                b_sendCommandData8(0x3c, 0xc0);
                b_sendCommandData8(0x22, 0xdf);
            }
            break;

        default:

            // Work settings
            uint8_t indexTemperature; // Temperature
            uint8_t index00_work[2]; // PSR

            // FILM_P and FILM_K already checked
            if (updateMode != UPDATE_GLOBAL) // Specific settings for fast update
            {
                indexTemperature = u_temperature | 0x40; // temperature | 0x40
                index00_work[0] = COG_data[0] | 0x10; // PSR0 | 0x10
                index00_work[1] = COG_data[1] | 0x02; // PSR1 | 0x02
            }
            else // Common settings
            {
                indexTemperature = u_temperature; // Temperature
                index00_work[0] = COG_data[0]; // PSR0
                index00_work[1] = COG_data[1]; // PSR1
            } // u_codeExtra updateMode

            // New algorithm
            b_sendCommandData8(0x00, 0x0e); // Soft-reset
            b_waitBusy();

            b_sendCommandData8(0xe5, indexTemperature); // Input Temperature
            b_sendCommandData8(0xe0, 0x02); // Activate Temperature

            if (u_codeSize == SIZE_290) // No PSR
            {
                b_sendCommandData8(0x4d, 0x55);
                b_sendCommandData8(0xe9, 0x02);
            }
            else
            {
                b_sendIndexData(0x00, index00_work, 2); // PSR
            }

            // Specific settings for fast update, all screens
            // FILM_P and FILM_K already checked
            if (updateMode != UPDATE_GLOBAL)
            {
                b_sendCommandData8(0x50, 0x07); // Vcom and data interval setting
            }
            break;
    }
}

void Screen_EPD_EXT4_Fast::COG_SmallKP_sendImageData(uint8_t updateMode)
{
    // Application note § 5. Input image to the EPD
    FRAMEBUFFER_TYPE nextBuffer = s_newImage;
    FRAMEBUFFER_TYPE previousBuffer = s_newImage + u_pageColourSize;

    // Send image data
    // case UPDATE_FAST:
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:

            b_sendIndexData(0x24, previousBuffer, u_pageColourSize); // Next frame, blackBuffer
            b_sendIndexData(0x26, nextBuffer, u_pageColourSize); // Previous frame, 0x00
            break;

        default:
            // Additional settings for fast update, 154 213 266 370 and 437 screens (s_flag50)
            if (s_flag50)
            {
                b_sendCommandData8(0x50, 0x27); // Vcom and data interval setting
            }

            b_sendIndexData(0x10, previousBuffer, u_pageColourSize); // First frame, blackBuffer
            b_sendIndexData(0x13, nextBuffer, u_pageColourSize); // Second frame, 0x00

            // Additional settings for fast update, 154 213 266 370 and 437 screens (s_flag50)
            if (s_flag50)
            {
                b_sendCommandData8(0x50, 0x07); // Vcom and data interval setting
            }
            break;
    } // u_eScreen_EPD

    // Copy next frame to previous frame
    memcpy(previousBuffer, nextBuffer, u_pageColourSize); // Copy displayed next to previous
}

void Screen_EPD_EXT4_Fast::COG_SmallKP_update(uint8_t updateMode)
{
    // Application note § 6. Send updating command
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:

            b_waitBusy(LOW); // 152 specific
            b_sendCommand8(0x20); // Display Refresh
            digitalWrite(b_pin.panelCS, HIGH); // CS# = 1
            b_waitBusy(LOW); // 152 specific
            break;

        default:

            b_waitBusy();

            b_sendCommand8(0x04); // Power on
            b_waitBusy();

            b_sendCommand8(0x12); // Display Refresh
            b_waitBusy();
            break;
    }
}

void Screen_EPD_EXT4_Fast::COG_SmallKP_powerOff()
{
    // Application note § 7. Turn-off DC/DC
    switch (u_eScreen_EPD)
    {
        case eScreen_EPD_150_KS_0J:
        case eScreen_EPD_152_KS_0J:

            break;

        default:

            b_sendCommand8(0x02); // Turn off DC/DC
            b_waitBusy();
            break;
    }
}
//
// --- End of Small screens with K or P film
//
/// @endcond
//
// === End of COG section
//

//
// === Class section
//
Screen_EPD_EXT4_Fast::Screen_EPD_EXT4_Fast(eScreen_EPD_t eScreen_EPD_EXT3, pins_t board)
{
    u_eScreen_EPD = eScreen_EPD_EXT3;
    b_pin = board;
    s_newImage = 0; // nullptr
    COG_data[0] = 0;
}

void Screen_EPD_EXT4_Fast::begin()
{
    // u_eScreen_EPD = eScreen_EPD_EXT3;
    u_codeSize = SCREEN_SIZE(u_eScreen_EPD);
    u_codeFilm = SCREEN_FILM(u_eScreen_EPD);
    u_codeDriver = SCREEN_DRIVER(u_eScreen_EPD);
    u_codeExtra = SCREEN_EXTRA(u_eScreen_EPD);
    v_screenColourBits = 2; // BWR and BWRY

    //
    // === EXT4 section
    //
    if (b_pin.panelPower == NOT_CONNECTED)
    {
        mySerial.println();
        mySerial.println("hV * Required pin panelPower is NOT_CONNECTED");
        while (0x01);
    }
    //
    // === End of EXT4 section
    //

    // Checks
    switch (u_codeFilm)
    {
        case FILM_P: // BW, fast update
        case FILM_K: // BW, fast update and wide temperature

            break;

        default:

            debugVariant(FILM_P);
            break;
    }

    //
    // === Touch section
    //

    //
    // === End of touch section
    //

    //
    // === Large screen section
    //
    // Check panelCSS for large screens
    if (((u_codeSize == SIZE_969) or (u_codeSize == SIZE_1198)) and (b_pin.panelCSS == NOT_CONNECTED))
    {
        mySerial.println();
        mySerial.println("hV * Required pin panelCSS is NOT_CONNECTED");
        while (0x01);
    }
    //
    // === End of Large screen section
    //

    // Configure board
    switch (u_codeSize)
    {
        case SIZE_343: // 3.43"
        case SIZE_581: // 5.81"
        case SIZE_741: // 7.41"

            b_begin(b_pin, FAMILY_MEDIUM, 0);
            break;

        case SIZE_969: // 9.69"
        case SIZE_1198: // 11.98"

            b_begin(b_pin, FAMILY_LARGE, 50);
            break;

        default:

            b_begin(b_pin, FAMILY_SMALL, 0);
            break;
    }

    //
    // === Touch section
    //

    //
    // === End of touch section
    //

    // Sizes
    switch (u_codeSize)
    {
        case SIZE_150: // 1.50"
        case SIZE_152: // 1.52"

            v_screenSizeV = 200; // vertical = wide size
            v_screenSizeH = 200; // horizontal = small size
            break;

        case SIZE_154: // 1.54"

            v_screenSizeV = 152; // vertical = wide size
            v_screenSizeH = 152; // horizontal = small size
            break;

        case SIZE_206: // 2.06"

            v_screenSizeV = 248; // vertical = wide size
            v_screenSizeH = 128; // horizontal = small size
            break;

        case SIZE_213: // 2.13"

            v_screenSizeV = 212; // vertical = wide size
            v_screenSizeH = 104; // horizontal = small size
            break;

        case SIZE_266: // 2.66"

            v_screenSizeV = 296; // vertical = wide size
            v_screenSizeH = 152; // horizontal = small size
            break;

        case SIZE_271: // 2.71" and 2.71"-Touch

            v_screenSizeV = 264; // vertical = wide size
            v_screenSizeH = 176; // horizontal = small size
            break;

        case SIZE_287: // 2.87"

            v_screenSizeV = 296; // vertical = wide size
            v_screenSizeH = 128; // horizontal = small size
            break;

        case SIZE_290: // 2.90"

            v_screenSizeV = 384; // vertical = wide size
            v_screenSizeH = 168; // horizontal = small size
            break;

        // Those screens do not fit into the EXT4 extension board
        // case SIZE_343: // 3.43" and 3.43"-Touch
        //
        //     v_screenSizeV = 392; // vertical = wide size
        //     v_screenSizeH = 456; // horizontal = small size
        //     break;
        //
        // case SIZE_350: // 3.50"
        //
        //     v_screenSizeV = 384; // vertical = wide size
        //     v_screenSizeH = 168; // horizontal = small size
        //     break;
        //
        // case SIZE_370: // 3.70" and 3.70"-Touch
        //
        //     v_screenSizeV = 416; // vertical = wide size
        //     v_screenSizeH = 240; // horizontal = small size
        //     break;
        //
        // case SIZE_417: // 4.17"
        //
        //     v_screenSizeV = 300; // vertical = wide size
        //     v_screenSizeH = 400; // horizontal = small size
        //     break;
        //
        // case SIZE_437: // 4.37"
        //
        //     v_screenSizeV = 480; // vertical = wide size
        //     v_screenSizeH = 176; // horizontal = small size
        //     break;

        // Those screens are not available with embedded fast update or wide temperature
        // case SIZE_565: // 5.65"
        //
        //     v_screenSizeV = 600; // v = wide size
        //     v_screenSizeH = 448; // h = small size
        //     break;
        //
        // case SIZE_581: // 5.81"
        //
        //     v_screenSizeV = 720; // v = wide size
        //     v_screenSizeH = 256; // h = small size
        //     break;
        //
        // case SIZE_741: // 7.41"
        //
        //     v_screenSizeV = 800; // v = wide size
        //     v_screenSizeH = 480; // h = small size
        //     break;
        //
        // case SIZE_969: // 9.69"
        //
        //     v_screenSizeV = 672; // v = wide size
        //     v_screenSizeH = 960; // Actually, 960 = 480 x 2, h = small size
        //     break;
        //
        // case SIZE_1198: // 11.98"
        //
        //     v_screenSizeV = 768; // v = wide size
        //     v_screenSizeH = 960; // Actually, 960 = 480 x 2, h = small size
        //     break;

        default:

            mySerial.println();
            mySerial.println(formatString("hV * Screen %i-%cS-0%c is not supported", u_codeSize, u_codeFilm, u_codeDriver));
            while (0x01);
            break;
    } // u_codeSize
    v_screenDiagonal = u_codeSize;

    // Report
    mySerial.println(formatString("hV = Screen %s %ix%i", WhoAmI().c_str(), screenSizeX(), screenSizeY()));
    mySerial.println(formatString("hV = Number %i-%cS-0%c", u_codeSize, u_codeFilm, u_codeDriver));
    mySerial.println(formatString("hV = PDLS %s v%i.%i.%i", SCREEN_EPD_EXT3_VARIANT, SCREEN_EPD_EXT3_RELEASE / 100, (SCREEN_EPD_EXT3_RELEASE / 10) % 10, SCREEN_EPD_EXT3_RELEASE % 10));
    mySerial.println();

    u_bufferDepth = v_screenColourBits; // 2 colours
    u_bufferSizeV = v_screenSizeV; // vertical = wide size
    u_bufferSizeH = v_screenSizeH / 8; // horizontal = small size 112 / 8, 1 bit per pixel

    // Force conversion for two unit16_t multiplication into uint32_t.
    // Actually for 1 colour; BWR requires 2 pages.
    u_pageColourSize = (uint32_t)u_bufferSizeV * (uint32_t)u_bufferSizeH;

#if defined(BOARD_HAS_PSRAM) // ESP32 PSRAM specific case

    if (s_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = (uint8_t *) ps_malloc(u_pageColourSize * u_bufferDepth);
        s_newImage = (uint8_t *) _newFrameBuffer;
    }

#else // default case

    if (s_newImage == 0)
    {
        static uint8_t * _newFrameBuffer;
        _newFrameBuffer = new uint8_t[u_pageColourSize * u_bufferDepth];
        s_newImage = (uint8_t *) _newFrameBuffer;
    }

#endif // ESP32 BOARD_HAS_PSRAM

    memset(s_newImage, 0x00, u_pageColourSize * u_bufferDepth);

    setTemperatureC(25); // 25 Celsius = 77 Fahrenheit
    b_fsmPowerScreen = FSM_OFF;
    setPowerProfile(POWER_MODE_MANUAL, POWER_SCOPE_GPIO_ONLY);

    // Turn SPI on, initialise GPIOs and set GPIO levels
    // Reset panel and get tables
    resume();

    // Fonts
    hV_Screen_Buffer::begin(); // Standard

    if (f_fontMax() > 0)
    {
        f_selectFont(0);
    }
    f_fontSolid = false;

    // Orientation
    setOrientation(0);

    v_penSolid = false;
    u_invert = false;

    //
    // === Touch section
    //

    //
    // === End of Touch section
    //
}

STRING_TYPE Screen_EPD_EXT4_Fast::WhoAmI()
{
    char work[64] = {0};
    u_WhoAmI(work);

    return formatString("iTC %i.%02i\"%s", v_screenDiagonal / 100, v_screenDiagonal % 100, work);
}

void Screen_EPD_EXT4_Fast::suspend(uint8_t suspendScope)
{
    if (((suspendScope & FSM_GPIO_MASK) == FSM_GPIO_MASK) and (b_pin.panelPower != NOT_CONNECTED))
    {
        if ((b_fsmPowerScreen & FSM_GPIO_MASK) == FSM_GPIO_MASK)
        {
            b_suspend();
        }
    }
}

void Screen_EPD_EXT4_Fast::resume()
{
    // Target   FSM_ON
    // Source   FSM_OFF
    //          FSM_SLEEP
    if (b_fsmPowerScreen != FSM_ON)
    {
        if ((b_fsmPowerScreen & FSM_GPIO_MASK) != FSM_GPIO_MASK)
        {
            b_resume(); // GPIO

            s_reset(); // Reset

            b_fsmPowerScreen |= FSM_GPIO_MASK;
        }

        // Check type and get tables
        if (u_flagOTP == false)
        {
            s_getDataOTP(); // 3-wire SPI read OTP memory

            s_reset(); // Reset
        }

        // Start SPI
        switch (u_eScreen_EPD)
        {
            case eScreen_EPD_150_KS_0J:
            case eScreen_EPD_152_KS_0J:

                hV_HAL_SPI_begin(16000000); // 1.52" tested with 4, 8, 16 and 32 MHz, with unicity check
                break;

            case eScreen_EPD_206_KS_0E:
            case eScreen_EPD_290_KS_0F:

                hV_HAL_SPI_begin(16000000); // 2.06" tested with 4, 8 and 16 MHz, with unicity check
                break;

            default:

                hV_HAL_SPI_begin(); // Standard 8 MHz, with unicity check
                break;
        }

#if (USE_EXT_BOARD == BOARD_EXT4)

        // Start I2C
        hV_HAL_Wire_begin();

#endif // USE_EXT_BOARD
    }
}

void Screen_EPD_EXT4_Fast::s_reset()
{
    switch (b_family)
    {
        // case FAMILY_MEDIUM:
        //
        //     COG_MediumKP_reset();
        //     break;

        case FAMILY_SMALL:

            COG_SmallKP_reset();
            break;

        default:

            break;
    }
}

void Screen_EPD_EXT4_Fast::s_getDataOTP()
{
    hV_HAL_SPI_end(); // With unicity check

    hV_HAL_SPI3_begin(); // Define 3-wire SPI pins

    // Get data OTP
    switch (b_family)
    {
        // case FAMILY_MEDIUM:
        //
        //     COG_MediumKP_getDataOTP();
        //     break;

        case FAMILY_SMALL:

            COG_SmallKP_getDataOTP();
            break;

        default:

            break;
    }
}

void Screen_EPD_EXT4_Fast::s_flush(uint8_t updateMode)
{
    // Resume
    if (b_fsmPowerScreen != FSM_ON)
    {
        resume();
    }

    switch (b_family)
    {
        // case FAMILY_MEDIUM:
        //
        //     COG_MediumKP_initial(updateMode); // Initialise
        //     COG_MediumKP_sendImageData(updateMode); // Send image data
        //     COG_MediumKP_update(updateMode); // Update
        //     COG_MediumKP_powerOff(); // Power off
        //     break;

        case FAMILY_SMALL:

            COG_SmallKP_initial(updateMode); // Initialise
            COG_SmallKP_sendImageData(updateMode); // Send image data
            COG_SmallKP_update(updateMode); // Update
            COG_SmallKP_powerOff(); // Power off
            break;

        default:

            break;
    }

    // Suspend
    if (u_suspendMode == POWER_MODE_AUTO)
    {
        suspend(u_suspendScope);
    }
}

uint8_t Screen_EPD_EXT4_Fast::flushMode(uint8_t updateMode)
{
    updateMode = checkTemperatureMode(updateMode);

    switch (updateMode)
    {
        case UPDATE_FAST:
        case UPDATE_GLOBAL:

            s_flush(UPDATE_FAST);
            break;

        default:

            mySerial.println();
            mySerial.println("hV ! PDLS - UPDATE_NONE invoked");
            break;
    }

    return updateMode;
}

void Screen_EPD_EXT4_Fast::flush()
{
    flushMode(UPDATE_FAST);
}

void Screen_EPD_EXT4_Fast::clear(uint16_t colour)
{
    if (colour == myColours.grey)
    {
        // black = 0-1, white = 0-0
        for (uint16_t i = 0; i < u_bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b10101010 : 0b01010101;
            for (uint16_t j = 0; j < u_bufferSizeH; j++)
            {
                s_newImage[i * u_bufferSizeH + j] = pattern;
            }
        }
    }
    else if ((colour == myColours.white) xor u_invert)
    {
        // physical black 0-0
        memset(s_newImage, 0x00, u_pageColourSize);
    }
    else
    {
        // physical white 1-0
        memset(s_newImage, 0xff, u_pageColourSize);
    }
}

void Screen_EPD_EXT4_Fast::regenerate(uint8_t mode)
{
    clear(myColours.black);
    flush();
    delay(100);

    clear(myColours.white);
    flush();
    delay(100);
}

void Screen_EPD_EXT4_Fast::s_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    // Orient and check coordinates are within screen
    if (s_orientCoordinates(x1, y1) == RESULT_ERROR)
    {
        return;
    }

    // Convert combined colours into basic colours
    bool flagOdd = ((x1 + y1) % 2 == 0);

    if (colour == myColours.grey)
    {
        if (flagOdd)
        {
            colour = myColours.black; // black
        }
        else
        {
            colour = myColours.white; // white
        }
    }

    // Coordinates
    uint32_t z1 = s_getZ(x1, y1);
    uint16_t b1 = s_getB(x1, y1);

    // Basic colours
    if ((colour == myColours.white) xor u_invert)
    {
        // physical black 0-0
        bitClear(s_newImage[z1], b1);
    }
    else if ((colour == myColours.black) xor u_invert)
    {
        // physical white 1-0
        bitSet(s_newImage[z1], b1);
    }
}

void Screen_EPD_EXT4_Fast::s_setOrientation(uint8_t orientation)
{
    v_orientation = orientation % 4;
}

bool Screen_EPD_EXT4_Fast::s_orientCoordinates(uint16_t & x, uint16_t & y)
{
    bool _flagResult = RESULT_ERROR;
    switch (v_orientation)
    {
        case 3: // checked, previously 1

            if ((x < v_screenSizeV) and (y < v_screenSizeH))
            {
                x = v_screenSizeV - 1 - x;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 2: // checked

            if ((x < v_screenSizeH) and (y < v_screenSizeV))
            {
                x = v_screenSizeH - 1 - x;
                y = v_screenSizeV - 1 - y;
                hV_HAL_swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;

        case 1: // checked, previously 3

            if ((x < v_screenSizeV) and (y < v_screenSizeH))
            {
                y = v_screenSizeH - 1 - y;
                _flagResult = RESULT_SUCCESS;
            }
            break;

        default: // checked

            if ((x < v_screenSizeH) and (y < v_screenSizeV))
            {
                hV_HAL_swap(x, y);
                _flagResult = RESULT_SUCCESS;
            }
            break;
    }

    return _flagResult;
}

uint32_t Screen_EPD_EXT4_Fast::s_getZ(uint16_t x1, uint16_t y1)
{
    uint32_t z1 = 0;
    // According to 11.98 inch Spectra Application Note
    // at http://www.pervasivedisplays.com/LiteratureRetrieve.aspx?ID=245146
    switch (u_codeSize)
    {
        case SIZE_969:
        case SIZE_1198:

            if (y1 >= (v_screenSizeH >> 1))
            {
                y1 -= (v_screenSizeH >> 1); // rebase y1
                z1 += (u_pageColourSize >> 1); // buffer second half
            }
            z1 += (uint32_t)x1 * (u_bufferSizeH >> 1) + (y1 >> 3);
            break;

        default:

            z1 = (uint32_t)x1 * u_bufferSizeH + (y1 >> 3);
            break;
    }
    return z1;
}

uint16_t Screen_EPD_EXT4_Fast::s_getB(uint16_t x1, uint16_t y1)
{
    uint16_t b1 = 0;

    b1 = 7 - (y1 % 8);

    return b1;
}

uint16_t Screen_EPD_EXT4_Fast::s_getPoint(uint16_t x1, uint16_t y1)
{
    return 0x0000;
}
//
// === End of Class section
//

//
// === Touch section
//

//
// === End of Touch section
//

