#pragma once
#include <Arduino.h>

namespace sys
{


    constexpr char CR          = '\r';
    constexpr char LF          = '\n';
    constexpr char BS          = 0x7F;
    constexpr char NULLCHAR    = '\0';
    constexpr char SPACE       = ' ';


    String arrayToHex(const byte array[], const int len);
    unsigned int hexToDec(const String &hexString);

}