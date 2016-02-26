#include <pic32mx.h>
#include "helpers.h"
#include "font.h"
#include "display.h"
#include "types.h"

#define GAME_SPEED 	100
#define MAX_X		128
#define MAX_Y		32

Paddle p1, p2;
Ball ball;

void advance() {
	ball.x = (ball.x + ball.speedX);
	ball.y = (ball.y + ball.speedY);

	// vertical collision detection
	if (ball.y <= 0) {
		ball.y = 0;
		ball.speedY *= (-1);
	}else if (ball.y >= MAX_Y - 1) {
		ball.y = MAX_Y - 1;
		ball.speedY *= (-1);
	}

	// horizontal collision detection
	if (ball.x <= 0) {
		ball.x = 0;
		ball.speedX *= (-1);
	}else if (ball.x >= MAX_X - 1) {
		ball.x = MAX_X - 1;
		ball.speedX *= (-1);
	}
}

void init_game() {
	p1.x = 0;
	p1.y = 5;

	p2.x = 127;
	p2.y = 12;

	ball.x = 60;
	ball.y = 15;
	ball.speedX = 2;
	ball.speedY = 1;	
}

int main(void) {
	spi_init();
	display_wakeup();

	init_game();
	draw(p1, p2, ball);

	// setup timers
    enableTimer2(31250, 0x1B, 0x111, 1);
    
    // enable interrupts
    enable_interrupt();
	
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

int counter = GAME_SPEED;

/**
 * ISR general interrupt handler
 */
void core_interrupt_handler(void) {
	// clear interrupt flag
    IFSCLR(0) = 0x100;

    counter--;
    if (counter == 0)
    {
    	advance();
	    draw(p1, p2, ball);
	    counter = GAME_SPEED;
    }
}
