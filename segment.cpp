/**
 * Implementation of the Segment class.
 */
#include "segment.h"
#include <stdio.h>

/**
 * Initialises the segment with the passed order, assuming alone in a list.
 * Caller must also make calls to SetPrev/SetNext to integrate the segment
 *  in a snake (list)
 */
Segment::Segment(int o) {
    prev = NULL;
    next = NULL;
    dir = RIGHT;
    p_dir = RIGHT;
    head = false;
    x = 0;
    y = 0;
    p_x = -1;
    p_y = 0;
    order = o;
}
/**
 * Initialises the segment with the passed order, as head, assuming alone in
 *  a list.
 * Caller must also make calls to SetPrev/SetNext to integrate the segment
 *  in a snake (list)
 */
Segment::Segment(int o, bool h) {
    prev = NULL;
    next = NULL;
    dir = RIGHT;
    p_dir = RIGHT;
    head = h;
    x = 0;
    y = 0;
    p_x = -1;
    p_y = 0;
    order = o;
}

void Segment::SetPosition(int n_x, int n_y, Direction d) {
    x = n_x;
    y = n_y;
    dir = d;
}

void Segment::SetPrev(Segment* p) {
    prev = p;
}

void Segment::SetNext(Segment* n) {
    next = n;
}

void Segment::SetHead(bool h) {
    head = h;
}

void Segment::SetDirection(Direction d) {
    if(head) {
        dir = d;
    }
}

void Segment::SetX(int n_x) {
    x = n_x;
}

void Segment:: SetY(int n_y) {
    y = n_y;
}

/**
 * Moves the segment in the approproate direction.
 * The only segment moved according to set direction is the head.
 * All other segments are positioned in place of segments ahead of them.
 */
void Segment::Move() {
    p_x = x;
    p_y = y;
    p_dir = dir;
    if(head) {
        // The head moves independently
        switch(dir) {
            case UP:
                y -= 1;
                break;
            case RIGHT:
                x += 1;
                break;
            case DOWN:
                y += 1;
                break;
            case LEFT:
                x -= 1;
                break;
        }
    } else {
        // Other segments move in place segments ahead of them
        if(prev != NULL) {
            x = prev->GetPX();
            y = prev->GetPY();
            dir = prev->GetPDirection();
        } else {
            return ;
        }
    }
}

void Segment::Delete() {}

int Segment::GetX() {
    return x;
}
int Segment::GetY() {
    return y;
}
int Segment::GetPX() {
    return p_x;
}
int Segment::GetPY() {
    return p_y;
}

unsigned int Segment::GetOrder() {
    return order;
}
Direction Segment::GetDirection() {
    return dir;
}
Direction Segment::GetPDirection() {
    return p_dir;
}
Segment* Segment::GetNext() {
    return next;
}
Segment* Segment::GetPrev() {
    return prev;
}