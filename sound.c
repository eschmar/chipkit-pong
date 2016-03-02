#include "sound.h"
#include "freqmap.h"

/*
 * Timer1 registers
 */
#define T1CON 		PIC32_R (0x0600)
#define T1CONSET 	PIC32_R (0x0608)
#define TMR1  		PIC32_R (0x0610)
#define PR1   		PIC32_R (0x0620)

/*
 * Timer3 registers
 */
#define T3CON 		PIC32_R (0x0A00)
#define T3CONSET 	PIC32_R (0x0A08)
#define TMR3  		PIC32_R (0x0A10)
#define PR3   		PIC32_R (0x0A20)

#include <pic32mx.h>

/*
 * Enable Timer3 in PWM mode
 */
void enableTimer3PWM(int period, int prescaling, int compareRegs) {
    T3CON = 0x0;            		// stop timer
    TMR3 = 0;               		// clear timer
    PR3 = period;           		// set period
    T3CONSET = prescaling << 4; 	// set prescaling

    OC1CON = 0x0000;
    OC1R = compareRegs;
	OC1RS = compareRegs;
	OC1CON |= 0x000E;				// PWMmodeonOCx; Fault pin disable; Timer3 is the clock source for this Output Compare module
    
    T3CONSET = 0x8000;				// Timer3 module is enabled
    OC1CON |= 0x8000;				// Output compare peripheral is enabled
}

/*
 * Stop Timer3 PWM
 */
void mute() {
	T3CON = 0x0;
	OC1CON = 0x0;
}

/*
 * Timer 1 set interrupt flag every 1 millisecond
 */
void enableTimer1(void) {
    T1CON = 0x0;            		// stop timer
    TMR1 = 0;               		// clear timer
    PR1 = 1250;      				// set period to 1 millisecond
    T1CONSET = 0x0020; 				// set prescaling 64

    T1CONSET = 0x8000;				// Timer1 module is enabled
}

/*
 * Stop Timer1
 */
void disableTimer1(void) {
	T1CON = 0x0;
}

int tempo = 100;

/*
 * Generate a pause of specified length (* tempo)
 */
void pause(int duration) {
	mute();
	int duration = duration * tempo;
	int counter = 0;
	enableTimer1();
	while (counter == duration) {
		if (IFS(0) & 0x0010) {
			counter++;
			IFSCLR(0) = 0x0010;
		}
	}
	disableTimer1();
}

/*
 * Generate sound with specific note and duration (* tempo)
 * The buzzer must be connected to PIN 3
 */
void tone(int note, int duration) {
	int duration = duration * tempo;
	int counter = 0;
	int frequency = 625000 / note;
	int freqVol =  frequency / 2;		// Modify volume?

	enableTimer3PWM(frequency, 6, freqVol);
	enableTimer1();
	while (counter == duration) {
		if (IFS(0) & 0x0010) {
			counter++;
			IFSCLR(0) = 0x0010;
		}
	}
	disableTimer1();
	mute();
}