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