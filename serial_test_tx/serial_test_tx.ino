bool wat = false;

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(52, INPUT);

  Serial2.begin(1000000);
  while (!Serial2) {};
}

void loop() {
osdsendfeed(analogRead(A0) / 4*100/255,analogRead(A1) / 4*100/255,analogRead(A2) / 4*100/255, digitalRead(52),60);
//Serial.println(digitalRead(52));
}

//void serialwritefeed() {
//  while (!Serial1.availableForWrite()) {};
//
//  int buflen = 3;
//  char buf[buflen];
//  int dt = 60;
//
//  buf[0] = analogRead(A0) / 4;
//  buf[1] = analogRead(A1) / 4;
//  buf[2] = analogRead(A2) / 4;
//
//  if (millis() % dt == 0) {
//    Serial1.write(buf,buflen);
//  } 

//}


void osdsendfeed(byte a, byte b, byte c, byte d, int t) {
  while (Serial2.availableForWrite() < 5) {};
  if (millis() % t == 0) {
    Serial2.write(a);
    Serial2.write(b);
    Serial2.write(c);
    if (millis() % t*5 == 0)
      Serial2.write(d);
    //Serial2.flush();
  } 
}
