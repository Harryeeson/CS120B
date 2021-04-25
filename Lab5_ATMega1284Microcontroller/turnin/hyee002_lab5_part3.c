/*	Author: Harrison Yee
 *	Lab Section: 21
 *	Assignment: Lab 4  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/1fcS4w8yHi0tPcvjFueqLIxz1uZ5solY_/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char i = 0x00;
unsigned char flag = 0x00;
unsigned char arrowArr[16] = {0x0C, 0x12, 0x21, 0x12, 0x0C, 0x00, 0x0C, 0x1E, 0x3F, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20};
enum SM1_States {SM1_SMStart, SM1_First, SM1_Wait, SM1_OutWait, SM1_Out, SM1_InWait, SM1_In, SM1_ReverseWait, SM1_Reverse} SM1_State;
void Tick() {
    switch(SM1_State) {
	case SM1_SMStart:
		SM1_State = SM1_First;
		break;
	
	case SM1_First:
		SM1_State = SM1_Wait;
		break;

	case SM1_Wait:
		// if reached end of array iterate through array the other way
		if((i < 0x00) || (i >= 0x10)) {
			SM1_State = SM1_ReverseWait;
		}
		else if(~PINA & 0x01) {
			if((flag & 0x01) == 0x00) {			// inc index
				SM1_State = SM1_OutWait;
			}
			else {						// dec index
				SM1_State = SM1_InWait;
			}
		}
		else {
			SM1_State = SM1_Wait;
		}
		break;

	case SM1_OutWait:
		SM1_State = SM1_Out;
		break;

	case SM1_Out:
		if(~PINA & 0x01) {
			SM1_State = SM1_Out;
		}
		else {
			SM1_State = SM1_Wait;
		}
		break;

	case SM1_InWait:
		SM1_State = SM1_In;
		break;

	case SM1_In:
		if(~PINA & 0x01) {
			SM1_State = SM1_In;
		}
		else {
			SM1_State = SM1_Wait;
		}
		break;

	case SM1_ReverseWait:
		SM1_State = SM1_Reverse;
		break;

	case SM1_Reverse:
		if(~PINA & 0x01) {
			SM1_State = SM1_Reverse;
		}
		else {
			SM1_State = SM1_Wait;
		}
		break;
	
    }
    switch(SM1_State) {
	case SM1_First:
		PORTB = 0x00;
		break;

	case SM1_Wait:
		break;

	case SM1_OutWait:
		PORTB = arrowArr[i];
		i++;
		break;
	
	case SM1_Out:
		break;
	
	case SM1_InWait:
		PORTB = arrowArr[i];
		i--;
		break;
	
	case SM1_In:
		break;

	case SM1_ReverseWait:
		// set flag to indicate either dec or inc index
		// reset i to make smoother transitions
		if((flag & 0x01) == 0x00) {
			flag = 0x01;
			i = 0x0E;
		}
		else {
			flag = 0x00;
			i = 0x01;
		}
		break;

	case SM1_Reverse:
		break;

	default:
		PORTB = 0x00;
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
