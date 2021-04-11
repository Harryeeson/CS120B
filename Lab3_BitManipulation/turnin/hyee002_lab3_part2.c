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
    while (1) {
	tmpA = PINA;
	tmpC = 0x00;
	
	if(tmpA == 0x00) {
		tmpC = tmpC | 0x40;
	}

	// level 1 or 2 PC5 light
	else if(tmpA == 0x01 || tmpA == 0x02) {
                tmpC = tmpC | 0x60;
        }

	// level 3 or 4 PC5 and PC4
	else if(tmpA == 0x03 || tmpA == 0x04) {
                tmpC = tmpC | 0x70;
        }

	// level 5 or 6 PC5-PC3
	else if(tmpA == 0x05 || tmpA == 0x06) {
                tmpC = tmpC | 0x38;
        }

	// level 7-9 PC5-PC2
	else if(tmpA == 0x07 || tmpA == 0x08 || tmpA == 0x09) {
                tmpC = tmpC | 0x3C;
        }

	// level 10-12 PC5-PC1
	else if(tmpA == 0x0A || tmpA == 0x0B || tmpA == 0x0C) {
                tmpC = tmpC | 0x3E;
        }

	// level 13-15 PC5-PC0
	else if(tmpA == 0x0D || tmpA == 0x0E || tmpA == 0x0F) {
                tmpC = tmpC | 0x3F;
        }


	PORTC = tmpC;
    }
    return 1;
}
