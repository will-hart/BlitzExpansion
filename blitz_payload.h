#ifndef BLITZ_PAYLOAD_H
#define BLITZ_PAYLOAD_H

#include "Arduino.h"
//#include <stdio.h>
#include <string.h>

class blitz_payload
{
	private:
		unsigned long m_high;
		unsigned long m_low;
		int m_length;
		
		static const int MaxLength = 64;
		
		void _set_bit_safe(bool set_bit);
		
	public:
		blitz_payload();
		
		bool pack(bool set_bit);
		bool pack(unsigned char set_char, int precision);
		bool pack(unsigned int set_int, int precision);
		bool pack(unsigned long set_long, int precision);
		
		char *render(char *dest);
};


#endif // BLITZ_PAYLOAD_H
