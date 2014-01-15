#include "blitz_payload.h"

blitz_payload::blitz_payload()
{
    this->m_high = 0;
    this->m_low = 0;
    this->m_length = 0;
}

bool blitz_payload::_set_bit_safe(bool set_bit) {
    
    if (this->m_length > 63) {
        return false;
    } else {
    
        // a low bit only requires us to increment the length counter
        if (!set_bit) {
            ++this->m_length;
            return true;
        }
    
        // handle adding to high or low bit
        if (this->m_length > 31) {
            this->m_low |= (((blitz_u32)1) << (63 - this->m_length));
        } else {
            this->m_high |= (((blitz_u32)1) << (31 - this->m_length));
        }
    }
    
    this->m_length++;
    return true;
}

bool blitz_payload::pack(bool set_bit) {
    
    this->_set_bit_safe(set_bit);
    return true;
}

bool blitz_payload::pack(unsigned char set_char, blitz_u16 precision) {
    if (precision < 0) return false;
    bool result = true;

    // build a mask for the char to add
    unsigned char mask = ~(0xFF << precision);
    unsigned char masked_data = mask & set_char;

    // iterate through the data and setbit
    for (int i = precision; i > 0; --i) {
        bool set_bit = (masked_data & 0x01) == 0x01;
        result = result && this->_set_bit_safe(set_bit);
        masked_data >>= 1;
    }
    
    return result;
}

bool blitz_payload::pack(unsigned int set_int, blitz_u16 precision) {
    if (precision < 0) return false;    
    bool result = true;

    // build a mask for the char to add
    unsigned int mask = ~(0xFFFF << precision);
    unsigned int masked_data = mask & set_int;

    // iterate through the data and setbit
    for (int i = precision; i > 0; --i) {
        bool set_bit = (masked_data & 0x01) == 0x01;
        result = result && this->_set_bit_safe(set_bit);
        masked_data >>= 1;
    }
    
    return result;
}

bool blitz_payload::pack(unsigned long set_long, blitz_u16 precision) {
    if (precision < 0) return false;    
    bool result = true;

    // build a mask for the char to add
    unsigned long mask = ~(0xFFFFFFFF << precision);
    unsigned long masked_data = mask & set_long;

    // iterate through the data and setbit
    for (int i = precision; i > 0; --i) {
        bool set_bit = (masked_data & 0x01) == 0x01;
        result = result && this->_set_bit_safe(set_bit);
        masked_data >>= 1;
    }
    
    return result;
}

char *blitz_payload::render(char *dest) {
    
    // pad out the longs
    while (this->m_length < 63) {
        this->_set_bit_safe(false);
    }
    
    // break the longs into ints to allow sprintf conversion to hex
    char high_a[5];
    char high_b[5];
    char low_a[5];
    char low_b[5];

    sprintf(high_a, "%04x", (unsigned int)(this->m_high >> 16));
    sprintf(high_b, "%04x", (unsigned int)(this->m_high & 0x0000FFFF));
    sprintf(low_a, "%04x", (unsigned int)(this->m_low >> 16));
    sprintf(low_b, "%04x", (unsigned int)(this->m_low & 0x0000FFFF));
    
    // slightly hackishly copy all the variables over directly 
    // (instead of using memset or similar)
    dest[ 0] = high_a[0];
    dest[ 1] = high_a[1];
    dest[ 2] = high_a[2];
    dest[ 3] = high_a[3];
    dest[ 4] = high_b[0];
    dest[ 5] = high_b[1];
    dest[ 6] = high_b[2];
    dest[ 7] = high_b[3];
    dest[ 8] = low_a [0];
    dest[ 9] = low_a [1];
    dest[10] = low_a [2];
    dest[11] = low_a [3];
    dest[12] = low_b [0];
    dest[13] = low_b [1];
    dest[14] = low_b [2];
    dest[15] = low_b [3];
    
    return dest;
}
