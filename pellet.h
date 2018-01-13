/**
 * Class to store the grid coordinates of a pellet on a bidimensional grid
 */
#ifndef PELLET_H
#define PELLET_H

class Pellet {
    int x;
    int y;

    public:
        Pellet();
        Pellet(int n_x, int n_y);
        void Delete();
        bool Reposition(int n_x, int n_y);
        int GetX();
        int GetY();
};
#endif