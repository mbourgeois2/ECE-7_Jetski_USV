#include <Embedded_Template_Library.h>
#include <VarSpeedServo.h>

VarSpeedServo THROTTLE_S;

void setup() {
  // put your setup code here, to run once:
  TCCR0B = TCCR0B & B11111000 | B00000100; //chage PWM frequency on D4 and D13 to 244.14 Hz
  
  pinMode(4, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  THROTTLE_S.attach(4);  // attaches the servo on pin 9 to the servo object
  THROTTLE_S.write(0,255,true); // set the intial position of the servo, as fast as possible, wait until done

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  THROTTLE_S.write(180*map(analogRead(A1), 0, 1023, 0, 180)/180,255,false);
  Serial.println(analogRead(A0));
}
