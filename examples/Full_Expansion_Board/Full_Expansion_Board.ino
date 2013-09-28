/* This version drops I2C support and uses 
Serial at 57600 instead */

#include <BlitzExpansion.h>

#define SLAVE_ADDRESS 0x08
#define BUFFER_SIZE 180
#define FREQUENCY_HZ 1

// set up adc values
int adc[3];
const int ADC1_PIN = 5;
const int ADC2_PIN = 6;
const int ADC3_PIN = 7;
const int DIG1_PIN = 22;
const int DIG2_PIN = 23;
const int DIG3_PIN = 24;
const int DIG4_PIN = 25;
const int DIG5_PIN = 26;

// moving average values
const float alpha = 0.9;
const float oneMinusAlpha = 1 - alpha;

// Expansion Board
BlitzExpansion expansion = BlitzExpansion(SLAVE_ADDRESS, BUFFER_SIZE, FREQUENCY_HZ);
BlitzMessage builder = BlitzMessage(SLAVE_ADDRESS);

void setup() {  
  // zero out ADC values
  adc[0] = 0;
  adc[1] = 0;
  adc[2] = 0;
  
  // Set ADCs as inputs
  pinMode(ADC1_PIN, INPUT);
  pinMode(ADC2_PIN, INPUT);
  pinMode(ADC3_PIN, INPUT);
  
  // set digital pins as inputs
  pinMode(DIG1_PIN, INPUT);
  pinMode(DIG2_PIN, INPUT);
  pinMode(DIG3_PIN, INPUT);
  pinMode(DIG4_PIN, INPUT);
  pinMode(DIG5_PIN, INPUT);
  
  // set up serial and send ID
  Serial.begin(57600);
  
  // start the expansion board
  expansion.begin(logCurrentMessage, &Serial);
}

void loop() {
  expansion.sample();
}

void logCurrentMessage(void) {
  int adc1 = analogRead(ADC1_PIN), 
      adc2 = analogRead(ADC2_PIN), 
      adc3 = analogRead(ADC3_PIN);
  BlitzFormattedMessage rawMessage;
      
  // update the averages
  adc[0] = adc[0] * alpha + adc1 * oneMinusAlpha;
  adc[1] = adc[1] * alpha + adc2 * oneMinusAlpha;
  adc[2] = adc[2] * alpha + adc3 * oneMinusAlpha;
  
  // pack in the ADC values
  builder.pack(adc[0], 10);
  builder.pack(adc[1], 10);
  builder.pack(adc[2], 10);
  
  // set the digital flags
  builder.setFlag(1, digitalRead(DIG1_PIN));
  builder.setFlag(2, digitalRead(DIG2_PIN));
  builder.setFlag(3, digitalRead(DIG3_PIN));
  builder.setFlag(4, digitalRead(DIG4_PIN));
  builder.setFlag(5, digitalRead(DIG5_PIN));
  
  // render the message
  builder.renderInto(rawMessage);
  
  // log the raw message
  expansion.log(rawMessage);
}
