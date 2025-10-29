/*
Student ID:
Student Name: Raja M. Sumeer
Module: Software for Real-Time and Embedded Systems @ Year 3, Semester 1 (Technological University Dublin)

## Code Purpose:
Configuring and counting EXTERNAL interrupts on PD2 and sending information over USART.
Frequency estimation between interrupt counts.

## Last modified: 20/10/2025
*/

//-----------LIBRARIES------------//
#include <avr/io.h>
#include "usart.h" // USART header for serial communication
#include "stdio.h" // for sprintf()
#define F_CPU 1000000UL // define CPU clock speed (1 MHz)
#include <util/delay.h> // for _delay_ms()
#include <avr/interrupt.h> // for handling interrupts

//-----------GLOBAL VARIABLES------------//
uint16_t ISRCOUNT; // Count how many times the interrupt occurs

/*-----------INTERRUPT SERVICE ROUTINE------------
             INT0 INTERRIPT
Triggers whenever a RISING edge is detected on INT0
*/
ISR(INT0_vect) 
{
  ISRCOUNT++; // Increment count each time the interrupt triggers
  PORTB ^= (1<<5); // Toggle LED on PB5 for visual debugging
}

//-----------MAIN PROGRAM------------//
int main(void) 
{
//-----------REGISTER CONFIGURATIONS------------//
  DDRB |= (1<<5); // Set PB5 as output (LED)
  DDRD &= ~(1<<2); // Set PD2 as input
  PORTD &= ~(1<<2); // No internal pull-up resistor on PD2

  usartInit(); // Initialize serial communication

  EICRA |= (1<<ISC01); // INT0 triggers on RISING edge
  EIMSK |= (1<<INT0);  // Enable INT0 interrupt

  sei(); // Enable global interrupts

//-----------LOCAL VARIABLES------------//
  char str[10];
  uint8_t loopCount = 0;
  uint16_t oldCount = ISRCOUNT; // previous count
  uint16_t frequency;

/*-----------INFINITE LOOP------------
COUNTS INTERRUPTS OCCURED EACH SECOND
*/
  while(1) 
  { 
    loopCount++;
    if (loopCount == 4) {
      loopCount = 0;

      // Calculcate frequency
      frequency = ISRCOUNT - oldCount;

      sprintf(str, "Frequency: %u\n", frequency);
      usartSendString(str);

      // Update old count for next calculation
      oldCount = ISRCOUNT;
    }

    sprintf(str, "Count: %u\n", ISRCOUNT);
    usartSendString(str);

    _delay_ms(250);
  } 
  return 0; // never reaches heres
}