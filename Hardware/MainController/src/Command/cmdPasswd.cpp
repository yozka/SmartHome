#include "cmdPasswd.h"
#include "../Systems/sysStorage.h"
#include "../Systems/sysHash.h"
///--------------------------------------------------------------------------------------




///--------------------------------------------------------------------------------------
using namespace Command;
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// Команда смена логина и пароля
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
 void ACommandPasswd::help(Stream *console)
 {
      console->println(F("Change username and password for TCP telnet terminal"));
 }
///--------------------------------------------------------------------------------------






void ACommandPasswd::execute(const Terminal::AParameters &param, Stream *console)
{
    const String login = param[0];
    const String passwd = param[1];

    if (!login.length() || !passwd.length())
    {
        console->println(F("Sorry, passwords do not correct, use:"));
        console->println(F("passwd <user> <password>"));
        return;
    }

    sys::AStorage storage;
    storage.writeString(sys::hash_const("terminal-login"), login);
    storage.writeString(sys::hash_const("terminal-passwd"), passwd);
    console->println(F("passwd: all authentification terminal updated successfully"));
}