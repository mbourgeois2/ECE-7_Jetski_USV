const int buflen = 3;
char buf[buflen];
int dt = 60;

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  
  Serial1.begin(1000000);
  while (!Serial1) {};
}

void loop() {
  while (!Serial1.availableForWrite()) {};

  buf[0] = analogRead(A0) / 4;
  buf[1] = analogRead(A1) / 4;
  buf[2] = analogRead(A2) / 4;

  if (millis() % dt == 0) {
    Serial1.write(buf,buflen);
  } 
}
