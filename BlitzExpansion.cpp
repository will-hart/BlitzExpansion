#include "BlitzExpansion.h"

/**
 * Constructor for a BlitzExpansion object.  Sets the ID of 
 * the board, the message buffer size and the update frequency
 */
BlitzExpansion::BlitzExpansion(char id, int bufferSize, int frequency, int sendFrequency) {
    this->m_id = id;
    this->m_messageBuffer = new char*[bufferSize];
    this->m_sendFrequency = sendFrequency;
    this->m_sendCounter = 0;
    
    // pre-seed the buffer
    for (int i = 0; i < bufferSize; ++i) {
        this->m_messageBuffer[i] = new char[29];
    }
    
    this->m_currentIdx = 0;
    this->m_sendIdx = 0;
    this->m_maxIdx = bufferSize;
    this->m_frequency = frequency;
    this->m_frequencyDelay = (int)(1000 / frequency);
    this->m_logging = false;
    
    this->m_serialBuffer = new char[BlitzMessage::MESSAGE_LENGTH];
    this->clearSerialBuffer();
    
    this->m_bufferIdx = 0;
    
    pinMode(BlitzExpansion::ON_BOARD_LED, OUTPUT);
    digitalWrite(BlitzExpansion::ON_BOARD_LED, LOW);
}

/** 
 * Connects up sampling and logging functions and passes a reference to the serial class.  
 *
 * The "log" function MUST call BlitzExpansion::log to save messages
 */
void BlitzExpansion::connect(void (*sample)(void), void (*log)(void), HardwareSerial *serial)
{
  this->m_onSample = sample;
  this->m_onLog = log;
  this->m_onInstruction = NULL;
  this->m_serial = serial;
  
  // switch off the LED
  digitalWrite(BlitzExpansion::ON_BOARD_LED, LOW);
}

/** 
 * Connects up sampling, logging and instruction handling functions and passes 
 * a reference to the serial class.  
 *
 * The "log" function MUST call BlitzExpansion::log to save messages
 */
void BlitzExpansion::connect(void (*sample)(void), 
                            void (*log)(void), 
                            bool (*instruction)(blitz_u8, blitz_u16*), 
                            HardwareSerial *serial)
{
  this->m_onSample = sample;
  this->m_onLog = log;
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

    if (this->m_logging) {
        // log a sample with the user defined function
        this->m_onSample();
    
        // check if it is time to log a message
        if (this->m_sendCounter >= this->m_sendFrequency) {
            this->m_onLog();
            this->m_sendCounter = 0;
        } else {
            this->m_sendCounter++;
        }
    }
    
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
 * Sends a single message immediately to the data logger without buffering
 */
void BlitzExpansion::send(BlitzFormattedMessage message) {
    this->m_serial->println(message);
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
                if (this->m_logging == true) {
                    this->sendLog();
                } else {
                    this->sendShortResponse(BLITZ_ERROR_NOT_CURRENTLY_LOGGING);
                }
            } else if (msgType == BLITZ_INSTRUCTION) {
                char instruction = BlitzMessage::getInstruction(this->m_serialBuffer);
                
                if ((int)instruction == BLITZ_INSTRUCTION_ID) {
                    this->sendShortResponse(BLITZ_RESPONSE_ID);
                } else if ((int)instruction == BLITZ_INSTRUCTION_STATUS) {
                    this->sendStatus();
                } else {
                    bool handled = false;
                    if (this->m_onInstruction != NULL) {
                        
                        // pass off to user defined instruction handler
                        unsigned short payload[4] = { 0, 0, 0, 0 };
                        
                        if (this->m_bufferIdx >= BlitzMessage::MESSAGE_LENGTH) {
                            payload[0] = BlitzMessage::buildU16(this->m_serialBuffer, 12);
                            payload[1] = BlitzMessage::buildU16(this->m_serialBuffer, 16);
                            payload[2] = BlitzMessage::buildU16(this->m_serialBuffer, 20);
                            payload[3] = BlitzMessage::buildU16(this->m_serialBuffer, 24);
                        }
                        
                        handled = this->m_onInstruction(instruction, payload);
                    }
                    if (!handled) {
                        // unknown instruction error
                        this->sendShortResponse(BLITZ_ERROR_UNKNOWN_INSTRUCTION);
                    }
                }
            } else if (msgType == BLITZ_START) {
                // switch on the LED
                digitalWrite(BlitzExpansion::ON_BOARD_LED, HIGH);

                this->m_sendIdx = this->m_currentIdx;
                this->m_logging = true;
                this->m_sendCounter = 0;
                this->sendShortResponse(BLITZ_RESPONSE_ACK);
                
            } else if (msgType == BLITZ_STOP) {
                // switch off the LED
                digitalWrite(BlitzExpansion::ON_BOARD_LED, LOW);
                this->m_logging = false;

                this->sendShortResponse(BLITZ_RESPONSE_ACK);
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

    this->sendShortResponse(BLITZ_RESPONSE_ACK);
}

/** 
 * Sends the buffer status of this device - two space separated values
 * the first indicating the current buffer position and the second the
 * last sent buffer position
 */
void BlitzExpansion::sendStatus() {
    BlitzMessage *msg = new BlitzMessage(this->m_id);
    BlitzFormattedMessage output;
    
    msg->setFlag(3, true); // set as 'status response' instruction message
    msg->pack(this->m_logging);
    msg->pack(this->m_currentIdx, 10);
    msg->pack(this->m_sendIdx, 10);
    msg->setType(BLITZ_INSTRUCTION);
    msg->renderInto(output);
    
    this->m_serial->println(output);
}