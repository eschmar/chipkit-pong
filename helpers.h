#ifndef HELPERS_H_   /* Include guard */
#define HELPERS_H_

int getButtonInput();
void enableMultiVectorMode();
void enableTimer2(int period, int priority, int prescaling, int interrupts);
void enableTimer3(int period, int priority, int prescaling, int interrupts);
void enableButtons();
int isButtonPressed(int i);
void setupPotentiometers();
// int translateToScreen(int val);
// void updatePaddles(Paddle p1, Paddle p2);

#endif // HELPERS_H_