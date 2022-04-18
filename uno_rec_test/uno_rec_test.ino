#include <Embedded_Template_Library.h>
#include "sbus.h"

SbusRx x8r(&Serial);

const int PWM_MAX = 255;
const int numchannels = 25;
uint16_t channels[numchannels];

void setup() {
  // put your setup code here, to run once:
  //pinMode(5,OUTPUT);
  x8r.Begin();
  Serial.begin(100000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (x8r.Read()) {
    for (int i = 0; i < numchannels; i++) {
      int val = map(x8r.rx_channels()[i], 185, 1811, 0, PWM_MAX);
      channels[i] = val;
        if (channels[i] > PWM_MAX) {
          channels[i] = 0;
        }
    }
    
    Serial.println(channels[0]);
    //analogWrite(5,channels[0]);
    
  }
    else {
    //Serial.println("nothin");
    for (int i = 0; i < numchannels; i++) {
      channels[i] = 0;
    } 
  }
}
