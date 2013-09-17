#ifndef BLITZ_EXPANSION_H
#define BLITZ_EXPANSION_H

#include "blitz_payload.h"
#include "BlitzMessage.h"

#include "Arduino.h"
#include "HardwareSerial.h"

class BlitzExpansion
{
    private:
        HardwareSerial* m_serial;
        void (*m_onSample)(void);
        bool (*m_onInstruction)(blitz_u8, blitz_u16*);
        char m_id;
        char** m_messageBuffer;
        int m_currentIdx;
        int m_sendIdx;
        int m_maxIdx;
        int m_frequency;
        int m_frequencyDelay;
        //long m_startTime;
        bool m_logging;
        
        char* m_serialBuffer;
        int m_bufferIdx;
        
        void sendId();
        void sendStatus();
        void sendLog();
        void sendShortResponse(char *code);
        
        void clearSerialBuffer();
        void handleSerial();
        
        static const unsigned short ON_BOARD_LED = 13;


    public:
        BlitzExpansion(char id, int bufferSize, int frequency);
        
        void begin(void (*)(void), HardwareSerial *serial);
        void begin(void (*)(void), bool (*)(blitz_u8, blitz_u16*), HardwareSerial *serial);
        
        void sample();
        void log(BlitzFormattedMessage message);
};
#endif*/