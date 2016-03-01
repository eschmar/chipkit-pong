#include <pic32mx.h>
#include "helpers.h"
#include "assets.h"
#include "display.h"
#include "types.h"

#define GAME_SPEED          100
#define GAME_WIN_SCORE    3
#define MAX_X               128
#define MAX_Y               32
#define PADDLE_HEIGHT       8

#define STATE_START     0
#define STATE_PONG      1
#define STATE_END       2

int gameState = STATE_START;
Paddle p1, p2;
Ball ball;

/*
 *    One frame of the game
 */
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
        // score for p1?
        if (ball.y < p1.y || ball.y > p1.y + PADDLE_HEIGHT - 1) { p2.score += 1; }

        ball.x = 0;
        ball.speedX *= (-1);
    }else if (ball.x >= MAX_X - 1) {
        // score for p1?
        if (ball.y < p2.y || ball.y > p2.y + PADDLE_HEIGHT - 1) { p1.score += 1; }

        ball.x = MAX_X - 1;
        ball.speedX *= (-1);
    }
}

/*
 *    Set up game configuration
 */
void init_game() {
    p1.x = 0;
    p1.y = 5;
    p1.score = 0;

    p2.x = 127;
    p2.y = 5;
    p2.score = 0;

    ball.x = 60;
    ball.y = 15;
    ball.speedX = 2;
    ball.speedY = 1;    
}

/*
 *    Initialise
 */
int main(void) {
    spi_init();
    display_wakeup();

    drawLogo();
    init_game();

    // setup hardware
    enableButtons();
    enableTimer2(31250, 0x1B, 0x111, 1);
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
    // clear t2 interrupt flag
    IFSCLR(0) = 0x100;
    int btnVal = getButtonInput();
    counter--;

    if (counter != 0) { return; }
    counter = GAME_SPEED;

    switch (gameState) {
        case STATE_PONG:
            advance();
            draw(p1, p2, ball);

            // game end?
            if (p1.score >= GAME_WIN_SCORE || p2.score >= GAME_WIN_SCORE) {
                gameState = STATE_END;
                drawEnding(p1, p2);
            }
            break;
        case STATE_START:
            if (btnVal & 0x4) {
                init_game();
                gameState = STATE_PONG;
                draw(p1, p2, ball);
            }
            break;
        case STATE_END:
            if (btnVal & 0x4) {
                gameState = STATE_START;
                drawLogo();
            }
            // TODO: show who won.
            break;
    }

    // controllers    
    if (btnVal == 2) {
        // up
        if (p1.y > 0) { p1.y -= 1; p2.y -= 1; } 

    }else if (btnVal == 4) {
        // down
        if (p1.y < MAX_Y - PADDLE_HEIGHT - 1) { p1.y += 1; p2.y += 1; } 
    } 
}

