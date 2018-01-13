/**
 * Linked list class to store inormation related to a moving snake
 *  made of segments, and to interact with the snake data.
 */
#ifndef SNAKE_H
#define SNAKE_H

#include "segment.h"

class Snake {
    Segment* head;  // The head of the snake
    Segment* tail;  // The tail of the snake
    bool loop;      // If the snake should loop at the edges of the arena
    int limit;      // The order of the arena, in terms of number of cells
    unsigned int length;    // The length of the snake
    unsigned int score;     // The score of the snake (number of eaten pellets)
    // Perform necessary actions and add another snake segments
    void AddSegment();

    public:
        Snake();
        Snake(int headX, int headY, int n_limit, bool n_loop);
        void Delete();
        void SetLoop(bool n_loop);
        bool SetDirection(Direction d);
        bool Bite();
        unsigned int GetLength();
        unsigned int GetScore();
        unsigned int EatPellet();
        unsigned int Move();
        unsigned int** GetSnakePosition();
        unsigned int* GetHeadPosition();
        Direction GetDirection();
};
#endif