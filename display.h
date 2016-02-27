#ifndef DISPLAY_H_   /* Include guard */
#define DISPLAY_H_

void delay(int cyc);
uint8_t spi_send_recv(uint8_t data);
void spi_init();
void display_wakeup();
void lightUpPixel(int x, int y);
void drawPaddle(Paddle p);
void drawBall(Ball b);
void clearGame();
void drawScore(Paddle p1, Paddle p2);
void draw(Paddle p1, Paddle p2, Ball ball);

#endif // DISPLAY_H_