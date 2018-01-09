#include "snake.h"
#include <stdio.h>

Snake::Snake(int headX, int headY, int n_limit, bool n_loop) {
    length = 3;
    score = 0;
    limit = n_limit;
    loop = n_loop;

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
    s->SetPosition(tail->GetX(), tail->GetY(), tail->GetDirection());
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

void Snake::SetLoop(bool n_loop) {
    loop = n_loop;
}

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

bool Snake::Bite() {
    Segment* s;
    if(head != NULL) {
        s = head->GetNext();
    } else {
        return true;
    }
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

unsigned int Snake::Move() {
    Segment* s = head;
    while(s != NULL) {
        s->Move();
        if( s->GetX() >= limit ) {
            if(loop) {
                s->SetX(0);
            } else return s->GetOrder();
        } else if( s->GetX() < 0 ) {
            if(loop) {
                s->SetX(limit - 1);
            } else return s->GetOrder();
        }

        if( s->GetY() >= limit ) {
            if(loop) {
                s->SetY(0);
            } else return s->GetOrder();
        } else if( s->GetY() < 0 ) {
            if(loop) {
                s->SetY(limit - 1);
            } else return s->GetOrder();
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
        positions[i] = new unsigned int[3];
        positions[i][0] = s->GetY();
        positions[i][1] = s->GetX();
        positions[i][2] = s->GetDirection();
        s = s->GetNext();
    }
    return positions;
}
