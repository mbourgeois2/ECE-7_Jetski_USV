#include <Embedded_Template_Library.h>
#include "sbus.h"

byte start = 2;
byte kill = 3;
byte cam = 4;

byte sw_start = 7;
byte sw_kill = 6;
byte sw_cam = 11;

SbusRx x8r(&Serial);

const int PWM_MAX = 255;
const int numchannels = 25;
uint16_t channels[numchannels];

void setup() {
  // put your setup code here, to run once:
  //pinMode(5,OUTPUT);

  //start/kill/cam stuff start
  pinMode(start, OUTPUT);
  pinMode(kill, OUTPUT);
  pinMode(cam, OUTPUT);
  
  digitalWrite(start, 0);
  digitalWrite(kill, 0);
  digitalWrite(kill, 1);
  //start/kill stuff end
  
  x8r.Begin();
  //Serial.begin(100000);
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

  startswitch(channels[sw_start]);
  killswitch(channels[sw_kill]);
  camswitch(channels[sw_cam]);

  }
  else {
    for (int i = 0; i < numchannels; i++) {
      channels[i] = 0;
    }
  }
}

void startswitch(byte s) {
    if (s > 200) {
      digitalWrite(start, 1);
    }
    else if (s < 200) {
      digitalWrite(start, 0);
    }
}

void killswitch(int k) {
    if (k > 200) {
      digitalWrite(kill, 1);
    }
    else if (k < 200) {
      digitalWrite(kill, 0);
    }
}

void camswitch(int c) {
      if (c > 200) {
      digitalWrite(cam, 1);
    }
    else if (c <= 200 && c > 100) {
      digitalWrite(cam, 1);
    }
    else if (c <= 100) {
      digitalWrite(cam, 0);
    }
}
