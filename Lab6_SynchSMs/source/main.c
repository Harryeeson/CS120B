/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 6  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;  // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1;		// Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0;	// Current internal count of 1ms ticks

unsigned char cntr = 0x00;
enum States {Start, State_First, State_Wait, State_Inc, State_Dec, State_Reset, State_IncWait, State_DecWait} State;
void Tick() {
	switch(State) {
		case Start:
			State = State_First;
			break;
		
		case State_First:
			State = State_Wait;
			break;

		case State_Wait:
			if((~PINA & 0x03) == 0x03) {
				State = State_Reset;	
			}
			else if((~PINA & 0x03) == 0x01) {
				State = State_Inc;
			}
			else if((~PINA & 0x03) == 0x02) {
				State = State_Dec;
			}
			break;
		
		case State_Inc:
			if(cntr == 0x09) {
				State = State_Wait;
			}
			else if((~PINA & 0x03) == 0x01) {
				State = State_IncWait;
			}
			else {
				State = State_Wait;
			}
			break;

		case State_IncWait:
			if(cntr == 0x09) {			// After 1 second
				State = State_Inc;
			}
			else if((~PINA & 0x03) == 0x01) {	// If button is held start 1 second cntr
				State = State_IncWait;
			}
			else {
				State = State_Wait;
			}
			break;

		case State_Dec:
			if(cntr == 0x09) {
				State = State_Wait;
			}
			else if((~PINA & 0x03) == 0x02) {
				State = State_DecWait;
			}
			else {
				State = State_Wait;
			}
			break;
		
		case State_DecWait:
			if(cntr == 0x09) {			// After 1 second
				State = State_Dec;
			}
			else if((~PINA & 0x03) == 0x02) {	// If button is held start 1 second cntr
				State = State_DecWait;
			}
			else {
				State = State_Wait;
			}
			break;

		case State_Reset:
			State = State_Wait;
			break;

	}
	switch(State) {
		case State_First:
			PORTB = 0x07;
			break;

		case State_Wait:
			break;

		case State_Inc:
			cntr = 0x00;
			if(PORTB < 0x09) {
				PORTB = PORTB + 1;
			}
			break;

		case State_IncWait:
			cntr++;
			break;
		
		case State_Dec:
			cntr = 0x00;
			if(PORTB > 0x00) {
				PORTB = PORTB - 1;
			}
			break;
		
		case State_DecWait:
			cntr++;
			break;

		case State_Reset:
			PORTB = 0x00;
			break;

		default:
			PORTB = 0x07;
			break;
	}

}

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;	// bit 3 = 0: CTC mode (clear timer on compare)
			// bit2bit1bit0=011: pre-scaler /64
			// 00001011: 0x0B
			// SO, 8 Mhz clock or 8,000,000 /64 = 125,000 ticks/s
			// Thus, TCNT1 register will count as 125,000 ticks/s
	
	// AVR output compare register OCR1A.
	OCR1A = 125; 	// Timer interrupt will be generated when TCNT1==OCR1A
			// We want a 1ms tick. 0.001 s * 125,000 ticks/s = 125
			// So when TCNT1 register equals 125,
			// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02;	// bit1: OCIE1A -- enables compare match interrupt

	// Initialize avr counter
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	
	// Enable global interrupts
	SREG |= 0x80; 	// 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00;	// bit3bit1bit0 = 000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {	// results in a more efficient compare
		TimerISR();		// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
    TimerSet(100);
    TimerOn();
    /* Insert your solution below */
    while (1) {
	// User code (i.e. synchSM calls)
	Tick();
	while(!TimerFlag);	// Wait 1 sec
	TimerFlag = 0;
	// Note: For the above a better style would use a synchSM with TickSm()
	// This example just illustrates the use of the ISR and flag
    }
    return 1;
}
