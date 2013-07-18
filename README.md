# BlitzMessage

## About

> An Arduino library useful for rapidly prototyping Blitz Expansion Boards.  It provides 
> methods for packaging, sending and receiving data from the Data Logger.  BlitzMessage 
> should be used as a standard interface for all Arduino derived Expansion Boards.

## License

BlitzMessage is provided under a AGPLv3 license. 

## Usage

    #include <BlitzMessage.h>
    
    void setup() {
      Serial.begin(9600);
    }
    
    void loop() {
      // set up a blitz expansion board message with board ID 2
      BlitzMessage msg(2);
      
      // format the message (currently no variables set)
      char formatted_message[29];
      msg.render(formatted_message);
      
      // write to serial
      Serial.println(formatted_message);
      
      // rinse for 1 second, repeat
      delay(1000);
    }

## Examples

Several examples are included - either look in the `[Arduino Folder]/libraries/BlitzMessage/examples` 
folder or browse through the Sketchbook in the Arduino IDE.

## Speed tests

Running the `Speed_Tests` example I can regularly get the following on the Arduino Due:

 > Generated 10,000 messages in 1.330000013 seconds

On an Arduino Uno R3 I get:

 > Generated 10,000 messages in 7.996000289 seconds

