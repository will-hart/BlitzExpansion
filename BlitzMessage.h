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
#define BLITZ_FLAG_LENGTH 5
#define BLITZ_META_LENGTH 12
// allow custom payload length as long as its less than 16
#ifndef BLITZ_PAYLOAD_LENGTH
    #define BLITZ_PAYLOAD_LENGTH 16
#else
    #if BLITZ_PAYLOAD_LENGTH > 16
        #warning Maximum supported BLITZ_PAYLOAD_LENGTH is 16
        #define BLITZ_PAYLOAD_LENGTH 16
    #endif
#endif 

#define BLITZ_FULL_MESSAGE_LENGTH BLITZ_PAYLOAD_LENGTH + BLITZ_META_LENGTH

#define BLITZ_START 0
#define BLITZ_STOP 1
#define BLITZ_ACK 2
#define BLITZ_ERROR 3
#define BLITZ_INSTRUCTION 4
#define BLITZ_DATA 5
#define BLITZ_TRANSMIT 6
#define BLITZ_EXTENDED 7

// define the INT versions of the instruction IDs
#define BLITZ_INSTRUCTION_NONE 128 // 0x80
#define BLITZ_INSTRUCTION_ID 129 // 0x81
#define BLITZ_INSTRUCTION_STATUS 131 // 0x83

#define BLITZ_RESPONSE_ACK "40"

#define BLITZ_ERROR_MESSAGE_TOO_SHORT "61"
#define BLITZ_ERROR_SERIAL_BUFFER_FULL "62"
#define BLITZ_ERROR_NOT_IMPLEMENTED "63"
#define BLITZ_ERROR_UNKNOWN_MESSAGE "64"
#define BLITZ_ERROR_UNKNOWN_INSTRUCTION "65"
#define BLITZ_ERROR_NOT_CURRENTLY_LOGGING "66"

#define BLITZ_RESPONSE_ID "82"
#define BLITZ_RESPONSE_STATUS_META 0x84

class BlitzMessage
{
    private:
        char m_id;
        char m_meta;
        blitz_u32 m_timestamp;       
        blitz_payload *m_payload;
        
        /* utility functions */
        void resetFlags();
        
    public:
        BlitzMessage(char id);
    
        bool setType(char type_id);
        bool setMeta(char meta);
        
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
        bool setFlag(char flag_id, bool state);
        
        /* sending functions */
        void renderInto(char *dest);
        void renderInto(char *dest, long timestamp);
        
        /* receiving functions */
        static unsigned char getType(char *message);
        static unsigned char getInstruction(char *message);
        static bool getFlag(char *message, short flagId);
        static blitz_u16 buildU16(char *message, char index);
        static unsigned char asHex(char c);
        
        /* utility functions */
        void reset();
        
        /* static constants */
        static const char FLAG_MASK = 0b11100000;
        
    
};

#endif
