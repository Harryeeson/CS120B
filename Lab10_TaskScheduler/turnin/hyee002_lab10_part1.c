/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 10  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * 	Demo Link:
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

enum SM1_States {SM1_Start, SM1_Wait};
int SM1_Tick(int state) {
	x = GetKeypadKey();
	switch(state) {
		case SM1_Start:
			state = SM1_Wait;
			break;

		case SM1_Wait:
			state = SM1_Wait;
			break;
		
		default:
			state = SM1_Start;
			break;

	}
	
	switch(state) {
		case SM1_Wait:
			switch(x) {
				case '\0': PORTB = 0x1F; break;
				case '1': PORTB = 0x81; break;
				case '2': PORTB = 0x82; break;
				case '3': PORTB = 0x83; break;
				case '4': PORTB = 0x84; break;
				case '5': PORTB = 0x85; break;
				case '6': PORTB = 0x86; break;
				case '7': PORTB = 0x87; break;
				case '8': PORTB = 0x88; break;
				case '9': PORTB = 0x89; break;
				case 'A': PORTB = 0x8A; break;
				case 'B': PORTB = 0x8B; break;
				case 'C': PORTB = 0x8C; break;
				case 'D': PORTB = 0x8D; break;
				case '*': PORTB = 0x8E; break;
				case '0': PORTB = 0x80; break;
				case '#': PORTB = 0x8F; break;
				default: PORTB = 0x1B; break;
			}
			break;
		

		default:
			break;
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

    const char start = -1;
    task1.state = start;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &SM1_Tick;

    TimerSet(50);
    TimerOn();
    
    unsigned short i;
    while(1) {
       	for(i = 0; i < numTasks; i++) {
		if( tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 50;
	}
	while(!TimerFlag);
	TimerFlag = 0;
			
    }
    return 1;
}
