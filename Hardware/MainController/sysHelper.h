#pragma once


//#define CF(p) ((const __FlashStringHelper *)p)
#define TEXT(p) ((const char *)F(p))

#define CR '\r'
#define LF '\n'
#define BS 0x7F
#define NULLCHAR '\0'
#define SPACE ' '