#pragma once
#include <Arduino.h>

namespace Terminal
{
    ///--------------------------------------------------------------------------------------





     ///=====================================================================================
    ///
    /// Разбор параметров
    /// 
    /// 
    ///--------------------------------------------------------------------------------------
    class AParameters
    {
        public:
            AParameters(const String &data);
         
            String source() const; //Возвратить все данные которые есть
            
            bool isEmpty() const; //Проверка что параметры отсутствуют

        public:

            int conv2d(const int index) const; //взять два символа и переделать их в число

        public:

            bool isKey(const char key) const; //проверить есть ключь или нет
            String queryValue(const String &key) const; //извлечь параметр после ключа
            String queryIndex(const int index) const; //извлечь параметр по его порядковому номеру индексу (0 - первыое слово, 1 - второе, итд.)

        public:
    
            const String operator[](const int index) const
            {
                return queryIndex(index);
            }

        private:

            String mData;

    };

}