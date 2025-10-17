<<<<<<< HEAD
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
=======
#include <avr/io.h> 

#define F_CPU 1000000UL 

#include <util/delay.h> 

int main(void) 
{ 
  DDRB |= (1<<5); 

  while(1)  { 
  PORTB ^= (1<<5); 
  _delay_ms(500); 
  } 
   return 0; 
 }                      
>>>>>>> f89a42fb7c43eabbad64ca61df6d1016c1143fb2
