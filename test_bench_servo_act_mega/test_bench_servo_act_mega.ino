//--------------------------------------------------------
#include <Embedded_Template_Library.h>
#include "sbus.h"
#include <VarSpeedServo.h>

SbusRx x8r(&Serial3);

//--------------------------------------------------------------
VarSpeedServo THROTTLE_S;
VarSpeedServo CHOKE_S;
//--------------------------------------------------------------

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
int D_Kill = 49;
int D_Start = 47;

int FeedbackVal_Choke = 0;
int FeedbackVal_Throttle = 0;
int FeedbackVal_Steer = 0;

int I_O = 0;
int killState = 1;
boolean done;

int FeedbackRead(int x) {
 int t = analogRead(x);
 int u = 0;
 int v = 0;
 switch (x) {
 case A0:
 t = FeedbackSmooth(x);
 u = 31;
 v = 306;
 break;
 case A1:
 u = 55;
 v = 596;
 break;
 case A2:
 u = 34;
 v = 344;
 break;
 }
 return map(t, u, v, 0, 255);
}

int FeedbackSmooth(int num)
{
  int mean = 0;
  int result;
  for(int k = 0; k < 19; k++)
  {
    mean += analogRead(num);
  }
  result = mean/20;
  return(result);
}

void aControl(int x, int y, int z) {
 int w = FeedbackRead(Feedback_Act);
 if (w > z+2) {
    analogWrite(y, 0);
    analogWrite(x, 254);
 }
 else if (w < z-2) {
    analogWrite(y, 254);
    analogWrite(x, 0);
 }
 else if((w <= z+2) && (w >= z-2)) {
    analogWrite(y, 0);
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



void setup()
{
 TCCR0B = TCCR0B & B11111000 | B00000100; //chage PWM frequency on D4 and D13 to 244.14 Hz
 pinMode(PWM_Choke, OUTPUT);
 pinMode(PWM_Throttle, OUTPUT);
 pinMode(PWM_Steer1, OUTPUT);
 pinMode(PWM_Steer2, OUTPUT);
 pinMode(Feedback_Act, INPUT);
 pinMode(Feedback_S1, INPUT);
 pinMode(Feedback_S2, INPUT);
 pinMode(D_Kill, OUTPUT);
 pinMode(D_Start, OUTPUT);

 //reset choke and throttle
 THROTTLE_S.attach(PWM_Throttle);  // attaches the servo on pin 9 to the servo object
 THROTTLE_S.write(0,255,true); // set the intial position of the servo, as fast as possible, wait until done
  
 CHOKE_S.attach(PWM_Choke);  // attaches the servo on pin 9 to the servo object
 CHOKE_S.write(0,255,true); // set the intial position of the servo, as fast as possible, wait until done

  
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
    //------------------------------------------------------------------------------------------
    FeedbackVal_Steer = FeedbackRead(Feedback_Act);
    Serial.println(FeedbackVal_Steer);
    //FeedbackVal_Steer = int(x);
    //FeedbackVal_Choke = FeedbackRead(Feedback_S1);
    //FeedbackVal_Throttle = FeedbackRead(Feedback_S2);
    //Serial.println(FeedbackRead(A1));
    //Serial.println(FeedbackVal_Steer);
    //------------------------------------------------------------------------------------------
    aControl(PWM_Steer1, PWM_Steer2, channels[steer]);
    CHOKE_S.write(180*map(channels[8], 0, 255, 0, 180)/180,255,false);
    THROTTLE_S.write(180*map(channels[throttle], 0, 255, 0, 180)/180,255,false);
    //end control code
    //------------------------------------------------------------------------------------------
  }
  else {
    // Serial.println("nothin");
    for (int i = 0; i < numchannels; i++) {
      channels[i] = 0;
    }
  }
  //---------------------------------------------------------------------------------------------
}
