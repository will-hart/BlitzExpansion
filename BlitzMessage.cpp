/*
 *  BlitzMessage is an Arduino utility library released under the AGPLv3 and
 *  designed to allow the creation of Blitz Logger Expansion Boards using 
 *  Arduino derived micro-controller boards.  You can find more about the 
 *  Blitz Logger at http://www.blitzlogger.com
 * 
 *  The use of this software is at your own risk, no warranty is given or implied 
 */
 
#include "BlitzMessage.h"

#include "Arduino.h"
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* packing functions */
bool BlitzMessage::pack(bool data) { 
    return this->m_payload->pack(data);
}

bool BlitzMessage::pack(char data) {
    return this->m_payload->pack((unsigned)data, 8);
}

bool BlitzMessage::pack(char data, short precision) {
    if (precision > 8) {
        precision = 8;
    }
    return this->m_payload->pack((unsigned)data, precision);
}

bool BlitzMessage::pack(unsigned char data) {
    return this->m_payload->pack(data, 8);
}

bool BlitzMessage::pack(unsigned char data, short precision) {
    if (precision > 8) {
        precision = 8;
    }
    return this->m_payload->pack(data, precision);
}

bool BlitzMessage::pack(int data) {
    return this->m_payload->pack((unsigned)data, 16);
}

bool BlitzMessage::pack(int data, short precision) {
    if (precision > 16) {
        precision = 16;
    }
    return this->m_payload->pack((unsigned)data, precision);
}

bool BlitzMessage::pack(unsigned int data) {
    return this->m_payload->pack(data, 16);
}

bool BlitzMessage::pack(unsigned int data, short precision) {
    if (precision > 16) {
        precision = 16;
    }
    return this->m_payload->pack(data, precision);
}

bool BlitzMessage::pack(unsigned long data) {
    return this->m_payload->pack(data, 32);
}

bool BlitzMessage::pack(unsigned long data, short precision) {
    if (precision > 32) {
        precision = 32;
    }
    return this->m_payload->pack(data, precision);
}

bool BlitzMessage::pack(long data) {
    return this->m_payload->pack((unsigned)data, 32);
}

bool BlitzMessage::pack(long data, short precision) {
    if (precision > 32) {
        precision = 32;
    }
    return this->m_payload->pack((unsigned)data, precision);
}

/* flag setting functions */
bool BlitzMessage::setFlag(char flag_id, bool state) {
    if (flag_id < 1 || flag_id > FLAG_LENGTH) {
        return false;
    }
    
    unsigned char flag_mask = 1 << (FLAG_LENGTH - flag_id);
    
    if (state)
    {
        this->m_meta |= flag_mask; 
    }
    else
    {
        this->m_meta &= ~flag_mask;
    }

    return true;
}

/* sending functions */
void BlitzMessage::renderInto(char* dest) { 
	
    // build the id 
    char id_str[3];
    sprintf(id_str, "%02x", this->m_id);
	dest[0] = id_str[0];
	dest[1] = id_str[1];
    
    // build the meta
    char meta_str[3];
    sprintf(meta_str, "%02x", this->m_meta);
	dest[2] = meta_str[0];
	dest[3] = meta_str[1];
    
    // build the timestamp
    this->m_timestamp = millis();
    char time_str[9];
    sprintf(time_str, "%08lx", this->m_timestamp);
	dest[4] = time_str[0];
	dest[5] = time_str[1];
	dest[6] = time_str[2];
	dest[7] = time_str[3];
	dest[8] = time_str[4];
	dest[9] = time_str[5];
	dest[10] = time_str[6];
	dest[11] = time_str[7];
    
    // pad to the end with 0s
    char raw_payload[16] = "000000000000000";	
    this->m_payload->render(raw_payload);
	dest[12] = raw_payload[0];
	dest[13] = raw_payload[1];
	dest[14] = raw_payload[2];
	dest[15] = raw_payload[3];
	dest[16] = raw_payload[4];
	dest[17] = raw_payload[5];
	dest[18] = raw_payload[6];
	dest[19] = raw_payload[7];
	dest[20] = raw_payload[8];
	dest[21] = raw_payload[9];
	dest[22] = raw_payload[10];
	dest[23] = raw_payload[11];
	dest[24] = raw_payload[12];
	dest[25] = raw_payload[13];
	dest[26] = raw_payload[14];
	dest[27] = raw_payload[15];
    dest[28] = '\0';
    
    // reset the message for the next round :)
    this->reset();
}

/* Utility functions */
void BlitzMessage::reset() {
    free(this->m_payload);
    this->resetFlags();
    this->m_payload = new blitz_payload();
}

void BlitzMessage::resetFlags() 
{
    // initialise flags to "false"
    char flag_mask = 0b11100000;
    this->m_meta &= flag_mask;
}

bool BlitzMessage::setType(char type_id) 
{
    if (type_id > 7 || type_id < 0) {
        // ID must be a char between 0 - 7.  do not set
        return false;
    }
    
    unsigned char type_mask = type_id << 5;
    unsigned char var_mask = 0b00011111;
    
    this->m_meta &= var_mask;
    this->m_meta |= type_mask;
}

char BlitzMessage::getType(char *message) {
    return message[1] >> 5;
}

/* Constructor */
BlitzMessage::BlitzMessage(char id)
{
    this->m_id = id;
    this->m_payload = new blitz_payload();
    this->m_meta = 0xA0;
    this->m_timestamp = 0;
    
    this->reset();
}
