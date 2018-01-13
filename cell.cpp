/**
 * Simple implementation of the cell class
 */
#include "cell.h"

Cell::Cell() {
    x = 0;
    y = 0;
}

Cell::Cell(float n_x, float n_y) {
    x = n_x;
    y = n_y;
}

float Cell::GetX() {
    return x;
}

float Cell::GetY() {
    return y;
}

void Cell::Delete() {}