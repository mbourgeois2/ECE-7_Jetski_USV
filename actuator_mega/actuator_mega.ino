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
    analogWrite(y, 254);
    analogWrite(x, 0);
}

//void aControl(int x, int y, int z) {
// int w = FeedbackRead(A0);
// if (w > z+2) {
//    analogWrite(y, 0);
//    analogWrite(x, 254);
// }
// else if (w < z-2) {
//    analogWrite(y, 254);
//    analogWrite(x, 0);
// }
// else if((w <= z+2) && (w >= z-2)) {
//    analogWrite(y, 0);
//    analogWrite(x, 0);
// }
//}

 
 
void setup() { 
  aControl(2,3,0);
  Serial.begin(9600);
} 

void loop() {
  aControl(2,3,ugo);
  //Serial.println(FeedbackRead(A0));
  Serial.println(analogRead(A0));
} 
