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
