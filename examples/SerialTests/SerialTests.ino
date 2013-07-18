#include <BlitzMessage.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  BlitzMessage msg(2);
  msg.pack(729, 16);
  
  // get the message
  char formatted_message[29];
  msg.render(formatted_message);
  Serial.println(formatted_message);
  
  delay(1000);
}