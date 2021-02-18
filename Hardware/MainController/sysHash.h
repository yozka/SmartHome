#pragma once
#include <Arduino.h>
#include <avr/pgmspace.h>

namespace sys
{
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// подсчет хеша на этапе компиляции
    /// 
    ///
    ///--------------------------------------------------------------------------------------
    unsigned long constexpr hash_const(char const *input) 
    {
        return *input ? static_cast<unsigned long>(*input) ^ 33 * hash_const(input + 1) : 5381;
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// подсчет хеша из строчек расположенными в памяти программы
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    unsigned long hash_pgm(PGM_P input) 
    {
        unsigned char c = pgm_read_byte(input++);
        return c ? static_cast<unsigned long>(c) ^ 33 * hash_pgm(input) : 5381;
    }
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// подсчет хеша из строчек расположенными в памяти программы
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    unsigned long hash (const __FlashStringHelper *input)
    {
        return hash_pgm(reinterpret_cast<PGM_P>(input));
    }
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// Список устройств
    /// с динамической инциализацией
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    unsigned long hash (const String &input)
    {
        unsigned long dataHash = 5381;
        auto index = input.length();
        while (index) 
        {
            dataHash = (dataHash * 33) ^ input[--index];
        }
        return dataHash;
    }
}


constexpr unsigned long operator "" _hash(const char* input, size_t) 
{
    return sys::hash_const(input);
}
