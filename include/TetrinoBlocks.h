#pragma once
#include <Arduino.h>

namespace TetrominoBlocks
{
    static const byte L[4][4] PROGMEM = {
        {0b0010, 0b1110, 0b0000, 0b0000}, // L shape in default orientation
        {0b0100, 0b0100, 0b0110, 0b0000}, // 90 degrees rotated
        {0b0000, 0b1110, 0b1000, 0b0000}, // 180 degrees
        {0b1100, 0b0100, 0b0100, 0b0000}  // 270 degrees
    };

    static const byte J[4][4] PROGMEM = {
        {0b0000, 0b1110, 0b0010, 0b0000}, // L shape in default orientation
        {0b0100, 0b0100, 0b1100, 0b0000}, // 90 degrees rotated
        {0b1000, 0b1110, 0b0000, 0b0000}, // 180 degrees
        {0b0110, 0b0100, 0b0100, 0b0000}  // 270 degrees
    };

    static const byte S[4][4] PROGMEM = {
        {0b0110, 0b1100, 0b0000, 0b0000}, // Default orientation
        {0b0100, 0b0110, 0b0010, 0b0000}, // 90 degrees rotated
        {0b0100, 0b0110, 0b0010, 0b0000}, // 180 degrees, same as default
        {0b0110, 0b1100, 0b0000, 0b0000}  // 270 degrees, same as 90 degrees
    };

    static const byte Z[4][4] PROGMEM = {
        {0b1100, 0b0110, 0b0000, 0b0000}, // Default orientation
        {0b0010, 0b0110, 0b0100, 0b0000}, // 90 degrees rotated
        {0b1100, 0b0110, 0b0000, 0b0000}, // 180 degrees, same as default
        {0b0010, 0b0110, 0b0100, 0b0000}  // 270 degrees, same as 90 degrees
    };

    static const byte I[4][4] PROGMEM = {
        {0b0000, 0b1111, 0b0000, 0b0000}, // Default orientation
        {0b0010, 0b0010, 0b0010, 0b0010}, // 90 degrees rotated
        {0b0000, 0b1111, 0b0000, 0b0000}, // 180 degrees, same as default
        {0b0010, 0b0010, 0b0010, 0b0010}  // 270 degrees, same as 90 degrees
    };

    static const byte O[4][4] PROGMEM = {
        {0b1100, 0b1100, 0b0000, 0b0000}, // Default orientation
        {0b1100, 0b1100, 0b0000, 0b0000}, // 90 degrees,  same as default
        {0b1100, 0b1100, 0b0000, 0b0000}, // 180 degrees, same as default
        {0b1100, 0b1100, 0b0000, 0b0000}  // 270 degrees, same as default
    };

    static const byte T[4][4] PROGMEM = {
        {0b0100, 0b1110, 0b0000, 0b0000}, // Default orientation
        {0b0100, 0b0110, 0b0100, 0b0000}, // 90 degrees
        {0b0000, 0b1110, 0b0100, 0b0000}, // 180 degrees
        {0b0100, 0b1100, 0b0100, 0b0000}  // 270 degrees
    };
}
