#pragma once
#include <Arduino.h>
#include <Controllino.h>
#include "src/Device/devices.h"
#include "src/Device/deviceRelay.h"
#include "src/Device/deviceButton.h"

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






    //Описание реле
    namespace meta
    {
        struct Relay00  { static String name() { return F("Relay00"); }};
        struct Relay01  { static String name() { return F("Relay01"); }};
        struct Relay02  { static String name() { return F("Relay02"); }};
        struct Relay03  { static String name() { return F("Relay03"); }};
        struct Relay04  { static String name() { return F("Relay04"); }};
        struct Relay05  { static String name() { return F("Relay05"); }};
        struct Relay06  { static String name() { return F("Relay06"); }};
        struct Relay07  { static String name() { return F("Relay07"); }};
        struct Relay08  { static String name() { return F("Relay08"); }};
        struct Relay09  { static String name() { return F("Relay09"); }};
        struct Relay10  { static String name() { return F("Relay10"); }};
        struct Relay11  { static String name() { return F("Relay11"); }};
        struct Relay12  { static String name() { return F("Relay12"); }};
        struct Relay13  { static String name() { return F("Relay13"); }};
        struct Relay14  { static String name() { return F("Relay14"); }};
        struct Relay15  { static String name() { return F("Relay15"); }};
        struct Relay16  { static String name() { return F("Relay16"); }};
        struct Relay17  { static String name() { return F("Relay17"); }};
        struct Relay18  { static String name() { return F("Relay18"); }};
        struct Relay19  { static String name() { return F("Relay19"); }};
        struct Relay20  { static String name() { return F("Relay20"); }};
        struct Relay21  { static String name() { return F("Relay21"); }};
        struct Relay22  { static String name() { return F("Relay22"); }};
        struct Relay23  { static String name() { return F("Relay23"); }};
    }
   
       
    //Конфигурация реле <Пин контроллера, Описание реле>
    using ARelay00  = Device::TRelay< CONTROLLINO_R0,  meta::Relay00 >;
    using ARelay01  = Device::TRelay< CONTROLLINO_R1,  meta::Relay01 >;
    using ARelay02  = Device::TRelay< CONTROLLINO_R2,  meta::Relay02 >;
    using ARelay03  = Device::TRelay< CONTROLLINO_R3,  meta::Relay03 >;
    using ARelay04  = Device::TRelay< CONTROLLINO_R4,  meta::Relay04 >;
    using ARelay05  = Device::TRelay< CONTROLLINO_R5,  meta::Relay05 >;
    using ARelay06  = Device::TRelay< CONTROLLINO_R6,  meta::Relay06 >;
    using ARelay07  = Device::TRelay< CONTROLLINO_R7,  meta::Relay07 >;
    using ARelay08  = Device::TRelay< CONTROLLINO_R8,  meta::Relay08 >;
    using ARelay09  = Device::TRelay< CONTROLLINO_R9,  meta::Relay09 >;
    using ARelay10  = Device::TRelay< CONTROLLINO_R10, meta::Relay10 >;
    using ARelay11  = Device::TRelay< CONTROLLINO_R11, meta::Relay11 >;
    using ARelay12  = Device::TRelay< CONTROLLINO_R12, meta::Relay12 >;
    using ARelay13  = Device::TRelay< CONTROLLINO_R13, meta::Relay13 >;
    using ARelay14  = Device::TRelay< CONTROLLINO_R14, meta::Relay14 >;
    using ARelay15  = Device::TRelay< CONTROLLINO_R15, meta::Relay15 >;
    using ARelay16  = Device::TRelay< CONTROLLINO_D0,  meta::Relay16 >;
    using ARelay17  = Device::TRelay< CONTROLLINO_D1,  meta::Relay17 >;
    using ARelay18  = Device::TRelay< CONTROLLINO_D2,  meta::Relay18 >;
    using ARelay19  = Device::TRelay< CONTROLLINO_D3,  meta::Relay19 >;
    using ARelay20  = Device::TRelay< CONTROLLINO_D4,  meta::Relay20 >;
    using ARelay21  = Device::TRelay< CONTROLLINO_D5,  meta::Relay21 >;
    using ARelay22  = Device::TRelay< CONTROLLINO_D6,  meta::Relay22 >;
    using ARelay23  = Device::TRelay< CONTROLLINO_D7,  meta::Relay23 >;


    //массив используемых релюшек
    using ARelays = Device::TDevices<   Device::ARelay::ptr, //базовый класс реле
                                                ARelay00, ARelay01, ARelay02, 
                                                ARelay03, ARelay04, ARelay05,
                                                ARelay06, ARelay07, ARelay08,
                                                ARelay09, ARelay10, ARelay11,
                                                ARelay12, ARelay13, ARelay14,
                                                ARelay15, ARelay16, ARelay17,
                                                ARelay18, ARelay19, ARelay20,
                                                ARelay21, ARelay22, ARelay23 >;






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