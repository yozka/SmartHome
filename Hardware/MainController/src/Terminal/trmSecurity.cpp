#include "trmSecurity.h"

#include "../Systems/sysUtils.h"
#include "../Systems/sysStorage.h"
#include "../Systems/sysHash.h"
#include "../../user_config.h"


using namespace Terminal;


ASecurityLogin::ASecurityLogin(const bool echo)
    :
        mEcho(echo)
{

}




//сброс состояния аутентификации
void ASecurityLogin::reset()
{
    mStatus = ready;
    mSession = nullptr;
    mLogin = "";
    mPasswd = "";
    mTimeSession = 0;
}



//вышло время 
bool ASecurityLogin::isOutTimeSession() const
{
    return mTimeSession != 0 && mTimeSession < millis();
}



//процесс аутентификации
ASecurity::EAuthentication ASecurityLogin::process(Stream *stream)
{
    if (isOutTimeSession())
    {
        reset();
        return deny;
    }

    switch (mStatus)
    {
        case ready          : return cmd_ready(stream);
        case readyLogin     : return cmd_readyLogin(stream);
        case inputLogin     : return cmd_inputLogin(stream);
        case readyPasswd    : return cmd_readyPasswd(stream);
        case inputPasswd    : return cmd_inputPasswd(stream);
    }
    return processing;
}



//начало ввода данных
ASecurity::EAuthentication ASecurityLogin::cmd_ready(Stream *stream)
{
    if (mSession || mSession == stream)
    {
        //уже идет аутентификация, от другой сессии, сбросим все
        reset();
        return deny;
    }

    //готовы ждать ввода данных
    mStatus = readyLogin;
    mSession = stream;
    mLogin = "";
    mPasswd = "";
    mTimeSession = millis() + Config::Network::Terminal::timeLoginSession;
    return processing;
}


//начинаем ожидать готовность на ввод логина
ASecurity::EAuthentication ASecurityLogin::cmd_readyLogin(Stream *stream)
{
    if (stream == nullptr || stream != mSession)
    {
        //пришла другая сессия
        reset();
        return deny;
    }

    if (stream->available() > 0 )
    {
        //есть данные, ждем пока все придет
        stream->read();
        return processing;
    }

    //дальше идет ввод логина
    mStatus = inputLogin;
    mLogin = "";
    mPasswd = "";
    if (mEcho)
    {
        stream->println();
    }
    stream->print(F("Login: "));
    return processing;
}



//процесс вода логина
ASecurity::EAuthentication ASecurityLogin::cmd_inputLogin(Stream *stream)
{
    if (stream == nullptr || stream != mSession)
    {
        //начали ввод логина с другой сессии
        reset();
        return deny;
    }
    
    
    if (stream->available() <= 0)
    {
        //данных нет, ожидаем
        return processing;
    }

    const char data = stream->read();
    if (data == sys::CR || data == sys::LF)
    {
        //нажали ентер, подтвердили выбора имени пользователя
        mStatus = readyPasswd;
        return processing;
    }

    if (mEcho)
    {
        stream->write(data);
    }
    mLogin.concat(data);
    if (mLogin.length() > Config::Network::Terminal::lengthAuth)
    {
        //превысели допустимую длину пароля
        reset();
        return deny;
    }
    return processing;
}



//Выполнен вод имя пользователя
ASecurity::EAuthentication ASecurityLogin:: cmd_readyPasswd (Stream *stream)
{
    if (stream == nullptr || stream != mSession)
    {
        //начали ввод с другой сессии
        reset();
        return deny;
    }

    if (stream->available() > 0 )
    {
        //есть данные, ждем пока все придет
        stream->read();
        return processing;
    }

    //переключаем на ввод пароля
    if (mEcho)
    {
        stream->println();
    }
    stream->print(F("Password: "));
    mStatus = inputPasswd;
    return processing;
}


ASecurity::EAuthentication ASecurityLogin::cmd_inputPasswd(Stream *stream)
{
    if (stream == nullptr || stream != mSession)
    {
        //начали ввод пароля с другой сессии
        reset();
        return deny;
    }
    
    if (stream->available() <= 0)
    {
        //данных нет
        return processing;
    }

    const char data = stream->read();
    if (data == sys::CR || data == sys::LF)
    {
        //нажали ентер, подтвердили выбор
        return completed(stream);
    }

    if (mEcho)
    {
        stream->write('*');
    }
    mPasswd.concat(data);
    if (mPasswd.length() > Config::Network::Terminal::lengthAuth)
    {
        reset();
        return deny;
    }

    return processing;
}



//выполнен ввод пароля
ASecurity::EAuthentication ASecurityLogin::completed(Stream *stream)
{
    mLogin.trim();
    mPasswd.trim();
    if (isAuthentication())
    {
        if (mEcho)
        {
            stream->println();
        }
        stream->println(F("Granted"));
        reset();
        return allow;
    }
    reset();
    return deny;
}



//проверка, логин пароль введен правельно или нет
bool ASecurityLogin::isAuthentication() const
{
    sys::AStorage storage;
    const auto login =  storage.readString(sys::hash_const("terminal-login"));
    const auto passwd = storage.readString(sys::hash_const("terminal-passwd"));

    return mLogin  == login && mPasswd == passwd && login.length() > 0 && passwd.length() > 0;
}
