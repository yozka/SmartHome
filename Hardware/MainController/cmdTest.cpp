#include "cmdTest.h"
#include "cmdTerminal.h"
#include "sysHash.h"
#include "sysStorage.h"

using namespace Command;




void ACommandTest::execute(const Terminal::AParameters &param, Stream *console)
{
    /*
    unsigned long hs1 = sys::hash_const("Test string from cashed");
    console->println(hs1);

    unsigned long hs2 = "Test string from cashed"_hash;
    console->println(hs2);



    unsigned long hs3 = sys::hash(F("Test string from cashed"));
    console->println(hs3);


    const String text = F("Test string from cashe1");
    unsigned long hs4 = sys::hash(text);
    console->println(hs4);


    console->println();
    console->print(param.source());
    console->print(' ');
    console->println(sys::hash(param.source()));*/

    String key = param[0];
    String value = param[1];

    if (key.length() == 0)
    {
        return;
    }

    sys::AStorage storage;
    if (value.length() == 0)
    {
        //прочитаем параметр
        console->print(key);
        console->print(F(" -> "));
        console->println(storage.readString(sys::hash(key)));
    }
    else
    {
        //сохраним параметр
        storage.writeString(sys::hash(key), value);
    }
}