#pragma once
#include <Stream.h>

namespace Terminal
{
    ///--------------------------------------------------------------------------------------








     ///=====================================================================================
    ///
    /// Система ввода пользователя и пароля
    /// проверка аутентификации
    ///
    ///--------------------------------------------------------------------------------------
    class ASecurity
    {
        public:

            enum EAuthentication
            {
                allow,      //доступ разрешен
                deny,       //доступ запрещен
                processing  //идет процесс авторизации
            };
    };
    ///--------------------------------------------------------------------------------------






     ///=====================================================================================
    ///
    /// без проверки аутентификации
    /// 
    ///--------------------------------------------------------------------------------------
    class ASecurityFree
        :
            public ASecurity
    {
        public:

            EAuthentication process(Stream *stream) { return allow; };
    };
    ///--------------------------------------------------------------------------------------






     ///=====================================================================================
    ///
    /// Проверка аутентификации
    /// через логин пароль
    /// 
    ///--------------------------------------------------------------------------------------
    class ASecurityLogin
        :
            public ASecurity
    {
        public:
            ASecurityLogin(const bool echo = true);

            EAuthentication process(Stream *stream);

        private:

            //текущее состояние ввода аккаунта
            enum EStatus : unsigned char
            {
                ready,          //готов к вводу данных
                readyLogin,     ///готовность в воду логина. удаление специсимволов
                inputLogin,     //ввод логина
                readyPasswd,    //ггтовность ввода пароля
                inputPasswd     //ввод пароля
            };

            EStatus mStatus     = { ready };    //текущее состояния ввода
            Stream* mSession    = { nullptr };  //поток который начал аутентификацию
            String  mLogin;                     //вводные данные имя пользователя
            String  mPasswd;                    //пароль
            bool    mEcho               = { true };     //дублирование ввода в обратный поток
            unsigned long mTimeSession  = { 0 }; //время ввода логин пароля

        private:

            EAuthentication cmd_ready       (Stream *stream);
            EAuthentication cmd_readyLogin  (Stream *stream);
            EAuthentication cmd_inputLogin  (Stream *stream);
            EAuthentication cmd_readyPasswd (Stream *stream);
            EAuthentication cmd_inputPasswd (Stream *stream);

            EAuthentication completed       (Stream *stream); //выполнен ввод пароля

            void reset(); //сброс всего состояния
            bool isOutTimeSession() const; //вышло время 
            bool isAuthentication() const; //проверка, логин пароль введен правельно или нет

    };

}