#define PAYLOAD_LENGTH 4

#include <BlitzExpansion.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  // set up a blitz expansion board message with board ID 2
  BlitzMessage msg(0x08);
  
  // format the message (currently no variables set)
  BlitzFormattedMessage formatted_message;
  msg.pack(100,8);
  msg.pack(100,8);
  msg.renderInto(formatted_message);
  
  // write to serial
  Serial.println(formatted_message);
  
  // rinse for 1 second, repeat
  delay(1000);
}
