#include "blitz_payload.h"

blitz_payload::blitz_payload()
{
	this->m_high = 0;
	this->m_low = 0;
	this->m_length = 0;
}

bool blitz_payload::pack(bool set_bit) {
	if (this->m_length >= 63) {
		return false;
	}
	
	this->_set_bit_safe(set_bit);
	return true;
}

void blitz_payload::_set_bit_safe(bool set_bit) {
	if (this->m_length > 31) {
		// get the top most bit in low and shift it into high
		char mask = this->m_low >> 31;
		this->m_high <<= 1;
		this->m_high |= mask;
	}
	
	// add the new bit to the end of low
	this->m_low <<= 1;
	
	if (set_bit) {
		this->m_low |= 0x01;
	}
	
	this->m_length++;
}

bool blitz_payload::pack(unsigned char set_char, int precision) {    
	if (precision + this->m_length >= 63 ||
		precision > 8) {
        return false;
    }

    // build a mask for the char to add
    unsigned char mask = ~(0xFF << precision);
    
    // apply the mask
    unsigned char masked_data = mask & set_char;

	// iterate through the data and setbit
	for (int i = precision; i > 0; --i) {
		bool set_bit = (masked_data & 0x01) == 0x01;
		this->_set_bit_safe(set_bit);
		masked_data >>= 1;
	}
	
    return true;
}

bool blitz_payload::pack(unsigned int set_int, int precision) {
	if (precision + this->m_length >= 63 ||
		precision > 16) {
        return false;
    }

    // build a mask for the char to add
    unsigned int mask = ~(0xFFFF << precision);
    
    // apply the mask
    unsigned int masked_data = mask & set_int;

	// iterate through the data and setbit
	for (int i = precision; i > 0; --i) {
		bool set_bit = (masked_data & 0x01) == 0x01;
		this->_set_bit_safe(set_bit);
		masked_data >>= 1;
	}
	
    return true;
}

bool blitz_payload::pack(unsigned long set_long, int precision) {
	if (precision + this->m_length >= 63 ||
		precision > 32) {
        return false;
    }

    // build a mask for the char to add
    unsigned long mask = ~(0xFFFFFFFF << precision);
    
    // apply the mask
    unsigned long masked_data = mask & set_long;

	// iterate through the data and setbit
	for (int i = precision; i > 0; --i) {
		bool set_bit = (masked_data & 0x01) == 0x01;
		this->_set_bit_safe(set_bit);
		masked_data >>= 1;
	}
	
    return true;
}

char *blitz_payload::to_char() {
	char first[9];
	char second[9];
	char final[17];
	
	// pad out the longs
	while (this->m_length < 63) {
		this->_set_bit_safe(false);
		++this->m_length;
	}
		
	sprintf(first, "%08lx", this->m_high);
	sprintf(second, "%08lx", this->m_low);
	
	strcpy(first, final);
	strcpy(second, final + 8);
	
	return final;
}
