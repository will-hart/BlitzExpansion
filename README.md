# BlitzMessage

## About

> An Arduino library useful for rapidly prototyping Blitz Expansion Boards.  It provides 
> methods for packaging, sending and receiving data from the Data Logger.  BlitzMessage 
> should be used as a standard interface for all Arduino derived Expansion Boards.

## License

BlitzMessage is provided under a AGPLv3 license. 

## Usage

    void setup() {
        provider = BlitzMessage(1);
        Serial.begin(9600);
    }
    
    void loop() {
        int one = 1;
        
        // add an integer with 16 bit precision into the message payload (max 64 bits)
        result = provider.pack(one, 16);
        
        // set one of the five flags to true
        result = result && provider.set_flag(1, true);
        
        if (result) {
            // for initial development:
            Serial.println(provider.render());
            
            
            // for production:
            // Wire.write(provider.render());
        } else {
            Serial.write("Error packing message");
        }
    }
