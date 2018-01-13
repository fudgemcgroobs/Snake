/**
 * Class which stores a bidimensional array of cells representing a grid,
 *  along with other information related to the grid.
 */
#ifndef GRID_H
#define GRID_H

#include "cell.h"

class Grid {
    int cell_size;          // The size of the side of a cell
    unsigned int grid_size; // The number of cells on one grid row
    Cell*** cells;      // Array containing the coordinates of cells

    // Generates cell coordinates based on coordinates of first cell
    void GenerateCells(float firstX, float firstY);  
    

    public:
        Grid();
        Grid(float a_s, unsigned int g_s, float s_p,
                float edgeX, float edgeY);
        float GetCellSize();
        void Delete();
        unsigned int GetGridSize();
        Cell* GetCellAt(int i, int j);        
};
#endif