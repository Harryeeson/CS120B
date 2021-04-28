/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/1IGOoQmPlaHJoqH08ePdpw15xKb9egu2n/view?usp=sharing
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

unsigned char i = 0x00;
unsigned char pinArr[3] = {0x01, 0x02, 0x04};
enum States {Start, State_Wait, State_Iterate} State;
void Tick() {
	switch(State) {
		case Start:
			State = State_Wait;
			break;

		case State_Wait:
			State = State_Iterate;
			break;

		case State_Iterate:
			State = State_Iterate;
			break;

	}
	switch(State) {
		case State_Wait:
			break;

		case State_Iterate:
			PORTB = pinArr[i];
			i++;
			if(i == 0x03) {
				i = 0x00;
			}
			break;

		default:
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
    DDRB = 0xFF;
    PORTB = 0x00;
    TimerSet(1000);
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
