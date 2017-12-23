#include "snake.h"
#include <stdio.h>

Snake::Snake(int headX, int headY, int limit) {
    length = 3;
    score = 0;
    x_limit = limit;
    y_limit = limit;

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

void Snake::AddSegment() {
    length++;
    Segment* s = new Segment(length);
    tail->SetNext(s);
    s->SetPrev(tail);
    tail = s;
}

void Snake::Delete() {
    Segment* s = head;
    while(s != NULL) {
        Segment* n = s->GetNext();
        s->Delete();
        delete s;
        s = n;
    }
}

void Snake::SetDirection(Direction d) {
    head->SetDirection(d);
}

unsigned int Snake::GetLength() {
    return length;
}

unsigned int Snake::GetScore() {
    return score;
}

unsigned int Snake::EatPallet() {
    score++;
    AddSegment();
    return score;
}

unsigned int Snake::Move() {
    Segment* s = head;
    while(s != NULL) {
        s->Move();
        if( s->GetX() >= x_limit ) {
            return s->GetOrder();
        } else if( s->GetX() < 0 ) {
            return s->GetOrder();
        }

        if( s->GetY() >= x_limit ) {
            return s->GetOrder();
        } else if( s->GetY() < 0 ) {
            return s->GetOrder();
        }

        s = s->GetNext();
    }
    return -1;
}

unsigned int** Snake::GetSnakePosition() {
    unsigned int** positions = 0;
    positions = new unsigned int*[length];
    Segment* s = head;
    for(int i = 0; i < length && s != NULL; i++) {
        positions[i] = new unsigned int[2];
        positions[i][0] = s->GetY();
        positions[i][1] = s->GetX();
        s = s->GetNext();
    }

    return positions;
}