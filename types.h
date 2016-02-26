#ifndef TYPES_H_   /* Include guard */
#define TYPES_H_

typedef struct Paddle {
    short x, y, speed;
} Paddle;

typedef struct Ball {
    short x, y, speedX, speedY;
} Ball;

#endif // TYPES_H_