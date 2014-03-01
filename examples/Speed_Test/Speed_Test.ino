#include <BlitzExpansion.h>

BlitzMessage message = BlitzMessage(0x08);

void setup() {
  Serial.begin(57600);  
}

void loop() {

  long then = millis();
  
  for (int i = 0; i < 10000; i++) 
  {
    // check for our alerts and set a flag
    message.setFlag(1, true);
    message.setFlag(2, false);
    message.setFlag(3, true);
    message.setFlag(4, false);
    message.setFlag(5, true);
  
    // pack in the ADCs to 10 bit precision and one bit spacing between each
    unsigned int data16 = 0xFFFF;
    message.pack(data16, 16);
    message.pack(data16, 16);
    message.pack(data16, 16);
    message.pack(data16, 16);
    
    // render and discard the message
    BlitzFormattedMessage formatted_message;
    message.renderInto(formatted_message);
  }
  
  long now = millis();
  
  Serial.print("Generated 10,000 messages in ");
  Serial.print((float)(now - then) / 1000.0f, DEC);
  Serial.println(" seconds");
}