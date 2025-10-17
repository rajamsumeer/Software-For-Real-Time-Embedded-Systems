#include <avr/io.h> // AVR input/output definitions

#define F_CPU 1000000UL // CPU clock speed @1MHz (util/delay)

#include <util/delay.h> // library for _delay_ms()

int main(void) 
{ 
  DDRB |= (1<<5); // set PB5 as output

  while(1) // infinite loop
  { 
  PORTB ^= (1<<5); // toggle (OR operation) PB5 (LED)
  _delay_ms(500); 
  } 
   return 0; 
}