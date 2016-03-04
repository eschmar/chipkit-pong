#include <pic32mx.h>
#include "types.h"
#include "helpers.h"
#include "assets.h"
#include "display.h"
#include "sound.h"
#include "freqmap.h"

#define GAME_SPEED          100
#define GAME_WIN_SCORE      3
#define MAX_X               128
#define MAX_Y               32
#define PADDLE_HEIGHT       8
#define CONTROLLER_SPEED    40

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

int tuneCount = 1;
int tuneScale = 0;
/*
 *      Plays a sequence of notes
 */
void playTune(int tune[], int tempo) {
    int length = tune[0];
    
    tone(tune[tuneCount]);
    
    if (tune[tuneCount] == 0) {
        tuneScale = tempo - 1;
    }
    
    tuneScale++;
    if (tuneScale == tempo) {
    tuneScale = 0;
        tuneCount++;
        if (tuneCount == length) {
            tuneCount = 1;
        }
    }
}

/*
 *    Initialise
 */
int main(void) {
    spi_init();
    display_wakeup();

    drawLogo();
    init_game();
    enableTimer3PWM();

    // setup hardware
    enableButtons();
    enableTimer2(31250, 0x1B, 0x111, 1);
    enableTimer3(31250, 0x1B, 0x111, 1);
    setupPotentiometers();

    enableMultiVectorMode();
    enable_interrupt();

    for(;;) ;
    return 0;
}

int counter = GAME_SPEED;

/**
 *  Linear mapping from [0,1023] to valid paddle position
 */
int translateToScreen(int val) {
    return val > 0 ? ((MAX_Y - PADDLE_HEIGHT) * val) / 1024 : 0;
}

void updatePaddles() {
    int ADCValueP1, ADCValueP2;

    // start sampling and wait to complete
    IFSCLR(1) = 0x0002;
    AD1CON1SET = 0x0004;
    while (!IFS(1) & 0x0002);
    
    // check which buffer to read from
    if (AD1CON2 & 0x0080) {
        ADCValueP1 = ADC1BUF0;
        ADCValueP2 = ADC1BUF1;
    } else {
        ADCValueP1 = ADC1BUF8;
        ADCValueP2 = ADC1BUF9;
    }

    p1.y = translateToScreen(ADCValueP1);
    p2.y = translateToScreen(ADCValueP2);
}

// TODO: Tetris tune assets to move
int tetris[] = {E5, E5, B4, C5, D5, D5, C5, B4, 
                A4, A4, PP, A4, C5, E5, E5, D5, C5, 
                B4, B4, B4, C5, D5, D5, E5, E5,
                C5, C5, A4, A4, PP, A4, B4, C5,
                D5, D5, D5, F5, A5, A5, G5, F5,
                E5, E5, E5, C5, E5, E5, D5, C5,
                B4, B4, PP, B4, C5, D5, D5, E5, E5,
                C5, C5, A4, A4, PP, A4, A4, PP, PP, PP};
int tetrisCount = 0;
int tetrisScale = 0;

int counter = GAME_SPEED;
/**
 * ISR Interrupt handler for timer 2
 */
void timer2_interrupt_handler(void) {
    IFSCLR(0) = 0x100;
    counter--;

    if (counter != 0) { return; }
    counter = GAME_SPEED;
    updatePaddles();

    switch (gameState) {
        case STATE_PONG:
            advance();
            draw(p1, p2, ball);

            playTune(tetris, 3);

            // game end?
            if (p1.score >= GAME_WIN_SCORE || p2.score >= GAME_WIN_SCORE) {
                gameState = STATE_END;
                tuneCount = 0;
                mute();
                drawEnding(p1, p2);
            }
            break;
        case STATE_START:
            playTune(starWars, 3);
            if (isButtonPressed(4)) {
                init_game();
                gameState = STATE_PONG;
                tuneCount = 0;
                mute();
                draw(p1, p2, ball);
            }
            break;
        case STATE_END:
            if (isButtonPressed(4)) {
                gameState = STATE_START;
                tuneCount = 0;
                mute();
                drawLogo();
            }
            break;
    }
}

/**
 * ISR Interrupt handler for timer 3
 */
void timer3_interrupt_handler(void) {
    // IFSCLR(0) = 0x1000;
}

/**
 * ISR general interrupt handler
 */
void core_interrupt_handler(void) {}
