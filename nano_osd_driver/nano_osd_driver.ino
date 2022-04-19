#include <TVout.h>
#include <fontALL.h>

TVout TV;

const int buflen = 4;
byte buf[buflen];
bool alt = true;
bool wet = false;

void setup()  {
  TV.begin(PAL,87,87);
  TV.select_font(font6x8);
  initOverlay();
  TV.println(0,60,("St"));
  TV.println("Th");
  TV.print("Ch");
  
  Serial.begin(1000000);  //big guy
  while (!Serial) {}
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
  //
  sei();
  if (Serial.available() >= buflen) {
    for (int i=0;i<buflen;i++) {
      buf[i] = Serial.read();
    }
    
    //while (Serial.available()) {Serial.read();};

    if (alt) {
      TV.set_cursor(45,60);
      TV.print('\b'); TV.print('\b'); TV.print('\b'); TV.print('\b');
      TV.print((int(buf[0]) - 50)*2);

      TV.set_cursor(39,68);
      TV.print('\b'); TV.print('\b'); TV.print('\b');
      TV.println(int(buf[1]));

      TV.set_cursor(39,76);
      TV.print('\b'); TV.print('\b'); TV.print('\b');
      TV.println(int(buf[2]));

      if (buf[3] && !wet) {
        TV.print(0,0,"!!!");
        wet = true;
      }
      else if (!buf[3] && wet) {
        TV.print(0,0,"   ");
        wet = false;
      }
    }
    alt = ~alt;

    while (Serial.available()) {Serial.read();};
}
}
