#include <BlitzExpansion.h>

#define SLAVE_ADDRESS 0x08
#define BUFFER_SIZE 180
#define SAMPLE_FREQUENCY 10    // 10 Hz sample rate
#define SEND_FREQUENCY 10      // Save a message every 10 samples - i.e. send at 1 Hz

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

// moving average values - not strictly necessary but provides a tiny bit of smoothing
const float alpha = 0.5;
const float oneMinusAlpha = 1 - alpha;

// Expansion Board
BlitzExpansion expansion = BlitzExpansion(SLAVE_ADDRESS, BUFFER_SIZE, SAMPLE_FREQUENCY, SEND_FREQUENCY);
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
  
  // connect up the required functions and get going
  expansion.connect(sampleAdcs, logMessage, &Serial);
}

/** 
 * Runs the expansion board loop, which samples the board at the required frequency.
 * It samples at SAMPLE_FREQUENCY rate and saves messages for sending every SEND_FREQUENCY samples
 */
void loop() {
  expansion.sample();
}

/**
 * Called at SAMPLE_FREQUENCY Hz to save ADC values into memory.  Messages should not be send
 * or logged from this.
 */
void sampleAdcs() {
  int adc1 = analogRead(ADC1_PIN), 
      adc2 = analogRead(ADC2_PIN), 
      adc3 = analogRead(ADC3_PIN);
      
  // update the averages
  adc[0] = adc[0] * alpha + adc1 * oneMinusAlpha;
  adc[1] = adc[1] * alpha + adc2 * oneMinusAlpha;
  adc[2] = adc[2] * alpha + adc3 * oneMinusAlpha;
}

/** 
 * Called every SEND_FREUQENCY samples, saves a message for transmission to the data logger using
 * the BlitzExpansion.log() function. 
 */
void logMessage() {
  BlitzFormattedMessage rawMessage;
  
  // pack in the ADC values
  builder.pack(adc[0], 12);
  builder.pack(adc[1], 12);
  builder.pack(adc[2], 12);
  
  // set the digital flags
  builder.setFlag(1, digitalRead(DIG1_PIN));
  builder.setFlag(2, digitalRead(DIG2_PIN));
  builder.setFlag(3, digitalRead(DIG3_PIN));
  builder.setFlag(4, digitalRead(DIG4_PIN));
  builder.setFlag(5, digitalRead(DIG5_PIN));
  
  // render the message
  builder.renderInto(rawMessage);
  
  // log the raw message - MUST be included otherwise no messages will be logged
  expansion.log(rawMessage);
}

bool handleInstruction(unsigned char instructionId, unsigned short* payload) {
  return true;
} 