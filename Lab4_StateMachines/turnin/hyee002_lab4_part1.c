/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum SM1_States {SM1_SMStart, SM1_WaitS0, SM1_S0, SM1_WaitS1, SM1_S1} SM1_State;
void Tick() {
    switch(SM1_State) {
	case SM1_SMStart:
		SM1_State = SM1_WaitS0;
		break;
	case SM1_WaitS0:
		if(PINA == 0x01) {
			SM1_State = SM1_S0;
		}
		else {
			SM1_State = SM1_WaitS0;
		}
		break;
	case SM1_S0:
		if(PINA == 0x01) {
			SM1_State = SM1_S0;
		}
		else {
			SM1_State = SM1_WaitS1;
		}
		break;
	case SM1_WaitS1:
		if(PINA == 0x01) {
			SM1_State = SM1_S1;
		}
		else {
			SM1_State = SM1_WaitS1;
		}
		break;
	case SM1_S1:
		if(PINA == 0x01) {
			SM1_State = SM1_S1;
		}
		else {
			SM1_State = SM1_WaitS0;
		}
		break;
    }
    switch(SM1_State) {
	case SM1_WaitS0:
		PORTB = 0x01;
	        break;	
	case SM1_S0:
		PORTB = 0x02;
		break;
	case SM1_WaitS1:
		PORTB = 0x02;
		break;
	case SM1_S1:
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
