#include "cmdHelp.h"


using namespace Command;


void ACommandHelp::execute(const Terminal::AParameters &param, Stream &console)
{
    console.println(F("Heeeelpp!"));
}