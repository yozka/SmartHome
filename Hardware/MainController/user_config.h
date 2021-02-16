#pragma once
#include <Arduino.h>


#include "user_board001.h"


namespace Config
{

    ///--------------------------------------------------------------------------------------
    namespace Network
    {
        namespace Terminal
        {
            constexpr uint16_t          port = 23; //порт подключения к устройству
            constexpr unsigned long     timeSession = 1800000; //время длины сессии с терминалом 30min
            constexpr unsigned long     timeLoginSession = 60000; //время длины сессии вода пароля 1 минута
        }

    /*
        constexpr byte mac[]        = { 0xAD, 0xBD, 0xBE, 0xEF, 0xFE, 0xED };  
        constexpr byte dns[]        = { 192, 168, 0, 1 };    
        constexpr byte ip[]         = { 192, 168, 0, 160 };    
        constexpr byte gateway[]    = { 192, 168, 0, 1 };
        constexpr byte subnet[]     = { 255, 255, 0, 0 };*/
    }
    ///--------------------------------------------------------------------------------------




    ///--------------------------------------------------------------------------------------
    namespace Security
    {
        constexpr int lengthAuth = 10; //максимальное количество символов системе аккаунта
    }
    ///--------------------------------------------------------------------------------------






    ///--------------------------------------------------------------------------------------
    namespace Command
    {

        constexpr int lengthCommand = 100; //максимальное количество символов в команде


        namespace Help
        {
            constexpr int margin = 12; //отступ команды помощи
        }


    }
    ///--------------------------------------------------------------------------------------







}

