/*	Author: Harrison Yee
 *  	Partner(s) Name: None
 *	Lab Section: 21
 *	Assignment: Lab 2  Exercise 3
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
	DDRC = 0xFF; 
	PORTC = 0x00; 
    /* Insert your solution below */	
    unsigned char tmpA = 0x00;
    while (1) {
	tmpA = PINA & 0x0F;	// Read input of 4 pins: 3 to 0
      	if(tmpA == 0x00){ // 4 available
		PORTC = 0x04;
	}

	else if(tmpA == 0x01 || tmpA == 0x02 || tmpA == 0x04 || tmpA == 0x08) {	// 3 available
		PORTC = 0x03;
	}

 	else if(tmpA == 0x03 || tmpA == 0x05 || tmpA == 0x06 || tmpA == 0x09 || tmpA == 0x0A || tmpA == 0x0C) {	// 2 available
		PORTC = 0x02;
	}
	else if(tmpA == 0x0E || tmpA == 0x0D || tmpA == 0x0B || tmpA == 0x07) { // 1 available
		PORTC = 0x01;
	}
	else {	// 0 available
		PORTC = 0x80;
	}
    }
    return 1;
}
