/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 8  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * 	Demo Link:
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

// 0.954 hz is lowest frequency possible with this funcion,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
        static double current_frequency;        // Keeps track of the currently set frequency
        // Will only update the registers when the frequency changes, otherwise allows
        // music to play uninterrupted.
	if(frequency != current_frequency) {
		if(!frequency) { TCCR3B &= 0x08; }      // stops timer/counter
		else { TCCR3B |= 0x03; }                // resumes/continues timer/counter

		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if(frequency < 0.954) { OCR3A = 0xFFFF;}

		// prevents OCR3A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if(frequency > 31250) { OCR3A = 0x0000; }

		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0;                              // resets counter
		current_frequency = frequency;          // Updates the current frequency
	}
}


void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

unsigned char i = 0x00;
unsigned char flag = 0x00;					// initially off
double noteArr[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
enum States {Start, State_Wait, Note_Inc, Note_IncWait, Note_Dec, Note_DecWait, Toggle_Sound, Toggle_Wait} State;
void Tick() {
	switch(State) {
		case Start:
			State = State_Wait;
			break;

		case State_Wait:
			if((~PINA & 0x07) == 0x01) {		// Turn on/off
				State = Toggle_Sound;
			}
			else if((~PINA & 0x07) == 0x02) {	// Dec
				State = Note_Dec;
			}
			else if((~PINA & 0x07) == 0x04) {	// Inc
				State = Note_Inc;
			}
			else {
				State = State_Wait;
			}
			break;

		case Note_Inc:
			State = Note_IncWait;
			break;

		case Note_IncWait:
			if((~PINA & 0x07) == 0x04) {
				State = Note_IncWait;
			}
			else {
				State = State_Wait;
			}
			break;

		case Note_Dec:
			State = Note_DecWait;
			break;

		case Note_DecWait:
			if((~PINA & 0x07) == 0x02) {
				State = Note_DecWait;
			}
			else {
				State = State_Wait;
			}
			break;

		case Toggle_Sound:
			State = Toggle_Wait;
			break;

		case Toggle_Wait:
			if((~PINA & 0x07) == 0x01) {
				State = Toggle_Wait;
			}
			else {
				State = State_Wait;
			}
			break;
	}
	switch(State) {
		case State_Wait:
			break;

		case Note_Inc:
			if(i < 0x07) {				// always increment
				i++;
			}
			if(flag == 0x01) {			// if on then output sound
				set_PWM(noteArr[i]);
			}
			break;
		
		case Note_IncWait:
			break;

		case Note_Dec:
			if(i > 0) {				// always decrement
				i--;
			}				
			if(flag == 0x01) {			// if on then output sound
				set_PWM(noteArr[i]);
			}
			break;

		case Note_DecWait:
			break;

		case Toggle_Sound:
			if(flag == 0x00) {			// Initially off
				flag = 0x01;
				set_PWM(noteArr[i]);
			}
			else {					// turn off sound
				flag = 0x00;
				set_PWM(0);
			}
			break;
	
		case Toggle_Wait:
			break;

		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
    /* Insert your solution below */
    PWM_on();
    while (1) {
	Tick();	
    }
    return 1;
}
