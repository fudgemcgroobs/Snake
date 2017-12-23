#include "grid.h"
#include <stdio.h>

Grid::Grid() {}
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

unsigned int Grid::GetGridSize() {
    return grid_size;
}

Cell* Grid::GetCellAt(int i, int j) {
    if(i >= grid_size) {
        i = grid_size - 1;
    } else if(i < 0) {
        i = 0;
    }

    if(j >= grid_size) {
        j = grid_size - 1;
    } else if(j < 0) {
        j = 0;
    }
    return cells[i][j];
}

float Grid::GetCellSize() {
    return cell_size;
}

void Grid::GenerateCells(float firstX, float firstY) {
    float x;
    float y = firstY;
    for(int i = 0; i < grid_size; i++) {
        x = firstX;
        for(int j = 0; j < grid_size; j++) {
            cells[i][j] = new Cell(x, y);
            x += cell_size; 
        }
        y -= cell_size;
    }
}

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