/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 4  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum SM1_States {SM1_SMStart, SM1_Wait, SM1_Pound, SM1_WaitPound, SM1_Y, SM1_WaitY } SM1_State;
void Tick() {
    switch(SM1_State) {
	case SM1_SMStart:
		SM1_State = SM1_Wait;
		break;

	case SM1_Wait:
		if(PINA == 0x04) {
			SM1_State = SM1_Pound;
		}
		else {
			SM1_State = SM1_Wait;
		}
		break;

	case SM1_Pound:
		if(PINA == 0x04) {
			SM1_State = SM1_Pound;
		}
		else if(PINA == 0x00) {
			SM1_State = SM1_WaitPound;
		}
		else {
			SM1_State = SM1_Wait;
		}
		break;

	case SM1_WaitPound:
		if(PINA == 0x02) {
			SM1_State = SM1_Y;
		}
		else if(PINA == 0x00) {
			SM1_State = SM1_WaitPound;
		}
		else {
			SM1_State = SM1_Wait;
		}
		break;

	case SM1_Y:
		if(PINA == 0x02) {
			SM1_State = SM1_Y;
		}
		else if(PINA == 0x00) {
			SM1_State = SM1_WaitY;
		}
		else {
			SM1_State = SM1_Wait;
		}
		break;
	
	case SM1_WaitY:
		if(PINA == 0x00) {
			SM1_State = SM1_WaitY;
		}
		else {
			SM1_State = SM1_Wait;
		}
		break;
			
    }
    switch(SM1_State) {
	case SM1_Wait:
		PORTB = 0x00;
		break;

	case SM1_Pound:
		PORTB = 0x00;
		break;

	case SM1_WaitPound:
		PORTB = 0x00;
		break;

	case SM1_Y:
		PORTB = 0x01;
		break;

	case SM1_WaitY:
		PORTB = 0x01;
		break;

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
    while (1) {
	Tick();	
    }
    return 1;
}
