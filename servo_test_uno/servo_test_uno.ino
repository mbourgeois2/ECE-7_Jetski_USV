#include <Embedded_Template_Library.h>
#include "sbus.h"
#include <VarSpeedServo.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



VarSpeedServo myservo;  // create servo object to control a servo 
                        // a maximum of eight servo objects can be created 
 
const int servoPin = 5; // the digital pin used for the servo

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

  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(0,255,true); // set the intial position of the servo, as fast as possible, wait until done

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  pinMode(A0,INPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  TCCR0B = TCCR0B & B11111000 | B00000100; // for PWM frequency of 244.14 Hz
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
  //analogWrite(5, channels[AUX]);
  //analogWrite(6, channels[AUX]);
  sControl(analogRead(A0), 5, channels[AUX]);
  
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(channels[AUX]);
  display.println(FeedbackSmooth(A0, 60));
  //display.println(analogRead(A0));
  display.display();

  //if ((analogRead(w)/4 + 5 != x) || (analogRead(w)/4 - 5 != x)){
  //analogWrite(x,y);
  //}
}

void sControl(int w, int x, int y) {
//  for (int i=0; i <= x; i++ ){
//    analogWrite(x,y);
//    delay(15);
//}
int val = map(y, 0, 255, 0, 180);   //112-180 works, 255 breaks it for some reason
  myservo.write(180*val/180,255,false);
}

int FeedbackSmooth(int num, int samples)
{
  int samp[samples];
  int result;
  //collect samples
  for(int k = 0; k < samples; k++)
  {
    samp[k] = analogRead(num);
  }
  //sort samples
  qsort(samp, samples, sizeof(int), compare);
  
  //remove bottom 20% of samples
  for(int i = 0; i < (samples / 5); i++)
  {
    samp[i] = 0;
  } 
  
  //remove top 20% of samples
  for(int j = samples - samples / 5; j < samples; j++)
  {
    samp[j] = 0;
  } 
  
  //calculate avg of samples
  int sum = 0;
  for(int m = 0; m < samples; m++)
  {
    sum += samp[m];
  }
  result = sum/(samples - (samples/5)*2);
  return result;
}

int compare( const void* a, const void* b)
{
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}
