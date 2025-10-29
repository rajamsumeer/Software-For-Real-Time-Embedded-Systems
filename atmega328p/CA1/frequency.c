/*
Student ID:
Student Name: Raja M Sumeer
Module: Software for Real-Time and Embedded Systems @ Year 3, Semester 1 (Technological University Dublin)

Last modified: 20/10/25
Code Purpose:
Measuring the frequency of an external square-wave signal connected to pin PD2 (INT0 mapped interrupt).
Timer1 is configured in CTC mode to generate a 1s interrupt.
The number of counts is used to calculate the frequency of the incoming signal and displayed over USART.

*/

//-------LIBRARIES-------//
#include <avr/io.h>
#include <avr/interrupt.h> // for handling interrupts/ISR
#include <stdio.h> // for sprintf()
#include "usart.h"

#define F_CPU 1000000UL // define clock speed @1MHz
#include <util/delay.h> // for _delay_ms()

//-------GLOBAL VARIABLES-------//
volatile uint16_t count = 0; // counts rising edges (pulses)
volatile uint16_t oldCount = 0; // previous count value (to find frequency)
volatile uint16_t frequency = 0; // count and oldcount difference

//-------INTERRUPT SERVICE ROUTINE FOR INT0-------//
// PD2(INT0)
ISR(INT0_vect) {
  count++; // increment count each rising edge on PD2
}

//-------INTERRUPT SERVICE ROUTINE FOR TIMER1 COMPARE MATCH-------//
ISR(TIMER1_COMPA_vect) {
  frequency = count - oldCount; // How many pulses in 1 sec
  oldCount = count; // store "current" count (old)
  PORTB ^= (1 << 5); // visual debugging of ISR
}

//-------MAIN PROGRAM-------//
int main(void) {
//-------REGISTER CONFIGURATIONS-------//
  DDRB |= (1 << 5); // set PB5 as output (LED)
  DDRD &= ~(1 << 2); // set PD2 as input
  PORTD &= ~(1 << 2); // no pullup on PD2

  usartInit(); // Initialize USART

//-------INT0 CONFIGURATIONS-------// 
  EICRA |= (1 << ISC01) | (1 << ISC00); // INT0  Triggers on rising edge
  EIMSK |= (1 << INT0);                 // Enable INT0 interrupt

//-------TIMER1 CONFIGURATIONS-------//
  TCCR1B |= (1 << WGM12);               // Timer1 CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10);  // Prescaler 64
  OCR1A = 15624;                        // FCPU/PRESCALER = 15625-1 counts (250k/15625) = 1s
  TIMSK1 |= (1 << OCIE1A);              // Enable Timer1 compare match interrupt

  sei(); // Enable global interrupts

  char str[20]; // string buffer
//-------INFINITE LOOP-------//
  while (1) {
    sprintf(str, "Frequency: %u Hz \n", frequency);
    usartSendString(str);
    _delay_ms(250);
  }
  return 0; // never gets here
}
