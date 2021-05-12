/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 9  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/1z3avUDsobUCKbMMYVqFZ6Tok93APQJG3/view?usp=sharing
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
unsigned char thirdPattern[7] = {0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02};
unsigned char i = 0x00;
unsigned char j = 0x00;
unsigned char k = 0x00;
unsigned short counterFirst = 0x0000;
unsigned short counterSecond = 0x0000;
unsigned char counterThird = 0x00;
unsigned char output = 0x00;

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

enum Third_States{Third_Start, Third_Wait, Third_On, Third_Inc, Third_IncWait, Third_Dec, Third_DecWait} Third_State;
void SpeakerSM() {
	switch(Third_State) { 
		case Third_Start:
			Third_State = Third_Wait;
			break;
	
		case Third_Wait:
			if((~PINA & 0x07) == 0x04) {
				Third_State = Third_On;
			}
			else if((~PINA & 0x07) == 0x01) {		// Inc
				Third_State = Third_Inc;
			}
			else if((~PINA & 0x07) == 0x02) {		// Dec
				Third_State = Third_Dec;
			}
			else {
				Third_State = Third_Wait;
			}
			break;
	
		case Third_On:
			if((~PINA & 0x07) == 0x04) {
				Third_State = Third_On;
			}
			else {
				Third_State = Third_Wait;
			}
			break;

		case Third_Inc:
			Third_State = Third_IncWait;
			break;
		
		case Third_IncWait:
			if((~PINA & 0x07) == 0x01) {
				Third_State = Third_IncWait;
			}
			else {
				Third_State = Third_Wait;
			}
			break;

		case Third_Dec:
			Third_State = Third_DecWait;
			break;
		
		case Third_DecWait:
			if((~PINA & 0x07) == 0x02) {
				Third_State = Third_DecWait;
			}
			else {
				Third_State = Third_Wait;
			}
			break;
	}
	
	switch(Third_State) {
		case Third_Wait:
			counterThird = 0x00;
			output = 0x00;
			break;
		
		case Third_On:
			if(counterThird <= thirdPattern[k]) {
				output = 0x10;
				counterThird++;
			}
			else if(counterThird <= (2 * thirdPattern[k])) {
				output = 0x00;
				counterThird++;
			}
			else {						// Greater than 4ms
				counterThird = 0x00;
			}
			break;
		
		case Third_Inc:
			if(k < 6) {
				k++;
			}
			break;

		case Third_IncWait:
			break;

		case Third_Dec:
			if(k > 0) {
				k--;
			}
			break;

		case Third_DecWait:
			break;

		default:
			break;	
	}
}

void CombineLEDsSM() {
	PORTB = (threeLEDs | blinkingLED) | output;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
    /* Insert your solution below */
    TimerSet(1);
    TimerOn();
    while (1) {
	ThreeLEDsSM();	
	BlinkingLEDSM();
	SpeakerSM();
	CombineLEDsSM();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
