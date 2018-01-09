#ifndef PELLET_H
#define PELLET_H

class Pellet {
    int x;
    int y;

    public:
        Pellet();
        Pellet(int n_x, int n_y);
        bool Reposition(int n_x, int n_y);
        int GetX();
        int GetY();
};
#endif