/*	Author: Harrison Yee
 *  	Partner(s) Name: None
 *	Lab Section: 21
 *	Assignment: Lab 2  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0x00;
        PORTB = 0xFF;
	DDRC = 0x00;
        PORTC = 0xFF;
	DDRD = 0xFF; 
	PORTD = 0x00; 
    /* Insert your solution below */
    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char tmpC = 0x00;    
    unsigned char tmpD = 0x00;				// 8 bits
    unsigned short totalWeight = 0x0000;		// 16 bits
    while (1) {
	/* Read input */
	tmpA = PINA & 0xFF;
	tmpB = PINB & 0xFF;
	tmpC = PINC & 0xFF;

	/* Calculate total weight */
	totalWeight = tmpA + tmpB + tmpC;
	
	tmpD = (totalWeight >> 2) & 0x00FC;	// 6 leftmost digits are approximate total weight
						// 2 right most digits are for flags

	if(totalWeight > 0x008C) {	// total weight > 140 kg
		tmpD = tmpD | 0x01;
	}
	
	/* Avoid negatives in subtraction */
	if(tmpA > tmpC) {
		if((tmpA - tmpC) > 0x50) {
			tmpD = tmpD | 0x02;
		}
	}
	else { 
		if((tmpC - tmpA) > 0x50) {
			tmpD = tmpD | 0x02;
		}
	}

	PORTD = tmpD;

    }
    return 1;
}
