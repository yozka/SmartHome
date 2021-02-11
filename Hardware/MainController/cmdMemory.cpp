#include "cmdMemory.h"
#include "configuration.h"
#include <Arduino.h>

using namespace Command;
///--------------------------------------------------------------------------------------







 ///=====================================================================================
///
/// Команда вывод информации по внутренней памяти девайса
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
 void ACommandMemory::help(Stream *console)
 {
      console->println(F("Reported memory usage"));
 }
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// Get RAM size in Bytes.
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
unsigned int getRamSize()
{
    return RAMEND - RAMSTART + 1;
}
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// Get data section size in Bytes.
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
unsigned int getDataSectionSize()
{
    extern unsigned int __data_start;
    extern unsigned int __data_end;

    return (unsigned int)&__data_end - (unsigned int)&__data_start;
}
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// Get BSS section size in Bytes.
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
unsigned int getBssSectionSize()
{
    extern unsigned int __bss_start;
    extern unsigned int __bss_end;

    return (unsigned int)&__bss_end - (unsigned int)&__bss_start;
}
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// Get stack size in Bytes.
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
unsigned int getStackSize()
{
    return (unsigned int)RAMEND - (unsigned int)SP;
}
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// Get heap size in Bytes.
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
unsigned int getHeapSize()
{
    extern unsigned int __heap_start;
    extern unsigned int *__brkval;

    if (__brkval == NULL) {
        return 0;
    } else {
        return (unsigned int)__brkval - (unsigned int)&__heap_start;
    }
}
///--------------------------------------------------------------------------------------





 ///=====================================================================================
///
/// Get free memory size in Bytes.
/// 
/// 
/// 
///--------------------------------------------------------------------------------------
unsigned int getFreeMemSize()
{
    return getRamSize() -
           getStackSize() -
           getHeapSize() -
           getBssSectionSize() -
           getDataSectionSize();
}
///--------------------------------------------------------------------------------------





void ACommandMemory::execute(const Terminal::AParameters &param, Stream *console)
{
    console->print(F(" SRAM size: "));
    console->print(getRamSize());
    console->println(F(" Bytes"));

    console->print(F(".data size: "));
    console->print(getDataSectionSize());
    console->println(F(" Bytes"));

    console->print(F(" .bss size: "));
    console->print(getBssSectionSize());
    console->println(F(" Bytes"));

    console->print(F("Stack size: "));
    console->print(getStackSize());
    console->println(F(" Bytes"));

    console->print(F(" Heap size: "));
    console->print(getHeapSize());
    console->println(F(" Bytes"));

    for (int i = 0; i < 25; i++)
    {
        console->write('-');
    }
    console->println();

    console->print(F("  Free mem: "));
    console->print(getFreeMemSize());
    console->println(F(" Bytes"));
}