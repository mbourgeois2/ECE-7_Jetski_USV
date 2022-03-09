//--------------------------------------------------------
#include <Embedded_Template_Library.h>
#include "sbus.h"

SbusRx x8r(&Serial3);
uint16_t channels[16];
int numchannels = 12;
int PWM_MAX = 255;
int throttle = 0;
int steer = 1;
int start = 7;
int kill = 6;
//--------------------------------------------------------------
int PWM_Throttle = 4;
int PWM_Choke = 13;
int PWM_Steer1 = 2;
int PWM_Steer2 = 3;
int Feedback_Act = A0;
int Feedback_S1 = A1;
int Feedback_S2 = A2;
//  int D_Kill = 3;
//  int D_Start = 2;
int I_O = 0;
int killState = 1;
boolean done;

void setup()
{
 TCCR0B = TCCR0B & B11111000 | B00000100; //chage PWM frequency on D5 and D6 to 244.14 Hz
 pinMode(PWM_Choke, OUTPUT);
 pinMode(PWM_Throttle, OUTPUT);
 pinMode(PWM_Steer1, OUTPUT);
 pinMode(PWM_Steer2, OUTPUT);
 pinMode(Feedback_Act, INPUT);
 pinMode(Feedback_S1, INPUT);
 pinMode(Feedback_S2, INPUT);
 
// pinMode(31, OUTPUT);
// while (true) {
// digitalWrite(31, HIGH);
// delay(500);
// digitalWrite(31, LOW);
// delay(500);
// }
 
//   for (int j = 3; j++; j<14) {
// for (int i = 1; i++; i<256) {
//  pinMode(j, OUTPUT);
// analogWrite(j, i);
// delay(20);
// }
//   }

 //------------------------------------------------------
 x8r.Begin();
 Serial.begin(9600);
 //------------------------------------------------------
}

void loop()
{
  //------------------------------------------------------------------------------------------------
  //Serial.begin(9600);
  if (x8r.Read()) {
    for (int i = 0; i < numchannels; i++) {
      int val = map(x8r.rx_channels()[i], 185, 1811, 0, PWM_MAX);
      channels[i] = val;
      if (channels[i] > PWM_MAX) {
        channels[i] = 0;
      }
    }
    //-----------========================================================================================================--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------==============-
    aControl(Feedback_Act, PWM_Steer1, PWM_Steer2, channels[steer]);
    sControl(Feedback_S1, PWM_Choke, channels[8]);
    sControl(Feedback_S2, PWM_Throttle, channels[throttle]);
    Serial.println(FeedbackSmooth(A0));
    
    //end control code
    //---------------------------------------------------------------------------------
  }
  else {
    // Serial.println("nothin");
    for (int i = 0; i < numchannels; i++) {
      channels[i] = 0;
    }
  }
  //---------------------------------------------------------------------------------------------
}
  
void aControl(int w, int x, int y, int z) {
 
// while ((z != analogRead(w)/4 + 4) || (z != analogRead(w)/4 - 4)) {
// if (z < 125)
// {
// // reverse rotation
// analogWrite(y, 0);
// analogWrite(x, 255);
// }
// if (z > 130)
// {
// // forward rotation
// analogWrite(y, 255);
// analogWrite(x, 0);
// }
// }

 z=z*4;
 if (z < 505)
 {
 // reverse rotation
 int reversePWM = -(z - 511) / 2;
 analogWrite(y, 0);
 analogWrite(x, reversePWM);
 }
 if (z > 520)
 {
 // forward rotation
 int forwardPWM = (z - 512) / 2;
 analogWrite(y, forwardPWM);
 analogWrite(x, 0);
 }

 
}

void sControl(int w, int x, int y) {
  analogWrite(x,y);
  //if ((analogRead(w)/4 + 5 != x) || (analogRead(w)/4 - 5 != x)){
  //analogWrite(x,y);
  //}
}


void ski_kill(int x, int y, int z) {
  if (y < (PWM_MAX-1)/2) {
    digitalWrite(x, HIGH);
  }
  if (y > (PWM_MAX-1)/2) {
    digitalWrite(x, LOW);
    digitalWrite(z, LOW);
    I_O = 0;
    killState = 1;
  }
}

void ski_start(int x, int y) {
  if ((y > (PWM_MAX-1)/2) && (killState == 0)) {
    digitalWrite(x, HIGH);
    I_O = 1;
  }
  if ((y < (PWM_MAX-1)/2)) {
    digitalWrite(x, LOW);
    I_O = 0;
    killState = 0;
  }
}

int FeedbackSmooth(int num)
{
  int mean;
  int reading[20];
  int temp;
  int result;
  for(int q = 0; q < 20; q++)
  {
    reading[q] = analogRead(num);
    // delay here maybe? delay(3);
  }
  done = false;
  while(done != true)
  {
    done = true;
    for(int q = 0; q < 20; q++)
    {
      temp = 0;
      if(reading[q] > reading[q + 1])
      {
        temp = reading[q + 1];
        reading[q + 1] = reading[q];
        reading[q] = temp;
        done=false;
      }
    }
  }
  mean = 0;
  for(int k = 6; k < 14; k++)
  {
    mean += reading[k];
  }
  result=mean/14;
  return(result);
}
