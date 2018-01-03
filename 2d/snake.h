#ifndef SNAKE_H
#define SNAKE_H

#include "segment.h"

class Snake {
    Segment* head;
    Segment* tail;
    bool loop;
    int limit;
    unsigned int length;
    unsigned int score;
    void AddSegment();

    public:
        Snake();
        Snake(int headX, int headY, int n_limit, bool n_loop);
        void Delete();
        bool SetDirection(Direction d);
        bool Bite();
        unsigned int GetLength();
        unsigned int GetScore();
        unsigned int EatPallet();
        unsigned int Move();
        unsigned int** GetSnakePosition();
};
#endif