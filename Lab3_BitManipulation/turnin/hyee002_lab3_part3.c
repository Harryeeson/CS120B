/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 3  Exercise 2
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
	unsigned char tmpC = 0x00;
	unsigned char temp = 0x00;
        unsigned char tempTwo = 0x00;
    while (1) {
	tmpA = PINA;
	tmpC = 0x00;
	temp = tmpA << 4;
	tempTwo = tmpA >> 4;
	
	if(temp == 0x00) {
		tmpC = tmpC | 0x40;
	}

	// level 1 or 2 PC5 light
	else if(temp == 0x10 || temp == 0x20) {
                tmpC = tmpC | 0x60;
        }

	// level 3 or 4 PC5 and PC4
	else if(temp == 0x30 || temp == 0x40) {
                tmpC = tmpC | 0x70;
        }

	// level 5 or 6 PC5-PC3
	else if(temp == 0x50 || temp == 0x60) {
                tmpC = tmpC | 0x38;
        }

	// level 7-9 PC5-PC2
	else if(temp == 0x70 || temp == 0x80 || temp == 0x90) {
                tmpC = tmpC | 0x3C;
        }

	// level 10-12 PC5-PC1
	else if(temp == 0xA0 || temp == 0xB0 || temp == 0xC0) {
                tmpC = tmpC | 0x3E;
        }

	// level 13-15 PC5-PC0
	else if(temp == 0xD0 || temp == 0xE0 ||  temp == 0xF0) {
                tmpC = tmpC | 0x3F;
        }

	// Fasten seatbelt PC7 is 1 if PA4 and PA5 and !PA6
	if(tempTwo == 0x03) {
		tmpC = tmpC | 0x80;
	}

	PORTC = tmpC;
    }
    return 1;
}
