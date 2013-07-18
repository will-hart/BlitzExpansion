#include <BlitzMessage.h>

BlitzMessage message = BlitzMessage(2);

void setup() {
  Serial.begin(9600);  
}

void loop() {

  long then = millis();
  
  for (int i = 0; i < 10000; i++) 
  {
    // check for our alerts and set a flag
    message.set_flag(1, true);
    message.set_flag(2, false);
    message.set_flag(3, true);
    message.set_flag(4, false);
    message.set_flag(5, true);
  
    // pack in the ADCs to 10 bit precision and one bit spacing between each
    unsigned int data16 = 0xFFFF;
    message.pack(data16, 16);
    message.pack(data16, 16);
    message.pack(data16, 16);
    message.pack(data16, 16);
    
    // render and discard the message
    char formatted_message[29];
    message.render(formatted_message);
  }
  
  long now = millis();
  
  Serial.print("Generated 10,000 messages in ");
  Serial.print((float)(now - then) / 1000.0f, DEC);
  Serial.println(" seconds");
}