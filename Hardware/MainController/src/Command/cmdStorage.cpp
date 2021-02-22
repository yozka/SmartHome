#include "cmdStorage.h"
#include "../Systems/sysStorage.h"
///--------------------------------------------------------------------------------------




///--------------------------------------------------------------------------------------
using namespace Command;
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// Команда вывод информации по сохраняемым данным
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
 void ACommandStorage::help(Stream *console)
 {
      console->println(F("Reported flash storag usage"));
 }
///--------------------------------------------------------------------------------------






void ACommandStorage::execute(const Terminal::AParameters &param, Stream *console)
{
    sys::AStorage storage;
    if (param.source().equalsIgnoreCase(F("erase")))
    {
        console->println(F("Erase flash storage..."));
        storage.erase();
        console->println(F("OK."));
        return;
    }


    if (param.isEmpty())
    {
        storage.dump(console);
    }

}