#pragma once
#include <Arduino.h>



namespace Device
{
    ///--------------------------------------------------------------------------------------




     ///=====================================================================================
    ///
    /// Список устройств
    /// с динамической инциализацией
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    template<class TDevicePtr, class ...TT>
    struct TDevices
    {
        TDevicePtr data[sizeof...(TT)] = { new TT() ... };


        //настройка инциализация устройств
        void setup()
        {
            for (auto device : data)
            {
                device->setup();
            }
        }


        //поиск нужного девайса
        TDevicePtr findDevice(const String &name)
        {
            for (auto device : data)
            {
                if (device->name() == name)
                {
                    return device;
                }
            }

            return {};
        }


        //обновление девайса
        template<class ...Args>
        void update(const Args& ...args)
        {
            for (auto device : data)
            {
                device->update(args...);
            }
        }

    };
    ///--------------------------------------------------------------------------------------









    ///--------------------------------------------------------------------------------------
}