#include "snake.h"

Snake::Snake(int headX, int headY, int x_l, int y_l) {
    delay = 2000.0f;
    length = 3;
    score = 0;

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

float Snake::GetDelay() {
    return delay;
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

void Snake::AddSegment() {
    Segment* s = new Segment();
    tail->SetNext(s);
    s->SetPrev(tail);
    tail = s;
    length++;
}

unsigned int Snake::Move() {
    Segment* s = head;
    while(s != NULL) {
        s->Move();
        if( s->GetX() > x_limit ) {
            return s->GetOrder();
        } else if( s->GetX() < 0 ) {
            return s->GetOrder();
        }

        if( s->GetY() > x_limit ) {
            return s->GetOrder();
        } else if( s->GetY() < 0 ) {
            return s->GetOrder();
        }

        s = s->GetNext();
    }
}

unsigned int** GetSnakePosition() {
    unsigned int** positions = 0;
    positions = new unsigned int*[length];
    Segment* s = head;
    for(int i = 0; i < length && s != NULL; i++) {
        positions[i] = new unsigned int[2];
        positions[i][0] = s->GetX();
        positions[i][1] = s->GetY();
        s = s->GetNext();
    }

    return positions;
}

void Snake::Delete() {
    Segment* s = head;
    while(s != NULL) {
        Segment* n = s->next;
        s->Delete();
        delete s;
        s = n;
    }
}