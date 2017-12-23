#ifndef SNAKE_H
#define SNAKE_H

#include "segment.h"

class Snake {
    Segment* head;
    Segment* tail;
    float delay;
    float x_limit;
    float y_limit;
    unsigned int length;
    unsigned int score;
    void AddSegment()

    public:
        Snake();
        float GetDelay();
        unsigned int GetLength();
        unsigned int GetScore();
        unsigned int EatPallet();
        unsigned int Move();
        unsigned int** GetSnakePosition();
};
#endif