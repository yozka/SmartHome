#include "cmdHelp.h"
#include "cmdTerminal.h"
#include "user_config.h"

using namespace Command;


/*
template<typename Char, Char ... Chars> struct static_string{};

template<typename Char, Char ... Chars1, Char ... Chars2>
constexpr static_string<Char, Chars1 ..., Chars2 ... > operator+(
    const static_string<Char, Chars1 ... >& str1,
    const static_string<Char, Chars2 ... >& str2) 
{
    return static_string<Char, Chars1 ..., Chars2 ...>{};
}


template<typename Char, Char ch, Char ... Chars>
std::basic_ostream<Char>& operator<<(std::basic_ostream<Char>& bos, const static_string<Char, ch, Chars ...>& str) {
    bos << ch << static_string<Char, Chars ... >{};
    return bos;
}

template<typename Char>
std::basic_ostream<Char>& operator<<(std::basic_ostream<Char>& bos, const static_string<Char>& str) {
    return bos;
}



template<typename Char, Char ... Chars>
constexpr static_string<Char, Chars ... > operator"" _ss() 
{
    return static_string<Char, Chars ... >{};
};



constexpr int operator "" _ss(const char* str, const size_t size)
{
    return 0;
}
*/

void test(Stream *console)
{
    //constexpr auto str1 = "abc111111111111111122222222222222222"_ss;
    //constexpr auto str2 = "def"_ss;
    //onstexpr auto str3 = str1 + str2 + str1;

}



void ACommandHelp::execute(const Terminal::AParameters &param, Stream *console)
{
    test(console);

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
        for (int i = name.length(); i < Config::Command::Help::margin; i++)
        {
            name += sys::SPACE;
        }
        console->print(name);
        cmd.help(console);
    };
    Terminal::ACommands::call(iterator);

}