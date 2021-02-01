#include "commonTerminal.h"



namespace Common
{
    ATerminal terminal;
    AProviderServer  providerServer(terminal);
    AProviderSerial  providerSerial(terminal);
}