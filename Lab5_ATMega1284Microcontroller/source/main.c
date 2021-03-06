/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 4  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char button0 = 0x00;
unsigned char button1 = 0x00;
enum SM1_States {SM1_SMStart, /*SM1_First,*/ SM1_Wait, SM1_Inc, SM1_WaitInc, SM1_Dec, SM1_WaitDec, SM1_Reset, SM1_WaitReset, SM1_WaitTemp} SM1_State;
void Tick() {
    switch(SM1_State) {
	case SM1_SMStart:
		SM1_State = SM1_Wait;
		break;

	/*case SM1_First:
		SM1_State = SM1_Wait;
		break;*/

	case SM1_Wait:
		if(button0 && button1) {
			SM1_State = SM1_Reset;
		}
		else if(button0 && !button1) {
			SM1_State = SM1_Inc;
		}
		else if(!button0 && button1) {
			SM1_State = SM1_Dec;
		}
		break;

	case SM1_WaitTemp:
		if(button0 && button1) {
			SM1_State = SM1_Reset;
		}
		else if(button0 && !button1) {
			SM1_State = SM1_Inc;
		}
		else if(!button0 && button1) {
			SM1_State = SM1_Dec;
		}
		break;

	case SM1_Inc:
		SM1_State = SM1_WaitInc;
		break;

	case SM1_WaitInc:
		if(button0 && button1) {
			SM1_State = SM1_Reset;
		}
		else if(button0 && !button1) {
			SM1_State = SM1_WaitInc;
		}
		else if(!button0 && !button1) {
			SM1_State = SM1_WaitTemp;
		}
		break;

	case SM1_Dec:
		SM1_State = SM1_WaitDec;
		break;

	case SM1_WaitDec:
		if(button0 && button1) {
			SM1_State = SM1_Reset;
		}
		else if(!button0 && button1) {
			SM1_State = SM1_WaitDec;
		}
		else if(!button0 && !button1) {
			SM1_State = SM1_WaitTemp;
		}

		break;

	case SM1_Reset:
		SM1_State = SM1_WaitReset;
		break;

	case SM1_WaitReset:
		if(button0 && button1) {
			SM1_State = SM1_Reset;
		}
		else if(!button0 && !button1) {
			SM1_State = SM1_WaitTemp;
		}
		break; 
			
    }
    switch(SM1_State) {
	/*case SM1_First:
		PORTC = 0x07;
		break;*/

	case SM1_Wait:
		PORTC = 0x07;
		break;

	case SM1_WaitTemp:
		break;

	case SM1_Inc:
		if(PORTC < 0x09) {
			PORTC = PORTC + 1;
		}
		break;

	case SM1_WaitInc:
		break;

	case SM1_Dec:
		if(PORTC > 0x00) {
			PORTC = PORTC - 1;
		}
		break;

	case SM1_WaitDec:
		break;

	case SM1_Reset:
		PORTC = 0x00;
		break;

	case SM1_WaitReset:
		break;

	default:
		PORTC = 0x07;
		break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRC = 0xFF;
    PORTC = 0x00;
    /* Insert your solution below */
    while (1) {
	button0 = (~PINA & 0x01);		// button connected to A0
    	button1 = (~PINA & 0x02);		// button connected to A1	
	Tick();	
    }
    return 1;
}
