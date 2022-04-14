#include <TVout.h>
#include <fontALL.h>

TVout TV;
const int buflen = 3;
byte buf[buflen];
bool alt = 0;

void setup()  {
  TV.begin(PAL,120,96);
  TV.select_font(font6x8);
  initOverlay();
  TV.println(0,70,("Steer    "));
  TV.println(("Throt    "));
  TV.print(("Choke    "));
  
  Serial.begin(1000000);  //big guy
}

void initOverlay() {
  TCCR1A = 0;
  // Enable timer1.  ICES0 is set to 0 for falling edge detection on input capture pin.
  TCCR1B = _BV(CS10);

  // Enable input capture interrupt
  TIMSK1 |= _BV(ICIE1);

  // Enable external interrupt INT0 on pin 2 with falling edge.
  EIMSK = _BV(INT0);
  EICRA = _BV(ISC01);
}

// Required to reset the scan line when the vertical sync occurs
ISR(INT0_vect) {
  display.scanLine = 0;
}

void loop() {
  if (Serial.available() >= buflen) {
    for (int i=0;i<buflen;i++) {
      buf[i] = Serial.read();
    }
    
    while (Serial.available()) {Serial.read();};
    Serial.flush();

    if (alt == true) {
      TV.set_cursor(55,70);
      TV.print('\b'); TV.print('\b'); TV.print('\b');
      TV.print(int(buf[0])*100/255);

      TV.set_cursor(55,78);
      TV.print('\b'); TV.print('\b'); TV.print('\b');
      TV.println(int(buf[1])*100/255);

      TV.set_cursor(55,86);
      TV.print('\b'); TV.print('\b'); TV.print('\b');
      TV.println(int(buf[2])*100/255);
    }
    alt = ~alt;

    while (Serial.available()) {Serial.read();};
  }
}
