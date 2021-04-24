/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 4  Exercise 5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char seqArr[4] = {0x04, 0x01, 0x02, 0x01};
unsigned char index = 0x00;

enum SM1_States {SM1_SMStart, SM1_Wait, SM1_Check, SM1_Unlock, SM1_WaitUnlock} SM1_State;
void Tick() {
    switch(SM1_State) {
	case SM1_SMStart:
		SM1_State = SM1_Wait;
		break;

	case SM1_Wait:
		if(PINA == seqArr[index]) {
			SM1_State = SM1_Check;
		}
		else {
			SM1_State = SM1_Wait;
		}
		break;
	
	case SM1_Check:
		if(index == 0x03) {
			SM1_State = SM1_Unlock;
		}
		else if(PINA == 0x00) {
			index++;
			SM1_State = SM1_Wait;
		}
		else {
			SM1_State = SM1_Check;
		}
	        break;

	case SM1_Unlock:
		if(PINA == 0x00) {
			SM1_State = SM1_WaitUnlock;
		}
		else {
			SM1_State = SM1_Unlock;
		}
		break;

	case SM1_WaitUnlock:
		if(PINA == 0x00) {
			SM1_State = SM1_WaitUnlock;
		}
		else {
			index = 0x00;
			SM1_State = SM1_Wait;
		}
		break;
			
    }
    switch(SM1_State) {

	case SM1_Wait:
		PORTB = 0x00;
		break;
	
	case SM1_Check:
		PORTB = 0x00;
		break;

	case SM1_Unlock:
		PORTB = 0x01;
		break;

	case SM1_WaitUnlock:
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
