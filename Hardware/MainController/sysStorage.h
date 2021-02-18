#pragma once
#include <Arduino.h>

namespace sys
{
    ///--------------------------------------------------------------------------------------








     ///=====================================================================================
    ///
    /// Система для хранения данных в энергонезависемой памяти
    /// 
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    class AStorage
    {
        using Key = unsigned long;
        public:

            void dump(Stream *console) const; //вывести информацию о внутреннм состоянии памяти
  
        public:

            String readString(const Key &key, const String &defValue = {}) const; //чтение строчки


        public:
            void writeString(const Key &key, const String &value); //запись строчки

        private:

            //один чанк с данными
            struct Chunk
            {
                enum TypeChunk : uint8_t
                {
                    none    = 0,
                    eString = 1
                };

                bool        valid       = { false }; //валидный чанк
                Key         key         = { 0 }; //ключь данных чанка
                size_t      address     = { 0 }; //адрес чанка
                size_t      sizeChunk   = { 0 }; //размер тела чанка (без заголовка)
                TypeChunk   typeChunk   = { none }; //тип данных который содежит чанк

                Chunk() = default;
                Chunk(const Key &key, const size_t address, const TypeChunk typeChunk, const size_t sizeChunk)
                            : valid(true), key(key), address(address), typeChunk(typeChunk), sizeChunk(sizeChunk) {};
            };
            //


            Chunk findChunk(const Key &key) const; //поиск ноды с данными
            Chunk findFreeChunk(const size_t sizeChunk) const; //поиск свободного чанка с заданным размером
            Chunk endChunk() const; //самый последний чанк

            Chunk emplaceChunk(const Key &key, const Chunk::TypeChunk typeChunk, const size_t sizeChunk); //создание чанка
            Chunk createChunk(const size_t sizeChunk); //создание чанки и чанка заглушки
            void clearChunk(const Chunk &chunk); //очистка чанка
    };
    ///--------------------------------------------------------------------------------------





    ///--------------------------------------------------------------------------------------
}