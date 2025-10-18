/*
Student ID:
Student Name: Raja M. Sumeer
Module: Software for Real-Time and Embedded Systems @ Year 3, Semester 1 (Technological University Dublin)

## Code Purpose:
Configuring Timer1 in CTC mode to generate a 1Hz interrupt
and display count over USART

## Last modified:
*/
#include <avr/io.h>
#include <avr/interrupt.h> // handling interrupts
#include "usart.h" // USART serial communication
#include <stdio.h> // for sprintf()

#define F_CPU 1000000UL // define clock speed @1MHz

#include <util/delay.h> 

uint16_t count; // count timer interrupts variable

// Timer 1 Compare Match ISR
ISR(TIMER1_COMPA_vect)
{
  count++;
  PORTB ^= (1 << 5); // Toggle the LED
}

int main(void) {
DDRB |= (1 << 5); // Set LED as output
usartInit(); // initialize USART

TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode
TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt when matches OCR1A

OCR1A = 15624; // Set CTC compare value (FCPU/PRESCALER = 15624 counts)
TCCR1B |= ((1 << CS10) | (1 << CS11)); // prescaler 64 + FCPU

sei(); // Enable global interrupts

char countStr[10];
uint8_t loopCtr = 0;
uint16_t oldCount = 0; // previous count

while(1) {
  loopCtr++;
if (loopCtr >= 4) { // 250ms x 4 = 1s
    usartSendString("Count is: ");
    sprintf(countStr,"%u\n",count-oldCount); // diff
    usartSendString(countStr);
    loopCtr = 0; // reset counter
    oldCount = count; // update old count
}
  _delay_ms(250);
 }
 return 0;
}
