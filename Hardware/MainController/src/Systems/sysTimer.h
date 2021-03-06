#pragma once

namespace Time
{
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// Таймер
    /// в качестве параметра передаются милисикунды 
    /// 1 сек 1000 мсек
    /// 
    ///--------------------------------------------------------------------------------------
    template <int interval = 0>
    class TTimer
    {
        public:

            bool active()
            {
                const unsigned long time = millis();
                if (time < mPeriod)
                {
                    return false;
                }    
                mPeriod = time + interval;
                return true;
            };

        private:

            unsigned long mPeriod = { 0 };

    };
    ///--------------------------------------------------------------------------------------



    //очень медленный таймер, нужен для обнолвения несущественной переферии
    using ASlowTimer = TTimer<100>;




    ///--------------------------------------------------------------------------------------


}