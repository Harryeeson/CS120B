/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 11  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 	Demo Link:
 */
#include <avr/io.h>
#include "timer.h"
#include "bit.h"
#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

// pattern: (0 - Off), (1 - On)
// row: (0 - On), (1 - Off)

static task task1, task2, task3, task4;
task *tasks[] = {&task1, &task2, &task3, &task4};
//starting point for each array
unsigned char colArr[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
unsigned char rowArr[3] = {0x03, 0x07, 0x0F}; 
unsigned char colWin[4] = {0x20, 0x10, 0x08, 0x04};
unsigned char rowWin[4] = {0x15, 0x15, 0xEE, 0x00};
unsigned char colLose[4] = {0x20, 0x10, 0x04, 0x02};
unsigned char rowLose[4] = {0x15, 0x15, 0x00, 0xEE};
unsigned char stackedCol[8];
unsigned char stackedRow[8];
unsigned char pattern = 0x01;
unsigned char row = 0x03;
unsigned char j = 0x00;
unsigned char k = 0x00;
unsigned char x = 0x00;
unsigned char FirstFlag = 0x00;
unsigned char temp = 0x00;
unsigned char cntr = 0x00;

enum LED_States {LED_Start, LED_Display, LED_Reverse, LED_Off};
int LED_Tick(int state) {
	switch (state) {
		case LED_Start:
			state = LED_Display;
			break;

		case LED_Display:
			if(row == 0xF8 || row == 0xFC || row == 0xFE) {
				state = LED_Reverse;
			}
			else {
				state = LED_Display;
			}
			break;

		case LED_Reverse:
			if(row == 0xE3 || row == 0xE7 || row == 0xEF) {
				state = LED_Display;
			}
			else {
				state = LED_Reverse;
			}
			break;

		case LED_Off:
			state = LED_Off;
			break;
		
		default:
			state = LED_Start;
			break;
	}
	
	switch (state) {
		case LED_Display:
			if(FirstFlag == 0x00) {			
				row = (row >> 1) | 0xF0;
			}
			else {
				FirstFlag = 0x00;
			}
			break;

		case LED_Reverse:
			row = (row << 1) | 0x01;
			break;
	
		case LED_Off:
			break;

		default:
			break;
	}
	PORTC = pattern;
	PORTD = row;
	return state;
}

enum Stacked_States {Stacked_Display, Stacked_Off, Stacked_Win, Stacked_Lose};
int Stacked_Tick(int state) {
	switch(state) {
		case Stacked_Display:
			state = Stacked_Display;
			break;

		case Stacked_Off:
			state = Stacked_Off;
			break;

		case Stacked_Lose:
			state = Stacked_Lose;
			break;

		case Stacked_Win:
			state = Stacked_Win;
			break;

		default:
			state = Stacked_Display;
			break;
	}

	switch(state) {
		case Stacked_Display:
			if(j > 0x00) {
				if(x < j) {
					PORTC = stackedCol[x];
					PORTD = stackedRow[x];
					x++;
				}
				else {
					x = 0x00;
					PORTC = stackedCol[x];
					PORTD = stackedRow[x];
				}
			}
			break;

		case Stacked_Off:
			break;

		case Stacked_Win:
			if(cntr < 0x04) {
				PORTC = colWin[cntr];
				PORTD = rowWin[cntr];
				cntr++;
			}
			else {
				cntr = 0x00;
				PORTC = colWin[cntr];
				PORTD = rowWin[cntr];
			}
			break;

		case Stacked_Lose:
			if(cntr < 0x04) {
				PORTC = colLose[cntr];
				PORTD = rowLose[cntr];
				cntr++;
			}
			else {
				cntr = 0x00;
				PORTC = colLose[cntr];
				PORTD = rowLose[cntr];
			}
			break;

		default:
			break;
	}

	return state;
}

enum Current_States {Current_Display, Current_Off};
int Current_Tick(int state) {
	switch(state) {
		case Current_Display:
			state = Current_Display;
			break;

		case Current_Off:
			state = Current_Off;
			break;

		default:
			state = Current_Display;
			break;
	}

	switch(state) {
		case Current_Display:
			PORTC = pattern;
			PORTD = row;
			break;

		case Current_Off:
			break;

		default:
			break;
	}

	return state;
}

enum Button_States {Button_Start, Button_Wait, Button_Next, Button_NextWait, Button_On, Button_Off, Button_OffWait, Button_Reset, Button_Win, Button_Lose, Button_LoseWait, Button_WinWait};
int Button_Tick(int state) {
	switch(state) {
		case Button_Start:
			state = Button_Wait;
			break;

		case Button_Wait:
			if((~PINA & 0x03) == 0x01) {				// Place block
				state = Button_Next;
			}
			else if((~PINA & 0x03) == 0x02) {			// Turn on/off game
				state = Button_Off;
			}
			else if((~PINA & 0x03) == 0x03) {			// Reset game
				state = Button_Reset;
			}
			else {
				state = Button_Wait;
			}
			break;

		case Button_Next:
			state = Button_NextWait;
			break;

		case Button_NextWait:
			if((~PINA & 0x03) == 0x01) {
				state = Button_NextWait;
			}
			else {
				state = Button_Wait;
			}
			break;

		case Button_Off:
			if((~PINA & 0x03) == 0x02) {			
				state = Button_OffWait;
			}
			else {
				state = Button_Off;
			}
			break;

		case Button_OffWait:
			if((~PINA & 0x03) == 0x02) {			
				state = Button_On;
			}
			else {
				state = Button_OffWait;
			}
			break;

		case Button_On:
			state = Button_Wait;
			break;
			
		case Button_Reset:
			state = Button_Wait;
			break;

		case Button_Lose:
			if((~PINA & 0x03) == 0x01) {
				state = Button_Lose;
			}
			else {
				state = Button_LoseWait;
			}
			break;

		case Button_LoseWait:
			if((~PINA & 0x03) == 0x01) {
				state = Button_Reset;
			}
			else {
				state = Button_LoseWait;
			}
			break;

		case Button_Win:
			if((~PINA & 0x03) == 0x01) {
				state = Button_Win;
			}
			else {
				state = Button_WinWait;
			}
			break;

		case Button_WinWait:
			if((~PINA & 0x03) == 0x01) {
				state = Button_Reset;
			}
			else {
				state = Button_WinWait;
			}
			break;

		default:
			state = Button_Start;
			break;
	}
	
	switch(state) {
		case Button_Wait:
			break;

		case Button_Next:
			stackedCol[j] = pattern;
			if(j == 0) {			
				stackedRow[j] =  row;
			}
			else {							
				temp = stackedRow[j - 1] | row;
				if((temp & 0x1F) == 0x1F) {			// If block is not ontop of stack
					state = Button_Lose;
					tasks[0]->state = LED_Off;
					tasks[2]->state = Stacked_Lose;
					tasks[3]->state = Current_Off;
					break;
				}
				else {
					stackedRow[j] = temp;
					if(j == 0x07) {
						state = Button_Win;
						tasks[0]->state = LED_Off;
						tasks[2]->state = Stacked_Win;
						tasks[3]->state = Current_Off;
						break;
					}
				}
			}
			if(tasks[0]->state == LED_Reverse) {
				tasks[0]->state = LED_Start;
			}
			j++;
                        if(j == 2 || j == 5) {					// Change block size
				k++;
			}
                        pattern = colArr[j];
			row = rowArr[k];
			FirstFlag = 0x01;
			break;

		case Button_NextWait:
			break;
		
		case Button_Off:
			tasks[0]->state = LED_Off;
			tasks[2]->state = Stacked_Off;
			pattern = 0x00;
			break;

		case Button_On:
			tasks[0]->state = LED_Start;
			tasks[2]->state = Stacked_Display;
			pattern = colArr[j];
			row = rowArr[k];
			FirstFlag = 0x01;
			break;

		case Button_Reset:
			j = 0x00;
			k = 0x00;
			cntr = 0x00;
			tasks[0]->state = LED_Start;
			tasks[2]->state = Stacked_Display;
			tasks[3]->state = Current_Display;
			pattern = colArr[0];
			row = rowArr[0];
			PORTC = pattern;
			PORTD = row;
			break;

		case Button_Lose:
			break;

		case Button_Win:
			break;

		default:
			break;
	}

	return state;
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

    const char start = -1;
    task1.state = start;
    task1.period = 450;
    task1.elapsedTime = task1.period;
    task1.TickFct = &LED_Tick;

    task2.state = start;
    task2.period = 50;
    task2.elapsedTime = task2.period;
    task2.TickFct = &Button_Tick;

    task3.state = start;
    task3.period = 1;
    task3.elapsedTime = task3.period;
    task3.TickFct = &Stacked_Tick;

    task4.state = start;
    task4.period = 10;
    task4.elapsedTime = task4.period;
    task4.TickFct = &Current_Tick;

	
    unsigned short i;
    unsigned long GCD = tasks[0]->period;
    for(i = 1; i < numTasks; i++) {
	GCD = findGCD(GCD, tasks[i]->period);
    }
    TimerSet(GCD);
    TimerOn();

    while (1) {
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
