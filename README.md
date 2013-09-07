# BlitzExpansion 1.1.0

## About

> An Arduino library useful for rapidly prototyping Blitz Expansion Boards.  
> It provides utilities for sampling to a given frequency, and packing messages
> in the correct data format using the BlitzMessage class.  This library can be  
> used as a standard interface for all Arduino derived Blitz Logger Expansion Boards.

## License

This Arduino library is Copyright William Hart (2013) under an LGPLv3 license.  See [www.blitzlogger.com](http://www.blitzlogger.com) for more information. 

## Usage

The following example reads a single ADC and stores this in a message format.  The `BlitzExpansion` object takes care of all formatting and buffering of messages and serial communications.

    // include the library
	#include <BlitzExpansion.h>
	
	// set up some definitions for board configuration
	#define SLAVE_ADDRESS 0x08
	#define BUFFER_SIZE 180
	#define FREQUENCY_HZ 1
	#define ADC_PIN 2
	
	// create the library instance
	BlitzExpansion expansion = BlitzExpansion(SLAVE_ADDRESS, BUFFER_SIZE, FREQUENCY_HZ);
	
	// set up a message builder
	BlitzMessage builder = BlitzMessage(SLAVE_ADDRESS);
	
	void setup() {
	    pinMode(ADC_PIN, INPUT);
	    Serial.begin(57600);
	
	    // The expansion board needs to know the function 
	    // to use for saving data and also needs a reference
        // to the serial port
	    expansion.begin(logMessage, &Serial);
	}
	
	void loop() {
	    // this samples at the given freuqency and
	    // also listens for serial messages
	    // from the data logger
	    expansion.sample();
	}
	
	// our data logging function, linked to
	// the expansion board through expansion.begin()
	void logMessage() {
	    BlitzFormattedMessage rawMessage;
	    int adc = analogRead(ADC_PIN);
	
	    // build the message
	    builder.pack(adc, 10); // pack the ADC reading to 
	                           // 10 bit precision
	    builder.renderInto(rawMessage);
	
	    // queue the message in the expansion board buffer
	    expansion.log(rawMessage);
	}
    

The following simple example shows how to use the `BlitzMessage` class for packing messages in the data logger format:

    #include <BlitzExpansion.h>
    
    void setup() {
      Serial.begin(9600);
    }
    
    void loop() {
      // set up a blitz expansion board message with board ID 2
      BlitzMessage msg(2);
      
      // add a random number at 10 bit precision
      int var = 457;
      msg.pack(457, 10);
      
      // format the message
      BlitzFormattedMessage formatted_message;
      msg.renderInto(formatted_message);
      
      // write to serial
      Serial.println(formatted_message);
      
      // twiddle thumbs
      delay(1000);
    }

## Examples

Several examples are included - either look in the `[Arduino Folder]/libraries/BlitzMessage/examples` folder or browse through the Sketchbook in the Arduino IDE.

## Speed tests

Running the `Speed_Tests` example in version `1.0.0` I get the following on the Arduino Due:

 > Generated 10,000 messages in 1.330000013 seconds

On an Arduino Uno R3 I get:

 > Generated 10,000 messages in 7.996000289 seconds


## API

### BlitzExpansion

#### Constructor (BlitzExpansion::BlitzExpansion)

    BlitzExpansion(char id, int bufferSize, int frequency);

Constructs a new instance of a BlitzExpansion board that can be used to carry out communications and buffer messages for transmission to the data logger. The constructor takes three arguments:

 - **id**: a single character (e.g. `0x08`) representing the expansion board ID
 - **bufferSize**: the number of `BlitzFormattedMessages` to store (in a circular buffer)
 - **frequency**: the (integer) frequency to sample at
       
#### BlitzExpansion::begin() 

    void begin(void (function)(void), HardwareSerial *serial);
        
Configures the `BlitzExpansion` instance for logging and communicating using the given sampling function and reference to the serial class. The two arguments are:

 - **function**: a function to use to sample and log messages.  This function should call `BlitzExpansion::log` with a formatted message.
 - **serial**: a reference to the serial adapter being used for communications - usually obtained by `&Serial`


#### BlitzExpansion::log()

    void log(BlitzFormattedMessage message);

Takes a `BlitzFormattedMessage` and stores it in the `BlitzExpansion` circular buffer.  Typically this should be called within the sampling function bassed to `BlitzExpansion::begin`.  This function takes a single argument:

 - **message**: the `BlitzFormattedMessage` (a `char[29]`) to save in the buffer

#### BlitzExpansion::sample()

    void sample();
        
This function should be called from within the main loop. It takes a single sample using the given sample function, then listens for serial messages until the required sampling frequency is obtained. If a significant amount of serial activity is undertaken (for instance transmitting a large amount of messages from the buffer) there may be some delays in sampling.  This function takes no arguments.

### BlitzFormattedMessage

    typedef char BlitzFormattedMessage[29];

`BlitzFormattedMessage` is a convenience shortcut to a character array of length 29 (`char[29]`)  This represents the length of a Blitz data logger message plus a null terminating character `\0` at the end.

### BlitzMessage

#### Constructor (BlitzMessage::BlitzMessage)

    BlitzMessage(char id);

Constructs a new `BlitzMessage`, setting the board ID to the passed `id`.  This function takes one argument:

 - **id**: the id (e.g. `0x08`) to use for messages constructed with this `BlitzMessage` instance

#### BlitzMessage::pack

    bool pack(TYPE data, OPTIONAL SHORT precision);

Takes the given variable and "packs" it into the `BlitzMessage` payload.  Supports the following types:

 - `bool`  (1 bit - no optional precision argument)
 - `char` (8 bits)
 - `unsigned char` (8 bits)
 - `int` (16 bits)
 - `unsigned int` (16 bits)
 - `long` (32 bits)
 - `unsigned long` (32 bits)

If an optional `short` precision is supplied, truncate the precision to this amount (for instance an `int` can be recorded to 10 bit precision to obtain a value between 0 and 1023).  If a precision greater than the bit length of the type is provided, (for instance a precision of 64 for an int) then the entire variable will be packed.

The maximum payload size is 8 bytes, or 64 bits. (Internally this is stored as two `long` variables).  It is up to the application to ensure less than 64 bits are packed into any one message.  Flags are not included within this limit.

This function has one mandatory argument and one optional one:

 - **data**: a supported variable to be packed into the `BlitzMessage`
 - **precision**: an optional bit precision to truncate the `data` to (not supported for `bool` type)


#### BlitzMessage::renderInto

    void renderInto(char *dest);

Takes a `char[29]` or `BlitzFormattedMessage` and populates it with the formatted message.  This function takes one argument:

 - **dest**: the destination `char[29]` or `BlitzFormattedMessage` to save the output message to

#### BlitzMessage::reset

    void reset();

Resets the current `BlitzMessage`, clearing all saved data.  This is automatically called by `BlitzMessage::renderInto`.  This function takes no arguments.

#### BlitzMessage:setFlag

    bool setFlag(char flag_id, bool state);

Sets one of five flags on the `BlitzMessage` to the given state.  This function takes two arguments:

 - **flag_id**: the number of the flag to set (from 1-5)
 - **state**: the boolean state to set the flag to - `true == 1` and `false == 0`

Returns **false** if the `flag_id` is outside the permitted bounds 

#### BlitzMessage::setType

    bool setType(char type_id);

Sets the type of message to construct.  Types can be between 0 and 7 and a number of constants are provided:

 - **0**: `BLITZ_START` Start logging
 - **1**: `BLITZ_STOP` Stop logging
 - **2**: `BLITZ_ACK` Acknowledge
 - **3**: `BLITZ_ERROR` Error
 - **4**: `BLITZ_INSTRUCTION` Instruction
 - **5**: `BLITZ_DATA` Data
 - **6**: `BLITZ_TRANSMIT` Transmit data instruction
 - **7**: `BLITZ_EXTENDED` Extended data

Typically messages transmitted from the expansion board to the data logger will be of type `5`. This function takes one argument:

 - **type_id**: the type of message being transmitted (e.g. `0x05`).  

This function is optional.  If it is not called the message will default to type `5`. 

## Change log

### Version 1.1.0

 - `+` Add support for `BlitzExpansion` class to handle all buffering, sending and queueing of messages

### Version 1.0.1

 - `+` Provide `BlitzFormattedMessage` type for rendered message outputs
 - `~` Change `BlitzMessage::render` to `BlitzMessage::renderInto`
 - `~` Some other minor tweaks

### Version 1.0.0

- `+` All features! See the examples folder for usage