#ifndef HELPERS_H_   /* Include guard */
#define HELPERS_H_

int getButtonInput();
void enableMultiVectorMode();
void enableTimer2(int period, int priority, int prescaling, int interrupts);
void enableTimer3(int period, int priority, int prescaling, int interrupts);
void enableButtons();

#endif // HELPERS_H_