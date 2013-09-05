/*
 *  BlitzMessage is an Arduino utility library released under the AGPLv3 and
 *  designed to allow the creation of Blitz Logger Expansion Boards using 
 *  Arduino derived micro-controller boards.  You can find more about the 
 *  Blitz Logger at http://www.blitzlogger.com
 * 
 *  The use of this software is at your own risk, no warranty is given or implied 
 */

#ifndef _BLITZ_MESSAGE_DEFINES_

#include "blitz_payload.h"

#define _BLITZ_MESSAGE_DEFINES_
#define FLAG_LENGTH 5
#define PAYLOAD_LENGTH 16
#define PAYLOAD_BITS 64
#define PACKED_MESSAGE_CHAR_LENGTH 28

// TODO define these per board (arduinos vs Due)?
#define BLITZ_CHAR_LENGTH 8
#define BLITZ_INT_LENGTH 16
#define BLITZ_LONG_LENGTH 32

#define BLITZ_START 0
#define BLITZ_STOP 1
#define BLITZ_ACK 2
#define BLITZ_ERROR 3
#define BLITZ_INSTRUCTION 4
#define BLITZ_DATA 5
#define BLITZ_TRANSMIT 6
#define BLITZ_EXTENDED 7

class BlitzMessage
{
    private:
        char m_id;
        char m_meta;
        blitz_u32 m_timestamp;       
        blitz_payload *m_payload;
        
        /* utility functions */
        void reset_flags();
        
    public:
        BlitzMessage(char id);
    
    bool set_type(char type_id);
    
    /* packing functions */
    bool pack(bool data);
    bool pack(unsigned char data, short precision);
    bool pack(unsigned char data);
    bool pack(char data, short precision);
    bool pack(char data);
    bool pack(unsigned int data, short precision);
    bool pack(unsigned int data);
    bool pack(int data, short precision);
    bool pack(int data);
    bool pack(unsigned long data, short precision);
    bool pack(unsigned long data);
    bool pack(long data, short precision);
    bool pack(long data);
    
    
    /* flag setting functions */
    bool set_flag(char flag_id, bool state);
    
    /* sending functions */
    char *renderInto(char *dest);
    
    /* utility functions */
    void reset();
    
};

#endif
