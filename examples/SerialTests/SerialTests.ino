#include <BlitzMessage.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  BlitzMessage msg(1);
  
  // set all the flags on
  msg.set_flag(1, true);
  msg.set_flag(2, false);
  msg.set_flag(3, true);
  msg.set_flag(4, false);
  msg.set_flag(5, true);
  
  // add an int to the payload
  int adc = 1025;
  msg.pack(adc);
  msg.pack(adc);
  
  // get the message
  char *formatted_message = msg.render();
  
  // send char by char
  for (int i = 0; i < 28; ++i) {
    Serial.print(formatted_message[i], HEX);
  }
  Serial.println(" ");
  
  delay(1000);
}
