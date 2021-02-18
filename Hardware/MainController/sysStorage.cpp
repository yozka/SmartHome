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
    DEBUG_PRINTLN(key);
    DEBUG_PRINTLN(value);

    const auto length = value.length();
    if (auto chunk = emplaceChunk(key, Chunk::eString, length + 1); chunk.valid)
    {
        DEBUG_PRINT(F("Write data string: "));
        uint16_t adr = chunk.address + sizeof(THeader);
        DEBUG_PRINTLN(adr);
        for (int i = 0; i < length; i++)
        {
            eeprom_update_byte(adr++, value[i]);
        }
        eeprom_update_byte(adr, 0);
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
    DEBUG_PRINTLN(F("emplace chunk"));

    auto chunk = findChunk(key);
    if (chunk.valid)
    {
        DEBUG_PRINTLN(F("find chunk valid"));
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
        DEBUG_PRINTLN(F("Not old chunk"));
        chunk = findFreeChunk(sizeChunk);
        if (!chunk.valid)
        {
            chunk = createChunk(sizeChunk);
        }
    }

    DEBUG_PRINTLN(F("Write chunk"));
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
    DEBUG_PRINT(F("Create chunk: "));
    auto chunk = endChunk();
    chunk.sizeChunk = sizeChunk;
    DEBUG_PRINT(chunk.address);

    //запишем закрывающий последний чанк
    THeader header;
    header.key = 0;
    header.sizeChunk = 0;
    header.typeChunk = Chunk::none;
    header.magic = Settings::magic;
    eeprom_update_block(&header, chunk.address + sizeChunk, sizeof(THeader));
    
    return chunk;  
 }



//вывести информацию о внутреннм состоянии памяти
 void AStorage::dump(Stream *console) const
 {
    const auto print = [&console]<class T>(const T &data, const int count)
    {
        for (auto i = console->print(data); i < count; i++)
        {
            console->print(' ');
        }
    };
    //

    for (size_t src = 0; src < E2END; )
    {
        THeader header;
        eeprom_read_block(&header, src, sizeof(THeader));
        if (header.magic != Settings::magic)
        {
            //разрушена целостность данных
            break;
        }
        //выведем информацию
        // ключ, адрес чанка, размер чанка, дефрагментация, тип чанка, какие данные там находятся 
        console->print(' '); //ключ
        print(header.key, 10);
        console->print('|'); //адрес чанка
        print(src, 10);
        console->print('|'); //размер чанка
        print(header.sizeChunk, 5);
        console->print('|');

        
        
        console->println();
        //перейдем к следующим данным
        src += sizeof(THeader);
        src += header.sizeChunk;
        //
    }
 }