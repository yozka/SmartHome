#pragma once
#include <Arduino.h>




constexpr char CR          = '\r';
constexpr char LF          = '\n';
constexpr char BS          = 0x7F;
constexpr char NULLCHAR    = '\0';
constexpr char SPACE       = ' ';


String arrayToHex(const char array[], const int len);
