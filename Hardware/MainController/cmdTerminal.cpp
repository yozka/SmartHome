#include "cmdTerminal.h"



namespace Terminal
{
    ATerminal terminal;
    AProviderServer  providerServer(terminal);
    AProviderSerial  providerSerial(terminal);
}