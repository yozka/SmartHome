#include "sysHelper.h"



String arrayToHex(const char array[], const int len)
{
    String buffer;
    buffer.reserve(len * 2);
    for (int i = 0; i < len; i++)
    {
        const unsigned char nib1 = (array[i] >> 4) & 0x0F;
        const unsigned char nib2 = (array[i] >> 0) & 0x0F;
        buffer += static_cast<char>(nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA);
        buffer += static_cast<char>(nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA);
    }
    return buffer;
};