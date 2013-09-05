#ifndef BLITZ_PAYLOAD_H
#define BLITZ_PAYLOAD_H

#include "Arduino.h"
//#include <stdio.h>
#include <string.h>

#ifdef _SAM3XA_
typedef unsigned int blitz_u32;
#else
typedef unsigned long blitz_u32;
#endif

typedef unsigned short blitz_u16;
typedef unsigned char blitz_u8;

class blitz_payload
{
    private:
        
        static const blitz_u16 MaxLength = 64;
        
        blitz_u32 m_high;
        blitz_u32 m_low;
        blitz_u16 m_length;
        
        void _set_bit_safe(bool set_bit);
        
    public:
        blitz_payload();
        
        bool pack(bool set_bit);
        bool pack(unsigned char set_char, blitz_u16 precision);
        bool pack(unsigned int set_int, blitz_u16 precision);
        bool pack(unsigned long set_long, blitz_u16 precision);
        
        char *render(char *dest);
};


#endif // BLITZ_PAYLOAD_H
