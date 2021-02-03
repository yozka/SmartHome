#include "cmdTime.h"
#include "commonTerminal.h"

using namespace Command;




 void ACommandTime::help(Stream &console)
 {
      console.println(F("The command displays the current time"));
 }



void ACommandTime::execute(const Terminal::AParameters &param, Stream &console)
{
    if (!param.isEmpty())
    {
        //установим екущее время
        bool error = true;
 	    unsigned char day, weekday, month, year, hour, minute, second = 0;
	    if (Controllino_ReadTimeDate(&day, &weekday, &month, &year, &hour, &minute, &second) >= 0)
        {
            hour    = param.conv2d(0);
            minute  = param.conv2d(3);
            second  = param.conv2d(6);
            error   = Controllino_SetTimeDate(day, weekday, month, year, hour, minute, second) < 0;
        }

        if (error)
        {
            console.println(F("Invalid time specified"));
        }
    }
    


    //покажем текущую дату
    const auto print = [&console](const unsigned char value)
    {
        if (value < 10)
        {
            console.print('0');
        }
        console.print(value);
    };

    unsigned char hour, minute, second = 0;
	if (Controllino_ReadTimeDate(nullptr, nullptr, nullptr, nullptr, &hour, &minute, &second) >= 0)
	{
        //выведем текущее время
        console.print(F("Current time: "));
        print(hour);
        console.print(':');
        print(minute);
        console.print(':');
        print(second);
        console.println();
    }
    else
    {
        console.println(F("RTC chip was error"));
    }
}