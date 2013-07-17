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

/* packing functions */
bool BlitzMessage::pack(bool data) { 
    if (this->m_payloadIndex + 1 > PAYLOAD_LENGTH) {
        return false;   
    }
    
    this->m_rawPayload <<= 1;
    
    if (data == true) {
        this->m_rawPayload |= 1;
    }
    
    this->m_payloadIndex++;
    
    return true;
}

bool BlitzMessage::pack(char data) { 
    if (this->m_payloadIndex + BLITZ_CHAR_LENGTH > PAYLOAD_LENGTH) {
        return false;
    }
    this->m_rawPayload <<= BLITZ_CHAR_LENGTH;
    this->m_rawPayload |= data;
    this->m_rawPayload += BLITZ_CHAR_LENGTH;
    
    return true;
}

bool BlitzMessage::pack(char data, int precision) {
    if (precision > BLITZ_CHAR_LENGTH ||
            this->m_payloadIndex + precision < PAYLOAD_LENGTH) {
        return false;
    }

    // build a mask for the char to add
    // take 11111111 shift by precision (e.g. 3 gives 11111000)
    // then flip all the bits
    unsigned char mask = ~(0xFF << precision);
    
    // apply the mask
    unsigned char masked_data = mask & data;
    
    // shift the raw payload up the required bits and apply
    this->m_rawPayload <<= (BLITZ_CHAR_LENGTH - precision);
    this->m_rawPayload = this->m_rawPayload | masked_data;
    
    // increment the index
    this->m_payloadIndex += precision;
    
    return true;
}

bool BlitzMessage::pack(int data) { 
    if (m_payloadIndex + BLITZ_INT_LENGTH > PAYLOAD_LENGTH) {
        return false;
    }
    this->m_rawPayload <<= BLITZ_INT_LENGTH;
    this->m_rawPayload |= data;
    this->m_payloadIndex += BLITZ_INT_LENGTH;
    
    return true;
}

bool BlitzMessage::pack(int data, int precision) { return false; }

bool BlitzMessage::pack(long data) { 
    if (m_payloadIndex + BLITZ_LONG_LENGTH > PAYLOAD_LENGTH) {
        return false;
    }
    this->m_rawPayload <<= BLITZ_LONG_LENGTH;
    this->m_rawPayload |= data;
    this->m_payloadIndex += BLITZ_LONG_LENGTH;
    
    return true;
}

bool BlitzMessage::pack(long data, int precision) { return false; }
bool BlitzMessage::pack(char* data, int precision) { return false; }

/* flag setting functions */
bool BlitzMessage::set_flag(int flag_id, bool state) {
    if (flag_id < 0 || flag_id > FLAG_LENGTH) {
        return false;
    }
    
    this->m_flags[flag_id] = state;
    return true;
}

/* sending functions */
char *BlitzMessage::render() { 
    // pad to the end with 0s
    this->m_rawPayload <<= PAYLOAD_BITS - this->m_payloadIndex;
    
    // return as hex string
    char *output;
    sprintf(output, "%x", m_rawPayload);

    this->reset();
    
    return output;
}

/* Utility functions */
void BlitzMessage::reset() {
    this->reset_payload();
    this->reset_flags();
}

void BlitzMessage::reset_payload() 
{
    for (int i = 0; i < PAYLOAD_LENGTH; ++i) 
    {
        this->m_payload[i] = 0;
    }
    
    this->m_rawPayload = 0;
    this->m_payloadIndex = 0;
}

void BlitzMessage::reset_flags() 
{
    // initialise flags
    for (int i = 0; i < FLAG_LENGTH; ++i) {
        this->m_flags[i] = false;
    }
}

/* Constructor */
BlitzMessage::BlitzMessage(char id)
{
    this->m_id = id;
    this->m_rawPayload = 0;
    this->m_payloadIndex = 0;
    this->m_payload = new char[PAYLOAD_LENGTH];
    this->m_flags = new bool[FLAG_LENGTH];
    this->m_meta = 0;
    this->m_timestamp = 0;
    this->m_rawPayload = 0;
    
    this->reset_flags();
    this->reset_payload();
}
