#include "pellet.h"
#include <time.h>
#include <stdlib.h>

Pellet::Pellet() {
    x = 0;
    y = 0;
}

Pellet::Pellet(int n_x, int n_y) {
    x = n_x;
    y = n_y;
}

bool Pellet::Reposition(int n_x, int n_y) {
    if(x != n_x && y != n_y) {
        x = n_x;
        y = n_y;
        return true;
    } else {
        return false;
    }
    return false;
}

int Pellet::GetX() {
    return x;
}

int Pellet::GetY() {
    return y;
}