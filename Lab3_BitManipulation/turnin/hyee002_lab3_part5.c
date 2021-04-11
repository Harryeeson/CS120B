/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 4
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
	DDRD = 0x00;
	PORTD = 0xFF;
	DDRB = 0x0F;		// input on high nibble output on low nibble
	PORTB = 0x00;
    /* Insert your solution below */
	unsigned char tmpD = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char temp = 0x00;
	unsigned short totalWeight = 0x0000;
    while (1) {
	tmpB = 0x00;
	tmpD = PIND;
	temp = PINB >> 4;
	totalWeight = (tmpD << 1) | (temp & 0x01);
	if(totalWeight >= 0x0046){
		tmpB = tmpB | 0x02;
	}
	else if(totalWeight > 0x0005) {
		tmpB = tmpB | 0x04;
	}
	else {
	    tmpB = 0x00;
	}	    
	PORTB = tmpB;
    }
    return 1;
}
