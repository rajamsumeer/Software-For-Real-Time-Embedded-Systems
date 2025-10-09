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
