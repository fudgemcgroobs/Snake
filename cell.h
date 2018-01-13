/**
 * Class to store the coordinates of a cell in a bidimensional grid
 */
#ifndef CELL_H
#define CELL_H

class Cell {
    float x;
    float y;

    public:
        Cell();
        Cell(float n_x, float n_y);
        float GetX();
        float GetY();
        void Delete();
};
#endif