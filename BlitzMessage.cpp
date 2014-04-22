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
    this->renderInto(dest, millis());
}

void BlitzMessage::renderInto(char* dest, long timestamp) { 

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

    // build the timestamp - strncpy was quite a bit slower
    // than just accessing bits one by one
    this->m_timestamp = timestamp;
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

    // have to use strcpy here as the string length is variable
    // this will be compensated for through the possibility of 
    // shorter serial transmissions
    char raw_payload[PAYLOAD_LENGTH];
    this->m_payload->render(raw_payload);
    strcpy(dest + META_LENGTH, raw_payload);
    
    // reset the message for the next round :)
    this->reset();
}

/* Utility functions */
void BlitzMessage::reset() {
    free(this->m_payload);
    this->resetFlags();
    this->m_payload = new blitz_payload();
}

void BlitzMessage::resetFlags() {
    this->m_meta &= BlitzMessage::FLAG_MASK;
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

bool BlitzMessage::setMeta(char meta) 
{
    this->m_meta = meta;
    return true;
}

bool BlitzMessage::getFlag(char *message, short flagId) {
    if (flagId < 1 || flagId > 5) {
        return false;
    } else {
        char val = message[1] >> (5 - flagId);
        val &= 0x01;
        return (bool)val;
    }    
}

unsigned char BlitzMessage::getType(char *message) {
    unsigned char first = BlitzMessage::asHex(message[2]) << 4;
    first |= BlitzMessage::asHex(message[3]);
    return first >> 5;
}

unsigned char BlitzMessage::getInstruction(char *message) {
    unsigned char first = (BlitzMessage::asHex(message[2])) << 4 ;
    return first | BlitzMessage::asHex(message[3]);
}
unsigned char BlitzMessage::asHex(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else {
        return 0;
    }
}

blitz_u16 BlitzMessage::buildU16(char *message, char index) {
    unsigned short result = 0;
    for (int i = index; i < index + 4; ++i) {
        result <<= 4;
        result |= BlitzMessage::asHex(message[i]);
    }
    
    return result;
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
