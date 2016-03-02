#include "helpers.h"

/*
 * Timer3 registers
 */
#define T3CON 		PIC32_R (0x0A00)
#define T3CONSET 	PIC32_R (0x0A08)
#define TMR3  		PIC32_R (0x0A10)
#define PR3   		PIC32_R (0x0A20)

#include <pic32mx.h>

int getButtonInput(void){
    return (PORTD & (0x7 << 5)) >> 5;
}

void enableMultiVectorMode() {
    INTCONSET = 0x1000;
}

void enableTimer2(int period, int priority, int prescaling, int interrupts) {
    T2CON = 0x0;            // stop timer
    TMR2 = 0;               // clear timer
    PR2 = period;           // set period
    IPCSET(2) = priority;   // set priority
    
    IFSCLR(0) = 0x100;      // reset timer interrupt status flag
    
    // enable timer interrupts
    if (interrupts == 1) { IECSET(0) = 0x100; }
    
    // set prescaling
    T2CONSET = prescaling << 4;
    
    // enable
    T2CONSET = 0x8000;
}

void enableTimer3(int period, int priority, int prescaling, int interrupts) {
    T3CON = 0x0;            // stop timer
    TMR3 = 0;               // clear timer
    PR3 = period;           // set period
    IPCSET(3) = priority;   // set priority
    
    IFSCLR(0) = 0x1000;      // reset timer interrupt status flag
    
    // enable timer interrupts
    if (interrupts == 1) { IECSET(0) = 0x1000; }
    
    // set prescaling
    T3CONSET = prescaling << 4;
    
    // enable
    T3CONSET = 0x8000;
}

void enableButtons() {
    TRISD |= (0x3f << 5);
}