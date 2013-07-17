/*
 *  BlitzMessage is an Arduino utility library released under the AGPLv3 and
 *  designed to allow the creation of Blitz Logger Expansion Boards using 
 *  Arduino derived micro-controller boards.  You can find more about the 
 *  Blitz Logger at http://www.blitzlogger.com
 * 
 *  The use of this software is at your own risk, no warranty is given or implied 
 */

#ifndef _BLITZ_MESSAGE_DEFINES_

    #define _BLITZ_MESSAGE_DEFINES_
    #define FLAG_LENGTH 5
    #define PAYLOAD_LENGTH 16
    #define PAYLOAD_BITS 64
    #define PACKED_MESSAGE_CHAR_LENGTH 28
    
    // TODO define these per board (arduinos vs Due)
    #define BLITZ_CHAR_LENGTH 8
    #define BLITZ_INT_LENGTH 16
    #define BLITZ_LONG_LENGTH 32
    
class BlitzMessage
{
    private:
        bool *m_flags;
        char m_id;
        char m_meta;
        long m_timestamp;        
        unsigned long long m_rawPayload;
        char *m_payload;
        int m_payloadIndex;
        
        /* utility functions */
        void reset_payload();
        void reset_flags();
        
    public:
        BlitzMessage(char id);
    
    /* packing functions */
    bool pack(bool data);
    bool pack(char data);
    bool pack(char data, int precision);
    bool pack(int data);
    bool pack(int data, int precision);
    bool pack(long data);
    bool pack(long data, int precision);
    bool pack(char* data, int precision);
    
    /* flag setting functions */
    bool set_flag(int flag_id, bool state);
    
    /* sending functions */
    char *render();
    
    /* utility functions */
    void reset();
    
};

#endif
