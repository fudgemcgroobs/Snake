/**
 * Implementation of the snake class.
 * The snake is always initialised at the headX/headY location facing right,
 *  with a size of 3, and the body to the left of the head.
 */
#include "snake.h"
#include <stdio.h>

/**
 * Initialises a snake object with the position of the head, the cell limit
 *  to the right and towards the bottom of the arena, as well as a flag
 *  setting whether or not the snake should loop at the sides of the arena.
 */
Snake::Snake(int headX, int headY, int n_limit, bool n_loop) {
    length = 3;
    score = 0;
    limit = n_limit;
    loop = n_loop;

    // Initialise 3 segments and set head/tail
    head = new Segment(1, true);
    Segment* s = new Segment(2);
    tail = new Segment(3);
    head->SetNext(s);
    s->SetNext(tail);
    s->SetPrev(head);
    tail->SetPrev(s);

    head->SetPosition(headX, headY, RIGHT);
    s->SetPosition(headX-1, headY, RIGHT);
    tail->SetPosition(headX-2, headY, RIGHT);
}

/**
 * Adds another segment to the snake, at the tail-end, resetting the
 *  tail to this segment.
 */
void Snake::AddSegment() {
    length++;
    Segment* s = new Segment(length);
    tail->SetNext(s);
    s->SetPrev(tail);
    s->SetPosition(tail->GetX(), tail->GetY(), tail->GetDirection());
    tail = s;
}

/**
 * Release memory allocated when creating snake segments.
 */
void Snake::Delete() {
    Segment* s = head;
    while(s != NULL) {
        Segment* n = s->GetNext();
        s->Delete();
        delete s;
        s = n;
    }
}

/**
 * Sets the loop flag to the passed value.
 */
void Snake::SetLoop(bool n_loop) {
    loop = n_loop;
}

/**
 * Sets the direction of the snake.
 * Does not allow the snake to turn 180 degrees on itself.
 */
bool Snake::SetDirection(Direction d) {
    switch(d) {
        case UP:
            if(head->GetDirection() != DOWN) {
                head->SetDirection(d);
                return true;
            }
            break;
        case RIGHT:
            if(head->GetDirection() != LEFT) {
                head->SetDirection(d);
                return true;
            }
            break;
        case DOWN:
            if(head->GetDirection() != UP) {
                head->SetDirection(d);
                return true;
            }
            break;
        case LEFT:
            if(head->GetDirection() != RIGHT) {
                head->SetDirection(d);
                return true;
            }
            break;
    }
    return false;
}

/**
 * Checks if the snake is about to bite its body.
 * If there is not head, the method returns true.
 */
bool Snake::Bite() {
    // Will store segments temporarily to check if they are in front of the head
    Segment* s;
    if(head != NULL) {
        s = head->GetNext();
    } else {
        return true;
    }
    // Retrieve the coordinates of the cell in front of the head
    int x;
    int y;
    switch(head->GetDirection()) {
        case UP:
            x = head->GetX();
            y = head->GetY() - 1;
            break;
        case RIGHT:
            x = head->GetX() + 1;
            y = head->GetY();
            break;
        case DOWN:
            x = head->GetX();
            y = head->GetY() + 1;
            break;
        case LEFT:
            x = head->GetX() - 1;
            y = head->GetY();
            break;
    }
    // Check if any of the elements are in the cell ahead of the head
    while(s != NULL) {
        if(s->GetX() == x && s->GetY() == y) {
            return true;
        }
        s = s->GetNext();
    }
    return false;
}

unsigned int Snake::GetLength() {
    return length;
}

unsigned int Snake::GetScore() {
    return score;
}

unsigned int Snake::EatPellet() {
    score++;
    AddSegment();
    return score;
}

Direction Snake::GetDirection() {
    return head->GetDirection();
}
/**
 * Moves each segment forward, looping around arena when appropriate.
 * Returns -1 if all segments were moved successfully, or the number of
 *  of the segment which caused the snake to stop advancing (for debugging).
 */
unsigned int Snake::Move() {
    Segment* s = head;
    while(s != NULL) {
        s->Move();  // Move the segment
        if( s->GetX() >= limit ) {
            // Reached Right edge            
            if(loop) {
                s->SetX(0);
            } else return s->GetOrder();
        } else if( s->GetX() < 0 ) {
            // Reached Left edge
            if(loop) {
                s->SetX(limit - 1);
            } else return s->GetOrder();
        }

        if( s->GetY() >= limit ) {
            // Reached Bottom edge
            if(loop) {
                s->SetY(0);
            } else return s->GetOrder();
        } else if( s->GetY() < 0 ) {
            // Reached Top edge
            if(loop) {
                s->SetY(limit - 1);
            } else return s->GetOrder();
        }

        s = s->GetNext();
    }
    return -1;
}

/**
 * Returns a bidimensional array containing, coordinates, direction, and
 *  previous direction of each of the segments.
 * Allocates memory which must be freed by the caller.
 */
unsigned int** Snake::GetSnakePosition() {
    unsigned int** positions = 0;
    positions = new unsigned int*[length];
    Segment* s = head;
    for(int i = 0; i < length && s != NULL; i++) {
        positions[i] = new unsigned int[3];
        positions[i][0] = s->GetY();
        positions[i][1] = s->GetX();
        positions[i][2] = s->GetDirection();
        positions[i][3] = s->GetPDirection();
        s = s->GetNext();
    }
    return positions;
}

/**
 * Returns an array containing the coordinates of the snake head
 */
unsigned int* Snake::GetHeadPosition() {
    unsigned int* position = 0;
    position = new unsigned int[2];
    position[0] = head->GetY();
    position[1] = head->GetX();
    return position;
}
