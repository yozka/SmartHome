#include "cmdTest.h"
#include "cmdTerminal.h"
#include "sysHash.h"

using namespace Command;




void ACommandTest::execute(const Terminal::AParameters &param, Stream *console)
{
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
    console->println(sys::hash(param.source()));
}