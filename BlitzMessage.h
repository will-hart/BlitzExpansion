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


class BlitzMessage
{
    private:
        bool *m_flags;
        char m_id;
        char m_meta;
        
        blitz_u32 m_timestamp;       
        blitz_payload *m_payload;
        
        /* utility functions */
        void reset_flags();
        
    public:
        BlitzMessage(char id);
    
    /* packing functions */
    bool pack(bool data);
    bool pack(unsigned char data, int precision);
    bool pack(unsigned char data);
    bool pack(char data, int precision);
    bool pack(char data);
    bool pack(unsigned int data, int precision);
    bool pack(unsigned int data);
    bool pack(int data, int precision);
    bool pack(int data);
    bool pack(unsigned long data, int precision);
    bool pack(unsigned long data);
    bool pack(long data, int precision);
    bool pack(long data);
    
    /* flag setting functions */
    bool set_flag(int flag_id, bool state);
    
    /* sending functions */
    char *render(char *dest);
    
    /* utility functions */
    void reset();
    
};

#endif
