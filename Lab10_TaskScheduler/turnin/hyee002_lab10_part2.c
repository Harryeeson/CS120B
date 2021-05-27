/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 10  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * 	Demo Link: https://drive.google.com/file/d/1WR1rgsINdii0LEKy7P5kdKmdWYhwl3TC/view?usp=sharing
 *
 */
#include <avr/io.h>
#include "timer.h"
#include "bit.h"
#include "scheduler.h"
#include "keypad.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char x = 0x00;
unsigned char j = 0x00;
unsigned char code[6] = {'#', '1', '2', '3', '4', '5'};
enum SM1_States {SM1_Start, SM1_Wait, SM1_First, SM1_Check, SM1_CheckWait, SM1_Unlock};
int SM1_Tick(int state) {
	x = GetKeypadKey();
	switch(state) {
		case SM1_Start:
			state = SM1_Wait;
			break;

		case SM1_Wait:
			if(x == code[j]) {
				state = SM1_First;
			}
			else {
				state = SM1_Wait;
			}
			break;

		case SM1_First:
			state = SM1_Check;
			break;

		case SM1_Check:
			if(x == code[j - 1]) {
				state = SM1_Check;
			}
			else if(x == '\0') {
				state = SM1_CheckWait;
			}
			else {
				state = SM1_Wait;
			}
			break;

		case SM1_CheckWait:
			if(j == 0x06) {
				state = SM1_Unlock;
			}
			else if(x == '\0') {
				state = SM1_CheckWait;
			}
			else if(x == code[j]) {
				state = SM1_First;
			}
			else {
				state = SM1_Wait;
			}
			break;

		case SM1_Unlock:
			if(x == '\0') {
				state = SM1_Unlock;
			}
			else {
				state = SM1_Wait;
			}
			break;

		default:
			state = SM1_Start;
			break;

	}
	
	switch(state) {
		case SM1_Wait:
			j = 0x00;
			PORTB = 0x00;
			break;

		case SM1_First:
			j++;
			break;

		case SM1_Check:
			break;

		case SM1_CheckWait:
			if(j == 0x06) {
				PORTB = 0x01;
			}
			break;

		case SM1_Unlock:
			break;
		
		default:
			break;
	}
	return state;
}

enum LockStates {SM2_Start, SM2_Wait, SM2_Lock};
int Lock_Tick(int state) {
	switch(state) {
		case SM2_Start:
			state = SM2_Wait;
			break;

		case SM2_Wait:
			if((~PINB & 0x80) == 0x80) {
				state = SM2_Lock;
			}
			else {
				state = SM2_Wait;
			}
			break;

		case SM2_Lock:
			state = SM2_Wait;
			break;

		default:
			state = SM2_Start;
			break;
	}
	switch(state) {
		case SM2_Wait:
			break;

		case SM2_Lock:
			PORTB = 0x00;
			break;

		default:
			break;
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0x0F; PORTB = 0xF0;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    static task task1, task2;
    task *tasks[] = {&task1, &task2};
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

    const char start = -1;
    task1.state = start;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &SM1_Tick;

    task2.state = start;
    task2.period = 25;
    task2.elapsedTime = task2.period;
    task2.TickFct = &Lock_Tick;

    unsigned short i;
    unsigned long GCD = tasks[0]->period;
    for(i = 1; i < numTasks; i++) {
	GCD = findGCD(GCD, tasks[i]->period);
    }
    TimerSet(GCD);
    TimerOn();
    
    while(1) {
       	for(i = 0; i < numTasks; i++) {
		if( tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += GCD;
	}
	while(!TimerFlag);
	TimerFlag = 0;
			
    }
    return 1;
}
