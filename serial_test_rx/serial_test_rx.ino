const int buflen = 3;
uint8_t buf[buflen];
uint8_t a, s1, s2;
void setup() {
  Serial.begin(9600);
  while (!Serial) {};

//  Serial1.begin(9600);
//  while (!Serial1) {};
}

void loop() {
  while (Serial.available() < buflen) {};
  for (int i = 0; i<buflen; i++) {
    buf[i] = Serial.read();
  }
  
  Serial.print(buf[0]);
  Serial.print(" ");
  Serial.print(buf[1]);
  Serial.print(" ");
  Serial.println(buf[2]);
}
