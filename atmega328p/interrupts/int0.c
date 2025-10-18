/*
Student ID:
Student Name: Raja M. Sumeer
Module: Software for Real-Time and Embedded Systems @ Year 3, Semester 1 (Technological University Dublin)

## Code Purpose:
Configuring and counting EXTERNAL interrupts on PD2 and sending information over USART.
Frequency estimation between interrupt counts.

## Last modified:
*/
#include <avr/io.h>
#include "usart.h" // USART header for serial communication
#include "stdio.h" // for sprintf()
#define F_CPU 1000000UL // define CPU clock speed (1 MHz)
#include <util/delay.h> // delay functions (_delay_ms)
#include <avr/interrupt.h> // for handling interrupts

// Count how many times the interrupt occurs
uint16_t ISRCOUNT;

// Triggered whenever INT0 detects edge
ISR(INT0_vect) 
{
  ISRCOUNT++; // Increment count each time the interrupt triggers
  PORTB ^= (1<<5); // Toggle LED on PB5
}

int main(void) 
{
  // I/O
  DDRB |= (1<<5); // Set PB5 as output (LED)
  DDRD &= ~(1<<2); // Set PD2 as input
  PORTD |= (1<<2); // Enable internal pull-up resistor on PD2

  usartInit(); // Initialize serial communication

  // Interrupts
  EICRA |= (1<<ISC01); // IRQ
  EIMSK |= (1<<INT0);  // Enable interrupt INT0
  sei(); // Enable global interrupts

  // Buffer, Count & Frequency variables
  char str[10];
  uint8_t loopCount = 0;
  uint16_t oldCount = ISRCOUNT;
  uint16_t frequency;

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

    // Check PIN state
    if (PINB & (1<<5)) {
      usartSendString("PIN HIGH\n"); // LED ON
    } else {
      usartSendString("PIN LOW\n"); // LED OFF
    }

    sprintf(str, "Count: %u\n", ISRCOUNT);
    usartSendString(str);

    _delay_ms(250);
  } 

  return 0; 
}
