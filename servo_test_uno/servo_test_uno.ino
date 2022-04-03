#include <Embedded_Template_Library.h>
#include "sbus.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
const int ledPin = 5; // the digital pin used for the servo

// a SBUS object, which is on hardware
// serial port 1
SbusRx x8r(&Serial);

// channel, fail safe, and lost frames data
uint16_t channels[12];
int numchannels = 12;
int AUX = 0;

void setup() {

  //  PORTB |= 0b00000001;
  //  DDRD |= 0b11111101;
  //  DDRB &= 0b11111110;


  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  pinMode(5,OUTPUT);
  //TCCR0B = TCCR0B & B11111000 | B00000100; // for PWM frequency of 244.14 Hz
  //TCCR0B = TCCR0B & B11111000 | B00000101; // for PWM frequency of 61.04 Hz


  //  Serial.begin(9600);
  //  while (!Serial) {}
  //  // begin the SBUS communication
  x8r.Begin();
}



void loop() {
  //PORTD = ~(PINB<<2);

  if (x8r.Read()) {
    for (int i = 0; i < numchannels; i++) {
      //digitalWrite(2,!digitalRead(8));
      int val = map(x8r.rx_channels()[i], 185, 1811, 0, 255);
      channels[i] = val;
      if (channels[i] > 255) {
        channels[i] = 0;
      }



      //ch[i].writeMicroseconds(val);
      //Serial.println(i);
      //Serial.println(channels[0]);
    }
  }
  else {
    // Serial.println("nothin");
    for (int i = 0; i < numchannels; i++) {
      channels[i] = 0;
    }
  }
  analogWrite(5, channels[AUX]);
  //analogWrite(6, channels[AUX]);
  
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(channels[AUX]);
  //display.println(analogRead(A0));
  display.display();

  //if ((analogRead(w)/4 + 5 != x) || (analogRead(w)/4 - 5 != x)){
  //analogWrite(x,y);
  //}
}
