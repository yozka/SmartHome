#include "sysHash.h"
#include <avr/pgmspace.h>
namespace sys
{

  

    unsigned long hash_pgm(PGM_P input) 
    {
        unsigned char c = pgm_read_byte(input++);   
        return c ? static_cast<unsigned long>(c) ^ 33 * hash_pgm(input) : 5381;
    }



    unsigned long hash (const __FlashStringHelper *ifsh)
    {
        PGM_P p = reinterpret_cast<PGM_P>(ifsh);
        return hash_pgm(p);
    }


    unsigned long hash (const String &input)
    {
        unsigned long dataHash = 5381;
        auto index = input.length();
        while (index) 
        {
            dataHash = (dataHash * 33) ^ input[--index];
        }
        return dataHash;
    }
}