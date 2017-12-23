#ifndef SEGMENT_H
#define SEGMENT_H

enum Direction {UP, RIGHT, DOWN, LEFT};

class Segment {
    Segment* prev;
    Segment* next;
    Direction dir;
    bool head;
    int x;
    int y;
    int p_x;
    int p_y;
    unsigned int order;
    
    public:
        Segment();
        Segment(int o);
        Segment(int o, bool h);
        void SetPosition(int n_x, int n_y, Direction d);
        void SetPrevious(Segment* p);
        void SetNext(Segment* n);
        void Move();    // Move in direction end reset x/y
        void Delete();
        unsigned int GetX();
        unsigned int GetY();
        unsigned int GetOrder();
};
#endif