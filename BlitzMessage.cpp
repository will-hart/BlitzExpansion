/*
 *  BlitzMessage is an Arduino utility library released under the AGPLv3 and
 *  designed to allow the creation of Blitz Logger Expansion Boards using 
 *  Arduino derived micro-controller boards.  You can find more about the 
 *  Blitz Logger at http://www.blitzlogger.com
 * 
 *  The use of this software is at your own risk, no warranty is given or implied 
 */

#ifndef _BlitzMessage
 
#include "blitz_message.h"
#include "Arduino.h"

/* packing functions */
bool BlitzMessage::pack(bool data) { 
    if (m_payloadIndex + 1 > PAYLOAD_LENGTH) {
        return false;   
    }
    
    m_rawPayload <<= 1;
    
    if (data == true) {
        m_rawPayload |= 1;
    }
    
    ++m_payloadIndex;
    
    return true;
}

bool BlitzMessage::pack(char data) { 
    if (m_payloadIndex + BLITZ_CHAR_LENGTH > PAYLOAD_LENGTH) {
        return false;
    }
    m_rawPayload <<= BLITZ_CHAR_LENGTH;
    m_rawPayload |= data;
    m_rawPayload += BLITZ_CHAR_LENGTH;
    
    return true;
}

bool BlitzMessage::pack(char data, int precision) {
    if (precision > BLITZ_CHAR_LENGTH ||
            m_payloadIndex + precision < PAYLOAD_LENGTH) {
        return false;
    }

    // build a mask for the char to add
    // take 11111111 shift by precision (e.g. 3 gives 11111000)
    // then flip all the bits
    unsigned char mask = ~(0xFF << precision);
    
    // apply the mask
    unsigned char masked_data = mask & data;
    
    // shift the raw payload up the required bits and apply
    m_rawPayload = m_rawPayload << (BLITZ_CHAR_LENGTH - precision);
    m_rawPayload = m_rawPayload | masked_data;
    
    // increment the index
    m_payloadIndex += precision;
    
    return true;
}

bool BlitzMessage::pack(int data) { 
    if (m_payloadIndex + BLITZ_INT_LENGTH > PAYLOAD_LENGTH) {
        return false;
    }
    m_rawPayload <<= BLITZ_INT_LENGTH;
    m_rawPayload |= data;
    m_payloadIndex += BLITZ_INT_LENGTH;
    
    return true;
}

bool BlitzMessage::pack(int data, int precision) {
    return false;
}

bool BlitzMessage::pack(long data) { 
    if (m_payloadIndex + BLITZ_LONG_LENGTH > PAYLOAD_LENGTH) {
        return false;
    }
    m_rawPayload <<= BLITZ_LONG_LENGTH;
    m_rawPayload |= data;
    m_payloadIndex += BLITZ_LONG_LENGTH;
    
    return true;
}

bool BlitzMessage::pack(long data, int precision) { return false; }
bool BlitzMessage::pack(char* data, int precision) { return false; }

/* flag setting functions */
void BlitzMessage::set_flag(int flag_id, bool state) {
    if (flag_id < 0 || flag_id > MAX_FLAGS) {
        return false;
    }
    
    m_flags[flag_id] = state;
    return true;
}

/* sending functions */
char *BlitzMessage::render() { 
    // pad to the end with 0s
    m_rawPayload <<= PAYLOAD_BITS - m_payloadIndex;
    
    // return as hex string
    char *output;
    sprintf(output, "%016x", m_rawPayload);
    
    return output;
}

/* Utility functions */
void BlitzMessage::zero_payload() 
{
    for (int i = 0; i < PAYLOAD_LENGTH; ++i) 
    {
        m_payload[i] = 0;
    }
}

void BlitzMessage::zero_flags() 
{
    // initialise flags
    for (int i = 0; i < FLAG_LENGTH; ++i) {
        m_flags[i] = false;
    }
}

/* Constructor */
BlitzMessage::BlitzMessage(char id)
{
    m_id = id;
    m_rawPayload = 0;
    m_payloadIndex = 0;
    m_payload = new char[PAYLOAD_LENGTH];
    m_flags = new bool[FLAG_LENGTH];
    m_meta = 0;
    m_timestamp = 0;
    m_rawPayload = 0;
    
    this->zero_flags();
    this->zero_payload();
}
