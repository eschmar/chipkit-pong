#include "sound.h"
#include "freqmap.h"

/*
 * Timer1 registers
 */
#define T1CON       PIC32_R (0x0600)
#define T1CONSET    PIC32_R (0x0608)
#define TMR1        PIC32_R (0x0610)
#define PR1         PIC32_R (0x0620)

/*
 * Timer3 registers
 */
#define T3CON       PIC32_R (0x0A00)
#define T3CONSET    PIC32_R (0x0A08)
#define TMR3        PIC32_R (0x0A10)
#define PR3         PIC32_R (0x0A20)

#include <pic32mx.h>

//int volumeSet = 2;   // MAX 2 - MIN 800

/*
 * Enable Timer3 in PWM mode
 */
void enableTimer3PWM(void) {
    T3CON = 0x0;               // stop timer
    TMR3 = 0;                  // clear timer
    PR3 = 0;                   // set period to 0 (mute)
    T3CONSET = 6 << 4;         // set prescaling to 64

    OC1CON = 0x0000;
    OC1R = 0;
    OC1RS = 0;
    OC1CON |= 0x000E;               // PWMmodeonOCx; Fault pin disable; Timer3 is the clock source for this Output Compare module
    
    T3CONSET = 0x8000;              // Timer3 module is enabled
    OC1CON |= 0x8000;               // Output compare peripheral is enabled
}

/*
 * Stop Timer3 PWM
 */
void mute(void) {
    PR3 = 0;
    OC1R = 0;
    OC1RS = 0;
}

/*
 * Generate sound with specific note
 * The buzzer must be connected to PIN 3
 */
void tone(int note, int volumeParam) {
    if (note == 0) {
        PR3 = 0;
        OC1R = 0;
        OC1RS = 0;
    } else {
        int frequency = (int) 625000 / note;
        int volume =  (int) frequency / volumeParam;

        PR3 = frequency;
        OC1R = volume;
        OC1RS = volume;
    }
}