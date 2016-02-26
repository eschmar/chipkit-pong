#include <pic32mx.h>
#include "helpers.h"
#include "font.h"
#include "display.h"
#include "types.h"

Paddle p1, p2;
Ball ball;

int main(void) {
	spi_init();
	display_wakeup();

	p1.x = 0;
	p1.y = 5;

	p2.x = 127;
	p2.y = 12;

	ball.x = 60;
	ball.y = 15;

	draw(p1, p2, ball);
	
	for(;;) ;
	return 0;
}

/**
 * ISR Interrupt handler for timer 2
 */
void timer2_interrupt_handler(void) {
   
}

/**
 * ISR Interrupt handler for timer 3
 */
void timer3_interrupt_handler(void) {
   
}

/**
 * ISR general interrupt handler
 */
void core_interrupt_handler(void) {
    // clear interrupt flag
    IFSCLR(0) = 0x100;
}

