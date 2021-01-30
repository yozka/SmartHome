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