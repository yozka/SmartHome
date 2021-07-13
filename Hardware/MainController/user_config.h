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
            constexpr uint16_t      port                = 23; //порт подключения к устройству
            constexpr unsigned long timeSession         = 1000L * 60L * 30L;     //время длины сессии с терминалом 30min
            constexpr unsigned long timeLoginSession    = 1000L * 60L;  //время длины сессии вода пароля 1 минута
            constexpr int           lengthAuth          = 10; //максимальное количество символов системе аккаунта
        }

        namespace Mqtt
        {
            constexpr uint16_t      port            = 1883; //порт подключения к брокеру

            constexpr unsigned long timeReconnect   = 1000L * 30L; //время пересоздания соеденения 30 секунд
            constexpr unsigned long timeStep        = 1000L * 10L; //после не удачной попытки соеденения, увеличивам время для следующей попытки
            constexpr unsigned long timeMax         = 1000L * 60L * 10L; //максимальное время ожидания 10 минут
        }    

    }
    ///--------------------------------------------------------------------------------------




    ///--------------------------------------------------------------------------------------
    namespace Button
    {
        constexpr unsigned long timeHysteresisDown  = 200L; //время срабатывания вмомент нажатия на кнопку
        constexpr unsigned long timeHysteresisUp    = 200L; //время срабатывания при отпускания кнопки
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

