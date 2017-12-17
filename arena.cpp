#include "arena.h"

Arena::Arena() {}
Arena::Arena(unsigned float a_s, unsigned int g_s, unsigned float s_p,
             float edgeX, float edgeY) {
    // s_p is screen padding
    arena_size = a_s;
    grid_size = g_s;
    cell_size = (a_s - s_p) / (float) g_s;
    cells = new Cell[g_s][g_s];
    GenerateCells(edgeX + s_p + cell_size/2,
                  edgeY - s_p - cell_size/2);
}

unsigned int Arena::GetGridSize() {
    return grid_size;
}

unsigned float Arena::GetArenaSize() {
    return arena_size;
}

Cell& Arena::GetCellAt(int i, int j) {
    return cells[i][j];
}