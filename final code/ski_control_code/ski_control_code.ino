//--------------------------------------------------------
#include <Embedded_Template_Library.h>
#include "sbus.h"
#include <VarSpeedServo.h>

SbusRx x8r(&Serial3);

//--------------------------------------------------------------
VarSpeedServo THROTTLE_S;
VarSpeedServo CHOKE_S;
//--------------------------------------------------------------

const int numchannels = 18;
uint16_t channels[numchannels];
const int PWM_MAX = 255;
const int throttle = 0;
const int choke = 8;
const int steer = 1;
const int kill = 7;
const int start = 6;
const int cam = 11;
//--------------------------------------------------------------
const int PWM_Throttle = 4;
const int PWM_Choke = 13;
const int PWM_Steer1 = 2;
const int PWM_Steer2 = 3;
const int Feedback_Act = A0;
const int Feedback_S1 = A1;
const int Feedback_S2 = A2;
const int waterSensor = A3;
const int D_Kill = 49;
const int D_Start = 47;
const int D_Cam = 31;

int FeedbackVal_Choke = 0;
int FeedbackVal_Throttle = 0;
int FeedbackVal_Steer = 0;

int I_O = 0;
int killState = 1;
boolean done;

byte FeedbackRead(int x) {
 //int t = analogRead(x);
 int t = FeedbackSmooth(x);
 int u = 0;
 int v = 0;
 switch (x) {
 case Feedback_Act:
 //t = FeedbackSmooth(x);
 u = 30;
 v = 414;
//414 = 3in
 //542 = 4in
 break;
 case Feedback_S1:
 u = 55;
 v = 596;
 break;
 case Feedback_S2:
 u = 55;
 v = 600;
 break;
 }
 int res = map(t, u, v, 0, PWM_MAX);
   if (res < 0)
    res = 0;
  else if (res > PWM_MAX)
    res = PWM_MAX;
  return res;
}

int FeedbackSmooth(int num)
{
  int mean = 0;
  int result;
  for(int k = 0; k < 29; k++)
  {
    mean += analogRead(num);
  }
  result = mean/30;
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

void aMax(int x, int y, int z) {
  if (z = 1) {
      //extend
      analogWrite(y, 254);
      analogWrite(x, 0);
  }
  else if (z = -1) {
     //retract
     analogWrite(y, 0);
     analogWrite(x, 254);
  }
}

void startswitch(byte s) {
    if (s > 200) {
      digitalWrite(D_Start, 1);
    }
    else if (s < 200) {
      digitalWrite(D_Start, 0);
    }
}

void killswitch(int k) {
    if (k > 200) {
      digitalWrite(D_Kill, 1);
    }
    else if (k < 200) {
      digitalWrite(D_Kill, 0);
    }
}

void camswitch(int c) {
      if (c > 200) {
      digitalWrite(cam, 1);
    }
    else if (c <= 200 && c > 100) {
      digitalWrite(D_Cam, 1);
    }
    else if (c <= 100) {
      digitalWrite(D_Cam, 0);
    }
}

void shutdown() {
      for (int i = 0; i < numchannels; i++) {
        channels[i] = 0;
      }
      channels[steer] = 128;
}

void osdsendfeed(byte a, byte b, byte c, byte d) {
  while (Serial2.availableForWrite() < 4) {}; 
    Serial2.write(a);
    Serial2.write(b);
    Serial2.write(c);
    Serial2.write(d);
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
 pinMode(waterSensor, INPUT);
 pinMode(D_Kill, OUTPUT);
 pinMode(D_Start, OUTPUT);
 pinMode(D_Cam, OUTPUT);

 digitalWrite(D_Start, 0);
 digitalWrite(D_Kill, 1);
 digitalWrite(D_Cam, 1);
 

 //reset choke and throttle
 THROTTLE_S.attach(PWM_Throttle);  // attaches the servo on pin 9 to the servo object
 THROTTLE_S.write(180,255,true); // set the intial position of the servo, as fast as possible, wait until done
  
 CHOKE_S.attach(PWM_Choke);  // attaches the servo on pin 9 to the servo object
 CHOKE_S.write(180,255,true); // set the intial position of the servo, as fast as possible, wait until done

  
 //------------------------------------------------------
 x8r.Begin();
 //Serial.begin(9600);
 Serial2.begin(9600);
 //------------------------------------------------------
}

void loop()
{
  //------------------------------------------------------------------------------------------------
  if (x8r.Read()) {
    for (int i = 0; i < numchannels; i++) {
      int val = map(x8r.rx_channels()[i], 185, 1811, 0, PWM_MAX);
      channels[i] = val;
      if (channels[i] > PWM_MAX) {
        channels[i] = 0;
      }
    }
    //------------------------------------------------------------------------------------------
    startswitch(channels[start]);
    killswitch(channels[kill]);
    camswitch(channels[cam]);
    //Serial.println(channels[kill]);
    
    FeedbackVal_Steer = 255-FeedbackRead(Feedback_Act);
    FeedbackVal_Throttle = 255-FeedbackRead(Feedback_S1);
    FeedbackVal_Choke = 255-FeedbackRead(Feedback_S2);
    
    if (Serial2) {
      osdsendfeed(FeedbackVal_Steer*100/255, FeedbackVal_Throttle*100/255, 
      FeedbackVal_Choke*100/255, 0);
    }

//    osdsendfeed(FeedbackVal_Steer, FeedbackVal_Throttle, 
//    FeedbackVal_Choke, 1);
    
    //Serial.println(FeedbackVal_Steer);
    //Serial.println(FeedbackVal_Throttle);
    //Serial.println(FeedbackVal_Choke);
    //Serial.println(analogRead(A0));
    //Serial.println(analogRead(A1));
    //Serial.println(analogRead(A2));
    //------------------------------------------------------------------------------------------
    aControl(PWM_Steer1, PWM_Steer2, 255-channels[steer]);
    CHOKE_S.write(180*map(255-channels[throttle], 0, 255, 0, 180)/180,255,false);
    THROTTLE_S.write(180*map(255-channels[choke], 0, 255, 0, 180)/180,255,false);
    
    //end control code
    //------------------------------------------------------------------------------------------
  }
  else {
    // Serial.println("nothin");
//    for (int i = 0; i < numchannels; i++) {
//      channels[i] = 0;
//    }
      shutdown();
  }
  Serial.println(channels[1]);
  //---------------------------------------------------------------------------------------------
}
