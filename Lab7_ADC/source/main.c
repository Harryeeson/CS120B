/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 7  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link:
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// 	  in Free Running Mode, a new conversion will trigger whenever
	// 	  the previous conversion completes.
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
    DDRD = 0xFF;
    PORTD = 0x00;
    /* Insert your solution below */
    unsigned short x = 0x0000;
    unsigned short max = 0x03C7;	// 967
    ADC_init();
    while (1) {
	x = ADC;
	if(x == max) {
		PORTB = 0xFF;
	}
	else if (x >= (max * 7 / 8)){
		PORTB = 0x7F;
	}
	else if (x >= (max * 6 / 8)){
		PORTB = 0x3F;
	}
	else if (x >= (max * 5 / 8)){
		PORTB = 0x1F;
	}
	else if (x >= (max * 4 / 8)){
		PORTB = 0x0F;
	}
	else if (x >= (max * 3 / 8)){
		PORTB = 0x07;
	}
	else if (x >= (max * 2 / 8)){
		PORTB = 0x03;
	}
	else {
		PORTB = 0x01;
	}
    }
    return 1;
}
