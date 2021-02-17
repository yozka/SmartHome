#pragma once
#include <Arduino.h>

namespace sys
{
    //подсчет хеша алгоритмом 
    unsigned long constexpr hash_const(char const *input) 
    {
        return *input ? static_cast<unsigned long>(*input) ^ 33 * hash_const(input + 1) : 5381;
    }


    unsigned long hash (const __FlashStringHelper *ifsh);
    unsigned long hash (const String &input);
}


constexpr unsigned long operator "" _hash(const char* input, size_t) 
{
    return sys::hash_const(input);
}
