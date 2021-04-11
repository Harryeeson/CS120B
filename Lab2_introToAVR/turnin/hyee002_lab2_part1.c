/*	Author: Harrison Yee
 *  Partner(s) Name: None
 *	Lab Section: 21
 *	Assignment: Lab 2  Exercise 1
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
	DDRB = 0xFF; 
	PORTB = 0x00;
    /* Insert your solution below */
    unsigned char tmpB = 0x00;	
    unsigned char tmpA = 0x00;
    while (1) {
	tmpA = PINA & 0x0F;        
        if (tmpA == 0x00 || tmpA == 0x02 || tmpA == 0x03) {
            tmpB = tmpB & 0x00;	// output 0
        } 
	else{  // PA1 == 0 and PA0 == 1
            tmpB = tmpB | 0x01;	// output 1
        }    
	PORTB = tmpB;    

    }
    return 1;
}
