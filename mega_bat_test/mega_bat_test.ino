/*
  SweepTwoServos
  By Philip van Allen <philvanallen.com> for the VarSpeedServo.h library (October 2013)
  This example code is in the public domain
  
  Sweep two servos from 0-180, 180-0 in unison
  Uses the wait feature of the 2013 version of VarSpeedServo to start the first servo moving in the background
  and immediately starting a second servo moving and waiting for the second one to finish.
  
  Note that two servos will require more power than is available from the USB port - use an external power supply!
*/

#include <VarSpeedServo.h> 
 
VarSpeedServo myservo1;  // create servo object to control a servo 
                         // a maximum of eight servo objects can be created 
VarSpeedServo myservo2;
 
const int servoPin1 = 4; // the digital pin used for the first servo
const int servoPin2 = 13; // the digital pin used for the second servo
int iteratorVariable = 0;
int ugo = 0;

int FeedbackRead(int x) {
 int t = analogRead(x);
 int u = 0;
 int v = 0;
 switch (x) {
 case A0:
 t = FeedbackSmooth(x);
 u = 30;
 v = 542;
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
 int w = FeedbackRead(A0);
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

 
 
void setup() { 
  myservo1.attach(servoPin1);  // attaches the servo on pin 9 to the servo object
  myservo1.write(0,255,false); // set the intial position of the servo, as fast as possible, run in background
  myservo2.attach(servoPin2);  // attaches the servo on pin 9 to the servo object
  myservo2.write(0,255,true);  // set the intial position of the servo, as fast as possible, wait until done

  aControl(2,3,0);
  Serial.begin(9600);
} 

void loop() {
  myservo1.write(180,127,false);        // move the servo to 180, fast speed, run background
                                        // write(degrees 0-180, speed 1-255, wait to complete true-false)
  myservo2.write(180,127,true);         // move the servo to 180, fast speed, wait until done
  
  myservo1.write(0,30,false);           // move the servo to 180, slow speed, run in background
  myservo2.write(0,30,true);            // move the servo to 180, slow speed, wait until done


  Serial.println(FeedbackRead(A0));
  if ((ugo == 254) && (FeedbackRead(A0) > 240)) {
    ugo = 0;
  }
  
  if ((ugo == 0) && (FeedbackRead(A0) < 10)) {
    ugo = 254;
  }
  aControl(2,3,ugo);
} 
