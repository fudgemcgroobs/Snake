#ifndef SEGMENT_H
#define SEGMENT_H

enum Direction {UP, RIGHT, DOWN, LEFT};

class Segment {
    Segment* prev;
    Segment* next;
    Direction dir;
    Direction p_dir;
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
        void SetPrev(Segment* p);
        void SetNext(Segment* n);
        void SetHead(bool h);
        void SetDirection(Direction d);
        void Move();    // Move in direction end reset x/y
        void Delete();
        unsigned int GetX();
        unsigned int GetY();
        unsigned int GetPX();
        unsigned int GetPY();
        unsigned int GetOrder();
        Direction GetDirection();
        Direction GetPDirection();
        Segment* GetNext();
        Segment* GetPrev();
};
#endif