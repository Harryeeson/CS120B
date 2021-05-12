/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 9  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/10_JcJ1PkAHigZupmchqmTS50HZCQqlJG/view?usp=sharing
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char firstPattern[3] = {0x01, 0x02, 0x04};
unsigned char secondPattern[2] = {0x08, 0x00};
unsigned char i = 0x00;
unsigned char j = 0x00;
unsigned short counterFirst = 0x0000;
unsigned short counterSecond = 0x0000;

enum First_States{First_Start, First_Display, First_Wait} First_State;
void ThreeLEDsSM() {
	switch(First_State) {
		case First_Start:
			First_State = First_Display;
			break;

		case First_Display:
			First_State = First_Wait;
			break;

		case First_Wait:
			if(counterFirst == 0x012C) {			// 300 ms
				counterFirst = 0x0000;
				First_State = First_Display;
			}
			else {
				counterFirst++;
				First_State = First_Wait;
			}
			break;
	}

	switch(First_State) {
		case First_Display:
			if(i == 2) {
				threeLEDs = firstPattern[i];
				i = 0;
			}
			else {
				threeLEDs = firstPattern[i];
				i++;
			}
			break;
		
		default:
			break;
	}
}

enum Second_States{Second_Start, Second_Display, Second_Wait} Second_State;
void BlinkingLEDSM() {
	switch(Second_State) {
		case Second_Start:
			Second_State = Second_Display;
			break;

		case Second_Display:
			Second_State = Second_Wait;
			break;

		case Second_Wait:
			if(counterSecond == 0x03E8) {			// 1000 ms
				counterSecond = 0x0000;
				Second_State = Second_Display;
			}
			else {
				counterSecond++;
				Second_State = Second_Wait;
			}
			break;
	}

	switch(Second_State) {
		case Second_Display:
			if(j == 1) {
				blinkingLED = secondPattern[j];
				j = 0;
			}
			else {
				blinkingLED = secondPattern[j];
				j++;
			}
			break;

		case Second_Wait:
			break;
		
		default:
			break;
	}
}

void CombineLEDsSM() {
	PORTB = threeLEDs | blinkingLED;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF;
    PORTB = 0x00;
    /* Insert your solution below */
    TimerSet(1);
    TimerOn();
    while (1) {
	ThreeLEDsSM();	
	BlinkingLEDSM();
	CombineLEDsSM();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
