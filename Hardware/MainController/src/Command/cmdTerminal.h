#pragma once

#include "../Terminal/trmSecurity.h"
#include "../Terminal/trmTerminal.h"
#include "../Terminal/trmProviderServer.h"
#include "../Terminal/trmProviderSerial.h"

#include "cmdTest.h"
#include "cmdHelp.h"
#include "cmdEthernet.h"
#include "cmdDateTime.h"
#include "cmdMemory.h"
#include "cmdStorage.h"
#include "cmdRelay.h"
#include "cmdPasswd.h"
#include "cmdMqtt.h"



namespace Terminal
{
    ///--------------------------------------------------------------------------------------







     ///=====================================================================================
    ///
    /// Терминал контроллера
    /// работает с ком портом и через сетевой интерфейс
    /// 
    ///--------------------------------------------------------------------------------------

    //описаник команд
    using ACommands = Terminal::TCommands<
                                            Command::ACommandTest,
                                            Command::ACommandHelp,
                                            Command::ACommandRelay, 
                                            Command::ACommandDate,
                                            Command::ACommandTime,
                                            Command::ACommandIPConfig,
                                            Command::ACommandMqtt,
                                            Command::ACommandMemory,
                                            Command::ACommandStorage,
                                            Command::ACommandPasswd >;



    //терминал для обработки внешних команд
    using ATerminal = Terminal::TTerminal<ACommands>;


    //поставщики потоков для терминала
    using AProviderServer = Terminal::TProviderServer<ATerminal, Terminal::ASecurityLogin>;
    using AProviderSerial = Terminal::TProviderSerial<ATerminal, Terminal::ASecurityFree>;





     ///=====================================================================================
    ///
    /// Переменные для доступа
    /// это глобальные переменные, 
    /// 
    ///--------------------------------------------------------------------------------------
    extern ATerminal        terminal;
    extern AProviderServer  providerServer;
    extern AProviderSerial  providerSerial;
    ///--------------------------------------------------------------------------------------






    ///--------------------------------------------------------------------------------------

}
