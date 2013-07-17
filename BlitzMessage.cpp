/*
 *  BlitzMessage is an Arduino utility library released under the AGPLv3 and
 *  designed to allow the creation of Blitz Logger Expansion Boards using 
 *  Arduino derived micro-controller boards.  You can find more about the 
 *  Blitz Logger at http://www.blitzlogger.com
 * 
 *  The use of this software is at your own risk, no warranty is given or implied 
 */
 
#include "BlitzMessage.h"

//#include "Arduino.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* packing functions */
bool BlitzMessage::pack(bool data) { 
    return this->m_payload->pack(data);
}

bool BlitzMessage::pack(unsigned char data) {
	return this->m_payload->pack(data, 8);
}

bool BlitzMessage::pack(unsigned char data, int precision) {
    return this->m_payload->pack(data, precision);
}

bool BlitzMessage::pack(unsigned int data) {
	return this->m_payload->pack(data, 16);
}

bool BlitzMessage::pack(unsigned int data, int precision) {
    return this->m_payload->pack(data, precision);
}

bool BlitzMessage::pack(unsigned long data) {
	return this->m_payload->pack(data, 32);
}

bool BlitzMessage::pack(unsigned long data, int precision) {
    return this->m_payload->pack(data, precision);
}

/* flag setting functions */
bool BlitzMessage::set_flag(int flag_id, bool state) {
    if (flag_id < 0 || flag_id > FLAG_LENGTH) {
        return false;
    }
    
    this->m_flags[flag_id] = state;
    return true;
}

/* sending functions */
char *BlitzMessage::render(char* dest) { 
		
	// build the id 
	char id_str[3];
	sprintf(id_str, "%02x", this->m_id);
	
	// build the meta
	char meta_str[3];
	sprintf(meta_str, "%02x", this->m_meta);
	
	// build the timestamp
	this->m_timestamp = 1001020;
	char time_str[9];
	sprintf(time_str, "%08x", this->m_timestamp);
	
    // pad to the end with 0s
    char raw_payload[17];
	this->m_payload->render(raw_payload);
	
	// build the message by appending the parts
	strncat(dest, id_str, 2);
	strncat(dest, meta_str, 2);
	strncat(dest, time_str, 8);
	strncat(dest, raw_payload, 16);
	
	// reset the message for the next round :)
    this->reset();
    return dest;
}

/* Utility functions */
void BlitzMessage::reset() {
    free(this->m_payload);
    this->reset_flags();
	
	this->m_payload = new blitz_payload();
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
    this->m_payload = new blitz_payload();
    this->m_flags = new bool[FLAG_LENGTH];
    this->m_meta = 0;
    this->m_timestamp = 0;
    
    this->reset();
}
