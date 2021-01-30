#include "trmSecurity.h"
#include "sysHelper.h"


using namespace Terminal;




//сброс состояния аутентификации
void ASecurityLogin::reset()
{
    mStatus = ready;
    mSession = nullptr;
    mLogin = "";
    mPasswd = "";
}




//процесс аутентификации
ASecurity::EAuthentication ASecurityLogin::process(Stream *stream)
{
    switch (mStatus)
    {
        case ready          : return cmd_ready(stream);
        case inputLogin     : return cmd_inputLogin(stream);
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

    int i = 5;
    while (stream->available() && i > 0)
    {
        stream->read();
        i--;
    }
    if (stream->available())
    {
        //данные идут сплошным потоком, это ненормально
        reset();
        return deny;
    }

    //готовы ждать ввода данных
    mStatus = inputLogin;
    mSession = stream;
    mLogin = "";
    mPasswd = "";

    stream->println();
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
    
    if (!stream->available())
    {
        reset();
        return deny;
    }

    const char data = stream->read();
    if (data == CR || data == LF)
    {
        //нажали ентер, подтвердили выбора имени пользователя
        return completedLogin(stream);
    }

    stream->write(data);
    mLogin += data;
    if (mLogin.length() > Settings::lengthAuth)
    {
        reset();
        return deny;
    }

    return processing;
}



//Выполнен вод имя пользователя
ASecurity::EAuthentication ASecurityLogin::completedLogin(Stream *stream)
{
    mLogin.trim();
    if (mLogin.length() == 0)
    {
        reset();
        return deny;
    }

    //переключаем на ввод пароля
    stream->println();
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
    
    if (!stream->available())
    {
        reset();
        return deny;
    }

    const char data = stream->read();
    if (data == CR || data == LF)
    {
        //нажали ентер, подтвердили выбора имени пользователя
        return completedPasswd(stream);
    }

    stream->write('*');
    mPasswd += data;
    if (mPasswd.length() > Settings::lengthAuth)
    {
        reset();
        return deny;
    }

    return processing;
}



//выполнен ввод пароля
ASecurity::EAuthentication ASecurityLogin::completedPasswd(Stream *stream)
{
    mPasswd.trim();
    if (isAuthentication())
    {
        stream->println();
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
    return mLogin == F("admin") && mPasswd == F("123");
}
