/**
 * Implementation of the grid class.
 * The coordinates of each cell represent the X/Y coordinates
 *  at the upper-left corner of each cell.
 */
#include "grid.h"
#include <stdio.h>

Grid::Grid() {}

/**
 * Initialises the Grid object, based on the area that it will occupy
 *  (a_s x a_s), the required number of cells, as well as the upper left
 *  corner of the grid (and of cell [0,0]).
 */
Grid::Grid(float a_s, unsigned int g_s, float s_p,
             float edgeX, float edgeY) {
    // s_p is screen padding
    // a_s is arena size
    grid_size = g_s;
    cell_size = (a_s - s_p) / g_s;
    cells = new Cell**[g_s];
    for(int i = 0; i < g_s; i++) {
        cells[i] = new Cell*[g_s];
    }
    GenerateCells(edgeX, edgeY);
}

/**
 * Generates cells starting from the upper-left corner of the grid.
 * Cells are generated from Left to Right, Top to Bottom (as one would read).
 *  __________________________
 *  |  0 |  1 |  2 |  3 |  4 |
 *  |____|____|____|____|____|
 *  |  5 |  6 |... |    |    |
 *  |____|____|____|____|____|
 */
 void Grid::GenerateCells(float firstX, float firstY) {
    float x;
    float y = firstY;
    for(int i = 0; i < grid_size; i++) {
        x = firstX;
        for(int j = 0; j < grid_size; j++) {
            cells[i][j] = new Cell(x, y);
            // Increase the X coordinate by the cell width
            x += cell_size; 
        }
        // Decrease the Y coordinate by the cell width (moving down)
        y -= cell_size;
    }
}

float Grid::GetCellSize() {
    return cell_size;
}

/**
 * Releases memory allocatoed when cells were generated
 */
void Grid::Delete() {
    for(int i = 0; i < grid_size; i++) {
        for(int j = 0; j < grid_size; j++) {
            cells[i][j]->Delete();
            delete cells[i][j];
        }
        delete cells[i];
    }
    delete cells;
}

unsigned int Grid::GetGridSize() {
    return grid_size;
}
 /**
  * Returns a pointer to the cell object stored at the [i,j]
  *  coordinated in the grid.
  */
Cell* Grid::GetCellAt(int i, int j) {
    if(i >= (int) grid_size) {
        i = grid_size - 1;
    } else if(i < 0) {
        i = 0;
    }

    if(j >= (int) grid_size) {
        j = grid_size - 1;
    } else if(j < 0) {
        j = 0;
    }
    return cells[i][j];
}