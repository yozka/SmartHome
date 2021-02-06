#include "trmParameters.h"


using namespace Terminal;






 AParameters::AParameters(const String &data)
    :
        mData(data)
{
    mData.trim();
}




//Возвратить все данные которые есть
String AParameters::source() const
{
    return mData;
}



//Проверка что параметры отсутствуют
bool AParameters::isEmpty() const
{
    return mData.length() == 0 ? true : false;
}



//взять два символа и переделать их в число
int AParameters::conv2d(const int index) const
{
    String val = mData.substring(index, index + 2);
    val.trim();
    return val.toInt();
}


//проверить есть ключь или нет
bool AParameters::isKey(const char key) const
{
    return mData.indexOf(key) >= 0 ? true : false;
}


//извлечь параметр после ключа
 String AParameters::queryValue(const String &key) const
 {
    auto idxBegin = mData.indexOf(key + ' ');
    if (idxBegin < 0)
    {
        //ключ не найден
        return {};
    }
    //найден ключ найдем значение
    idxBegin += key.length() + 1;
    auto idxEnd = idxBegin;
    const auto count = mData.length();
    while (mData[idxEnd] != ' ' && idxEnd < count)
    {
        idxEnd++;
    }
    return mData.substring(idxBegin, idxEnd);
 }
