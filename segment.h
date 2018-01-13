/**
 * Class which stores data of a snake segment (practically a specialised)
 *  linked list element).
 * CAUTION: Only one element should have the 'head' flag set to true
 */
#ifndef SEGMENT_H
#define SEGMENT_H

enum Direction { UP, RIGHT, DOWN, LEFT };

class Segment {
    Segment* prev;  // The previous segment in the snake (head has prev = NULL)
    Segment* next;  // The next segment in the snake (tail has next = NULL)
    Direction dir;  // The current direction of the segment
    Direction p_dir;// The previous direction of the segment
    bool head;  // Flag which determines if the segment is the snake head
    int x;  // The grid coordinates of the segment in a 2d grid
    int y;
    int p_x;    // The previous coordinates of the segment
    int p_y;
    unsigned int order; // The position of the segment in the snake (list)
    
    public:
        Segment();
        Segment(int o);
        Segment(int o, bool h);
        void SetPosition(int n_x, int n_y, Direction d);
        void SetPrev(Segment* p);
        void SetNext(Segment* n);
        void SetHead(bool h);
        void SetDirection(Direction d);
        void SetX(int n_x);
        void SetY(int n_y);
        void Move();    // Move in direction end reset x/y
        void Delete();
        int GetX();
        int GetY();
        int GetPX();
        int GetPY();
        unsigned int GetOrder();
        Direction GetDirection();
        Direction GetPDirection();
        Segment* GetNext();
        Segment* GetPrev();
};
#endif