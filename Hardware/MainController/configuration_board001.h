#pragma once
#include <Arduino.h>
#include <Controllino.h>


namespace Board
{
    ///--------------------------------------------------------------------------------------
    ///
    /// Используется контроллер Controllino mega
    ///
    /// https://www.controllino.com/product/controllino-mega-pur/
    ///
    /// Здесь находяся все платформозависемые настройки девайса
    /// можно редактировать и настраивать девайс под свою систему
    ///
    ///--------------------------------------------------------------------------------------






     ///=====================================================================================
    ///
    /// Настройка девайса
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    inline void setup()
    {
        //инциализация часов
        Controllino_RTC_init();
    }
    ///--------------------------------------------------------------------------------------






     ///=====================================================================================
    ///
    /// Работа с часами
    /// 
    /// Используются часы реального времени
    /// 
    ///--------------------------------------------------------------------------------------
    namespace DateTime
    {
        //чтение текущегоо времени
        inline bool read(   unsigned char *aDay, 
                            unsigned char *aWeekDay, 
                            unsigned char *aMonth, 
                            unsigned char *aYear, 
                            unsigned char *aHour, 
                            unsigned char *aMinute, 
                            unsigned char *aSecond )
        {
            return Controllino_ReadTimeDate(aDay, aWeekDay, aMonth, aYear, aHour, aMinute, aSecond) >= 0;
        }

        //установка текущего времени
        inline bool write(  const unsigned char aDay, 
                            const unsigned char aWeekDay, 
                            const unsigned char aMonth, 
                            const unsigned char aYear, 
                            const unsigned char aHour, 
                            const unsigned char aMinute, 
                            const unsigned char aSecond )
        {
            return Controllino_SetTimeDate(aDay, aWeekDay, aMonth, aYear, aHour, aMinute, aSecond) >= 0;
        }
    }
    ///--------------------------------------------------------------------------------------

    







}