#include "cmdDate.h"
#include "commonTerminal.h"

using namespace Command;




 void ACommandDate::help(Stream &console)
 {
      console.println(F("The command displays the current date in the format DAY.MONTH.YEAR"));
 }



void ACommandDate::execute(const Terminal::AParameters &param, Stream &console)
{
    if (!param.isEmpty())
    {
        //установим текущую дату
        bool error = true;
 	    unsigned char day, weekday, month, year, hour, minute, second = 0;
	    if (Controllino_ReadTimeDate(&day, &weekday, &month, &year, &hour, &minute, &second) >= 0)
        {
            day     = param.conv2d(0);
            month   = param.conv2d(3);
            year    = param.conv2d(6);
            error   = Controllino_SetTimeDate(day, weekday, month, year, hour, minute, second) < 0;
        }

        if (error)
        {
            console.println(F("Invalid date specified"));
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

    unsigned char day, month, year = 0;
	if (Controllino_ReadTimeDate(&day, nullptr, &month, &year, nullptr, nullptr, nullptr) >= 0)
	{
        //выведем текущую дату
        console.print(F("Current data [dd.mm.yy]: "));
        print(day);
        console.print('.');
        print(month);
        console.print('.');
        print(year);
        console.println();
    }
    else
    {
        console.println(F("RTC chip was error"));
    }
}