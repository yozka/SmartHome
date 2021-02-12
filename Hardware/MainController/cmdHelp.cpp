#include "cmdHelp.h"
#include "cmdTerminal.h"
#include "configuration.h"

using namespace Command;


void ACommandHelp::execute(const Terminal::AParameters &param, Stream *console)
{
    console->println(F("For information about a specific command, type HELP <command name>"));
    console->print(F("Date frimware: "));
    console->print(F(__DATE__ ));
    console->print(' ');
    console->println(F(__TIME__ ));
    for (int i = 0; i < 70; i++)
    {
        console->write('-');
    }
    console->println();
  
    const auto iterator = [&console]<class T>(const T &cmd)
    {
        String name = cmd.name();
        if (name == ACommandHelp::name())
        {
            //справку о самого себе не показываем
            return;
        }
        name.toUpperCase();
        for (int i = name.length(); i < Configuration::Command::Help::margin; i++)
        {
            name += SPACE;
        }
        console->print(name);
        cmd.help(console);
    };
    Terminal::ACommands::call(iterator);

}