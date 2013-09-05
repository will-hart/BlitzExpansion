# BlitzMessage 1.0.1

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

Several examples are included - either look in the `[Arduino Folder]/libraries/BlitzMessage/examples` 
folder or browse through the Sketchbook in the Arduino IDE.

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