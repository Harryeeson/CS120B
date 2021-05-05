/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 8  Exercise 3
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
double noteArr[6] = {523.25, 293.66, 440.00, 392.00, 523.25, 440.00};
enum States {Start, State_Wait, Play/*, Play_Wait*/} State;
void Tick() {
	switch(State) {
		case Start:
			State = State_Wait;
			break;

		case State_Wait:
			if(~PINA & 0x01) {	
				State = Play;
			}
			else {
				State = State_Wait;
			}
			break;

		case Play:
			if(i < 0x06) {
				State = Play;
			}
			else {
				State = State_Wait;
			}
			break;

		/*case Play_Wait:
			if((~PINA & 0x01) == 0x00) {		// button is not held
				State = State_Wait;
			}
			else {
				State = Play_Wait;
			}
			break;*/
			
	}
	switch(State) {
		case State_Wait:
			set_PWM(0);
			i = 0x00;
			break;
		
		case Play:
			if(i < 0x06) {
				set_PWM(noteArr[i]);
				i++;
			}
			break;
	
		/*case Play_Wait:
			set_PWM(0);
			break;*/

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
