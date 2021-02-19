#pragma once
#include <Arduino.h>


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
    unsigned long hash (const __FlashStringHelper *input);
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// Список устройств
    /// с динамической инциализацией
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    unsigned long hash (const String &input);
    ///--------------------------------------------------------------------------------------



}


constexpr unsigned long operator "" _hash(const char* input, size_t) 
{
    return sys::hash_const(input);
}




