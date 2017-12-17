#ifndef ARENA_H
#define ARENA_H

#include "cell.h"

class Arena {
    unsigned float cell_size;       // The size of the side of a cell
    unsigned float arena_size;      // The overall size of the Snake arena
    unsigned int grid_size;         // The number of cells on one arena row
    Cell *cells;    // Array containing the coordinates of cells

    // Generates cell coordinates based on coordinates of first cell
    void GenerateCells(float firstX, float firstY);  
    

    public:
        Arena();
        Arena(unsigned float a_s, unsigned int g_s, unsigned float s_p,
                float edgeX, float edgeY);
        unsigned int GetGridSize();
        unsigned float GetArenaSize();
        Cell* GetCellAt(int i, int j);
};
#endif