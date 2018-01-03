#ifndef PALLET_H
#define PALLET_H

class Pallet {
    int x;
    int y;

    public:
        Pallet();
        Pallet(int n_x, int n_y);
        bool Reposition(int n_x, int n_y);
        int GetX();
        int GetY();
};
#endif