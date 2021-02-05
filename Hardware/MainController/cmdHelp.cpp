#include "cmdHelp.h"
#include "commonTerminal.h"

using namespace Command;


void ACommandHelp::execute(const Terminal::AParameters &param, Stream *console)
{
    console->println(F("For information about a specific command, type HELP <command name>"));
    console->print(F("Date frimware: "));
    console->print(F(__DATE__ ));
    console->print(' ');
    console->println(F(__TIME__ ));
  
    const auto iterator = [&console]<class T>(const T &cmd)
    {
        String name = cmd.name();
        if (name == ACommandHelp::name())
        {
            //справку о самого себе не показываем
            return;
        }
        name.toUpperCase();
        for (int i = name.length(); i < Settings::helpMargin; i++)
        {
            name += SPACE;
        }
        console->print(name);
        cmd.help(console);
    };
    Common::ACommands::call(iterator);

}