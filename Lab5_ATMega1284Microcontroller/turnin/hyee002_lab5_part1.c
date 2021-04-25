/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 5  Exercise 1
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
    DDRA = 0x00; PORTA = 0xFF;		// inputs
    DDRC = 0xFF; PORTC = 0x00;		// outputs
    unsigned char led = 0x00;
    unsigned char button0 = 0x00;
    unsigned char button1 = 0x00;
    unsigned char button2 = 0x00;
    unsigned char button3 = 0x00;
    /* Insert your solution below */
    while (1) {
	led = 0x00;
    	button0 = ~PINA & 0x01;		// button connected to A0
	button1 = ~PINA & 0x02;		// button connected to A1
	button2 = ~PINA & 0x04;		// button connected to A2
	button3 = ~PINA & 0x08;		// button connected to A3
	
	// level 13 - 15
	if((button3 && button2 && button1 && button0) || (button3 && button2 && button1) || (button3 && button2 && button0)) {
		led = led | 0x3F;
	}

	// level 10 - 12
	else if((button3 && button2) || (button3 && button1 && button0) || (button3 && button1)) {
		led = led | 0x3E;
	}

	// level 7 - 9
	else if((button3 && button0) || (button3) || (button2 && button1 && button0)) {
		led = led | 0x3C;
	}

	// level 5 - 6
	else if((button2 && button1) || (button2 && button0)) {
		led = led | 0x38;
	}

	// level 3 - 4
	else if((button2) || (button1 && button0)) {
		led = led | 0x70;
	}

	// level 1 - 2
	else if((button1) || (button0)) {
		led = led | 0x60;
	}

	// level 0
	else {
		led = led | 0x40;
	}
	
	PORTC = led;
    }
    return 1;
}
