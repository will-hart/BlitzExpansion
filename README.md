# BlitzExpansion 1.1.0

## About

> An Arduino library useful for rapidly prototyping Blitz Expansion Boards.  
> It provides utilities for sampling to a given frequency, and packing messages
> in the correct data format using the BlitzMessage class.  This library can be  
> used as a standard interface for all Arduino derived Blitz Logger Expansion Boards.

## License

This Arduino library is Copyright William Hart (2013) under an LGPLv3 license. 

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

Running the `Speed_Tests` example I can regularly get the following on the Arduino Due:

 > Generated 10,000 messages in 1.330000013 seconds

On an Arduino Uno R3 I get:

 > Generated 10,000 messages in 7.996000289 seconds

## Change log

### Version 1.0.1

 - `+` Provide `BlitzFormattedMessage` type for rendered message outputs
 - `~` Change `BlitzMessage::render` to `BlitzMessage::renderInto`
 - `~` Some other minor tweaks

### Version 1.0

- `+` All features! See the examples folder for usage