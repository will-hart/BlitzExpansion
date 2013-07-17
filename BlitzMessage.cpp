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
bool BlitzMessage::pack(bool data) { return false; }
bool BlitzMessage::pack(char data) { return false; }
bool BlitzMessage::pack(char data, int precision) { return false; }
bool BlitzMessage::pack(int data) { return false; }
bool BlitzMessage::pack(int data, int precision) { return false; }
bool BlitzMessage::pack(long data) { return false; }
bool BlitzMessage::pack(long data, int precision) { return false; }
bool BlitzMessage::pack(char* data, int precision) { return false; }

/* flag setting functions */
void BlitzMessage::set_flag(int flag_id, bool state) { return; }

/* sending functions */
char *BlitzMessage::render() { return m_payload; }

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
    m_payload = new char[PAYLOAD_LENGTH];
    m_flags = new bool[FLAG_LENGTH];
    m_meta = 0;
    m_timestamp = 0;
    m_rawPayload = 0;
    
    this->zero_flags();
    this->zero_payload();
}

