#include "helpers.h"
#include "types.h"

/*
 * Timer3 registers
 */
#define T3CON       PIC32_R (0x0A00)
#define T3CONSET    PIC32_R (0x0A08)
#define TMR3        PIC32_R (0x0A10)
#define PR3         PIC32_R (0x0A20)

#include <pic32mx.h>

/**
 * Setup port D as button input
 */
void enableButtons() {
    TRISD |= (0x3f << 5);
}

/**
 * Returns raw button values at port D
 */
int getButtonInput(void){
    return (PORTD & (0x7 << 5)) >> 5;
}

/**
 * Returns if button was pressed and allows
 * pressing of any length.
 */
int buttons[] = {0, 0, 0, 0, 0};
int isButtonPressed(int i) {
    int btnVal = getButtonInput();
    int shift = (i > 1 && i < 5) ? (-2) + i : 0;
    int pin = 0x1 << shift;
    i--;

    if (buttons[i] == 0 && btnVal & pin) {
        buttons[i] = 1;
        return 1;
    }else if (buttons[i] == 1 && !(btnVal & pin)) {
        buttons[i] = 0;
    }

    return 0;
}

/**
 * Enables multi vector mode which allows
 * multiple distinct interrupt handlers.
 */
void enableMultiVectorMode() {
    INTCONSET = 0x1000;
}

/**
 *  Set up and start timer 2
 */
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

/**
 *  Set up and start timer 3
 */
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

/**
 *  Set up ADC on pins A1 and A2
 */
void setupPotentiometers() {
    // reset
    AD1CON1 = 0x0000;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x0000;

    // ASAM: ADC Sample Auto-Start bit
    AD1CON1SET = 0x4;
    // SSRC: auto convert
    AD1CON1SET = 0xE0;
    // FORM: uint32, 0 - 1024
    AD1CON1SET = 0x400;

    // BUFM: Result buffer mode, split in 2
    AD1CON2SET = 0x2;
    // SMPI: 2 sample/convert sequences between interrupts
    AD1CON2SET = 0x4;
    // CSCNA: Scan inputs
    AD1CON2SET = 0x400;

    // SAMC: 31 TAD
    AD1CON3SET = 0xF00;

    // CSSL: ADC Input Pin Scan Selection bits
    AD1CSSLSET = 0x0110;

    // start
    AD1CON1SET = 0x8000;
}
