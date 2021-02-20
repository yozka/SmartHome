#pragma once
#include <Arduino.h>

namespace sys
{
    ///--------------------------------------------------------------------------------------








     ///=====================================================================================
    ///
    /// Система для хранения данных в энергонезависемой памяти
    /// данные хранятся ввдие ключ - значение
    /// каждая пара находится в чанке
    /// 
    ///--------------------------------------------------------------------------------------
    class AStorage
    {
        using Key = unsigned long;
        public:

            void dump(Stream *console) const; //вывести информацию о внутреннм состоянии памяти
            void erase(); //удалить все

        public:

            template <typename T>
            bool read(const Key &key, T &value) const //чтение данных ввиде блоба
            {
                return readRaw(key, reinterpret_cast<uint8_t*>(&value), sizeof(T));
            }
            
            template <typename T>
            void write(const Key &key, const T &value) //запись данных ввиде блоба
            {
                writeRaw(key, reinterpret_cast<const uint8_t*>(&value), sizeof(T));
            }

        public:


            bool        readRaw     (const Key &key, uint8_t *data, const size_t dataSize) const; //чтение бинарных данных
            String      readString  (const Key &key, const String &defValue = {}) const; //чтение строчки
            uint32_t    read_uint32 (const Key &key, const uint32_t &defValue = 0) const;

            void writeRaw       (const Key &key, const uint8_t *data, const size_t dataSize); //запись бинарных данных
            void writeString    (const Key &key, const String &value); //запись строчки
            void write_uint32   (const Key &key, const uint32_t &value); 

        private:

            //один чанк с данными
            struct Chunk
            {
                enum TypeChunk : uint8_t
                {
                    none    = 0,
                    eRaw    = 1,
                    eString = 2,
                    eInt    = 3,
                    eFloat  = 4,
                    eUint32 = 5
                    
                };

                bool        valid       = { false }; //валидный чанк
                Key         key         = { 0 }; //ключь данных чанка
                size_t      address     = { 0 }; //адрес чанка
                size_t      sizeChunk   = { 0 }; //размер тела чанка (без заголовка)
                TypeChunk   typeChunk   = { none }; //тип данных который содежит чанк

                Chunk() = default;
                Chunk(const Key &key, const size_t address, const TypeChunk typeChunk, const size_t sizeChunk)
                            : valid(true), key(key), address(address), typeChunk(typeChunk), sizeChunk(sizeChunk) {};

                size_t fragSize()const; //размер фрагментирования чанка
                String typeString()const; //тип чанка ввиде строчки
            };
            //


            Chunk findChunk(const Key &key) const; //поиск ноды с данными
            Chunk findFreeChunk(const size_t sizeChunk) const; //поиск свободного чанка с заданным размером
            Chunk endChunk() const; //самый последний чанк

            Chunk emplaceChunk(const Key &key, const Chunk::TypeChunk typeChunk, const size_t sizeChunk); //создание чанка
            Chunk createChunk(const size_t sizeChunk); //создание чанки и чанка заглушки
            void clearChunk(const Chunk &chunk); //очистка чанка


            template <typename T>
            T read(const Key &key, const Chunk::TypeChunk typeChunk, const T &defValue) const;

            template <typename T>
            void write(const Key &key, const Chunk::TypeChunk typeChunk, const T &value);
            

    };
    ///--------------------------------------------------------------------------------------





    ///--------------------------------------------------------------------------------------
}