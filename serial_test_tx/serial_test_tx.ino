uint8_t a,s1,s2;
const int buflen = 3;
byte buf[buflen];
int dt = 60;
void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  Serial.begin(9600);
  while (!Serial) {};
}

void loop() {
  while (!Serial.availableForWrite()) {};
  buf[0] = analogRead(A0) / 4;
  buf[1] = analogRead(A1) / 4;
  buf[2] = analogRead(A2) / 4;
  if (millis() % dt == 0)
    Serial.write(buf,buflen);
}
