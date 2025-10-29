/*
Student ID:
Student Name: Raja M. Sumeer
Module: Software for Real-Time and Embedded Systems @ Year 3, Semester 1 (Technological University Dublin)

## Code Purpose:
Configuring Timer1 in CTC mode to generate a 1Hz interrupt
and display count over USART serial communication.

## Last modified: 20/10/2025
*/

//-----------LIBRARIES------------//
#include <avr/io.h>
#include <avr/interrupt.h> // handling interrupts
#include "usart.h" // USART serial communication
#include <stdio.h> // for sprintf()
#define F_CPU 1000000UL // define clock speed @1MHz
#include <util/delay.h> // for _delay_ms()

//-----------GLOBAL VARIABLES------------//
uint16_t count; // count timer interrupts

/*-----------INTERRUPT SERVICE ROUTINE------------
             TIMER1 COMPARE MATCH INTERRUPT
Triggers when Timer1 reaches compare value, resets and triggers interrupt
*/
ISR(TIMER1_COMPA_vect)
{
  count++;
  PORTB ^= (1 << 5); // Toggle the LED
}

//-----------MAIN PROGRAM------------//
int main(void) 
{
//-----------REGISTER CONFIGURATIONS------------//
DDRB |= (1 << 5); // Set LED as output
usartInit(); // initialize USART

TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode
TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt when timer matches OCR1A value

OCR1A = 15624; // Set CTC compare value (FCPU/PRESCALER = 15624 counts)
TCCR1B |= ((1 << CS10) | (1 << CS11)); // Set prescaler of 64

sei(); // Enable global interrupts

//-----------LOCAL VARIABLES------------//
char countStr[10];
uint16_t oldCount = 0; // previous count

/*-----------INFINITE LOOP------------
*/
while(1) {
    usartSendString("Count is: ");
    sprintf(countStr,"%u\n",count-oldCount); // diff
    usartSendString(countStr);
    oldCount = count; // update old count

  _delay_ms(250);
 }
 return 0; // never reaches here
}