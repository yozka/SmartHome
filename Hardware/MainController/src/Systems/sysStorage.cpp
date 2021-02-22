#include "sysStorage.h"
#include "sysUtils.h"
#include <avr/eeprom.h>
///--------------------------------------------------------------------------------------


///--------------------------------------------------------------------------------------
using namespace sys;
///--------------------------------------------------------------------------------------

/*
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
*/
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)



///--------------------------------------------------------------------------------------
namespace Settings
{
    constexpr uint8_t magic = 82;
}
struct THeader
{
    uint32_t    key;        //ключь данных
    uint16_t    sizeChunk;  //размер чанка
    uint8_t     typeChunk;  //тип данных в которых содержится чанк
    uint8_t     magic;      //контрольные данные
};
///--------------------------------------------------------------------------------------




//чтение бинарных данных
bool AStorage::readRaw(const Key &key, uint8_t *data, const size_t dataSize) const
{
    if (const auto chunk = findChunk(key); chunk.valid && chunk.typeChunk == Chunk::eRaw)
    {
        const void *adr = chunk.address + sizeof(THeader);
        
        size_t size = 0;
        eeprom_read_block(&size, adr, sizeof(size_t));
        if (size == dataSize)
        {
            adr += sizeof(size_t);
            eeprom_read_block(data, adr, size);
            return true;        
        }
    }
    return false;
}


//запись бинарных данных
void AStorage::writeRaw(const Key &key, const uint8_t *data, const size_t dataSize)
{
    const auto size =  dataSize + sizeof(size_t);
    if (auto chunk = emplaceChunk(key, Chunk::eRaw, size); chunk.valid)
    {
        uint16_t adr = chunk.address + sizeof(THeader);
        eeprom_update_block(&dataSize, adr, sizeof(size_t));
        eeprom_update_block(data, adr + sizeof(size_t), dataSize);
    }
}





String AStorage::readString(const Key &key, const String &defValue) const
{
    if (const auto chunk = findChunk(key); chunk.valid && chunk.typeChunk == Chunk::eString)
    {
        const void *adr = chunk.address + sizeof(THeader);
        String value;
        while (char data = eeprom_read_byte(adr++))
        {
            value.concat(data);
        }
        return value;
    }
    return defValue;
}





void AStorage::writeString(const Key &key, const String &value)
{
    const auto length = value.length();
    if (auto chunk = emplaceChunk(key, Chunk::eString, length + 1); chunk.valid)
    {
        uint16_t adr = chunk.address + sizeof(THeader);
        for (int i = 0; i < length; i++)
        {
            eeprom_update_byte(adr++, value[i]);
        }
        eeprom_update_byte(adr, 0);
    }
}



//
uint32_t AStorage::read_uint32(const Key &key, const uint32_t &defValue = 0) const
{
    return read<uint32_t>(key, Chunk::eUint32, defValue);
}
void AStorage::write_uint32(const Key &key, const uint32_t &value)
{
    write<uint32_t>(key, Chunk::eUint32, value);
}
//


//
uint16_t AStorage::read_uint16(const Key &key, const uint16_t &defValue = 0) const
{
    return read<uint16_t>(key, Chunk::eUint16, defValue);
}
void AStorage::write_uint16(const Key &key, const uint16_t &value)
{
    write<uint16_t>(key, Chunk::eUint16, value);
}
//



//
uint8_t AStorage::read_uint8(const Key &key, const uint8_t &defValue = 0) const
{
    return read<uint8_t>(key, Chunk::eUint8, defValue);
}
void AStorage::write_uint8(const Key &key, const uint8_t &value)
{
    write<uint8_t>(key, Chunk::eUint8, value);
}
//



template <typename T>
T AStorage::read(const Key &key, const AStorage::Chunk::TypeChunk typeChunk, const T &defValue) const
{
    if (const auto chunk = findChunk(key); chunk.valid && chunk.typeChunk == typeChunk)
    {
        T value;
        eeprom_read_block(&value, chunk.address + sizeof(THeader), sizeof(T));
        return value;
    }
    return defValue;
}




template <typename T>
void AStorage::write(const Key &key, const AStorage::Chunk::TypeChunk typeChunk, const T &value)
{
    if (auto chunk = emplaceChunk(key, typeChunk, sizeof(T)); chunk.valid)
    {
        eeprom_update_block(&value, chunk.address + sizeof(THeader), sizeof(T));
    }
}




//поиск чанка с данными
AStorage::Chunk AStorage::findChunk(const Key &key) const
{

    THeader header;
    for (size_t src = 0; src < E2END; )
    {
        eeprom_read_block(&header, src, sizeof(THeader));

        if (header.magic != Settings::magic || header.sizeChunk == 0)
        {
            //разрушена целостность данных
            return {};
        }

        if (header.key == key)
        {
            //даные нашли
            return {header.key, src, header.typeChunk, header.sizeChunk};
        }

        //перейдем к следующим данным
        src += sizeof(THeader);
        src += header.sizeChunk;
    }
    return {};
}


//поиск свободного чанка с заданным размером
 AStorage::Chunk AStorage::findFreeChunk(const size_t sizeChunk) const
 {
    THeader header;
    for (size_t src = 0; src < E2END; )
    {
        eeprom_read_block(&header, src, sizeof(THeader));

        if (header.magic != Settings::magic || header.sizeChunk == 0)
        {
            //разрушена целостность данных
            return {};
        }

        if (header.key == 0 && header.typeChunk == Chunk::none &&
            header.sizeChunk >= sizeChunk)
        {
            //подходящий пустой чанк нашли
            return {header.key, src, header.typeChunk, header.sizeChunk};
        }

        //перейдем к следующим данным
        src += sizeof(THeader);
        src += header.sizeChunk;
    }
    return {};
 }



//самый последний чанк
AStorage::Chunk AStorage::endChunk() const
 {
    THeader header;
    size_t adr = 0;
    for (size_t src = 0; src < E2END; )
    {
        eeprom_read_block(&header, src, sizeof(THeader));

        if (header.magic != Settings::magic || header.sizeChunk == 0)
        {
            //разрушена целостность данных
            //либо это пустой блок
            adr = src;
            break;
        }

        //перейдем к следующим данным
        src += sizeof(THeader);
        src += header.sizeChunk;
    }
    return {0, adr, Chunk::none, 0};
 }



//создание чанка
AStorage::Chunk AStorage::emplaceChunk(const Key &key, const Chunk::TypeChunk typeChunk, const size_t sizeChunk)
{
    auto chunk = findChunk(key);
    if (chunk.valid)
    {
        //чанк найден
        //и он маленького размера, удалим его
        if (chunk.sizeChunk < sizeChunk)
        {
            clearChunk(chunk);
            chunk = {};
        }
    }

    if (!chunk.valid)
    {
        chunk = findFreeChunk(sizeChunk);
        if (!chunk.valid)
        {
            chunk = createChunk(sizeChunk);
        }
    }

    THeader header;
    header.key = key;
    header.sizeChunk = chunk.sizeChunk;
    header.typeChunk = typeChunk;
    header.magic = Settings::magic;
    eeprom_update_block(&header, chunk.address, sizeof(THeader));
    return {header.key, chunk.address, header.typeChunk, header.sizeChunk};
}



//очистка чанка
void AStorage::clearChunk(const AStorage::Chunk &chunk)
{
    if (chunk.valid)
    {
        THeader header;
        header.key = 0;
        header.sizeChunk = chunk.sizeChunk;
        header.typeChunk = Chunk::none;
        header.magic = Settings::magic;
        eeprom_update_block(&header, chunk.address, sizeof(THeader));
    }
}



//создание чанки и чанка заглушки
AStorage::Chunk AStorage::createChunk(const size_t sizeChunk)
{
    auto chunk = endChunk();
    chunk.sizeChunk = sizeChunk;

    //запишем закрывающий последний чанк
    THeader header;
    header.key = 0;
    header.sizeChunk = 0;
    header.typeChunk = Chunk::none;
    header.magic = Settings::magic;
    eeprom_update_block(&header, chunk.address + sizeChunk, sizeof(THeader));
    
    return chunk;  
 }




//удалить все
void AStorage::erase()
{
    THeader header;
    header.key = 0;
    header.sizeChunk = 0;
    header.typeChunk = Chunk::none;
    header.magic = Settings::magic;
    eeprom_update_block(&header, 0, sizeof(THeader));
}


//удалить данные по ключу
void AStorage::erase(const Key &key)
{
    if (const auto chunk = findChunk(key); chunk.valid)
    {
        THeader header;
        header.key = 0;
        header.sizeChunk = chunk.sizeChunk;
        header.typeChunk = Chunk::none;
        header.magic = Settings::magic;
        eeprom_update_block(&header, chunk.address, sizeof(THeader));
    }
}



//вывести информацию о внутреннм состоянии памяти
 void AStorage::dump(Stream *console) const
 {
    const auto print = [&console]<class T>(const T &data, const int count)
    {
        String str(data);
        for (int i = str.length(); i < count; i++)
        {
            console->print(' ');
        }
        console->print(str);
    };
    //
    
    const auto printLine = [&console](const char s = '=')
    {
        for (int i = 0; i < 70; i++) console->print(s);
        console->println();
    };

    //отрисуем заголовок
    printLine();
    print(F("Key"),     12); console->print('|'); 
    print(F("Addr"),    5);  console->print('|');
    print(F("Size"),    5);  console->print('|');
    print(F("Frag"),    5);  console->print('|');
    print(F("Type"),    8);  console->print('|');
    console->println(F(" DATA"));
    printLine();

    size_t src = 0;
    while (src < E2END)
    {
        THeader header;
        eeprom_read_block(&header, src, sizeof(THeader));
        if (header.magic != Settings::magic)
        {
            break; //разрушена целостность данных
        }
        const Chunk chunk(header.key, src, header.typeChunk, header.sizeChunk);
        //перейдем к следующим данным
        src += sizeof(THeader);
        src += header.sizeChunk;
        //

        //выведем информацию
        // ключ, адрес чанка, размер чанка, дефрагментация, тип чанка, какие данные там находятся 
        print(chunk.key, 12);           console->print('|'); 
        print(chunk.address, 5);        console->print('|'); //адрес чанка
        print(chunk.sizeChunk, 5);      console->print('|'); //размер чанка
        print(chunk.fragSize(), 5);     console->print('|'); //фрагментация чанка
        print(chunk.typeString(), 8);   console->print('|'); //тип чанка

        console->println();

    }
    printLine('-');
    const size_t freeFlash = E2END - src;
    console->print(F("Free flash memory: ")); console->print(freeFlash); console->println(F(" Bytes"));
 }



 
//размер фрагментирования чанка
size_t AStorage::Chunk::fragSize()const
{
    return 0;
}

//тип чанка ввиде строчки
String AStorage::Chunk::typeString()const
{
    if (!valid)
    {
        return F("invalid");
    }
    if (key == 0 && typeChunk == none)
    {
        return F("FREE"); //свободный чанк
    }

    switch (typeChunk)
    {
        case none       : return F("none");
        case eRaw       : return F("Raw");
        case eString    : return F("String");
        case eInt       : return F("Int");
        case eFloat     : return F("Float");
        case eUint32    : return F("Uint32");
        case eUint16    : return F("Uint16");
        case eUint8     : return F("Uint8");
    }
    return String(typeChunk);
}
