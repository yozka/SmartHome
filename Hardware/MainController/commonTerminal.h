#pragma once

#include "trmSecurity.h"
#include "trmTerminal.h"
#include "trmProviderServer.h"
#include "trmProviderSerial.h"

#include "cmdHelp.h"
#include "cmdEthernet.h"
#include "cmdDateTime.h"


namespace Common
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
                                            Command::ACommandHelp,
                                            Command::ACommandDate,
                                            Command::ACommandTime,
                                            Command::ACommandIPConfig,
                                            Command::TestCommandA, 
                                            Command::TestCommandB>;



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
