#include <BlitzMessage.h>

BlitzMessage message = BlitzMessage(2);

void setup() {
  Serial.begin(9600);  
  
  message.set_type(BLITZ_DATA);
  
  // set up some input pins
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
}

void loop() {
  
  // read ADC pins
  int pin2 = analogRead(2);
  int pin3 = analogRead(3);
  int pin4 = analogRead(4);
  int pin5 = analogRead(5);
  
  // check for our alerts and set a flag
  message.set_flag(1, pin2 > 500);

  // pack in the ADCs to 10 bit precision and one bit spacing between each
  message.pack(pin2, 10);
  message.pack(false);
  message.pack(pin3, 10);
  message.pack(false);
  message.pack(pin4, 10);
  message.pack(false);
  message.pack(pin5, 10);
  
  // write out the message
  char formatted_message[29];
  message.render(formatted_message);
  Serial.println(formatted_message);
  
  // rinse for 1 second, repeat
  delay(1000);
}