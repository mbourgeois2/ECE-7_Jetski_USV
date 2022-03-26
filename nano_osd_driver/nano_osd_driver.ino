#include <TVout.h>
#include <fontALL.h>

TVout TV;

void setup() {
  // put your setup code here, to run once:
  TV.begin(PAL,120,96);
  TV.select_font(font4x6);
  initOverlay();
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
  // put your main code here, to run repeatedly:
  TV.clear_screen();
  TV.print(0,85,"Throttle ");
  TV.print((int)((float)analogRead(A0)/1023 * 100));
  TV.print("%\n");
  TV.print("Choke ");
  TV.print("2%");
  TV.delay(50);
}
