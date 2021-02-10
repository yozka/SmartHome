#include "sysHelper.h"



String arrayToHex(const byte array[], const int len)
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


unsigned int hexToDec(const String &hexString) 
{
    unsigned int decValue = 0;
    for (int i = 0; i < hexString.length(); i++) 
    {
        auto nextInt = hexString.charAt(i);
        if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
        if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
        if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
        nextInt = constrain(nextInt, 0, 15);
        decValue = (decValue * 16) + nextInt;
    }
    return decValue;
}