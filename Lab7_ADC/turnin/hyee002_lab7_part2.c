/*	Author: Harrison Yee
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/1qUUKjgbkGvAq1m0AOjBZTzer_nAgoFRj/view?usp=sharing
 *
 *      The MAX value which I determined by the the largest value
 *	outputted by the LED lights when a light source is shined on it. The value turned out to be
 * 	896 in decimal.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// 	  in Free Running Mode, a new conversion will trigger whenever
	// 	  the previous conversion completes.
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
    DDRD = 0xFF;
    PORTD = 0x00;
    /* Insert your solution below */
    unsigned short x = 0x0000;
    ADC_init();
    while (1) {
	x = ADC;
	PORTB = (char)x;
	PORTD = (char)(x >> 8);
    }
    return 1;
}
