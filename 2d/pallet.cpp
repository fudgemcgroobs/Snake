#include "pallet.h"
#include <time.h>
#include <stdlib.h>

Pallet::Pallet() {
    x = 0;
    y = 0;
}

Pallet::Pallet(int n_x, int n_y) {
    x = n_x;
    y = n_y;
}

bool Pallet::Reposition(int n_x, int n_y) {
    if(x != n_x && y != n_y) {
        x = n_x;
        y = n_y;
        return true;
    } else {
        return false;
    }
    return false;
}

int Pallet::GetX() {
    return x;
}

int Pallet::GetY() {
    return y;
}