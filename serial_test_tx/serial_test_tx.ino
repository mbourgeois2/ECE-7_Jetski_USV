void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  
  Serial1.begin(1000000);
  while (!Serial1) {};
}

void loop() {
osdsendfeed(analogRead(A0) / 4,analogRead(A1) / 4,analogRead(A2) / 4,60);
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

void osdsendfeed(byte a, byte b, byte c, int t) {
  while (!Serial1.availableForWrite()) {};
  if (millis() % t == 0) {
    Serial1.write(a);
    Serial1.write(b);
    Serial1.write(c);
  } 
}
