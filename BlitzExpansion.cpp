#include "BlitzExpansion.h"

/**
 * Constructor for a BlitzExpansion object.  Sets the ID of 
 * the board, the message buffer size and the update frequency
 */
BlitzExpansion::BlitzExpansion(char id, int bufferSize, int frequency) {
    m_id = id;
    this->m_messageBuffer = new char*[bufferSize];
    
    // pre-seed the buffer
    for (int i = 0; i < bufferSize; ++i) {
        this->m_messageBuffer[i] = new char[29];
    }
    
    this->m_currentIdx = 0;
    this->m_sendIdx = 0;
    this->m_maxIdx = bufferSize;
    this->m_frequency = frequency;
    this->m_frequencyDelay = (int)(1000 / frequency);
    
    this->m_serialBuffer = new char[BlitzMessage::MESSAGE_LENGTH];
    this->clearSerialBuffer();
    
    this->m_bufferIdx = 0;
}

/** 
 * The function (specified by the user) that determines
 * what happens when a sample is taken.  This should use
 * the BlitzExpansion->builder to create a BlitzFormattedMessage
 * and then save this message to the buffer using 
 * BlitzExpansion::log()
 */
void BlitzExpansion::begin(void (*function)(void), HardwareSerial *serial)
{
  this->m_onSample = function;
  this->m_onInstruction = NULL;
  this->m_serial = serial;
}

/** 
 * The function (specified by the user) that determines
 * what happens when a sample is taken.  This should use
 * the BlitzExpansion->builder to create a BlitzFormattedMessage
 * and then save this message to the buffer using 
 * BlitzExpansion::log()
 */
void BlitzExpansion::begin(void (*sample)(void), bool (*instruction)(int, char*), HardwareSerial *serial)
{
  this->m_onSample = sample;
  this->m_onInstruction = instruction;
  this->m_serial = serial;
}

/**
 * A blocking function which takes a sample (using the function 
 * defined by BlitzExpansion::onSample) and then delays for the
 * period of time required to maintain the sample frequency given
 * in the constructor.  During this time it will also handle any
 * serial messages that are received.  Serial responses may delay
 * the sampling of new data.
 *
 * This function should be called from within the main loop()
 */
void BlitzExpansion::sample() {
    // log a sample with the user defined function
    this->m_onSample();
        
    // now delay (listening to serial) until the sampling
    // frequency delay rate is met
    long target = millis() + this->m_frequencyDelay;
    
    // check serial at least once
    this->handleSerial();
    
    // loop until our target time is met
    while (millis() < target) {
        delay(1);
        this->handleSerial();
    }
}

/** 
 * Takes a serial message passed by the user and saves it
 * to the serial buffer, incrementing indices as required
 */
void BlitzExpansion::log(BlitzFormattedMessage message) {

    // allocate memory for the new message
    char *savedMessage = new BlitzFormattedMessage;
    strcpy(savedMessage, message);
    
    // delete the previous item
    //delete[] this->m_messageBuffer[this->m_currentIdx];
    free(this->m_messageBuffer[this->m_currentIdx]);
    
    // save the new item to the buffer
    this->m_messageBuffer[this->m_currentIdx] = savedMessage;
    ++this->m_currentIdx;
    
    // check for wrapping
    if (this->m_currentIdx >= this->m_maxIdx) {
        this->m_currentIdx = 0;
    }
    
    // check for overtaking the last logged indicator
    if (this->m_currentIdx == this->m_sendIdx) {
        this->m_sendIdx = this->m_currentIdx + 1;
        
        // check if we need to wrap the send Idx back to 0
        if (this->m_sendIdx >= this->m_maxIdx) {
            this->m_sendIdx = 0;
        }
    }
}


/** 
 * Clears the serial buffer
 */
void BlitzExpansion::clearSerialBuffer() {
    this->m_bufferIdx = 0;
    for (int i = 0; i < BlitzMessage::MESSAGE_LENGTH; ++i) {
        this->m_serialBuffer[i] = 0;
    }
}


/**
 * Handles serial communications with the data logger
 */
void BlitzExpansion::handleSerial() {
    while (this->m_serial->available() > 0) {        
        this->m_serialBuffer[this->m_bufferIdx] = this->m_serial->read();
        ++this->m_bufferIdx;
        
        // we have received a complete message
        if (this->m_serialBuffer[this->m_bufferIdx - 1] == '\n') {
            if (this->m_bufferIdx < 4) {
                // message too short error
                this->clearSerialBuffer();
                this->sendShortResponse(BLITZ_ERROR_MESSAGE_TOO_SHORT);
            }
        
            short msgType = BlitzMessage::getType(this->m_serialBuffer);
            if (msgType == BLITZ_TRANSMIT) {
                this->sendLog();
            } else if (msgType == BLITZ_INSTRUCTION) {
                char instruction = BlitzMessage::getInstruction(this->m_serialBuffer);
                
                if (instruction == BLITZ_INSTRUCTION_ID) {
                    this->sendShortResponse(BLITZ_RESPONSE_ID);
                } else if (instruction == BLITZ_INSTRUCTION_STATUS) {
                    this->sendStatus();
                } else {
                    bool handled = false;
                    if (this->m_onInstruction != NULL) {
                        handled = this->m_onInstruction(instruction, this->m_serialBuffer);
                    }
                    if (!handled) {
                        // unknown instruction error
                        this->sendShortResponse(BLITZ_ERROR_UNKNOWN_INSTRUCTION);
                    }
                }
            } else {
                // unknown message error
                this->sendShortResponse(BLITZ_ERROR_UNKNOWN_MESSAGE);
            }
            
            this->clearSerialBuffer();
        } else if (this->m_bufferIdx >= BlitzMessage::MESSAGE_LENGTH) {
            // message too long error
            this->clearSerialBuffer();
            this->sendShortResponse(BLITZ_ERROR_SERIAL_BUFFER_FULL);
        }  
    }
}

/**
 * Send a response over serial with the board ID then the two digit code.
 * Formats should be a two digit hex response code along the lines of "91"
 */
void BlitzExpansion::sendShortResponse(char *code) {
    char format[7];
    char buffer[5];
    strcpy(format, "%02x");
    strcat(format, code);
    sprintf(buffer, format, this->m_id);
    this->m_serial->println(buffer);
}

/**
 * Sends the backlog of messages over serial
 */
void BlitzExpansion::sendLog() {
    while (this->m_sendIdx != this->m_currentIdx) {
        this->m_serial->println(this->m_messageBuffer[this->m_sendIdx]);
        ++this->m_sendIdx;
      
        // wrap sendIdx around if required
        if (this->m_sendIdx >= this->m_maxIdx) {
            this->m_sendIdx = 0;
        }
    }
}

/** 
 * Sends the buffer status of this device - two space separated values
 * the first indicating the current buffer position and the second the
 * last sent buffer position
 */
void BlitzExpansion::sendStatus() {
    this->sendShortResponse(BLITZ_ERROR_NOT_IMPLEMENTED);
}