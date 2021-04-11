/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 1
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
	DDRC = 0xFF;
	PORTC = 0x00;
    /* Insert your solution below */
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
    while (1) {
	    tmpA = PINA;
	    tmpB = PINB;
	    tmpC = 0x00;
	    for(int i = 0; i < 8; i++) {
		    if(((tmpA >> i) & 0x01) == 0x01) {
			    tmpC = tmpC + 1;
		    }
		    if(((tmpB >> i) & 0x01) == 0x01) {
			    tmpC = tmpC + 1;
		    }
	    }

	PORTC = tmpC;
    }
    return 1;
}
